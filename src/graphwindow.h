#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QWidget>
#include <QTimer>
//#include "World.h"
#include "qcustomplot.h"

namespace Ui {
class GraphWindow;
}

class GraphWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GraphWindow(QWidget *parent = nullptr);
    explicit GraphWindow(QString xlabel, QString ylabel, QWidget *parent = nullptr);
    ~GraphWindow();
protected:
    void resizeEvent(QResizeEvent* event);

private slots:
    void updateHeightGraph(const QVector<double> *data);
    void updateMaxwellDistGraph(const QVector<double> *data);
    void rangeCheck();
    void closeEvent(QCloseEvent *);
private:
    Ui::GraphWindow *ui;
    QCPBars* heightBars = nullptr;
    QTimer rangeTimer;
    double maxYHeightGraph;
};

#endif // GRAPHWINDOW_H
