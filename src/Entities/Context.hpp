#ifndef CONTEXT_HPP
#define CONTEXT_HPP

class Wallbreaker;
class ParticleSystem;

struct Context
{
    static Context& get();

    Wallbreaker* wallbreaker;
    ParticleSystem* particles;

private:
    Context();
};

#endif
