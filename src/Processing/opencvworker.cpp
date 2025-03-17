#include "opencvworker.h"
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdlib.h>
#include <thread>

using namespace std;

//---- Constructor
OpenCvWorker::OpenCvWorker(QObject* parent) :
    QObject(parent),
    m_status(false)
{
    m_classesFile = "data/classes.txt";
    ifstream ifs(m_classesFile.c_str());
    string line;
    while (getline(ifs, line)) { // TODO (for optimization): replace by iterator
        m_classes.push_back(line);
    }
}

//---- Destructor
OpenCvWorker::~OpenCvWorker()
{
}

//---- capture the frames
void OpenCvWorker::captureFrame()
{
    Mat frameToGui;

    //(*m_cap) >> m_currentFrame;
    if (m_currentFrame.empty()) {
        return;
    }
    flip(m_currentFrame, m_currentFrame, +1);
    frameToGui = m_currentFrame.clone();

    postProcess(frameToGui, m_outs);

    // NOTE: the following line is needed to be compatible with QImage
    cv::cvtColor(frameToGui, frameToGui, cv::COLOR_BGR2RGB); // convert opencv image from BGR to RGB
    QImage output((const unsigned char*)frameToGui.data, frameToGui.cols, frameToGui.rows, QImage::Format_RGB888);//Format_Indexed8);
    emit sendFrame(output);
}

//---- process the frame to draw outputs
void OpenCvWorker::postProcess(Mat& frame, const vector<Mat>& outs)
{
    vector<int> classIds;
    vector<float> confidences;
    vector<Rect> boxes;

    for (size_t i = 0; i < outs.size(); ++i)
    {
        float* data = (float*)outs[i].data;
        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
        {
            Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
            Point classIdPoint;
            double confidence;
            minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
            if (confidence > m_confThreshold)
            {
                int centerX = (int)(data[0] * frame.cols);
                int centerY = (int)(data[1] * frame.rows);
                int width = (int)(data[2] * frame.cols);
                int height = (int)(data[3] * frame.rows);
                int left = centerX - width / 2;
                int top = centerY - height / 2;

                classIds.push_back(classIdPoint.x);
                confidences.push_back((float)confidence);
                boxes.push_back(Rect(left, top, width, height));
            }
        }
    }

    vector<int> indices;
    NMSBoxes(boxes, confidences, m_confThreshold, m_nmsThreshold, indices);
    for (size_t i = 0; i < indices.size(); ++i)
    {
        int idx = indices[i];
        Rect box = boxes[idx];
        drawPred(classIds[idx], confidences[idx], box.x, box.y,
            box.x + box.width, box.y + box.height, frame);
    }
}

//---- draw outputs
void OpenCvWorker::drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat& frame) {
    rectangle(frame, Point(left, top), Point(right, bottom), Scalar(0, 255, 0));
    std::string label = std::format("%.2f", conf);
    if (!m_classes.empty()) {
        CV_Assert(classId < (int)m_classes.size());
        label = m_classes[classId] + ": " + label;
    }
    int baseLine;
    Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
    top = max(top, labelSize.height);
    rectangle(frame, Point(left, top - labelSize.height),
        Point(left + labelSize.width, top + baseLine), Scalar::all(255), FILLED);
    putText(frame, label, Point(left, top), FONT_HERSHEY_SIMPLEX, 0.5, Scalar());
}

//---- start yolo
void OpenCvWorker::activateYOLO() {
    Mat FrameToSend = m_currentFrame.clone();
    emit sendNewFrameYolo(FrameToSend);
}

//---- receive new yolo outputs
void OpenCvWorker::receiveNewParameters(vector<Mat> outs_) {
    //qDebug() << "opencvworker, receiveNewParameters, thread id  " << QThread::currentThreadId();
    m_outs = outs_;
    //send a new frame to yolo
    Mat FrameToSend = m_currentFrame.clone();
    emit sendNewFrameYolo(FrameToSend);
}