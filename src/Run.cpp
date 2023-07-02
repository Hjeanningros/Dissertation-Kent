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
        std::shared_ptr<Benchmark> _benchmarkSuite;
        int _numIterations;
        int _innerIterations;
        long _total;

        static std::shared_ptr<Benchmark> getSuiteFromName(const std::string &name) {

            if (name == "Bounce")
                return std::make_shared<Bounce>();
            if (name == "Sieve")
                return std::make_shared<Sieve>();
            if (name == "List")
                return std::make_shared<List>();
            if (name == "Mandelbrot")
                return std::make_shared<Mandelbrot>();
            if (name == "Permute")
                return std::make_shared<Permute>();
            if (name == "Queens")
                return std::make_shared<Queens>();
            if (name == "Towers")
                return std::make_shared<Towers>();
            
            throw Error("No benchmark found with the name: " + name);
        }


        void measure(std::shared_ptr<Benchmark> bench) {
            auto startTime = std::chrono::high_resolution_clock::now();

            if(!bench->innerBenchmarkLoop(_innerIterations)) {
                throw Error("Benchmark fail with incorrect result");
            }
            
            auto endTime = std::chrono::high_resolution_clock::now();
            long runTime = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count() / 1000;

            printResult(runTime);
            _total += runTime;
        };

        void doRuns(std::shared_ptr<Benchmark> bench) {
            for (int i = 0; i < _numIterations; i++)
                measure(bench);
        };

        void reportBenchmark() {
            std::cout << _name << ": iterations=" << _numIterations << " average: " 
            << (_total / _numIterations) << "us total: " << _total << "us" << std::endl;
        }

        void printResult(long runTime) {
            std::cout << _name << ": iterations=1 runtime: " << runTime << "us" << std::endl;
        }
    
    public: 

        Run(const std::string& name) {
            _name = name;
            _benchmarkSuite = getSuiteFromName(name);
            _numIterations = 1;
            _innerIterations = 1;
            _total = 0;
        }

        void runBenchmark() {
            std::cout << "Starting " << _name << " benchmark ..." << std::endl;

            doRuns(_benchmarkSuite);
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