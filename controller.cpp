#include "controller.h"
#include "landmark.h"
#include "lite/lite.h"

#include <fstream>

#include <QVideoSurfaceFormat>


namespace app{

    const static char *kStateHome = "home";

    Controller::Controller(QObject *parent) :
       QObject(parent), m_currentState(kStateHome),  m_surface(nullptr)
    {
        // basic cam settings
        cam_width = 1400;
        cam_height = 1400;
        cam_offset_x = 0;
        cam_offset_y = 0;
        FPS = 50;

        // defaults
        isPylon = false; // do we use cam settings from Pylon?
        saving_data = true;
        saving_frames = true;

        // defaults
        save_path = "C:/Users/Dorka/Documents/save_path/";
        portname = "COM5";

        PR = 68;
        SpO2 = 15;
        currentEAR = 0;
        currentPerclos = 0;

        leftEyeClosed = false;
        rightEyeClosed = false;
        mouthOpened = false;

        start_timer = false;
        isBlinking = false;

        connect(this, &Controller::startCameraSignal, this, &Controller::startCameraSlot);
        connect(this, &Controller::stopAll, this, &Controller::threadCarnage);
        connect(this, &Controller::startPPGSignal, this, &Controller::startPPGSensor);
        connect(this, &Controller::startPPGSignal, this, &Controller::startPPGPlot);

        connect(this, &Controller::startLandmarkSignal, this, &Controller::startLandmark);

        connect(this, &Controller::endOfTheCalibration, this, &Controller::calibrationEARs);
        setupLandmark();
    }

    Controller::~Controller()
    {
        threadCarnage();
        delete m_surface;
        delete mSurfaceFormat;
        delete mImage;

    }

    void Controller::setCurrentState(QString state)
    {
      this->m_currentState = state;
      emit currentStateChanged();
      qDebug() << "State changed: " << m_currentState;
    }

    QImage Controller::Mat2QImage(const cv::Mat &src)
    {
        cv::Mat temp; // make the same cv::Mat
        src.convertTo(temp,CV_8UC3);

        QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
        dest = dest.convertToFormat(QImage::Format_ARGB32);

        dest.bits(); // enforce deep copy, see documentation
        return dest;
    }

    void Controller::libtorch_trial(){

//        <AdditionalIncludeDirectories>$(SolutionDir)dependencies/libtorch111/include;$(SolutionDir)dependencies/libtorch111/include/torch/csrc/api/include;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>



//              <AdditionalLibraryDirectories>$(SolutionDir)dependencies/libtorch111/lib/;C:/Program Files/NVIDIA Corporation/NvToolsExt/lib/x64/;$(SolutionDir)dependencies/cuda/lib/x64/;%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>

//              <AdditionalDependencies>torch.lib;torch_cuda.lib;torch_cpu.lib;torch_cuda_cu.lib;c10.lib;c10_cuda.lib;caffe2_nvrtc.lib;nvToolsExt64_1.lib;cudnn.lib;cufft.lib;curand.lib;cublas.lib;cudart_static.lib;%(AdditionalDependencies)</AdditionalDependencies>
//              <!--<AdditionalOptions>/INCLUDE:?searchsorted_cuda@native@at@@YA?AVTensor@2@AEBV32@0_N1@Z%;(AdditionalOptions)</AdditionalOptions>-->
//            </Link>

//        int device_count = 0;
//             #ifdef __linux__
//                     // in linux no need for LoadLibrary
//             #else
//                 LoadLibraryA("torch_cuda.dll");
//             #endif

//             device_count = static_cast<int>(torch::cuda::device_count());
////             () << "Libtorch GPU report: " << LogMsgCategory::DEBUG_INFO;
//             () << "Device count: " << device_count;
//             () <<"CUDA available: " << torch::cuda::is_available();
//             () <<"CuDNN available: " << torch::cuda::cudnn_is_available();


             //Logger::getLogger()->logAsync("Has CUDA test: " std::to_string(caffe2::HasCudaRuntime()), LogMsgCategory::DEBUG_INFO);

             // if(!on_gpu)
             // {
             // c10::WeakOrStrongTypePtr* ptr_for_compilation = nullptr;
             // auto ptr = ptr_for_compilation->asWeakTypePtr();
             // }

             // #ifdef _DEBUG
                 // std::cout << "libtorch gpu report/n";
                 // std::cout << "device count: " << torch::cuda::device_count() << std::endl;
                 // std::cout << "cuda available: " << torch::cuda::is_available() << std::endl;
                 // std::cout << "cudnn available: " << torch::cuda::cudnn_is_available() << std::endl;
                 // std::cout << "has cuda test: " << torch::hascuda() << std::endl;
                 // std::cout << "at cuda device count: " << static_cast<int>(at::cuda::device_count()); //std::endl;
                 // auto cu_stream = c10::cuda::getdefaultcudastream();
                 // std::cout << "cuda stream device index: " << static_cast<int>(cu_stream.device_index()) << " ";
                 // std::cout << "cuda stream device id: " << static_cast<int>(cu_stream.id()) << "/n";
             // #endif
/*
             // Deserialize the ScriptModule from a file using torch::jit::load().
             _nn = torch::jit::load(path_to_model);
             Logger::getLogger()->logAsync("NN - Model succesfully loaded!",LogMsgCategory::DEBUG_INFO);
             inited = true;
         }
         catch (const c10::Error& e)
         {
             Logger::getLogger()->logAsync("NN - Error at loading the model" + std::string(e.what()), LogMsgCategory::FAILURE);
         }
     //    on_gpu = false;

         try
         {
             // Put nn on the selected device
             if (device_count > 0 && on_gpu)
             {
                 _nn.to(torch::kCUDA);
             }
             else
             {
                 _nn.to(torch::kCPU);
                 _on_gpu = false;
                 Logger::getLogger()->logAsync("NN - No GPU was detected, NN utilizing CPU", LogMsgCategory::WARNING);
             }
         }
         catch (const c10::Error & e)
         {
             Logger::getLogger()->logAsync("NN - Error at switching to device" + std::string(e.what()), LogMsgCategory::FAILURE);
             inited = false;
         }

         try
         {
             // Inference mode
             _nn.eval();
         }
         catch (const c10::Error & e)
         {
             Logger::getLogger()->logAsync("NN - Error at initing inference" + std::string(e.what()), LogMsgCategory::FAILURE);
             inited = false;
         }

         */
    }

