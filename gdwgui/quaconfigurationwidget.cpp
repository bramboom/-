#include "quaconfigurationwidget.h"
#include "stylesheet.h"

QUaConfigurationWidget::QUaConfigurationWidget(QWidget *parent)
    : QWidget(parent)
{
    dteStart->setCalendarPopup(true);
    dteEnd->setCalendarPopup(true);
    dteStart->setDisplayFormat("dd.MM.yyyy HH:mm:ss");
    dteEnd->setDisplayFormat("dd.MM.yyyy HH:mm:ss");
    grid->addWidget(list, 0, 0, 5, 5);
    grid->addWidget(lStart, 1, 5);
    grid->addWidget(dteStart, 1, 6);
    grid->addWidget(lEnd, 2, 5);
    grid->addWidget(dteEnd, 2, 6);
    grid->addWidget(btnUpdate, 4, 6);
    btnUpdate->setMinimumHeight(25);
    SetStyle();
    connect(dteStart,   &QDateTimeEdit::dateTimeChanged, dteEnd, &QDateTimeEdit::setMinimumDateTime);
    connect(dteEnd,     &QDateTimeEdit::dateTimeChanged, dteStart, &QDateTimeEdit::setMaximumDateTime);
    connect(btnUpdate,  &QPushButton::clicked, this, &QUaConfigurationWidget::buttonUpdateClicked);
    list->setSelectionMode(QAbstractItemView::SingleSelection);
    list->setDragEnabled(true);
    list->setDefaultDropAction(Qt::DropAction::MoveAction);
    list->setDragDropMode(QAbstractItemView::DragDrop);
    list->viewport()->setAcceptDrops(true);
    list->setDropIndicatorShown(true);
}

void QUaConfigurationWidget::buttonUpdateClicked(bool checked)
{
    emit dateTimeRangeUpdated(dteStart->dateTime(), dteEnd->dateTime());
}

void QUaConfigurationWidget::SetStyle()
{
    StyleSheet::GetStyleFromFile();
    btnUpdate->setStyleSheet(StyleSheet::GetStyleBtn());
    dteStart->setStyleSheet(StyleSheet::GetStyleDatePicker());
    dteEnd->setStyleSheet(StyleSheet::GetStyleDatePicker());
    list->setStyleSheet(StyleSheet::GetLightStyleWidget());
}

