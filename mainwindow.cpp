#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGridLayout>
//#include <opcuaclient/UaConnection.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cw->setLayout(grid);
    ui->scrollArea->setWidget(cw);
    connect(ui->btnRdDt,      &QPushButton::clicked, this, &MainWindow::btnPathClicked);
    connect(ui->btnAddGraph,  &QPushButton::clicked, this, &MainWindow::btnAddGraphClicked);
    connect(ui->btnClose, &QPushButton::clicked, this, &MainWindow::close);
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::cbIndexChanged);
}

MainWindow::~MainWindow()
{
    delete btnAddGraph;
    delete btnPath;
    delete lePath;
    delete grid;
    delete cw;
    delete ui;
    for(auto& graph : graphField) {
        delete graph;
    }
}

bool MainWindow::read_from_file(const QString &path)
{
    FILE* file = fopen(path.toLocal8Bit().data(), "r");
    if(!file) {
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
    ui->lblRdTm->setText(QString::number(time.elapsed()) + "s");
    return true;
}

void MainWindow::addGraph()
{
    Graph* newGraph { new Graph };
    newGraph->customPlot()->addGraph();
    newGraph->customPlot()->xAxis->setTicker(QSharedPointer<QCPAxisTickerDateTime>(new QCPAxisTickerDateTime));
    newGraph->setField(comboBoxAlg);
    connect(newGraph, &Graph::close, this, &MainWindow::btnCloseGraphClicked);
    grid->addWidget(newGraph->widget(), gridRow++, 0, 1, 4);
    graphField.push_back(newGraph);
}

void MainWindow::btnPathClicked(bool checked)
{
    storage->clear();
    if(read_from_file(lePath->text())) {
        for(const auto& x : graphField) {
            x->setStorage(storage);
            x->replot(0);
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

void MainWindow::btnCloseGraphClicked(Graph* graph)
{
    graphField.removeOne(graph);
    grid->removeWidget(graph->widget());
}

void MainWindow::cbIndexChanged(bool checked)
{
    QString cbCurrentText = ui->comboBox->currentText();
    ui->btnRdDt->setText("Read data from " + cbCurrentText);
    if(cbCurrentText=="File")
    {
        ui->lblPath->setText("Path to file:");
    }
    else
    {
        ui->lblPath->setText("Server IP");
    }
}



