# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = app
TARGET = DV_Client
DESTDIR = ../Win32/Debug
QT += core gui network opengl
CONFIG += debug
DEFINES += QT_DLL QT_OPENGL_LIB QT_NETWORK_LIB
INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/Debug
LIBS += -lopengl32 \
    -lglu32
PRECOMPILED_HEADER = StdAfx.h
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(DV_Client.pri)
