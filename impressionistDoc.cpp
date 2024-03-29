// 
// impressionistDoc.cpp
//
// It basically maintain the bitmap for answering the color query from the brush.
// It also acts as the bridge between brushes and UI (including views)
//


#include <FL/fl_ask.H>

#include <algorithm>

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include "impBrush.h"

// Include individual brush headers here.
#include "pointBrush.h"
#include "lineBrush.h"
#include "circleBrush.h"
#include "sPointsBrush.h"
#include "sCirclesBrush.h"
#include "sLinesBrush.h"
#include "blobBrush.h"

#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }

ImpressionistDoc::ImpressionistDoc() 
{
	// Set NULL image name as init. 
	m_imageName[0]	='\0';	

	m_nWidth		= -1;
	m_ucBitmap		= NULL;
	m_ucPainting	= NULL;
	m_ucPreviewBackup = NULL;
	m_nScale = 1;
	m_nOffset = 0;
	m_nFilterChoice = 0;
	m_ucEdgeImage = NULL;

	// create one instance of each brush
	ImpBrush::c_nBrushCount	= NUM_BRUSH_TYPE;
	ImpBrush::c_pBrushes	= new ImpBrush* [ImpBrush::c_nBrushCount];

	ImpBrush::c_pBrushes[BRUSH_POINTS]	= new PointBrush( this, "Points" );

	// Note: You should implement these 5 brushes.  They are set the same (PointBrush) for now
	ImpBrush::c_pBrushes[BRUSH_LINES]				
		= new LineBrush( this, "Lines" );
	ImpBrush::c_pBrushes[BRUSH_CIRCLES]				
		= new CircleBrush( this, "Circles" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_POINTS]	
		= new SPointsBrush( this, "Scattered Points" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES]		
		= new SLinesBrush( this, "Scattered Lines" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES]	
		= new SCirclesBrush( this, "Scattered Circles" );
	ImpBrush::c_pBrushes[BRUSH_BLOB]	
		= new BlobBrush( this, "Blob" );

	// make one of the brushes current
	m_pCurrentBrush	= ImpBrush::c_pBrushes[0];

}


//---------------------------------------------------------
// Set the current UI 
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI* ui) 
{
	m_pUI	= ui;
}

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char* ImpressionistDoc::getImageName() 
{
	return m_imageName;
}

//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType(int type)
{
	m_pCurrentBrush	= ImpBrush::c_pBrushes[type];
}

//---------------------------------------------------------
// Called by the UI when the user changes the angle.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setAngleChoice(int type){
	m_pAngleChoice = type;
}

int ImpressionistDoc::getAngleChoice(){
	return m_pAngleChoice;
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getSize()
{
	return m_pUI->getSize();
}

//---------------------------------------------------------
// Returns the angle of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getAngle()
{
	return m_pUI->getAngle();
}

int ImpressionistDoc::getAlpha(){
	return m_pUI->getAlpha();}
//---------------------------------------------------------
// Load the specified image
// This is called by the UI when the load image button is 
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadImage(char *iname) 
{
	// try to open the image to read
	unsigned char*	data;
	int				width, 
					height;

	if ( (data=readBMP(iname, width, height))==NULL ) 
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// reflect the fact of loading the new image
	m_nWidth		= width;
	m_nPaintWidth	= width;
	m_nHeight		= height;
	m_nPaintHeight	= height;

	// release old storage
	delete [] m_ucBitmap;
	delete [] m_ucPainting;
	delete [] m_ucPreviewBackup;

	DESTROY(m_ucEdgeImage);
	DESTROY(m_ucEdgeX);
	DESTROY(m_ucEdgeY);

	m_ucBitmap		= data;

	// allocate space for draw view
	m_ucPainting		= new unsigned char [width*height*3];
	m_ucPreviewBackup	= new unsigned char [width*height*3];
	m_ucEdgeImage = new unsigned char [width*height*3];
	m_ucEdgeX = new unsigned char [m_nWidth*m_nHeight*3];
	m_ucEdgeY = new unsigned char [m_nWidth*m_nHeight*3];

	memset(m_ucPainting, 0, width*height*3);

	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(), 
								m_pUI->m_mainWindow->y(), 
								width*2, 
								height+25);

	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);	
	m_pUI->m_origView->refresh();

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);	
	m_pUI->m_paintView->refresh();

	updateEdgeImage();
	return 1;
}