    void Controller::SlotCameraFound(bool isCam)
    {
        if (isCam == true){

        }
        else {
            qDebug() << "camera not found!!!!!!";
        }
    }

    double Controller::calculateAvgCalibEARs(std::vector<std::vector <double>> calib_ears){
        double sum = 0.0;
        int count = 0;
        for (const auto& row : calib_ears) {
            for (const auto& value : row) {
                sum += value;
                count++;
            }
        }

        double result = sum / count;
        return result;

    }



    void Controller::calibrationEARs(){
        fullyClosedAvgEAR = calculateAvgCalibEARs(fullyClosedEyes);
        qDebug() << " fully closed avg ear: " << fullyClosedAvgEAR;

        fullyOpenedAvgEAR = calculateAvgCalibEARs(fullyOpenedEyes);
        qDebug() << " fully opened avg ear: " << fullyOpenedAvgEAR;

        normalAvgEAR = calculateAvgCalibEARs(normalEyes);
        qDebug() << " norml avg ear: " << normalAvgEAR;

        eighty_percent_EAR = (fullyOpenedAvgEAR-fullyClosedAvgEAR)*0.8 + fullyClosedAvgEAR ;
        qDebug() << "80% EAR: " << eighty_percent_EAR;
        twenty_percent_EAR = (fullyOpenedAvgEAR-fullyClosedAvgEAR)*0.2 + fullyClosedAvgEAR;
        qDebug() << "20% EAR: " << twenty_percent_EAR;
    }



