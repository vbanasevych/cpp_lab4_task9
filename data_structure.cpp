#include "data_structure.h"
#include <mutex>

OptimizedDataStructure::OptimizedDataStructure() : fields{0, 0, 0} {}

void OptimizedDataStructure::set(int index, int value) {
    if (index < 0 || index >= NUM_FIELDS) return;
    std::unique_lock lock(mutexes[index]);
    fields[index] = value;
}

int OptimizedDataStructure::get(int index) {
    if (index < 0 || index >= NUM_FIELDS) return 0;
    std::shared_lock lock(mutexes[index]);
    return fields[index];
}

OptimizedDataStructure::operator std::string() const {
    std::shared_lock lock0(mutexes[0], std::defer_lock);
    std::shared_lock lock1(mutexes[1], std::defer_lock);
    std::shared_lock lock2(mutexes[2], std::defer_lock);

    std::lock(lock0, lock1, lock2);

    return std::format("Field 0: {}, Field 1: {}, Field 2: {}", fields[0], fields[1], fields[2]);
}
