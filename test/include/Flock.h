#ifndef FLOCK_H
#define FLOCK_H

#include <vector>
#include <ngl/Camera.h>
#include <ngl/Vec3.h>

#include "Boid.h"

class Flock
{
public:
    Flock(ngl::Vec3 _pos, int _numBoids); //something else is missing
    ~Flock(); //is Flock need dctor?

    void update();
    void draw(const ngl::Mat4 &_globalMouseTx);

    //for camera and shader
    inline void setCam(ngl::Camera *_cam){m_cam=_cam;}
    inline ngl::Camera * getCam()const {return m_cam;}
    inline void setShaderName(const std::string &_n){m_shaderName=_n;}
    inline const std::string getShaderName()const {return m_shaderName;}

    void BBoxCollision();
    std::unique_ptr<ngl::BBox> m_bbox;

    void alignment();
    void separation();
    void cohesion();

    std::vector<Boid*> getNeighbours(int i);
    std::vector<Boid*> getNeighboursSep(int y);

    void addBoid();
    void removeBoid();
private :
      // @brief the position of the emitter
      //ngl::Vec3 m_pos;
      //ngl::Vec3 m_dir;
      //float max_speed;

      /// @brief the number of boids
      int m_numBoids;
      /// @brief the container for the particles
      std::vector<Boid>m_boids;

    /// @brief the name of the shader to use
    std::string m_shaderName;
    /// @brief a pointer to the camera used for drawing
    ngl::Camera *m_cam;

};



#endif // FLOCK_H
