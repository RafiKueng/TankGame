QT -= gui core opengl
TARGET = lab
TEMPLATE = app
OBJECTS_DIR = ../../build/make/obj
DESTDIR = ../../build/make/bin
VPATH = ../../src

DEFINES += GL_EXT_PROTOTYPES

INCLUDEPATH += ../../src

linux-g++ {
    LIBS += -lGL -lGLU -lglut 
}

macx {
        LIBS += -framework OpenGL -framework GLUT
}


SOURCES +=  \
time.cpp\
textBox.cpp\
cannon.cpp\
bullet.cpp\
azimuthBar.cpp\
elevationBar.cpp\
powerBar.cpp\
radar.cpp\
speedBar.cpp\
scene.cpp\
main.cpp\

HEADERS += \
defs.hpp\
types.hpp\
gl_includes.hpp\
exception.hpp\
time.hpp\
drawable.hpp\
textBox.hpp\
cannon.hpp\
bullet.hpp\
azimuthBar.hpp\
elevationBar.hpp\
powerBar.hpp\
radar.hpp\
speedBar.hpp\
scene.hpp\
