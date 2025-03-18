#include <iostream>
#include "yolo.h"
#include <QtCore>

const int NET_WIDTH = 640;
const int NET_HEIGHT = 640;

Yolo::Yolo(QObject* parent) :QObject(parent) {
    setup();
}

Yolo::~Yolo() {
}

//---- Setup network
void Yolo::setup(void) {
    mOuts.clear();

    // Initialize YOLO classes
    mClasses.push_back("person");
    mClasses.push_back("tvmonitor");
    mClasses.push_back("bed");
    mClasses.push_back("chair");
    mClasses.push_back("remote");
    mClasses.push_back("cell phone");

    // Add periodic processing of last frame
    pTimer = new QTimer(this);
	connect(pTimer, &QTimer::timeout, this, &Yolo::processLatestFrame);
	pTimer->start(30);

    // Configure Network
    // Give the configuration and weight files for the model
    mModelConfig = "Processing/data/yolov5n.onnx";

    if (!QFile(QString::fromStdString(mModelConfig)).exists()) {
        // TODO: Do more here
        return;
    }

    // Load the network
    mNet = cv::dnn::readNet(mModelConfig);

    if (mNet.empty()) {
        // TODO: add something here
        return;
    }

    mNet.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    mNet.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
}

std::vector<Yolo::Detection> Yolo::postProcess(const std::vector<Mat>& outs) {
    std::vector<int> classIds;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;

    float x_factor = mFrameWidth / NET_WIDTH;
    float y_factor = mFrameHeight / NET_HEIGHT;

    float* data = (float*)outs[0].data;

    const int dimensions = 85;
    const int rows = 25200;

	for (int i = 0; i < rows; ++i) { // Heavily based on this implementation: https://learnopencv.com/object-detection-using-yolov5-and-opencv-dnn-in-c-and-python/

        float confidence = data[4];
        if (confidence >= mConfidenceThreshold) {

            float* classes_scores = data + 5;
            cv::Mat scores(1, mClasses.size(), CV_32FC1, classes_scores);
            cv::Point class_id;
            double max_class_score;
            minMaxLoc(scores, 0, &max_class_score, 0, &class_id);
            if (max_class_score > mNonMaximumSuppressionThreshold) {

                confidences.push_back(confidence);

                classIds.push_back(class_id.x);

                float x = data[0];
                float y = data[1];
                float w = data[2];
                float h = data[3];
                // Width, height and x,y coordinates of bounding box

                int left = int((x - 0.5 * w) * x_factor);
                int top = int((y - 0.5 * h) * y_factor);
                int width = int(w * x_factor);
                int height = int(h * y_factor);
                boxes.push_back(cv::Rect(left, top, width, height));
            }

        }
        data += 85;

    }

    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, mConfidenceThreshold, mNonMaximumSuppressionThreshold, indices);

    std::vector<Detection> detections;
    for (int idx : indices) {
        Detection det;
        det.classId = classIds[idx];
        det.confidence = confidences[idx];
        det.rect = QRect(boxes[idx].x, boxes[idx].y, boxes[idx].width, boxes[idx].height);
        detections.push_back(det);
    }
    return detections;
}

void Yolo::feedForward(Mat& frame_) {
    // Create blob
    cv::Mat blob;
    cv::dnn::blobFromImage(frame_, blob, 1.0 / 255.0, cv::Size(NET_WIDTH, NET_HEIGHT), cv::Scalar(), true, false);

    // Sets the input to classId the network
    mNet.setInput(blob);

    // Runs the forward pass to get output of the output layers
    mNet.forward(mOuts, mNet.getUnconnectedOutLayersNames());
}


void Yolo::receiveNewFrame(QImage imageFrame) {
    // Lock the mutex and update the latest frame.
    QMutexLocker locker(&mMutex);
    mLatestFrame = imageFrame;  // Overwrite previous frame.
}

void Yolo::processLatestFrame() {
    QImage frameToProcess;
    {
        // Lock the mutex to safely retrieve and clear latest frame
        QMutexLocker locker(&mMutex);
        if (mLatestFrame.isNull()) {
            // No new frame available
            return;
        }

        // Copy the latest frame for processing
        frameToProcess = mLatestFrame;

        // Clear the stored frame so that subsequent calls know it's been processed
        mLatestFrame = QImage();
    }

    // Convert frameToProcess to the appropriate format (cv::Mat)
    QImage formatted = frameToProcess.convertToFormat(QImage::Format_ARGB32);
    cv::Mat mat = cv::Mat(formatted.height(), formatted.width(), CV_8UC4,
        reinterpret_cast<uchar*>(formatted.bits()), formatted.bytesPerLine()).clone();
    cv::Mat matBGR;
    cv::cvtColor(mat, matBGR, cv::COLOR_BGRA2BGR);

    // Update frame size parameters
    mFrameWidth = frameToProcess.width();
    mFrameHeight = frameToProcess.height();

    // Create blob and run network
	this->feedForward(matBGR);

    // Process raw outputs into detection results
    std::vector<Detection> detections = postProcess(mOuts);

    // Emit detections
    emit sendDetections(detections);
}