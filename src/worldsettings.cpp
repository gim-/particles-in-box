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

#include "worldsettings.h"
#include "ui_worldsettings.h"

#include <QString>
#include <QLineEdit>
#include <QFileDialog>

#include "World.h"
#include "aboutwindow.h"

WorldSettings::WorldSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WorldSettings)
{
    ui->setupUi(this);
}

WorldSettings::~WorldSettings()
{
    delete ui;
}

QVariant WorldSettings::getSetting(QString key) {
    QLineEdit* object = this->findChild<QLineEdit*>(key);
    if (!object) {
        return QVariant(QString("Not found!"));
    }
    if (object->text().length() > 0) {
        return QVariant(object->text());
    }
    return QVariant(QString("Empty"));
}

void WorldSettings::on_m_FileNameButton_released()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Pick a file for dump"), QString(), "*.bin");
    this->ui->m_FileName->setText(filename);
}

void WorldSettings::on_buttonRun_pressed()
{
    // TODO: VALIDATE
    this->hide();
    emit submitSettings();
}

void WorldSettings::on_buttonExit_pressed()
{
    qApp->quit();
}

void WorldSettings::on_buttonAbout_pressed()
{
    AboutWindow* aboutW = new AboutWindow(this);
    aboutW->show();
}
