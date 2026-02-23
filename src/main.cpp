#include <iostream>
#include <opencv2/opencv.hpp>
#include "KittiLoader.hpp"
#include "GeometryUtils.hpp"
#include "BEVVisualizer.hpp"

int main() {
	std::string base_path = "../data/training";
	std::string seq_id = "0004";
	KittiLoader loader(base_path, seq_id);
	BEVVisualizer bev_viz(10.0f);

	for (int i = 0; i < loader.getTotalFrames(); ++i) {
		cv::Mat frame = loader.getImage(i);
		if (frame.empty()) break;

		std::vector<KittiLabel> labels = loader.getLabels(i);
		std::vector<cv::Point3f> object_positions;

		for (const auto& lbl : labels) {
			if (lbl.type == "Car" || lbl.type == "Van" || lbl.type == "Truck") {
				cv::rectangle(frame, lbl.bbox, cv::Scalar(0, 255, 0), 2);

				cv::Point3f pos = GeometryUtils::projectTo3D(lbl.bbox, loader.getP2());
				object_positions.push_back(pos);
			}
		}

		cv::Mat bev = bev_viz.drawBEV(object_positions);

		cv::imshow("Camera", frame);
		cv::imshow("BEV", bev);

		char key = (char)cv::waitKey(50);
		if (key == 27) break;
	}

	return 0;
}
