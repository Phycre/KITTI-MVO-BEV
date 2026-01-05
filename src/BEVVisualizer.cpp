#include "BEVVisualizer.hpp"

BEVVisualizer::BEVVisualizer(float scale) : px_per_meter(scale) {}

cv::Mat BEVVisualizer::drawBEV(const std::vector<cv::Point3f>& objects) {
	cv::Mat bev = cv::Mat::zeros(height, width, CV_8UC3);

	cv::Point center(width / 2, height - 20);
	std::vector<cv::Point> ego_tri = { 
		center + cv::Point(-10, 10), 
		center + cv::Point(10, 10), 
		center + cv::Point(0, -15) 
	};

	cv::fillPoly(bev, std::vector<std::vector<cv::Point>>{ego_tri}, cv::Scalar(0, 255, 0));

	for (const auto& obj : objects) {
		if (obj.z <= 0) continue;

		int x_map = center.x + (int)(obj.x * px_per_meter);
		int y_map = center.y - (int)(obj.z * px_per_meter);

		if (x_map >= 0 && x_map < width && y_map >= 0 && y_map < height) {
			cv::circle(bev, cv::Point(x_map, y_map), 4, cv::Scalar(0, 0, 255), -1);

			std::string dist = std::to_string((int)obj.z) + "m";
			cv::putText(bev, dist, cv::Point(x_map+5, y_map), cv::FONT_HERSHEY_SIMPLEX, 0.3, cv::Scalar(255,255,255));
		}
	}
	return bev;
}
