#include <math.h>
#include "cfg_widgets.h"
#define  PI 3.14159265

extern QTimer* pulse;

val_slider::val_slider(QWidget* parent, float *p_value) : QSlider ( Qt::Horizontal, parent ) {
  dst  = p_value;
  setValue((int)(*dst*100));
  connect(this, SIGNAL(valueChanged(int)), this, SLOT(alter_dst(int)));  
}

void val_slider::alter_dst(int n) {
  *dst = (float)n/100.0;
  main_widget->update();
}

cfg_source::cfg_source(QWidget* parent, const char* label, float *p_offset, float *p_contrast, p_value *p_source) : QGroupBox(label, parent) {  
  p_dst = p_source;
  QVBoxLayout *main = new QVBoxLayout;
  
  QComboBox *cb_src = new QComboBox(this);
  cb_src->addItem("Height");
  cb_src->addItem("Velocity");
  cb_src->addItem("Intensity");
  cb_src->setToolTip(QString("Source Value"));
  connect(cb_src, SIGNAL(activated(int)), this, SLOT(set_source(int)));
  
  main->addWidget(cb_src);
  
  if (p_offset) {
    val_slider* vs_offset = new val_slider(this, p_offset);
    vs_offset->setToolTip(QString("Offset"));
    main->addWidget( vs_offset );
  }
  if (p_contrast) {
    val_slider* vs_contrast = new val_slider(this, p_contrast);
    vs_contrast->setToolTip(QString("Contrast"));
    main->addWidget( vs_contrast );
  }

  main->addStretch();
  setLayout(main);
}

void cfg_source::set_source(int s) {
  switch (s) {
    case 0:
      *p_dst = &s_particle::s;
      break;
    case 1:
      *p_dst = &s_particle::v;
      break;
    case 2:
      *p_dst = &s_particle::i;
      break;
  }
  main_widget->updateGL();
}

cfg_sources::cfg_sources(QWidget* parent) : QGroupBox("Source Configuration", parent) {
  setMaximumHeight(148);
  
  QHBoxLayout* meta_layout = new QHBoxLayout(this);
  cfg_source* src_r = new cfg_source(this, "Red",   &(main_widget->o_red),    &(main_widget->c_red),   &(main_widget->p_red));
  cfg_source* src_g = new cfg_source(this, "Green", &(main_widget->o_green),  &(main_widget->c_green), &(main_widget->p_green));
  cfg_source* src_b = new cfg_source(this, "Blue",  &(main_widget->o_blue),   &(main_widget->c_blue),  &(main_widget->p_blue));
  cfg_source* src_z = new cfg_source(this, "Z",     &(main_widget->k_height), NULL,                      &(main_widget->p_z));
 
  meta_layout->addWidget( src_r );
  meta_layout->addWidget( src_g );
  meta_layout->addWidget( src_b );
  meta_layout->addWidget( src_z );
  setLayout(meta_layout);
  
}

cfg_ws::cfg_ws(QWidget* parent) : QGroupBox("Wave Simulation",parent) {
  setMaximumWidth(164);
  QSpinBox* sb_samples = new QSpinBox(this);
  sb_samples->setRange(0, 64); sb_samples->setSuffix(" Samples");
  connect(sb_samples, SIGNAL(valueChanged(int)), this, SLOT(set_samples(int)));    

  QSlider* sl_tq = new QSlider(Qt::Horizontal, this);
  connect(sl_tq, SIGNAL(valueChanged(int)), this, SLOT(set_loss(int)));    
  
  QPushButton* b_fopen = new QPushButton("Open Map...", this); 
  connect(b_fopen, SIGNAL(clicked()), this, SLOT(set_file()));

  QVBoxLayout* lt_general = new QVBoxLayout;
  lt_general->addWidget( b_fopen );
  lt_general->addWidget( sb_samples );
  lt_general->addWidget( new QLabel("Interita", this) );
  lt_general->addWidget( new val_slider(this, &(main_pool->w)));
  lt_general->addWidget( new QLabel("Transmisson", this) );
  lt_general->addWidget( new val_slider(this, &(main_pool->c)));
  lt_general->addWidget( new QLabel("Loss", this) );
  lt_general->addWidget( sl_tq );
  setLayout(lt_general);
  
}

void cfg_ws::set_samples(int n) {
  main_pool->set_samples(n);
}