void ImpressionistDoc::updateEdgeImage(){
	double gaussian[FLT_WIDTH*FLT_HEIGHT] = {1,4,7,4,1,4,16,26,16,4,7,26,41,26,7,1,4,7,4,1,4,16,26,16,4};
	double sobelX[FLT_WIDTH*FLT_HEIGHT] = {0,0,0,0,0,0,1,0,-1,0,0,2,0,-2,0,0,1,0,-1,0,0,0,0,0,0};
	double sobelY[FLT_WIDTH*FLT_HEIGHT] = {0,0,0,0,0,0,1,2,1,0,0,0,0,0,0,0,-1,-2,-1,0,0,0,0,0,0};

	applyFilter(m_ucBitmap,m_nWidth,m_nHeight,m_ucEdgeImage,gaussian,FLT_WIDTH,FLT_HEIGHT,273,0);
	
	applyFilter(m_ucEdgeImage,m_nWidth,m_nHeight,m_ucEdgeX,sobelX,FLT_WIDTH,FLT_HEIGHT,1,128);
	applyFilter(m_ucEdgeImage,m_nWidth,m_nHeight,m_ucEdgeY,sobelY,FLT_WIDTH,FLT_HEIGHT,1,128);
	
	unsigned char* beginEdge = m_ucEdgeImage;
	unsigned char* endEdge = m_ucEdgeImage + (m_nWidth*m_nHeight*3);
	unsigned char* beginEdgeX = m_ucEdgeX;
	unsigned char* beginEdgeY = m_ucEdgeY;
	while(beginEdge!=endEdge){
		// *beginEdge = std::abs(std::sqrt(((int)*beginEdgeX - 128) * ((int) *beginEdgeX -128) + ((int)*beginEdgeY - 128) * ((int) *beginEdgeY -128)));
		*beginEdge = std::sqrt(((float)*beginEdgeX -128)* ((float)*beginEdgeX -128) + ((float)*beginEdgeY -128) * ((float)*beginEdgeY -128));
		++beginEdge;
		++beginEdgeX;
		++beginEdgeY;}}

//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is 
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveImage(char *iname) 
{

	writeBMP(iname, m_nPaintWidth, m_nPaintHeight, m_ucPainting);

	return 1;
}

//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is 
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clearCanvas() 
{

	// Release old storage
	if ( m_ucPainting ) 
	{
		delete [] m_ucPainting;

		// allocate space for draw view
		m_ucPainting	= new unsigned char [m_nPaintWidth*m_nPaintHeight*3];
		memset(m_ucPainting, 0, m_nPaintWidth*m_nPaintHeight*3);

		// refresh paint view as well	
		m_pUI->m_paintView->refresh();
	}
	
	return 0;
}

// Apply the filter specified by filter_kernel to the 
// each pixel in the source buffer and place the resulting
// pixel in the destination buffer.  

// This is called from the UI when the
// "preview" or "apply" button is pressed in the filter dialog.



/*
 *	INPUT: 
 *		sourceBuffer:		the original image buffer, 
 *		srcBufferWidth:		the width of the image buffer
 *		srcBufferHeight:	the height of the image buffer
 *							the buffer is arranged such that 
 *							origImg[3*(row*srcBufferWidth+column)+0], 
 *							origImg[3*(row*srcBufferWidth+column)+1], 
 *							origImg[3*(row*srcBufferWidth+column)+2]
 *							are R, G, B values for pixel at (column, row).
 *		destBuffer:			the image buffer to put the resulting
 *							image in.  It is always the same size
 *							as the source buffer.
 *
 *      filterKernel:		the 2D filter kernel,
 *		knlWidth:			the width of the kernel
 *		knlHeight:			the height of the kernel
 *
 *		divisor, offset:	each pixel after filtering should be
 *							divided by divisor and then added by offset
 */
