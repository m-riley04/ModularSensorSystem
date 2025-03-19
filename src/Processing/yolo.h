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
    double mInputWidth, mInputHeight;
    int mFrameWidth, mFrameHeight;
    double mConfidenceThreshold, mNmsThreshold;

    std::vector<std::string> mClasses;
    Net mNet;
    Mat mFrame;
    std::vector<Mat> mOuts;
    std::string mModelConfig;

    QImage mLatestFrame;
    QMutex mMutex;
    QTimer* pTimer = nullptr;

    void setup(void);
    void preProcess(Mat& frame);
    std::vector<Detection> postProcess(const std::vector<Mat>& outs);

public:
    Yolo(QObject* parent = nullptr, 
        double inputWidth = 640.0, 
        double inputHeight = 640.0, 
        double confidenceThreshold = 0.5, 
        double nmsThreshold = 0.4,
        std::vector<std::string> classes = std::vector<std::string>());
    ~Yolo();
    
	std::vector<std::string> classes() const { return mClasses; }


public slots:
    void receiveNewFrame(QImage imageFrame);
    void processLatestFrame();
	void setClasses(std::vector<std::string> classes);

signals:
    void sendDetections(std::vector<Detection> detections);

};

#endif // YOLO_H