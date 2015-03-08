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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "World.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //this->setFixedSize(this->size());

    //CREATING ACTIONS FIRST!!! THEN CREATING MENUS...
    createActions();
    createMenus();
    //------------------
    createStatusBar();
    //------------------
    //Setting amount of graphWindows
    graphWindows.fill(nullptr, 3);
    //------------------
}

MainWindow::~MainWindow()
{
    delete ui;
    delete New;
    delete open;
    delete save;
    delete saveAs;
    delete print;
    delete printPrev;
    delete printSetup;
    delete recFile;
    delete exit;

    delete undo;
    delete cut;
    delete copy;
    delete paste;

    delete heightDist;
    delete MaxwellDist;
    delete particleDist;

    delete toolBar;
    delete statusBr;
    delete about;

    deleteStatusBar();
    //delete scene;
}

void MainWindow::toolBarSlot(bool checked)
{
    if (checked)
        ui->mainToolBar->setVisible(true);
    else
        ui->mainToolBar->setVisible(false);
}

void MainWindow::statusBarSlot(bool checked)
{
    if (checked)
    {
        ui->statusBar->setVisible(true);
        ui->Particles->setGeometry(0, 10, this->width(), this->height()-ui->statusBar->geometry().height()-44);
    }
    else
    {
        ui->statusBar->setVisible(false);
        ui->Particles->setGeometry(0, 10, this->width(), this->height()-43);
    }
}

void MainWindow::exitSlot()
{
    qApp->quit();
}

void MainWindow::onAboutClick()
{
    AboutWindow* aboutW = new AboutWindow(this);
    aboutW->show();
}

void MainWindow::onOpenClick()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Files (*.*)"));
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return;
    //working with opened file here...

    //--------------------
}

void MainWindow::onSaveClick()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                     "",
                                                     tr("Files (*.txt)"));
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    //generating file here...

    //------
    file.close();
}

void MainWindow::onSaveAsClick()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                     "",
                                                     tr("Files (*.*)"));
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    //generating file here...

    //------
    file.close();
}

void MainWindow::onNewClick()
{

}

void MainWindow::onHeightDistClick()
{
    if (senderW)
    {
        if (!graphWindows[0])
        {
            graphWindows[0] = new GraphWindow("H", "<V>", this);
            senderW->setHeightDistActive(true);
            connect(senderW, SIGNAL(RedrawHeightGraph(const QVector<double>*)), graphWindows[0], SLOT(updateHeightGraph(const QVector<double>*)));
            connect(graphWindows[0], SIGNAL(destroyed()), this, SLOT(onHeightDistClose()));
        }

        graphWindows[0]->setWindowFlags( Qt::Window );
        graphWindows[0]->setWindowTitle("Speed Distribution");
        graphWindows[0]->show();
    }
}

void MainWindow::onMaxwellDistClick()
{
    if (senderW)
    {
        if (!graphWindows[1])
        {
            graphWindows[1] = new GraphWindow("V", "N", this);
            senderW->setMaxwellDistActive(true);
            connect(senderW, SIGNAL(RedrawMaxwellDistGraph(const QVector<double>*)), graphWindows[1], SLOT(updateMaxwellDistGraph(const QVector<double>*)));
            connect(graphWindows[1], SIGNAL(destroyed()), this, SLOT(onMaxwellDistClose()));
        }

        graphWindows[1]->setWindowFlags( Qt::Window );
        graphWindows[1]->setWindowTitle("Maxwell Distribution");
        graphWindows[1]->show();
    }
}


void MainWindow::onParticleDistClick()
{
    if (senderW)
    {
        if (!graphWindows[2])
        {
            graphWindows[2] = new GraphWindow("H", "N", this);
            senderW->setParticleDistActive(true);
            connect(senderW, SIGNAL(RedrawParticleDistGraph(const QVector<double>*)), graphWindows[2], SLOT(updateParticleDistGraph(const QVector<double>*)));
            connect(graphWindows[2], SIGNAL(destroyed()), this, SLOT(onParticleDistClose()));
        }

        graphWindows[2]->setWindowFlags( Qt::Window );
        graphWindows[2]->setWindowTitle("Particle Distribution");
        graphWindows[2]->show();
    }
}

