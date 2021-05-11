#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGridLayout>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    grid->addWidget(plot, gridRow++, 0);
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
    newGraph->qcp->setMinimumSize(600,300);
    newGraph->setField(comboBoxAlg);
    --gridRow;
    grid->addWidget(newGraph->qcp, gridRow++, 0, graphSize.height(), graphSize.width());
    grid->addWidget(newGraph->lbreplotingTime, gridRow, graphSize.width());
    grid->addWidget(newGraph->replotingTime, gridRow++, graphSize.width() + 1, 1, 2);
    grid->addWidget(newGraph->lbnData, gridRow, graphSize.width());
    grid->addWidget(newGraph->nData, gridRow++, graphSize.width() + 1, 1, 2);
    grid->addWidget(newGraph->dteStart, gridRow, graphSize.width(), 1, 2);
    grid->addWidget(newGraph->dteEnd, gridRow++, graphSize.width() + 2, 1, 2);
    grid->addWidget(newGraph->comboBoxAlg, gridRow, graphSize.width());
    grid->addWidget(newGraph->btnDeleteGraph, gridRow, graphSize.width() + 0, 2, 1);
    gridRow += graphSize.height();
    graphField.push_back(newGraph);
    plot->setMaximumSize(1,1);
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
    auto graph = graphField.back();
    graph->setStorage(storage);
    graph->replot(0);
}

