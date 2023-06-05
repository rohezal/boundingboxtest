#include "boundingbox.h"

BoundingBox::BoundingBox()
{
	srand (time(NULL));
	lowerLeftCorner = Vec2i(0,0);
	upperRightCorner = Vec2i(0,0);
	center = (lowerLeftCorner+upperRightCorner)/2;
}

BoundingBox::BoundingBox(Island &island) : BoundingBox()
{
	int minX = INT_MAX;
	int maxX = INT_MIN;

	int minY = INT_MAX;
	int maxY = INT_MIN;


	for(size_t i = 0; i < island.pixels.size(); i++)
	{
		const Vec2i pixel = island.pixels[i];
		const int x = pixel[1];
		const int y = pixel[0];

		minX = x < minX? x : minX;
		minY = y < minY? y : minY;

		maxX = x > maxX? x : maxX;
		maxY = y > maxY? y : maxY;
	}

	/*
	maxX = (maxX-minX < 2) ? maxX+2 : maxX;
	maxY = (maxY-minY < 2) ? maxY+2 : maxY;
	*/

	lowerLeftCorner = Vec2i(minY,minX);
	upperRightCorner = Vec2i(maxY,maxX);
	center = (lowerLeftCorner+upperRightCorner)/2;

	islands.push_back(&island);



}

BoundingBox::BoundingBox(const Vec2i ll, const Vec2i ur): BoundingBox()
{
	lowerLeftCorner = ll;
	upperRightCorner = ur;
	center = (lowerLeftCorner+upperRightCorner)/2;
}

BoundingBox::BoundingBox(const Vec2i center) : BoundingBox()
{
	lowerLeftCorner = center;
	upperRightCorner = center;
	this->center = center;
	upperRightCorner[0] += 2;
	upperRightCorner[1] += 2;
	std::cout << getArea() << std::endl;
}

void BoundingBox::draw(Mat &image) const
{
	if(this->getArea() < minDrawArea)
	{
		return;
	}

	int minX = lowerLeftCorner[1];
	int maxX = upperRightCorner[1];

	int minY = lowerLeftCorner[0];
	int maxY = upperRightCorner[0];

	/*
	maxX = (maxX-minX < 2) ? maxX+2 : maxX;
	maxY = (maxY-minY < 2) ? maxY+2 : maxY;
	*/

	Vec2i _lowerLeftCorner = Vec2i(minY,minX);
	Vec2i _upperRightCorner = Vec2i(maxY,maxX);

	Scalar line_Color(rand()%256, rand()%256, rand()%256);//Color of the line
	Point ll(_lowerLeftCorner[1],_lowerLeftCorner[0]); //lower left
	Point ul(_upperRightCorner[1],_lowerLeftCorner[0]); //lower right
	Point ur(_upperRightCorner[1],_upperRightCorner[0]); // upper right
	Point lr(_lowerLeftCorner[1],_upperRightCorner[0]); //lower right
	int thickness = 1;

	//draw the box with 4 lines
	line(image, ll, lr, line_Color, thickness);//using line() function to draw the line//
	line(image, ul, ur, line_Color, thickness);//using line() function to draw the line//
	line(image, ll, ul, line_Color, thickness);//using line() function to draw the line//
	line(image, lr, ur, line_Color, thickness);//using line() function to draw the line//

}

bool BoundingBox::isInsideY(const BoundingBox &other) const
{
	//0 is on the top left. So y=100 is "lower" y=10.
	return isInBetweenY(other.lowerLeftCorner[0]) || isInBetweenY(other.upperRightCorner[0]);
}

bool BoundingBox::isInsideX(const BoundingBox &other) const
{
	return isInBetweenX(other.lowerLeftCorner[1]) || isInBetweenX(other.upperRightCorner[1]);
}

bool BoundingBox::contains(const BoundingBox &other) const
{
	const bool is_inside_x = isInsideX(other);
	const bool is_inside_y = isInsideY(other);
	return is_inside_x && is_inside_y;
}

