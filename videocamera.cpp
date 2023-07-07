#include "videocamera.h"

#include <QDebug>

VideoCamera::VideoCamera()
{
    // Create output buffer to contain the result of the pixel type conversion of the pylon-image
    MyOutputBuffer = new uint8_t[2064*1544*3*2];
    save_path = "/home/";
    isPylon = true;
    run_flag = true;
}

VideoCamera::VideoCamera(int width_, int height_, int offset_x_, int offset_y_, int fps_, QString save_path_, bool isPylon_)
{
    raw_width = width_;
    raw_height = height_;
    raw_offset_x = offset_x_;
    raw_offset_y = offset_y_;
    FPS = fps_;
    save_path = save_path_;
    isPylon = isPylon_;

    run_flag = true;
}

VideoCamera::~VideoCamera()
{
    if (MyOutputBuffer != nullptr) delete MyOutputBuffer;
}

cv::Mat VideoCamera::GrabResult2CvMat(CGrabResultPtr &ptrGrabResult)
{

    if (ptrGrabResult->GrabSucceeded()) {

        // Create a converter to be able to convert the GrabResult to the suitable pixel format.
        CImageFormatConverter fc;
        fc.OutputPixelFormat = PixelType_RGB8packed;//PixelType_Mono8;//PixelType_BGR8packed;//PixelType_Mono8;
        fc.OutputBitAlignment = OutputBitAlignment_MsbAligned;

        image.AttachGrabResultBuffer(ptrGrabResult);

        try {
            fc.Convert(MyOutputBuffer,
                       raw_width * raw_height * 6,
                       (char *)image.GetBuffer(),
                       raw_width * raw_height * 2,
                       PixelType_BayerRG12, //da1280-54uc nel GB12 masiknal BG12
                       raw_width,
                       raw_height,
                       0,
                       ImageOrientation_TopDown
                       );
        }catch(cv::Exception e){
            std::cout << "error in /GrabResult2CvMat/fc.Convert()"  << e.what() << std::endl;
        }

        // convert the properly formatted pylon-image to cv::Mat
        cv_img = cv::Mat(ptrGrabResult->GetHeight(),  ptrGrabResult->GetWidth(), CV_8UC3, static_cast<uint8_t*>(MyOutputBuffer));
    }
    return cv_img;
}

void VideoCamera::doWork()
{
    try{
        // is initialized during the lifetime of this object.
        PylonAutoInitTerm autoInitTerm;
        // Create an instant camera object with the camera device found first.
        CBaslerUsbInstantCamera camera( Pylon::CTlFactory::GetInstance().CreateFirstDevice());
        emit SignalCameraFound(true);
        // We have to open the camera
        camera.Open();
        GenApi::INodeMap& nodemap = camera.GetNodeMap();

        if (isPylon == true) {
            raw_width = CIntegerParameter(nodemap, "Width").GetValue();
            raw_height = CIntegerParameter(nodemap, "Height").GetValue();
            if (!CBooleanParameter(nodemap, "CenterX").GetValue())
                raw_offset_x = CIntegerParameter(nodemap, "OffsetX").GetValue();
            if (!CBooleanParameter(nodemap, "CenterY").GetValue())
                raw_offset_y = CIntegerParameter(nodemap, "OffsetY").GetValue();
            FPS = int(camera.AcquisitionFrameRate.GetValue());
            max_width = CIntegerParameter(nodemap, "WidthMax").GetValue();
            max_height = CIntegerParameter(nodemap, "HeightMax").GetValue();
            CBooleanParameter(nodemap, "AcquisitionFrameRateEnable").SetValue(true);
            CFloatParameter(nodemap, "AcquisitionFrameRate").SetValue(FPS);

            emit SignalSendFPS(FPS);
            emit SignalCamFinished();
            qDebug() << "isPylon true";
        }
        else {
            CIntegerParameter(nodemap, "Width").SetValue(raw_width);
            CIntegerParameter(nodemap, "Height").SetValue(raw_height);
            CBooleanParameter(nodemap, "CenterX").SetValue(true);
            CBooleanParameter(nodemap, "CenterY").SetValue(true);
            max_width = CIntegerParameter(nodemap, "WidthMax").GetValue();
            max_height = CIntegerParameter(nodemap, "HeightMax").GetValue();
            CBooleanParameter(nodemap, "AcquisitionFrameRateEnable").SetValue(true);
            CFloatParameter(nodemap, "AcquisitionFrameRate").SetValue(float(FPS));
            CFloatParameter(nodemap, "AutoTargetBrightness").SetValue(0.4);
            CEnumParameter(nodemap, "ExposureAuto").SetValue("Once");
            qDebug() << "isPylon false";
        }

        if (MyOutputBuffer != nullptr) delete MyOutputBuffer;
        MyOutputBuffer = new uint8_t[max_width*max_height*3*2];

        std::cout << "Width: " <<raw_width << " Height: " << raw_height << "  FPS: " << FPS << " OffSetX: " << raw_offset_x <<
                     " Y: " << raw_offset_y << " maxW: " << max_width << " maxH: " << max_height << std::endl;

        GenApi::CEnumerationPtr(camera.GetNodeMap().GetNode("PixelFormat"))->FromString("BayerRG12"); // dart -> "BayerGR12", nem dart "BayerRG12"

        std::cout << "Using device " << camera.GetDeviceInfo().GetModelName() << endl;

        camera.StopGrabbing(); //finish previous task
        camera.StartGrabbing(); //start the new grabbing process

        // This smart pointer will receive the grab result data.
        CGrabResultPtr ptrGrabResult;

        while (camera.IsGrabbing())
        {
            // Wait for an image and then retrieve it. A timeout of 5000 ms is used.
            camera.RetrieveResult(1000, ptrGrabResult, TimeoutHandling_ThrowException);

            // Image grabbed successfully?
            if (ptrGrabResult->GrabSucceeded())
            {
                mat =  GrabResult2CvMat(ptrGrabResult);
                sendmat = mat.clone();

                emit SignalSendFrame(sendmat);
            }
            else
            {
                cout << "Error: " << ptrGrabResult->GetErrorCode() << " " << ptrGrabResult->GetErrorDescription() << endl;
            }
            if (run_flag == false) break;
        }

        }
        catch (const GenericException& e)
        {
            if (e.GetSourceLine() == 694) emit SignalCameraFound(false);
            // Error handling
            cerr << "An exception occurred." << endl << e.GetDescription() << endl;
        }
}
