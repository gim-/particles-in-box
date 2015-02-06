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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setFixedSize(this->size());

    //CREATING ACTIONS FIRST!!! THEN CREATING MENUS...
    createActions();
    createMenus();
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

    delete toolBar;
    delete statusBr;
    delete about;
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
        ui->statusBar->setVisible(true);
    else
        ui->statusBar->setVisible(false);
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
    //working with opend file here...

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

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QSize size = this->size();
    ui->Particles->resize(size.width(), size.height());
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

    viewMenu = ui->menuBar->addMenu(tr("&View"));
    viewMenu->addAction(toolBar);
    viewMenu->addAction(statusBr);

    helpMenu = ui->menuBar->addMenu(tr("&Help"));
    helpMenu->addAction(about);
    //------------
}

void MainWindow::onWorldInitialized() {
    // Show a window since it is hidden
    CWorld* senderW = qobject_cast<CWorld*>(sender());
    this->show();

    this->ui->Particles->resizeGL(ui->Particles->width(), ui->Particles->height());

    this->ui->Particles->initializeWorld(senderW);
    connect(senderW, SIGNAL(RedrawWorld(SGeometry)), this->ui->Particles, SLOT(OnRenderScene(SGeometry)));


    // set up plot 1
    const short int s = 135;
    QVector<double>nLeft(s), time(s);
    for (double i=0.0; i<s; i++) {
        nLeft[i] = i;
        time[i] = i;
    }
    ui->Plot1->addGraph();
    ui->Plot1->graph()->setData(time, nLeft);
    ui->Plot1->xAxis->setLabel("t, s");
    ui->Plot1->xAxis->setRange(QCPRange(0, s));
    ui->Plot1->yAxis->setLabel("N");
    ui->Plot1->yAxis->setRange(QCPRange(0, s));
    ui->Plot1->replot();
    return;
}
