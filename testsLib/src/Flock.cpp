/// @file Flock.cpp
/// @brief Library for creating a flock which contains boids with their attributes and sets of flocking behaviour rules
/// @brief the idea and initial code is from my first year assignment DustyParticles (https://github.com/Anastasija3793/DustyParticles)
/// @author Anastasija Belaka
/// @author of BBoxCollision function (slightly modified) belongs to Jon Macey (https://github.com/NCCA/Collisions/tree/master/BoundingBox)
/// @version 15.0
/// @date 19/03/2018 Updated to NCCA Coding standard
/// Revision History : https://github.com/Anastasija3793/FlockingSystem
/// Initial Version 05/03/2018

#include "Flock.h"
#include <ngl/Random.h>

Flock::Flock(ngl::Vec3 _pos, ngl::Vec3 _target, int _numBoids)
{
    ngl::Random *rand=ngl::Random::instance();
    auto randomVelocity = rand->getRandomVec3();
    for (int i=0; i< _numBoids; ++i)
    {
    m_boids.push_back(Boid(_pos,randomVelocity,_target,this));
    randomVelocity = rand->getRandomVec3();
    }
    m_numBoids=_numBoids;
}
//----------------------------------------------------------------------------------------------------------------------
Flock::~Flock()
{
    //dctor
}
//----------------------------------------------------------------------------------------------------------------------
void Flock::resetBBox()
{
    m_bbox.reset( new ngl::BBox(ngl::Vec3(),12.0f,12.0f,12.0f));
}
//----------------------------------------------------------------------------------------------------------------------
std::vector<Boid*> Flock::getNeighbours(int j)
{
    std::vector<Boid*> neighbours;
    auto& thisBoid = m_boids[j];

    for(int i=0; i<m_numBoids; ++i)
    {
        if (i == j) continue;

        auto dir = thisBoid.m_pos - m_boids[i].m_pos;
        // 0.5 is a radius of neighbourhood
        if (dir.length() < 0.5f)
        {
            neighbours.push_back(&m_boids[i]);
        }
    }
    return neighbours;
}
//----------------------------------------------------------------------------------------------------------------------
/// @brief neighbours for separation (make a radius of "neighbourhood" smaller)
//----------------------------------------------------------------------------------------------------------------------
std::vector<Boid*> Flock::getNeighboursSep(int y)
{
    std::vector<Boid*> neighboursSep;
    auto& thisBoidSep = m_boids[y];

    for(int i=0; i<m_numBoids; ++i)
    {
        if (i == y) continue;

        auto dirSep = thisBoidSep.m_pos - m_boids[i].m_pos;
        // 0.3 is a radius of smaller neighbourhood
        if (dirSep.length() < 0.3f)
        {
            neighboursSep.push_back(&m_boids[i]);
        }
    }
    return neighboursSep;
}
//----------------------------------------------------------------------------------------------------------------------
/// @brief a method to update each of the boids contained in the flocking system
//----------------------------------------------------------------------------------------------------------------------
void Flock::update()
{
    for(int i=0; i<m_numBoids; ++i)
    {
        auto neighbours = getNeighbours(i);
        m_boids[i].setNeighbours(neighbours);

        auto neighboursSep = getNeighboursSep(i);
        m_boids[i].setNeighboursSep(neighboursSep);

        m_boids[i].update();
    }
}
//----------------------------------------------------------------------------------------------------------------------
/// @brief a method to draw all the particles contained in the system
//----------------------------------------------------------------------------------------------------------------------
void Flock::draw(const ngl::Mat4 &_globalMouseTx)
{
    for(int i=0; i<m_numBoids; ++i)
    {
        m_boids[i].draw(_globalMouseTx);
    }
}
//----------------------------------------------------------------------------------------------------------------------
void Flock::normal()
{
    for(int i=0; i<m_numBoids; ++i)
    {
        m_boids[i].normal();
    }
}
//----------------------------------------------------------------------------------------------------------------------
void Flock::steer()
{
    for(int i=0; i<m_numBoids; ++i)
    {
        m_boids[i].steer();
    }
}
//----------------------------------------------------------------------------------------------------------------------
void Flock::alignment()
{
    for(int i=0; i<m_numBoids; ++i)
    {
        m_boids[i].align();
    }
}
//----------------------------------------------------------------------------------------------------------------------
void Flock::separation()
{
    for(int i=0; i<m_numBoids; ++i)
    {
        m_boids[i].separate();
    }
}
//----------------------------------------------------------------------------------------------------------------------
void Flock::cohesion()
{
    for(int i=0; i<m_numBoids; ++i)
    {
        m_boids[i].centre();
    }
}
//----------------------------------------------------------------------------------------------------------------------
void Flock::wandering()
{
    for(int i=0; i<m_numBoids; ++i)
    {
        m_boids[i].wander();
    }
}
//----------------------------------------------------------------------------------------------------------------------
void Flock::addBoid(ngl::Vec3 _randomPos)
{
    ngl::Random *rand=ngl::Random::instance();
    auto randomVel = rand->getRandomVec3();
    auto randomTar = rand->getRandomVec3();
    m_boids.push_back(Boid(_randomPos,randomVel,randomTar,this));

    ++m_numBoids;
}
//----------------------------------------------------------------------------------------------------------------------
void Flock::removeBoid()
{
    if(--m_numBoids == 0)
    {
        m_numBoids = 1;
    }
    else
    {
        m_boids.pop_back();
    }
}
//----------------------------------------------------------------------------------------------------------------------
void Flock::BBoxCollision()
{
      //create an array of the extents of the bounding box
      float ext[6];
      ext[0]=ext[1]=(m_bbox->height()/2.0f);
      ext[2]=ext[3]=(m_bbox->width()/2.0f);
      ext[4]=ext[5]=(m_bbox->depth()/2.0f);
      // Dot product needs a Vector so we convert The Point Temp into a Vector so we can
      // do a dot product on it
      ngl::Vec3 newP;
      // D is the distance of the Agent from the Plane. If it is less than ext[i] then there is
      // no collision
      GLfloat dist;
      // Loop for each sphere in the vector list
      for(Boid &b : m_boids)
      {
        newP=b.getPos();
        //Now we need to check the Sphere agains all 6 planes of the BBOx
        //If a collision is found we change the dir of the Sphere then Break
        for(int i=0; i<6; ++i)
        {
          //to calculate the distance we take the dotporduct of the Plane Normal
          //with the new point P
          dist=m_bbox->getNormalArray()[i].dot(newP);
          //Now Add the Radius of the sphere to the offsett
          dist+=b.getRadius();
          // If this is greater or equal to the BBox extent /2 then there is a collision
          //So we calculate the Spheres new direction
          if(dist >=ext[i])
          {
            //We use the same calculation as in raytracing to determine the
            // the new direction
            GLfloat x= 2*( b.getVel().dot((m_bbox->getNormalArray()[i])));
            ngl::Vec3 d =m_bbox->getNormalArray()[i]*x;
            b.setVel(b.getVel()-d);
            b.setHit();
            //180 to make it rotate "inside"
            b.m_angle+=180;
          }//end of hit test
         }//end of each face test
        }//end of for
}
//----------------------------------------------------------------------------------------------------------------------
