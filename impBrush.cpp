//
// impBrush.cpp
//
// The implementation of virtual brush. All the other brushes inherit from it.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "impBrush.h"

// Static class member initializations
int			ImpBrush::c_nBrushCount	= 0;
ImpBrush**	ImpBrush::c_pBrushes	= NULL;

ImpBrush::ImpBrush(ImpressionistDoc*	pDoc, 
				   const char*			name) :
					m_pDoc(pDoc), 
					m_pBrushName(name)
{
	angleEnabled = false;
	bValidDragPoints = false;
}

//---------------------------------------------------
// Return m_pDoc, which connects the UI and brushes
//---------------------------------------------------
ImpressionistDoc* ImpBrush::GetDocument(void)
{
	return m_pDoc;
}

//---------------------------------------------------
// Return the name of the current brush
//---------------------------------------------------
const char* ImpBrush::BrushName(void)
{
	return m_pBrushName;
}

//----------------------------------------------------
// Set the color to paint with to the color at source,
// which is the coord at the original window to sample 
// the color from
//----------------------------------------------------
void ImpBrush::SetColor (const Point source)
{
	ImpressionistDoc* pDoc = GetDocument();


	GLubyte color[4];
	// glBlendFunc(GL_ZERO, GL_SRC1_ALPHA);
	memcpy ( color, pDoc->GetOriginalPixel( source ), 3 );
 	color[3] = pDoc->getAlpha();
	glColor4ubv( color );}

bool ImpBrush::getAngleStatus(){
	return angleEnabled;}

void ImpBrush::setInitDragPoint(Point point){
	this->initDragPoint = point;}

void ImpBrush::setFinalDragPoint(Point point){
	this->finalDragPoint = point;
	bValidDragPoints = true;}
