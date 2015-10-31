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
    void getPathFileToRead()
    {
        this->fileToRead = QFileDialog::getOpenFileName(this,tr("Open SessionStore"), QDir::toNativeSeparators(QDir::homePath() + "/.mozilla/firefox/"), tr("sessionstore file (*.js *.bak)"));
        this->lineFiletodecode->setText(this->fileToRead);
    }

    void getPathFileToWrite()
    {
        this->fileToWrite= QFileDialog::getSaveFileName(this,tr("Save extracted website"), QDir::toNativeSeparators(QDir::homePath()), tr("Extracted Websites (*)"));
        this->lineWriteFile->setText(this->fileToWrite);
    }

    void processFile()
    {
        QFile fileReaded(fileToRead);
        QFile fileWrited(fileToWrite);
        if (!fileReaded.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox msgBoxErrorOpening;
            msgBoxErrorOpening.setIcon(QMessageBox::Warning);
            msgBoxErrorOpening.setText("There was an error while openning the file to scan. \nPlease check if the path is valid \nSorry :(");
            msgBoxErrorOpening.exec();
            return;
        }
        QTextStream in(&fileReaded);


        if(!fileWrited.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox msgBoxErrorSaving;
            msgBoxErrorSaving.setText("There was an error while trying to create the file for exporting the result. \nPlease check an output file is selected and if you can write it\nSorry :(");
            msgBoxErrorSaving.exec();
            fileReaded.close();
            return;
        }
        QTextStream out(&fileWrited);

        in.setCodec("UTF-8");

        /*will be used for the progress bar*/
        qint64 maxPos = fileReaded.size();
        //to avoid calculating each time we meet the if inside the while
        qint64 firstPos = maxPos/4;
        qint64 secondPos = maxPos/2;
        qint64 thirdPos = firstPos+secondPos;
        qint64 actualPos=0;
        progressBar->setMaximum(maxPos);

        while (!in.atEnd())
        {
            QString line = in.read(1);
            if(
                (line=="\"") &&
                    ((line = in.read(1))=="u") &&
                        ((line = in.read(1))=="r") &&
                            ((line = in.read(1))=="l") &&
                                ((line = in.read(1))=="\"") &&
                                    ((line = in.read(1))==":") &&
                                        ((line = in.read(1))=="\"")
                                            )
            {
              while((line = in.read(1))!="\"")
                  out << line;
              out << "\n";
            }
            //because in the documentation of in.pos(); we learn that : " this function may have to seek the device to reconstruct a valid device position. This operation can be expensive"
            //we actually count ourself
            actualPos++;
            if(actualPos==firstPos) //to not set the progress bar each time
                progressBar->setValue(actualPos);
            if(actualPos==secondPos)
                progressBar->setValue(actualPos);
            if(actualPos==thirdPos)
                progressBar->setValue(actualPos);
        }
        progressBar->setValue(progressBar->maximum());
        fileReaded.close();
        fileWrited.close();
    }

};

#endif // MAINWINDOW_H
