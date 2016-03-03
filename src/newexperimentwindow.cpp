#include "newexperimentwindow.h"
#include "ui_NewExperimentWindow.h"

NewExperimentWindow::NewExperimentWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewExperimentWindow)
{
    ui->setupUi(this);
}

NewExperimentWindow::~NewExperimentWindow()
{
    delete ui;
}
