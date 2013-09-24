//
// paintview.cpp
//
// The code maintaining the painting view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "paintView.h"
#include "impBrush.h"


#define LEFT_MOUSE_DOWN		1
#define LEFT_MOUSE_DRAG		2
#define LEFT_MOUSE_UP		3
#define RIGHT_MOUSE_DOWN	4
#define RIGHT_MOUSE_DRAG	5
#define RIGHT_MOUSE_UP		6


#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#define max(a, b)	( ( (a)>(b) ) ? (a) : (b) )
#endif

static int		eventToDo;
static int		isAnEvent=0;
static Point	coord;

PaintView::PaintView(int			x, 
					 int			y, 
					 int			w, 
					 int			h, 
					 const char*	l)
						: Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;
	this->bDrawFilterResult = false;
	this->bDrawTransparentBackground = false;
	this->backgroundTransparency = 150;}

PaintView::~PaintView(){}

void PaintView::draw()
{
	#ifndef MESA
	// To avoid flicker on some machines.
  //	glDrawBuffer(GL_FRONT_AND_BACK);
	#endif // !MESA

	if(!valid())
	{

		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		ortho();

		glClear( GL_COLOR_BUFFER_BIT );
	}
	static bool drawFirst = true;
	if(m_pDoc->m_ucPainting && drawFirst){
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable( GL_BLEND );
		drawFirst = false;}

	if(m_pDoc->m_ucPainting && this->bDrawTransparentBackground){
		static bool bDraw = true;
		if(bDraw){
			drawTransparent();
			bDraw = false;
		}else{
			this->bDrawTransparentBackground = false;
			bDraw = true;
			RestoreContent();}
		glFlush();
		#ifndef MESA
		// To avoid flicker on some machines.
		glDrawBuffer(GL_BACK);
		#endif // !MESA
		return;}

	if(m_pDoc->m_ucPainting && this->bDrawFilterResult){
		m_pPaintBitstart = m_pDoc->m_ucPainting;
		this->drawFilterResult();
		m_pPaintBitstart = m_pDoc->m_ucPainting;
		if(filterPreview == 1)
			SaveCurrentContent();
		else if(filterPreview == -1)
			RestoreContent();
		this->bDrawFilterResult = false;
		return;}

	Point scrollpos;// = GetScrollPosition();
	scrollpos.x = 0;
	scrollpos.y	= 0;

	m_nWindowWidth	= w();
	m_nWindowHeight	= h();

	int drawWidth, drawHeight;
	drawWidth = min( m_nWindowWidth, m_pDoc->m_nPaintWidth );
	drawHeight = min( m_nWindowHeight, m_pDoc->m_nPaintHeight );

	int startrow = m_pDoc->m_nPaintHeight - (scrollpos.y + drawHeight);
	if ( startrow < 0 ) startrow = 0;

	m_pPaintBitstart = m_pDoc->m_ucPainting + 
		3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);

	m_nDrawWidth	= drawWidth;
	m_nDrawHeight	= drawHeight;

	m_nStartRow		= startrow;
	m_nEndRow		= startrow + drawHeight;
	m_nStartCol		= scrollpos.x;
	m_nEndCol		= m_nStartCol + drawWidth;

	if ( m_pDoc->m_ucPainting && !isAnEvent) 
	{
		RestoreContent();

	}
	if ( m_pDoc->m_ucPainting && isAnEvent) 
	{
		// Clear it after processing.
		isAnEvent	= 0;	

		Point source( coord.x + m_nStartCol, m_nEndRow - coord.y );
		Point target( coord.x, m_nWindowHeight - coord.y );
		// This is the event handler
		switch (eventToDo) 
		{
		case LEFT_MOUSE_DOWN:
			m_pDoc->m_pUI->m_origView->setMarker(true, source);
			m_pDoc->m_pUI->m_origView->refresh();
			m_pDoc->m_pCurrentBrush->BrushBegin( source, target );

			break;
		case LEFT_MOUSE_DRAG:
			m_pDoc->m_pUI->m_origView->setMarker(true, source);

			m_pDoc->m_pCurrentBrush->BrushMove( source, target );

			m_pDoc->m_pUI->m_origView->refresh();
			break;
		case LEFT_MOUSE_UP:
			m_pDoc->m_pUI->m_origView->setMarker(false);
			m_pDoc->m_pUI->m_origView->refresh();
			m_pDoc->m_pCurrentBrush->BrushEnd( source, target );
			SaveCurrentContent();
			RestoreContent();
			break;
		case RIGHT_MOUSE_DOWN:
			if(m_pDoc->m_pCurrentBrush->getAngleStatus()){
				m_pDoc->m_pCurrentBrush->setInitDragPoint(target );}
			break;
		case RIGHT_MOUSE_DRAG:

			break;
		case RIGHT_MOUSE_UP:
			if(m_pDoc->m_pCurrentBrush->getAngleStatus()){
				m_pDoc->m_pCurrentBrush->setFinalDragPoint(target );}
			break;

		default:
			printf("Unknown event!!\n");		
			break;
		}
	}

	glFlush();

	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);
	#endif // !MESA
}


