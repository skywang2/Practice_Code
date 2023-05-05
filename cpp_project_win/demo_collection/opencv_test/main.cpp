#include <iostream>
#include <sstream>
#include <string>
#include "opencv2\opencv.hpp"
using namespace std;

void on_mouse(int event, int x, int y, int flags, void* ustc)
{
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		cout << "ustc addr:" << ustc << endl;
		cv::Mat& dst = *(cv::Mat*)ustc;
		cv::Point point(x, y);
		stringstream ss;
		ss << " (x:" << x << ", y:" << y << ")";
		cv::circle(dst, point, 3, cv::Scalar(0, 255, 255, 255), 1);
		cv::putText(dst, ss.str(), point, cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0,255,255,255));
		cv::imshow("output", dst);
	}
}

int main()
{
	const char* PicName = "eleline.jpg";

	//IplImage	*src = cvLoadImage(PicName, 1);
	//cvNamedWindow("show", CV_WINDOW_NORMAL);
	//cvResizeWindow("show", 1920, 1080);
	//cvShowImage("show", src);
	//cvWaitKey(0);
	//cvDestroyAllWindows();
	//cvReleaseImage(&src);

	cv::Mat src = cv::imread(PicName);
	cv::Mat dst;
	cv::resize(src, dst, cv::Size(1920, 1080), 0, 0, cv::INTER_AREA);
	cv::namedWindow("output",CV_WINDOW_NORMAL);
	//cv::resizeWindow("output", 1920, 1080);

	cout << "dst addr:" << &dst << endl;
	cv::setMouseCallback("output", on_mouse, (void*)&dst);

	cv::imshow("output", dst);
	cv::waitKey(0);
	cv::destroyAllWindows();

	return 0;
}
