#ifndef DEMONSTRATIONWINDOW_H
#define DEMONSTRATIONWINDOW_H

#include <QMainWindow>
#include "world.h"
#include <QVector>
#include <QString>

namespace Ui {
class DemonstrationWindow;
}

class DemonstrationWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit DemonstrationWindow(QString simulationFile, QWidget *parent = 0);
    ~DemonstrationWindow();

public slots:
    void onTimerTimeout();

private slots:
    void on_comboFps_valueChanged(int arg1);

    void on_sliderState_valueChanged(int value);

private:
    Ui::DemonstrationWindow *ui;
    World* mWorld;
    QTimer* mTimer;



};

#endif // DEMONSTRATIONWINDOW_H
