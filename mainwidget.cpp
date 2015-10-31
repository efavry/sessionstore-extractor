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

MainWidget::~MainWidget()
{
    //delete ui;
}