float BoundingBox::getDistance(const BoundingBox &other) const
{
	if(this->contains(other) || other.contains(*this) )
	{
		return 0;
	}

	//if(this->isInsideY(other) || other.isInsideY(*this) )
	if(false)
	{
		/*   y1
		 *		<--------> distance x
		 *   y0
		 *
		 */

		return getMinimumXDistance(other);
	}

	//if(this->isInsideX(other) || other.isInsideX(*this) )
	if(false)
	{
		/*		distance y
		 *		|
		 *		|
		 *		|
		 *   x0   x1
		 */
		return getMinimumYDistance(other);
	}

	const Vec2i ll(lowerLeftCorner[0],lowerLeftCorner[1]); //lower left
	const Vec2i ul(upperRightCorner[0],lowerLeftCorner[1]); //lower right
	const Vec2i ur(upperRightCorner[0],upperRightCorner[1]); // upper right
	const Vec2i lr(lowerLeftCorner[0],upperRightCorner[1]); //lower right

	const float d1 = other.getMinimalDistanceToAllCorners(ll);
	const float d2 = other.getMinimalDistanceToAllCorners(ul);
	const float d3 = other.getMinimalDistanceToAllCorners(ur);
	const float d4 = other.getMinimalDistanceToAllCorners(lr);

	return std::min( std::min( std::min(d1,d2), d3), d4);
}

float BoundingBox::getMinimumYDistance(const BoundingBox &other) const
{
	const int distance_thislower_otherupper = std::abs(lowerLeftCorner[0] - other.upperRightCorner[0]);
	const int distance_thisupper_otherlower = std::abs(upperRightCorner[0] - other.lowerLeftCorner[0]);

	return std::min(distance_thislower_otherupper,distance_thisupper_otherlower)*y_penalty;
}

float BoundingBox::getMinimumXDistance(const BoundingBox &other) const
{
	const int distance_thislower_otherupper = std::abs(lowerLeftCorner[1] - other.upperRightCorner[1]);
	const int distance_thisupper_otherlower = std::abs(upperRightCorner[1] - other.lowerLeftCorner[1]);

	return std::min(distance_thislower_otherupper,distance_thisupper_otherlower) * x_penalty;
}

float BoundingBox::getCenterDistance(const BoundingBox &other) const
{
	const Vec2i one = this->center;
	const Vec2i two = other.center;


	const float distance_y = (one[0]-two[0])*(one[0]-two[0]) * y_penalty;
	const float distance_x = (one[1]-two[1])*(one[1]-two[1]) * x_penalty;
	return std::sqrt( distance_y + distance_x );

}

float BoundingBox::getIslandQuality() const
{
	assert (islands.size() > 0);

	return islands.front()->quality;
}

std::vector<BoundingBox *> BoundingBox::getGoodBoxes(std::vector<BoundingBox> &boxes) const
{
	std::vector<BoundingBox*> goodBoxes;
	const size_t NUMBER_OF_BOXES = boxes.size();
	for(size_t i = 0; i < NUMBER_OF_BOXES; i++)
	{
		const float distance = getDistance(boxes[i]);
		if( (distance/2*std::sqrt(distance)) < boxes[i].getIslandQuality())
		{
			if(this != &boxes[i])
			{
				goodBoxes.push_back(&boxes[i]);
			}
		}
	}

	return goodBoxes;

}

float BoundingBox::getMinimalDistanceToAllCorners(Vec2i point) const
{
	const Vec2i ll(lowerLeftCorner[0],lowerLeftCorner[1]); //lower left
	const Vec2i ul(upperRightCorner[0],lowerLeftCorner[1]); //lower right
	const Vec2i ur(upperRightCorner[0],upperRightCorner[1]); // upper right
	const Vec2i lr(lowerLeftCorner[0],upperRightCorner[1]); //lower right

	const float d1 = distance(ll,point);
	const float d2 = distance(ul,point);
	const float d3 = distance(ur,point);
	const float d4 = distance(lr,point);

	return std::min( std::min( std::min(d1,d2), d3), d4);
}

float BoundingBox::distance(Vec2i one, Vec2i two)
{
	const float distance_y = (one[0]-two[0])*(one[0]-two[0]) * y_penalty;
	const float distance_x = (one[1]-two[1])*(one[1]-two[1]) * x_penalty;
	return std::sqrt( distance_y + distance_x );
}

int BoundingBox::minDrawArea = 0;

float BoundingBox::x_penalty = Island::factor_distance_in_x_direction_penality;
float BoundingBox::y_penalty = Island::factor_distance_in_y_direction_penality;

bool BoundingBox::isInBetween(const Vec2i point) const
{
		return isInBetweenY(point[0]) && isInBetweenX(point[1]);
}
