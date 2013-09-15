//
// SCirclesBrush.cpp
//
// The implementation of Circle Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "sCirclesBrush.h"

extern float frand();
extern float ufRandMap(float);

SCirclesBrush::SCirclesBrush( ImpressionistDoc* pDoc, const char* name ) :
	ImpBrush(pDoc,name)
{
}

void SCirclesBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	int size = pDoc->getSize();

	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glPointSize( (float)size );

	BrushMove( source, target );
}

void SCirclesBrush::BrushMove( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	if ( pDoc == NULL ) {
		printf( "SCirclesBrush::BrushMove  document is NULL\n" );
		return;
	}
	const int scatterRad = 10;
	const int scatterSize = 10;
	std::vector<float> offsetsX(scatterSize,scatterRad);
	std::vector<float> offsetsY(scatterSize,scatterRad);
	std::transform(offsetsX.begin(),offsetsX.end(),offsetsX.begin(),ufRandMap);
	std::transform(offsetsY.begin(),offsetsY.end(),offsetsY.begin(),ufRandMap);
	glBegin( GL_POINTS );
		SetColor( source );
		
		std::vector<float>::iterator itX = offsetsX.begin();
		std::vector<float>::iterator itY = offsetsY.begin();
		for(int i = 0;i<scatterSize;++i){
			glVertex2d( target.x + *itX, target.y + *itY );
			++itX;
			++itY;}
	glEnd();
}

void SCirclesBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}

