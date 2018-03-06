#ifndef BOID_H
#define BOID_H

#include <ngl/Vec3.h>
#include <ngl/Colour.h>

class Flock;

class Boid
{
public:
    Boid(ngl::Vec3 _pos, Flock *_flock);//ngl::Vec3 _vel);
    //~Boid();

    void update();
    void draw();

private:
    //position
    ngl::Vec3 m_pos;
    //velocity
    ngl::Vec3 m_vel;

    ngl::Vec3 max_vel;

    ngl::Vec3 desired_vel;
    ngl::Vec3 m_steer;
    //direction
    ngl::Vec3 m_dir;
    //acceleration
    ngl::Vec3 m_acc;

    GLfloat m_gravity;

    ngl::Vec3 m_target;

    float m_thetaX;
    float m_thetaY;
    float m_thetaZ;
    float m_angle;
    float m_angleX;
    float m_angleY;
    float m_angleZ;
    ngl::Vec3 m_rotation;

    const Flock *m_flock; //const?
};



#endif // BOID_H
