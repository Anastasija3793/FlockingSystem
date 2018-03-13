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
    //do random position for each boid
    m_pos=_pos;
    //_pos=m_pos;
    m_angle=360;
    m_acc.set(0,0,0);
    //m_target.set(-2,-2,-0.1);
    //m_target.set(-30,30,0);

    max_speed = 4;
    max_force = 0.1;
    //max_speed = 0.05;
    //max_force = 20;

    //random direction - look in the internet
    ngl::Random *rand=ngl::Random::instance();

    //for(float k=-9.5;k<10; k+=1)
    //{
    m_radius=rand->randomPositiveNumber(2)+0.5f;
        //m_forward.normalize();
        m_pos=rand->getRandomVec3();
        //m_pos.set(-10,0,0);
        //m_vel.set(0,0,0); //-0.5
        //m_vel=rand->getRandomNormalizedVec3();
        m_vel=rand->getRandomVec3();
        m_vel *=0.05;
        m_target=rand->getRandomVec3();
    //}

    //angle to face direction
    //m_angle=ngl::degrees(acos(m_vel.dot(m_forward)/(m_vel.length()*m_forward.length())));

    m_desired = m_target - m_pos;
    m_desired.normalize();
//    auto d = m_desired;
//    d.normalize();


    m_desired*= max_speed;
    m_steer = m_desired - m_vel;
//    auto copySteer = m_steer;
//    copySteer.normalize();
    m_steer.normalize();

    // if statement for limiting steering by max force
    auto NormS = m_steer;
    auto speed = m_steer.length();
    if (speed > max_force)
    {
        NormS.normalize();
        m_steer = NormS * max_force;
    }


    m_acc+=m_steer;
    m_flock=_flock;

    m_hit=false;
}

Boid::Boid()
{
    m_hit=false;
}

void Boid::update()
{
    //m_forward=m_vel;
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

    //std::cout<<"Angle= "<<m_angle;
    //std::cout<<"ForwardX= "<<m_forward.m_x;
}

void Boid::align()
{
    ngl::Vec3 sum(0,0,0);
    //float neighbourDist=30.f;
    //max_speed=4;
    int count=0;

    //for(Boid &n : m_neighbours)
    for(int i=0; i<m_neighbours.size(); ++i)
    {
//        //distance between two boids
//        auto dist = m_pos - (m_neighbours[i]->m_pos);
//        dist.normalize();
//        if(dist.length()>0 && dist.length()<neighbourDist)
//        {
//            sum+=m_vel;
//            ++count;
//        }

//    if(count>0)
//    {
//        sum/=count;
//        sum.normalize();
//        sum*=max_speed;
//        //subdivide?
//       // m_boids[i].m_steer=average(sum,m_boids[i].m_vel);
//        m_steer= sum - m_vel;

//        //m_steer.limit by (maxforce)
//        //apply force = steer (acc to steer)
//    }
        m_vel = m_neighbours[i]->m_vel;
    //m_boids[i].update();
    }
}

void Boid::separate()
{
    for(int i=0; i<m_neighbours.size(); ++i)
    {
        m_vel = m_neighbours[i]->m_vel*(-1);
    }
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
