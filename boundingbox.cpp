#include "boundingbox.h"

BoundingBox::BoundingBox()
{
	srand (time(NULL));
	lowerLeftCorner = Vec2i(0,0);
	upperRightCorner = Vec2i(0,0);
}

BoundingBox::BoundingBox(const Island &island) : BoundingBox()
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

	maxX = (maxX-minX < 2) ? maxX+2 : maxX;
	maxY = (maxY-minY < 2) ? maxY+2 : maxY;


	lowerLeftCorner = Vec2i(minY,minX);
	upperRightCorner = Vec2i(maxY,maxX);


}

BoundingBox::BoundingBox(const Vec2i ll, const Vec2i ur): BoundingBox()
{
	lowerLeftCorner = ll;
	upperRightCorner = ur;
}

BoundingBox::BoundingBox(const Vec2i center) : BoundingBox()
{
	lowerLeftCorner = center;
	upperRightCorner = center;
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



	Scalar line_Color(rand()%256, rand()%256, rand()%256);//Color of the line
	Point ll(lowerLeftCorner[1],lowerLeftCorner[0]); //lower left
	Point ul(upperRightCorner[1],lowerLeftCorner[0]); //lower right
	Point ur(upperRightCorner[1],upperRightCorner[0]); // upper right
	Point lr(lowerLeftCorner[1],upperRightCorner[0]); //lower right
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

	if(this->isInsideY(other) || other.isInsideY(*this) )
	{
		/*   y1
		 *		<--------> distance x
		 *   y0
		 *
		 */

		return getMinimumXDistance(other);
	}

	if(this->isInsideX(other) || other.isInsideX(*this) )
	{
		/*		distance y
		 *		|
		 *		|
		 *		|
		 *   x0   x1
		 */
		return getMinimumYDistance(other);
	}

	const Vec2i ll(lowerLeftCorner[1],lowerLeftCorner[0]); //lower left
	const Vec2i ul(upperRightCorner[1],lowerLeftCorner[0]); //lower right
	const Vec2i ur(upperRightCorner[1],upperRightCorner[0]); // upper right
	const Vec2i lr(lowerLeftCorner[1],upperRightCorner[0]); //lower right

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

	return std::min(distance_thislower_otherupper,distance_thisupper_otherlower);
}

float BoundingBox::getMinimumXDistance(const BoundingBox &other) const
{
	const int distance_thislower_otherupper = std::abs(lowerLeftCorner[1] - other.upperRightCorner[1]);
	const int distance_thisupper_otherlower = std::abs(upperRightCorner[1] - other.lowerLeftCorner[1]);

	return std::min(distance_thislower_otherupper,distance_thisupper_otherlower);
}

float BoundingBox::getMinimalDistanceToAllCorners(Vec2i point) const
{
	const Vec2i ll(lowerLeftCorner[1],lowerLeftCorner[0]); //lower left
	const Vec2i ul(upperRightCorner[1],lowerLeftCorner[0]); //lower right
	const Vec2i ur(upperRightCorner[1],upperRightCorner[0]); // upper right
	const Vec2i lr(lowerLeftCorner[1],upperRightCorner[0]); //lower right

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
