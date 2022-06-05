#include "polygon3d.h"
#include <QGraphicsItemGroup>
#include <QDebug>

Polygon3D::Polygon3D()
{

}
Polygon3D::Polygon3D(QVector<float3> points)
    : points(points)
{
    init();
}

void Polygon3D::init()
{
    RecalcCenter();
    RecalcPlane();

//    qDebug() << "before connect";
    connect(this, &Object3D::ObjectChanged, this, &Polygon3D::RecalcCenter);
    connect(this, &Object3D::ObjectChanged, this, &Polygon3D::RecalcPlane);

//    qDebug() << "after connect";
}

int Polygon3D::NumVertex()
{
    return points.size();
}

void Polygon3D::SetColor(QColor col)
{
    color = col;
    calcColor = false;
}

QColor Polygon3D::GetColor(vec p)
{
    if (calcColor) {
        return NormToColor(p);
    } else {
        return color;
    }
}

void Polygon3D::AddPoint(vec p)
{
    if (NumVertex() > 2) {
        if (plane.Distance(p) < 1e-4) {
            points << p;
            RecalcCenter();
        }
    } else {
        points << p;
        RecalcCenter();
        RecalcPlane();
    }


}

QColor Polygon3D::NormToColor(vec p)
{
    int s = 1;
    if (!p.IsZero()) {
        if (plane.IsOnPositiveSide(p)) {
            s = -1;
        }
    }

    QColor col;

    int r = 127 + s*plane.normal.y * 127;
    int g = 127 + s*plane.normal.x * 127;
    int b = 127 + s*plane.normal.z * 127;

    col.setRgb(r,g,b, 127);
//    qDebug() << "norm color" << col << "from" << plane.normal;
    return col;
}

void Polygon3D::RecalcCenter()
{
    vec c;
    for (auto i : points) {
        c += i;
    }
    c /= NumVertex();
    center = c;
}

void Polygon3D::RecalcPlane()
{
    std::vector<float3> p = points.toStdVector();
    if (p.size() > 3)
    {
        Plane plane;
        for(size_t i = 0; i < p.size()-2; ++i)
            for(size_t j = i+1; j < p.size()-1; ++j)
            {
                vec pij = vec(p[j])-vec(p[i]);
                for(size_t k = j+1; k < p.size(); ++k)
                {
                    plane.normal = pij.Cross(vec(p[k])-vec(p[i]));
                    float lenSq = plane.normal.LengthSq();
                    if (lenSq > 1e-8f)
                    {
                        plane.normal /= sqrt(lenSq);
                        plane.d = plane.normal.Dot(vec(p[i]));

                        this->plane = plane;
                        return;
                    }
                }
            }

        // Polygon contains multiple points, but they are all collinear.
        // Pick an arbitrary plane along the line as the polygon plane (as if the polygon had only two points)
        vec dir = (vec(p[1])-vec(p[0])).Normalized();
        vec norm = dir.Perpendicular();
        vec perpNormal = norm - norm.ProjectToNorm(dir);
        this->plane = Plane(p[0], perpNormal.Perpendicular());
    }
    if (p.size() == 3){
        this->plane = Plane(p[0], p[1], p[2]);
        return;
    }
    if (p.size() == 2)
    {
        vec dir = (vec(p[1])-vec(p[0])).Normalized();
        vec norm = dir.Perpendicular();
        vec perpNormal = norm - norm.ProjectToNorm(dir);
        this->plane = Plane(p[0], perpNormal.Perpendicular());
        return;
    }
    if (p.size() == 1)
    {
        this->plane = Plane(p[0], float3(0,1,0));
        return;
    }
    this->plane = Plane();
    return;
}

Plane Polygon3D::PolygonPlane()
{
//    qDebug() << "recalc plane start";
    std::vector<float3> p = points.toStdVector();
    if (p.size() > 3)
    {
        Plane plane;
        for(size_t i = 0; i < p.size()-2; ++i)
            for(size_t j = i+1; j < p.size()-1; ++j)
            {
                vec pij = vec(p[j])-vec(p[i]);
                for(size_t k = j+1; k < p.size(); ++k)
                {
                    plane.normal = pij.Cross(vec(p[k])-vec(p[i]));
                    float lenSq = plane.normal.LengthSq();
                    if (lenSq > 1e-8f)
                    {
                        plane.normal /= sqrt(lenSq);
                        plane.d = plane.normal.Dot(vec(p[i]));

                        return plane;
                    }
                }
            }

        // Polygon contains multiple points, but they are all collinear.
        // Pick an arbitrary plane along the line as the polygon plane (as if the polygon had only two points)
        vec dir = (vec(p[1])-vec(p[0])).Normalized();
        vec norm = dir.Perpendicular();
        vec perpNormal = norm - norm.ProjectToNorm(dir);
        return Plane(p[0], perpNormal.Perpendicular());
    }
    if (p.size() == 3)
        return Plane(p[0], p[1], p[2]);
    if (p.size() == 2)
    {
        vec dir = (vec(p[1])-vec(p[0])).Normalized();
        vec norm = dir.Perpendicular();
        vec perpNormal = norm - norm.ProjectToNorm(dir);
        return Plane(p[0], perpNormal.Perpendicular());
    }
    if (p.size() == 1)
        return Plane(p[0], float3(0,1,0));
    return Plane();
}

void Polygon3D::DrawWithoutTransform(Camera &cam)
{
    cam.sImage.FillPolygon(points.toStdVector(), GetColor());
}


QGraphicsItemGroup *Polygon3D::DrawOnCameraView(Camera &cam)
{
    MatrixF pr = cam.ProjectOnScreen(this);
    Polygon3D prPol;
    prPol.FromMatrix(pr);
    if (calcColor)
        prPol.SetColor(NormToColor());
    else
        prPol.SetColor(color);

    prPol.DrawWithoutTransform(cam);

    return new QGraphicsItemGroup();
}

Matrix<float> Polygon3D::ToMatrix()
{
    MatrixF res(NumVertex(),4);
    for (int i = 0; i < NumVertex(); ++i) {
        memcpy(res.at(i), points[i].ToVector4().data(), 4 * sizeof (float));
    }
    return res;
}

void Polygon3D::FromMatrix(MatrixF m)
{
    points.clear();
    m = m.NormalizedW();
    for (u32 row = 0; row < m.RowCount(); ++row) {
        points << float3(m.at(row));
    }
    RecalcCenter();
    RecalcPlane();
}

void Polygon3D::Normalize()
{
}
