#include "landmark.h"
//#undef LITEORT_DEBUG
//#define LITEORT_DEBUG false

Landmark::Landmark()
{
//    std::string onnx_path_UF = "C:/Users/Dorka/Documents/gui_onnx/lite.ai.toolkit/hub/onnx/cv/ultraface-rfb-320.onnx";
    std::string onnx_path_RetinaFace = "C:/Users/Dorka/Documents/gui_onnx/lite.ai.toolkit/hub/onnx/cv/Pytorch_RetinaFace_mobile0.25.onnx";
    std::string onnx_path_PN = "C:/Users/Dorka/Documents/gui_onnx/lite.ai.toolkit/hub/onnx/cv/pipnet_resnet18_68.onnx";
//    std::string var_onnx_path = "C:/Users/Dorka/Documents/gui_onnx/lite.ai.toolkit/hub/onnx/cv/fsanet-var.onnx";
//    std::string conv_onnx_path = "C:/Users/Dorka/Documents/gui_onnx/lite.ai.toolkit/hub/onnx/cv/fsanet-1x1.onnx";

//    ultraface = new lite::onnxruntime::cv::face::detect::UltraFace(onnx_path_UF);
    retinaface = new lite::onnxruntime::cv::face::detect::RetinaFace(onnx_path_RetinaFace);
    pipnet68 = new lite::onnxruntime::cv::face::align::PIPNet68(onnx_path_PN);
//    var_fsanet = new lite::onnxruntime::cv::face::pose::FSANet(var_onnx_path);
//    conv_fsanet = new lite::onnxruntime::cv::face::pose::FSANet(conv_onnx_path);

    EAR_THRESHOLD = 0.2;
    leftEyeOpened = false;
    mouthOpened = false;
    rightEyeOpened = false;

}

Landmark::~Landmark()
{

//    delete ultraface;
    delete retinaface;
    delete pipnet68;
//    delete var_fsanet;
//    delete conv_fsanet;

}

double Landmark::distanceCalculate(double x1, double y1, double x2, double y2)
{
    double x = x1 - x2; //calculating number to square in next step
    double y = y1 - y2;
    double dist;

    dist = pow(x, 2) + pow(y, 2);       //calculating Euclidean distance
    dist = sqrt(dist);

    return dist;
}

std::vector<double> Landmark::EAR_eye(lite::types::Landmarks landmarks){

    std::vector<double> EAR;

    double p2_minus_p6_l = distanceCalculate(landmarks.points[37].x,landmarks.points[37].y,landmarks.points[41].x,landmarks.points[41].y);
    double p3_minus_p5_l = distanceCalculate(landmarks.points[38].x,landmarks.points[38].y,landmarks.points[40].x,landmarks.points[40].y);
    double p1_minus_p4_l = distanceCalculate(landmarks.points[36].x,landmarks.points[36].y,landmarks.points[39].x,landmarks.points[39].y);
    double ear_left = (p2_minus_p6_l + p3_minus_p5_l) / (2.0 * p1_minus_p4_l);
    if(ear_left < EAR_THRESHOLD){
       // qDebug() << "left eye closed";
        leftEyeOpened = true;
    } else {
        leftEyeOpened = false;
    }

    double p2_minus_p6_r = distanceCalculate(landmarks.points[43].x,landmarks.points[43].y,landmarks.points[47].x,landmarks.points[47].y);
    double p3_minus_p5_r = distanceCalculate(landmarks.points[44].x,landmarks.points[44].y,landmarks.points[46].x,landmarks.points[46].y);
    double p1_minus_p4_r = distanceCalculate(landmarks.points[42].x,landmarks.points[42].y,landmarks.points[45].x,landmarks.points[45].y);
    double ear_right = (p2_minus_p6_r + p3_minus_p5_r) / (2.0 * p1_minus_p4_r);
    if(ear_right < EAR_THRESHOLD){
        rightEyeOpened = true;
    } else {
        rightEyeOpened = false;
    }

    EAR.push_back(ear_left);
    EAR.push_back(ear_right);

    return EAR;
}

cv::Point2f Landmark::upperLip(lite::types::Landmarks landmarks){
    std::vector<cv::Point2f> ulip;
    for(int i = 49; i <54; i++){
        ulip.push_back(landmarks.points[i]);

    }
    for(int i = 60; i <65; i++){
        ulip.push_back(landmarks.points[i]);

    }

    cv::Point2f ulip_mean;
    ulip_mean.x = 0;
    ulip_mean.y = 0;
    for (const auto& point : ulip) {
        ulip_mean.x += point.x;
        ulip_mean.y += point.y;
    }

    ulip_mean.x /= landmarks.points.size();
    ulip_mean.y /= landmarks.points.size();

    return ulip_mean;

}

cv::Point2f Landmark::lowerLip(lite::types::Landmarks landmarks){
    std::vector<cv::Point2f> llip;
    for(int i = 63; i < 68; i++){
        llip.push_back(landmarks.points[i]);

    }
    for(int i = 55; i <60; i++){
        llip.push_back(landmarks.points[i]);

    }

    cv::Point2f llip_mean;
    llip_mean.x = 0;
    llip_mean.y = 0;
    for (const auto& point : llip) {
        llip_mean.x += point.x;
        llip_mean.y += point.y;
    }

    llip_mean.x /= landmarks.points.size();
    llip_mean.y /= landmarks.points.size();

    return llip_mean;
}

void Landmark::yawning(lite::types::Landmarks landmarks){
    cv::Point2f upperlip_mean = upperLip(landmarks);
    cv::Point2f lowerlip_mean = lowerLip(landmarks);

    double dist = distanceCalculate(upperlip_mean.x, upperlip_mean.y, lowerlip_mean.x, lowerlip_mean.y);
    if(dist>15){
        mouthOpened = true;
    } else {
        mouthOpened = false;
    }

}


void Landmark::doWork(cv::Mat input_img)
{
    // silence cout from library
//    std::ostringstream local;
//    auto cout_buff = std::cout.rdbuf();
//    std::cout.rdbuf(local.rdbuf());

    cv::Mat img_bgr = input_img.clone();

    if(!detected_boxes.empty()){
        detected_boxes.clear();
    }

//    auto start = std::chrono::high_resolution_clock::now();
    retinaface->detect(img_bgr, detected_boxes);

 //   auto stop = std::chrono::high_resolution_clock::now();
 //   auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
 //   qDebug() << "Retinaface runtime:" << duration.count() << "ms";

    if (detected_boxes[0].rect().x >= 0 && detected_boxes[0].rect().y >= 0 && detected_boxes[0].rect().x + detected_boxes[0].rect().width <= img_bgr.cols
            && detected_boxes[0].rect().y + detected_boxes[0].rect().height <= img_bgr.rows){
        emit signalSendBoxes(detected_boxes);
  //      auto start2 = std::chrono::high_resolution_clock::now();
        img_bgr = img_bgr(detected_boxes[0].rect());
        pipnet68->detect(img_bgr,landmarks);
  //      auto stop2 = std::chrono::high_resolution_clock::now();
   //     auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(stop2 - start2);
   //     qDebug() << "Pipnet68 runtime:" << duration2.count() << "ms";
        emit signalSendLandmarks(landmarks);

        yawning(landmarks);
        std::vector<double> EAR = EAR_eye(landmarks);

        emit signalOpeness(leftEyeOpened,mouthOpened,rightEyeOpened);
        emit calculated_EAR(EAR);
    }

    // silence cout from library
//    std::cout.rdbuf(cout_buff);

}
