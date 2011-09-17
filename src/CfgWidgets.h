#include <QFrame>
#include <QLabel>
#include <QGroupBox>
#include <QSlider>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QFileDialog>
#include <QSlider>
#include "WaveSim.h"
#include "WaveWidget.h"

extern WaveWidget* main_widget;
extern WaveSim*    main_pool;

class val_slider : public QSlider {
  Q_OBJECT;
  public:
    val_slider(QWidget* parent, float *p_value);

  public slots:
    void alter_dst(int);    
  private:
    float *dst;
};

class cfg_source : public QGroupBox {
  Q_OBJECT
  public:
    cfg_source(QWidget* parent, const char* label, float* p_offset, float* p_contrast, p_value* p_source);
  private slots:
    void set_source(int);
  private:
    p_value *p_dst;
};

class cfg_sources : public QGroupBox {
  public:
    cfg_sources(QWidget *parent);
};


class cfg_ws : public QGroupBox {
  Q_OBJECT
  public:
    cfg_ws(QWidget *parent);
  private slots:
//     void set_speed(int);
//     void set_transmission(int);
    void set_samples(int);
    void set_loss(int);
    void set_file();
  private:
    void read_file();
    QString cur_file;
//     void 
};

class cfg_time : public QGroupBox {
  Q_OBJECT;
  public:
    cfg_time(QWidget*);
  private slots:
    void set_speed(int);
    void set_pause(bool);
    void step();
};

class cfg_emitter:public QGroupBox{
  Q_OBJECT;
  public:
    cfg_emitter(QWidget*);
  private slots:
    void set_emitter_override(bool);
    void set_freq(int);
};