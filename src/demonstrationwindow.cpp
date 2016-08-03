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

    ui->plotMaxwell->xAxis->grid()->setPen(QPen(QColor(0, 0, 0, 0)));
    ui->plotMaxwell->yAxis->grid()->setPen(QPen(QColor(0, 0, 0, 0)));

    ui->plotBoltzmann->xAxis->setLabel("Height, m");
    ui->plotBoltzmann->yAxis->setLabel("Particle density");
    mDataBoltzmann = new QCPBars(ui->plotBoltzmann->xAxis, ui->plotBoltzmann->yAxis);
    ui->plotBoltzmann->addPlottable(mDataBoltzmann);

    ui->plotBoltzmann->xAxis->grid()->setPen(QPen(QColor(0, 0, 0, 0)));
    ui->plotBoltzmann->yAxis->grid()->setPen(QPen(QColor(0, 0, 0, 0)));

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

void DemonstrationWindow::updateBoltzmannPlot(QVector<SParticle> particles, int numBins) {
    QVector<double> x(numBins), y(numBins);
    double hMin = std::numeric_limits<double>::max();
    double hMax = std::numeric_limits<double>::min();
    double yMin = std::numeric_limits<double>::max();
    double yMax = std::numeric_limits<double>::min();
    foreach (SParticle particle, particles) {
        hMin = qMin(hMin, particle.y);
        hMax = qMax(hMax, particle.y);
    }
    double dH = hMax / numBins;
    for (quint8 i = 0; i < numBins; i++) {
        x[i] = dH * (i + 1);
    }

    foreach (SParticle particle, particles) {
        quint8 chunk = int((particle.y - dH / 2.0) / dH);
        y[chunk]++;
    }


    for (quint8 i = 0; i < numBins; i++) {
        y[i] = y[i] / (double)particles.size();
    }

    foreach (double val, y) {
        yMin = qMin(yMin, val);
        yMax = qMax(yMax, val);
    }

    mDataBoltzmann->setWidth(dH);
    mDataBoltzmann->setData(x, y);
    ui->plotBoltzmann->rescaleAxes();
    ui->plotBoltzmann->replot();
}

void DemonstrationWindow::updateMaxwellPlot(QVector<SParticle> particles, int numBins) {
    QVector<double> x(numBins), y(numBins);
    double vMin = std::numeric_limits<double>::max();
    double vMax = std::numeric_limits<double>::min();
    double yMin = std::numeric_limits<double>::max();
    double yMax = std::numeric_limits<double>::min();
    foreach (SParticle particle, particles) {
        double velocity = particleVelocity(particle);
        vMin = qMin(vMin, velocity);
        vMax = qMax(vMax, velocity);
    }
    double dV = vMax / numBins;
    for (quint8 i = 0; i < numBins; i++) {
        x[i] = dV * (i + 1);
    }

    foreach (SParticle particle, particles) {
        double velocity = particleVelocity(particle);
        quint8 chunk = int((velocity - dV / 2.0) / dV);
        y[chunk]++;
    }


    for (quint8 i = 0; i < numBins; i++) {
        y[i] = y[i] / (double)particles.size();
    }

    foreach (double val, y) {
        yMin = qMin(yMin, val);
        yMax = qMax(yMax, val);
    }

    mDataMaxwell->setWidth(dV);
    mDataMaxwell->setData(x, y);

    double vProbable = x[y.indexOf(yMax)];
    double k = 4 / sqrt(M_PI) * pow(1.0 / vProbable, 3.0);
    for (quint8 i = 0; i < numBins; i++) {
        y[i] = k * pow(x[i], 2.0) * exp(-pow(x[i], 2.0) / pow(vProbable, 2.0));
    }
    mDataMaxwellTheoretical->setData(x, y);
    ui->plotMaxwell->rescaleAxes();
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
    if (particles.size() != 0) {
        updateBoltzmannPlot(particles, ui->boltzmannNumBins->value());
        updateMaxwellPlot(particles, ui->maxwellNumBins->value());
    }
}

void DemonstrationWindow::on_buttonPlay_released() {
    if (mActive) {
        pause();
    }
    else {
        start();
    }
}


void DemonstrationWindow::on_buttonExportMaxwell_released()
{
    QString directoryScreenshots = "Screenshots";
    QFileInfo fi(mWorld->getFileName());
    QDir dir(fi.absolutePath() + "/" + directoryScreenshots);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    QString pngName = fi.baseName() + "_maxwell_" + QString::number(mWorld->getTime()) + ".png";
    ui->plotMaxwell->savePng(dir.absolutePath() + "/" + pngName,
                             ui->maxwellWidth->value(),
                             ui->maxwellHeight->value());
}

void DemonstrationWindow::on_buttonExportBoltzmann_released()
{
    QString directoryScreenshots = "Screenshots";
    QFileInfo fi(mWorld->getFileName());
    QDir dir(fi.absolutePath() + "/" + directoryScreenshots);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    QString pngName = fi.baseName() + "_boltzmann_" + QString::number(mWorld->getTime()) + ".png";
    ui->plotBoltzmann->savePng(dir.absolutePath() + "/" + pngName,
                             ui->boltzmannWidth->value(),
                             ui->boltzmannHeight->value());
}
