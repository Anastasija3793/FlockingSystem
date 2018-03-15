#include "Boid.h"

#include <ngl/Camera.h>
#include <ngl/Random.h>
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <math.h>

#include "Flock.h"

Boid::Boid(ngl::Vec3 _pos, Flock *_flock)
{
    //m_gravity=-9;//4.65;
    m_pos=_pos;
    //_pos=m_pos;
    m_angle=360;
    m_acc.set(0,0,0);
    //m_target.set(-2,-2,-0.1);
    //m_target.set(-30,30,0);

    max_speed = 4;
    max_force = 0.03; //0.1

    //random velocity & position
    ngl::Random *rand=ngl::Random::instance();

    m_radius=rand->randomPositiveNumber(2)+0.5f;
        //m_forward.normalize();
    //random position
    m_pos=rand->getRandomVec3();
        //m_vel=rand->getRandomNormalizedVec3();
    m_vel=rand->getRandomVec3();
    m_vel *=0.05;
    m_target=rand->getRandomVec3();

    m_flock=_flock;
    //m_acc*=0;
    m_hit=false;
}

Boid::Boid()
{
    m_hit=false;
}

void Boid::applyForce(ngl::Vec3 force)
{
    m_acc+=force;
}

void Boid::normal()
{
    m_pos+=m_vel;
}

void Boid::steer()
{
    ngl::Random *rand=ngl::Random::instance();
    m_target=rand->getRandomVec3();
    //m_target.set(-3,10,0);

    m_desired = m_target - m_pos;
    m_desired.normalize();

    m_desired*= max_speed;
    m_steer = m_desired - m_vel;
    m_steer.normalize();

    // if statement for limiting steering by max force
    auto NormS = m_steer;
    auto speed = m_steer.length();
    if (speed > max_force)
    {
        NormS.normalize();
        m_steer = NormS * max_force;
    }
    //m_acc+=m_steer;
    applyForce(m_steer);
}

void Boid::update()
{
    m_vel+=m_acc;

    // if statement for limiting velocity by max speed
    auto NormV = m_vel;
    auto speed = m_vel.length();
    if (speed > max_speed)
    {
        NormV.normalize();
        m_vel = NormV * max_speed;
    }

    m_pos+=m_vel;
    m_acc*=0;
}

void Boid::align()
{
    for(int i=0; i<m_neighbours.size(); ++i)
    {
        m_vel = m_neighbours[i]->m_vel;
        //m_desired = m_neighbours[i]->m_desired;
        //max_speed = m_neighbours[i]->max_speed;
        //max_force = m_neighbours[i]->max_force;
        //m_acc = m_neighbours[i]->m_acc*2;
        //m_acc = 0.002;
        //m_steer = m_neighbours[i]->m_steer;
    }
}

//for separation - other neighbours' settings (smaller "neighbourhood" radius)
void Boid::separate()
{
    for(int i=0; i<m_neighboursSep.size(); ++i)
    {
        m_vel = m_neighboursSep[i]->m_vel*(-1);
    }
}

void Boid::centre()
{
    for(int i=0; i<m_neighbours.size(); ++i)
    {
        m_pos = m_neighbours[i]->m_pos;
    }
}

void Boid::goal()
{
    //need to fix
    //maybe apply to random sphere?
    m_target.set(-30,30,0);
    //m_target.set(sphere&);
}

void Boid::wander()
{
//    float r = (rand->randomNumber(2)+0.5f * 2 * M_PI);
//    m_steer.set(cos(r),sin(r),cos(r));

    ngl::Random *rand=ngl::Random::instance();
    float R = 3.0f;
    //ngl::Vec3 targ = m_vel - m_pos;
    //displacement
    ngl::Vec3 disp=rand->getRandomVec3();
    disp.normalize();
    disp*=R * 0.5f * 4 * M_PI; //2
    m_vel=(m_target + disp) - m_desired;
    m_vel*=0.01; //0.001
}

//draw function with shader and camera
//modified (start from jm)
void Boid::draw(const ngl::Mat4 &_globalMouseTx)
{
    // get the VBO instance and draw the built in teapot
    ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
    ngl::Transformation transform;
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    shader->use(m_flock->getShaderName());
    transform.setPosition(m_pos);
    //transform.setRotation(0,m_angle,m_angle);
    //transform.setScale(0.5,0.5,0.5);
    ngl::Mat4 MV;
    ngl::Mat4 MVP;
    ngl::Mat3 normalMatrix;
    ngl::Mat4 M;
    M=transform.getMatrix();
    MV=m_flock->getCam()->getViewMatrix()*_globalMouseTx*transform.getMatrix();
    //rotate towards direction
    MV.rotateX(m_angle); //180 to make it rotate "inside"
    MV.rotateY(m_angle);
    MV.rotateZ(m_angle);
    MV.scale(0.5,0.5,0.5);

    MVP=m_flock->getCam()->getProjectionMatrix() *MV;
    normalMatrix=MV;
    normalMatrix.inverse().transpose();
    shader->setUniform("MV",MV);
    shader->setUniform("MVP",MVP);
    shader->setUniform("normalMatrix",normalMatrix);
    shader->setUniform("M",M);
    prim->draw("cone");
}
