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
    cv::dnn::blobFromImage(mFrame, blob, 1.0 / 255.0, cv::Size(NET_WIDTH, NET_HEIGHT), cv::Scalar(), true, false);

    // Sets the input to classId the network
    mNet.setInput(blob);

    // Runs the forward pass to get output of the output layers
    mNet.forward(mOuts, mNet.getUnconnectedOutLayersNames());
}

void Yolo::receiveNewFrame(QImage imageFrame) {
    // Check if image frame is empty
	if (imageFrame.isNull()) {
        emit sendDetections(std::vector<Detection>());
		return;
	}

    // Set the frame input width and height
	mFrameWidth = imageFrame.width();
	mFrameHeight = imageFrame.height();

    QImage formatted = imageFrame.convertToFormat(QImage::Format_ARGB32);

    // Convert QImage to cv::Mat
    cv::Mat mat = cv::Mat(formatted.height(), formatted.width(), CV_8UC4,
        reinterpret_cast<uchar*>(formatted.bits()), formatted.bytesPerLine()).clone(); // TODO: use this repo to make it more accurate: https://github.com/dbzhang800/QtOpenCV/blob/master/cvmatandqimage.cp
    
    cv::Mat matBGR;
    cv::cvtColor(mat, matBGR, cv::COLOR_BGRA2BGR);
    mFrame = matBGR.clone();
    
    // Run the network
    this->feedForward(mFrame);

    // Process raw outputs into detection results
	std::vector<Detection> detections = postProcess(mOuts);

    // Emit detections
    emit sendDetections(detections); // send new detected vector<Mat> for next boxes drawing
}