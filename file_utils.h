#pragma once
#include <string>
#include <vector>
#include "data_structure.h"

void generate_all_op_files();

std::vector<Command> load_commands(const std::string& filename);