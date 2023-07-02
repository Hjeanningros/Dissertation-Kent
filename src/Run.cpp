#include "Bounce.cpp"
#include "Mandelbrot.cpp"
#include "Sieve.cpp"
#include "List.cpp"
#include "Permute.cpp"
#include "Queens.cpp"
#include "Towers.cpp"
#include <string>
#include <functional>
#include <memory>
#include <vector>
#include <iostream>
#include <chrono>
#include "Benchmark.cpp"


class Run{
    private: 
        std::string _name;
        std::function<std::shared_ptr<Benchmark>()> _benchmarkSuite;
        int _numIterations;
        int _innerIterations;
        long _total;

        static std::function<std::shared_ptr<Benchmark>()> getSuiteFromName(std::string name) {
            std::vector<std::string> benchmarkName = {"Bounce", "Sieve"};
            std::vector<std::function<std::shared_ptr<Benchmark>()>> benchmarkFunction;
            benchmarkFunction.push_back([]() {return std::shared_ptr<Bounce>(); });
            benchmarkFunction.push_back([]() {return std::shared_ptr<Sieve>(); });
            benchmarkFunction.push_back([]() {return std::shared_ptr<List>(); });
            benchmarkFunction.push_back([]() {return std::shared_ptr<Mandelbrot>(); });
            benchmarkFunction.push_back([]() {return std::shared_ptr<Permute>(); });
            benchmarkFunction.push_back([]() {return std::shared_ptr<Queens>(); });
            benchmarkFunction.push_back([]() {return std::shared_ptr<Towers>(); });

            for (int i = 0; i < (int)benchmarkName.size(); i++) { // delete cast
                if (benchmarkName[i] == name)
                    return benchmarkFunction[i];
            }
            
            throw Error("No benchmark found with the name: " + name);
        }


        void measure(std::shared_ptr<Benchmark> bench) {
            auto startTime = std::chrono::high_resolution_clock::now();
            if(!bench->innerBenchmarkLoop(_innerIterations)) {
                throw Error("Benchmark fail with incorrect result");
            }
            auto endTime = std::chrono::high_resolution_clock::now();
            long runTime = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count() / 1000;

            std::cout << runTime << std::endl;
            _total += runTime;
        };

        void doRuns(std::shared_ptr<Benchmark> bench) {
            for (int i = 0; i < _numIterations; i++)
                measure(bench);
        };

        void reportBenchmark() {
            std::cout << _name << ": iterations=" << _numIterations + " average: " + (_total / _numIterations) 
                << "us total: " << _total << "us"<< std::endl;
        }

        void printResult(long runTime) {
            std::cout << _name << ": iterations=1 runtime: " << runTime + "us" << std::endl;
        }
    
    public: 

        Run(const std::string& name) {
            _name = name;
            _benchmarkSuite = getSuiteFromName(name);
            _numIterations = 1;
            _innerIterations = 1;
        }

        void runBenchmark() {
            std::cout << "Starting " << _name << " benchmark ..." << std::endl;

            doRuns(_benchmarkSuite());
            reportBenchmark();

            std::cout << std::endl;
        }

        void printTotal() {
            std::cout << "Total Runtime: " << _total << "us" << std::endl;
        }

        void setNumIterations(int numIterations) {
            _numIterations = numIterations;
        }  

        void setInnerIterations(int innerIterations) {
            _innerIterations = innerIterations;
        }
};