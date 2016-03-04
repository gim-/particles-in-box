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

private:
    Ui::DemonstrationWindow *ui;
    World* mWorld;



};

#endif // DEMONSTRATIONWINDOW_H
