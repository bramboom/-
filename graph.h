#pragma once
#include "qcustomplot.h"
#include "util.h"

struct AlgorithmComboBoxEl
{
    std::function<QSharedPointer<QCPGraphDataContainer>(const QSharedPointer<QCPGraphDataContainer>&, double)> function;
    QString text;
};

class Graph : public QObject
{
    Q_OBJECT
public:

    Graph();
    virtual ~Graph();

    QCustomPlot* plot() { return qcp; }

    void setField(const QVector<AlgorithmComboBoxEl>& el);

    void replot(int id = 0);
    void replot(int id, const QSharedPointer<QCPGraphDataContainer>& data);
    void replot(int id, const QSharedPointer<QCPGraphDataContainer>& data, const QCPRange& range);
    void setStorage(const QSharedPointer<QCPGraphDataContainer>& data);

    QSharedPointer<QCPGraphDataContainer> storage   { new QCPGraphDataContainer };
    const QVector<AlgorithmComboBoxEl>*   comboBoxEl      { nullptr };
    QCustomPlot*    qcp             { new QCustomPlot };
    QDateTimeEdit*  dteStart        { new QDateTimeEdit};
    QDateTimeEdit*  dteEnd          { new QDateTimeEdit};
    QLabel*         nData           { new QLabel() };
    QLabel*         replotingTime   { new QLabel() };
    QLabel*         lbnData         { new QLabel("Data count:") };
    QLabel*         lbreplotingTime { new QLabel("Replot time:") };
    QComboBox*      comboBoxAlg     { new QComboBox };
    QPushButton*    btnDeleteGraph  { new QPushButton("Delete graph")};
    size_t          currentAlg      { 0 };
    QCPRange        xRange          { };
    QCPRange        yRange          { };

public slots:
    void currentAlgChanged(int index);
    void dateChanged(const QDateTime& dateTime);
    void deleteGraph();
};
