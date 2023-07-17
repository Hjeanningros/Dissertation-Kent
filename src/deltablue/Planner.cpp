#include "Planner.h"

using namespace std;

namespace deltablue {
    
    Planner::Planner() {
        _currentMark = 1;
    }

    void Planner::incrementalAdd(shared_ptr<AbstractConstraint> c) {
        int mark = newMark();
        shared_ptr<AbstractConstraint> overridden = c->satisfy(mark, shared_ptr<Planner>());

        while (overridden != nullptr) {
            overridden = overridden->satisfy(mark, shared_ptr<Planner>());
        }
    }

    void incrementalRemove(shared_ptr<AbstractConstraint> c) {
        shared_ptr<Variable> out = c->getOutput();
        c->markUnsatisfied();
        c->removeFromGraph();

        vector<shared_ptr<AbstractConstraint>> unsatisfied = removePropagateFrom(out);
        for (shared_ptr<AbstractConstraint> u : unsatisfied) {
            incrementalAdd(u);
        }
    }

    shared_ptr<Plan> Planner::extractPlanFromConstraints(vector<shared_ptr<AbstractConstraint>> constraints) {
        vector<shared_ptr<AbstractConstraint>> sources;

        for (shared_ptr<AbstractConstraint> c : constraints) {
            if (c->isInput() && c->isSatisfied()) {
                sources.push_back(c);
            }
        }

        return makePlan(sources);
    }

    shared_ptr<Plan> Planner::makePlan(vector<shared_ptr<AbstractConstraint>> sources) {
        int mark = newMark();
        shared_ptr<Plan> plan;
        vector<shared_ptr<AbstractConstraint>> todo = sources;

        while (!todo.empty()) {
            shared_ptr<AbstractConstraint> c = todo.front();
            todo.erase(todo.begin());

            if (c->getOutput()->getMark() != mark && c->inputsKnown(mark)) {
                plan->push_back(c);
                c->getOutput()->setMark(mark);
                addConstraintsConsumingTo(c->getOutput(), todo);
            }
        }

        return plan;
    }

    void Planner::propagateFrom(shared_ptr<Variable> v) {
        vector<shared_ptr<AbstractConstraint>> todo;
        addConstraintsConsumingTo(v, todo);

        while (!todo.empty()) {
            shared_ptr<AbstractConstraint> c = todo.front();
            todo.erase(todo.begin());
            c->execute();
            addConstraintsConsumingTo(c->getOutput(), todo);
        }
    }

    void Planner::addConstraintsConsumingTo(shared_ptr<Variable> v, vector<shared_ptr<AbstractConstraint>> coll) {
        shared_ptr<AbstractConstraint> determiningC = v->getDeterminedBy();

        for (shared_ptr<AbstractConstraint> c : v->getConstraints()) {
            if (c != determiningC && c->isSatisfied()) {
                coll.push_back(c);
            }
        }
    }

    bool Planner::addPropagate(shared_ptr<AbstractConstraint> c, int mark) {
        vector<shared_ptr<AbstractConstraint>> todo;
        todo.push_back(c);

        while (!todo.empty()) {
            shared_ptr<AbstractConstraint> d = todo.front();
            todo.erase(todo.begin());

            if (d->getOutput()->getMark() == mark) {
                incrementalRemove(c);
                return false;
            }

            d->recalculate();
            addConstraintsConsumingTo(d->getOutput(), todo);
        }

        return true;
    }

    void Planner::change(shared_ptr<Variable> var, int newValue) {
        shared_ptr<EditConstraint> editC = make_shared<EditConstraint>(var, Strength::PREFERRED, make_shared<Planner>());

        vector<shared_ptr<AbstractConstraint>> editV = {editC};
        shared_ptr<Plan> plan = extractPlanFromConstraints(editV);
        for (int i = 0; i < 10; i++) {
            var->setValue(newValue);
            plan->execute();
        }

        editC->destroyConstraint(make_shared<Planner>());
    }

