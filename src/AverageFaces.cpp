#include "AverageFaces.h"


using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
	Mat out, aligned, face, flandmark, in = imread(argv[1]);
	Rect face_rect;
	if(!FindFace(in,face_rect,face)) 
		cout << "Error couldn't find face";
	else {
		//resize(face,face,Size(WIDTH,HEIGHT));

		int ret1 = AlignFLandmark(in,face_rect,flandmark);
		if(ret1 < 0) {
			cout << "Couldn't find facial landmarks, using slow method" << endl;
			//AlignImage(flandmark,out);
			//flandmark = out;
		}

		imshow("in",in);
		imshow("face",face);
		//imshow("features",aligned);
		imshow("landmarks",flandmark);
		waitKey();
	}
	return 0;
}
