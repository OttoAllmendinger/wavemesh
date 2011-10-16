#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGLWidget>
#include <QWidget>
#include <QSize>
#include <QTimer>

#include "PoolModel.h"
#include "PoolView.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QMainWindow* main = new QMainWindow();
  PoolModel* poolModel = new PoolModel(511, 257);
  PoolView* poolView = new PoolView(main, poolModel);
  main->show();
  return a.exec();
}
