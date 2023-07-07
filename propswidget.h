#ifndef PROPSWIDGET_H
#define PROPSWIDGET_H

#include "controller.h"
#include <QtWidgets>

class PropsWidget: public QTableWidget {
    Q_OBJECT
public:
    PropsWidget(app::Controller *controller, QWidget *parent = nullptr) ;
    ~PropsWidget();

public slots:
    void handleRefresh();

private:
    app::Controller *m_controller;
    QTimer m_timer;

};

#endif // PROPSWIDGET_H
