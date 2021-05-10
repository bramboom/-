#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "datadecimator.h"
#include "graph.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    using storage_t = QSharedPointer<QCPGraphDataContainer>;


    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool read_from_file(const QString& path);

private:
    QWidget* cw = new QWidget;
    QGridLayout* grid = new QGridLayout;
    Ui::MainWindow* ui;
    QSize graphSize {10, 6};
    size_t gridRow;

    QVector<QSharedPointer<Graph>> graphField;
    QVector<AlgorithmComboBoxEl> comboBoxAlg {
        { [](const storage_t& data, double sv) { return data; }, " None" },
        { DataDecimator::douglas_peucker, "Douglas-Peucker" }
    };
    storage_t storage { new storage_t::value_type };
    QSharedPointer<QLineEdit> lePath { new QLineEdit("../Data1.txt") };
    QSharedPointer<QPushButton> btnPath { new QPushButton("Set Path") };
    QSharedPointer<QPushButton> btnAddGraph { new QPushButton("Add Graph") };
    void addGraph();

public slots:
    void btnPathClicked(bool checked);
    void btnAddGraphClicked(bool checked);
};
#endif // MAINWINDOW_H
