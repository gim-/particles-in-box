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

#include "glwidget.h"
#include "World.h"
#include <cmath>

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
}

void GLWidget::initializeGL()
{
    setFormat(QGLFormat(QGL::DoubleBuffer));
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

}

void GLWidget::resizeGL(int width, int height)
{
   height = height?height:1;
    glViewport( 0, 0, (GLint)width, (GLint)height );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //аgluPerspective(45.0f,(GLfloat)width/(GLfloat)height, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


}

void GLWidget::clearGL() {

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLWidget::paintGL()
{

    this->clearGL();
    glLoadIdentity();


    const double sin45 = 0.70710678118654752440084436210485d;
    const double cos45 = sin45;
    SGeometry *pGeometry = &mGeometry;
    double R = pGeometry->Rparticle;
    double Rsin45 = R * sin45;
    double Rcos45 = R * cos45;

    for (int i = 0; i < m_pWorld->GetParticleCount(); i++)
    {
        SParticle *pParticle = m_pWorld->GetParticle(i);
        if (pParticle->color)
            glColor3ubv(m_pWorld->getRightColor());
        else
            glColor3ubv(m_pWorld->getLeftColor());


        // TODO: use display  lists here for speed
        glBegin(GL_TRIANGLE_FAN);
            glVertex2d(pParticle->x+0, pParticle->y+0);
            glVertex2d(pParticle->x+R, pParticle->y+0);
            glVertex2d(pParticle->x+Rcos45, pParticle->y+Rsin45);
            glVertex2d(pParticle->x+0, pParticle->y+R);
            glVertex2d(pParticle->x-Rcos45, pParticle->y+Rsin45);
            glVertex2d(pParticle->x-R, pParticle->y+0);
            glVertex2d(pParticle->x-Rcos45, pParticle->y-Rsin45);
            glVertex2d(pParticle->x+0, pParticle->y-R);
            glVertex2d(pParticle->x+Rcos45, pParticle->y-Rsin45);
            glVertex2d(pParticle->x+R, pParticle->y+0);
        glEnd();
    }

    glColor3f(1,1,1);
    glLineWidth(2);
    glBegin(GL_LINE_STRIP);
        glVertex2d(pGeometry->WLeft, pGeometry->Ymax);
        glVertex2d(pGeometry->WLeft, pGeometry->YGapTop);
        glVertex2d(pGeometry->WRight, pGeometry->YGapTop);
        glVertex2d(pGeometry->WRight, pGeometry->Ymax);
    glEnd();
    glBegin(GL_LINE_STRIP);
        glVertex2d(pGeometry->WLeft, pGeometry->Ymin);
        glVertex2d(pGeometry->WLeft, pGeometry->YGapBottom);
        glVertex2d(pGeometry->WRight, pGeometry->YGapBottom);
        glVertex2d(pGeometry->WRight, pGeometry->Ymin);
    glEnd();
}

void GLWidget::timeOut()
{

}

void GLWidget::changeCoord()
{
    updateGL();
}

//virtual
double GLWidget::GetWorldSizeX()
{
    return (m_pWorld->GetGeometry()->Xright - m_pWorld->GetGeometry()->Xleft);
}

//virtual
double GLWidget::GetWorldSizeY()
{
    return (m_pWorld->GetGeometry()->Ymax - m_pWorld->GetGeometry()->Ymin);
}

//virtual
double GLWidget::GetDefaultWorldCenterX()
{
    return (m_pWorld->GetGeometry()->Xright + m_pWorld->GetGeometry()->Xleft)/2;
}

//virtual
double GLWidget::GetDefaultWorldCenterY()
{
    return (m_pWorld->GetGeometry()->Ymax + m_pWorld->GetGeometry()->Ymin)/2;
}

//virtual
void GLWidget::OnRenderScene(SGeometry geometry)
{
    mGeometry = geometry;
    paintGL();
    updateGL();
}


void GLWidget::initializeWorld(CWorld* world) {
    this->m_pWorld = world;
    paintGL();
    updateGL();
    return;
}
