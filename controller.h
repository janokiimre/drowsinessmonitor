#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "includes.h"
#include "lite/lite.h"
#include "ppgsensor.h"
#include "videocamera.h"
#include "landmark.h"

#include <chrono>
#include <thread>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
//#include <opencv2/cudaimgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>


using namespace std::chrono;


namespace app{

class Controller: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QAbstractVideoSurface *videoSurface READ videoSurface \
               WRITE setVideoSurface NOTIFY videoSurfaceChanged)

    Q_PROPERTY(QString currentState READ currentState WRITE setCurrentState NOTIFY currentStateChanged)
    Q_PROPERTY(QString currentSPO2 READ currentSPO2 NOTIFY currentSPO2Changed)
    Q_PROPERTY(int currentPulse READ currentPulse NOTIFY currentPulseChanged)
    Q_PROPERTY(int cam_width READ camWidth CONSTANT)
    Q_PROPERTY(int cam_height READ camHeight CONSTANT)
    Q_PROPERTY(bool start_timer READ startTimer NOTIFY timerChanged)
    Q_PROPERTY(bool leftEyeClosed READ closedLeftEye NOTIFY leftEyeClosedChanged)
    Q_PROPERTY(bool rightEyeClosed READ closedRightEye NOTIFY rightEyeClosedChanged)
    Q_PROPERTY(bool mouthOpened READ openedMouth NOTIFY openedMouthChanged)
    Q_PROPERTY(double currentEAR READ readCurrentEAR WRITE setCurrentEAR NOTIFY currentEARChanged)

    // threads to run the classes
    QThread saverWorker;
    QThread sensorWorker;
    QThread videoCameraWorker;
    //QThread HR_networkWorker;
    QThread landmarkWorker;


public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller();

    QString currentState() { return m_currentState; }

    void setVideoSurface(QAbstractVideoSurface *surface);
    void setCurrentState(QString state);
    void setCurrentEAR( double EAR);
    QAbstractVideoSurface *videoSurface() const;
    void setupLandmark();

    QString currentSPO2() const { return QString::number(SpO2) + '%'; }
    float currentPulse() const { return PR; }
    int camWidth() const { return cam_width; }
    int camHeight() const { return cam_height; }
    bool closedLeftEye() { return leftEyeClosed; }
    bool closedRightEye() { return rightEyeClosed; }
    bool openedMouth() { return mouthOpened; }
    bool startTimer() { return start_timer; }
    double readCurrentEAR() { return currentEAR; }

    void calcPERCLOS(double EAR);

    void libtorch_trial();
    bool leftEyeClosed, rightEyeClosed, mouthOpened, start_timer;
    std::vector<std::vector<double>> fullyOpenedEyes, fullyClosedEyes, normalEyes;
    double normalAvgEAR=0, fullyClosedAvgEAR=0, fullyOpenedAvgEAR = 0;
    double currentEAR = 0;
    long long currentBlinkingTime= 0.0;
    std::vector<double> observedFrames;
    std::vector<long long> savedBlinkingTimes;
    int blinkCounter= 0, FPS = 0;

    std::vector<double> observedEARsForPERCLOS;
    double currentPerclos;
    int yawningPerMin = 0, blinkingPerMin = 0;



signals:
    void SignalStartCamera(); // start cam
    void SignalStartSensor(); // start ppg sensor

    //void SignalSendToPulseNet(std::vector<torch::jit::IValue> pulsenet_inputs); // send the inputs to pulse net
    void signalSendToNets(cv::Mat mat);    // send image to landmark detector

    void startLandmarkSignal();
    void startCameraSignal();
    void startPPGSignal();
    void startEARPlotSignal();
    void videoSurfaceChanged();
    void currentStateChanged();

    void stopAll();

    void saveFrame(QImage frame);

    void currentSPO2Changed(float SpO2);
    void currentPulseChanged(float PR);
    void currentEARChanged(double EAR);

    void signalChangeState(QString state);

    void leftEyeClosedChanged(bool leftEyeClosed);
    void rightEyeClosedChanged(bool rightEyeClosed);
    void openedMouthChanged(bool mouthOpened);

    void timerChanged(bool start_timer);

    void signalReadyForCalibration();
    void endOfTheCalibration();
    void startPERCLOSSignal();


