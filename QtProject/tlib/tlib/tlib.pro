#-------------------------------------------------
#
# Project created by QtCreator 2015-12-01T09:54:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tlib
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    plot/qcustomplot.cpp \
    tlib/core/core.cpp \
    tlib/interface/i4cpp/tlib.cpp \
    tlib/core/bch.cpp \
    tlib/core/buffer_c.cpp \
    tlib/core/corr_helper.cpp \
    tlib/core/dem.cpp \
    tlib/core/fft/Complexs.cpp \
    tlib/core/fft/ExpGen.cpp \
    tlib/core/fft/Expression.cpp \
    tlib/core/fft/IdxGen.cpp \
    tlib/core/fft/Transformer.cpp \
    tlib/core/big_int.cpp \
    tlib/core/rsa_use.cpp

HEADERS  += mainwindow.h \
    plot/qcustomplot.h \
    tlib/core/core.h \
    tlib/interface/i4cpp/tlib.h \
    tlib/core/bch.h \
    tlib/core/big_int.h \
    tlib/core/buffer_c.h \
    tlib/core/corr_helper.h \
    tlib/core/dem.h \
    tlib/core/rsa_use.h \
    tlib/core/sab_code/CodeA00.h \
    tlib/core/sab_code/CodeA01.h \
    tlib/core/sab_code/CodeA10.h \
    tlib/core/sab_code/CodeA11.h \
    tlib/core/sab_code/CodeB00.h \
    tlib/core/sab_code/CodeB01.h \
    tlib/core/sab_code/CodeB10.h \
    tlib/core/sab_code/CodeB11.h \
    tlib/core/sab_code/CodeS0.h \
    tlib/core/sab_code/CodeS1.h \
    tlib/core/fft/Complexs.h \
    tlib/core/fft/ExpGen.h \
    tlib/core/fft/Expression.h \
    tlib/core/fft/IdxGen.h \
    tlib/core/fft/Transformer.h

FORMS    += mainwindow.ui
qtHaveModule(printsupport): QT += printsupport
