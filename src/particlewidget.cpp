#include "particlewidget.h"

ParticleWidget::ParticleWidget(QWidget *parent) : QOpenGLWidget(parent) {
    initializeGL();
}


void ParticleWidget::setBoxGeometry(const SGeometry &newGeometry) {
    this->mBoxGeometry = newGeometry;
    paintGL();
}

void ParticleWidget::setParticles(const QVector<SParticle> &newParticles) {
    this->mParticles = newParticles;
    paintGL();
}

void ParticleWidget::initializeWorld(SGeometry geometry, QVector<SParticle> particles) {
    this->mBoxGeometry = geometry;
    this->mParticles = particles;
    paintGL();
}


void ParticleWidget::clearGL() {
    glClearColor(0.05, 0.05, 0.05, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ParticleWidget::paintGL() {
    clearGL();
    glLoadIdentity();


    const double sin45 = 0.70710678118654752440084436210485d;
    const double cos45 = sin45;
    SGeometry *pGeometry = &mBoxGeometry;
    double R = pGeometry->rParticle;
    double Rsin45 = R * sin45;
    double Rcos45 = R * cos45;

    for (int i = 0; i < mParticles.size(); i++)
    {
        SParticle *pParticle = &mParticles[i];
        if (pParticle->color)
            glColor3ub(255, 0, 0);
        else
            glColor3ub(0, 255, 255);

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
        glVertex2d(pGeometry->wLeft, pGeometry->yMax);
        glVertex2d(pGeometry->wLeft, pGeometry->yGapTop);
        glVertex2d(pGeometry->wRight, pGeometry->yGapTop);
        glVertex2d(pGeometry->wRight, pGeometry->yMax);
    glEnd();
    glBegin(GL_LINE_STRIP);
        glVertex2d(pGeometry->wLeft, 0.0);
        glVertex2d(pGeometry->wLeft, pGeometry->yGapBottom);
        glVertex2d(pGeometry->wRight, pGeometry->yGapBottom);
        glVertex2d(pGeometry->wRight, 0.0);
    glEnd();
    doneCurrent();
}



void ParticleWidget::initializeGL() {
    glShadeModel(GL_SMOOTH);
    glClearColor(0.05, 0.05, 0.05, 1.0);
    glClearDepth(1.0);

    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void ParticleWidget::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);

    glOrtho(0.0, this->mBoxGeometry.xRight,
            0.0, this->mBoxGeometry.yMax,
            0.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
