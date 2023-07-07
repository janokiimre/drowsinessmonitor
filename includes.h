// this is for all the common includes, so all classes can just include this header
// this header file has no .cpp!

#ifndef INCLUDES_H
#define INCLUDES_H

#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>

//#include <torch/torch.h>

#include <pylon/PylonIncludes.h>
#include <pylon/usb/BaslerUsbCamera.h>
#include <pylon/usb/BaslerUsbInstantCamera.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
//#include <opencv2/cudaimgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>
#include <fstream>

#include <QVideoSurfaceFormat>
#include <QCheckBox>
#include <QColor>
#include <QDateTime>
#include <QDebug>
#include <QDesktopWidget>
#include <QDir>
#include <QGraphicsSceneMouseEvent>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QKeyEvent>
#include <QLabel>
#include <QLCDNumber>
#include <QMainWindow>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPixmap>
#include <QPushButton>
#include <QSize>
#include <QSlider>
#include <QSplashScreen>
#include <QStyle>
#include <QStyleFactory>
#include <QThread>
#include <QTimer>
#include <QToolTip>
#include <QVector>
#include <QAbstractVideoSurface>
#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFontDatabase>
#include <QtCharts/QChartView>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QGuiApplication>
#include <QtCharts>
#include <QTimer>

//#include <qcustomplot.h>



// undef slots needed, else there will be a confusion with qt slots and torch slots
//#undef slots
//#define slots Q_SLOTS

#endif // INCLUDES_H

