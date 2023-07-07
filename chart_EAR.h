#ifndef CHART_EAR_H
#define CHART_EAR_H

#include "controller.h"

#include <QtCharts>
#include <QTimer>
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QPen>

//QT_BEGIN_NAMESPACE
//class QSplineSeries;
//class QValueAxis;
//QT_END_NAMESPACE

//QT_USE_NAMESPACE


//![1]
class Chart_EAR: public QChart
{
    Q_OBJECT
public:
    Chart_EAR(app::Controller *controller, QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = {});
    virtual ~Chart_EAR();

public slots:
    void handleTimeout();
    void startEARPlot();

private:
    app::Controller *m_controller;
    QTimer m_timer;
    QLineSeries *m_series;
    QStringList m_titles;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
    qreal m_step;
    qreal m_x;
    qreal m_y;
    QPen pen;


};
//![1]
#endif // CHART_EAR_H
