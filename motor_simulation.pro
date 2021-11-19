QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#INCLUDEPATH += A:/install/soft/Python39/include
#INCLUDEPATH += A:/install/soft/Python39
#LIBS += A:/install/soft/Python39/libs/python39.lib


#INCLUDEPATH += $$PWD/components/math/rk4
#LIBS += -L$$PWD/components/math/rk4 -lrk4

SOURCES += \
    app/sim_example/dc_brush_ctrl/dc_brush_ctrl.c \
    app/sim_example/dc_brush_ctrl/sim_dc_brush_ctrl.c \
    app/sim_example/pmsm_foc/pmsm_foc.c \
    app/sim_example/pmsm_foc/sim_pmsm_foc.c \
    app/sim_main.c \
    app/sim_module_test/test_dc_brush/sim_test_dc_brush.c \
    app/sim_module_test/test_dc_brush/test_dc_brush.c \
    app/sim_module_test/test_pmsm/sim_test_pmsm.c \
    app/sim_module_test/test_pmsm/test_pmsm.c \
    components/controller/pid/pid.c \
    components/math/coordinate/coordinate.c \
    components/math/rk4/rk4.c \
    components/service/base_obj.c \
    #components/service/cpy_plot.c \
    components/service/qt_plot.cpp \
    components/simscape/electrical/dc_brush/dc_brush.c \
    components/simscape/electrical/inverter/inverter.c \
    components/simscape/electrical/pmsm/pmsm.c \
    main.cpp \
    page/mainwindow.cpp\
    page/multicurvesplot.cpp \
    page/qcustomplot.cpp \
    page/waveplot.cpp

HEADERS += \
    app/sim_config.h \
    app/sim_example/dc_brush_ctrl/dc_brush_ctrl.h \
    app/sim_example/dc_brush_ctrl/sim_dc_brush_ctrl.h \
    app/sim_example/pmsm_foc/pmsm_foc.h \
    app/sim_example/pmsm_foc/sim_pmsm_foc.h \
    app/sim_main.h \
    app/sim_module_test/test_dc_brush/sim_test_dc_brush.h \
    app/sim_module_test/test_dc_brush/test_dc_brush.h \
    app/sim_module_test/test_pmsm/sim_test_pmsm.h \
    app/sim_module_test/test_pmsm/test_pmsm.h \
    components/controller/pid/pid.h \
    components/math/coordinate/coordinate.h \
    components/math/rk4/rk4.h \
    components/service/base_obj.h \
    #components/service/cpy_plot.h \
    components/service/qt_plot.h \
    components/simscape/electrical/dc_brush/dc_brush.h \
    components/simscape/electrical/inverter/inverter.h \
    components/simscape/electrical/pmsm/pmsm.h \
    page/mainwindow.h\
    page/multicurvesplot.h \
    page/qcustomplot.h \
    page/waveplot.h


FORMS += \
    page/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    py/wave_plot.py






