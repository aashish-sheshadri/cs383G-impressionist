//
// sPointsBrush.h
//
// The header file for Scattered Points Brush. 
//

#ifndef SPOINTSBRUSH_H
#define SPOINTSBRUSH_H

#include "impBrush.h"
#include <vector>
#include <algorithm>



class SPointsBrush : public ImpBrush
{
public:
	SPointsBrush( ImpressionistDoc* pDoc = NULL, const char* name = NULL );

	void BrushBegin( const Point source, const Point target );
	void BrushMove( const Point source, const Point target );
	void BrushEnd( const Point source, const Point target );
	char* BrushName( void );};

#endif
