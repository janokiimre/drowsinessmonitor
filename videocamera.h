// class for camera handling

#ifndef VIDEOCAMERA_H
#define VIDEOCAMERA_H

#include "includes.h"

// Namespace for using pylon objects. we should not use it like this tho...
using namespace Pylon;
using namespace std;

class VideoCamera : public QObject
{
    Q_OBJECT

public:
    VideoCamera();
    VideoCamera(int width_, int height_, int offset_x_, int offset_y_, int fps_, QString save_path_, bool isPylon_);
    ~VideoCamera();
    // This will contain the converted pylon image
    unsigned char *MyOutputBuffer = nullptr; // Buffer for color conversion

    // This will cast the pylon image to cv::Mat
    cv::Mat GrabResult2CvMat(CGrabResultPtr& ptrGrabResult);

    cv::Mat cv_img;
    cv::Mat mat,sendmat;

    bool run_flag;
    bool isPylon = true;

    CPylonImage image;

    // camera sensor resolution
    int max_width;
    int max_height;

    // size of raw image:
    int raw_width;
    int raw_height;

    // raw image offsets:
    int raw_offset_x;
    int raw_offset_y;

    int FPS;
    QString save_path;

signals:
    void SignalSendFrame(cv::Mat frame);
    void SignalSendFPS(int FPS);
    void SignalSendCamParams(int raw_width, int raw_height, int raw_offset_x, int raw_offset_y, int FPS);
    void SignalCamFinished();
    void SignalCameraFound(bool found);

private slots:
    void doWork();
};

#endif // VIDEOCAMERA_H
