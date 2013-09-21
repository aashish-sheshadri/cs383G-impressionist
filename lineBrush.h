//
// lineBrush.h
//
// The header file for Line Brush. 
//

#ifndef LINEBRUSH_H
#define LINEBRUSH_H

#include "impBrush.h"
#include <cmath>
class LineBrush : public ImpBrush
{
	Point prevPoint;
public:
	LineBrush( ImpressionistDoc* pDoc = NULL, const char* name = NULL );
	void enableAngle();
	void BrushBegin( const Point source, const Point target );
	void BrushMove( const Point source, const Point target );
	void BrushEnd( const Point source, const Point target );
	char* BrushName( void );
};

#endif