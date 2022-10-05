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
	if (!cap.isOpened()) { cout << "����" << endl; }


	//�����������
	cap >> frame;
	cap >> frame;
	cap >> frame;

	resize(frame, frame, Size(500, 250));
	imshow("frame", frame);

	//roi = Rect(205, 77, 65, 65);// �Ķ��챸
	roi = Rect(150, 145, 65, 65); //�����챸
	cvtColor(frame, img_hsv, COLOR_BGR2HSV); 
	//inRange(img_hsv, Scalar(80, 60, 50), Scalar(180, 255, 255), img_mask); //�ش簪 ���̿� ���� �ش�Ǹ� 255���ش�.//����ũ ���� ��
	inRange(img_hsv, Scalar(100, 100, 50), Scalar(200, 255, 255), img_mask); //�ش簪 ���̿� ���� �ش�Ǹ� 255���ش�.//����ũ ���� ��
	
	roi_hsv = Mat(img_hsv, roi);
	roi_mask = Mat(img_mask, roi);

	//imshow("hsv", img_hsv);//
	//imshow("mask", img_mask);

	imshow("roi", roi_hsv);

	calcHist(&roi_hsv, 1, channels, roi_mask, roi_hist, 1, hsize, histRange);//���ɿ����� ������׷�
	normalize(roi_hist, roi_hist, 0, 255, NORM_MINMAX);

	while(1) {

		cap >> frame; 
		resize(frame, frame, Size(500, 250));


		Mat back_mask;//������ ����
		cvtColor(frame, img_hsv,COLOR_BGR2HSV);
		calcBackProject(&img_hsv, 1, channels, roi_hist,back_mask, histRange);  //��ó�� �ٽ� ����� �κи� ��ȯ
		
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

