#include "wave_widget.h"
#include <QMouseEvent>
#include <QImage>
#include <QTimer>
// #include <QFrame>
// #include <QFiledialog>
#include <time.h>
#define PI 3.14159265

#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

extern QTimer* pulse;

/* return current time (in seconds) */
static int
current_time(void)
{
   struct timeval tv;
   struct timezone tz;
   (void) gettimeofday(&tv, &tz);
   return (int) tv.tv_sec;
}


wave_widget::wave_widget(QWidget* parent) 
            :QGLWidget  (parent) { 

  setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  setMinimumWidth(128);
  setMinimumHeight(128);

  x_rot = y_rot = 45.0;     
  k_height    = 1.0;
  o_red=o_green=o_blue = 0.0;  
  c_red=c_green=c_blue = 1.0;
  o_blue=0.5; 
  
  wave_pool   = new WaveSim(128, 128);
  wave_pool->add_emitter(64, 64, 0.2, PI/16, 0, 0.0);

  p_red   = &s_particle::v;
  p_green = &s_particle::i;
  p_blue  = &s_particle::s;
  p_z     = &s_particle::s;
  
  connect( pulse, SIGNAL(timeout()), this, SLOT(update()));

  paused=false;

  frames=0;
  t0    = current_time();
  t     = current_time();
};

// wave_widget::~wave_widget() {};

void wave_widget::update() {   
  if (!paused) wave_pool->update();
  updateGL();
  t = current_time();
  if ((t-t0)>=10) {    
    printf("%d frames, %3.2f fps\n", frames, (float)frames/(float)(t-t0));
    frames=0;
    t0=current_time();
  }
}

void wave_widget::initializeGL() {
  glClearColor(0.0f,0.0f,0.0f,1.0f);
  glClearDepth(1.0f);  
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);  
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    
}

void wave_widget::resizeGL(int w, int h) {
  glViewport(0,0,w,h);    
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
//   glFrustum(-1,1,-1,1,-1,1);
  gluPerspective(20.0f, (GLfloat)w/(GLfloat)h, 0.1f, 10.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void wave_widget::paintGL() {  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // clear the window
  glLoadIdentity();
  glTranslatef( 0.0, 0.0, -3.14 );
  glRotatef( y_rot, -1.0, 0.0, 0.0);
  glRotatef( x_rot, +0.0, 0.0, 1.0);
  
  int n=0;
  

  Coord* tq; 
  glBegin(GL_QUADS);
  while (n<wave_pool->n_mesh_nodes) {
    tq = &wave_pool->mesh[n];   
    glColor3f( o_red   + tq->p->*p_red * c_red     * 4,
               o_green + tq->p->*p_green * c_green * 4, 
               o_blue  + tq->p->*p_blue * c_blue   * 4 );
    glVertex3f(tq->x, tq->y, k_height*tq->p->*p_z);
    
    n++;        
  }
  glEnd(); 
 
  frames++;
}

void wave_widget::mousePressEvent(QMouseEvent *event) {
  last_pos = event->pos();
  start_x_rot = x_rot;
  start_y_rot = y_rot;
}

void wave_widget::mouseMoveEvent(QMouseEvent *event) {
  int dx = event->x() - last_pos.x();
  int dy = event->y() - last_pos.y();
  x_rot = start_x_rot + 180 * dx/(float)width();
  y_rot = start_y_rot + 180 * dy/(float)height();
  if (y_rot<0) y_rot=0;
  if (y_rot>90)y_rot=90;
  updateGL();
}
