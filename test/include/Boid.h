#ifndef BOID_H
#define BOID_H

#include <ngl/Vec3.h>
#include <ngl/Colour.h>

class Flock;

class Boid
{
public:
    Boid(Flock *_flock);//ngl::Vec3 _vel);

    void update();
    void draw();

private:
    //position
    ngl::Vec3 m_pos;
    //velocity
    ngl::Vec3 m_vel;
    //direction
    ngl::Vec3 m_dir;
    //acceleration
    ngl::Vec3 m_acc;

    GLfloat m_gravity;

    const Flock *m_flock; //const?
};



#endif // BOID_H
