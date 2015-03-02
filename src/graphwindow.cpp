#include "graphwindow.h"
#include "ui_graphwindow.h"
#include <QDebug>

GraphWindow::GraphWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphWindow)
{
    ui->setupUi(this);

    heightBars = new QCPBars(ui->graph->xAxis, ui->graph->yAxis);
    ui->graph->setGeometry(0,0, this->width(), this->height());
    ui->graph->addPlottable(heightBars);
    heightBars->setName("Height distribution");
    ui->graph->xAxis->setLabel("Heights");
    ui->graph->yAxis->setLabel("Particles");
    ui->graph->yAxis->setRange(0.0, 300.0);
}

GraphWindow::~GraphWindow()
{
    qDebug()<<"GraphWindow destroyed";
    delete ui;
}

void GraphWindow::updateHeightGraph(const QVector<double>* data) const
{
    QVector<double> heights(data->size());
    //QVector<double> counts;
    int heightsCount = 0;
//    for (const int &i : *data) {
//        heights << heights.size() + 1;
//        counts << i;
//    }
    for (int i = 0; i<heights.size(); i++)
        heights[i] = ++heightsCount;
    ui->graph->xAxis->setRange(1, data->size());
    heightBars->setData(heights, *data);
    //heightBars->setData(heights,counts);
    ui->graph->replot();
}

void GraphWindow::updateMaxwellDistGraph(const QVector<double> *data) const
{

}

void GraphWindow::closeEvent(QCloseEvent *)
{
    this->deleteLater();
}

void GraphWindow::resizeEvent(QResizeEvent *event)
{
    ui->graph->setGeometry(0,0, this->width(), this->height());
    QWidget::resizeEvent(event);
}
