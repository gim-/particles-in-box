#include "demonstrationwindow.h"
#include "particlewidget.h"
#include "ui_DemonstrationWindow.h"
#include <QTimer>

DemonstrationWindow::DemonstrationWindow(QString simulationFile, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DemonstrationWindow) {
    ui->setupUi(this);
    mWorld = new World(simulationFile);
    ui->canvas->initializeWorld(*mWorld->getGeometry(), mWorld->getParticles());
    ui->sliderState->setMaximum(mWorld->getStateCount());
    mTimer = new QTimer(this);
    connect(mTimer, &QTimer::timeout, this, &DemonstrationWindow::onTimerTimeout);
    connect(mWorld, &World::onStateChanged, ui->canvas, &ParticleWidget::setParticles);
}

DemonstrationWindow::~DemonstrationWindow() {
    delete ui;
    delete mWorld;
}

void DemonstrationWindow::onTimerTimeout() {
    mWorld->readParticlesState(ui->sliderState->value());
}

void DemonstrationWindow::on_comboFps_valueChanged(int value) {
}

void DemonstrationWindow::on_sliderState_valueChanged(int value){
    mWorld->readParticlesState(value);
}
