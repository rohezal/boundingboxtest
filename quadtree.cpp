#include "quadtree.h"

cv::Mat* Quadtree::image = nullptr;
std::map<const Island*, BoundingBox*> Quadtree::islandToBoundingBoxLookup;
std::map<const Island*, std::vector<Quadtree*> > Quadtree::islandToQuadtreeVectorLookup;


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


		//Unoverlapping points

		Vec2i centerBoxMovedTopRight(centerY+1,centerX+1);
		Vec2i centerLeftEdgeMovedTop(centerY+1,upperLeftCorner[1]);
		Vec2i centerLowerEdgeMovedRight(lowerRightCorner[0],centerX+1);




		BoundingBox ul = BoundingBox(centerLeftEdgeMovedTop,centerTopEdge); //upper left
        BoundingBox ll = BoundingBox(dims.lowerLeftCorner,centerBox); //lower left
		BoundingBox ur = BoundingBox(centerBoxMovedTopRight,dims.upperRightCorner); //upper right
		BoundingBox lr = BoundingBox(centerLowerEdgeMovedRight,centerRightEdge); //lower right

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

		if(isLeaf())
		{
			for(std::set<const Island*>::iterator it = islands.begin(); it != islands.end(); it++)
			{
				islandToQuadtreeVectorLookup[*it].push_back(this);
			}
		}
	}
}

void Quadtree::draw(int _level)
{
	if(_level == -1)
	{
		if(image != nullptr)
		{
			if(isLeaf())
			{
				box.draw(*image);
			}
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

bool Quadtree::containsPixel(const Vec2i pixel) const
{
	return box.contains(pixel);
}

Quadtree *Quadtree::getCellOfPixel(Vec2i pixel)
{
	if(box.contains(pixel))
	{
		for(int i = 0; i < NUMBER_OF_NODES; i++)
		{
			if(nodes[i])
			{
				if(nodes[i]->containsPixel(pixel))
				{
					return(nodes[i]->getCellOfPixel(pixel));
				}
			}
		}
		return this;
	}
	return nullptr;
}

std::pair<const Island*, float>  Quadtree::getFirstEstimate(Island* _island, const Vec2i pixel)
{
	//get the islands in the cell and the parent node
	Quadtree* cell = getCellOfPixel(pixel);

	std::vector<const Island*> candidates;

	std::set<const Island*>::iterator it;

	std::pair<const Island*, float> result;
	result.first = nullptr;
	result.second = std::numeric_limits<float>::max();

	for(it = cell->islands.begin(); it != cell->islands.end(); it++)
	{
		if(*it != _island)
		{
			candidates.push_back(*it);
		}
	}

	if(candidates.size() == 0)
	{
		if(this->hasParent())
		{
			return parent->getFirstEstimate(_island,pixel);
		}
	}

	for(size_t i = 0; i < candidates.size(); i++)
	{
		const float distance = _island->calculateDistanceFromIsland(*candidates[i]);

		if(distance < result.second)
		{
			result.second =distance;
			result.first = candidates[i];
		}

	}

	return result;
}

std::vector<const Island *> Quadtree::getCandidates(Island *_island)
{
	std::vector<const Island*> result;

	return result;
}