    void Controller::setupLandmark()
    {
        std::ifstream infile("C:/Users/Dorka/Documents/gui_onnx/other/model.txt"); // read 3d model for 68 points
        float tempFloat;
        std::vector<float> tempVector;
        while (infile >> tempFloat){
            tempVector.push_back(tempFloat);
        }
        // create proper 3d coordinate sets
        for (int i = 0; i < static_cast<int>(tempVector.size()/3); i++){
            model_points.push_back(cv::Point3d(-tempVector[i], -tempVector[i+68], -tempVector[i+68+68]));
        }

        std::cout << model_points.size() << " " << model_points[0] << std::endl;

        // ///////// END TEST //////////////

        // create pointset for drawing the attention box
        std::vector<int>_val = {50, 0, 125, 100}; // rear_size, rear_depth, front_size, front_depth

        box_3d_points.push_back(cv::Point3d(-_val[0],-_val[0], _val[1]));
        box_3d_points.push_back(cv::Point3d(-_val[0],_val[0], _val[1]));
        box_3d_points.push_back(cv::Point3d(_val[0],_val[0], _val[1]));
        box_3d_points.push_back(cv::Point3d(_val[0],-_val[0], _val[1]));
        box_3d_points.push_back(cv::Point3d(-_val[0],-_val[0], _val[1]));
        box_3d_points.push_back(cv::Point3d(-_val[2],-_val[2], _val[3]));
        box_3d_points.push_back(cv::Point3d(-_val[2],_val[2], _val[3]));
        box_3d_points.push_back(cv::Point3d(_val[2],_val[2], _val[3]));
        box_3d_points.push_back(cv::Point3d(_val[2],-_val[2], _val[3]));
        box_3d_points.push_back(cv::Point3d(-_val[2],-_val[2], _val[3]));
    }

    void Controller::calcAttentionBox(std::vector<cv::Point2f> _landmarks)
    {
        // 2d image points
    //    std::vector<cv::Point2i> stdLandmarks = std::vector<cv::Point2i>(landmarks.begin(), landmarks.end());
        if (!_landmarks.empty()){
            std::vector<cv::Point2f> image_points;
            image_points = _landmarks;

            // get rotation and transform vectors
    //       auto start = std::chrono::high_resolution_clock::now();
            cv::solvePnPRansac(model_points, image_points, camera_matrix, distCoeffs, rotation_vector, transform_vector, cv::SOLVEPNP_EPNP);
             // default, cv::SOLVEPNP_EPNP, cv::SOLVEPNP_SQPNP
            cv::projectPoints(model_points, rotation_vector, transform_vector, camera_matrix, distCoeffs, nose_end_2d);

            box_2d_points = get2dPoints(rotation_vector,transform_vector, camera_matrix, distCoeffs);
    //        auto stop = std::chrono::high_resolution_clock::now();

            // average landmarks from last 3 measurements to smooth it out
            if (box_2d_list.size()<3){
                box_2d_list.push_back(box_2d_points);
            }
            else{
                box_2d_list.pop_front();
                box_2d_list.push_back(box_2d_points);
            }
/*
            // get runtime
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
            time_took += duration;
            run_count++;
            if (run_count==10){
                qDebug() << "Average PnP runtime:" << time_took.count()/10.0 << "ms";
                time_took = std::chrono::milliseconds::zero();
                run_count = 0;
            } */
        }
    }

    std::vector<cv::Point2d> Controller::get2dPoints(std::vector<double> _rotation_vector, std::vector<double> _transform_vector, cv::Mat _camera_matrix, std::vector<double> _distCoeffs)
    {
        std::vector<cv::Point2d> _box_2d_points;
        cv::projectPoints(box_3d_points, _rotation_vector, _transform_vector, _camera_matrix, _distCoeffs, _box_2d_points);
        return _box_2d_points;
    }


    cv::Mat Controller::drawAttentionBox(cv::Mat _image)
    {
        cv::Scalar color = cv::Scalar(255, 255, 0);
        int line_width = 1;

        vector<cv::Point2d> box_points;

        if (!box_2d_list.empty()){
            for (const vector<cv::Point2d> &point : box_2d_list){
                if (box_points.empty()){
                    box_points=point;
    //                std::cout << "box points1: " << box_points << std::endl;
                }
                else{
                    // sum the landmark coordinates across the last n landmark detections
                    std::transform(box_points.begin(), box_points.end(),
                                   point.begin(), box_points.begin(),
                                   [](cv::Point2d x, cv::Point2d y) {return x+y;});
    //                std::cout << "box points trafo: " << box_points << std::endl;
                }
            }
        }
    //    std::cout << "Size: " << static_cast<int>(box_2d_list.size()) << std::endl;
        int n = static_cast<int>(box_2d_list.size());
        if (!box_points.empty()){
            for (int i=0; i< box_points.size(); i++){
                box_points[i]=box_points[i]/n; // divide by n so we get the average
            }
        }

        // draw the box onto the image
        if (!box_points.empty()){
            vector<cv::Point> _box_points(box_points.begin(), box_points.end());
            cv::polylines(_image, _box_points, true, color, line_width, cv::LINE_AA);
            cv::line(_image, _box_points[1], _box_points[6], color, line_width, cv::LINE_AA);
            cv::line(_image, _box_points[2], _box_points[7], color, line_width, cv::LINE_AA);
            cv::line(_image, _box_points[3], _box_points[8], color, line_width, cv::LINE_AA);
        }
        return _image;
    }



