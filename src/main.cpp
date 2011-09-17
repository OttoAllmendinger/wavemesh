#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGLWidget>
#include <QWidget>
#include <QSize>
#include <QTimer>
#include "WaveWidget.h"
#include "CfgWidgets.h"
#include "Sensors.h"

WaveWidget* main_widget;
WaveSim*    main_pool;
QTimer*      pulse;

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QWidget* main = new QWidget();
  pulse=new QTimer();
  pulse->start(50);
  main_widget = new WaveWidget(main);
  main_pool   = main_widget->wave_pool;
  cfg_sources* cgs = new cfg_sources(main);
//   cfg_ws*      cwp = new cfg_ws(main);
  QHBoxLayout meta_layout(main);
  QVBoxLayout gl_layout;
  QVBoxLayout cfg_layout;
  cfg_layout.addWidget(new cfg_time(main));
  cfg_layout.addWidget(new cfg_ws(main));
  cfg_layout.addWidget(new cfg_emitter(main));

//  wg_Sensors* tmp_Sensors = new wg_Sensors(main);			// beta

//  tmp_Sensors->add_value( &(main_pool->pool[60][60].s) );		// too
//  tmp_Sensors->add_value( &(main_pool->pool[60][60].v) );	

//  cfg_layout.addWidget(tmp_Sensors);

  cfg_layout.addStretch();
  gl_layout.addWidget(main_widget);
  gl_layout.addWidget(cgs);
  meta_layout.addLayout(&cfg_layout);
  meta_layout.addLayout(&gl_layout);
  main->resize( 400, 400 );
  main->show();
  return a.exec();
}