    void Planner::constraintsConsuming(shared_ptr<Variable> v, function<void(shared_ptr<AbstractConstraint>)> fn) {
        shared_ptr<AbstractConstraint> determiningC = v->getDeterminedBy();

        for (shared_ptr<AbstractConstraint> c : v->getConstraints()) {
            if (c != determiningC && c->isSatisfied()) {
                fn(c);
            }
        }
    }

    int newMark() {
        return ++_currentMark;
    }

    vector<shared_ptr<AbstractConstraint>> Planner::removePropagateFrom(shared_ptr<Variable> out) {
        vector<shared_ptr<AbstractConstraint>> unsatisfied;
        out->setDeterminedBy(nullptr);
        out->setWalkStrength(Strength::absoluteWeakest());
        out->setStay(true);

        vector<shared_ptr<Variable>> todo = {out};

        while (!todo.empty()) {
            shared_ptr<Variable> v = todo.front();
            todo.erase(todo.begin());

            for (shared_ptr<AbstractConstraint> c : v->getConstraints()) {
                if (!c->isSatisfied()) {
                    unsatisfied.push_back(c);
                }
            }   

            constraintsConsuming(v, [&todo](shared_ptr<AbstractConstraint> c) -> void {
                c->recalculate();
                todo.push_back(c->getOutput());
            });
        }

        sort(unsatisfied.begin(), unsatisfied.end(), [&](shared_ptr<AbstractConstraint> c1, shared_ptr<AbstractConstraint> c2) {
            return c1->getStrength().stronger(c2->getStrength());
        });

        return unsatisfied;
    }

    void Planner::chainTest(int n) {
        shared_ptr<Planner> planner;
        vector<shared_ptr<Variable>> vars(n + 1);
        generate(vars.begin(), vars.end(), []() { return new Variable(); });

        for (int i = 0; i < n; i++) {
            shared_ptr<Variable> v1 = vars[i];
            shared_ptr<Variable> v2 = vars[i + 1];
            make_shared<EqualityConstraint>(v1, v2, Strength::REQUIRED, planner);
        }

        make_shared<StayConstraint>(vars[n], Strength::STRONG_DEFAULT, planner);
        shared_ptr<AbstractConstraint> editC = make_shared<EditConstraint>(vars[0], Strength::PREFERRED, planner);

        vector<shared_ptr<AbstractConstraint>> editV = {editC};
        shared_ptr<Plan> plan = planner->extractPlanFromConstraints(editV);
        for (int i = 0; i < 100; i++) {
            vars[0]->setValue(i);
            plan->execute();
            if (vars[n]->getValue() != i) {
                throw Error("Chain test failed!");
            }
        }

        editC->destroyConstraint(planner);
    }

    void Planner::projectionTest(int n) {
        shared_ptr<Planner> planner;
        vector<shared_ptr<Variable>> dests;

        shared_ptr<Variable> scale = Variable::value(10);
        shared_ptr<Variable> offset = Variable::value(1000);

        shared_ptr<Variable> src = nullptr;
        shared_ptr<Variable> dst = nullptr;
        for (int i = 1; i <= n; i++) {
            src = Variable::value(i);
            dst = Variable::value(i);
            dests.push_back(dst);
            make_shared<StayConstraint>(src, Strength::DEFAULT, planner);
            make_shared<ScaleConstraint>(src, scale, offset, dst, Strength::REQUIRED, planner);
        }

        planner->change(src, 17);
        if (dst->getValue() != 1170) {
            throw Error("Projection test 1 failed!");
        }

        planner->change(dst, 1050);
        if (src->getValue() != 5) {
            throw Error("Projection test 2 failed!");
            
        }

        planner->change(scale, 5);
        for (int i = 0; i < n - 1; ++i) {
            if (dests[i]->getValue() != (i + 1) * 5 + 1000) {
                throw Error("Projection test 3 failed!");
            }
        }

        planner->change(offset, 2000);
        for (int i = 0; i < n - 1; ++i) {
            if (dests[i]->getValue() != (i + 1) * 5 + 2000) {
                throw Error("Projection test 4 failed!");
            }
        }
    }
}
