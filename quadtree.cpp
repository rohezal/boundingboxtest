#include "quadtree.h"

Quadtree::Quadtree(BoundingBox dims, Quadtree *parent, std::set<Island> *islands, int level)
{
	nodes[0] = nullptr;
	nodes[1] = nullptr;
	nodes[2] = nullptr;
	nodes[3] = nullptr;

    this->parent = parent;

    this->level = level;

    this->islands = islands;

    if(islands->size() > 4 && level < 10)
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
        BoundingBox ur = BoundingBox(centerBox,dims.upperRightCorner); //upper right
        BoundingBox lr = BoundingBox(centerLowerEdge,centerRightEdge); //lower right

        std::set<Island> *isUL;
        std::set<Island> *isLL;
        std::set<Island> *isUR;
        std::set<Island> *isLR;

        for(size_t i = 0; i < islands->size(); i++){
            for(size_t j = 0; j < islands->at(i).pixels.size(); j++)
            {
                if(ul.isInBetweenY(islands->at(i).pixels.at(j)[0]) && ul.isInBetweenX(islands->at(i).pixels.at(j)[1])) {
                    isUL->insert(islands->at(i));
                }
                if(ll.isInBetweenY(islands->at(i).pixels.at(j)[0]) && ll.isInBetweenX(islands->at(i).pixels.at(j)[1])) {
                    isLL->insert(islands->at(i));
                }
                if(ur.isInBetweenY(islands->at(i).pixels.at(j)[0]) && ur.isInBetweenX(islands->at(i).pixels.at(j)[1])) {
                    isUR->insert(islands->at(i));
                }
                if(lr.isInBetweenY(islands->at(i).pixels.at(j)[0]) && lr.isInBetweenX(islands->at(i).pixels.at(j)[1])) {
                    isLR->insert(islands->at(i));
                }
            }
        }

        nodes[0] = Quadtree(ul, this, isUL, level+1);
        nodes[1] = Quadtree(ul, this, isUL, level+1);
        nodes[2] = Quadtree(ul, this, isUL, level+1);
        nodes[3] = Quadtree(ul, this, isUL, level+1);
    }

}
