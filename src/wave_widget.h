#include <QGLWidget>
#include <QTimer>
#include "wave_sim.h"

#ifndef _wave_widget_h_
#define _wave_widget_h_

class wave_widget : public QGLWidget
{
    Q_OBJECT
public:
  wave_widget( QWidget* parent );
//     ~wave_widget();
  wave_sim* wave_pool;
  
  float c_red, c_green, c_blue;
  float o_red, o_green, o_blue;
  float delay;
  bool  paused;
  p_value p_red, p_green, p_blue, p_z;
  int   frames;
  int   t0, t;
  float k_height;  
  QTimer* timer;
    
public slots:
    void  update();
    
protected:    
  void  initializeGL();
  void  paintGL();
  void  resizeGL( int w, int h );
  void  mousePressEvent(QMouseEvent* e);
  void  mouseMoveEvent(QMouseEvent* e);
//     void  mouseDoubleClickEvent(QMouseEvent*);
//     void  read_file();

private:
    GLfloat x_rot, y_rot;
    GLfloat start_x_rot, start_y_rot;
    int start_x, start_y;
    
    float g_emitter_a, g_emitter_w;        
    bool override_emitter;
    GLuint object;
    
    QString cur_file;
    QPoint last_pos;
};


#endif 
