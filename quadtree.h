#ifndef QUADTREE_H
#define QUADTREE_H

#include "boundingbox.h"
#include <opencv.hpp>
#include <set>


class Quadtree
{
public:
	Quadtree(BoundingBox dims, Quadtree* _parent, std::set<const Island*>& _islands, size_t _level = 0);

	BoundingBox box;
	Quadtree* nodes[4];
	Quadtree* parent;
    int level;
	std::set<const Island*> islands;

	static cv::Mat* image;

	const size_t MAX_LEVEL = 10;
	const size_t MIN_ELEMENTS = 4;

	void draw(int _level = -1);

	bool inline isRoot()
	{
		return parent == nullptr;
	}

    bool inline isLeaf() {
        return (nodes[0] == nullptr && nodes[1] == nullptr && nodes[2] == nullptr && nodes[3] == nullptr);
    }

	static void setImage(cv::Mat& _image)
	{
		image = &_image;
	}
};

#endif // QUADTREE_H
