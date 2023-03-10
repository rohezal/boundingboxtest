#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <vector>
#include <crackdetection/island.h>
#include <opencv.hpp>
#include <stdlib.h>
#include <time.h>


using namespace cv;

class BoundingBox
{
public:
	BoundingBox();
	BoundingBox(const Island& island);

	BoundingBox* parent;
	std::vector<BoundingBox*> children;
	std::vector<Island*> islands;

	cv::Vec2i lowerLeftCorner;
	cv::Vec2i upperRightCorner;

	void draw(Mat& image) const;
	void add(Island* island);
	inline int getArea() const
	{
		return (upperRightCorner[0]-lowerLeftCorner[0])*(upperRightCorner[1]-lowerLeftCorner[1]);
	}
	const int minDrawArea = 4;

};

#endif // BOUNDINGBOX_H
