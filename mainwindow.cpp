#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QDateTime>
#include <qcustomplot.h>
#include <QDateTimeEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QtConcurrent/QtConcurrent>

void debug(const QString& str)
{
    QMessageBox mb;
    mb.setText(str);
    mb.show();
    mb.exec();
}

QCustomPlot* dateTime_double_graph(QWidget* parent = nullptr)
{
    auto customPlot = new QCustomPlot(parent);
    customPlot->xAxis->setTicker(QSharedPointer<QCPAxisTickerDateTime>(new QCPAxisTickerDateTime));
    return customPlot;
}

Pair<QCPRange> graph_storage(QCPGraph* graph, const storage_t& storage)
{
    double min = std::numeric_limits<double>::max();
    double max = std::numeric_limits<double>::min();
    for(const auto& x : storage) {
        graph->addData(x.first.toTime_t(), x.second);
        if(x.second > max) {
            max = x.second;
        }
        else if (x.second < min) {
            min = x.second;
        }
    }
    return Pair<QCPRange>(
        { static_cast<double>(storage.front().first.toTime_t()), static_cast<double>(storage.back().first.toTime_t()) },
        { min, max}
    );
}

void redraw_by_storage(QCustomPlot* graph, int id, const storage_t& storage)
{
    auto ranges = graph_storage(graph->graph(id), storage);
    graph->xAxis->setRange(ranges.first);
    graph->yAxis->setRange(ranges.second);
    graph->replot();
}

QCustomPlot* get_dtdg_with_size(int w, int h, QWidget* parent = nullptr)
{
    auto customPlot = dateTime_double_graph(parent);
    customPlot->setMinimumSize(w, h);
    customPlot->addGraph();
    return customPlot;
}


QLabel* get_label_fixed_width(int w, QWidget* parent = nullptr)
{
    auto label = new QLabel(parent);
    label->setFixedWidth(w);
    return label;
}

bool read_from_file(const QString& path, storage_t& storage)
{
    QFile file(path);
    storage.clear();
    bool res = false;
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while(!file.atEnd()) {
            auto line   = file.readLine();
            int  day    = 10*line[0] + line[1] - 528;
            int  month  = 10*line[3] + line[4] - 528;
            int  year   = 1000*line[6] + 100*line[7] + 10*line[8] + line[9] - 53328;

            int  hour;
            uint index  = 11;
            if(line[index + 7] == '\t') {
                hour    = line[index] - 48;
            }
            else {
                hour    = 10*line[index] + line[index + 1] - 528;
                ++index;
            }
            index += 2;
            int  minute = 10*line[index] + line[index + 1] - 528;
            index += 3;
            int  second = 10*line[index] + line[index + 1] - 528;
            storage.append({ QDateTime(QDate(year, month, day), QTime(hour, minute, second)), line.mid(index + 3).toDouble() });
        }
        file.close();
        res = true;
    }
    return res;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEditPath->setText("../GraphDecimator/Data1.txt");
    ui->plotFirst->addGraph(0);
    connect(ui->setPathButton, &QPushButton::clicked, this, &MainWindow::set_filename_clicked);
    connect(ui->closeButton, &QPushButton::clicked, this, &MainWindow::close);
    connect(ui->plusButton, &QPushButton::pressed, this, &MainWindow::scale_x);
    //this->resize(1800, 900);
/*
    plot.append(get_dtdg_with_size(640, 320));
    plot.back()->setMouseTracking(true);

    label.append(get_label_fixed_width(150));

    plot.append(get_dtdg_with_size(640, 320));

    label.append(get_label_fixed_width(150));

    filename_edit   = new QLineEdit("../GraphDecimator/Data1.txt");
    filename_edit->setMaximumWidth(150);
    auto set_file_btn = new QPushButton("set file");
    connect(set_file_btn, &QPushButton::clicked, this, &MainWindow::set_filename_clicked);

    dateTime_edit  = new QDateTimeEdit;
    dateTime_edit->setMaximumWidth(150);

    auto grid       = new QGridLayout;
    grid->addWidget(plot[0], 0, 0);
    grid->addWidget(plot[1], 1, 0);
    grid->addWidget(label[0], 0, 1);
    grid->addWidget(label[1], 1, 1);
    grid->addWidget(filename_edit, 2, 0);
    grid->addWidget(dateTime_edit, 2, 1);
    grid->addWidget(set_file_btn, 3, 0);

    auto widget     = new QWidget(this);
    widget->setLayout(grid);

    this->setCentralWidget(widget);
    auto plus_ebat = new QPushButton("+");
    connect(plus_ebat, &QPushButton::pressed, this, &MainWindow::scale_x);
    grid->addWidget(plus_ebat, 4, 0);*/
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::work()
{
    jjjj(0);
    jjjj(1);
    //auto f1 = QtConcurrent::run(this, &MainWindow::jjjj, 0);
    //auto f2 = QtConcurrent::run(this, &MainWindow::jjjj, 1);
}

void MainWindow::jjjj(int i)
{
    label[i]->setText(QString::number(timer(redraw_by_storage, plot[i], 0, storage)));
}
