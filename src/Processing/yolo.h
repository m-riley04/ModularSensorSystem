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
#include <onnxruntime_cxx_api.h>

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
    std::string mModelConfig = "models/yolov5n.onnx";
    int mCaptureIntervalMs = 30;
    bool mError = false;

    // ONNX Runtime members
    Ort::Env mEnv;                   // Environment for ONNXRuntime
    Ort::SessionOptions mSessionOptions;
    std::unique_ptr<Ort::Session> mSession;
    std::vector<std::string> mInputNames;
    std::vector<std::string> mOutputNames;
    
    QImage mLatestFrame;
    QMutex mMutex;
    QTimer* pTimer = nullptr;

    void setup(void);
    std::vector<float> preProcess(Mat& frame);
    std::vector<Detection> postProcess(Mat& frame, const std::vector<Mat>& outs);

public:
    Yolo(QObject* parent = nullptr, 
        double inputWidth = 640.0, 
        double inputHeight = 640.0, 
        double confidenceThreshold = 0.35, 
        double nmsThreshold = 0.4,
        std::vector<std::string> classes = std::vector<std::string>());
    ~Yolo();
    
	std::vector<std::string> classes() const { return mClasses; }


public slots:
    void receiveNewFrame(QImage imageFrame);
    void processLatestFrame();
	void setClasses(std::vector<std::string> classes);
    void setModel(QString modelPath);

signals:
    void sendDetections(std::vector<Detection> detections);
	void modelChanged(QString modelPath);
	void errorOccurred(QString errorMessage);
};

#endif // YOLO_H