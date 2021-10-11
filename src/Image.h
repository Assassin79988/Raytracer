#ifndef IMAGE_H_
#define IMAGE_H_

#include "opencv2/core/core.hpp"       
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"

namespace raytracer {
	class Image;
}

class raytracer::Image {
private:
	const unsigned DEFAULT_COLS = 100;
	const unsigned DEFAULT_ROWS = 100;

	/* Number of pixels in a row */
	unsigned cols_;
	/* Number of pixels in a column */
	unsigned rows_;
public:
	/* Matrix that stores the image */
	cv::Mat image = cv::Mat(rows_, cols_, CV_8UC3, cv::Scalar(255, 255, 255));

	/* Constructors */
	Image() :cols_(DEFAULT_COLS), rows_(DEFAULT_ROWS) {};
	Image(unsigned xRes, unsigned yRes) : cols_(xRes), rows_(yRes) {};

	/* Getters and Setters */
	unsigned getCols() { return cols_; }
	unsigned getRows() { return rows_; }
	unsigned setCols(unsigned xRes) { cols_ = xRes; }
	unsigned setRows(unsigned yRes) { rows_ = yRes; }

	cv::Vec3b& operator()(int row, int col);

	/* Displays the image and pauses program execution (until key pressed) */
	void display();

	/* Use files with "*.png" extension */
	void save(const std::string& filename);
};

cv::Vec3b& raytracer::Image::operator()(int row, int col)
{
	assert(row >= 0 && row < rows_);
	assert(col >= 0 && col < cols_);
	return image.at<cv::Vec3b>(cv::Point(col, row));
}
void raytracer::Image::display() {
	/// Display the image
	cv::imshow("image", image);
	cv::waitKey(0);
}

void raytracer::Image::save(const std::string& filename) {
	/// Save the image
	cv::imwrite(filename, image);
}

#endif
