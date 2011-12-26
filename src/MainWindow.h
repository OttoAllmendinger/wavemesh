#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class PoolModel;

class PoolView;

class MainWindow : public QMainWindow {
  public:
    MainWindow();
    ~MainWindow();
  private:
    PoolModel* poolModel_;
    PoolView* poolView_;
};

#endif
