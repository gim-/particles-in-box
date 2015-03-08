/*Copyright (c) 2014 Gennady Gaidukov

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.*/

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
