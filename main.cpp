#include <QApplication>
#include <QtCore>
#include <QtGui>
#include <QPushButton>
#include <QGridLayout>
#include <qcustomplot.h>


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    auto *gridLayout    = new QGridLayout;
    auto *firstgraph    = new QCustomPlot;
    auto *secondgraph   = new QCustomPlot;
    auto *gbox          = new QGroupBox;
    auto *plotButtom    = new QPushButton("Plot");
    auto *lineEdit      = new QLineEdit;
    auto *dateEdit      = new QDateEdit;
    auto *timeEdit      = new QTimeEdit;
    auto *dateLabel     = new QLabel("Date:");
    auto *timeLabel     = new QLabel("Time:");
    auto *epsLabel      = new QLabel("eps:");


    gridLayout->addWidget(firstgraph,0,0,1,1);
    gridLayout->addWidget(secondgraph,1,0,1,1);
    gridLayout->addWidget(gbox, 2,0,1,1);
    auto ggbox = new QHBoxLayout;
    ggbox->addWidget(dateLabel);
    ggbox->addWidget(dateEdit);
    ggbox->addWidget(timeLabel);
    ggbox->addWidget(timeEdit);
    ggbox->addWidget(epsLabel);
    ggbox->addWidget(lineEdit);
    ggbox->addWidget(plotButtom);
    gbox->setLayout(ggbox);
    gbox->setMaximumSize(1980,100);






    auto *w = new QWidget();
    firstgraph->setMinimumSize(600,200);
    secondgraph->setMinimumSize(600,200);
    w->setMinimumSize(640,480);

    w->setLayout(gridLayout);

    w->setWindowTitle("Data");


    w->show();

    return app.exec();
}
