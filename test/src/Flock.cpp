#include "Flock.h"

Flock::Flock(int _numBoids)
{
    for (int i=0; i< _numBoids; ++i)
    {
    m_boids.push_back(Boid(this));
    }
    m_numBoids=_numBoids;
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
