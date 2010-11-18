#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGLWidget>
#include <QWidget>
#include <QSize>
#include <QTimer>
#include "wave_widget.h"
#include "cfg_widgets.h"
#include "sensor.h"

wave_widget* main_widget;
wave_sim*    main_pool;
QTimer*      pulse;

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QWidget* main = new QWidget();
  pulse=new QTimer();
  pulse->start(50);
  main_widget = new wave_widget(main);
  main_pool   = main_widget->wave_pool;
  cfg_sources* cgs = new cfg_sources(main);
//   cfg_ws*      cwp = new cfg_ws(main);
  QHBoxLayout meta_layout(main);
  QVBoxLayout gl_layout;
  QVBoxLayout cfg_layout;
  cfg_layout.addWidget(new cfg_time(main));
  cfg_layout.addWidget(new cfg_ws(main));
  cfg_layout.addWidget(new cfg_emitter(main));

//  wg_sensor* tmp_sensor = new wg_sensor(main);			// beta

//  tmp_sensor->add_value( &(main_pool->pool[60][60].s) );		// too
//  tmp_sensor->add_value( &(main_pool->pool[60][60].v) );	

//  cfg_layout.addWidget(tmp_sensor);

  cfg_layout.addStretch();
  gl_layout.addWidget(main_widget);
  gl_layout.addWidget(cgs);
  meta_layout.addLayout(&cfg_layout);
  meta_layout.addLayout(&gl_layout);
  main->resize( 400, 400 );
  main->show();
  return a.exec();
}