void ImpressionistDoc::applyFilter( const unsigned char* sourceBuffer,
		int srcBufferWidth, int srcBufferHeight,
		unsigned char* destBuffer,
		const double *filterKernel, 
		int knlWidth, int knlHeight, 
		double divisor, double offset )
{
	for(int i=0;i<srcBufferHeight;++i){
		for(int j=0;j<srcBufferWidth;++j){
			std::vector<unsigned int> neighboursRed(knlWidth*knlHeight,0); 
			std::vector<unsigned int> neighboursGreen(knlWidth*knlHeight,0); 
			std::vector<unsigned int> neighboursBlue(knlWidth*knlHeight,0); 
			
			loadNeighbours(i,j,knlWidth,knlHeight,sourceBuffer,srcBufferWidth,srcBufferHeight,neighboursRed.begin(),
				neighboursGreen.begin(),neighboursBlue.begin());

			std::transform(neighboursRed.begin(),neighboursRed.end(),filterKernel,neighboursRed.begin(),std::multiplies<float>());
			std::transform(neighboursGreen.begin(),neighboursGreen.end(),filterKernel,neighboursGreen.begin(),std::multiplies<float>());
			std::transform(neighboursBlue.begin(),neighboursBlue.end(),filterKernel,neighboursBlue.begin(),std::multiplies<float>());
			
			
			destBuffer[3*(i*srcBufferWidth+j)+0] = (unsigned char)(((double)std::accumulate(neighboursRed.begin(),neighboursRed.end(),0)/divisor) + offset);
			destBuffer[3*(i*srcBufferWidth+j)+1] = (unsigned char)(((double)std::accumulate(neighboursGreen.begin(),neighboursGreen.end(),0)/divisor) + offset);
			destBuffer[3*(i*srcBufferWidth+j)+2] = (unsigned char)(((double)std::accumulate(neighboursBlue.begin(),neighboursBlue.end(),0)/divisor) + offset);}}
	// This needs to be implemented for image filtering to work.
}


//------------------------------------------------------------------
// Get the color of the pixel in the original image at coord x and y
//------------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( int x, int y )
{
	if ( x < 0 ) 
		x = 0;
	else if ( x >= m_nWidth ) 
		x = m_nWidth-1;

	if ( y < 0 ) 
		y = 0;
	else if ( y >= m_nHeight ) 
		y = m_nHeight-1;

	return (GLubyte*)(m_ucBitmap + 3 * (y*m_nWidth + x));
}

float ImpressionistDoc::GetGradientAtPixel( int x, int y )
{
	if ( x < 0 ) 
		x = 0;
	else if ( x >= m_nWidth ) 
		x = m_nWidth-1;

	if ( y < 0 ) 
		y = 0;
	else if ( y >= m_nHeight ) 
		y = m_nHeight-1;
	unsigned char* rY = m_ucEdgeY + 3 * (y*m_nWidth + x);
	unsigned char* gY = m_ucEdgeY + 3 * (y*m_nWidth + x) + 1;
	unsigned char* bY = m_ucEdgeY + 3 * (y*m_nWidth + x) + 2;

	unsigned char* rX = m_ucEdgeX + 3 * (y*m_nWidth + x);
	unsigned char* gX = m_ucEdgeX + 3 * (y*m_nWidth + x) + 1;
	unsigned char* bX = m_ucEdgeX + 3 * (y*m_nWidth + x) + 2;

	float gradientAtR = getRadian(((float)*rY -128),((float)*rX -128));
	float gradientAtG = getRadian(((float)*gY -128),((float)*gX -128));
	float gradientAtB = getRadian(((float)*bY -128),((float)*bX -128));


	float result  = gradientAtR;
	if(std::fabs(result)<std::fabs(gradientAtG))
		result = gradientAtG;
	if(std::fabs(result)<std::fabs(gradientAtB))
		result = gradientAtB;
	return result;}

float ImpressionistDoc::getRadian(float numer, float den){
	if(numer == 0){
		return (M_PI * 90)/(float)180;
	} else if(den == 0){
		return 0;
	} else {
		return M_PI/2.0f + std::atan(numer/den);}}

//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( const Point p )
{
	return GetOriginalPixel( p.x, p.y );
}

void ImpressionistDoc::processFilterCall(int type){
	m_ucFilterResult = new unsigned char[m_nWidth*m_nHeight*3];
	switch(this->m_nFilterChoice){
		case 0:
			this->applyFilter(m_ucBitmap,m_nWidth,m_nHeight,m_ucFilterResult,m_pUI->getFilterKernel(),FLT_WIDTH,FLT_HEIGHT,m_nScale,m_nOffset);
			break;
		case 1:
			this->applyFilter(this->m_pUI->m_paintView->getPaintView(),m_nWidth,m_nHeight,m_ucFilterResult,m_pUI->getFilterKernel(),FLT_WIDTH,FLT_HEIGHT,m_nScale,m_nOffset);
			break;
		default:
			this->applyFilter(m_ucBitmap,m_nWidth,m_nHeight,m_ucFilterResult,m_pUI->getFilterKernel(),FLT_WIDTH,FLT_HEIGHT,m_nScale,m_nOffset);}

	this->m_pUI->m_paintView->setDrawFilterResult(type);
	delete[] m_ucFilterResult;} 

void ImpressionistDoc::setScale(float val){
	this->m_nScale = val;}
void ImpressionistDoc::setOffset(float val){
	this->m_nOffset = val;}

void ImpressionistDoc::setFilterChoice(int type){
	this->m_nFilterChoice = type;
}



