#include "file_utils.h"
#include "config.h"
#include "data_structure.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <format>

void generate_one_file(const std::string& filename, const OpWeights& weights, int totalOps) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
        return;
    }

    std::vector<Command> weighted_ops;
    for (int i = 0; i < weights.read0; ++i) weighted_ops.push_back({"read", 0, 0});
    for (int i = 0; i < weights.write0; ++i) weighted_ops.push_back({"write", 0, 1});
    for (int i = 0; i < weights.read1; ++i) weighted_ops.push_back({"read", 1, 0});
    for (int i = 0; i < weights.write1; ++i) weighted_ops.push_back({"write", 1, 1});
    for (int i = 0; i < weights.read2; ++i) weighted_ops.push_back({"read", 2, 0});
    for (int i = 0; i < weights.write2; ++i) weighted_ops.push_back({"write", 2, 1});
    for (int i = 0; i < weights.string_op; ++i) weighted_ops.push_back({"string", 0, 0});

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, weighted_ops.size() - 1);

    for (int i = 0; i < totalOps; ++i) {
        const auto& cmd = weighted_ops[dis(gen)];
        if (cmd.op == "write") {
            outFile << cmd.op << " " << cmd.field << " " << cmd.value << "\n";
        } else if (cmd.op == "read") {
            outFile << cmd.op << " " << cmd.field << "\n";
        } else {
            outFile << cmd.op << "\n";
        }
    }
    outFile.close();
}

void generate_all_op_files() {
    std::cout << "Generating operation files (" << OPS_PER_FILE << " ops/file)..." << std::endl;

    OpWeights weights_a = {10, 10, 10, 10, 40, 5, 15};

    OpWeights weights_b = {14, 14, 14, 14, 14, 14, 16};

    OpWeights weights_c = {5, 5, 5, 5, 5, 5, 70};

    for (int i = 0; i < 3; ++i) {
        generate_one_file(std::format("scenario_a_{}.txt", i), weights_a, OPS_PER_FILE);
        generate_one_file(std::format("scenario_b_{}.txt", i), weights_b, OPS_PER_FILE);
        generate_one_file(std::format("scenario_c_{}.txt", i), weights_c, OPS_PER_FILE);
    }
    std::cout << "File generation complete." << std::endl;
}

std::vector<Command> load_commands(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error: Could not open file for reading: " << filename << std::endl;
        return {};
    }

    std::vector<Command> commands;
    std::string line;
    while (std::getline(inFile, line)) {
        std::stringstream ss(line);
        std::string op;
        ss >> op;
        if (op == "read") {
            int field;
            ss >> field;
            commands.push_back({op, field, 0});
        } else if (op == "write") {
            int field, value;
            ss >> field >> value;
            commands.push_back({op, field, value});
        } else if (op == "string") {
            commands.push_back({op, 0, 0});
        }
    }
    return commands;
}
