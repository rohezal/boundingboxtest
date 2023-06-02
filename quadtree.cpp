#include "quadtree.h"

Quadtree::Quadtree(BoundingBox dims, Quadtree *parent, std::vector<Island> *islands)
{
	nodes[0] = nullptr;
	nodes[1] = nullptr;
	nodes[2] = nullptr;
	nodes[3] = nullptr;

	if(parent == nullptr)
	{
		if(islands->size() > 0)
		{
			//[0] is y, 0,0 is upper left corner
			Vec2i upperLeftCorner (dims.upperRightCorner[0], dims.lowerLeftCorner[1]);
			Vec2i lowerRightCorner (dims.lowerLeftCorner[0], dims.upperRightCorner[1]);

			//lets calculate in between points

			int centerX = dims.lowerLeftCorner[1] + (lowerRightCorner[1] - dims.lowerLeftCorner[1]) / 2;
			int centerY = upperLeftCorner[0] + (dims.lowerLeftCorner[0] - upperLeftCorner[0]) / 2;

			Vec2i centerLeftEdge(centerY,upperLeftCorner[1]);
			Vec2i centerTopEdge(upperLeftCorner[0],centerX);

			Vec2i centerBox(centerY,centerX);

			Vec2i centerLowerEdge(lowerRightCorner[0],centerX);
			Vec2i centerRightEdge(centerY,lowerRightCorner[1]);

			BoundingBox ul = BoundingBox(centerLeftEdge,centerTopEdge); //upper left
			BoundingBox ll = BoundingBox(dims.lowerLeftCorner,centerBox); //lower left
			BoundingBox ur = BoundingBox(centerBox,dims.upperRightCorner); //lower left
			BoundingBox lr = BoundingBox(centerLowerEdge,centerRightEdge); //lower left
		}
	}
}
