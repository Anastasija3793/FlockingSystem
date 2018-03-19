/// @file Boid.cpp
/// @brief Library for creating a boid with its attributes and sets of flocking behaviour rules
/// @brief the idea and initial code is from my first year assignment DustyParticles (https://github.com/Anastasija3793/DustyParticles)
/// @author Anastasija Belaka
/// @version 15.0
/// @date 19/03/2018 Updated to NCCA Coding standard
/// Revision History : https://github.com/Anastasija3793/FlockingSystem
/// Initial Version 05/03/2018

#include "Boid.h"
#include <ngl/Camera.h>
#include <ngl/Random.h>
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <math.h>
#include "Flock.h"

Boid::Boid(ngl::Vec3 _pos, ngl::Vec3 _vel, ngl::Vec3 _target, Flock *_flock)
{
    m_pos=_pos;
    m_vel=_vel;
    m_target=_target;
    m_angle=360;
    m_acc.set(0,0,0);

    max_speed = 4;
    max_force = 0.03;
    m_vel *=0.05;
    m_flock=_flock;
    m_hit=false;
}
//----------------------------------------------------------------------------------------------------------------------
void Boid::applyForce(ngl::Vec3 force)
{
    m_acc+=force;
}
//----------------------------------------------------------------------------------------------------------------------
void Boid::normal()
{
    m_pos+=m_vel;
}
//----------------------------------------------------------------------------------------------------------------------
void Boid::steer()
{
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
    // applying the force to the steering
    applyForce(m_steer);
}
//----------------------------------------------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------------------------------------------------
void Boid::align()
{
    for(int i=0; i<m_neighbours.size(); ++i)
    {
        m_vel = m_neighbours[i]->m_vel;
    }
}
//----------------------------------------------------------------------------------------------------------------------
/// @brief for separation - other neighbours' settings (smaller "neighbourhood" radius)
//----------------------------------------------------------------------------------------------------------------------
void Boid::separate()
{
    for(int i=0; i<m_neighboursSep.size(); ++i)
    {
        m_vel = m_neighboursSep[i]->m_vel*(-1);
    }
}
//----------------------------------------------------------------------------------------------------------------------
void Boid::centre()
{
    for(int i=0; i<m_neighbours.size(); ++i)
    {
        m_pos = m_neighbours[i]->m_pos;
    }
}
//----------------------------------------------------------------------------------------------------------------------
void Boid::wander()
{
    ngl::Random *rand=ngl::Random::instance();
    float R = 10.0f;
    //"displacement" for velocity
    ngl::Vec3 disp=rand->getRandomVec3();
    disp.normalize();
    disp*=R * 2.f * M_PI;
    m_vel=(m_target + disp) - m_pos;
    m_vel*=0.001;
}
//----------------------------------------------------------------------------------------------------------------------
/// @brief draw function with shader and camera
/// @brief modified (initial code from Jon Macey)
//----------------------------------------------------------------------------------------------------------------------
void Boid::draw(const ngl::Mat4 &_globalMouseTx)
{
    // get the VBO instance and draw the built in teapot
    ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
    ngl::Transformation transform;
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    shader->use(m_flock->getShaderName());
    transform.setPosition(m_pos);
    ngl::Mat4 MV;
    ngl::Mat4 MVP;
    ngl::Mat3 normalMatrix;
    ngl::Mat4 M;
    M=transform.getMatrix();
    MV=m_flock->getCam()->getViewMatrix()*_globalMouseTx*transform.getMatrix();
    //rotate towards direction
    MV.rotateX(m_angle);
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
//----------------------------------------------------------------------------------------------------------------------
