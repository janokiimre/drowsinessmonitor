#ifndef CHART_H
#define CHART_H

#include "controller.h"

#include <QtCharts>
#include <QTimer>

//QT_BEGIN_NAMESPACE
//class QSplineSeries;
//class QValueAxis;
//QT_END_NAMESPACE

//QT_USE_NAMESPACE


//![1]
class Chart: public QChart
{
    Q_OBJECT
public:
    Chart(app::Controller *controller, QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = {});
    virtual ~Chart();

public slots:
    void handleTimeout();
    void startPPGPlot();

private:
    app::Controller *m_controller;
    QTimer m_timer;
    QSplineSeries *m_series;
    QStringList m_titles;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
    qreal m_step;
    qreal m_x;
    qreal m_y;
};
//![1]
#endif // CHART_H
