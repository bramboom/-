#include "graph.h"

Graph::Graph()
    : QObject(nullptr)
{
    connect(comboBoxAlg, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Graph::currentAlgChanged);
    connect(btnDeleteGraph, &QPushButton::clicked, this, &Graph::deleteGraph);
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
    }
    qcp->xAxis->setRange(xRange);
    qcp->yAxis->setRange(yRange);
    qcp->replot();
    replotingTime->setText(QString::number(timer.elapsed()) + "ms");
    nData->setText(QString::number(graph->data()->size()));
}

void Graph::replot(int id, const QSharedPointer<QCPGraphDataContainer>& data)
{
    qcp->graph(id)->setData(data);
    replot(id);
}

void Graph::replot(int id, const QSharedPointer<QCPGraphDataContainer>& data, const QCPRange& range)
{
    auto graph = qcp->graph(id);
    graph->setData(QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer(*data)));
    graph->data()->removeBefore(range.lower);
    graph->data()->removeAfter(range.upper);
    replot(id);
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

