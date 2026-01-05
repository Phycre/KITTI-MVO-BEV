#pragma once
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

struct KittiLabel {
	std::string type;
	float truncated;
	float occluded;
	float alpha;
	cv::Rect2d bbox;
};

class KittiLoader {
public:
	KittiLoader(const std::string& base_path, const std::string& sequence_id);

	void loadAllLabels();
	bool loadCalibration();
	std::map<int, std::vector<KittiLabel>> all_labels;
	std::vector<KittiLabel> getLabels(int);
	cv::Mat getImage(int frame_idx);
	cv::Mat getP2() const { return P2; }
	int getTotalFrames() const { return total_frames; }

private:
	std::string base_path;
	std::string seq_id;
	cv::Mat P2;
	int total_frames;
};
