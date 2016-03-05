#include "demonstrationwindow.h"
#include "particlewidget.h"
#include "ui_DemonstrationWindow.h"
#include <QTimer>
#include "qcustomplot/qcustomplot.h"
#include <limits>

DemonstrationWindow::DemonstrationWindow(QString simulationFile, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DemonstrationWindow) {
    ui->setupUi(this);
    mWorld = new World(simulationFile);
    ui->canvas->initializeWorld(*mWorld->getGeometry(), mWorld->getParticles());
    ui->sliderState->setMaximum(mWorld->getStateCount());

    ui->plotMaxwell->xAxis->setLabel("Velocity, m/s");
    ui->plotMaxwell->yAxis->setLabel("Particle density");
    mDataMaxwell = new QCPBars(ui->plotMaxwell->xAxis, ui->plotMaxwell->yAxis);
    mDataMaxwellTheoretical = new QCPCurve(ui->plotMaxwell->xAxis, ui->plotMaxwell->yAxis);
    ui->plotMaxwell->addPlottable(mDataMaxwell);
    ui->plotMaxwell->addPlottable(mDataMaxwellTheoretical);

    ui->plotBoltzmann->xAxis->setLabel("Height, m");
    ui->plotBoltzmann->yAxis->setLabel("Particle density");
    mDataBoltzmann = new QCPBars(ui->plotBoltzmann->xAxis, ui->plotBoltzmann->yAxis);
    ui->plotBoltzmann->addPlottable(mDataBoltzmann);


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
    delete mDataBoltzmann;
    delete mDataMaxwellTheoretical;
    delete mDataMaxwell;
}

void DemonstrationWindow::updateBoltzmannPlot(QVector<SParticle> particles) {
    QVector<double> x(15), y(15);
    double hMin = std::numeric_limits<double>::max();
    double hMax = std::numeric_limits<double>::min();
    double yMin = std::numeric_limits<double>::max();
    double yMax = std::numeric_limits<double>::min();
    foreach (SParticle particle, particles) {
        hMin = qMin(hMin, particle.y);
        hMax = qMax(hMax, particle.y);
    }
    double dH = hMax / 15.0;
    for (quint8 i = 0; i < 15; i++) {
        x[i] = dH * (i + 1);
    }

    foreach (SParticle particle, particles) {
        quint8 chunk = int((particle.y - dH / 2.0) / dH);
        y[chunk]++;
    }


    for (quint8 i = 0; i < 15; i++) {
        y[i] = y[i] / (double)particles.size();
    }

    foreach (double val, y) {
        yMin = qMin(yMin, val);
        yMax = qMax(yMax, val);
    }


    ui->plotBoltzmann->xAxis->setRange(0.0, ((int)(hMax / 5.0) + 1) * 5.0);
    ui->plotBoltzmann->yAxis->setRange(0.0, 1.0);
    mDataBoltzmann->setWidth(dH);
    mDataBoltzmann->setData(x, y);
    ui->plotBoltzmann->replot();
}

void DemonstrationWindow::updateMaxwellPlot(QVector<SParticle> particles) {
    QVector<double> x(15), y(15);
    double vMin = std::numeric_limits<double>::max();
    double vMax = std::numeric_limits<double>::min();
    double yMin = std::numeric_limits<double>::max();
    double yMax = std::numeric_limits<double>::min();
    foreach (SParticle particle, particles) {
        double velocity = particleVelocity(particle);
        vMin = qMin(vMin, velocity);
        vMax = qMax(vMax, velocity);
    }
    double dV = vMax / 15.0;
    for (quint8 i = 0; i < 15; i++) {
        x[i] = dV * (i + 1);
    }

    foreach (SParticle particle, particles) {
        double velocity = particleVelocity(particle);
        quint8 chunk = int((velocity - dV / 2.0) / dV);
        y[chunk]++;
    }


    for (quint8 i = 0; i < 15; i++) {
        y[i] = y[i] / (double)particles.size();
    }

    foreach (double val, y) {
        yMin = qMin(yMin, val);
        yMax = qMax(yMax, val);
    }

    ui->plotMaxwell->xAxis->setRange(0.0, ((int)(vMax / 5.0) + 1) * 5.0);
    ui->plotMaxwell->yAxis->setRange(0.0, 1.0);
    ui->plotMaxwell->rescaleAxes();
    mDataBoltzmann->setWidth(dV);
    mDataMaxwell->setData(x, y);

    double vProbable = x[y.indexOf(yMax)];
    double k = 4 / sqrt(M_PI) * pow(1.0 / vProbable, 3.0);
    for (quint8 i = 0; i < 15; i++) {
        y[i] = k * pow(x[i], 2.0) * exp(-pow(x[i], 2.0) / pow(vProbable, 2.0));
    }
    mDataMaxwellTheoretical->setData(x, y);
    ui->plotMaxwell->replot();

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
    QVector<SParticle> particles = mWorld->getParticles();
    updateBoltzmannPlot(particles);
    updateMaxwellPlot(particles);
}

void DemonstrationWindow::on_buttonPlay_released() {
    if (mActive) {
        pause();
    }
    else {
        start();
    }

}
