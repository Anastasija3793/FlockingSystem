#include <QGuiApplication>
#include <QMouseEvent>

#include "NGLScene.h"
#include <iostream>
#include <ngl/Vec3.h>
#include <ngl/Light.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/Material.h>
#include <ngl/ShaderLib.h>
#include <QColorDialog>


//----------------------------------------------------------------------------------------------------------------------
NGLScene::NGLScene( QWidget *_parent ) : QOpenGLWidget( _parent )
{
  // set this widget to have the initial keyboard focus
  setFocus();
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  this->resize(_parent->size());

	m_wireframe=false;
	m_rotation=0.0;
	m_scale=1.0;
    m_position=0.0;

	m_selectedObject=0;
}

// This virtual function is called once before the first call to paintGL() or resizeGL(),
//and then once whenever the widget has been assigned a new QGLContext.
// This function should set up any required OpenGL context rendering flags, defining display lists, etc.

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::initializeGL()
{

  ngl::NGLInit::instance();
  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  /// create our camera
  ngl::Vec3 eye(10,10,10); //(2,2,2) //10,10,10
  ngl::Vec3 look(0,0,0);
  ngl::Vec3 up(0,1,0);

  m_cam.set(eye,look,up);
  m_cam.setShape(45,float(1024/720),0.1,300);
  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // we are creating a shader called Phong
  shader->createShaderProgram("Phong");
  // now we are going to create empty shaders for Frag and Vert
  shader->attachShader("PhongVertex",ngl::ShaderType::VERTEX);
  shader->attachShader("PhongFragment",ngl::ShaderType::FRAGMENT);
  // attach the source
  shader->loadShaderSource("PhongVertex","shaders/PhongVertex.glsl");
  shader->loadShaderSource("PhongFragment","shaders/PhongFragment.glsl");
  // compile the shaders
  shader->compileShader("PhongVertex");
  shader->compileShader("PhongFragment");
  // add them to the program
  shader->attachShaderToProgram("Phong","PhongVertex");
  shader->attachShaderToProgram("Phong","PhongFragment");
  // now bind the shader attributes for most NGL primitives we use the following
  // layout attribute 0 is the vertex data (x,y,z)
  shader->bindAttribute("Phong",0,"inVert");
  // attribute 1 is the UV data u,v (if present)
  shader->bindAttribute("Phong",1,"inUV");
  // attribute 2 are the normals x,y,z
  shader->bindAttribute("Phong",2,"inNormal");

  // now we have associated this data we can link the shader
  shader->linkProgramObject("Phong");
  // and make it active ready to load values
  (*shader)["Phong"]->use();
  shader->setUniform("Normalize",1);
  shader->setUniform("viewerPos",m_cam.getEye().toVec3());
  // now pass the modelView and projection values to the shader
  // the shader will use the currently active material and light0 so set them
  ngl::Material m(ngl::STDMAT::POLISHEDSILVER);
  m.loadToShader("material");
  // we need to set a base colour as the material isn't being used for all the params
  shader->setUniform("Colour",0.23125f,0.23125f,0.23125f,1.0f);

  ngl::Light light(ngl::Vec3(2,2,2),ngl::Colour(1,1,1,1),ngl::Colour(1,1,1,1),ngl::LightModes::POINTLIGHT);
  // now create our light this is done after the camera so we can pass the
  // transpose of the projection matrix to the light to do correct eye space
  // transformations
  ngl::Mat4 iv=m_cam.getViewMatrix();
  iv.transpose();
  light.setTransform(iv);
  light.setAttenuation(1,0,0);
  light.enable();
  // load these values to the shader as well
  light.loadToShader("light");

  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  prim->createSphere("sphere",0.1,40);
  prim->createCone("cone",0.2,0.7f,20,20);
  //prim->createLineGrid("wall", 10, 10, 5);

//  m_wind.set(1,1,1);
//  m_emitter.reset(new Emitter(ngl::Vec3(0,0,0),200,&m_wind)); //400
//  m_emitter->setCam(&m_cam);
//  m_emitter->setShaderName("Phong");

  //m_flock.reset(new Flock(ngl::Vec3(0,0,0),ngl::Vec3(0,0,0),200));
  m_flock.reset(new Flock(ngl::Vec3(0,0,0),300)); //100
  m_flock->setCam(&m_cam);
  m_flock->setShaderName("Phong");

  //call again to draw properly
  m_bbox.reset( new ngl::BBox(ngl::Vec3(),12.0f,12.0f,12.0f));
  m_bbox->setDrawMode(GL_LINE);

  m_text.reset(  new  ngl::Text(QFont("Arial",18)));
  m_text->setScreenSize(this->size().width(),this->size().height());
  m_text->setColour(1.0,1.0,0.0);

  //glViewport(0,0,width(),height());
  m_particleTimer=startTimer(20);
  //update();
}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget has been resized.
// The new size is passed in width and height.
void NGLScene::resizeGL( int _w, int _h )
{
  m_cam.setShape( 45.0f, static_cast<float>( _w ) / _h, 0.05f, 350.0f );
  m_win.width  = static_cast<int>( _w * devicePixelRatio() );
  m_win.height = static_cast<int>( _h * devicePixelRatio() );
}


