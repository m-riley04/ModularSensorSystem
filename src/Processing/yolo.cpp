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
    mClasses.push_back("bottle");
    mClasses.push_back("car");
    mClasses.push_back("dog");
    mClasses.push_back("cat");

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

    // Use the original frame dimensions for scaling
    int frameWidth = mFrameWidth;
    int frameHeight = mFrameHeight;

    // Compute scaling factors (from network input size to original image size)
    float x_factor = frameWidth / static_cast<float>(NET_WIDTH);
    float y_factor = frameHeight / static_cast<float>(NET_HEIGHT);

    // Loop over each output layer.
    for (size_t i = 0; i < outs.size(); ++i) {
        float* data = reinterpret_cast<float*>(outs[i].data);
        int dimensions = outs[i].cols; // number of elements per detection row
        int rows = outs[i].rows;
        for (int j = 0; j < rows; ++j, data += dimensions) {
            float confidence = data[4];
            if (confidence > mConfidenceThreshold) {
                // Get class scores (assume scores start at index 5)
                cv::Mat scores = outs[i].row(j).colRange(5, dimensions);
                cv::Point classIdPoint;
                double max_class_score;
                cv::minMaxLoc(scores, 0, &max_class_score, 0, &classIdPoint);
                if (max_class_score > mConfidenceThreshold) {
                    // Get bounding box coordinates in network scale (normalized for NET_WIDTH/NET_HEIGHT)
                    float cx = data[0];
                    float cy = data[1];
                    float w = data[2];
                    float h = data[3];
                    // Convert to top-left corner coordinates on the original image scale
                    int left = static_cast<int>((cx - 0.5f * w) * x_factor);
                    int top = static_cast<int>((cy - 0.5f * h) * y_factor);
                    int width = static_cast<int>(w * x_factor);
                    int height = static_cast<int>(h * y_factor);
                    classIds.push_back(classIdPoint.x);
                    confidences.push_back(confidence);
                    boxes.push_back(cv::Rect(left, top, width, height));
                }
            }
        }
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



//---- compute network with frame
void Yolo::feedForward(Mat& frame_) {
    // create dnn input variable
    cv::Mat blob;
    cv::dnn::blobFromImage(mFrame, blob, 1.0 / 255.0, cv::Size(NET_WIDTH, NET_HEIGHT), cv::Scalar(), true, false);

    //Sets the input to classId the network
    mNet.setInput(blob);

    //Runs the forward pass to get output of the output layers
    mNet.forward(mOuts, mNet.getUnconnectedOutLayersNames());
}

//---- receive a new frame to compute
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