void MainWindow::onParticleDistClose()
{
    graphWindows[2] = nullptr;
    senderW->setParticleDistActive(false);
}



void MainWindow::onMaxwellDistClose()
{
    graphWindows[1] = nullptr;
    senderW->setMaxwellDistActive(false);
}

void MainWindow::onHeightDistClose()
{
    graphWindows[0] = nullptr;
    senderW->setHeightDistActive(false);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    if(ui->statusBar->isVisible() || statusBr->isChecked())
        ui->Particles->setGeometry(0, 10, this->width(), this->height()-ui->statusBar->geometry().height()-44);
    else
        ui->Particles->setGeometry(0, 10, this->width(), this->height()-43);
    ui->Particles->resizeGL(ui->Particles->width(), ui->Particles->height());

    QMainWindow::resizeEvent(event);
}

void MainWindow::createActions()
{
    //Creating actions...
    New = new QAction(tr("&New"), this);
    New->setShortcut(QKeySequence::New);
    New->setToolTip(tr("Create a new file"));
    open = new QAction(tr("&Open"), this);
    open->setShortcut(QKeySequence::Open);
    open->setToolTip(tr("open a file"));
    save = new QAction(tr("&Save"), this);
    save->setShortcut(QKeySequence::Save);
    save->setToolTip(tr("Save a file"));
    saveAs = new QAction(tr("&Save As..."), this);
    saveAs->setShortcut(QKeySequence::SaveAs);
    saveAs->setToolTip(tr("Save a file as..."));
    print = new QAction(tr("&Print"), this);
    print->setShortcut(QKeySequence::Print);
    print->setToolTip(tr("Print a file"));
    printPrev = new QAction(tr("&Print Preview"), this);
    printSetup = new QAction(tr("&Print Setup..."), this);
    recFile = new QAction(tr("&Recent File"), this);
    exit = new QAction(tr("&Exit"), this);
    exit->setToolTip(tr("Exit"));

    undo = new QAction(tr("&Undo"), this);
    undo->setShortcut(QKeySequence::Undo);
    undo->setToolTip(tr("Undo the action"));
    cut = new QAction(tr("&Cut"), this);
    cut->setShortcut(QKeySequence::Cut);
    copy = new QAction(tr("&Copy"), this);
    copy->setShortcut(QKeySequence::Copy);
    paste =  new QAction(tr("&Paste"), this);
    paste->setShortcut(QKeySequence::Paste);


    heightDist = new QAction(tr("&Height distribution"), this);
    MaxwellDist = new QAction(tr("&Maxwell distribution"), this);
    particleDist = new QAction(tr("&Particle distribution"), this);

    toolBar = new QAction(tr("&Toolbar"), this);
    toolBar->setCheckable(true);
    toolBar->setChecked(true);
    statusBr =  new QAction(tr("&Status Bar"), this);
    statusBr->setCheckable(true);
    statusBr->setChecked(true);

    about = new QAction(tr("&About ParticlesInBox"), this);
    //-------------------

    //some actions are disabled here by default
    recFile->setEnabled(false);
    undo->setEnabled(false);
    cut->setEnabled(false);
    copy->setEnabled(false);
    paste->setEnabled(false);
    //---------------------

    //connecting signals of actions to slots...
    connect(New, SIGNAL(triggered()), this, SLOT(onNewClick()));
    connect(saveAs, SIGNAL(triggered()), this, SLOT(onSaveAsClick()));
    connect(save, SIGNAL(triggered()), this, SLOT(onSaveClick()));
    connect(open, SIGNAL(triggered()), this, SLOT(onOpenClick()));
    connect(heightDist, SIGNAL(triggered()), this, SLOT(onHeightDistClick()));
    connect(MaxwellDist, SIGNAL(triggered()), this, SLOT(onMaxwellDistClick()));
    connect(particleDist, SIGNAL(triggered()), this, SLOT(onParticleDistClick()));

    connect(toolBar, SIGNAL(triggered(bool)), this, SLOT(toolBarSlot(bool)));
    connect(statusBr, SIGNAL(triggered(bool)), this, SLOT(statusBarSlot(bool)));
    connect(about, SIGNAL(triggered()), this, SLOT(onAboutClick()));
    connect(exit, SIGNAL(triggered()), this, SLOT(exitSlot()));
    //-------------------
}

