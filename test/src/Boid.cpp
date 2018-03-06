#include "Boid.h"

#include <ngl/Camera.h>
#include <ngl/Random.h>
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <math.h>

#include "Flock.h"

//DO all stuff here (flock is just for boidNumbers AND camera)

Boid::Boid(ngl::Vec3 _pos, Flock *_flock)
{
    //ngl::Vec3 target(3,-3,-2);
    //do random position for each boid
    m_pos=_pos;

    max_vel=0.005;
    //m_vel=normalize(target - m_pos)*max_vel;

    //rotated in the direction of velocity
    //theta = @velocity.heading() + @p.radians(90);
    //rotate(theta);

    //random direction - look in the internet
    ngl::Random *rand=ngl::Random::instance();
//    m_dir.m_x=0.01; //m_dir.m_x=rand->randomNumber(5)+0.5;
//    m_dir.m_y=0.01; //m_dir.m_y=rand->randomNumber(5)+0.5;
//    m_dir.m_z=0.01; //m_dir.m_z=rand->randomNumber(5)+0.5;

    //for(float k=-9.5;k<10; k+=1)
    //{
        m_angle=10.0f;
        //m_angle=rand->randomNumber(5);
        m_pos=rand->getRandomVec3();
        m_vel=rand->getRandomNormalizedVec3();
        //m_vel=rand->getRandomVec3();
        m_vel *=0.05;

        //m_scene->addParticle(emitterPos, dir, rng->getRandomColour(), 0.3); // emitterPos, initialSpeed, colour, radius,
    //}


//      m_gravity=-9;//4.65;

    //acceleration? maybe need as a separate function
    //m_acc=_acc;






        //SEEK
   //m_vel=(target-m_pos)*max_vel;

//    m_vel=rand->getRandomVec3();
//    desired_vel=(target - m_pos) * max_vel;
//    m_steer = desired_vel - m_vel;
//        m_vel.m_x = sin((M_PI/180)*m_angle) * 0.0005 * 50;
//        m_vel.m_y = cos((M_PI/180)*m_angle)* 0.0005 * 50;
//        m_vel.m_z = cos((M_PI/180)*m_angle)* 0.0005 * 50;

    m_flock=_flock;
}

//Boid::~Boid()
//{
//dctor
//}

void Boid::update()
{
    double radians=M_PI / 180;
    m_thetaX=m_vel.m_x + radians*90;
    m_thetaY=m_vel.m_y + radians*90;
    m_thetaZ=m_vel.m_z + radians*90;
   // m_pos+=m_dir;//m_vel?
    //m_angle+=0.5f;
   // m_angleX=m_vel.m_x;
   // m_angleY=m_vel.m_y;
   // m_angleZ=m_vel.m_z;
    m_pos+=m_vel;

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
    //transform.setRotation(m_angleX,m_angleY,m_angleZ);
    transform.setRotation(m_thetaX,m_thetaY,m_thetaZ);
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
