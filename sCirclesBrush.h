//
// sCirclesBrush.h
//
// The header file for Circle Brush. 
//

#ifndef SCIRCLESBRUSH_H
#define SCIRCLESBRUSH_H

#include "impBrush.h"
#include <vector>
#include <algorithm>

class SCirclesBrush : public ImpBrush
{
public:
	SCirclesBrush( ImpressionistDoc* pDoc = NULL, const char* name = NULL );

	void BrushBegin( const Point source, const Point target );
	void BrushMove( const Point source, const Point target );
	void BrushEnd( const Point source, const Point target );
	char* BrushName( void );
};

#endif