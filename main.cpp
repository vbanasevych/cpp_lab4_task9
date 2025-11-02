#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <map>
#include <iomanip>
#include <format>

#include "config.h"
#include "data_structure.h"
#include "file_utils.h"
#include "worker.h"

int main() {
    generate_all_op_files();

    std::vector<std::string> scenarios = {"scenario_a", "scenario_b", "scenario_c"};
    std::vector<int> thread_counts = {1, 2, 3};

    std::map<std::string, std::map<int, double>> results;

    std::cout << "\nScenarios' tests" << std::endl;

    for (const auto& scenario : scenarios) {
        std::cout << "Testing " << scenario << std::endl;
        for (int num_threads : thread_counts) {

            OptimizedDataStructure ds;
            std::vector<std::thread> threads;
            std::vector<std::vector<Command>> thread_commands(num_threads);

            for (int i = 0; i < num_threads; ++i) {
                thread_commands[i] = load_commands(std::format("{}_{}.txt", scenario, i));
            }

            auto start_time = std::chrono::high_resolution_clock::now();

            for (int i = 0; i < num_threads; ++i) {
                threads.emplace_back(worker_thread, std::ref(ds), std::cref(thread_commands[i]));
            }

            for (auto& t : threads) {
                t.join();
            }

            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end_time - start_time;

            std::cout << "  Threads: " << num_threads << ", Time: "
                      << std::fixed << std::setprecision(4) << elapsed.count() << " s" << std::endl;
            results[scenario][num_threads] = elapsed.count();
        }
    }

    std::cout << "\nResults Table (for report)" << std::endl;
    std::cout << std::left << std::setw(15) << "Time, s"
              << std::setw(10) << "1 thread"
              << std::setw(10) << "2 threads"
              << std::setw(10) << "3 threads" << std::endl;
    std::cout << std::string(45, '-') << std::endl;

    std::cout << std::fixed << std::setprecision(4);
    std::cout << std::left << std::setw(15) << "Scenario (a)"
              << std::setw(10) << results["scenario_a"][1]
              << std::setw(10) << results["scenario_a"][2]
              << std::setw(10) << results["scenario_a"][3] << std::endl;

    std::cout << std::left << std::setw(15) << "Scenario (b)"
              << std::setw(10) << results["scenario_b"][1]
              << std::setw(10) << results["scenario_b"][2]
              << std::setw(10) << results["scenario_b"][3] << std::endl;

    std::cout << std::left << std::setw(15) << "Scenario (c)"
              << std::setw(10) << results["scenario_c"][1]
              << std::setw(10) << results["scenario_c"][2]
              << std::setw(10) << results["scenario_c"][3] << std::endl;

    return 0;
}

