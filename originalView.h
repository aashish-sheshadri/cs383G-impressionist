//
// originalview.h
//
// The header file for original view of the input images
//

#ifndef ORIGINALVIEW_H
#define ORIGINALVIEW_H

#include <FL/Fl.H>

#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <FL/glu.h>

#include <stdlib.h>

#include "impBrush.h"

class ImpressionistDoc;

class OriginalView : public Fl_Gl_Window
{
public:
	OriginalView(int x, int y, int w, int h, const char *l);
	
	void draw();
	void refresh();
	void drawMarker();

	void resizeWindow(int width, int height);

	ImpressionistDoc*	m_pDoc;
	void setMarker(bool bMarker, Point source = Point());
private:
	int	m_nWindowWidth, 
		m_nWindowHeight;
	bool bMarker;
	Point marker;

};

#endif

