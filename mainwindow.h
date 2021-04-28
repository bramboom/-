#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMessageBox>
#include <QLineEdit>
#include <qcustomplot.h>
#include <QDateTimeEdit>

template<typename Tp>
using Pair = QPair<Tp, Tp>;

using storage_t = QVector<QPair<QDateTime, double>>;

void debug(const QString& str);
bool read_from_file(const QString&, storage_t&);

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


template <typename Func, typename... Args>
int timer(Func&& f, Args&&... args)
{
    QTime timer;
    timer.start();
    f(args...);
    return timer.elapsed();
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void work();
    void jjjj(int);

private:
    Ui::MainWindow *ui;
    storage_t storage;
    QLineEdit* filename_edit;
    QVector<QCustomPlot*> plot;
    QVector<QLabel*> label;
    QDateTimeEdit* dateTime_edit;

public slots:
    void set_filename_clicked(bool checked)
    {
        if(!read_from_file(filename_edit->text(), storage)) {
            debug("This file does not exists\nfu");
        }
        else {
            dateTime_edit->setMinimumDateTime(storage.front().first);
            dateTime_edit->setMaximumDateTime(storage.back().first);
            work();
        }
    }
    void scale_x()
    {
        int scalek = 10000;
        auto r = plot.front()->xAxis->range();
        plot.front()->xAxis->setRange(r.lower + scalek, r.upper - scalek);
        plot.front()->replot();
    }
};
#endif // MAINWINDOW_H
