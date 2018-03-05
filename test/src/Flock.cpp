#include "Flock.h"

Flock::Flock(ngl::Vec3 _pos, int _numBoids)
{
   // m_pos=_pos;
   // m_dir=_dir;
    for (int i=0; i< _numBoids; ++i)
    {
    m_boids.push_back(Boid(_pos,this));
    }
    m_numBoids=_numBoids;
}

Flock::~Flock()
{
    //dctor
}
/// @brief a method to update each of the particles contained in the system
void Flock::update()
{
    for(int i=0; i<m_numBoids; ++i)
    {
        m_boids[i].update();
    }
}
/// @brief a method to draw all the particles contained in the system
void Flock::draw()
{
    for(int i=0; i<m_numBoids; ++i)
    {
        m_boids[i].draw();
    }
}