private:
    QImage *mImage;
    QVideoSurfaceFormat *mSurfaceFormat;
    QString m_currentState;
    QAbstractVideoSurface *m_surface;
    PPGSensor *sensor = nullptr;
    VideoCamera *camera = nullptr;
    Landmark *landmark = nullptr;

//    HR_network *physnet = nullptr;

    //variables
    const double PI  =3.141592653589793238463;
    int cam_width, cam_height, cam_offset_x, cam_offset_y, save_FPS;
    float PR, waveForm, SpO2;
    bool isPylon, saving_frames, saving_data;
    QString save_path, portname;
    QImage displayed_frame;
    QDateTime date_time;
    std::vector<lite::types::Boxf> detected_boxes;
    lite::types::Landmarks landmarks;
    lite::types::EulerAngles euler_angles;
    cv::Rect rect;
    bool blinkStart=false;
    long long currentTimeMillis();
    double EAR_THRESHOLD;
    bool isBlinking;
    bool reached_t1, reached_t2, reached_t3, reached_t4;
    double eighty_percent_EAR, twenty_percent_EAR;
    double calculateAvgCalibEARs(std::vector<std::vector <double>> calib_ears);



    // variables
    int frame_counter = 0;
//    torch::Tensor im_container;
//    std::vector<torch::jit::IValue> pulsenet_inputs;
//    std::vector<cv::Point2d> landmarks;

    vector<cv::Point2f> imagePoints;
    QVector<double>output_vector;
    QVector<double>time_vector;

    high_resolution_clock::time_point startBlink, stopBlink, t1, t2, t3, t4, startOneMin, checkOneMin;

    // //////////////////// LANDMARK /////////////////////

    int run_count = 0;
    std::chrono::duration<double, std::milli> time_took;

//    // 3d and 2d image point containers for SolvePnP
    std::vector<cv::Point3d> model_points;
    std::vector<cv::Point3d> box_3d_points;
    std::vector<cv::Point2d> box_2d_points;
    std::deque<std::vector<cv::Point2d>> box_2d_list;

//    // camera matrix for SolvePnP
    double focal_length = 800;
    double center = 400;
    cv::Mat camera_matrix = (cv::Mat_<double>(3,3) << focal_length, 0, center, 0, focal_length, center, 0, 0, 1);
    cv::Mat distCoeffs = cv::Mat::zeros(4, 1, cv::DataType<double>::type);; // empty = no distortion
    cv::Mat rotation_vector;
    cv::Mat transform_vector;
    std::vector<cv::Point2d>nose_end_2d; // not used

    // ////////////////// END ///////////////////////////

    QImage Mat2QImage(const cv::Mat &src); // convert mat to QImage for display
//    torch::Tensor PulseIm2Tensor(cv::Mat mat); // convert mat to Tensor for pulse network
//    torch::Tensor LandmarkIm2Tensor(cv::Mat mat); // convert mat to Tensor for landmark network
    cv::Mat drawAttentionBox(cv::Mat _image); // nomen est omen

    //void functions

    void calcAttentionBox(std::vector<cv::Point2f> _landmarks); // do SolvePnP and related stuff
    std::vector<cv::Point2d> get2dPoints(std::vector<double>_rotation_vector, std::vector<double>_transform_vector,
                                         cv::Mat _camera_matrix, std::vector<double> _distCoeffs); // do projection

private slots:
    void startCameraSlot();
    void startPPGSensor();
    void startPPGPlot();
    void startLandmark();

    void saveFullyOpenedLandmarks(std::vector<double> EAR);
    void saveFullyClosedLandmarks(std::vector<double> EAR);
    void saveNormalLandmarks(std::vector<double> EAR);
    void frameEAR(std::vector<double> EAR);

    void SlotCameraFound(bool isCam); // checking when cam is connected
    void saveQImage (QImage img);
    void SlotFrameGetter(cv::Mat frame); // camera frame getter
    void UpdateLandmarks(lite::types::Landmarks); // update landmarks for painting them on each frame
    void UpdateBBox(std::vector<lite::types::Boxf>);
    void SlotGetSensor(float PR, float waveForm, float SpO2); // get pulseoximeter data
    void openessChanged(bool left, bool mouth, bool right);
    void calibrationEARs();
    void observedBlinkingTime(double ear);

    void threadCarnage();
};

}

#endif // CONTROLLER_H
