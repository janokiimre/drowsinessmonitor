#ifndef PPGSENSOR_H
#define PPGSENSOR_H

#include <QObject>
#include <QSerialPort>


class PPGSensor : public QObject
{
    Q_OBJECT

public:
    PPGSensor(QString);

    ~PPGSensor();

    QString portname; // communication port
    bool run_flag;
//   bool cms50d = false; // csiptetős or medical grade sensor
    QSerialPort *ppg_port = nullptr;

private:

signals:
    void SignalSendSensor(float, float, float); // send value to mainwindow

private slots:
    void doWork();
};


#endif // PPGSENSOR_H
