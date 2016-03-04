#include "demonstrationwindow.h"
#include "particlewidget.h"
#include "ui_DemonstrationWindow.h"

DemonstrationWindow::DemonstrationWindow(QString simulationFile, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DemonstrationWindow) {
    ui->setupUi(this);
    mWorld = new World(simulationFile);
    ui->canvas->initializeWorld(*mWorld->getGeometry(), mWorld->getParticles());
    ui->sliderState->setMaximum(mWorld->getStateCount());
}

DemonstrationWindow::~DemonstrationWindow() {
    delete ui;
    delete mWorld;
}
