#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_loadFile_clicked();

    void on_launchAssemblyButton_clicked();

    void on_launchCppButton_clicked();

private:
    Ui::MainWindow *ui;
    QString filePath;
    int threadNumber;
    char* startingVertex;
};
#endif // MAINWINDOW_H
