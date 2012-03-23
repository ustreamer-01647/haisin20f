#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>
#include <QImage>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

QImage mat2qimage(const cv::Mat &mat, const bool isIndexed8 = false);
cv::Mat qimage2mat(const QImage& qimage);
cv::Mat topPlaneofMat(const cv::Mat &mat);
cv::Mat thinningMomma(const cv::Mat &input);

#endif // FUNCTION_H
