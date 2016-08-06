#ifndef DEMONSTRATIONWINDOW_H
#define DEMONSTRATIONWINDOW_H

#include <QMainWindow>
#include "world.h"
#include <QVector>
#include <QString>
#include "qcustomplot/qcustomplot.h"

namespace Ui {
class DemonstrationWindow;
}

class DemonstrationWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit DemonstrationWindow(QString simulationFile, QWidget *parent = 0);
    ~DemonstrationWindow();
    void pause();
    void start();

    void updateMaxwellPlot(QVector<SParticle> particles, int numBins);
    void updateBoltzmannPlot(QVector<SParticle> particles, int numBins);

public slots:
    void onTimerTimeout();

private slots:
    void on_comboFps_valueChanged(int arg1);

    void on_sliderState_valueChanged(int value);

    void on_buttonPlay_released();

    void on_buttonExportMaxwell_released();

    void on_buttonExportBoltzmann_released();

private:
    Ui::DemonstrationWindow *ui;
    World* mWorld;
    QTimer* mTimer;
    QCPBars *mDataMaxwell;
    QCPBars *mDataBoltzmann;
    QCPCurve* mDataMaxwellTheoretical;
    bool mActive;



};

#endif // DEMONSTRATIONWINDOW_H
