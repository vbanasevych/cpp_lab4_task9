#pragma once
#include <vector>
#include "data_structure.h"

void worker_thread(OptimizedDataStructure& ds, const std::vector<Command>& commands);