void cfg_ws::set_loss(int n) {
  float d = 1.0 - (float)pow(M_E,n/10.0)/(float)pow(M_E,10.0);
  printf("%d, %.6f\n", n, d);
  if (d<0.0) d=0.0;
  main_pool->d = d; 
}

void cfg_ws::set_file() {
  cur_file = QFileDialog::getOpenFileName( this, "Choose a file", "", "Images (*.png *.xpm *.jpg)" );
  read_file();
}

void cfg_ws::read_file() {
  int width, height;
  int x,y;
  if (cur_file.isEmpty()) {
    printf("wont read\n");    
    return;
  }
    
  QImage Image;
  Image.load(cur_file);  
  height = (Image.height()<256)?(Image.height()):(256);
  width  = (Image.width() <256)?(Image.width() ):(256);
  
  main_pool->resize(width, height);
  
  QRgb Pixel;
  for (x=1; x<width; x++)
    for (y=1; y<height; y++) {    
      Pixel = Image.pixel(x,y);
      int r = qRed(Pixel);
      int g = qGreen(Pixel);
      int b = qBlue(Pixel);
      if (r==255 && b==0) {
        main_pool->add_emitter(x-1, y-1, (qBlue(Pixel)/255.0/10.0), (PI/(qGreen(Pixel)+6)), 0.0, 0.0 );
        continue;
      } 
      if ((r==255) && (b==255)) {
        main_pool->set_particle(x-1,y-1,1.0,1.0);
        continue;
      }
      if (r==0 && b==0) {
        main_pool->set_particle(x-1,y-1,1.0,0.0);
        continue;
      }
    }
  printf("done read_file()\n");  
}

cfg_time::cfg_time(QWidget* parent):QGroupBox("Timing",parent) {
  setMaximumWidth(164);
//   setMaximumHeight(128);

  QVBoxLayout *main = new QVBoxLayout;
  QSlider* sl_speed = new QSlider(Qt::Horizontal, this);

  QPushButton* b_pause = new QPushButton("Pause", this);
  b_pause->setCheckable(true);
  connect(b_pause, SIGNAL(clicked(bool)), this, SLOT(set_pause(bool)));

  QPushButton* b_step = new QPushButton("Step", this);
  b_step->setAutoRepeat(true);
  connect(b_step, SIGNAL(clicked()), this, SLOT(step()));


  sl_speed->setRange(0,100);
  connect(sl_speed, SIGNAL(valueChanged(int)), this, SLOT(set_speed(int)));  
  
  main->addWidget(b_pause);
  main->addWidget(b_step);
  main->addWidget(sl_speed);
  setLayout(main);
}

void cfg_time::set_speed(int n) {
  pulse->setInterval(101-n);
}

void cfg_time::set_pause(bool pause) {
  main_widget->paused=pause;
}

void cfg_time::step() {
  main_pool->update();
  main_widget->updateGL();
}

cfg_emitter::cfg_emitter(QWidget* parent) : QGroupBox("Override Emitter", parent) {
  setMaximumWidth(164);
  setCheckable(true); 
  setChecked(false);
  connect(this, SIGNAL(toggled(bool)), this, SLOT(set_emitter_override(bool)));

  QSpinBox* sb_freq = new QSpinBox(this);
  sb_freq->setRange(0, 64); sb_freq->setPrefix("PI /"); sb_freq->setValue(16);
  connect(sb_freq, SIGNAL(valueChanged(int)), this, SLOT(set_freq(int)));    


  QVBoxLayout* lt_emitter = new QVBoxLayout;  
  lt_emitter->addWidget(new QLabel("Frequency"));
  lt_emitter->addWidget(sb_freq);
  lt_emitter->addWidget(new QLabel("Amplitude"));
  lt_emitter->addWidget(new val_slider(this, &(main_pool->g_emitter_a)));
  setLayout(lt_emitter);
}

void cfg_emitter::set_emitter_override (bool e_over) {
  main_pool->override_emitter=e_over;
}

void cfg_emitter::set_freq(int f) {
  if (f>0) {
    main_pool->g_emitter_w = PI/(float)f;
    main_pool->g_emitter_t = 0.0;
  } else {
    main_pool->g_emitter_w = 0.0;
    main_pool->g_emitter_t = PI/2.0;
  }
}
