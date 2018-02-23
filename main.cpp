#include <nan.h>
#include "solver.hpp"

// NAN_METHOD is a Nan macro enabling convenient way of creating native node functions.
// It takes a method's name as a param. By C++ convention, I used the Capital cased name.
NAN_METHOD(GetNextMove) {
    Solver solver;
    Position P;
    auto score = solver.solve(P);
    info.GetReturnValue().Set(score);
}

// Module initialization logic
NAN_MODULE_INIT(Initialize) {
    NAN_EXPORT(target, GetNextMove);
}

// Create the module called "addon" and initialize it with `Initialize` function (created with NAN_MODULE_INIT macro)
NODE_MODULE(addon, Initialize);