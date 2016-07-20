#include "main_func.h"
#define PI 3.1415926

using namespace std;
using namespace cv;

Point vanish_point_detection(Mat & image, Mat & cdst)
{
    Mat dst;
    vector<Vec2f> left_lines;
    vector<Point> Intersection;
    vector<Vec2f> right_lines;
    hough_line_detect(image, cdst, left_lines, right_lines);
    draw_line(cdst, left_lines, Scalar(0, 255, 0));
    draw_line(cdst, right_lines, Scalar(255, 0, 0));
    size_t i = 0, j = 0;
    long sum_x = 0;
    long sum_y = 0;
    double x = 0;
    double y = 0;
    Canny(image, dst, 30, 70, 3);
    cvtColor(dst, cdst, CV_GRAY2BGR);

    vector<Vec2f> lines;
    //detect lines
    HoughLines(dst, lines, 1, CV_PI / 180, 150, 0, 0);

    //// draw lines
    for (size_t i = 0; i < lines.size(); i++) {
	float rho = lines[i][0], theta = lines[i][1];
	Point pt1, pt2;
	double a = cos(theta), b = sin(theta);
	double x0 = a*rho, y0 = b*rho;
	pt1.x = cvRound(x0 + 1000 * (-b));
	pt1.y = cvRound(y0 + 1000 * (a));
	pt2.x = cvRound(x0 - 1000 * (-b));
	pt2.y = cvRound(y0 - 1000 * (a));
	if (10 * PI / 180 < theta && theta < 60 * PI / 180) {
	    line(image, pt1, pt2, Scalar(0, 255, 0), 3, CV_AA);
	} else if (110 * PI / 180 < theta && theta < 170 * PI / 180) {
	    line(image, pt1, pt2, Scalar(255, 0, 0), 3, CV_AA);
	} else {
	    line(image, pt1, pt2, Scalar(0, 0, 255), 3, CV_AA);
	}
    }
    for (i = 0; i < left_lines.size(); ++i) {
	for (j = 0; j < right_lines.size(); ++j) {
	    float rho_l = left_lines[i][0], theta_l = left_lines[i][1];
	    float rho_r = right_lines[j][0], theta_r = right_lines[j][1];
	    double denom = (sin(theta_l) * cos(theta_r) - cos(theta_l) * sin(theta_r));
	    x = (rho_r * sin(theta_l) - rho_l * sin(theta_r)) / denom;
	    y = (rho_l * cos(theta_r) - rho_r * cos(theta_l)) / denom;
	    //cout<<"("<<x<<","<<y<<")"<<endl;
	    Point pt(x, y);
	    Intersection.push_back(pt);
	    circle(image, pt, 5, Scalar(0, 0, 0),5 );
	}
    }

    return get_vanish_point(Intersection, image);
}

Point get_vanish_point(vector<Point> & points, Mat & cdst)
{
    long x = 0, y = 0;
    if (points.size() == 0) {
	return Point(0, 0);
    }
    for (size_t i = 0; i < points.size(); ++i) {
	x += points[i].x;
	y += points[i].y;
    }
    x /= points.size();
    y /= points.size();
    Point vp(x, y);
    circle(cdst, vp, 5, Scalar(0, 0, 0),5 );
    return vp;
}

bool hough_line_detect(Mat & image, Mat & cdst, vector<Vec2f> & left_lines, vector<Vec2f> & right_lines)
{
    Mat dst;
    Canny(image, dst, 30, 70, 3);
    cvtColor(dst, cdst, CV_GRAY2BGR);

    vector<Vec2f> lines;
    // detect lines
    HoughLines(dst, lines, 1, CV_PI / 180, 150, 0, 0);
    for (size_t i = 0; i < lines.size(); i++) {
	float rho = lines[i][0], theta = lines[i][1];
	if (10 * PI / 180 < theta && theta < 60 * PI / 180) {
	    left_lines.push_back(lines[i]);
	} else if (110 * PI / 180 < theta && theta < 170 * PI / 180) {
	    right_lines.push_back(lines[i]);
	}
    }
    return true;
}

bool draw_line(Mat & image, vector<Vec2f> & vec_lines, Scalar color)
{
    for (size_t i = 0; i < vec_lines.size(); ++i) {
	float rho = vec_lines[i][0], theta = vec_lines[i][1];
	Point pt1, pt2;
	double a = cos(theta), b = sin(theta);
	double x0 = a*rho, y0 = b*rho;
	pt1.x = cvRound(x0 + 1000 * (-b));
	pt1.y = cvRound(y0 + 1000 * (a));
	pt2.x = cvRound(x0 - 1000 * (-b));
	pt2.y = cvRound(y0 - 1000 * (a));
	cv::line(image, pt1, pt2, color, 3, CV_AA);
    }
    return true;
}
