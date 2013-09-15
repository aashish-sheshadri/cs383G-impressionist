//
// lineBrush.cpp
//
// The implementation of Line Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "lineBrush.h"
#include <cmath>
extern float frand();

LineBrush::LineBrush( ImpressionistDoc* pDoc, const char* name ) :
	ImpBrush(pDoc,name)
{
}

void LineBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;
	int size = pDoc->getSize();
	glLineWidth ((float)size);
	BrushMove( source, target );
}

void LineBrush::BrushMove( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	if ( pDoc == NULL ) {
		printf( "LineBrush::BrushMove  document is NULL\n" );
		return;
	}
	
	int angle = pDoc->getAngle();
	float rad = (M_PI * (float)angle)/(float)180;
	Point begin = target;
	Point end = target;
	int magnitude = 20;
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
