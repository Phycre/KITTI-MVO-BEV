#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

class VisualOdometry {
public:
	VisualOdometry(cv::Mat P2);

	void update(const cv::Mat& curr_img, const cv::Mat& mask, double speed);

	cv::Mat getR() const { return R_f; }
	cv::Mat getT() const { return t_f; }

	cv::Mat getGlobalR() const { return R_total; }
	cv::Mat getGlobalT() const { return t_total; }

	cv::Point3f transformToWorld(const cv::Point3f& local_pt);

private:
	cv::Mat K;

	cv::Mat R_f, t_f;

	//Acum total
	cv::Mat R_total, t_total;

	cv::Mat prev_img;
	std::vector<cv::Point2f> prev_pts;
};
