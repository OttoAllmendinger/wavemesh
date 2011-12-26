#include <QMouseEvent>
#include <QTimer>
#include <QDateTime>

#include "PoolView.h"
#include "PoolModel.h"

PoolView::PoolView(QWidget* parent, PoolModel& poolModel)
  : QGLWidget(parent), poolModel_(poolModel), paused_(false) {

  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(20);
};

PoolView::~PoolView() {
  //
}

void PoolView::update() {   
  if (!paused_) {
    poolModel_.update();
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
  int side = qMin(width, height);
  glViewport((width - side) / 2, (height - side) / 2, side, side);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.0, +1.0, -1.0, 1.0, 5.0, 60.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslated(0.0, 0.0, -40.0);
}


void PoolView::paintGL() {  
  paintPoints();
}

void PoolView::paintPoints() {
  const int w = poolModel_.getWidth();
  const int h = poolModel_.getHeight();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // clear the window

  glLoadIdentity();

  glTranslated(0, 0, -3);

  glRotated(rotX_,  0, 0, 1);
  glRotated(rotY_, -1, 0, 0);
  
  glBegin(GL_POINTS);

  for (int x=0; x < w; x++) {
    for (int y=0; y < h; y++) {
      GLdouble glX = (x / w) - 0.5;
      GLdouble glY = (y / h) - 0.5;
      GLdouble glZ = poolModel_.getParticleHeight(x, y);
      
      glVertex3d(glX, glY, glZ);
    }
  }

  glEnd(); 
}

void PoolView::mousePressEvent(QMouseEvent *event) {
  (void) event; // avoid unused parameter warning
}

void PoolView::mouseMoveEvent(QMouseEvent *event) {
  (void) event; // avoid unused parameter warning
}
