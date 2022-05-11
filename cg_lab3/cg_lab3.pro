QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    MathEngine/float2.cpp \
    MathEngine/float3.cpp \
    MathEngine/float4.cpp \
    MathEngine/matrix.cpp \
    MathEngine/plane.cpp \
    MathEngine/ray.cpp \
    Objects3D/camera.cpp \
    Objects3D/line3d.cpp \
    Objects3D/object3d.cpp \
    Objects3D/point3.cpp \
    Objects3D/scene3d.cpp \
    Objects3D/selectable.cpp \
    Objects3D/selectablepoint.cpp \
    customgraphicsscene.cpp \
    main.cpp \
    mainwindow.cpp \
    mygraphicsview.cpp \
    styleloader.cpp

HEADERS += \
    EnumMacros.h \
    MathEngine/Intersections.h \
    MathEngine/MathAll.h \
    MathEngine/MathConstants.h \
    MathEngine/float2.h \
    MathEngine/float3.h \
    MathEngine/float4.h \
    MathEngine/matrix.h \
    MathEngine/ncl.h \
    MathEngine/plane.h \
    MathEngine/ray.h \
    MathEngine/somemath.h \
    Objects3D/camera.h \
    Objects3D/clss.h \
    Objects3D/line3d.h \
    Objects3D/object3d.h \
    Objects3D/point3.h \
    Objects3D/scene3d.h \
    Objects3D/selectable.h \
    Objects3D/selectablepoint.h \
    commondec.h \
    customgraphicsscene.h \
    mainwindow.h \
    myassert.h \
    mygraphicsview.h \
    styleloader.h

FORMS += \
    mainwindow.ui \
    mygraphicsview.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    cg_lab3.pro.user \
    style.qss

RESOURCES += \
    rsc.qrc
