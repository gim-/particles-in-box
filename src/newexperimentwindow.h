#ifndef NEWEXPERIMENTWINDOW_H
#define NEWEXPERIMENTWINDOW_H

#include <QMainWindow>
#include <QString>

namespace Ui {
class NewExperimentWindow;
}

class NewExperimentWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NewExperimentWindow(QWidget *parent = 0);
    ~NewExperimentWindow();

    void setInputFile(const QString& newFile);
    void clearInputFile();

private slots:
    void on_buttonRun_released();

    void on_buttonOutputFile_released();

    void on_labelInputFile_clicked();
    void on_labelClearInputFile_clicked();

    void onSimulationComplete();

private:
    Ui::NewExperimentWindow *ui;
    QString mInputFileName;
    QString mLabelInputFileText;
};

#endif // NEWEXPERIMENTWINDOW_H
