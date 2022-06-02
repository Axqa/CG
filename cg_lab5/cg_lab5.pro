QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    MathEngine/aabb.cpp \
    MathEngine/float2.cpp \
    MathEngine/float3.cpp \
    MathEngine/float4.cpp \
    MathEngine/line.cpp \
    MathEngine/linesegment.cpp \
    MathEngine/matrix.cpp \
    MathEngine/plane.cpp \
    MathEngine/ray.cpp \
    Object3D/camera.cpp \
    Object3D/line3d.cpp \
    Object3D/object3d.cpp \
    Object3D/scene3d.cpp \
    Object3D/sceneview.cpp \
    main.cpp \
    mainwindow.cpp \
    styleloader.cpp

HEADERS += \
    EnumMacros.h \
    MathEngine/Intersections.h \
    MathEngine/MathAll.h \
    MathEngine/MathConstants.h \
    MathEngine/aabb.h \
    MathEngine/beziermath.h \
    MathEngine/float2.h \
    MathEngine/float3.h \
    MathEngine/float4.h \
    MathEngine/line.h \
    MathEngine/linesegment.h \
    MathEngine/matrix.h \
    MathEngine/ncl.h \
    MathEngine/plane.h \
    MathEngine/ray.h \
    MathEngine/somemath.h \
    Object3D/camera.h \
    Object3D/classes.h \
    Object3D/line3d.h \
    Object3D/object3d.h \
    Object3D/scene3d.h \
    Object3D/sceneview.h \
    commondec.h \
    mainwindow.h \
    myassert.h \
    styleloader.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rsc.qrc

DISTFILES += \
    .gitignore \
    style.qss
