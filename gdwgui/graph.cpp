#include "graph.h"
#include "stylesheet.h"

Graph::Graph()
    : QObject(nullptr)
{
    connect(m_cbAlgorithm,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Graph::currentAlgChanged);
    connect(m_buttonClose,  &QPushButton::clicked, this, &Graph::btnCloseClicked);
    connect(m_dteStart,     &QDateTimeEdit::dateTimeChanged, this, &Graph::dteStartChanged);
    connect(m_dteEnd,       &QDateTimeEdit::dateTimeChanged, this, &Graph::dteEndChanged);
    m_buttonClose->setMinimumSize(25, 25);
    m_buttonClose->setMaximumSize(25, 25);
    m_dteEnd->setMaximumWidth(180);
    m_dteStart->setMaximumWidth(180);
    m_dteEnd->setMinimumWidth(180);
    m_dteStart->setMinimumWidth(180);
    m_dteEnd->setDisplayFormat("dd.MM.yyyy HH:mm:ss");
    m_dteStart->setDisplayFormat("dd.MM.yyyy HH:mm:ss");
    m_customPlot->setMinimumHeight(300);
    m_dteStart->setCalendarPopup(true);
    m_dteEnd->setCalendarPopup(true);
    m_grid->addWidget(m_customPlot,  0, 0, 8, 15 );
    m_grid->addWidget(m_buttonClose, 0, 16 );
    m_grid->addWidget(m_lReplotTime, 9, 0 );
    m_grid->addWidget(m_nData,       9, 3 );
    m_grid->addWidget(m_dteStart,    9, 12 );
    m_grid->addWidget(m_dteEnd,      9, 13 );
    m_grid->addWidget(m_cbAlgorithm, 9, 14 );
    m_widget->setLayout(m_grid);
    m_cbAlgorithm->setMinimumHeight(25);
    SetStyle();
    m_customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

Graph::~Graph()
{

}

void Graph::changeColor(QColor newColor)
{
    color = newColor;
}

void Graph::setField(const QVector<AlgorithmComboBoxEl> &el)
{
    comboBoxEl = &el;
    for(const auto& x : *comboBoxEl) {
        m_cbAlgorithm->addItem(x.text);
    }
}

void Graph::replot(QColor color)
{
    if(m_customPlot->graphCount() != 0)
    {
        replot(0, color);
    }
    for(auto i = 1; i < m_customPlot->graphCount() && i < m_storage->size(); ++i)
    {
        replot(i, m_storage->at(i), color);
    }
}

void Graph::replot(int id, QColor color)
{
    if(!comboBoxEl || m_algIndex >= comboBoxEl->size() || !m_storage || id >= m_storage->size()) {
        return;
    }
    auto graph = m_customPlot->graph(id);
    bool xEx, yEx;
    auto yr = m_storage->at(id)->valueRange(yEx);
    auto xr = m_storage->at(id)->keyRange(xEx);
    if(!xEx || !yEx) {
        graph->data()->clear();
    }
    else {
        xRange = xr;
        yRange = yr;
        auto alg = (*comboBoxEl)[m_algIndex];
        graph->setData(alg.function(m_storage->at(id), alg.kFunc(xRange, yRange)));
        auto lower = QDateTime::fromTime_t(xRange.lower);
        auto upper = QDateTime::fromTime_t(xRange.upper);
        m_dteStart->setDateTimeRange(lower, upper);
        m_dteEnd->setDateTimeRange(lower, upper);
        m_dteStart->setDateTime(lower);
        m_dteEnd->setDateTime(upper);
    }
    QTime timer;
    timer.start();
    m_customPlot->xAxis->setRange(xRange);
    m_customPlot->yAxis->setRange(yRange);
    m_customPlot->graph()->setPen(QColor(color));
    m_customPlot->replot();
    m_lReplotTime->setText("Replot time:\t" + QString::number(timer.elapsed()) + "ms");
    m_nData->setText("Data count:\t" + QString::number(graph->data()->size()));
}

void Graph::replot(int id, const QSharedPointer<QCPGraphDataContainer>& data, QColor color)
{
    if(!comboBoxEl || m_algIndex >= comboBoxEl->size() || !m_storage) {
        return;
    }
    auto graph = m_customPlot->graph(id);
    bool xEx, yEx;
    auto yr = data->valueRange(yEx);
    auto xr = data->keyRange(xEx);
    if(!xEx || !yEx) {
        graph->data()->clear();
    }
    else {
        xRange.expand(xr);
        yRange.expand(yr);
        auto alg = (*comboBoxEl)[m_algIndex];
        graph->setData(alg.function(data, alg.kFunc(xRange, yRange)));
    }
    QTime timer;
    timer.start();
    m_customPlot->xAxis->setRange(xr);
    m_customPlot->yAxis->setRange(yr);
    m_customPlot->graph()->setPen(QColor(color));
    m_customPlot->replot();
    m_lReplotTime->setText("Replot time:\t" + QString::number(timer.elapsed()) + "ms");
    m_nData->setText("Data count:\t" + QString::number(graph->data()->size()));
}

void Graph::setStorage(const storage_t& data)
{
    m_storage = data;
    m_customPlot->clearGraphs();
    for(auto i = 0; i < m_storage->size(); ++i)
    {
        m_customPlot->addGraph();
    }
}

//                  Slots

void Graph::currentAlgChanged(int index)
{
    m_algIndex = index;
    replot(0, color);
}

void Graph::dteStartChanged(const QDateTime& dateTime)
{
    m_dteEnd->setMinimumDateTime(m_dteStart->dateTime());
    for(auto i = 0; i < m_customPlot->graphCount() && i < m_storage->size(); ++i)
    {
        auto s2 = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer(*m_storage->at(i)));
        s2->removeBefore(static_cast<double>(m_dteStart->dateTime().toTime_t()));
        s2->removeAfter(static_cast<double>(m_dteEnd->dateTime().toTime_t()));
        replot(i, s2, color);
    }
}

void Graph::dteEndChanged(const QDateTime& dateTime)
{
    m_dteStart->setMaximumDateTime(m_dteEnd->dateTime());
    for(auto i = 0; i < m_customPlot->graphCount() && i < m_storage->size(); ++i)
    {
        auto s2 = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer(*m_storage->at(i)));
        s2->removeBefore(static_cast<double>(m_dteStart->dateTime().toTime_t()));
        s2->removeAfter(static_cast<double>(m_dteEnd->dateTime().toTime_t()));
        replot(i, s2, color);
    }
}

void Graph::btnCloseClicked(bool checked)
{
    emit close(this);
    this->~Graph();
}

void Graph::SetStyle()
{
    StyleSheet::GetStyleFromFile();
    m_dteEnd->setStyleSheet(StyleSheet::GetBlueStyleDatePicker());
    m_dteStart->setStyleSheet(StyleSheet::GetBlueStyleDatePicker());
    m_buttonClose->setStyleSheet(StyleSheet::GetBlueStyleBtn());
    m_cbAlgorithm->setStyleSheet(StyleSheet::GetBlueStyleComboBox());
    m_widget->setStyleSheet(StyleSheet::GetBlueDarkStyleWidget());
}
