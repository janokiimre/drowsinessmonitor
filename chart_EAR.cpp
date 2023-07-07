#include "chart_EAR.h"
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCore/QRandomGenerator>
#include <QtCore/QDebug>

Chart_EAR::Chart_EAR(app::Controller *controller, QGraphicsItem *parent, Qt::WindowFlags wFlags):
    QChart(QChart::ChartTypeCartesian, parent, wFlags),
    m_series(0),
    m_axisX(new QValueAxis()),
    m_axisY(new QValueAxis()),
    m_step(0),
    m_x(70),
    m_y(0.1)
{
    QObject::connect(&m_timer, &QTimer::timeout, this, &Chart_EAR::handleTimeout);
    m_timer.setInterval(100);
    m_controller = controller;
    m_series = new QLineSeries(this);
    pen.setWidth(3);
    m_series->setPen(pen);
    pen.setColor(Qt::green);
    m_series->append(m_x, m_y);

    addSeries(m_series);

    addAxis(m_axisX,Qt::AlignBottom);
    addAxis(m_axisY,Qt::AlignLeft);
    m_series->attachAxis(m_axisX);
    m_series->attachAxis(m_axisY);
    m_axisX->setTickCount(100);
    m_axisX->setRange(0, 100);
    m_axisY->setRange(0, 0.6);
}

Chart_EAR::~Chart_EAR()
{
    delete m_axisX;
    delete m_axisY;
    delete m_series;
    delete m_controller;
}

void Chart_EAR::startEARPlot(){
    m_timer.start();
}

void Chart_EAR::handleTimeout()
{
    qreal x = plotArea().width() / m_axisX->tickCount();
    qreal y = (m_axisX->max() - m_axisX->min()) / m_axisX->tickCount();

    m_x += y;
    m_y = m_controller->readCurrentEAR();
    m_series->setPen(pen);
    m_series->append(m_x, m_y);
    scroll(x, 0);
    if (m_x == 1000)
        m_timer.stop();
}


