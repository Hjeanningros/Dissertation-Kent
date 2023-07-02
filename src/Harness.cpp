#include "Run.cpp"
#include <string>
#include <vector>

class Harness {
    public:

        static Run processArguments(char** args, int argc) {
            Run run = Run(args[1]);

            if (argc > 2) {
                run.setNumIterations(std::atoi(args[2]));
                if (argc > 3) {
                    run.setInnerIterations(std::atoi(args[3]));
                }
            }
            return run;
        }

        static void printUsage() {
            std::cout << "Harness [benchmark] [num-iterations [inner-iter]]" << std::endl << std::endl;
            std::cout << "  benchmark      - benchmark class name " << std::endl;
            std::cout << "  num-iterations - number of times to execute benchmark, default: 1" << std::endl;
            std::cout << "  inner-iter     - number of times the benchmark is executed in an inner loop, " << std::endl;
            std::cout << "                   which is measured in total, default: 1" << std::endl;
        }


        ~Harness() {
        }
};

int main(int argc, char** argv) {
    Harness harness = Harness();
    if (argc < 2) {
        harness.printUsage();
        return 1;
    }

    Run run = harness.processArguments(argv, argc);
    run.runBenchmark();
    run.printTotal();
}
