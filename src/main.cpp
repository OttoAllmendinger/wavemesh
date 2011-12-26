#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGLWidget>
#include <QWidget>
#include <QSize>
#include <QTimer>

#include "MainWindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow m;
  m.show();
  return a.exec();
}
