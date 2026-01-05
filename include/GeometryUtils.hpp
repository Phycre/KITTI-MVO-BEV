#pragma once
#include <opencv2/opencv.hpp>

class GeometryUtils {
public:
	static cv::Point3f projectTo3D(const cv::Rect2d& bbox, const cv::Mat& P2);

	//Base Kitti color camera height
	static constexpr float CAMERA_HEIGHT = 1.65f; 
};
