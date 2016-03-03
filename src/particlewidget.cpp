#include "particlewidget.h"

ParticleWidget::ParticleWidget(QWidget *parent) : QOpenGLWidget(parent) {

}


void ParticleWidget::setBoxGeometry(const SGeometry &newGeometry) {
    this->mBoxGeometry = newGeometry;
}


void ParticleWidget::clearGL() {
    glClearColor(0.05, 0.05, 0.05, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ParticleWidget::paintGL() {
    clearGL();
    glLoadIdentity();

    // TODO: do actual painting
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
