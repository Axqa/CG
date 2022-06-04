#include "sceneimage.h"

SceneImage::SceneImage(QObject *parent) : QObject(parent)
{
    init();
}

SceneImage::~SceneImage()
{
    if (zValues)
        delete  zValues;
    if (aValues)
        delete aValues;
}

void SceneImage::init()
{
    image = QImage(width,height,QImage::Format_ARGB32);
    Clear();
}

void SceneImage::Clear()
{
    image.fill(bgColor);
    if (zValues) {
        delete  zValues;
    }
    zValues = new float[height*width];
    if (aValues)
        delete aValues;
    aValues = new float[height*width];
    for (int i = 0; i < height*width; ++i) {
        zValues[i] = -FLOAT_INF;
        aValues[i] = 0;
    }
}

void SceneImage::DrawLine(vec p1, vec p2, QColor col, float width)
{
    float2 lt = float2(rect.left(), rect.top());
    float2 rb = float2(rect.right(), rect.bottom());

//    qDebug() << "clipping" << p1 << p2 << "in rect" << lt << rb;

    if (Line3DClipping(p1,p2, lt, rb)) {
//        qDebug() << "success clipping" << p1 << p2;
        int     x0 = (p1.x - lt.x) / rect.width() * this->width,
                x1 = (p2.x- lt.x) / rect.width() * this->width,
                y0 = -(p1.y- rb.y) / rect.height() * this->height,
                y1 = -(p2.y- rb.y) / rect.height() * this->height;
        float   wd = __max(1, width * sqrt((this->width*this->width + this->height*this->height) /
                                  (rect.width()*rect.width() + rect.height()*rect.height()) ) );
        float z0 = p1.z, z1 = p2.z;

//        PlotLineWidth(x0, y0, z0, x1, y1, z1, col, wd);
        PlotLineAA(x0, y0, z0, x1, y1, z1, col);
//        PlotThickLine(x0, y0, z0, x1, y1, z1, col, wd)  ;
//        PlotThickLine2(x0, y0, z0, x1, y1, z1, col, wd)  ;
    }
}

void SceneImage::DrawCircle(int xm, int ym, float z, float r, QColor color)
{
    int x = -r, y = 0, err = 2-2*r; /* II. Quadrant */
       do {
          PutPixel(xm-x, ym+y, color, z); /*   I. Quadrant */
          PutPixel(xm-y, ym-x, color, z); /*  II. Quadrant */
          PutPixel(xm+x, ym-y, color, z); /* III. Quadrant */
          PutPixel(xm+y, ym+x, color, z); /*  IV. Quadrant */
          r = err;
          if (r <= y) err += ++y*2+1;           /* e_xy+e_y < 0 */
          if (r > x || err > y) err += ++x*2+1; /* e_xy+e_x > 0 or no 2nd y-step */
    } while (x < 0);
}

void SceneImage::DrawSphere(float3 center, float r, QColor color)
{
    float2 lt = float2(rect.left(), rect.top());
    float2 rb = float2(rect.right(), rect.bottom());
    int     x = (center.x - lt.x) / rect.width() * this->width,
            y = -(center.y- rb.y) / rect.height() * this->height;

    float rad = __max(1, r * sqrt((float)(this->width*this->width + this->height*this->height) /
                                     (rect.width()*rect.width() + rect.height()*rect.height()) ) );

    PutPixel(x,y,color, center.z+r);
    for (float i = 1; i <= rad; i+=0.5) {
        DrawCircle(x,y,center.z + r * (rad-i)/rad, i, color);
    }

}

void SceneImage::PlotLineAA(int x0, int y0, float z0, int x1, int y1, float z1, QColor color)
{
   int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
   int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
   int err = dx-dy, e2, x2;                       /* error value e_xy */
   int ed = dx+dy == 0 ? 1 : sqrt((float)dx*dx+(float)dy*dy);

   float dz = dx+dy == 0 ? 0 : (z1-z0) / (dx+dy);

   for ( ; ; ){                                         /* pixel loop */
      color.setAlpha(255 - 255*abs(err-dx+dy)/ed);
      PutPixel(x0,y0,color, z0);
      e2 = err; x2 = x0;
      if (2*e2 >= -dx) {                                    /* x step */
         if (x0 == x1) break;
         if (e2+dy < ed) {
             color.setAlpha(255 - 255*(e2+dy)/ed);
             PutPixel(x0,y0+sy, color, z0);
         }
         err -= dy; x0 += sx; z0 += dz;
      }
      if (2*e2 <= dy) {                                     /* y step */
         if (y0 == y1) break;
         if (dx-e2 < ed) {
             color.setAlpha(255 - 255*(dx-e2)/ed);
             PutPixel(x2+sx,y0, color, z0);
         }
         err += dx; y0 += sy; z0 += dz;
    }
   }
}