int PaintView::handle(int event)
{
	switch(event)
	{
	case FL_ENTER:
	    // redraw();
		break;
	case FL_PUSH:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DOWN;
		else
			eventToDo=LEFT_MOUSE_DOWN;
		isAnEvent=1;
		redraw();
		break;
	case FL_DRAG:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DRAG;
		else
			eventToDo=LEFT_MOUSE_DRAG;
		isAnEvent=1;
		redraw();
		break;
	case FL_RELEASE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_UP;
		else
			eventToDo=LEFT_MOUSE_UP;
		isAnEvent=1;
		redraw();
		break;
	case FL_MOVE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		break;
	default:
		return 0;
		break;

	}



	return 1;
}

void PaintView::refresh()
{
	redraw();
}

void PaintView::resizeWindow(int width, int height)
{
	resize(x(), y(), width, height);
}

void PaintView::SaveCurrentContent()
{
	// Tell openGL to read from the front buffer when capturing
	// out paint strokes
	glReadBuffer(GL_FRONT);

	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei( GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	
	glReadPixels( 0, 
				  m_nWindowHeight - m_nDrawHeight, 
				  m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart );
}


void PaintView::RestoreContent()
{
	glDrawBuffer(GL_BACK);

	glClear( GL_COLOR_BUFFER_BIT );

	glRasterPos2i( 0, m_nWindowHeight - m_nDrawHeight );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	glDrawPixels( m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart);

	glDrawBuffer(GL_FRONT);
}

void PaintView::drawTransparent(){
	unsigned char* alphaImage = new unsigned char[m_pDoc->m_nWidth * m_pDoc->m_nHeight *4 ];
	if ( m_pDoc->m_ucBitmap ){
		copyImage(m_pDoc->m_ucBitmap,alphaImage, m_pDoc->m_nWidth * m_pDoc->m_nHeight, this->backgroundTransparency);
		int drawWidth, drawHeight;
		GLvoid* bitstart;

		// we are not using a scrollable window, so ignore it
		Point scrollpos;	// = GetScrollPosition();
		scrollpos.x=scrollpos.y=0;

		drawWidth	= min( m_nWindowWidth, m_pDoc->m_nWidth );
		drawHeight	= min( m_nWindowHeight, m_pDoc->m_nHeight );

		int	startrow	= m_pDoc->m_nHeight - (scrollpos.y + drawHeight);
		if ( startrow < 0 ) 
			startrow = 0;


		bitstart = alphaImage + 4 * ((m_pDoc->m_nWidth * startrow) + scrollpos.x);
		// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// // glBlendFunc(GL_ZERO, GL_SRC1_ALPHA);
		// // glBlendFunc(GL_SRC1_COLOR, GL_ONE_MINUS_SRC_ALPHA);
		// glEnable( GL_BLEND );
		// just copy image to GLwindow conceptually
		glRasterPos2i( 0, m_nWindowHeight - drawHeight );
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
		glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nWidth );
		glDrawBuffer( GL_BACK );
		glDrawPixels( drawWidth, drawHeight, GL_RGBA, GL_UNSIGNED_BYTE, bitstart );
		// glDisable(GL_BLEND);
	}
	delete []alphaImage;}

void PaintView::drawFilterResult(){
	if ( m_pDoc->m_ucFilterResult ){
		int drawWidth, drawHeight;
		GLvoid* bitstart;

		// we are not using a scrollable window, so ignore it
		Point scrollpos;	// = GetScrollPosition();
		scrollpos.x=scrollpos.y=0;

		drawWidth	= min( m_nWindowWidth, m_pDoc->m_nWidth );
		drawHeight	= min( m_nWindowHeight, m_pDoc->m_nHeight );

		int	startrow	= m_pDoc->m_nHeight - (scrollpos.y + drawHeight);
		if ( startrow < 0 ) 
			startrow = 0;

		bitstart = m_pDoc->m_ucFilterResult + 3 * ((m_pDoc->m_nWidth * startrow) + scrollpos.x);
		// just copy image to GLwindow conceptually
		glRasterPos2i( 0, m_nWindowHeight - drawHeight );
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
		glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nWidth );
		glDrawBuffer( GL_BACK );
		glDrawPixels( drawWidth, drawHeight, GL_RGB, GL_UNSIGNED_BYTE, bitstart );
	}
}

void PaintView::setDrawFilterResult(int type){
	this->filterPreview = type;
	this->bDrawFilterResult = true;
	this->refresh();}

void PaintView::setDrawTransparentBackground(unsigned char transparency){
	this->backgroundTransparency = transparency;
	this->bDrawTransparentBackground = true;
	this->refresh();}

unsigned char* PaintView::getPaintView(){
	return reinterpret_cast<unsigned char*>(m_pPaintBitstart);
}