#include <iostream>
#include "yolo.h"
#include <QtCore>

Yolo::Yolo(QObject* parent, double inputWidth, double inputHeight, double confidenceThreshold, double nmsThreshold, std::vector<std::string> classes)
	: QObject(parent), 
    mInputWidth(inputWidth), 
    mInputHeight(inputHeight), 
    mConfidenceThreshold(confidenceThreshold), 
    mNmsThreshold(nmsThreshold),
    mClasses(classes),
    mEnv(ORT_LOGGING_LEVEL_WARNING, "Yolo"),
    mSessionOptions()
{
    setup();
}

Yolo::~Yolo() {
}

//---- Setup network
void Yolo::setup(void) {
    // Reset error flag
    mError = false;
    mInputNames.clear();
    mOutputNames.clear();

    // Add periodic processing of last frame
    if (!pTimer) {
        pTimer = new QTimer(this);
        connect(pTimer, &QTimer::timeout, this, &Yolo::processLatestFrame);
        pTimer->start(mCaptureIntervalMs);
    }

    // Check if model file exists
    if (!QFile(QString::fromStdString(mModelConfig)).exists()) {
        mError = true;
		emit errorOccurred(QString("Model file not found: %1").arg(QString::fromStdString(mModelConfig)));
        return;
    }

    try {
		mSessionOptions.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
#ifdef _WIN32
        std::wstring w_modelPath(mModelConfig.begin(), mModelConfig.end());
        mSession = std::make_unique<Ort::Session>(mEnv, w_modelPath.c_str(), mSessionOptions);
#else
        mSession = std::make_unique<Ort::Session>(mEnv, mModelConfig.c_str(), mSessionOptions);
#endif
	}
	catch (const cv::Exception& e) {
		mError = true;
		emit errorOccurred(QString("Failed to load model: %1").arg(e.what()));
        qDebug() << "Error: " + QString(e.what());
		return;
	}

	// Retrieve names of input and output nodes
    Ort::AllocatorWithDefaultOptions allocator;
    mInputNames.push_back(std::string(mSession->GetInputNameAllocated(0, allocator).get()));
    mOutputNames.push_back(std::string(mSession->GetOutputNameAllocated(0, allocator).get()));
}

std::vector<float> Yolo::preProcess(Mat& frame) {
    cv::Mat resized;
    cv::resize(frame, resized, cv::Size(static_cast<int>(mInputWidth), static_cast<int>(mInputHeight)));
    cv::Mat rgb;
    cv::cvtColor(resized, rgb, cv::COLOR_BGR2RGB);
    cv::Mat floatImage;
    rgb.convertTo(floatImage, CV_32FC3, 1.0 / 255.0);

    int channels = floatImage.channels();
    int imgH = floatImage.rows;
    int imgW = floatImage.cols;
    std::vector<float> inputTensorValues(imgH * imgW * channels);
    std::vector<cv::Mat> chw(channels);
    for (int i = 0; i < channels; ++i) {
        // Each channel will occupy a contiguous block in the vector.
        chw[i] = cv::Mat(imgH, imgW, CV_32FC1, inputTensorValues.data() + i * imgH * imgW);
    }
    cv::split(floatImage, chw);
    return inputTensorValues;
}

std::vector<Yolo::Detection> Yolo::postProcess(Mat& frame, const std::vector<Mat>& outs) {
    std::vector<int> classIds;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;

    float x_factor = static_cast<float>(mFrameWidth) / mInputWidth;
    float y_factor = static_cast<float>(mFrameHeight) / mInputHeight;

    float* data = reinterpret_cast<float*>(outs[0].data);
    const int dimensions = 6;
    const int rows = 300;

    for (int i = 0; i < rows; ++i) {
        float confidence = data[4];
        if (confidence >= mConfidenceThreshold) {
            float* classes_scores = data + 5;
            cv::Mat scores(1, static_cast<int>(mClasses.size()), CV_32FC1, classes_scores);
            cv::Point class_id;
            double max_class_score;
            cv::minMaxLoc(scores, 0, &max_class_score, 0, &class_id);
            if (max_class_score > mNmsThreshold) {
                confidences.push_back(confidence);
                classIds.push_back(class_id.x);

                float cx = data[0];
                float cy = data[1];
                float w = data[2];
                float h = data[3];

                int left = static_cast<int>((cx - 0.5 * w) * x_factor);
                int top = static_cast<int>((cy - 0.5 * h) * y_factor);
                int width = static_cast<int>(w * x_factor);
                int height = static_cast<int>(h * y_factor);
                boxes.push_back(cv::Rect(left, top, width, height));
            }
        }
        data += dimensions;
    }

    std::vector<int> indices;
    // Use OpenCV’s NMS function (it is independent of the DNN module)
    cv::dnn::NMSBoxes(boxes, confidences, mConfidenceThreshold, mNmsThreshold, indices);

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

    // Prepare the input tensor
    std::vector<float> inputTensorValues = preProcess(matBGR);
    std::vector<int64_t> inputShape = { 1, 3, static_cast<int64_t>(mInputHeight), static_cast<int64_t>(mInputWidth) };

    Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
    Ort::Value inputTensor = Ort::Value::CreateTensor<float>(
        memoryInfo, inputTensorValues.data(), inputTensorValues.size(),
        inputShape.data(), inputShape.size());

	// Create c-string lists for input and output names
    std::vector<const char*> inputNamesCStr;
    for (const auto& name : mInputNames) {
        inputNamesCStr.push_back(name.c_str());
    }

    std::vector<const char*> outputNamesCStr;
    for (const auto& name : mOutputNames) {
        outputNamesCStr.push_back(name.c_str());
    }

    // Run the inference
    std::vector<Ort::Value> outputTensors = mSession->Run(Ort::RunOptions{ nullptr },
        inputNamesCStr.data(), & inputTensor, 1,
        outputNamesCStr.data(), 1);

    // Assume output tensor shape is [1, numDetections, 6]
    const float* outputData = outputTensors[0].GetTensorData<float>();
    std::vector<int64_t> outputShape = outputTensors[0].GetTensorTypeAndShapeInfo().GetShape();
    // Assuming outputShape = {1, numDetections, 6}
    int numDetections = outputShape.size() >= 3 ? static_cast<int>(outputShape[1]) : 0;
    cv::Mat outputMat(numDetections, 6, CV_32F, const_cast<float*>(outputData));
    std::vector<cv::Mat> outs{ outputMat };

    std::vector<Detection> detections = postProcess(matBGR, outs);
    emit sendDetections(detections);
}

void Yolo::setClasses(std::vector<std::string> classes)
{
	mClasses = classes;
}

void Yolo::setModel(QString modelPath)
{
    // Check if model is different
	if (modelPath.toStdString() == mModelConfig) {
		qDebug() << "Model path is the same as current model, no change made.";
		return;
	}

    if (modelPath.isEmpty()) {
        qDebug() << "Error: Model path is empty.";
        return;
	}
	// Stop the timer to prevent processing while changing model
	if (pTimer) {
		pTimer->stop();
		delete pTimer;
		pTimer = nullptr;
	}

    // Change model path
	mModelConfig = modelPath.toStdString();
	setup();

	emit modelChanged(modelPath);
}
