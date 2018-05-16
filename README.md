# FlockingSystem

Flocking System is a second year assignment for Programming Paradigms. The project contains folders with libraries, tests and demo showing the flocking system simulation using flocking behaviour rules. The project was created using NGL and QtNGL (demo).

## How to build
To build libraries go to testsLib folder -> open Flocking.pro project (with qtcreator) -> press build Flocking.pro project.

## How to use

In demo (or other project) create new flock :

```m_flock.reset(new Flock(pos, target, boidNum));```

Here user can declare (using ```ngl::Vec3```) :

1. Position of the flock - from where flock starts its journey;
2. Target for all boids - the point where they will steer (when the steering behaviour is called);
3. Number of boids - how many boids the flock will contain.

To draw and update the flock do :

```m_flock->draw(m_mouseGlobalTX);```

```m_flock->update();```

Flock library also contains BBox (for collision) which user needs first to reset and then call BBoxCollision function :

```m_bbox.reset( new ngl::BBox(ngl::Vec3(),12.0f,12.0f,12.0f));```

```m_bbox->draw();```

```m_flock->BBoxCollision();```

Boid library contains methods representing specific rules/behaviours for flocking simulation and user can call it :

1. ```m_flock->steer();``` - steering towards the target;
2. ```m_flock->alignment();``` - alignment rule;
3. ```m_flock->separation();``` - separation rule;
4. ```m_flock->cohesion();``` - cohesion rule;
5. ```m_flock->wandering();``` - boids are wandering around;
6. ```m_flock->normal();``` - normal/default movement of boids (no rules applied).

Some extras methods (in Flock library) :
- ```m_flock->addBoid(randPos);``` - adding a new boid to the flock with random position (user can change the position by declaring a new position);
- ```m_flock->removeBoid();``` - removing a boid from the flock (can do it until there is only one boid left).

If user wants to use the existed demo and call any of rules/behaviours - simply tick or untick the check boxes in QtNGL window.

## Folders

### Tests

Tests folder contains tests for Flocking System library.

Tests are written mostly for Boid library and a couple of tests are for Flock library. 

### TestsLib

TestsLib folder contains the Flocking System library (Boid and Flock classes). All rules/behaviours for flocking simulation are declared there.

### Demo

Demo folder contains the demo of Flocking System using QtNGL.

## References

- Craig Reynolds (2001). *Boids. Background and Update* [online]. Available from: http://www.red3d.com/cwr/boids/ [Accessed 2017]
- Computer Graphics (2010). *Flocking and Steering Behaviors* [online]. Available from: http://www.cs.cmu.edu/afs/cs/academic/class/15462-s10/www/lec-slides/Lecture24_flocking.pdf [Accessed 2017]
- Fernando Bevilacqua (2012). *Understanding Steering Behaviors: Seek* [online]. Available from: https://gamedevelopment.tutsplus.com/tutorials/understanding-steering-behaviors-seek--gamedev-849 [Accessed 2018]
- Daniel Shiffman (2012). *Chapter 6. Autonomous Agents* [online]. Available from: http://natureofcode.com/book/chapter-6-autonomous-agents/ [Accessed 2018]
