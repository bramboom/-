#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGridLayout>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    grid->addWidget(new QLabel("test"), gridRow++, 0);
    //ui->scrollArea->setLayout(grid);
    auto widget = new QWidget;
    widget->setLayout(grid);
    ui->scrollArea->setWidget(widget);
    connect(ui->btnSetPath, &QPushButton::clicked, this, &MainWindow::btnPathClicked);
    connect(ui->btnAddGraph, &QPushButton::clicked, this, &MainWindow::btnAddGraphClicked);
    connect(ui->btnClose, &QPushButton::clicked, this, &MainWindow::close);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::read_from_file(const QString &path)
{
    FILE* file = fopen(path.toLocal8Bit().data(), "r");
    if(file == nullptr) {
        return false;
    }
    size_t  l;
    char*   line { nullptr };
    double  value;
    QTime time;
    time.start();
    while(!feof(file)) {
        getline(&line, &l, file);
        struct tm t { };
        sscanf(line, "%d.%d.%d %d:%d:%d\t%lf", &t.tm_mday, &t.tm_mon, &t.tm_year, &t.tm_hour, &t.tm_min, &t.tm_sec, &value);
        t.tm_year -= 1900;
        --t.tm_mon;
        storage->add({ static_cast<double>(mktime(&t)), value });
    }
    ui->lbRdTm->setText(QString::number(time.elapsed()));
    return true;
}

void MainWindow::addGraph()
{
    QSharedPointer<Graph> newGraph { new Graph };
    newGraph->qcp->addGraph();
    newGraph->qcp->xAxis->setTicker(QSharedPointer<QCPAxisTickerDateTime>(new QCPAxisTickerDateTime));
    newGraph->qcp->setMinimumSize(600,200);
    //newGraph->qcp->setMaximumSize(200,200);
    newGraph->setField(comboBoxAlg);
    --gridRow;
    grid->addWidget(newGraph->qcp, gridRow++, 0, graphSize.height(), graphSize.width());
    grid->addWidget(new QLabel("Replot time: "), gridRow, graphSize.width());
    grid->addWidget(newGraph->replotingTime, gridRow++, graphSize.width() + 1, 1, 2);
    grid->addWidget(new QLabel("Data count: "), gridRow, graphSize.width());
    grid->addWidget(newGraph->nData, gridRow++, graphSize.width() + 1, 1, 2);
    grid->addWidget(newGraph->comboBoxAlg, gridRow, graphSize.width());
    gridRow += graphSize.height();
    graphField.push_back(newGraph);
}

void MainWindow::btnPathClicked(bool checked)
{
    storage->clear();
    if(read_from_file(lePath->text())) {
        for(const auto& x : graphField) {
            x->replot(0, storage);
        }
    }
}

void MainWindow::btnAddGraphClicked(bool checked)
{
    addGraph();
    graphField.back()->replot(0, storage);
}

