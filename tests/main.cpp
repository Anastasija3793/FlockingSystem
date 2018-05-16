/// @file main.cpp
/// @brief Tests for flocking system libraries
/// @author Anastasija Belaka
/// @version 3.0
/// @date 19/03/2018 Updated to NCCA Coding standard
/// Revision History : https://github.com/Anastasija3793/FlockingSystem
/// Initial Version 05/03/2018

#include <gtest/gtest.h>
#include "Boid.h"
#include "Flock.h"
#include <ngl/VAOPrimitives.h>

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
//----------------------------------------------------------------------------------------------------------------------
/// @brief testing normal/default behaviour
//----------------------------------------------------------------------------------------------------------------------
TEST(Boid, normal)
{
    auto startPos = ngl::Vec3(0.f,0.f,0.f);
    auto startVel = ngl::Vec3(1.f,0.f,0.f);
    auto startTarget = ngl::Vec3(0.f,0.f,0.f);
    Boid b(startPos, startVel, startTarget, 0);
    auto vel = ngl::Vec3(0.f, 1.0f, 0.f);
    b.setVel(vel);
    auto expected = startPos + vel;
    b.normal();
    EXPECT_TRUE(b.m_pos == expected);
}
//----------------------------------------------------------------------------------------------------------------------
/// @brief testing alignment rule
//----------------------------------------------------------------------------------------------------------------------
TEST(Boid, alignment)
{

    auto startPos = ngl::Vec3(0.f,0.f,0.f);
    auto startVel = ngl::Vec3(1.f,0.f,0.f);
    auto startTarget = ngl::Vec3(0.f,0.f,0.f);
    Boid b(startPos, startVel, startTarget, 0);
    auto vel = ngl::Vec3(0.f, 1.0f, 0.f);
    b.setVel(vel);

    b.align();
    std::vector<Boid*> near = b.m_neighbours;
    for(int i=0; i<near.size(); ++i)
    {
        auto nearVel = near[i]->m_vel;
        EXPECT_TRUE(b.m_vel == nearVel);
    }
}
//----------------------------------------------------------------------------------------------------------------------
/// @brief testing separation rule
//----------------------------------------------------------------------------------------------------------------------
TEST(Boid, separation)
{
    auto startPos = ngl::Vec3(0.f,0.f,0.f);
    auto startVel = ngl::Vec3(1.f,0.f,0.f);
    auto startTarget = ngl::Vec3(0.f,0.f,0.f);
    Boid b(startPos, startVel, startTarget, 0);
    auto vel = ngl::Vec3(0.f, 1.0f, 0.f);
    b.setVel(vel);

    b.separate();
    std::vector<Boid*> near = b.m_neighboursSep;
    for(int i=0; i<near.size(); ++i)
    {
        auto nearVel = near[i]->m_vel*(-1);
        EXPECT_TRUE(b.m_vel == nearVel);
    }
}
//----------------------------------------------------------------------------------------------------------------------
/// @brief testing cohesion rule
//----------------------------------------------------------------------------------------------------------------------
TEST(Boid, cohesion)
{
    auto startPos = ngl::Vec3(0.f,0.f,0.f);
    auto startVel = ngl::Vec3(1.f,0.f,0.f);
    auto startTarget = ngl::Vec3(0.f,0.f,0.f);
    Boid b(startPos, startVel, startTarget, 0);

    b.centre();
    std::vector<Boid*> near = b.m_neighbours;
    for(int i=0; i<near.size(); ++i)
    {
        auto nearPos = near[i]->m_pos;
        EXPECT_TRUE(b.m_pos == nearPos);
    }
}
//----------------------------------------------------------------------------------------------------------------------
/// @brief testing wandering behaviour
//----------------------------------------------------------------------------------------------------------------------
TEST(Boid, wander)
{
    auto startPos = ngl::Vec3(0.f,0.f,0.f);
    auto startVel = ngl::Vec3(1.f,0.f,0.f);
    auto startTarget = ngl::Vec3(0.f,0.f,0.f);
    Boid b(startPos, startVel, startTarget, 0);

    auto vel = ngl::Vec3(0.f, 0.0f, 0.f);

    b.setVel(vel);
    b.wander();
    EXPECT_TRUE(b.m_vel != vel);
}
//----------------------------------------------------------------------------------------------------------------------
/// @brief testing remove a boid
//----------------------------------------------------------------------------------------------------------------------
TEST(Flock, removeBoid)
{
    auto pos = ngl::Vec3(0.f,0.f,0.f);
    auto tar = ngl::Vec3(0.f,0.f,0.f);
    int num=200;
    Flock f(pos, tar, num);

    f.removeBoid();
    EXPECT_TRUE(f.m_numBoids == (num-1));
}
//----------------------------------------------------------------------------------------------------------------------
/// @brief testing add a boid
//----------------------------------------------------------------------------------------------------------------------
TEST(Flock, addBoid)
{
    auto pos = ngl::Vec3(0.f,0.f,0.f);
    auto tar = ngl::Vec3(0.f,0.f,0.f);
    int num=200;
    Flock f(pos, tar, num);

    f.addBoid(pos);
    EXPECT_TRUE(f.m_numBoids == (num+1));
}
//----------------------------------------------------------------------------------------------------------------------
