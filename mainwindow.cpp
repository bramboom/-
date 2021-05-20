#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cw->setLayout(grid);
    ui->scrollArea->setWidget(cw);
    QGridLayout* grid = new QGridLayout;
    grid->addWidget(lblPath, 0, 0);
    grid->addWidget(lePath, 0, 1, 1, 4);
    grid->addWidget(btnSetPathFile, 0, 5);
    widget->setLayout(grid);
    widget->setMinimumSize(300, 100);
    connect(btnSetPathFile,     &QPushButton::clicked, this, &MainWindow::btnSetFileClicked);
    connect(btnSetPathFile,     &QPushButton::clicked, this, &MainWindow::btnSetServerClicked);
    connect(ui->btnAddGraph,    &QPushButton::clicked, this, &MainWindow::btnAddGraphClicked);
    connect(ui->btnClose,       &QPushButton::clicked, this, &MainWindow::close);
    connect(lePath,             &QLineEdit::textEdited, this, &MainWindow::lePathEdited);
    connect(ui->cwConfiguration, &QUaConfigurationWidget::dateTimeRangeUpdated, this, &MainWindow::serverUpdate);
    connect(ui->actionFile,     &QAction::triggered, this, &MainWindow::inputFilePath);
    connect(ui->actionServer,   &QAction::triggered, this, &MainWindow::inputServerPath);
}

MainWindow::~MainWindow()
{
    delete grid;
    delete cw;
    delete ui;
    for(auto& graph : graphField) {
        delete graph;
    }
}

bool MainWindow::read_from_file(const QString &path)
{
    storage->push_back(storage_t(new storage_t::value_type));
    FILE* file = fopen(path.toLocal8Bit().data(), "r");
    if(!file) {
        return false;
    }
    size_t  l;
    char*   line { nullptr };
    double  value;
    while(!feof(file)) {
        getline(&line, &l, file);
        struct tm t { };
        sscanf(line, "%d.%d.%d %d:%d:%d\t%lf", &t.tm_mday, &t.tm_mon, &t.tm_year, &t.tm_hour, &t.tm_min, &t.tm_sec, &value);
        t.tm_year -= 1900;
        --t.tm_mon;
        storage->back()->add({ static_cast<double>(mktime(&t)), value });
    }
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

MainWindow::storage_t MainWindow::readFromServer(const QString &nodeId, const QDateTime &start, const QDateTime &end)
{
    storage_t qcpData { new storage_t::value_type };
    if(start < end)
    {
        using namespace EleSyOpcUaClient;
        UaBytes cPoint, cPointNext;
        auto id = nodeId.toStdString();
        auto data = history->getHistoryRawValues(id, 1000000, UaDateTime(start.toMSecsSinceEpoch() * 1000), UaDateTime(end.toMSecsSinceEpoch() * 1000), false, cPoint);
        for(const auto& x : data) {
            qcpData->add(QCPGraphData(x.sourceTimeStamp.toUnixMicrosec() / 1000000, x.val->v_double()));
        }
        do {
            data = history->getNextHistoryRawValues(id, cPoint, cPointNext);
            for(const auto& x : data) {
                qcpData->add(QCPGraphData(x.sourceTimeStamp.toUnixMicrosec() / 1000000, x.val->v_double()));
            }
            cPoint = cPointNext;
        } while(!data.empty());
        history->releasePoint(id, cPoint);
    }
    return qcpData;
}

void MainWindow::btnSetFileClicked(bool checked)
{
    storage->clear();
    QTime timer;
    timer.start();
    bool readed = read_from_file(lePath->text());
    if(!readed)
    {
        lePath->setStyleSheet("color: red;");
    }
    else
    {
        ui->lblRdTm->setText(QString::number(timer.elapsed()) + "ms");
        widget->close();
    }
    for(const auto& x : graphField)
    {
        x->setStorage(storage);
        x->replot(0);
    }
}

void MainWindow::btnSetServerClicked(bool checked)
{
    //opc.tcp://192.168.56.1:62456
    using namespace EleSyOpcUaClient;
    ConnectionParams params;
    params.serverUrl = lePath->text().toStdString();
    params.connectionName = "Graph Decimator Connection";
    UaStatusCode connectionStatus = createConnection(params, true, false, connection);
    ui->cwServerBrowser->setConnection(connection);
    UaStatusCode historyStatus = createHistory(connection, history);
    if(connectionStatus != uaGood || historyStatus != uaGood || !connection->isConnected() || ui->cwServerBrowser->isValid())
    {
        lePath->setStyleSheet("color: red;");
        return;
    }
    widget->close();
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

void MainWindow::lePathEdited(const QString &text)
{
    lePath->setStyleSheet("color: black");
}

void MainWindow::serverUpdate(const QDateTime &startDateTime, const QDateTime &endDateTime)
{
    //opc.tcp://192.168.56.1:62456
    storage->clear();
    for(auto i = 0; i < ui->cwConfiguration->list->count(); ++i)
    {
        storage->push_back(readFromServer(ui->cwConfiguration->list->item(i)->text(),startDateTime, endDateTime));
        messageBox(ui->cwConfiguration->list->item(i)->text() + '\n' + QString::number(storage->at(i)->size()));
    }

    for(const auto& x : graphField)
    {
        x->setStorage(storage);
        x->replot();
    }
}

void MainWindow::inputFilePath(bool cheked)
{
    lblPath->setText("Path to file:");
    btnSetPathFile->setText("Set path from file");
    connect(btnSetPathFile,    &QPushButton::clicked, this, &MainWindow::btnSetFileClicked);
    disconnect(btnSetPathFile, &QPushButton::clicked, this, &MainWindow::btnSetServerClicked);
    widget->setWindowModality(Qt::ApplicationModal);
    widget->show();
}

void MainWindow::inputServerPath(bool cheked)
{
    lblPath->setText("Server IP");
    btnSetPathFile->setText("Set path from server");
    disconnect(btnSetPathFile, &QPushButton::clicked, this, &MainWindow::btnSetFileClicked);
    connect(btnSetPathFile,    &QPushButton::clicked, this, &MainWindow::btnSetServerClicked);
    widget->setWindowModality(Qt::ApplicationModal);
    widget->show();
}


