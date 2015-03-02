#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QWidget>
#include "World.h"
#include "qcustomplot.h"

namespace Ui {
class GraphWindow;
}

class GraphWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GraphWindow(QWidget *parent = nullptr);
    ~GraphWindow();
protected:
    void resizeEvent(QResizeEvent* event);

private slots:
    void updateHeightGraph(const QVector<double> *data) const;
    void updateMaxwellDistGraph(const QVector<double> *data) const;
    void closeEvent(QCloseEvent *);
private:
    Ui::GraphWindow *ui;
    QCPBars* heightBars = nullptr;
};

#endif // GRAPHWINDOW_H
