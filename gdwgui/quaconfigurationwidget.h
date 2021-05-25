#pragma once
#include <QListWidget>
#include <QDateTimeEdit>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

class QUaConfigurationWidget : public QWidget
{
    Q_OBJECT
public:
    QGridLayout*    grid     { new QGridLayout(this) };
    QListWidget*    list     { new QListWidget };
    QLabel*         lStart   { new QLabel("Start time:") };
    QLabel*         lEnd     { new QLabel("End time:") };
    QDateTimeEdit*  dteStart { new QDateTimeEdit };
    QDateTimeEdit*  dteEnd   { new QDateTimeEdit };
    QPushButton*    btnUpdate{ new QPushButton("Update") };

public:
    explicit QUaConfigurationWidget(QWidget *parent = nullptr);
    void SetStyle();

signals:
    void dateTimeRangeUpdated(const QDateTime &startDateTime, const QDateTime &endDateTime);

private slots:
    void buttonUpdateClicked(bool checked);
};
