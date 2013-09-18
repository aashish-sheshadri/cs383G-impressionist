//
// paintview.h
//
// The header file for painting view of the input images
//

#ifndef PAINTVIEW_H
#define PAINTVIEW_H

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <FL/glu.h>
#include <stdlib.h>
#include "originalView.h"

template <typename II,typename OI>
void copyImage(II orig, OI dup, unsigned int size){
	const unsigned char alpha = 50;
	for(unsigned int i = 0;i<size;++i){
		*(dup+4*i+0) = *(orig+3*i+0);
		*(dup+4*i+1) = *(orig+3*i+1);
		*(dup+4*i+2) = *(orig+3*i+2);
		*(dup+4*i+3) = alpha;}}

class ImpressionistDoc;

class PaintView : public Fl_Gl_Window
{
	OriginalView *m_origView;
	void drawTransparent();
public:
	PaintView(int x, int y, int w, int h, const char* l);
	void draw();
	int handle(int event);

	void refresh();
	
	void resizeWindow(int width, int height);

	void SaveCurrentContent();

	void RestoreContent();

	ImpressionistDoc *m_pDoc;

	void setOrigView(OriginalView*);
	OriginalView* getOrigView();
private:
	GLvoid* m_pPaintBitstart;
	int		m_nDrawWidth,
			m_nDrawHeight,
			m_nStartRow, 
			m_nEndRow,
			m_nStartCol, 
			m_nEndCol,
			m_nWindowWidth, 
			m_nWindowHeight;

};

#endif

