#ifndef BOIDBEHAVIOUR_H
#define BOIDBEHAVIOUR_H

#include <ngl/Vec3.h>
#include <ngl/Colour.h>

//class Boid;

class BoidBehaviour
{
public:

    // draw Object (sphere)
    void addObject();
    // function for boids to avoid Object
    void avoidObject();
    // move towards Object
    void seekObject();

    //void wander();

private:
    // position of added Object
    ngl::Vec3 m_posObj;

    //need to pass position and velocity of boids
    //const Boid *m_boids;   OR
    //const Flock *m_flock;

};

#endif // BOIDBEHAVIOUR_H