void NGLScene::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["Phong"]->use();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  M=m_mouseGlobalTX*m_transform.getMatrix();
  MV=m_cam.getViewMatrix()*M;
  MVP=m_cam.getProjectionMatrix() *MV;
  //-----
//  M = m_mouseGlobalTX;
//  MV = m_cam.getViewMatrix() * M;
//  MVP = m_cam.getVPMatrix() * M;
  //-----

  normalMatrix=MV;
  normalMatrix.inverse().transpose();
  shader->setUniform("MV",MV);
  shader->setUniform("MVP",MVP);
  shader->setUniform("normalMatrix",normalMatrix);
  shader->setUniform("M",M);
}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget needs to be painted.
// this is our main drawing routine
void NGLScene::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_win.width,m_win.height);
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  if(m_wireframe == true)
  {
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  }
  else
  {
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  }
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["Phong"]->use();

//-----
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  // create the rotation matrices
  rotX.rotateX( m_win.spinXFace );
  rotY.rotateY( m_win.spinYFace );
  // multiply the rotations
  m_mouseGlobalTX = rotX * rotY;
  // add the translations
  m_mouseGlobalTX.m_m[ 3 ][ 0 ] = m_modelPos.m_x;
  m_mouseGlobalTX.m_m[ 3 ][ 1 ] = m_modelPos.m_y;
  m_mouseGlobalTX.m_m[ 3 ][ 2 ] = m_modelPos.m_z;
//-----
  m_transform.reset();

//    m_transform.setPosition(m_position);
//    m_transform.setScale(m_scale);
//    m_transform.setRotation(m_rotation);
    loadMatricesToShader();

    m_bbox->draw();

    //prim->draw("wall");

	switch(m_selectedObject)
	{
        case 0 :
        m_flock->draw(m_mouseGlobalTX);
        if(m_align == true)
        {
            m_flock->alignment();
        }
        if(m_separate == true)
        {
            m_flock->separation();
        }
        break;
        //case 0 : m_emitter->draw(); break;
        //case 0 : prim->draw("teapot"); break;
        case 1 : prim->draw("sphere"); break;
        //case 2 : prim->draw("cube"); break;
        case 2 : prim->draw("cone"); break;
	}



    m_text->renderText(10,10,"Flocking System"); //Qt Gui Demo
}



void NGLScene::timerEvent(QTimerEvent *_event )
{
    if(_event->timerId() ==   m_particleTimer)
    {
        //m_emitter->update();
        m_flock->update();
        m_flock->BBoxCollision();
        //updateScene();
        update();
    }

        // re-draw GL
}



NGLScene::~NGLScene()
{
}

void NGLScene::alignState(bool _mode)
{
    m_align=_mode;
    update();
}

void NGLScene::separateState(bool _mode)
{
    m_separate=_mode;
    update();
}

void NGLScene::add()
{
    m_flock->addBoid();
    //m_flock->update();
    update();
}

void NGLScene::remove()
{
    m_flock->removeBoid();
    update();
}

void NGLScene::toggleWireframe(bool _mode	 )
{
	m_wireframe=_mode;
	update();
}

void NGLScene::setXRotation( double _x		)
{
	m_rotation.m_x=_x;
	update();
}

void NGLScene::setYRotation( double _y	)
{
	m_rotation.m_y=_y;
	update();
}
void NGLScene::setZRotation( double _z )
{
	m_rotation.m_z=_z;
	update();
}

void NGLScene::setXScale( double _x	)
{
	m_scale.m_x=_x;
	update();
}

void NGLScene::setYScale(	 double _y)
{
	m_scale.m_y=_y;
	update();
}
void NGLScene::setZScale( double _z )
{
	m_scale.m_z=_z;
	update();
}

void NGLScene::setXPosition( double _x	)
{
	m_position.m_x=_x;
	update();
}

void NGLScene::setYPosition( double _y	)
{
	m_position.m_y=_y;
	update();
}
void NGLScene::setZPosition( double _z	 )
{
	m_position.m_z=_z;
	update();
}

void NGLScene::setObjectMode(	int _i)
{
	m_selectedObject=_i;
	update();
}
void NGLScene::setColour()
{
	QColor colour = QColorDialog::getColor();
	if( colour.isValid())
	{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["Phong"]->use();
    shader->setUniform("Colour",colour.redF(),colour.greenF(),colour.blueF(),1.0f);
    update();
	}
}
