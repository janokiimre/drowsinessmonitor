#ifndef LANDMARK_H
#define LANDMARK_H

#include "lite/lite.h"

#include <QMainWindow>
#include <QObject>
#include <QQuickItem>
#include <QSharedDataPointer>
#include <QWidget>

class LandmarkData;

class Landmark: public QObject
{
    Q_OBJECT
public:
    Landmark();
    ~Landmark();
    double EAR_THRESHOLD;

signals:
    void signalSendLandmarks(lite::types::Landmarks);
    void signalSendBoxes(std::vector<lite::types::Boxf>);
    void signalSendAngles(lite::types::EulerAngles);
    void signalOpeness(bool,bool,bool);
    void calculated_EAR(std::vector<double>);

private:
    lite::onnxruntime::cv::face::detect::UltraFace *ultraface = nullptr;
    lite::onnxruntime::cv::face::detect::RetinaFace *retinaface = nullptr;
    lite::onnxruntime::cv::face::align::PIPNet68 *pipnet68 = nullptr;
    lite::onnxruntime::cv::face::pose::FSANet *var_fsanet = nullptr;
    lite::onnxruntime::cv::face::pose::FSANet *conv_fsanet = nullptr;

    std::vector<lite::types::Boxf> detected_boxes;
    lite::types::Landmarks landmarks;
    lite::types::EulerAngles var_euler_angles, conv_euler_angles, euler_angles;

    double distanceCalculate(double x1, double y1, double x2, double y2);
    std::vector<double> EAR_eye(lite::types::Landmarks landmarks);
    cv::Point2f upperLip(lite::types::Landmarks landmarks);
    cv::Point2f lowerLip(lite::types::Landmarks landmarks);
    void yawning(lite::types::Landmarks landmarks);
    bool leftEyeOpened, mouthOpened, rightEyeOpened;

private slots:
    // run network
    void doWork(cv::Mat input_img);

};

#endif // LANDMARK_H
