#include "VisualOdometry.hpp"
#include <iostream>

VisualOdometry::VisualOdometry(cv::Mat P2) {
	K = P2(cv::Rect(0, 0, 3, 3)).clone();

	R_f = cv::Mat::eye(3, 3, CV_64F);
	t_f = cv::Mat::zeros(3, 1, CV_64F);

	R_total = cv::Mat::eye(3, 3, CV_64F);
	t_total = cv::Mat::zeros(3, 1, CV_64F);
}

void VisualOdometry::update(const cv::Mat& curr_img, const cv::Mat& mask, double speed) {
	cv::Mat curr_gray;
	if (curr_img.channels() == 3) 
		cv::cvtColor(curr_img, curr_gray, cv::COLOR_BGR2GRAY);
	else
		curr_gray = curr_img;

	//init frame1
	if (prev_img.empty()) {
		prev_img = curr_gray.clone();
		return;
	}

	if (prev_pts.size() < 1000) {
		std::vector<cv::Point2f> new_pts;
		// fastThreshold=20, nonmaxSuppression=true
		cv::FAST(prev_img, new_pts, 20, true); 

		for (auto& p : new_pts) {
			if (mask.at<uchar>(p) > 0) {
				prev_pts.push_back(p);
			}
		}
	}

	std::vector<cv::Point2f> curr_pts;
	std::vector<uchar> status;
	std::vector<float> err;

	if (!prev_pts.empty()) {
		cv::calcOpticalFlowPyrLK(prev_img, curr_gray, prev_pts, curr_pts, status, err);
	}

	std::vector<cv::Point2f> good_prev, good_curr;
	for (size_t i = 0; i < status.size(); i++) {
		if (status[i]) {
			if (curr_pts[i].x < 0 || curr_pts[i].y < 0 || curr_pts[i].x >= curr_img.cols || curr_pts[i].y >= curr_img.rows) continue;

			good_prev.push_back(prev_pts[i]);
			good_curr.push_back(curr_pts[i]);
		}
	}

	if (good_prev.size() > 50) {
		cv::Mat E, mask_ransac;
		E = cv::findEssentialMat(good_curr, good_prev, K, cv::RANSAC, 0.999, 1.0, mask_ransac);

		cv::recoverPose(E, good_curr, good_prev, K, R_f, t_f, mask_ransac);

		if (speed > 0.1) {
			t_f = t_f * speed;
		} else {
			t_f = cv::Mat::zeros(3, 1, CV_64F);
		}

		t_total = t_total + (R_total * t_f);
		R_total = R_total * R_f;
	}

	prev_img = curr_gray.clone();
	prev_pts = good_curr;
	}

	cv::Point3f VisualOdometry::transformToWorld(const cv::Point3f& local_pt) {
	//3x3 * 3x1
	double x = R_total.at<double>(0,0)*local_pt.x + R_total.at<double>(0,1)*local_pt.y + R_total.at<double>(0,2)*local_pt.z + t_total.at<double>(0);
	double y = R_total.at<double>(1,0)*local_pt.x + R_total.at<double>(1,1)*local_pt.y + R_total.at<double>(1,2)*local_pt.z + t_total.at<double>(1);
	double z = R_total.at<double>(2,0)*local_pt.x + R_total.at<double>(2,1)*local_pt.y + R_total.at<double>(2,2)*local_pt.z + t_total.at<double>(2);

	return cv::Point3f((float)x, (float)y, (float)z);
}
