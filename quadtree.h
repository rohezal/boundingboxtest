#ifndef QUADTREE_H
#define QUADTREE_H

#include "boundingbox.h"
#include <opencv.hpp>
#include <set>
#include <map>
#include <utility>

class Quadtree
{
public:
	Quadtree(BoundingBox dims, Quadtree* _parent, std::set<const Island*>& _islands, size_t _level = 0);

	BoundingBox box;
	Quadtree* nodes[4];
	Quadtree* parent;
    int level;
	std::set<const Island*> islands;
	static std::map<const Island*, BoundingBox*> islandToBoundingBoxLookup;

	static std::map<const Island*, Quadtree*> islandToQuadtreeCellLookup;

	static std::map<const Island*, std::vector<Quadtree*> > islandToQuadtreeVectorLookup;


	static cv::Mat* image;

	const size_t MAX_LEVEL = 10;
	const size_t MIN_ELEMENTS = 8;
	const int NUMBER_OF_NODES = 4;

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

	inline bool hasParent()
	{
		return !isRoot();
	}

	bool containsPixel(const Vec2i pixel) const;

	Quadtree* getCellOfPixel(Vec2i pixel);

	std::pair<const Island*, float> getFirstEstimate(Island* _island, const Vec2i pixel);
	std::vector<const Island*> getCandidates(Island* _island);
};

inline std::ostream &operator<<(std::ostream &os, const Quadtree &quad)
{
	os << quad.level << " | " << quad.box;
   return os;
}

#endif // QUADTREE_H
