#include "propswidget.h"

#include <QtWidgets>


PropsWidget::PropsWidget(app::Controller *controller, QWidget *parent) : QTableWidget(parent)
{
    m_controller = controller;
    QObject::connect(&m_timer, &QTimer::timeout, this, &PropsWidget::handleRefresh);
    m_timer.setInterval(100);
    m_timer.start();

}

PropsWidget::~PropsWidget()
{
    delete m_controller;
}

void PropsWidget::handleRefresh() {

    this->setItem(0, 0, new QTableWidgetItem(QString::number(m_controller->camWidth())));
    this->setItem(1, 0, new QTableWidgetItem(QString::number(m_controller->camHeight())));
    this->setItem(2, 0, new QTableWidgetItem(QString::number(m_controller->FPS)));
    this->setItem(3, 0, new QTableWidgetItem(QString::number(m_controller->fullyOpenedAvgEAR)));
    this->setItem(4, 0, new QTableWidgetItem(QString::number(m_controller->fullyClosedAvgEAR)));
    this->setItem(5, 0, new QTableWidgetItem(QString::number(m_controller->normalAvgEAR)));
    this->setItem(6, 0, new QTableWidgetItem(QString::number(m_controller->currentEAR)));
    this->setItem(7, 0, new QTableWidgetItem(QString::number(m_controller->currentBlinkingTime)));
    this->setItem(8, 0, new QTableWidgetItem(QString::number(m_controller->blinkingPerMin)));
    this->setItem(9,0, new QTableWidgetItem(QString::number(m_controller->currentPerclos)));
    this->setItem(10,0, new QTableWidgetItem(QString::number(m_controller->yawningPerMin)));
}
