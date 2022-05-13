#ifndef SCENE3D_H
#define SCENE3D_H

#include "clss.h"

#include <QObject>
#include <QList>
#include "object3d.h"
#include "camera.h"
#include "clss.h"

class Scene3D : public QObject
{
    Q_OBJECT
public:
    QList <Object3D*> elems;
    QList <Camera*> cams;
    QList <SelectablePoint*> selectables;

    SelectablePoint* selected = nullptr;
public:
    Scene3D();
    ~Scene3D();

    void addObject(Object3D *el);
    void addCamera(Camera *cam);

    // to have foreach-like loop
    decltype(elems.begin()) begin() {return elems.begin();};
    decltype(elems.end()) end() {return elems.end();};

    QList<Object3D*> items();
    QList<Camera*> cameras();

public slots:
    void MoveRay(Ray &from, Ray &to);
    void PressRay(Ray &ray);

signals:
    void SceneChanged();
};

#endif // SCENE3D_H
