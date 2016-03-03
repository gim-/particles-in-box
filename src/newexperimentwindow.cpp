#include "newexperimentwindow.h"
#include "ui_NewExperimentWindow.h"

NewExperimentWindow::NewExperimentWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewExperimentWindow) {
    ui->setupUi(this);
    ui->labelClearInputFile->setVisible(false);
}

NewExperimentWindow::~NewExperimentWindow() {
    delete ui;
}

void NewExperimentWindow::on_buttonRun_released() {

}

void NewExperimentWindow::on_buttonOutputFile_released() {

}

void NewExperimentWindow::on_labelInputFile_clicked() {
}

void NewExperimentWindow::on_labelClearInputFile_clicked() {

}
