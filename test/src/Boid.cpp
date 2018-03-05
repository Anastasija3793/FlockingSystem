#include "Boid.h"

#include <ngl/Camera.h>
#include <ngl/Random.h>
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>

#include "Flock.h"

//DO all stuff here (flock is just for boidNumbers AND camera)

Boid::Boid(ngl::Vec3 _pos, Flock *_flock)
{
    //do random position for each boid
    //m_pos=(0.0f,0.0f,0.0f);
    m_pos=_pos;

   // m_dir=_dir;
   //m_dir.m_y=rand->randomPositiveNumber(10)+0.5;

    //make slow velocity
    //m_vel=_vel;

    //random direction - look in the internet
    ngl::Random *rand=ngl::Random::instance();
//    m_dir.m_x=0.01;
//    m_dir.m_y=0.01;
//    m_dir.m_z=0.01;

    for(float k=-9.5;k<10; k+=1)
    {
        m_pos=rand->getRandomVec3();
        m_dir=rand->getRandomVec3();
        m_dir *=0.05;
        //m_scene->addParticle(emitterPos, dir, rng->getRandomColour(), 0.3); // emitterPos, initialSpeed, colour, radius,
    }



//      m_dir.m_x=rand->randomNumber(5)+0.5;
//      m_dir.m_y=rand->randomNumber(5)+0.5;
//      m_dir.m_z=rand->randomNumber(5)+0.5;

//      m_gravity=-9;//4.65;

    //acceleration? maybe need as a separate function
    //m_acc=_acc;

    m_flock=_flock;
}

//Boid::~Boid()
//{
//dctor
//}

void Boid::update()
{
    m_pos+=m_dir;//m_vel?
}


//draw function with shader and camera
//from jm
void Boid::draw()
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
    MV=m_flock->getCam()->getViewMatrix()*transform.getMatrix();

    MVP=m_flock->getCam()->getProjectionMatrix() *MV;
    normalMatrix=MV;
    normalMatrix.inverse().transpose();
    shader->setUniform("MV",MV);
    shader->setUniform("MVP",MVP);
    shader->setUniform("normalMatrix",normalMatrix);
    shader->setUniform("M",M);
    prim->draw("cone");
}
