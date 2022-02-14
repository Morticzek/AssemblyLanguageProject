#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dijsktra.h"

#include <QDebug>
#include <QTextBrowser>
#include <QPushButton>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->launchAssemblyButton, SIGNAL(clicked()), this, SLOT(on_launchAssemblyButton_clicked()));
    connect(ui->launchCppButton, SIGNAL(clicked()), this, SLOT(on_launchCppButton_clicked()));
    connect(ui->loadFileButton, SIGNAL(clicked()), this, SLOT(on_loadFile_clicked()));

    ui->launchAssemblyButton->setDisabled(true);
    ui->launchCppButton->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_loadFile_clicked()
{
    filePath = QFileDialog::getOpenFileName();
    ui->fileLabel->setText("File Loaded: \n" + filePath);
    ui->launchAssemblyButton->setDisabled(false);
    ui->launchCppButton->setDisabled(false);
}

void MainWindow::on_launchAssemblyButton_clicked()
{
    QByteArray convPath = filePath.toLocal8Bit();
    char* file = convPath.data();
    auto vertex = ui->vertexNameLine->text();
    QByteArray conv = vertex.toLocal8Bit();
    startingVertex = conv.data();
    ui->asmBrowser->clearHistory();
    char* asmResult = loadData(file, startingVertex);
    ui->asmBrowser->setText(asmResult);
}

void MainWindow::on_launchCppButton_clicked()
{
    QByteArray convPath = filePath.toLocal8Bit();
    char* file = convPath.data();
    auto vertex = ui->vertexNameLine->text();
    auto threadNum = ui->threadsNumLine->text();
    QByteArray conv = vertex.toLocal8Bit();
    startingVertex = conv.data();
    threadNumber = threadNum.toInt();
    if(threadNumber <1)
        threadNumber = 1;
    ui->cppBrowser->clearHistory();
    char* cppResult = loadDataOmp(file, startingVertex, threadNumber);
    ui->cppBrowser->setText(cppResult);
}
