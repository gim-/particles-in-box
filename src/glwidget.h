/*Copyright (c) 2014 Gennady Gaidukov

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.*/

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <GL/glu.h>
#include <QOpenGLPaintDevice>
#include <QDebug>
#include <QTimer>
#include "World.h"


class GLWidget : public QGLWidget
{
    friend class MainWindow;
    Q_OBJECT

private:
    CWorld* m_pWorld;
    SGeometry mGeometry;

public:
    explicit GLWidget(QWidget *parent = 0);
    void initializeWorld(CWorld* world);
    void clearGL();


protected:
    virtual void initializeGL();
    virtual void resizeGL(int width, int height);
    virtual void paintGL();
    virtual void timeOut();

    virtual double GetWorldSizeX();
    virtual double GetWorldSizeY();
    virtual double GetDefaultWorldCenterX();
    virtual double GetDefaultWorldCenterY();


signals:

private slots:
    void changeCoord();
    void OnRenderScene(SGeometry);

private:
    void drawParticle();
};

#endif // GLWIDGET_H
