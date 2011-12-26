#ifndef POOLVIEW_H
#define POOLVIEW_H

#include <QGLWidget>
#include <QTimer>

class PoolModel;

class PoolView : public QGLWidget {

  Q_OBJECT

public:
  PoolView(QWidget* parent, PoolModel& model);
  ~PoolView();

public slots:
  void update();

protected:    
  void initializeGL();
  void paintGL();
  void resizeGL(int width, int height);
  void mousePressEvent(QMouseEvent* event);
  void mouseMoveEvent(QMouseEvent* event);

private:
  PoolModel& poolModel_;

  bool paused_;
  float rotX_;
  float rotY_;

  void paintPoints();
};

#endif 
