#include "Context.hpp"

Context& Context::get()
{
    static Context self;
    return self;
}

Context::Context():
    wallbreaker(nullptr),
    particles(nullptr)
{
}
