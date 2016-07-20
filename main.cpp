#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <ios>
#include <opencv2/imgproc/imgproc_c.h>
#include "main_func.h"

#define PI 3.1415926
#define CENTER_RANGE 0.1

using namespace cv;
using namespace std;

int main()
{
    Point mark_point, current_point;
    int center_x_min, center_x_max, center_y_min, center_y_max;
    Mat image;
    Mat outImage;
    Mat dst, cdst;
    float size = 0.0;
    cout<<"what percentage to reduce the image: "<<endl;
    cin >>size;
    size = size/100;
    
    double start = double(getTickCount());

    VideoCapture cap("./Data/corredor7.wmv");
    if (!cap.isOpened()) {
	cout << "Capture could not be opened successfully" << endl;
	return -1;
    }/**
    int frame_width=   cap.get(CV_CAP_PROP_FRAME_WIDTH);
    int frame_height=   cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    VideoWriter video("out.avi",CV_FOURCC('M','J','P','G'),10, Size(frame_width,frame_height),true);**/
    //get the initial position image.
    /**while (1)
    {
	    cap >> image;
	    mark_point = vanish_point_detection(image, cdst);
	    cout << "what about this ?" << endl;
	    cin >> order;
	    if (order == 'y')
	    {
		    break;
	    }
    }**/
    cap >> image;
    resize(image, outImage, cv::Size(), size, size);
    
    mark_point = vanish_point_detection(outImage, cdst);
    center_x_max = (1 + CENTER_RANGE) * mark_point.x;
    center_x_min = (1 - CENTER_RANGE) * mark_point.x;
    center_y_max = (1 + CENTER_RANGE) * mark_point.y;
    center_y_min = (1 - CENTER_RANGE) * mark_point.y;

    while (char(waitKey(1)) != 27 && cap.isOpened()) {
	start = double(getTickCount());
	cap >> outImage;
	resize(outImage, outImage, cv::Size(), size, size);
	if (outImage.empty()) {
	    cout << "Video over" << endl;
	    break;
	} else {
	    current_point = vanish_point_detection(outImage, cdst);
	    /**
	    Canny(image, dst, 30, 70, 3);
	    cvtColor(dst, cdst, CV_GRAY2BGR);

	    vector<Vec2f> lines;
	    //detect lines
	    HoughLines(dst, lines, 1, CV_PI / 180, 150, 0, 0);
			
	    //// draw lines
	    for (size_t i = 0; i < lines.size(); i++)
	    {
		    float rho = lines[i][0], theta = lines[i][1];
		    Point pt1, pt2;
		    double a = cos(theta), b = sin(theta);
		    double x0 = a*rho, y0 = b*rho;
		    pt1.x = cvRound(x0 + 1000 * (-b));
		    pt1.y = cvRound(y0 + 1000 * (a));
		    pt2.x = cvRound(x0 - 1000 * (-b));
		    pt2.y = cvRound(y0 - 1000 * (a));
		    if (10 * PI / 180 < theta && theta < 60 * PI / 180)
		    {
			    line(cdst, pt1, pt2, Scalar(0, 255, 0), 3, CV_AA);
		    }
		    else if (110 * PI / 180 < theta && theta < 170 * PI / 180)
		    {
			    line(cdst, pt1, pt2, Scalar(255, 0, 0), 3, CV_AA);
		    }
		    else
		    {
			    line(cdst, pt1, pt2, Scalar(0, 0, 255), 3, CV_AA);
		    }
	    }**/
	    double duration_ms = (double(getTickCount()) - start) * 1000 / getTickFrequency();
	    cout << "It took " << duration_ms << " ms." << endl;
	    if (current_point.x > center_x_max) {
		cout << "left" << endl;
	    } else if (current_point.x < center_x_min) {
		cout << "right" << endl;
	    } else {
		cout << "center" << endl;
	    }
	    line(cdst, Point(mark_point.x, 0), Point(mark_point.x, outImage.rows), Scalar(100, 100, 100), 1, CV_AA);
	    //video.write(cdst);
	    imshow("detected lines", outImage);
	    
	}

    }

    cout << "Press any key to exit:" << endl;
    return 0;
}
