#pragma once
#include <string>
#include <shared_mutex>
#include <format>
#include "config.h"

struct Command {
    std::string op; // "read", "write", "string"
    int field;
    int value;
};

struct OpWeights {
    int read0, write0;
    int read1, write1;
    int read2, write2;
    int string_op;
};

class OptimizedDataStructure {
public:
    OptimizedDataStructure();

    // заборона копіювання та переміщення
    OptimizedDataStructure(const OptimizedDataStructure&) = delete;
    OptimizedDataStructure& operator=(const OptimizedDataStructure&) = delete;

    void set(int index, int value);


    int get(int index);

    explicit operator std::string() const;

private:
    int fields[NUM_FIELDS];
    mutable std::shared_mutex mutexes[NUM_FIELDS];
};
