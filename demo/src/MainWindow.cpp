#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), m_ui(new Ui::MainWindow)
{
  m_ui->setupUi(this);

  m_gl=new  NGLScene(this);

  m_ui->s_mainWindowGridLayout->addWidget(m_gl,0,0,2,1);
  connect(m_ui->m_wireframe,SIGNAL(toggled(bool)),m_gl,SLOT(toggleWireframe(bool)));
  /// set the combo box index change signal
  connect(m_ui->m_objectSelection,SIGNAL(currentIndexChanged(int)),m_gl,SLOT(setObjectMode(int)));
  connect(m_ui->m_colour,SIGNAL(clicked()),m_gl,SLOT(setColour()));
  /// add and remove boids
  connect(m_ui->m_add,SIGNAL(clicked()),m_gl,SLOT(add()));
  connect(m_ui->m_remove,SIGNAL(clicked()),m_gl,SLOT(remove()));
  /// set flock steering
  connect(m_ui->m_steer,SIGNAL(toggled(bool)),m_gl,SLOT(steerState(bool)));
  /// set flock alignment
  connect(m_ui->m_align,SIGNAL(toggled(bool)),m_gl,SLOT(alignState(bool)));
  /// set flock separation
  connect(m_ui->m_separate,SIGNAL(toggled(bool)),m_gl,SLOT(separateState(bool)));
  /// set flock cohesion
  connect(m_ui->m_centre,SIGNAL(toggled(bool)),m_gl,SLOT(centreState(bool)));
  /// set flock wandering
  connect(m_ui->m_wander,SIGNAL(toggled(bool)),m_gl,SLOT(wanderState(bool)));
}

MainWindow::~MainWindow()
{
    delete m_ui;
}
