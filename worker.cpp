#include "worker.h"
#include "data_structure.h"

void worker_thread(OptimizedDataStructure& ds, const std::vector<Command>& commands) {
    for (const auto& cmd : commands) {
        if (cmd.op == "read") {
            volatile int val = ds.get(cmd.field);
        } else if (cmd.op == "write") {
            ds.set(cmd.field, cmd.value);
        } else if (cmd.op == "string") {
            volatile std::string s = (std::string)ds;
        }
    }
}
