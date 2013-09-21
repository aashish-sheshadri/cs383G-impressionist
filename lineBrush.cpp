//
// lineBrush.cpp
//
// The implementation of Line Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "lineBrush.h"

extern float frand();

LineBrush::LineBrush( ImpressionistDoc* pDoc, const char* name ) :
	ImpBrush(pDoc,name){
		enableAngle();
	}

void LineBrush::enableAngle(){
	angleEnabled = true;}

void LineBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;
	int angleChoice = pDoc->getAngleChoice();
	if(angleChoice == 0){
		prevPoint = target;
	} else {
		BrushMove( source, target );
	}}

void LineBrush::BrushMove( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	if ( pDoc == NULL ) {
		printf( "LineBrush::BrushMove  document is NULL\n" );
		return;
	}

	float rad;
	int angleChoice = pDoc->getAngleChoice();
	int size = pDoc->getSize();
	int magnitude = size/2;
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
		if(bValidDragPoints){
			float numer = finalDragPoint.y-initDragPoint.y;
			float den = finalDragPoint.x-initDragPoint.x;
			if(numer == 0){
				rad = 0;
			} else if(den == 0){
				rad = (M_PI * 90)/(float)180;
			} else {
				rad = std::atan(numer/den);}
			magnitude = (std::sqrt((numer*numer)+(den*den)))/2;
		} else {
			return;}
	} else {
		int angle = pDoc->getAngle();
		rad = (M_PI * (float)angle)/(float)180;}

	glLineWidth ((float)size/4);
	Point begin = target;
	Point end = target;
	float dX = magnitude*std::cos(rad);
	float dY = magnitude*std::sin(rad);

	glBegin( GL_LINES );
		SetColor( source );
		glVertex2d( target.x + dX, target.y + dY);
		glVertex2d( target.x - dX, target.y - dY);
	glEnd();
}

void LineBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}
