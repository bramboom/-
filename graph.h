#pragma once
#include "qcustomplot.h"
#include "util.h"

struct AlgorithmComboBoxEl
{
    using storage_t     = QSharedPointer<QCPGraphDataContainer>;
    using function_t    = std::function<storage_t(const storage_t&, double)>;
    using text_t        = QString;
    using kfunc_t       = std::function<double(const QCPRange&, const QCPRange&)>;

    function_t  function;
    QString     text;
    kfunc_t     kFunc;
};

class Graph : public QObject
{
    Q_OBJECT
public:
    using storage_t = QVector<QSharedPointer<QCPGraphDataContainer>>*;
    using algFunc_t = AlgorithmComboBoxEl::function_t;
private:
    const QVector<AlgorithmComboBoxEl>*   comboBoxEl      { nullptr };
    storage_t       m_storage       { };
    QWidget*        m_widget        { new QWidget };
    QGridLayout*    m_grid          { new QGridLayout };
    QPushButton*    m_buttonClose   { new QPushButton("x") };
    QCustomPlot*    m_customPlot    { new QCustomPlot };
    QDateTimeEdit*  m_dteStart      { new QDateTimeEdit };
    QDateTimeEdit*  m_dteEnd        { new QDateTimeEdit };
    QLabel*         m_nData         { new QLabel };
    QLabel*         m_lReplotTime   { new QLabel };
    QComboBox*      m_cbAlgorithm   { new QComboBox };
    std::size_t     m_algIndex      { };
    QCPRange        xRange          { };
    QCPRange        yRange          { };
    QColor          color           { };

public:
    Graph();
    virtual ~Graph();

    void setField(const QVector<AlgorithmComboBoxEl>& el);

    void replot(QColor color);
    void replot(int id, QColor color);
    void replot(int id, const QSharedPointer<QCPGraphDataContainer>& data, QColor color);
    void replot(int id, const  QSharedPointer<QCPGraphDataContainer>& data, const QCPRange& range, QColor color);

    void setStorage(const storage_t& data);
    void changeColor(QColor newColor);

    inline storage_t        storage()               { return m_storage; }
    inline QWidget*         widget()                { return &*m_widget; }
    inline QPushButton*     closeButton()           { return &*m_buttonClose; }
    inline QCustomPlot*     customPlot()            { return &*m_customPlot; }
    inline QDateTimeEdit*   dateTimeEditStart()     { return &*m_dteStart; }
    inline QDateTimeEdit*   dateTimeEditEnd()       { return &*m_dteEnd; }
    inline std::size_t      currentAlgIndex() const { return m_algIndex; }

signals:
    void close(Graph* widget);

public slots:
    void currentAlgChanged(int index);
    void dteStartChanged(const QDateTime& dateTime);
    void dteEndChanged(const QDateTime& dateTime);
    void btnCloseClicked(bool checked);
};
