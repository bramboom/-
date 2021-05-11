#include "graph.h"

Graph::Graph()
    : QObject(nullptr)
{
    connect(comboBoxAlg, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Graph::currentAlgChanged);
    connect(btnDeleteGraph, &QPushButton::clicked, this, &Graph::deleteGraph);

    dteEnd->setCalendarPopup(true);
    dteStart->setCalendarPopup(true);
    connect(dteStart, &QDateTimeEdit::dateTimeChanged, this, &Graph::dateChanged);
    connect(dteEnd, &QDateTimeEdit::dateTimeChanged, this, &Graph::dateChanged);
}

Graph::~Graph()
{
    delete qcp;
    delete nData;
    delete lbnData;
    delete lbreplotingTime;
    delete replotingTime;
    delete comboBoxAlg;
    delete btnDeleteGraph;
    delete dteEnd;
    delete dteStart;
}

void Graph::setField(const QVector<AlgorithmComboBoxEl> &el)
{
    comboBoxEl = &el;
    for(const auto& x : *comboBoxEl) {
        comboBoxAlg->addItem(x.text);
    }
}

void Graph::replot(int id)
{
    if(!comboBoxEl || currentAlg >= comboBoxEl->size()) {
        messageBox("lol");
        return;
    }
    QTime timer;
    timer.start();
    auto graph = qcp->graph(id);
    bool xEx, yEx;
    auto yr = graph->data()->valueRange(yEx);
    graph->setData((*comboBoxEl)[currentAlg].function(graph->data(), (yr.upper - yr.lower) / 30));
    auto xr = graph->data()->keyRange(xEx);
    yr = graph->data()->valueRange(yEx);
    if(!xEx || !yEx) {
        graph->data()->clear();
    }
    else {
        xRange.lower = std::max(xr.lower, xRange.lower);
        xRange.upper = std::max(xr.upper, xRange.upper);
        yRange.lower = std::max(yr.lower, yRange.lower);
        yRange.upper = std::max(yr.upper, yRange.upper);
        auto lower = QDateTime::fromTime_t(xRange.lower);
        auto upper = QDateTime::fromTime_t(xRange.upper);
        dteStart->setDateTimeRange(lower, upper);
        dteEnd->setDateTimeRange(lower, upper);
        dteEnd->setDateTime(upper);
    }
    qcp->xAxis->setRange(xRange);
    qcp->yAxis->setRange(yRange);
    qcp->replot();
    replotingTime->setText(QString::number(timer.elapsed()) + "ms");
    nData->setText(QString::number(graph->data()->size()));
}

void Graph::replot(int id, const QSharedPointer<QCPGraphDataContainer>& data)
{
    QTime timer;
    timer.start();
    auto graph = qcp->graph(id);
    bool xEx, yEx;
    auto yr = data->valueRange(yEx);
    graph->setData((*comboBoxEl)[currentAlg].function(data, (yr.upper - yr.lower) / 30));
    auto xr = data->keyRange(xEx);
    if(!xEx || !yEx) {
        graph->data()->clear();
    }
    else {
        xRange = xr;
        yRange = yr;
        auto lower = QDateTime::fromTime_t(xRange.lower);
        auto upper = QDateTime::fromTime_t(xRange.upper);
    }
    qcp->xAxis->setRange(xRange);
    qcp->yAxis->setRange(yRange);
    qcp->replot();
    replotingTime->setText(QString::number(timer.elapsed()) + "ms");
    nData->setText(QString::number(graph->data()->size()));
}

void Graph::replot(int id, const QSharedPointer<QCPGraphDataContainer>& data, const QCPRange& range)
{
    auto graph = qcp->graph(id);
    graph->setData(QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer(*data)));
    graph->data()->removeBefore(range.lower);
    graph->data()->removeAfter(range.upper);
    replot(id);
}

void Graph::setStorage(const QSharedPointer<QCPGraphDataContainer>& data)
{
    storage = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer(*data));
}

void Graph::currentAlgChanged(int index)
{
    currentAlg = index;
    replot(0);
}

void Graph::deleteGraph()
{
    this->~Graph();
}

void Graph::dateChanged(const QDateTime& dateTime)
{
    auto s2 = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer(*storage));
    s2->removeBefore(static_cast<double>(dteStart->dateTime().toTime_t()));
    s2->removeAfter(static_cast<double>(dteEnd->dateTime().toTime_t()));
    replot(0, s2);
}

