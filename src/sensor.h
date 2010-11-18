#ifndef _sensor_h_
#define _sensor_h_

#include <QWidget>
#include <QPainter>
#include <QTimer>

struct s_sensor {
  float *src;
  float *samples;
  float k;
};

typedef struct s_sensor sensor;

class wg_sensor : public QWidget {
  Q_OBJECT;
  public:
    wg_sensor(QWidget*);
    void add_value(float*);
    void set_samples(int n);
  protected:
    void paintEvent(QPaintEvent*);
  private:    
    sensor* src_values;
    int  n_values;
    int  frame;
    int  n_samples;
    int  c_sample;
    QPainter* painter;
    QPixmap * canvas;
    QWidget * canvas_wg;
  public slots:
    void next_frame();
};

#endif