#include "quadtree.h"

cv::Mat* Quadtree::image = nullptr;

Quadtree::Quadtree(BoundingBox dims, Quadtree *_parent, std::set<const Island*> &_islands, size_t _level) : box(dims)
{
	nodes[0] = nullptr;
	nodes[1] = nullptr;
	nodes[2] = nullptr;
	nodes[3] = nullptr;

	this->parent = _parent;
	this->level = _level;
	this->islands = _islands;

	if(_islands.size() > MIN_ELEMENTS && _level < MAX_LEVEL)
    {
		std::cout << this <<  " | Level: " << _level << std::endl;
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
        BoundingBox ur = BoundingBox(centerBox,dims.upperRightCorner); //upper right
        BoundingBox lr = BoundingBox(centerLowerEdge,centerRightEdge); //lower right

		std::set<const Island*> isUL;
		std::set<const Island*> isLL;
		std::set<const Island*> isUR;
		std::set<const Island*> isLR;

		for(std::set<const Island*>::iterator it = islands.begin(); it != islands.end(); it++)
		{
			for(size_t j = 0; j < (*it)->pixels.size(); j++)
            {
				const Vec2i point = (*it)->pixels.at(j);
				const Island* island = *it;

				if(ul.isInBetween(point))
				{
					isUL.insert(island);
				}
				if(ll.isInBetween(point))
				{
					isLL.insert(island);
                }
				if(ur.isInBetween(point))
				{
					isUR.insert(island);
                }
				if(lr.isInBetween(point))
				{
					isLR.insert(island);
                }
            }
        }

		if(isUL.size() > 0)
		{
			nodes[0] = new Quadtree(ul, this, isUL, _level+1);
		}

		if(isLL.size() > 0)
		{
			nodes[1] = new Quadtree(ll, this, isLL, _level+1);
		}

		if(isUR.size() > 0)
		{
			nodes[2] = new Quadtree(ur, this, isUR, _level+1);
		}

		if(isLR.size() > 0)
		{
			nodes[3] = new Quadtree(lr, this, isLR, _level+1);
		}
	}
}

void Quadtree::draw(int _level)
{
	if(_level == -1)
	{
		if(image != nullptr)
		{
			box.draw(*image);

			if(nodes[0] != nullptr)
			{
				nodes[0]->draw(_level);
			}
			if(nodes[1] != nullptr)
			{
				nodes[1]->draw(_level);
			}
			if(nodes[2] != nullptr)
			{
				nodes[2]->draw(_level);
			}
			if(nodes[3] != nullptr)
			{
				nodes[3]->draw(_level);
			}
		}
	}

	if(_level == level)
	{
		if(image != nullptr)
		{
			box.draw(*image);
		}
	}
}
