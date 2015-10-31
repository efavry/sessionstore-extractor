#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFileDialog>
#include <QLineEdit>
#include <QTextStream>
#include <QMessageBox>
#include <QProgressBar>
#include <QDebug>
#include <iostream>
namespace Ui
{
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT
private :
    QString fileToRead;
    QLineEdit *lineFiletodecode;
    QString fileToWrite;
    QLineEdit *lineWriteFile;
    QProgressBar* progressBar;
public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

public slots:
    void getPathFileToRead(); //this slot is called when the user hit the find path button for reading the js
    void getPathFileToWrite(); //this slot is called when the user hit the find path button for writing the export file
    void processFile(); // this slot is called when the user begin the scan
};

#endif // MAINWINDOW_H