void MainWindow::createMenus()
{
    //Creating menus...
    fileMenu = ui->menuBar->addMenu(tr("&File"));
    fileMenu->addAction(New);
    fileMenu->addAction(open);
    fileMenu->addAction(save);
    fileMenu->addAction(saveAs);
    fileMenu->addSeparator();
    fileMenu->addAction(print);
    fileMenu->addAction(printPrev);
    fileMenu->addAction(printSetup);
    fileMenu->addSeparator();
    fileMenu->addAction(recFile);
    fileMenu->addSeparator();
    fileMenu->addAction(exit);

    editMenu = ui->menuBar->addMenu(tr("&Edit"));
    editMenu->addAction(undo);
    editMenu->addSeparator();
    editMenu->addAction(cut);
    editMenu->addAction(copy);
    editMenu->addAction(paste);

    charts = ui->menuBar->addMenu(tr("&Charts"));
    charts->addAction(heightDist);
    charts->addAction(MaxwellDist);
    charts->addAction(particleDist);

    viewMenu = ui->menuBar->addMenu(tr("&View"));
    viewMenu->addAction(toolBar);
    viewMenu->addAction(statusBr);

    helpMenu = ui->menuBar->addMenu(tr("&Help"));
    helpMenu->addAction(about);
    //------------
}

void MainWindow::createStatusBar()
{
    statusTime = new QLabel(this);
    statusTime->setAlignment(Qt::AlignRight);
    statusTime->setIndent(3);
    statusTime->setMargin(8);

    statusLeft = new QLabel(this);
    statusLeft->setAlignment(Qt::AlignRight);
    statusLeft->setIndent(3);
    statusLeft->setMargin(8);

    statusRight = new QLabel(this);
    statusRight->setAlignment(Qt::AlignRight);
    statusRight->setIndent(3);
    statusRight->setMargin(8);

    statusBar()->addWidget(statusTime, 1);
    statusBar()->addWidget(statusLeft, 1);
    statusBar()->addWidget(statusRight, 1);

    statusBar()->showMessage("Ready");
}

void MainWindow::deleteStatusBar()
{
    if (statusTime)
        delete statusTime;
    if (statusLeft)
        delete statusLeft;
    if (statusRight)
        delete statusRight;
}

void MainWindow::onWorldInitialized() {
    // Show a window since it is hidden
    //CWorld* senderW = qobject_cast<CWorld*>(sender());
    senderW = qobject_cast<CWorld*>(sender());
    this->show();
    this->ui->Particles->resizeGL(ui->Particles->width(), ui->Particles->height());

    this->ui->Particles->initializeWorld(senderW);
    connect(senderW, SIGNAL(RedrawWorld(SGeometry)), this->ui->Particles, SLOT(OnRenderScene(SGeometry)));
    connect(senderW, SIGNAL(onParticleSCountChange()), this, SLOT(updateStatusBar()));

    QSize partSize = ui->Particles->size();
    ui->Particles->resize(partSize.width() - 50, partSize.height() - 50);
    ui->Particles->resize(partSize.width(), partSize.height());

    return;
}

void MainWindow::updateStatusBar()
{
    if (senderW)
    {
        float nLeftPercent = senderW->nLeft*100.0f/senderW->GetParticleCount();
        float nRightPercent = senderW->nRight*100.0f/senderW->GetParticleCount();
        statusLeft->setText("Left: "+QString::number(senderW->nLeft)+" ("+QString::number(nLeftPercent)+"%) <V> = "+QString::number(senderW->VaverageL));
        statusRight->setText("Right: "+QString::number(senderW->nRight)+"("+QString::number(nRightPercent)+"%) <V> = "+QString::number(senderW->VaverageR));
        statusTime->setText("Time="+QString::number(senderW->Time)+" Step="+QString::number(senderW->DeltaTime));
    }

}
