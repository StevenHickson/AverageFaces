#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/ml/ml.hpp>
#include <boost/filesystem.hpp>
#include "flandmark_detector.h"

#define FACE_XML "C:/opencv/data/haarcascades/haarcascade_frontalface_default.xml"
#define EYE_XML "C:/opencv/data/haarcascades/haarcascade_eye.xml"
#define NOSE_XML "C:/opencv/data/haarcascades/haarcascade_mcs_nose.xml"
#define MOUTH_XML "C:/opencv/data/haarcascades/haarcascade_mcs_mouth.xml"
#define FLANDMARK_DATA "C:/Users/Steve/Documents/GitHub/AverageFaces/Data/flandmark_model.dat"
//#define ALIGNMENT_DATA2 { 43, 43, 36, 27, 49, 27, 29, 57, 56, 57, 22, 25, 66, 27, 40, 50 }
// ALIGNMENT		   { CENTER, LEFT EYE R, RIGHT EYE L, MOUTH L, MOUTH R, LEFT EYE L, RIGHT EYE R, NOSE }
#define ALIGNMENT_DATA { 337, 412, 310, 360, 400, 360, 300, 512, 425, 512, 240, 360, 470, 360, 360, 450 }
#define WIDTH 675
#define HEIGHT 825

int FindFace(cv::Mat &in, cv::Rect &face, cv::Mat &out);
int AlignFace(cv::Mat &in, cv::Mat &out);
int AlignFLandmark(cv::Mat &in, cv::Rect &face, cv::Mat &out);