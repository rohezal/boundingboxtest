#ifndef QUADTREE_H
#define QUADTREE_H

#include "boundingbox.h"
#include <opencv.hpp>


class Quadtree
{
public:
	Quadtree(BoundingBox dims, Quadtree* parent, std::vector<Island>* islands);

	BoundingBox box;
	Quadtree* nodes[4];
	Quadtree* parent;

	bool inline isRoot()
	{
		return parent == nullptr;
	}
};

#endif // QUADTREE_H
