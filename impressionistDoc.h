// 
// impressionistDoc.h
//
// header file for Doc 
//

#ifndef ImpressionistDoc_h
#define ImpressionistDoc_h

#include "impressionist.h"
#include "bitmap.h"
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>
#include <cstdlib>


template <typename RI1,typename RI2>
void loadNeighbours(int currY, int currX, int knlWidth, int knlHeight, RI1 sourceBuffer, int srcBufferWidth, int srcBufferHeight, RI2 redIt, RI2 greenIt, RI2 blueIt){
	int top = -knlHeight/2;
	int bottom = knlHeight/2;
	int left = -knlWidth/2;
	int right = knlWidth/2;

	if(knlHeight%2 == 0){
		--bottom;}

	if(knlWidth%2 == 0){
		--right;}

	int neighbourX = -1;
	int neighbourY = -1;

	for (int y = top;y<=bottom;++y){
		for(int x = left;x<=right;++x){
			neighbourY = currY + y;
			neighbourX = currX + x;
			if(neighbourX<0||neighbourY<0){
				++redIt;
				++greenIt;
				++blueIt;
				continue;}
			*redIt = sourceBuffer[3*(neighbourY*srcBufferWidth+neighbourX) + 0];
			*greenIt = sourceBuffer[3*(neighbourY*srcBufferWidth+neighbourX) + 1];
			*blueIt = sourceBuffer[3*(neighbourY*srcBufferWidth+neighbourX) + 2];
			++redIt;
			++greenIt;
			++blueIt;}}}

class ImpressionistUI;

class ImpressionistDoc 
{
public:
	ImpressionistDoc();

	void	setUI(ImpressionistUI* ui);		// Assign the UI to use

	int		loadImage(char *iname);			// called by the UI to load image
	int		saveImage(char *iname);			// called by the UI to save image


	int     clearCanvas();                  // called by the UI to clear the drawing canvas

	void applyFilter( const unsigned char* sourceBuffer, //The filter kernel
		int srcBufferWidth, int srcBufferHeight,
		unsigned char* destBuffer,
		const double *filterKernel, 
		int knlWidth, int knlHeight, 
		double divisor, double offset );

	void	setBrushType(int type);			// called by the UI to set the brushType
	void	setAngleChoice(int type);			// called by the UI to set the brushType
	int		getAngleChoice();			// called by the UI to set the brushType
	int		getSize();						// get the UI size
	void	setSize(int size);				// set the UI size
	int		getAngle();						// get the UI angle
	void	setAngle(int angle);			// set the UI angle
	char*	getImageName();					// get the current image name

	void 	processFilterCall(int);

	void	setScale(float val);
	void	setOffset(float val);

	void	setFilterChoice(int type);

	int getAlpha();
	

// Attributes
public:
	// Dimensions of original window.
	int				m_nWidth, 
					m_nHeight;
	// Dimensions of the paint window.
	int				m_nPaintWidth, 
					m_nPaintHeight;	
	// Bitmaps for original image and painting.
	unsigned char*	m_ucBitmap;
	unsigned char*	m_ucPainting;
	unsigned char*  m_ucEdgeImage;
	unsigned char*  m_ucEdgeX;
	unsigned char*  m_ucEdgeY;

	unsigned char*	m_ucFilterResult;
	// Used by the filtering code so that we can
	// preview filters before applying.
	unsigned char*	m_ucPreviewBackup;



	// The current active brush.
	ImpBrush*			m_pCurrentBrush;	
	// Size of the brush.
	int m_nSize;
	int m_nAngle;
	int m_pAngleChoice;

	float m_nScale;							
	float m_nOffset;							

	ImpressionistUI*	m_pUI;

// Operations
public:
	// Get the color of the original picture at the specified coord
	GLubyte* GetOriginalPixel( int x, int y );   
	// Get the color of the original picture at the specified point	
	GLubyte* GetOriginalPixel( const Point p );

	float GetGradientAtPixel( int x, int y );


private:
	char			m_imageName[256];
	int m_nFilterChoice;
	void updateEdgeImage();
	float getRadian(float numer, float denom);

};

extern void MessageBox(char *message);

#endif
