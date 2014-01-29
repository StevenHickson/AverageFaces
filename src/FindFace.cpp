
#include "AverageFaces.h"

using namespace cv;
using namespace std;

int FindFace(Mat &in, Rect &face, Mat &out) {
	CascadeClassifier face_cascade = CascadeClassifier(FACE_XML);
	vector<Rect> faces;
	face_cascade.detectMultiScale(in, faces);
	if(faces.size() == 0)
		return 0;
	int selected = 0;
	double max;
	max = faces[0].area();
	for(int i = 0; i < faces.size(); i++) {
		if(faces[i].area() > max) {
			max = faces[i].area();
			selected = i;
		}
	}
	out = in(faces[selected]).clone();
	face = faces[selected];
	return faces.size();
}

int AlignFace(Mat &in, Mat &out) {
	CascadeClassifier eye_cascade = CascadeClassifier(EYE_XML);
	CascadeClassifier nose_cascade = CascadeClassifier(NOSE_XML);
	CascadeClassifier mouth_cascade = CascadeClassifier(MOUTH_XML);
	vector<Rect> eyes, nose, mouth;
	Rect nose_interest, mouth_interest;
	int nose_add, mouth_add;

	eye_cascade.detectMultiScale(in, eyes);
	if(eyes.size() < 2) {
		cout << "Couldn't find both eyes" << endl;
		return 0;
	} 

	nose_add = in.cols/3;
	nose_interest = Rect(0,nose_add,in.cols, in.rows - nose_add);
	Mat nose_area = in(nose_interest);
	nose_cascade.detectMultiScale(nose_area, nose);
	if(nose.size() == 0) {
		cout << "Couldn't find nose" << endl;
		return 0;
	}
	
	mouth_add = in.cols/2;
	mouth_interest = Rect(0,mouth_add,in.cols, in.rows - mouth_add);
	Mat mouth_area = in(mouth_interest);
	mouth_cascade.detectMultiScale(mouth_area, mouth);
	if(mouth.size() == 0) {
		cout << "Couldn't find mouth" << endl;
		return 0;
	}

	out = in.clone();
	int i;
	for(i = 0; i < 2; i++)
		rectangle(out,eyes[i].tl(),eyes[i].br(),cvScalar(0,255,0));
	//for(i = 0; i < nose.size(); i++)
	rectangle(out,Point(nose[0].x,nose[0].y + nose_add),Point(nose[0].x + nose[0].width, nose[0].y + nose_add + nose[0].height),cvScalar(255,0,0));
	//for(i = 0; i < mouth.size(); i++)
	rectangle(out,Point(mouth[0].x,mouth[0].y + mouth_add),Point(mouth[0].x + mouth[0].width, mouth[0].y + mouth_add + mouth[0].height),cvScalar(0,0,255));
}

inline void DispFLandmark(Mat &out, FLANDMARK_Model *model, int *bbox, double *landmarks) {
	// display landmarks
	rectangle(out, cvPoint(bbox[0], bbox[1]), cvPoint(bbox[2], bbox[3]), CV_RGB(255,0,0) );
	rectangle(out, cvPoint(model->bb[0], model->bb[1]), cvPoint(model->bb[2], model->bb[3]), CV_RGB(0,0,255) );
	circle(out, cvPoint((int)landmarks[0], (int)landmarks[1]), 3, CV_RGB(0, 0,255), CV_FILLED);
	for (int i = 2; i < 2*model->data.options.M; i += 2)
	{
		circle(out, cvPoint(int(landmarks[i]), int(landmarks[i+1])), 3, CV_RGB(255,0,0), CV_FILLED);
	}
}

inline void GetCenterLandmarks(double *landmarks, vector<Point2f> &out) {
	out.resize(3); //center, nose, left eye, right eye, mouth
	//out[0] = Point(float(landmarks[0]), float(landmarks[1]));
	//out[0] = Point(float(landmarks[14]), float(landmarks[15]));
	out[0] = Point(float(landmarks[10] + (landmarks[2] - landmarks[10]) / 2), float(landmarks[11] + (landmarks[3] - landmarks[11]) / 2));
	out[1] = Point(float(landmarks[12] + (landmarks[4] - landmarks[12]) / 2), float(landmarks[13] + (landmarks[5] - landmarks[13]) / 2));
	out[2] = Point(float(landmarks[8] + (landmarks[6] - landmarks[8]) / 2), float(landmarks[9] + (landmarks[7] - landmarks[9]) / 2));
}

inline void DispCenterLandmark(Mat &out, vector<Point2f> &landmarks) {
	for (int i = 0; i < 3; i++)
	{
		circle(out, cvPoint(int(landmarks[i].x), int(landmarks[i].y)), 3, CV_RGB(0,255,0), CV_FILLED);
	}
}

int AlignFLandmark(Mat &in, Rect &face, Mat &out) {
	// load flandmark model structure and initialize
	FLANDMARK_Model * model = flandmark_init(FLANDMARK_DATA);
	Mat gray;
	cvtColor(in,gray,CV_BGR2GRAY);
	int bbox[] = { face.x, face.y, face.x + face.width, face.y + face.height };
	IplImage tmp = gray.operator IplImage();
	double * landmarks = (double*)malloc(2*model->data.options.M*sizeof(double));
	flandmark_detect(&tmp,bbox,model,landmarks);

	vector<Point2f> in_points, out_points;
	in_points.resize(model->data.options.M);
	out_points.resize(model->data.options.M);
	double align[] = ALIGNMENT_DATA;
	GetCenterLandmarks(landmarks,in_points);
	GetCenterLandmarks(align,out_points);

	Mat h = getAffineTransform(in_points,out_points);
	for(int j = 0; j < 3; j++) {
		if(in_points[j].x <= 0 || in_points[j].y <= 0)
			return -1;
	}
	warpAffine(in,out,h, Size(WIDTH,HEIGHT), CV_WARP_FILL_OUTLIERS + CV_INTER_LINEAR);
	DispFLandmark(in,model,bbox,landmarks);
	//DispCenterLandmark(in,in_points);
	return 1;
}