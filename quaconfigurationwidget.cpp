#include "quaconfigurationwidget.h"

QUaConfigurationWidget::QUaConfigurationWidget(QWidget *parent)
    : QWidget(parent)
{
    dteStart->setCalendarPopup(true);
    dteEnd->setCalendarPopup(true);
    dteStart->setDisplayFormat("dd.MM.yyyy HH:mm:ss");
    dteEnd->setDisplayFormat("dd.MM.yyyy HH:mm:ss");
    grid->addWidget(list, 0, 0, 3, 5);
    grid->addWidget(lStart, 3, 0);
    grid->addWidget(dteStart, 3, 1);
    grid->addWidget(lEnd, 4, 0);
    grid->addWidget(dteEnd, 4, 1);
    grid->addWidget(btnUpdate, 5, 0);
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

