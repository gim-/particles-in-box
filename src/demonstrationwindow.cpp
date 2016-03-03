#include "demonstrationwindow.h"
#include "ui_DemonstrationWindow.h"

DemonstrationWindow::DemonstrationWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DemonstrationWindow)
{
    ui->setupUi(this);
}

DemonstrationWindow::~DemonstrationWindow()
{
    delete ui;
}
