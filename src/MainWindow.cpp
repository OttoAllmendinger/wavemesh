#include <iostream>

#include "MainWindow.h"
#include "PoolModel.h"
#include "PoolView.h"

MainWindow::MainWindow() 
  : QMainWindow(),
    poolModel_(new PoolModel(512, 256)),
    poolView_(new PoolView(this, *poolModel_)) { }

MainWindow::~MainWindow() {
  delete poolView_;
  delete poolModel_;
}
