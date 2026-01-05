#include "GeometryUtils.hpp"

cv::Point3f GeometryUtils::projectTo3D(const cv::Rect2d& bbox, const cv::Mat& P2) {
	cv::Point2d uv(bbox.x + bbox.width / 2.0, bbox.y + bbox.height);

	double fu = P2.at<double>(0, 0);
	double fv = P2.at<double>(1, 1);
	double cu = P2.at<double>(0, 2);
	double cv = P2.at<double>(1, 2);

	double dy = (uv.y - cv) / fv;

	if (dy <= 0.001) {
		return cv::Point3f(0, 0, 0);
	}

	double s = CAMERA_HEIGHT / dy;

	double X = s * (uv.x - cu) / fu;
	double Y = CAMERA_HEIGHT;
	double Z = s * 1.0;

	return cv::Point3f((float)X, (float)Y, (float)Z);
}
