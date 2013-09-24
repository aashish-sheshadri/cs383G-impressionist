//
// blobBrush.h
//
// The header file for Scattered Points Brush. 
//

#ifndef BLOBBRUSH_H
#define BLOBBRUSH_H

#include "impBrush.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <utility>

template <typename T1, typename T2>
struct ComparePolar{
	bool operator () (T1 first, T2 second){
		return first.first<second.first;}};

template <typename T1, typename T2, typename T3>
T3 getPolar(T1 x, T2 y){
	float addPI = 0;
	if((x<0&&y>0) || (x<0&&y<0))
		addPI = M_PI;

	return (std::make_pair(addPI + std::atan((T1)y/(T1)x),std::sqrt(x*x+y*y)));}

class BlobBrush : public ImpBrush
{
public:
	BlobBrush( ImpressionistDoc* pDoc = NULL, const char* name = NULL );

	void BrushBegin( const Point source, const Point target );
	void BrushMove( const Point source, const Point target );
	void BrushEnd( const Point source, const Point target );
	char* BrushName( void );};

#endif
