QMAKE_CXXFLAGS=-Wfatal-errors -g
TEMPLATE = app
TARGET = wavemesh.bin
DEPENDPATH += . src
INCLUDEPATH += . src
QT += opengl
HEADERS += src/MainWindow.h src/PoolView.h src/PoolModel.h
SOURCES += src/main.cpp \
           src/MainWindow.cpp \
           src/PoolModel.cpp \
           src/PoolView.cpp
