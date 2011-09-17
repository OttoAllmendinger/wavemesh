#ifndef _Sensors_h_
#define _Sensors_h_

#include <QWidget>
#include <QPainter>
#include <QTimer>

struct s_Sensors {
  float *src;
  float *samples;
  float k;
};

typedef struct s_Sensors Sensors;

class wg_Sensors : public QWidget {
  Q_OBJECT;
  public:
    wg_Sensors(QWidget*);
    void add_value(float*);
    void set_samples(int n);
  protected:
    void paintEvent(QPaintEvent*);
  private:    
    Sensors* src_values;
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