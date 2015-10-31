#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QFrame>
#include <QLabel>
#include <QDir>
#include <QProgressBar>
#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{
    QGridLayout *generalLayout = new QGridLayout;

    QLabel *labelFileToDecode = new QLabel ("The file to decode : ");
    lineFiletodecode = new QLineEdit();
    lineFiletodecode->setReadOnly(true);
    lineFiletodecode->setMinimumWidth(200);
    //lineFiletodecode->setMaximumWidth(lineFiletodecode->maximumWidth()+lineFiletodecode->minimumWidth());
    QPushButton *findPathFileToDecode = new QPushButton(tr("Find the file"));
    connect(findPathFileToDecode,SIGNAL(clicked()),this,SLOT(getPathFileToRead()));
    generalLayout->addWidget(labelFileToDecode,0,0);
    generalLayout->addWidget(lineFiletodecode,0,1,1,2);
    generalLayout->addWidget(findPathFileToDecode,0,3);

    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    generalLayout->addWidget(line,1,0,1,4); //begin at line 1 and colomn 0 annd end at line 1, column 4

    QLabel *labelWriteFile = new QLabel ("The file output : ");
    lineWriteFile = new QLineEdit();
    lineWriteFile->setReadOnly(true);
    lineWriteFile->setMinimumWidth(200); //not necessary but more clear
    QPushButton *findPathWriteFile= new QPushButton(tr("Select output"));
    connect(findPathWriteFile,SIGNAL(clicked()),this,SLOT(getPathFileToWrite()));
    generalLayout->addWidget(labelWriteFile,2,0);
    generalLayout->addWidget(lineWriteFile,2,1,1,2);
    generalLayout->addWidget(findPathWriteFile,2,3);

    QFrame* line_bottom = new QFrame();
    line_bottom->setFrameShape(QFrame::HLine);
    line_bottom->setFrameShadow(QFrame::Sunken);
    generalLayout->addWidget(line_bottom,3,0,1,4); //begin at line 1 and colomn 0 annd end at line 1, column 4

    QPushButton *beginScan= new QPushButton(tr("Begin scan"));
    generalLayout->addWidget(beginScan,4,3);
    connect(beginScan,SIGNAL(clicked()),this,SLOT(processFile()));

    progressBar = new QProgressBar();
    generalLayout->addWidget(progressBar,4,0,1,3);
    setLayout(generalLayout);

    this->setMinimumSize(this->minimumSize());
    this->setMaximumSize(this->minimumSize());

}

void MainWidget::getPathFileToRead()
{
    this->fileToRead = QFileDialog::getOpenFileName(this,tr("Open SessionStore"), QDir::toNativeSeparators(QDir::homePath() + "/.mozilla/firefox/"), tr("sessionstore file (*.js *.bak)"));
    this->lineFiletodecode->setText(this->fileToRead);
}

void MainWidget::getPathFileToWrite()
{
    this->fileToWrite= QFileDialog::getSaveFileName(this,tr("Save extracted website"), QDir::toNativeSeparators(QDir::homePath()), tr("Extracted Websites (*)"));
    this->lineWriteFile->setText(this->fileToWrite);
}

void MainWidget::processFile()
{
    //creating the file and opening it
    QFile fileReaded(fileToRead);

    if (!fileReaded.open(QIODevice::ReadOnly | QIODevice::Text))
    {   //if the open fail we signal it and quit the method
        QMessageBox msgBoxErrorOpening;
        msgBoxErrorOpening.setIcon(QMessageBox::Warning);
        msgBoxErrorOpening.setText("There was an error while openning the file to scan. \nPlease check if the path is valid \nSorry :(");
        msgBoxErrorOpening.exec();
        return;
    }
    //we prepare the stream to read
    QTextStream in(&fileReaded);

    QFile fileWrited(fileToWrite);
    if(!fileWrited.open(QIODevice::WriteOnly | QIODevice::Text))
    {   //if the open fail we signal it and quit the method
        QMessageBox msgBoxErrorSaving;
        msgBoxErrorSaving.setText("There was an error while trying to create the file for exporting the result. \nPlease check an output file is selected and if you can write it\nSorry :(");
        msgBoxErrorSaving.exec();
        fileReaded.close();
        return;
    }
    //we prepare the stream to write
    QTextStream out(&fileWrited);

    //we set the codec of the opened file
    in.setCodec("UTF-8");

    /*will be used for the progress bar*/
    qint64 maxPos = fileReaded.size();
    //to avoid calculating each time we meet the if inside the while we do it now
    qint64 firstPos = maxPos/4;
    qint64 secondPos = maxPos/2;
    qint64 thirdPos = firstPos+secondPos;
    qint64 actualPos=0;
    progressBar->setMaximum(maxPos); //adjust the bar

    QString line;
    while (!in.atEnd())
    {

        //we try to find an url and store it in the new file
        line = in.read(1);
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
        if(actualPos==firstPos)
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

MainWidget::~MainWidget()
{
    //delete ui;
}
