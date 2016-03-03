#ifndef PARTICLEWIDGET_H
#define PARTICLEWIDGET_H

#include <QOpenGLWidget>
#include <GL/glu.h>
#include "generator.h"


class ParticleWidget : public QOpenGLWidget {
        Q_OBJECT
private:
    SGeometry mBoxGeometry;
public:
    ParticleWidget(QWidget *parent = 0);
    void clearGL();
    void setBoxGeometry(const SGeometry& newGeometry);

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

};

#endif // PARTICLEWIDGET_H