void SceneImage::PlotLineWidth(int x0, int y0, float z0, int x1, int y1, float z1, QColor col, float wd)
{
    int dx = abs(x1-x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1-y0), sy = y0 < y1 ? 1 : -1;
    float dz = dx+dy == 0 ? 0 : (z1-z0) / (dx+dy);
    int err = dx-dy, e2, x2, y2;                          /* error value e_xy */
    float ed = dx+dy == 0 ? 1 : sqrt((float)dx*dx+(float)dy*dy);

    for (wd = (wd+1)/2; ; ) {                                   /* pixel loop */
        col.setAlpha(255-fmax(0,255*(abs(err-dx+dy)/ed-wd+1)));
        PutPixel(x0,y0,col,z0);
        e2 = err; x2 = x0;
        if (2*e2 >= -dx) {                                           /* x step */
            for (e2 += dy, y2 = y0; e2 < ed*wd && (y1 != y2 || dx > dy); e2 += dx) {
                col.setAlpha(255-fmax(0,255*(abs(e2)/ed-wd+1)));
                PutPixel(x0, y2 += sy, col, z0);
            }
            if (x0 == x1) break;
            e2 = err; err -= dy; x0 += sx; z0 += dz;
        }
        if (2*e2 <= dy) {                                            /* y step */
            for (e2 = dx-e2; e2 < ed*wd && (x1 != x2 || dx < dy); e2 += dy) {
                col.setAlpha(255-fmax(0,255*(abs(e2)/ed-wd+1)));
                PutPixel(x2 += sx, y0, col, z0);
            }
            if (y0 == y1) break;
            err += dx; y0 += sy; z0 += dz ;
        }
    }

}

void SceneImage::PlotThickLine(int x0, int y0, float z0, int x1, int y1, float z1, QColor color, float thickness)
{
    float wy,wx;
    int i;
//    qDebug() << x0<< y0<< z0<< x1<< y1<<z1;
    PlotLine(x0, y0, z0, x1, y1, z1, color);
    if(abs(y1-y0)<abs(x1-x0))
    {
        wy=(thickness-1) * sqrt(pow((x1-x0),2)+pow((y1-y0),2))/(2* __max(1,fabs(x1-x0)));
        for(i=0;i<wy;i++)
        {
            PlotLineAA(x0, y0-i, z0, x1, y1-i, z1, color);
            PlotLineAA(x0, y0+i, z0, x1, y1+i, z1, color);
        }
    }
    else
    {
        wx = (thickness - 1) * sqrt((x1 - x0) * (x1 - x0) + (y1-y0) * (y1-y0)) / (float)(2 * __max(1,abs(y1 - y0)));

        for(i=0;i<wx;i++)
        {
            PlotLineAA(x0-i, y0, z0, x1-i, y1, z1, color);
            PlotLineAA(x0+i, y0, z0, x1+i, y1, z1, color);
        }
    }
}

void SceneImage::PlotLine(int x0, int y0, float z0, int x1, int y1, float z1, QColor color)
{
    /// doesn't work :(

   int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
   int dy =  abs(y1-y0), sy = y0<y1 ? 1 : -1;
   int err = dx+dy, e2; /* error value e_xy */
   float dz = dx+dy == 0 ? 0 : (z1-z0) / (dx+dy);

   for(;;){  /* loop */
      PutPixel(x0,y0,color,z0);
      if (x0==x1 && y0==y1) break;
      e2 = 2*err;
      if (e2 >= dy) { err += dy; x0 += sx; z0+=dz; } /* e_xy+e_x > 0 */
      if (e2 >= dx) { err += dx; y0 += sy; z0+=dz; } /* e_xy+e_y < 0 */
   }

}

void SceneImage::PlotThickLine2(int x0, int y0, float z0, int x1, int y1, float z1, QColor color, float thickness)
{
//    qDebug() << x0<< y0<< z0<< x1<< y1<<z1;
    PlotLineAA(x0, y0, z0, x1, y1, z1, color);
    float2 vec = (float2(x1,y1) - float2(x0,y0)).Perp().ScaledToLength(thickness/2);

//    int w = (thickness-1) * sqrt((x1 - x0) * (x1 - x0) + (y1-y0) * (y1-y0)) * (float)(2 * __max(1,__max(abs(x1-x0),abs(y1 - y0))));
    float w = __max(abs(vec.x), abs(vec.y));
//    qDebug() << w;
    for (int i = 1; i <= w; ++i) {
        vec.ScaleToLength(w/i);
        PlotLineAA(x0 + vec.x, y0 + vec.y, z0, x1 + vec.x, y1+vec.y, z1, color);
        PlotLineAA(x0 - vec.x, y0 - vec.y, z0, x1 - vec.x, y1-vec.y, z1, color);
    }
}

// Defining region codes
const u8 INSIDE = 0; // 0000
const u8 LEFT = 1; // 0001
const u8 RIGHT = 2; // 0010
const u8 BOTTOM = 4; // 0100
const u8 TOP = 8; // 1000
u8 ComputeCode(float2 p, float2 &lt, float2 &rb) {
    u8 code = INSIDE;
    if (p.x < lt.x) // to the left of rectangle
        code |= LEFT;
    else if (p.x > rb.x) // to the right of rectangle
        code |= RIGHT;
    if (p.y > rb.y) // below the rectangle
        code |= BOTTOM;
    else if (p.y < lt.y) // above the rectangle
        code |= TOP;

    return code;
}


