#ifndef DEMONSTRATIONWINDOW_H
#define DEMONSTRATIONWINDOW_H

#include <QMainWindow>

namespace Ui {
class DemonstrationWindow;
}

class DemonstrationWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DemonstrationWindow(QWidget *parent = 0);
    ~DemonstrationWindow();

private:
    Ui::DemonstrationWindow *ui;
};

#endif // DEMONSTRATIONWINDOW_H
