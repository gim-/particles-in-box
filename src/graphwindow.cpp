#include "graphwindow.h"
#include "ui_graphwindow.h"
#include <QDebug>

GraphWindow::GraphWindow(QString xlabel, QString ylabel, QWidget *parent):
    QWidget(parent),
    ui(new Ui::GraphWindow) {
    ui->setupUi(this);

    heightBars = new QCPBars(ui->graph->xAxis, ui->graph->yAxis);
    ui->graph->setGeometry(0,0, this->width(), this->height());
    ui->graph->addPlottable(heightBars);
    ui->graph->xAxis->setLabel(xlabel);
    ui->graph->yAxis->setLabel(ylabel);
    connect(&rangeTimer, SIGNAL(timeout()), this, SLOT(rangeCheck()));

    heightBars->setName("Height distribution");
}

GraphWindow::GraphWindow(QWidget *parent) :
    GraphWindow("", "", parent)
{
}

GraphWindow::~GraphWindow()
{
    qDebug()<<"GraphWindow destroyed";
    delete ui;
}

void GraphWindow::updateHeightGraph(const QVector<double>* data)
{
    QVector<double> heights(data->size());
    int heightsCount = 0;

    for (int i = 0; i<heights.size(); i++)
        heights[i] = ++heightsCount;

    maxYHeightGraph = *std::max_element(data->begin(), data->end());
    if (maxYHeightGraph>ui->graph->yAxis->range().upper)
    {
        ui->graph->yAxis->setRange(0.0, maxYHeightGraph);
    }

    ui->graph->xAxis->setRange(1, heightsCount);
    heightBars->setData(heights, *data);
    ui->graph->replot();
}

void GraphWindow::updateParticleDistGraph(const QVector<double>* data)
{
    QVector<double> heights(data->size());
    int heightsCount = 0;

    for (int i = 0; i<heights.size(); i++)
        heights[i] = ++heightsCount;

    maxYHeightGraph = *std::max_element(data->begin(), data->end());
    if (maxYHeightGraph>ui->graph->yAxis->range().upper)
    {
        ui->graph->yAxis->setRange(0.0, maxYHeightGraph);
    }

    ui->graph->xAxis->setRange(1, heightsCount);
    heightBars->setData(heights, *data);
    ui->graph->replot();
}

void GraphWindow::updateMaxwellDistGraph(const QVector<double> *data)
{
    QVector<double> heights(data->size());

    int heightsCount = 0;
    for (int i = 0; i<heights.size(); i++)
        heights[i] = ++heightsCount;

    maxYHeightGraph = *std::max_element(data->begin(), data->end());
    if (maxYHeightGraph>ui->graph->yAxis->range().upper)
    {
        ui->graph->yAxis->setRange(0.0, maxYHeightGraph);
        if (!rangeTimer.isActive())
        {
            rangeTimer.start(1100);
        }
    }

    ui->graph->xAxis->setRange(0, 40);
    heightBars->setData(heights, *data);
    ui->graph->replot();
}

void GraphWindow::rangeCheck()
{
    if (ui->graph->yAxis->range().upper>maxYHeightGraph)
    {
        ui->graph->yAxis->setRange(0.0, maxYHeightGraph+70);
    }
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
