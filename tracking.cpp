#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

Mat frame;

int main() {
	Mat img_hsv, img_mask, roi_mask,roi_hsv,roi_hist;
	Rect roi;
	int channels[] = { 0 };
	int hsize[] = { 256 };
	float range1[] = { 0,256 };
	const float* histRange[] = { range1 };

	VideoCapture cap("D:/tracking.mp4");
	if (!cap.isOpened()) { cout << "없음" << endl; }


	//결과영상저장
	cap >> frame;
	cap >> frame;
	cap >> frame;

	resize(frame, frame, Size(500, 250));
	imshow("frame", frame);

	//roi = Rect(205, 77, 65, 65);// 파란살구
	roi = Rect(150, 145, 65, 65); //빨간살구
	cvtColor(frame, img_hsv, COLOR_BGR2HSV); 
	//inRange(img_hsv, Scalar(80, 60, 50), Scalar(180, 255, 255), img_mask); //해당값 사이에 색이 해당되면 255를준다.//마스크 생성 파
	inRange(img_hsv, Scalar(100, 100, 50), Scalar(200, 255, 255), img_mask); //해당값 사이에 색이 해당되면 255를준다.//마스크 생성 빨
	
	roi_hsv = Mat(img_hsv, roi);
	roi_mask = Mat(img_mask, roi);

	//imshow("hsv", img_hsv);//
	//imshow("mask", img_mask);

	imshow("roi", roi_hsv);

	calcHist(&roi_hsv, 1, channels, roi_mask, roi_hist, 1, hsize, histRange);//관심영역의 히스토그램
	normalize(roi_hist, roi_hist, 0, 255, NORM_MINMAX);

	while(1) {

		cap >> frame; 
		resize(frame, frame, Size(500, 250));


		Mat back_mask;//역투영 영상
		cvtColor(frame, img_hsv,COLOR_BGR2HSV);
		calcBackProject(&img_hsv, 1, channels, roi_hist,back_mask, histRange);  //전처리 핵심 비슷한 부분만 반환
		
		//imshow("back_mask", back_mask);
		RotatedRect track_window = CamShift(back_mask, roi, TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 10, 1));

		
		Point2f ver[4];
		track_window.points(ver);
		for (int i = 0; i < 4; i++)
			line(frame, ver[i], ver[(i + 1) % 4], Scalar(255, 255, 0), 2);

		imshow("frame", frame);

		if (waitKey(30) >= 0) break;
	}
	return 0;


}