    void Controller::SlotFrameGetter(cv::Mat in_frame)
    {
     start_timer = true;
     emit signalSendToNets(in_frame);

     if(!detected_boxes.empty()){
         //   cv::rectangle(in_frame, detected_boxes[0].rect(), cv::Scalar(255, 255, 0), 2);

            if(!imagePoints.empty()){
                imagePoints.clear();
            }

            int idx = 0;
            std::vector<cv::Point2f> new_landmarks;
            for (const auto &p: landmarks.points){
                cv::Point2f new_p(p.x+detected_boxes[0].rect().x, p.y+detected_boxes[0].rect().y);
                new_landmarks.push_back(new_p);
                if(idx == 8 || idx == 30 || idx == 36 || idx == 45 || idx == 48 || idx == 54){
                   cv::circle(in_frame, new_p, 3, cv::Scalar(0, 255, 0), -1);
                }else{
                   cv::circle(in_frame, new_p, 3, cv::Scalar(255, 0, 0), -1);
                }
                idx++;
            }

            calcAttentionBox(new_landmarks);
            in_frame = drawAttentionBox(in_frame);
        }
      displayed_frame = Mat2QImage(in_frame);



        if (!m_surface->isActive()){
            QVideoFrame::PixelFormat f = QVideoFrame::pixelFormatFromImageFormat(displayed_frame.format());
            mSurfaceFormat = new QVideoSurfaceFormat(displayed_frame.size(), f);
            qDebug() << mSurfaceFormat->isValid();
            if(!m_surface->start(*mSurfaceFormat))
            {
                qDebug() << "Surface couldn't be started!";
            }
        }

        QVideoFrame frame(displayed_frame);
        bool r = m_surface->present(frame);
   //     qDebug() << "surface present succeeded? : " << r;

        checkOneMin = high_resolution_clock::now();
        std::chrono::seconds sec = duration_cast<seconds>(checkOneMin - startOneMin);
        if(sec.count()>= 60){
            startOneMin = high_resolution_clock::now();
            yawningPerMin = 0;
            blinkingPerMin = 0;

            //reset the perclos booleans
            reached_t1 = false;
            reached_t2 = false;
            reached_t3 = false;
            reached_t4 = false;
        }

        frame_counter++;
        detected_boxes.clear();
    }

    void Controller::UpdateLandmarks(lite::types::Landmarks landmarks)
    {
        this->landmarks = landmarks;
    }

    void Controller::UpdateBBox(std::vector<lite::types::Boxf> detected_boxes)
    {
        this->detected_boxes = detected_boxes;
    }

    void Controller::SlotGetSensor(float PR, float waveForm, float SpO2)
    {
        this->PR = PR;
        emit currentPulseChanged(PR);
//        qDebug() << PR << " " << SpO2;
        this->waveForm = waveForm;
        this->SpO2 = SpO2;
        emit currentSPO2Changed(SpO2);
    }

    void Controller::openessChanged(bool left, bool mouth, bool right)
    {
        leftEyeClosed = left;
        emit leftEyeClosedChanged(left);

        if(mouthOpened == false && mouth == true){
            yawningPerMin+=1;
        }
        mouthOpened = mouth;
        emit openedMouthChanged(mouth);

        rightEyeClosed = right;
        emit rightEyeClosedChanged(right);
    }


    void Controller::observedBlinkingTime(double EAR){
    //    qDebug() << "EAR" << EAR;

        calcPERCLOS(EAR);

        if( EAR < EAR_THRESHOLD && isBlinking == false){
           isBlinking = true;
           startBlink = high_resolution_clock::now();

        } else if (EAR > EAR_THRESHOLD && isBlinking == true){
            stopBlink = high_resolution_clock::now();
            std::chrono::milliseconds duration = duration_cast<milliseconds>(stopBlink - startBlink);
            currentBlinkingTime = duration.count();
            savedBlinkingTimes.push_back(currentBlinkingTime);
            qDebug() << "Blinking Time:" << currentBlinkingTime << " ms";
            isBlinking = false;
            blinkCounter++;
            blinkingPerMin++;
            qDebug() << "Blink counter: " << blinkCounter;
        }
    }

