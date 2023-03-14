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
	BoundingBox(const Vec2i ll, const Vec2i ur);
	BoundingBox(const Vec2i center);

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

	bool isInsideY(const BoundingBox &other) const;
	bool isInsideX(const BoundingBox &other) const;
	inline bool isInBetweenX(const int x) const
	{
		return x >= lowerLeftCorner[1] && x <= upperRightCorner[1];
	}
	inline bool isInBetweenY(const int y) const
	{
		//0 is top left corner. 100 is lower than 10
		return y <= lowerLeftCorner[0] && y >= upperRightCorner[0];
	}
	bool contains(const BoundingBox& other) const;

	float getDistance(const BoundingBox& other) const;
	float getMinimumYDistance(const BoundingBox& other) const;
	float getMinimumXDistance(const BoundingBox& other) const;

	float getMinimalDistanceToAllCorners(Vec2i point) const;
	static inline float distance(Vec2i one, Vec2i two);

};

#endif // BOUNDINGBOX_H
