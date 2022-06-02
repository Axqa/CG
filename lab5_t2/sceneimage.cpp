#include "sceneimage.h"

SceneImage::SceneImage(QObject *parent) : QObject(parent)
{
    init();
}

SceneImage::~SceneImage()
{
    if (zValues)
        delete  zValues;
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
    for (int i = 0; i < height*width; ++i) {
        zValues[i] = -FLOAT_INF;
    }
}

void SceneImage::DrawLine(vec &p1, vec &p2, QColor &col, int width)
{
    float2 lt = float2(rect.left(), rect.top());
    float2 rb = float2(rect.right(), rect.bottom());

    qDebug() << "clipping" << p1 << p2 << "in rect" << lt << rb;

    if (Line3DClipping(p1,p2, lt, rb)) {
        qDebug() << "success clipping" << p1 << p2;
        int     x0 = (p1.x - lt.x) / rect.width() * this->width,
                x1 = (p2.x- lt.x) / rect.width() * this->width,
                y0 = -(p1.y- rb.y) / rect.height() * this->height,
                y1 = -(p2.y- rb.y) / rect.height() * this->height,
                wd = 1;
        qDebug() << x0<<y0 <<x1<<y1 << width << wd;
        float z0 = p1.z, z1 = p2.z;

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
                for (e2 += dy, y2 = y0; e2 < ed*wd && (/*y1 != y2 ||*/ dx > dy); e2 += dx) {
                    col.setAlpha(255-fmax(0,255*(abs(e2)/ed-wd+1)));
                    PutPixel(x0, y2 += sy, col, z0);
                }
                if (x0 == x1) break;
                e2 = err; err -= dy; x0 += sx; z0 += dz;
            }
            if (2*e2 <= dy) {                                            /* y step */
                for (e2 = dx-e2; e2 < ed*wd && (/*x1 != x2 ||*/ dx < dy); e2 += dy) {
                    col.setAlpha(255-fmax(0,255*(abs(e2)/ed-wd+1)));
                    PutPixel(x2 += sx, y0, col, z0);
                }
                if (y0 == y1) break;
                err += dx; y0 += sy; z0 += dz ;
            }
        }
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
    if (p.y < rb.y) // below the rectangle
        code |= BOTTOM;
    else if (p.y > lt.y) // above the rectangle
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
            int alpha = col.alpha();
            if ( alpha != 255) {
                QColor old = image.pixelColor(x,y);
                col = QColor(col.red()*alpha/255 + old.red()*(255-alpha)/255,
                             col.green()*alpha/255 + old.green()*(255-alpha)/255,
                             col.blue()*alpha/255 + old.blue()*(255-alpha)/255);

            }
            image.setPixelColor(x,y,col);
            zValues[x + y*width] = z;
        }
    }
}

void SceneImage::ChangeSceneRect(QRectF sceneRect, QRect viewRect)
{
    this->rect = sceneRect;
    this->viewRect = viewRect;
    qDebug() << "changing rect in image" << sceneRect << "view rect" << viewRect;
    UpdateImage();
}
