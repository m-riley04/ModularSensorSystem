// HEAVILY based on barleo01's "yoloobjectdetector" on GitHub.
// Link: https://github.com/barleo01/yoloobjectdetector/blob/master/src/yolo.h

#ifndef YOLO_H
#define YOLO_H

#include <opencv2/dnn.hpp>
#include <opencv2/dnn/shape_utils.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video/video.hpp>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <iterator>

#include <QObject>
#include <QImage>
#include <QThread>
#include <QMutex>

using Mat = cv::Mat;
using Net = cv::dnn::Net;
using Rect = cv::Rect;

class Yolo : public QObject
{
    Q_OBJECT

public:
    // A detection result that will be used by the UI.
    struct Detection {
        int classId;
        float confidence;
        QRect rect;
    };

private:
    float mInputWidth = 640.0;        // Width of network's input image (YOLO model width)
    float mInputHeight = 640.0;       // Height of network's input image (YOLO model height)
    int mFrameWidth = 640;
    int mFrameHeight = 640;
    const float mConfidenceThreshold = 0.5f; // Confidence threshold
    const float mNonMaximumSuppressionThreshold = 0.4f; // NMS threshold

    std::vector<std::string> mClasses;
    Net mNet;
    Mat mFrame;
    std::vector<Mat> mOuts;
    std::string mModelConfig;

    QImage mLatestFrame;
    QMutex mMutex;
    QTimer* pTimer = nullptr;


public:
    explicit Yolo(QObject* parent = 0);
    ~Yolo();

    void setup(void);
    void feedForward(Mat& frame);
    std::vector<Detection> postProcess(const std::vector<Mat>& outs);

public slots:
    void receiveNewFrame(QImage imageFrame);
    void processLatestFrame();

signals:
    void sendDetections(std::vector<Detection> detections);

};

#endif // YOLO_H