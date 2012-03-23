#ifndef EXTRACTNUMBER_H
#define EXTRACTNUMBER_H

#include <vector>
#include <string>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class ExtractNumber
{
public:
	ExtractNumber();
	ExtractNumber(cv::Mat image);
	void setSource(cv::Mat image);
	int extract();

private:
	cv::Mat source;
	void split(std::vector<cv::Mat> &character);
	int number(cv::Mat character);
	int rowBrightPattern(const cv::Mat &bin, const int column);
	int colBrightPattern(const cv::Mat &bin, const int row);
	int utan(const cv::Mat &bin);
	int satan(const cv::Mat &bin);
};

#endif // EXTRACTNUMBER_H
