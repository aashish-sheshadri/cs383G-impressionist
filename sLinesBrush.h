//
// sLinesBrush.h
//
// The header file for Line Brush. 
//

#ifndef SLINESBRUSH_H
#define SLINESBRUSH_H

#include "impBrush.h"
#include <vector>
#include <algorithm>

class SLinesBrush : public ImpBrush
{
	Point prevPoint;
public:
	SLinesBrush( ImpressionistDoc* pDoc = NULL, const char* name = NULL );
	void enableAngle();
	void BrushBegin( const Point source, const Point target );
	void BrushMove( const Point source, const Point target );
	void BrushEnd( const Point source, const Point target );
	char* BrushName( void );
};

#endif