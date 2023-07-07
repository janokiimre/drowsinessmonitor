#include "ppgsensor.h"

#include <QStringRef>
#include <QDebug>

PPGSensor::PPGSensor(QString portname_)
{
//    cms50d = false;

    portname = portname_;
    run_flag = true;
    ppg_port = new QSerialPort(this);

    ppg_port->setBaudRate(QSerialPort::Baud38400);
    ppg_port->setParity(QSerialPort::NoParity);
    ppg_port->setDataBits(QSerialPort::Data8);
    ppg_port->setStopBits(QSerialPort::OneStop);


    /*
     * else{
        ppg_port->setBaudRate(QSerialPort::Baud19200);
        ppg_port->setParity(QSerialPort::OddParity);
        ppg_port->setDataBits(QSerialPort::Data8);
        ppg_port->setStopBits(QSerialPort::OneStop);
        ppg_port->setFlowControl(QSerialPort::SoftwareControl);
    }*/

    ppg_port->setPortName(portname);
}

PPGSensor::~PPGSensor()
{
    if (ppg_port->isOpen()) ppg_port->close();
}

void PPGSensor::doWork()
{
    qDebug()<< portname;
    ppg_port->open(QIODevice::ReadOnly);
    if (ppg_port->isOpen()){
        qDebug() << "ppg open";
        QByteArray responseData;
        QString ppg_answer;
        QByteArray pulseRate, pulseWaveform, bloodSpO2;
        pulseRate.resize(2);

        bool isError = false;
        float tempPulseRate, lastPulseRate = 0, lastWaveForm = 0, tempSpO2, lastSpO2 = 0;

      /*  if(cms50d == true){
//            pulseRate.resize(2);
            QVector<bool> validity = {true,false,false,false,false};
            while(run_flag){
                    while (ppg_port->waitForReadyRead(1000) && responseData.length()<5 && run_flag) {
                        responseData += ppg_port->readAll();
                    }
                    if (responseData.size()==5){
                        for (int i=0; i<5; i++){
                            if (validity.at(i)!= ((responseData.at(i) & 0x80) != 0)) isError=true;
                        }
                    }
                    if (!isError) { // valid package
                        pulseRate[0] = (responseData.at(2) & 0x40) << 1;
                        tempPulseRate = float(pulseRate[0] | (responseData.at(3) & 0x7f));
                        if(tempPulseRate>=30.0) { // otherwise if finger moves it can send -121,-120 and 1,3,7
                            lastPulseRate = tempPulseRate;
                        }
                        if (float(responseData.at(1))>=0.0) lastWaveForm = float(responseData.at(1));
                        tempSpO2 = float(responseData.at(4) & 0x7f); // otherwise if finger moves it can send -121,-120 and 1,3,7
                        if (tempSpO2>=70.0) lastSpO2 = tempSpO2;

                        emit SignalSendSensor(lastPulseRate,lastWaveForm,lastSpO2);

                        responseData.clear();
                        isError=false;
                    }
                    else { // invalid package, remove until expectedSize-1
                        while (responseData.size()>4){
                            responseData.remove(0,1);
                        }
                        isError=false;
                    }
            }
        }

        else{ */
            float pulse = 0.0;
            bool ok;

            while(run_flag){
                bool data_found = false;
                while (ppg_port->waitForReadyRead(1000) && run_flag) {
                    responseData = ppg_port->readAll();
                    if (responseData=="\xFA"){
                        while (ppg_port->waitForReadyRead(1000) && responseData.length()<11 && run_flag) { //5-->11
                            responseData += ppg_port->readAll();
                        }
//                        QString DataAsString = QTextCodec::codecForMib(106)->toUnicode(responseData);
//                        QString DataAsString = QByteArray::number(responseData);
                        QString DataAsString = QString (responseData.toHex());
                        lastWaveForm = DataAsString.midRef(2,4).toFloat();
                        lastPulseRate = DataAsString.midRef(6,4).toFloat();
                        lastSpO2 = DataAsString.midRef(10,4).toFloat();

                        if (lastPulseRate==9999) lastPulseRate=0;
                        if (lastSpO2==9999) lastSpO2=0;

                        emit SignalSendSensor(lastPulseRate,lastWaveForm,lastSpO2);
                        //qDebug() << "spo2: " << lastSpO2;

                }
            }
        }
    }
    else{
//        QMessageBox msgBox;
//        msgBox.setWindowTitle("Cannot open PPG sensor");
//        msgBox.setText("Cannot open PPG sensor, please check cables and the selected portname.");
//        msgBox.setIcon(QMessageBox::Warning);
//        msgBox.setStandardButtons(QMessageBox::Ok);
//        msgBox.setDefaultButton(QMessageBox::Ok);
//        msgBox.raise();
//        msgBox.setModal(true);
    }
    if (ppg_port->isOpen()) ppg_port->close();
}