    void Controller::calcPERCLOS(double EAR){
        if (EAR <= eighty_percent_EAR && reached_t1 == false){
            t1 = high_resolution_clock::now();
            reached_t1 = true;
            qDebug() << "t1: " << blinkCounter << " EAR: " << EAR;
        } else if(EAR <= twenty_percent_EAR && reached_t1 == true  && reached_t2 == false){
            t2 = high_resolution_clock::now();
            reached_t2 = true;
            qDebug() << "t2: " << blinkCounter << " EAR: " << EAR;
        } else if(EAR >= twenty_percent_EAR && reached_t1 == true  && reached_t2 == true && reached_t3 == false){
            t3 = high_resolution_clock::now();
            reached_t3 = true;
            qDebug() << "t3: " << blinkCounter << " EAR: " << EAR;
        } else if(EAR >= eighty_percent_EAR && reached_t1 == true  && reached_t2 == true && reached_t3 == true && reached_t4 == false){
            t4 = high_resolution_clock::now();
            reached_t4 = true;
            qDebug() << "t4: " << blinkCounter << " EAR: " << EAR;
        } else if(reached_t1 == true && reached_t2 == true && reached_t3 == true && reached_t4 == true){
            std::chrono::milliseconds duration1 = duration_cast<milliseconds>(t3-t2);
            qDebug() << "duration1: " << duration1.count();
            std::chrono::milliseconds duration2 = duration_cast<milliseconds>(t4-t1);
            qDebug() << "duration2: " << duration2.count();

            double dur1 =  duration1.count();
            double dur2 =  duration2.count();
            currentPerclos = (dur1/dur2)*100;

            qDebug() << "CurrentPerclos:" << currentPerclos;
            reached_t1 = false;
            reached_t2 = false;
            reached_t3 = false;
            reached_t4 = false;
        }
    }

    void Controller::startCameraSlot(){

        if (camera!=nullptr) {
            camera->run_flag = false;
            videoCameraWorker.quit();
            videoCameraWorker.wait();
            delete camera;
            camera = nullptr;
            qDebug() << "Camera is released.";
        } else {
//        then when camera is not running, start it
            camera = new VideoCamera(cam_width, cam_height, cam_offset_x, cam_offset_y, FPS, save_path, isPylon);
            camera->moveToThread(&videoCameraWorker);
            videoCameraWorker.start();

            QSize size(cam_width, cam_height);
            QVideoFrame::PixelFormat f = QVideoFrame::pixelFormatFromImageFormat(QImage::Format_ARGB32);
            mSurfaceFormat = new QVideoSurfaceFormat(size, f);

            if(!m_surface->start(*mSurfaceFormat))
            {
              qDebug() << "Surface couldn't be started!";
            }

            connect(this, SIGNAL(SignalStartCamera()), camera, SLOT(doWork()));
            connect(camera,SIGNAL(SignalSendFrame(cv::Mat)),this,SLOT(SlotFrameGetter(cv::Mat)));
            connect(this, SIGNAL(saveFrame(QImage)), this, SLOT(saveQImage(QImage)));
            emit SignalStartCamera();
        }
    }

    void Controller::saveQImage(QImage frame){
        bool saved = frame.save(save_path);

        if (saved){
            qDebug() << "Image saved at: " << save_path;
        } else {
            qDebug() << "Failed to save the image.";
        }

    }


    QAbstractVideoSurface *Controller::videoSurface() const
    {
        return m_surface;
    }

    void Controller::setVideoSurface(QAbstractVideoSurface *surface)
    {
        qDebug() << "setting video surface";
        m_surface = surface;
        emit videoSurfaceChanged();
    }


