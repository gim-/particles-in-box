#include "newexperimentwindow.h"
#include "ui_NewExperimentWindow.h"
#include "generator.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include <QThread>

NewExperimentWindow::NewExperimentWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewExperimentWindow) {
    ui->setupUi(this);
    ui->labelClearInputFile->setVisible(false);
    mLabelInputFileText = ui->labelInputFile->text();
    QFileInfo fi("pib_simulation.bin");
    if (fi.exists()) {
        for (unsigned short int i = 0; i < 0xFFF; i ++) {
            fi.setFile(QString("pib_simulation(%1).bin").arg(QString::number(i)));
            if (!fi.exists()) {
                break;
            }
        }
    }
    ui->outputFile->setText(fi.absoluteFilePath());
}

NewExperimentWindow::~NewExperimentWindow() {
    delete ui;
}

void NewExperimentWindow::setInputFile(const QString& newFile) {
    if ((!newFile.isNull()) && !(newFile.isEmpty())) {
        QFileInfo fi(newFile);
        mInputFileName = newFile;
        ui->labelInputFile->setText("Opening file: " + fi.fileName());
        ui->labelClearInputFile->setVisible(true);
        ui->boxWidth->setEnabled(false);
        ui->boxHeight->setEnabled(false);
        ui->deltaVTop->setEnabled(false);
        ui->deltaVBottom->setEnabled(false);
        ui->deltaVSide->setEnabled(false);
        ui->barrierX->setEnabled(false);
        ui->barrierWidth->setEnabled(false);
        ui->holeY->setEnabled(false);
        ui->holeHeight->setEnabled(false);
        ui->vLoss->setEnabled(false);
        ui->particleR->setEnabled(false);
        ui->g->setEnabled(false);
        ui->nLeft->setEnabled(false);
        ui->nRight->setEnabled(false);
        ui->vInit->setEnabled(false);
        ui->fps->setEnabled(false);
        ui->simulationTime->setEnabled(false);
        ui->outputFile->setEnabled(false);
        ui->buttonOutputFile->setEnabled(false);
    }
    else {
        clearInputFile();
    }
}

void NewExperimentWindow::clearInputFile() {
    mInputFileName = "";
    ui->labelInputFile->setText(mLabelInputFileText);
    ui->labelClearInputFile->setVisible(false);
    ui->boxWidth->setEnabled(true);
    ui->boxHeight->setEnabled(true);
    ui->deltaVTop->setEnabled(true);
    ui->deltaVBottom->setEnabled(true);
    ui->deltaVSide->setEnabled(true);
    ui->barrierX->setEnabled(true);
    ui->barrierWidth->setEnabled(true);
    ui->holeY->setEnabled(true);
    ui->holeHeight->setEnabled(true);
    ui->vLoss->setEnabled(true);
    ui->particleR->setEnabled(true);
    ui->g->setEnabled(true);
    ui->nLeft->setEnabled(true);
    ui->nRight->setEnabled(true);
    ui->vInit->setEnabled(true);
    ui->fps->setEnabled(true);
    ui->simulationTime->setEnabled(true);
    ui->outputFile->setEnabled(true);
    ui->buttonOutputFile->setEnabled(true);

}

void NewExperimentWindow::on_buttonRun_released() {
    if (mInputFileName.isEmpty() || mInputFileName.isNull()) {
        QFile outputFile(ui->outputFile->text());
        if (outputFile.open(QIODevice::WriteOnly)) {
            outputFile.close();
            int nLeftParticles = ui->nLeft->value();
            int nRightParticles = ui->nRight->value();
            double rParticle = ui->particleR->value();
            double vInit = ui->vInit->value();
            double vLoss = ui->vLoss->value();
            double boxWidth = ui->boxWidth->value();
            double boxHeight = ui->boxHeight->value();
            double barrierX = ui->barrierX->value();
            double barrierWidth = ui->barrierWidth->value();
            double holeY = ui->holeY->value();
            double holeHeight = ui->holeHeight->value();
            double deltaVTop = ui->deltaVTop->value();
            double deltaVBottom = ui->deltaVBottom->value();
            double deltaVSide = ui->deltaVSide->value();
            double g = ui->g->value();
            double minToSimulate = (double)ui->simulationTime->value();
            double fps = (double)ui->fps->value();


            QProgressDialog* progressDialog = new QProgressDialog(tr("Simulating..."),
                                                                  tr("Cancel"),
                                                                  0, fps * minToSimulate, this);
            progressDialog->show();

            Generator* generator = new Generator(nLeftParticles, nRightParticles, rParticle,
                                                 vInit, vLoss, boxWidth, boxHeight, barrierX,
                                                 barrierWidth, holeY, holeHeight, deltaVTop,
                                                 deltaVBottom, deltaVSide, g, minToSimulate,
                                                 fps, outputFile.fileName().toStdString());
            QThread *worker = new QThread(this);
            generator->moveToThread(worker);
            connect(generator, &Generator::onSimulationProgress, progressDialog, &QProgressDialog::setValue);
            connect(generator, &Generator::onSimulationFinished, progressDialog, &QProgressDialog::cancel);
            connect(progressDialog, &QProgressDialog::canceled, worker, &QThread::terminate);

            this->hide();

            generator->simulate();

        }
        else {
            QMessageBox::critical(this, tr("Error!"), QString("Cannot open file %1").arg(outputFile.fileName()));
        }
    }
    else {
        // TODO: play from file
    }

}

void NewExperimentWindow::on_buttonOutputFile_released() {
    QString outputFile = QFileDialog::getSaveFileName(this, tr("Save To:"), "", tr("Simulation reports (*.bin)"));
    if (!outputFile.isEmpty() && !outputFile.isNull()) {
        ui->outputFile->setText(outputFile);
    }
}

void NewExperimentWindow::on_labelInputFile_clicked() {
    QString newFile = QFileDialog::getOpenFileName(this, tr("Open File:"), "",
                                                   tr("Simulation reports (*.bin)"));
    setInputFile(newFile);
}

void NewExperimentWindow::on_labelClearInputFile_clicked() {
    clearInputFile();
}

