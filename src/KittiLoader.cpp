#include "KittiLoader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

KittiLoader::KittiLoader(const std::string& base_path, const std::string& sequence_id) : base_path(base_path), seq_id(sequence_id) {
	std::string img_dir = base_path + "/image_02/" + seq_id + "/";

	std::vector<cv::String> filenames;
	cv::glob(img_dir + "*.png", filenames);
	total_frames = filenames.size();
	
	loadCalibration();
	loadAllLabels();
}

void KittiLoader::loadAllLabels(){
	std::string label_file = base_path + "/label_02/" + seq_id + ".txt";
	std::ifstream file(label_file);
	if (!file.is_open()) {
		std::cerr << "Label file not found: " << label_file << std::endl;
		return;
	}

	std::string line;
	int line_count = 0;
	while (std::getline(file, line)) {
		if (line.empty()) continue;
		std::stringstream ss(line);
		int frame_idx, track_id;
		std::string type;

		ss >> frame_idx >> track_id >> type;

		float dummy;
		ss >> dummy >> dummy >> dummy;

		double x1, y1, x2, y2;
		if (!(ss >> x1 >> y1 >> x2 >> y2)) {
			std::cerr << "Line " << line_count << " failed to parse Bbox!" << std::endl;
			continue;
		}

		KittiLabel lbl;
		lbl.bbox = cv::Rect2d(x1, y1, x2 - x1, y2 - y1);
		lbl.type = type;

		all_labels[frame_idx].push_back(lbl);
		line_count++;
	}
}

bool KittiLoader::loadCalibration() {
	std::string calib_file = base_path + "/calib/" + seq_id + ".txt"; 

	std::ifstream file(calib_file);
	if (!file.is_open()) {
		std::cerr << "Failed to open calibration file: " << calib_file << std::endl;
		return false;
	}

	std::string line;
	while (std::getline(file, line)) {
		if (line.find("P2:") != std::string::npos) {
			std::stringstream ss(line.substr(3));
			P2 = cv::Mat::eye(3, 4, CV_64F);
			for (int i = 0; i < 12; ++i) ss >> P2.at<double>(i / 4, i % 4);
			return true;
		}
	}
	return false;
}

std::vector<KittiLabel> KittiLoader::getLabels(int frame_idx) {
	if (all_labels.count(frame_idx)) {
		return all_labels[frame_idx];
	}
	return {};
}

cv::Mat KittiLoader::getImage(int frame_idx) {
	std::stringstream ss;
	ss << std::setw(6) << std::setfill('0') << frame_idx;
	std::string img_path = base_path + "/image_02/" + seq_id + "/" + ss.str() + ".png";
	return cv::imread(img_path);
}