    void Controller::startLandmark(){
        landmark = new Landmark();
        this->EAR_THRESHOLD = landmark->EAR_THRESHOLD;
        if (landmark!=nullptr){
               landmarkWorker.quit();
               landmarkWorker.wait();
               delete landmark;
               landmark = nullptr;
               qDebug() << "Landmark network is released.";
           }

       if (landmark==nullptr) {
           landmark = new Landmark();
           landmark->moveToThread(&landmarkWorker);
           landmarkWorker.start();

           connect(this,SIGNAL(signalSendToNets(cv::Mat)),landmark,SLOT(doWork(cv::Mat)));
           connect(landmark,SIGNAL(signalSendLandmarks(lite::types::Landmarks)),this,SLOT(UpdateLandmarks(lite::types::Landmarks)));
           connect(landmark,SIGNAL(signalSendBoxes(std::vector<lite::types::Boxf>)),this,SLOT(UpdateBBox(std::vector<lite::types::Boxf>)));
           connect(landmark,SIGNAL(signalOpeness(bool, bool, bool)), this, SLOT(openessChanged(bool,bool,bool)));
           connect(landmark,SIGNAL(calculated_EAR(std::vector<double>)), this, SLOT(saveFullyOpenedLandmarks(std::vector<double>)));
           connect(landmark,SIGNAL(calculated_EAR(std::vector<double>)), this, SLOT(saveFullyClosedLandmarks(std::vector<double>)));
           connect(landmark,SIGNAL(calculated_EAR(std::vector<double>)), this, SLOT(saveNormalLandmarks(std::vector<double>)));

           connect(landmark,SIGNAL(calculated_EAR(std::vector<double>)), this, SLOT(frameEAR(std::vector<double>)));

           connect(this, SIGNAL(currentEARChanged(double)),this,SLOT(observedBlinkingTime(double)));

           startOneMin = high_resolution_clock::now();

       }
    }


    void Controller::saveFullyClosedLandmarks(std::vector<double> EAR){
        if( this->m_currentState == "fully_closed"){
            fullyClosedEyes.push_back(EAR);
        //    qDebug() << "fully closed: " <<fullyClosedEyes.size();
        }
    }

    void Controller::saveFullyOpenedLandmarks(std::vector<double> EAR){
        if( this->m_currentState == "fully_opened"){
            fullyOpenedEyes.push_back(EAR);
        //    qDebug() << "fully opened: " <<fullyOpenedEyes.size();
        }
    }

    void Controller::saveNormalLandmarks(std::vector<double> EAR){
        if( this->m_currentState == "normal"){
            normalEyes.push_back(EAR);
        //    qDebug() << "fully normal: " <<normalEyes.size();
        }
    }

    void Controller::frameEAR(std::vector<double> EAR){
        if (this->m_currentState == "videopage"){
        //    qDebug() << "EAR: " << (EAR[0]+EAR[1])/2.0;
            setCurrentEAR((EAR[0]+EAR[1])/2.0);
        }
    }

    void Controller::setCurrentEAR (double EAR){
        this->currentEAR = EAR;
        emit currentEARChanged(EAR);
    }




    void Controller::startPPGSensor(){
        if (sensor!=nullptr) {
            sensor->run_flag = false;
            sensorWorker.quit();
            sensorWorker.wait();
            delete sensor;
            sensor = nullptr;
            qDebug() << "PPG Sensor is released.";
        }
        else{
            sensor = new PPGSensor(portname);
            sensor->moveToThread(&sensorWorker);
            sensorWorker.start();
            connect(this, SIGNAL(SignalStartSensor()), sensor, SLOT(doWork()));
            connect(sensor, SIGNAL(SignalSendSensor(float, float, float)), this, SLOT(SlotGetSensor(float, float, float)));
            emit SignalStartSensor();
        }

    }

    void Controller::startPPGPlot(){

////        QMainWindow window;
//        Chart *chart = new Chart;
//        chart->setTitle("Dynamic spline chart");
//        chart->legend()->hide();
//        chart->setAnimationOptions(QChart::AllAnimations);
//        QChartView chartView(chart);
//        chartView.setRenderHint(QPainter::Antialiasing);
////        window.setCentralWidget(&chartView);
////        window.resize(400, 300);
////        window.show();
//        qDebug() << "startPPGPlot";

    }

    void Controller::threadCarnage(){
        if (camera!=nullptr) {
            camera->run_flag = false;
            videoCameraWorker.quit();
            videoCameraWorker.wait();
            delete camera;
            camera = nullptr;
            qDebug() << "Camera is released.";
        }

        if (sensor!=nullptr) {
            sensor->run_flag = false;
            sensorWorker.quit();
            sensorWorker.wait();
            delete sensor;
            sensor = nullptr;
            qDebug() << "Sensor is released.";
        }

        if (landmark!=nullptr) {
            landmarkWorker.quit();
            landmarkWorker.wait();
            delete landmark;
            landmark = nullptr;
            qDebug() << "Landmark is released.";
        }

    }

}


