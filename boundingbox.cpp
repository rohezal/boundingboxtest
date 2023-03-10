#include "boundingbox.h"

BoundingBox::BoundingBox()
{
	srand (time(NULL));
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

		minX = minX > x? x : minX;
		minY = minY > y? y : minY;

		minX = minX < x? x : minX;
		minY = minY < y? y : minY;
	}

	lowerLeftCorner = Vec2i(minY,minX);
	upperRightCorner = Vec2i(maxY,maxX);


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
	Point lr(lowerLeftCorner[1],lowerLeftCorner[0]); //lower right
	int thickness = 1;

	//draw the box with 4 lines
	line(image, ll, lr, line_Color, thickness);//using line() function to draw the line//
	line(image, ul, ur, line_Color, thickness);//using line() function to draw the line//
	line(image, ll, ul, line_Color, thickness);//using line() function to draw the line//
	line(image, lr, ur, line_Color, thickness);//using line() function to draw the line//

}
