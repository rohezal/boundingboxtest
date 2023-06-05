#ifndef QUADTREE_H
#define QUADTREE_H

#include "boundingbox.h"
#include <opencv.hpp>
#include <set>


class Quadtree
{
public:
    Quadtree(BoundingBox dims, Quadtree* parent, std::set<Island>* islands, int level = 0);

	BoundingBox box;
	Quadtree* nodes[4];
	Quadtree* parent;
    int level;
    std::set<Island>* islands;

	bool inline isRoot()
	{
		return parent == nullptr;
	}

    bool inline isLeaf() {
        return (nodes[0] == nullptr && nodes[1] == nullptr && nodes[2] == nullptr && nodes[3] == nullptr);
    }
};

#endif // QUADTREE_H