bool SceneImage::Line3DClipping(vec &p1, vec &p2, float2 &lt, float2 &rb)
{
    u8 code1 = ComputeCode(float2{p1.x, p1.y}, lt, rb);
    u8 code2 = ComputeCode(float2{p2.x, p2.y}, lt, rb);

    // Initialize line as outside the rectangular window
        bool accept = false;

        while (true) {
            if ((code1 == 0) && (code2 == 0)) {
                // If both endpoints lie within rectangle
                accept = true;
                break;
            }
            else if (code1 & code2) {
                // If both endpoints are outside rectangle,
                // in same region
                break;
            }
            else {
                // Some segment of line lies within the
                // rectangle
                int code_out;
                float x, y, z;

                // At least one endpoint is outside the
                // rectangle, pick it.
                if (code1 != 0)
                    code_out = code1;
                else
                    code_out = code2;

                // Find intersection point;
                // using formulas y = y1 + slope * (x - x1),
                // x = x1 + (1 / slope) * (y - y1)
                // z = z1 + (1 / slope) * [(y - y1) or (x-x1)]
                if (code_out & TOP) {
                    // point is above the clip rectangle
                    x = p1.x + (p2.x - p1.x) * (lt.y - p1.y) / (p2.y - p1.y);
                    y = lt.y;
                    z = p1.z + (p2.z - p1.z) * (lt.y - p1.y) / (p2.y - p1.y);
                }
                else if (code_out & BOTTOM) {
                    // point is below the rectangle
                    x = p1.x + (p2.x - p1.x) * (rb.y - p1.y) / (p2.y - p1.y);
                    y = rb.y;
                    z = p1.z + (p2.z - p1.z) * (rb.y - p1.y) / (p2.y - p1.y);
                }
                else if (code_out & RIGHT) {
                    // point is to the right of rectangle
                    y = p1.y + (p2.y - p1.y) * (rb.x - p1.x) / (p2.x - p1.x);
                    x = rb.x;
                    z = p1.z + (p2.z - p1.z) * (rb.x - p1.x) / (p2.x - p1.x);
                }
                else if (code_out & LEFT) {
                    // point is to the left of rectangle
                    y = p1.y + (p2.y - p1.y) * (lt.x - p1.x) / (p2.x - p1.x);
                    x = lt.x;
                    z = p1.z + (p2.z - p1.z) * (lt.x - p1.x) / (p2.x - p1.x);
                }

                // Now intersection point x, y is found
                // We replace point outside rectangle
                // by intersection point
                if (code_out == code1) {
                    p1.x = x;
                    p1.y = y;
                    p1.z = z;
                    code1 = ComputeCode({p1.x, p1.y}, lt, rb);
                }
                else {
                    p2.x = x;
                    p2.y = y;
                    p2.z = z;
                    code2 = ComputeCode({p2.x, p2.y}, lt, rb);
                }
            }
        }
        return accept;
}

void SceneImage::PutPixel(int x, int y, QColor col, float z)
{

    if (x>=0 && x < width && y>=0 && y<height) {
        if (z > zValues[x + y*width]) {
            float alpha = col.alphaF();
            if (alpha < 1e-4) return;
            if ( alpha != 1) {
                float oldA = aValues[x + y*width];
                float curA = aValues[x + y*width] = alpha + oldA * (1-alpha);
                QColor old = image.pixelColor(x,y);
//                qDebug() << col << alpha << oldA;
                col = QColor((col.red()*alpha + old.red()*(1-alpha)*oldA)/curA,
                             (col.green()*alpha + old.green()*(1-alpha)*oldA)/curA,
                             (col.blue()*alpha + old.blue()*(1-alpha)*oldA)/curA);

            }
            image.setPixelColor(x,y,col);
            zValues[x + y*width] = z;
        }
    }
}

/// @todo maybe change it
QPointF SceneImage::MapToScene(QPoint p)
{
//    qDebug() << "mapping" << p << "from view" << viewRect << "to plane rect" << rect << "with top" << rect.top();
    QPointF res;
    res.setX(p.x() / (float)viewRect.width() * rect.width() + rect.left());
    res.setY(-(p.y() / (float)viewRect.height() * rect.height() + rect.top()));

    return res;
}

QPointF SceneImage::MapFromScene(QPointF p)
{
    QPointF res;

    res.setX((p.x() - rect.left()) / rect.width() * viewRect.width());
    res.setY((-p.y()- rect.top()) / rect.height() * viewRect.height());

//    qDebug() << "Map" << p << "from scene" << rect << "in" << res << "for view" << viewRect;

    return res;
}

void SceneImage::ChangeSceneRect(QRectF sceneRect, QRect viewRect)
{
    this->rect = sceneRect;
    this->viewRect = viewRect;
    qDebug() << "changing rect in image" << sceneRect << "view rect" << viewRect;
    UpdateImage();
}


