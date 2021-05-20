#pragma once
#include <QMainWindow>
#include <QWidget>
#include "datadecimator.h"
#include "graph.h"
#include "quabrowser.h"
#include <opcuaclient/UaHistory.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    using storage_t = QSharedPointer<QCPGraphDataContainer>;
private:
    EleSyOpcUaClient::UaConnection::Ptr connection;
    EleSyOpcUaClient::UaHistory::Ptr    history;

private:
    Ui::MainWindow* ui;
    QWidget*        cw          { new QWidget };
    QGridLayout*    grid        { new QGridLayout };
    QSize           graphSize   {10, 6};
    size_t          gridRow     { };
    QList<Graph*>   graphField  { };
    QVector<storage_t>*       storage { new QVector<storage_t> };
    QMenu*          source      { new QMenu("source")};
    QPushButton*    btnSetPathFile   { new QPushButton()};
    QLineEdit*      lePath           { new QLineEdit()};
    QWidget*        widget           { new QWidget};
    QLabel*         lblPath          { new QLabel};
    QTime*          time             { new QTime};
    QColor*         lineColor        { new QColor(Qt::black)};
    QVector<AlgorithmComboBoxEl> comboBoxAlg {
        {
            [](const storage_t& data, double sv) { return storage_t{ new storage_t::value_type(*data) }; },
            " None" ,
            [](const QCPRange& xRange, const QCPRange& yRange) { return 0; }
        },
        {
            DataDecimator::douglas_peucker,
            "Douglas-Peucker",
            [](const QCPRange& xRange, const QCPRange& yRange) { return std::pow(yRange.upper - yRange.lower, 1 / 6); }
        },
        {
            DataDecimator::reumann_witkam,
            "Reumann-Witkam",
            [](const QCPRange& xRange, const QCPRange& yRange) { return (yRange.upper - yRange.lower) / 30.0; }
        },
        {
            DataDecimator::largest_triangle,
            "Largest triangle",
            [](const QCPRange& xRange, const QCPRange& yRange) { return xRange.upper - xRange.lower; }
        },
        {
            DataDecimator::longest_line,
            "Longest line",
            [](const QCPRange& xRange, const QCPRange& yRange) { return xRange.upper - xRange.lower; }
        },
        {
            DataDecimator::opheim,
            "Opheim",
            [](const QCPRange& xRange, const QCPRange& yRange) { return (xRange.upper - xRange.lower) / 700; }
        },
        {
            DataDecimator::lang,
            "Lang",
            [](const QCPRange& xRange, const QCPRange& yRange) { return (yRange.upper - yRange.lower) / 40.0;}
        }
    };
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool read_from_file(const QString& path);
    void addGraph();
    storage_t readFromServer(const QString &nodeId, const QDateTime& start, const QDateTime& end);

public slots:
    void btnSetFileClicked(bool);
    void btnSetServerClicked(bool);

    void btnAddGraphClicked(bool);
    void btnCloseGraphClicked(Graph*);
    void lePathEdited(const QString&);
    void serverUpdate(const QDateTime &startDateTime, const QDateTime &endDateTime);
    void inputFilePath(bool);
    void inputServerPath(bool);
    void changeLineColor(bool);
};
