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

#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QWidget>
#include <QTimer>
#include "qcustomplot.h"

namespace Ui {
class GraphWindow;
}

class GraphWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GraphWindow(QString xlabel = "", QString ylabel = "", QWidget *parent = nullptr);
    ~GraphWindow();
protected:
    void resizeEvent(QResizeEvent* event);

private slots:
    void updateHeightGraph(const QVector<double> *data);
    void updateMaxwellDistGraph(const QVector<double> *data);
    void updateParticleDistGraph(const QVector<double> *data);
    void rangeCheck();
    void closeEvent(QCloseEvent *);
private:
    Ui::GraphWindow *ui;
    QCPBars* heightBars = nullptr;
    QTimer rangeTimer;
    double maxYHeightGraph;
};

#endif // GRAPHWINDOW_H
