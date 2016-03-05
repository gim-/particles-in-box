#include "demonstrationwindow.h"
#include "particlewidget.h"
#include "ui_DemonstrationWindow.h"
#include <QTimer>
#include "qcustomplot/qcustomplot.h"

DemonstrationWindow::DemonstrationWindow(QString simulationFile, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DemonstrationWindow) {
    ui->setupUi(this);
    mWorld = new World(simulationFile);
    ui->canvas->initializeWorld(*mWorld->getGeometry(), mWorld->getParticles());
    ui->sliderState->setMaximum(mWorld->getStateCount());
    mTimer = new QTimer(this);
    mActive = true;
    connect(mTimer, &QTimer::timeout, this, &DemonstrationWindow::onTimerTimeout);
    connect(mWorld, &World::onStateChanged, ui->canvas, &ParticleWidget::setParticles);
    mTimer->setInterval((int)(1000.0 / ui->comboFps->value()));
    mTimer->start();
}

DemonstrationWindow::~DemonstrationWindow() {
    delete ui;
    delete mWorld;
}

void DemonstrationWindow::pause() {
    mTimer->stop();
    ui->buttonPlay->setText("▷");
    mActive = false;
}

void DemonstrationWindow::start() {
    mTimer->start();
    ui->buttonPlay->setText("▯▯");
    mActive = true;
}

void DemonstrationWindow::onTimerTimeout() {
    if (mActive) {
        ui->sliderState->setValue(ui->sliderState->value() + 1);
    }
}

void DemonstrationWindow::on_comboFps_valueChanged(int value) {
    mTimer->setInterval((int)(1000.0 / ui->comboFps->value()));
}

void DemonstrationWindow::on_sliderState_valueChanged(int value){
    mWorld->readParticlesState(value);
    ui->labelTime->setText(tr("Time elapsed: %1s").arg(mWorld->getTime(), 0, 'g', 5));
}

void DemonstrationWindow::on_buttonPlay_released() {
    if (mActive) {
        pause();
    }
    else {
        start();
    }

}
