#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

class BEVVisualizer {
public:
	BEVVisualizer(float scale = 10.0f);

	cv::Mat drawBEV(const std::vector<cv::Point3f>& objects);

private:
	float px_per_meter;
	int width = 400;
	int height = 600;
};
