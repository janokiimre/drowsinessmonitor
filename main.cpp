#include "includes.h"
#include "lite/lite.h"
#include "chart.h"
#include "chart_EAR.h"
#include "controller.h"
#include "propswidget.h"

#include <variant>
#include <string>
#include <cassert>
#include <iostream>

#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_USE_NAMESPACE

// register stuff so we can use them in signal-slots
Q_DECLARE_METATYPE(cv::Mat)
Q_DECLARE_METATYPE(QVector<double>)
Q_DECLARE_METATYPE(std::vector<cv::Rect>)
Q_DECLARE_METATYPE(QVector<int>)
Q_DECLARE_METATYPE(cv::Rect)
Q_DECLARE_METATYPE(QList<cv::Point2d>)
Q_DECLARE_METATYPE(QVector<cv::Point2d>)
Q_DECLARE_METATYPE(std::vector<cv::Point2d>)
Q_DECLARE_METATYPE(QList<cv::Mat>)
Q_DECLARE_METATYPE(std::vector<lite::types::Boxf>)
Q_DECLARE_METATYPE(lite::types::Landmarks)
Q_DECLARE_METATYPE(lite::types::EulerAngles);

void registerQMLTypes()
{
   qmlRegisterType<app::Controller>();
   qRegisterMetaType<cv::Mat>();
   qRegisterMetaType<std::vector<lite::types::Boxf>>();
   qRegisterMetaType<lite::types::Landmarks>();
   qRegisterMetaType<lite::types::EulerAngles>();
   qRegisterMetaType<QVector<double> >();
   qRegisterMetaType<std::vector<cv::Rect> >();
   qRegisterMetaType<std::vector<double> >();
   qRegisterMetaType<cv::Rect>();
   qRegisterMetaType<QVector<int> >("QVector<int>");
   qRegisterMetaType<QString>();
   qRegisterMetaType<QList<cv::Point2d>>();
   qRegisterMetaType<QVector<cv::Point2d>>();
   qRegisterMetaType<std::vector<cv::Point2d>>();
   qRegisterMetaType<QList<cv::Mat>>();
}

void InstallFont()
{
    qint32 fontId = QFontDatabase::addApplicationFont("C:/Users/Dorka/Documents/gui_onnx/font/Technology-Bold.ttf");
    QStringList fontList = QFontDatabase::applicationFontFamilies(fontId);

    QString family = fontList.first();
    QApplication::setFont(QFont(family));
}

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);

    registerQMLTypes();

    app::Controller controller;
    QMainWindow window;
    QMainWindow window2;

    QWidget window3;

    PropsWidget *table = new PropsWidget(&controller, &window3);
    table->setRowCount(11);
    table->setColumnCount(1);
    table->setGeometry(10, 10, 300, 450);
    table->setFont(QFont("Bookman"));
    window3.show();

    QStringList rowHeaders;
    rowHeaders << "Width" << "Height" << "FPS" << "Fully opened EAR" << "Fully closed EAR" << "Normal EAR" << "Current EAR" << "Last blinking time" << "Blink/min" << "Current Perclos" << "Yawning/min";
    table->setVerticalHeaderLabels(rowHeaders);

    QStringList columnHeaders;
    columnHeaders << "Properties";
    table->setHorizontalHeaderLabels(columnHeaders);

    Chart *chart = new Chart(&controller);
    Chart_EAR *chart2 = new Chart_EAR(&controller);
    chart->setTitle("Heart Rate");
    chart->setFont(QFont("Bookman",20));
    chart->legend()->hide();
    chart->setAnimationOptions(QChart::AllAnimations);

    chart2->setTitle("EAR");
    chart2->setFont(QFont("Bookman",20));
    chart2->legend()->hide();
    chart2->setAnimationOptions(QChart::AllAnimations);

    QObject::connect(&controller, &app::Controller::startPPGSignal, chart, &Chart::startPPGPlot);
    QObject::connect(&controller, &app::Controller::startEARPlotSignal, chart2, &Chart_EAR::startEARPlot);

    QChartView chartView(chart);
    QChartView chartView2(chart2);

    chartView.setRenderHint(QPainter::Antialiasing);
    window.setCentralWidget(&chartView);
    window.resize(450, 200);
    window.show();

    chartView2.setRenderHint(QPainter::Antialiasing);
    window2.setCentralWidget(&chartView2);
    window2.resize(800, 300);
    window2.show();

    InstallFont();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("controller", &controller);
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
