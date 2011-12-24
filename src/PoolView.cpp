#include <QMouseEvent>
#include <QImage>
#include <QTimer>
#include <QDateTime>

#include "PoolView.h"
#include "PoolModel.h"

PoolView::PoolView(QWidget* parent, PoolModel* poolModel)
  : QGLWidget(parent), 
    poolModel_(poolModel),
    paused_(false) {

  setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  setMinimumWidth(128);
  setMinimumHeight(128);
};

PoolView::~PoolView() {
  //
}

void PoolView::update() {   
  if (!paused_) {
    poolModel_->update();
  }
  updateGL();
}

void PoolView::initializeGL() {
  glClearColor(0, 0, 0, 1);
  glClearDepth(1);  
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);  
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    
}

void PoolView::resizeGL(int width, int height) {
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
// glFrustum(-1,1,-1,1,-1,1);
// gluPerspective(20.0f, (GLfloat)w/(GLfloat)h, 0.1f, 10.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void PoolView::paintGL() {  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // clear the window
  glLoadIdentity();
  glTranslatef(0, 0, -3);
  glRotated(rotX_,  0, 0, 1);
  glRotated(rotY_, -1, 0, 0);
  
  glBegin(GL_QUADS);

  // draw particle quads

  glEnd(); 
}

void PoolView::mousePressEvent(QMouseEvent *event) {
  (void) event; // avoid unused parameter warning
}

void PoolView::mouseMoveEvent(QMouseEvent *event) {
  (void) event; // avoid unused parameter warning
}