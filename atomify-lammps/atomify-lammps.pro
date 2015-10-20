TEMPLATE = app
CONFIG += c++11
DEFINES += LAMMPS_GZIP

QT += qml quick widgets opengl openglextensions
QMAKE_CXXFLAGS += -Wno-unused-parameter -Wno-unused-variable
QMAKE_CXXFLAGS += -g -fopenmp
QMAKE_LFLAGS += -fopenmp
CONFIG += warn_off

#DEFINES += _OPENMP

# lammps.pri includes the path and libs to lammps
# Run configure.py to generate lammps.pri


android {
    DEFINES += LAMMPS_XDR
    SOURCES += stdioext.c
    HEADERS += stdioext.h
    include(../lammps-android.pri)
} else {
    include(../lammps.pri)
}

RESOURCES += qml.qrc \
    images.qrc \
    scripts.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
include(../library_deployment.pri)


ios {
    QMAKE_INFO_PLIST = iOS.plist
}

SOURCES += \
    main.cpp \
    mysimulator.cpp \
    lammpscontroller.cpp \
    highlighter.cpp \
    CPcompute.cpp \
    lammpsoutput.cpp \
    lammpsfilehandler.cpp \
    atomstyle.cpp \
    fileio.cpp \
    scriptparser.cpp \
    scripthandler.cpp \
    nvt.cpp

HEADERS += \
    mysimulator.h \
    lammpscontroller.h \
    highlighter.h \
    CPcompute.h \
    lammpsoutput.h \
    lammpsfilehandler.h \
    atomstyle.h \
    fileio.h \
    scriptparser.h \
    scripthandler.h \
    nvt.h

DISTFILES += \
    iOS.plist
