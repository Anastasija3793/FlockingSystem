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
    m_bbox.reset( new ngl::BBox(ngl::Vec3(),12.0f,12.0f,12.0f));
    //m_bbox->setDrawMode(GL_LINE);
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
void Flock::draw(const ngl::Mat4 &_globalMouseTx)
{
    for(int i=0; i<m_numBoids; ++i)
    {
        m_boids[i].draw(_globalMouseTx);
            //m_bbox->draw();
    }
    //m_bbox->draw();
}

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
          }//end of hit test
         }//end of each face test
        }//end of for
}
