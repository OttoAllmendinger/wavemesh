TEMPLATE = app
TARGET = wavemesh.bin
DEPENDPATH += . src
INCLUDEPATH += . src
QT += opengl
HEADERS += src/PoolView.h src/PoolModel.h
SOURCES += src/main.cpp \
           src/PoolModel.cpp \
           src/PoolView.cpp
