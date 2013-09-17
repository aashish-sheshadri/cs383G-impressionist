//
// SLinesBrush.cpp
//
// The implementation of Line Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "sLinesBrush.h"
#include <cmath>
extern float frand();
extern float ufRandMap(float);
SLinesBrush::SLinesBrush( ImpressionistDoc* pDoc, const char* name ) :
	ImpBrush(pDoc,name)
{
}

void SLinesBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;
	int angleChoice = pDoc->getAngleChoice();
	if(angleChoice == 0){
		prevPoint = target;
	} else if (angleChoice == 1){

	} else {
		BrushMove( source, target );
	}}

void SLinesBrush::BrushMove( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	if ( pDoc == NULL ) {
		printf( "SLinesBrush::BrushMove  document is NULL\n" );
		return;
	}
	
	float rad;
	int angleChoice = pDoc->getAngleChoice();
	if(angleChoice == 0){
		
		float numer = target.y-prevPoint.y;
		float den = target.x-prevPoint.x;
		if(numer == 0){
			rad = 0;
		} else if(den == 0){
			rad = (M_PI * 90)/(float)180;
		} else {
			rad = std::atan(numer/den);}
		prevPoint = target;
	} else if (angleChoice == 1){

	} else {
		int angle = pDoc->getAngle();
		rad = (M_PI * (float)angle)/(float)180;}

	int size = pDoc->getSize();
	Point begin = target;
	Point end = target;
	int magnitude = size/2;
	float dX = magnitude*std::cos(rad);
	float dY = magnitude*std::sin(rad);


	const int scatterRad = size/2;
	const int scatterSize = size;
	glLineWidth ((float)size/(float)scatterSize);
	std::vector<float> offsetsX(scatterSize,scatterRad);
	std::vector<float> offsetsY(scatterSize,scatterRad);
	std::transform(offsetsX.begin(),offsetsX.end(),offsetsX.begin(),ufRandMap);
	std::transform(offsetsY.begin(),offsetsY.end(),offsetsY.begin(),ufRandMap);

	glBegin( GL_LINES );
		SetColor( source );

		std::vector<float>::iterator itX = offsetsX.begin();
		std::vector<float>::iterator itY = offsetsY.begin();
		glVertex2d( target.x + dX, target.y + dY);
		glVertex2d( target.x - dX, target.y - dY);
		for(int i = 0;i<scatterSize;++i){
			glVertex2d( target.x + *itX + dX, target.y + *itY + dY);
			glVertex2d( target.x + *itX - dX, target.y + *itY - dY);
			++itX;
			++itY;}
	glEnd();
}

void SLinesBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}
