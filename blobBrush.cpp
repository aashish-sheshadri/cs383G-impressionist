// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "blobBrush.h"

extern float frand();
extern float ufRandMap(float);

BlobBrush::BlobBrush( ImpressionistDoc* pDoc, const char* name ) :
	ImpBrush(pDoc,name)
{
}

void BlobBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;
	BrushMove( source, target );
}

void BlobBrush::BrushMove( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	if ( pDoc == NULL ) {
		printf( "BlobBrush::BrushMove  document is NULL\n" );
		return;}

	int size = pDoc->getSize();
	const int scatterRad = size/2;
	const int scatterSize = size;
	glPointSize( (float)size/(float)(scatterRad*5));
	std::vector<float> offsetsX(scatterSize,scatterRad);
	std::vector<float> offsetsY(scatterSize,scatterRad);
	std::transform(offsetsX.begin(),offsetsX.end(),offsetsX.begin(),ufRandMap);
	std::transform(offsetsY.begin(),offsetsY.end(),offsetsY.begin(),ufRandMap);
	
	std::vector<std::pair<float,float> > polarCords(offsetsX.size(),std::make_pair(0.0f,0.0f));
	
	std::transform(offsetsX.begin(),offsetsX.end(),offsetsY.begin(),polarCords.begin(),(std::pair<float,float>(*)(float,float))getPolar);
	std::sort(polarCords.begin(),polarCords.end(),ComparePolar<std::pair<float, float>, std::pair<float, float> >());
	glBegin( GL_TRIANGLES );
		SetColor( source );
		std::vector<float>::iterator itX = offsetsX.begin();
		std::vector<float>::iterator itY = offsetsY.begin();
		std::vector<std::pair<float,float> >::iterator secondIt = polarCords.begin();
		std::vector<std::pair<float,float> >::iterator thirdIt = polarCords.begin();
		++thirdIt;
		for(;thirdIt!=polarCords.end();++secondIt,++thirdIt){
			glVertex2d( target.x, target.y);
			glVertex2d( target.x + (*secondIt).second*std::cos((*secondIt).first), target.y + (*secondIt).second*std::sin((*secondIt).first));
			glVertex2d( target.x + (*thirdIt).second*std::cos((*thirdIt).first), target.y + (*thirdIt).second*std::sin((*thirdIt).first));}
	glEnd();}



void BlobBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}

