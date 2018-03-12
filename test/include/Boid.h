#ifndef BOID_H
#define BOID_H

#include <ngl/Vec3.h>
#include <ngl/Mat4.h>
#include <ngl/Colour.h>

class Flock;

class Boid
{
public:
    Boid(ngl::Vec3 _pos, Flock *_flock);//ngl::Vec3 _vel);
    Boid();

    void update();
    void draw(const ngl::Mat4 &_globalMouseTx);

    inline void reverse(){m_vel=m_vel*-1.0;}
    inline void setHit(){m_hit=true;}
    inline void setNotHit(){m_hit=false;}
    inline bool isHit()const {return m_hit;}
  inline ngl::Vec3 getPos() const {return m_pos;}
  //inline ngl::Vec3 getNextPos() const {return m_nextPos;}
    inline GLfloat getRadius() const {return m_radius;}
  inline void setVel(ngl::Vec3 _v){m_vel=_v;}
  inline ngl::Vec3 getVel() const { return m_vel;}

  float m_angle;

private:
    //position
    ngl::Vec3 m_pos;
    //velocity
    ngl::Vec3 m_vel;

    ngl::Vec3 max_vel;

    ngl::Vec3 m_desired;
    ngl::Vec3 m_target;
    ngl::Vec3 m_steer;

    bool m_hit;
    GLfloat m_radius;

    //direction
    //ngl::Vec3 m_dir;
    //acceleration
    ngl::Vec3 m_acc;
    float max_speed;
    float max_force;

    //ngl::Vec3 m_forward {1,0,1};

    //GLfloat m_gravity;


    //ngl::Vec3 m_rotation;

    const Flock *m_flock; //const?
};



#endif // BOID_H
