#include <stdio.h>
#include "Sensors.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

extern QTimer* pulse;

wg_Sensors::wg_Sensors(QWidget* parent) : QWidget(parent) {
  setMinimumHeight(128);
  setMinimumWidth(128);

//   setBackgroundRole(QPalette::Base);
//   setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  
  connect( pulse, SIGNAL(timeout()), this, SLOT(next_frame()));
  src_values=NULL;
  n_values=0;
  frame=0;
  n_samples=width();
  canvas = new QPixmap(128,128);  

//   update();  
}

void wg_Sensors::next_frame(){
//   for (int n; n<n_values, n++)
  update();
}

void wg_Sensors::add_value(float* p) {
  if (src_values!=NULL) {
    printf("realloc src_values (%d)\n",n_values+1);
    src_values=(Sensors*)realloc(src_values, (n_values+1)*sizeof(Sensors));
  } else {
    printf("alloc src_values\n");
    src_values=(Sensors*)malloc (sizeof(Sensors));
  }  
  src_values[n_values]=(Sensors){p, NULL, 1000.0};  
  n_values++;
  set_samples(n_samples);
//   printf("%d values\n", n_values);
}

void wg_Sensors::set_samples(int n) {
  printf("set %d samples for %d Sensorss\n",n,n_values);
  for (int j=0; j<n_values; j++) {
    printf("Sensors %d\n",j);
    if (src_values[j].samples==NULL) {
      src_values[j].samples=(float*)malloc(n*sizeof(float));  
      printf("alloc %d samples\n", n);
    } else {
      src_values[j].samples=(float*)realloc(src_values[j].samples, n*sizeof(float));
      printf("realloc %d samples\n", n);
    }
  }
  n_samples=n;
  printf("%d samples set\n",n);
}

void wg_Sensors::paintEvent(QPaintEvent* pev){
//   printf("paint_event\n");
  QWidget::paintEvent(pev);
//   bitBlt( this, r.x(), r.y(), &buffer, r.x(), r.y(), r.width(), r.height() );  
  QPainter painter(this);
  painter.setPen(Qt::red);  
  Sensors* ts;
  int dy=width()/2;
  for (int n=0; n<n_values; n++) {
    ts = &(src_values[n]);
    ts->samples[frame]=*(ts->src);
    for (int subn=0; subn<frame-1; subn++) {
      painter.drawLine(subn,dy+(int)(ts->k)*(ts->samples[subn]),subn+1,dy+(int)(ts->k)*(ts->samples[subn+1]));
    }    
  }
  frame = (frame+1)%n_samples;

}
