//
// impressionistUI.cpp
//
// The user interface part for the program.
//


#include <FL/fl_ask.H>

#include <algorithm>

#include <math.h>

#include "impressionistUI.h"
#include "impressionistDoc.h"

/*
//------------------------------ Widget Examples -------------------------------------------------
Here is some example code for all of the widgets that you may need to add to the 
project.  You can copy and paste these into your code and then change them to 
make them look how you want.  Descriptions for all of the widgets here can be found 
in links on the fltk help session page.

//---------Window/Dialog and Menubar-----------------------------------
	
	//----To install a window--------------------------
	Fl_Window* myWindow = new Fl_Window(600, 300, "MyWindow");
		myWindow->user_data((void*)(this));	// record self to be used by static callback functions
		
		// install menu bar
		myMenubar = new Fl_Menu_Bar(0, 0, 600, 25);
		Fl_Menu_Item ImpressionistUI::myMenuItems[] = {
			{ "&File",		0, 0, 0, FL_SUBMENU },
				{ "&Load...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load },
				{ "&Save...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save }.
				{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
				{ 0 },
			{ "&Edit",		0, 0, 0, FL_SUBMENU },
				{ "&Copy",FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_copy, (void *)COPY },
				{ "&Cut",	FL_ALT + 'x', (Fl_Callback *)ImpressionistUI::cb_cut, (void *)CUT },
				{ "&Paste",	FL_ALT + 'v', (Fl_Callback *)ImpressionistUI::cb_paste, (void *)PASTE },
				{ 0 },
			{ "&Help",		0, 0, 0, FL_SUBMENU },
				{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
				{ 0 },
			{ 0 }
		};
		myMenubar->menu(myMenuItems);
    myWindow->end();

	//----The window callback--------------------------
	// One of the callbacks
	void ImpressionistUI::cb_load(Fl_Menu_* o, void* v) 
	{	
		ImpressionistDoc *pDoc=whoami(o)->getDocument();

		char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
		if (newfile != NULL) {
			pDoc->loadImage(newfile);
		}
	}


//------------Slider---------------------------------------

	//----To install a slider--------------------------
	Fl_Value_Slider * mySlider = new Fl_Value_Slider(10, 80, 300, 20, "My Value");
	mySlider->user_data((void*)(this));	// record self to be used by static callback functions
	mySlider->type(FL_HOR_NICE_SLIDER);
    mySlider->labelfont(FL_COURIER);
    mySlider->labelsize(12);
	mySlider->minimum(1);
	mySlider->maximum(40);
	mySlider->step(1);
	mySlider->value(m_nMyValue);
	mySlider->align(FL_ALIGN_RIGHT);
	mySlider->callback(cb_MyValueSlides);

	//----The slider callback--------------------------
	void ImpressionistUI::cb_MyValueSlides(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyValue=int( ((Fl_Slider *)o)->value() ) ;
	}
	

//------------Choice---------------------------------------
	
	//----To install a choice--------------------------
	Fl_Choice * myChoice = new Fl_Choice(50,10,150,25,"&myChoiceLabel");
	myChoice->user_data((void*)(this));	 // record self to be used by static callback functions
	Fl_Menu_Item ImpressionistUI::myChoiceMenu[3+1] = {
	  {"one",FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)ONE},
	  {"two",FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)TWO},
	  {"three",FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)THREE},
	  {0}
	};
	myChoice->menu(myChoiceMenu);
	myChoice->callback(cb_myChoice);
	
	//-----The choice callback-------------------------
	void ImpressionistUI::cb_myChoice(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
		ImpressionistDoc* pDoc=pUI->getDocument();

		int type=(int)v;

		pDoc->setMyType(type);
	}


//------------Button---------------------------------------

	//---To install a button---------------------------
	Fl_Button* myButton = new Fl_Button(330,220,50,20,"&myButtonLabel");
	myButton->user_data((void*)(this));   // record self to be used by static callback functions
	myButton->callback(cb_myButton);

	//---The button callback---------------------------
	void ImpressionistUI::cb_myButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI*)(o->user_data()));
		ImpressionistDoc* pDoc = pUI->getDocument();
		pDoc->startPainting();
	}


//---------Light Button------------------------------------
	
	//---To install a light button---------------------
	Fl_Light_Button* myLightButton = new Fl_Light_Button(240,10,150,25,"&myLightButtonLabel");
	myLightButton->user_data((void*)(this));   // record self to be used by static callback functions
	myLightButton->callback(cb_myLightButton);

	//---The light button callback---------------------
	void ImpressionistUI::cb_myLightButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI *pUI=((ImpressionistUI*)(o->user_data()));

		if (pUI->myBool==TRUE) pUI->myBool=FALSE;
		else pUI->myBool=TRUE;
	}

//----------Int Input--------------------------------------

    //---To install an int input-----------------------
	Fl_Int_Input* myInput = new Fl_Int_Input(200, 50, 5, 5, "&My Input");
	myInput->user_data((void*)(this));   // record self to be used by static callback functions
	myInput->callback(cb_myInput);

	//---The int input callback------------------------
	void ImpressionistUI::cb_myInput(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyInputValue=int( ((Fl_Int_Input *)o)->value() );
	}

//------------------------------------------------------------------------------------------------
*/

//------------------------------------- Help Functions --------------------------------------------

//------------------------------------------------------------
// This returns the UI, given the menu item.  It provides a
// link from the menu items to the UI
//------------------------------------------------------------
ImpressionistUI* ImpressionistUI::whoami(Fl_Menu_* o)	
{
	return ( (ImpressionistUI*)(o->parent()->user_data()) );
}


//--------------------------------- Callback Functions --------------------------------------------

//------------------------------------------------------------------
// Brings up a file chooser and then loads the chosen image
// This is called by the UI when the load image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_load_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
	if (newfile != NULL) {
		pDoc->loadImage(newfile);
	}
}


//------------------------------------------------------------------
// Brings up a file chooser and then saves the painted image
// This is called by the UI when the save image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_save_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Save File?", "*.bmp", "save.bmp" );
	if (newfile != NULL) {
		pDoc->saveImage(newfile);
	}
}

//-------------------------------------------------------------
// Brings up the paint dialog
// This is called by the UI when the brushes menu item
// is chosen
//-------------------------------------------------------------
void ImpressionistUI::cb_brushes(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_brushDialog->show();
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc=whoami(o)->getDocument();

	pDoc->clearCanvas();
}

//------------------------------------------------------------
// Causes the Impressionist program to exit
// Called by the UI when the quit menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_exit(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_mainWindow->hide();
	whoami(o)->m_brushDialog->hide();
	whoami(o)->m_filterDialog->hide();
}

void ImpressionistUI::cb_filter(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_filterDialog->show();
}



//-----------------------------------------------------------
// Brings up an about dialog box
// Called by the UI when the about menu item is chosen
//-----------------------------------------------------------
void ImpressionistUI::cb_about(Fl_Menu_* o, void* v) 
{
	fl_message("Impressionist FLTK version 1.1.9 for CS 384G, Fall 2008");
}

//------- UI should keep track of the current for all the controls for answering the query from Doc ---------
//-------------------------------------------------------------
// Sets the type of brush to use to the one chosen in the brush 
// choice.  
// Called by the UI when a brush is chosen in the brush choice
//-------------------------------------------------------------
void ImpressionistUI::cb_brushChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc* pDoc=pUI->getDocument();

	//	int type=(int)v;
	long long tmp = reinterpret_cast<long long>(v);
	int type = static_cast<int>(tmp);

	pDoc->setBrushType(type);
}

//-------------------------------------------------------------
// Sets the type of angle inference
// Called by the UI when a angle set method is chosen
//-------------------------------------------------------------
void ImpressionistUI::cb_angleChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc* pDoc=pUI->getDocument();

	//	int type=(int)v;
	long long tmp = reinterpret_cast<long long>(v);
	int type = static_cast<int>(tmp);
	
	pDoc->setAngleChoice(type);
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas button is pushed
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas_button(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	pDoc->clearCanvas();
}


//-----------------------------------------------------------
// Updates the brush size to use from the value of the size
// slider
// Called by the UI when the size slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_sizeSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nSize=int( ((Fl_Slider *)o)->value() ) ;
}

//-----------------------------------------------------------
// Updates the brush angle to use from the value of the angle
// slider
// Called by the UI when the angle slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_angleSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nAngle=int( ((Fl_Slider *)o)->value() ) ;
}

void ImpressionistUI::cb_alphaSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nAlpha=int( ((Fl_Slider *)o)->value() ) ;
}

void ImpressionistUI::cb_backgroundAlphaSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nBackgroundAlpha=int( ((Fl_Slider *)o)->value() ) ;
}

//---------------------------------- per instance functions --------------------------------------

//------------------------------------------------
// Return the ImpressionistDoc used
//------------------------------------------------
ImpressionistDoc* ImpressionistUI::getDocument()
{
	return m_pDoc;
}

//------------------------------------------------
// Draw the main window
//------------------------------------------------
void ImpressionistUI::show() {
	m_mainWindow->show();
	m_paintView->show();
	m_origView->show();
}

//------------------------------------------------
// Change the paint and original window sizes to 
// w by h
//------------------------------------------------
void ImpressionistUI::resize_windows(int w, int h) {
	m_paintView->size(w,h);
	m_origView->size(w,h);
}

//------------------------------------------------ 
// Set the ImpressionistDoc used by the UI to 
// communicate with the brushes 
//------------------------------------------------
void ImpressionistUI::setDocument(ImpressionistDoc* doc)
{
	m_pDoc = doc;

	m_origView->m_pDoc = doc;
	m_paintView->m_pDoc = doc;
}

//------------------------------------------------
// Return the brush size
//------------------------------------------------
int ImpressionistUI::getSize()
{
	return m_nSize;
}

//-------------------------------------------------
// Set the brush size
//-------------------------------------------------
void ImpressionistUI::setSize( int size )
{
	m_nSize=size;

	if (size<=40) 
		m_BrushSizeSlider->value(m_nSize);
}

int ImpressionistUI::getAlpha()
{
		return m_nAlpha;
}

//------------------------------------------------
// Return the brush angle
//------------------------------------------------
int ImpressionistUI::getAngle()
{
	return m_nAngle;
}

//-------------------------------------------------
// Set the brush size
//-------------------------------------------------
void ImpressionistUI::setAngle( int angle )
{
	m_nAngle=angle;

	if (angle<=180) 
		m_BrushSizeSlider->value(angle);
}



// Main menu definition
Fl_Menu_Item ImpressionistUI::menuitems[] = {
	{ "&File", 0, 0, 0, FL_SUBMENU },
		{ "&Load Image...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load_image },
		{ "&Save Image...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save_image },
		{ "&Brushes...",	FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_brushes },
		{ "&Filter...",	FL_ALT + 'f', (Fl_Callback *)ImpressionistUI::cb_filter }, 
		{ "&Clear Canvas", FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_clear_canvas, 0, FL_MENU_DIVIDER },
		
		{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
		{ 0 },

	{ "&Help",		0, 0, 0, FL_SUBMENU },
		{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
		{ 0 },

	{ 0 }
};

// Brush choice menu definition
Fl_Menu_Item ImpressionistUI::brushTypeMenu[NUM_BRUSH_TYPE+1] = {
  {"Points",			FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_POINTS},
  {"Lines",				FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_LINES},
  {"Circles",			FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_CIRCLES},
  {"Scattered Points",	FL_ALT+'q', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_POINTS},
  {"Scattered Lines",	FL_ALT+'m', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_LINES},
  {"Scattered Circles",	FL_ALT+'d', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_CIRCLES},
  {"Blob",	FL_ALT+'b', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_BLOB},
  {0}
};

// Angle choice menu definition
Fl_Menu_Item ImpressionistUI::angleChoiceMenu[4+1] = {
  {"Follow",FL_ALT+'f', (Fl_Callback *)ImpressionistUI::cb_angleChoice, (void *)0},
  {"Drag",FL_ALT+'d', (Fl_Callback *)ImpressionistUI::cb_angleChoice, (void *)1},
  {"Slider",FL_ALT+'s', (Fl_Callback *)ImpressionistUI::cb_angleChoice, (void *)2},
  {"Follow Gradient",FL_ALT+'f', (Fl_Callback *)ImpressionistUI::cb_angleChoice, (void *)3},
  {0}
};

// Filter choice menu definition
Fl_Menu_Item ImpressionistUI::filterChoiceMenu[2+1] = {
  {"Original",FL_ALT+'o', (Fl_Callback *)ImpressionistUI::cb_filterChoice, (void *)0},
  {"Paint",FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_filterChoice, (void *)1},
  {0}
};


//----------------------------------------------------
// Constructor.  Creates all of the widgets.
// Add new widgets here
//----------------------------------------------------
ImpressionistUI::ImpressionistUI() {
	// Create the main window
	m_mainWindow = new Fl_Window(600, 300, "Impressionist");
		m_mainWindow->user_data((void*)(this));	// record self to be used by static callback functions
		// install menu bar
		m_menubar = new Fl_Menu_Bar(0, 0, 600, 25);
		m_menubar->menu(menuitems);

		// Create a group that will hold two sub windows inside the main
		// window
		Fl_Group* group = new Fl_Group(0, 25, 600, 275);

			// install paint view window
			m_paintView = new PaintView(300, 25, 300, 275, "This is the paint view");//0jon
			m_paintView->box(FL_DOWN_FRAME);

			// install original view window
			m_origView = new OriginalView(0, 25, 300, 275, "This is the orig view");//300jon
			m_origView->box(FL_DOWN_FRAME);
			// m_origView->deactivate();

		group->end();
		Fl_Group::current()->resizable(group);
    m_mainWindow->end();

	// init values

	m_nSize = 10;
	m_nAngle = 45;
	m_nAlpha = 255;
	m_nBackgroundAlpha = 100;

	// brush dialog definition
	m_brushDialog = new Fl_Window(400, 325, "Brush Dialog");
		// Add a brush type choice to the dialog
		m_BrushTypeChoice = new Fl_Choice(50,10,150,25,"&Brush");
		m_BrushTypeChoice->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushTypeChoice->menu(brushTypeMenu);
		m_BrushTypeChoice->callback(cb_brushChoice);

		m_ClearCanvasButton = new Fl_Button(240,10,150,25,"&Clear Canvas");
		m_ClearCanvasButton->user_data((void*)(this));
		m_ClearCanvasButton->callback(cb_clear_canvas_button);

		m_transparentBackgroundButton = new Fl_Button(130,280,150,25,"&Show Background");
		m_transparentBackgroundButton->user_data((void*)(this));
		m_transparentBackgroundButton->callback(cb_transparentBackground);

		m_edgeImageButton = new Fl_Button(130,180,150,25,"&Toggle Edge Image");
		m_edgeImageButton->user_data((void*)(this));
		m_edgeImageButton->callback(cb_edgeImage);

		//Add angle choice menu
		Fl_Choice * myChoice = new Fl_Choice(95,60,150,25,"&Angle Choice");
		myChoice->user_data((void*)(this));	 // record self to be used by static callback functions
		myChoice->menu(angleChoiceMenu);
		myChoice->callback(cb_angleChoice);

		// Add brush size slider to the dialog 
		m_BrushSizeSlider = new Fl_Value_Slider(10, 100, 300, 20, "Size");
		m_BrushSizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushSizeSlider->type(FL_HOR_NICE_SLIDER);
        m_BrushSizeSlider->labelfont(FL_COURIER);
        m_BrushSizeSlider->labelsize(12);
		m_BrushSizeSlider->minimum(1);
		m_BrushSizeSlider->maximum(40);
		m_BrushSizeSlider->step(1);
		m_BrushSizeSlider->value(m_nSize);
		m_BrushSizeSlider->align(FL_ALIGN_RIGHT);
		m_BrushSizeSlider->callback(cb_sizeSlides);

		// Add brush angle slider to the dialog 
		m_BrushSizeSlider = new Fl_Value_Slider(10, 120, 300, 20, "Angle");
		m_BrushSizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushSizeSlider->type(FL_HOR_NICE_SLIDER);
        m_BrushSizeSlider->labelfont(FL_COURIER);
        m_BrushSizeSlider->labelsize(12);
		m_BrushSizeSlider->minimum(0);
		m_BrushSizeSlider->maximum(180);
		m_BrushSizeSlider->step(1);
		m_BrushSizeSlider->value(m_nAngle);
		m_BrushSizeSlider->align(FL_ALIGN_RIGHT);
		m_BrushSizeSlider->callback(cb_angleSlides);

		// Add brush angle slider to the dialog 
		m_BrushSizeSlider = new Fl_Value_Slider(10, 140, 300, 20, "Opacity");
		m_BrushSizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushSizeSlider->type(FL_HOR_NICE_SLIDER);
        m_BrushSizeSlider->labelfont(FL_COURIER);
        m_BrushSizeSlider->labelsize(12);
		m_BrushSizeSlider->minimum(0);
		m_BrushSizeSlider->maximum(255);
		m_BrushSizeSlider->step(1);
		m_BrushSizeSlider->value(m_nAlpha);
		m_BrushSizeSlider->align(FL_ALIGN_RIGHT);
		m_BrushSizeSlider->callback(cb_alphaSlides);

		// Add brush angle slider to the dialog 
		m_BrushSizeSlider = new Fl_Value_Slider(10, 250, 300, 20, "Background\nOpacity");
		m_BrushSizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushSizeSlider->type(FL_HOR_NICE_SLIDER);
        m_BrushSizeSlider->labelfont(FL_COURIER);
        m_BrushSizeSlider->labelsize(12);
		m_BrushSizeSlider->minimum(0);
		m_BrushSizeSlider->maximum(255);
		m_BrushSizeSlider->step(1);
		m_BrushSizeSlider->value(m_nBackgroundAlpha);
		m_BrushSizeSlider->align(FL_ALIGN_RIGHT);
		m_BrushSizeSlider->callback(cb_backgroundAlphaSlides);

    m_brushDialog->end();	

    // filter dialog definition 
    std::fill(fltKernel,fltKernel+(FLT_WIDTH*FLT_HEIGHT)-1,0.0f);
    // m_nScale = 1.0f;
    // m_nOffset = 0.0f;
    m_filterDialog = new Fl_Window(400, 200, "Filter Dialog");

    	m_BrushTypeChoice = new Fl_Choice(150,10,150,25,"&Select Source");
		m_BrushTypeChoice->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushTypeChoice->menu(filterChoiceMenu);
		m_BrushTypeChoice->callback(cb_filterChoice);

    	m_ApplyFilterButton = new Fl_Button(30,170,100,25,"&Apply Filter");
		m_ApplyFilterButton->user_data((void*)(this));
		m_ApplyFilterButton->callback(cb_apply_filter_button);

		m_PreviewFilterButton = new Fl_Button(150,170,100,25,"&Preview Filter");
		m_PreviewFilterButton->user_data((void*)(this));
		m_PreviewFilterButton->callback(cb_preview_filter_button);

		m_CancelFilterButton = new Fl_Button(270,170,100,25,"&Cancel");
		m_CancelFilterButton->user_data((void*)(this));
		m_CancelFilterButton->callback(cb_cancel_filter_button);

		Fl_Float_Input* scaleInput = new Fl_Float_Input(50, 80, 40, 20,"&Scale");
		scaleInput->user_data((void*)(this)); 
		scaleInput->callback(cb_scaleInput);
		
		Fl_Float_Input* offsetInput = new Fl_Float_Input(50, 100, 40, 20,"&Offset");
		offsetInput->user_data((void*)(this)); 
		offsetInput->callback(cb_offsetInput);

    	Fl_Float_Input* filterInput_00 = new Fl_Float_Input(100, 50, 50, 20);
		filterInput_00->user_data((void*)(this)); 
		filterInput_00->callback(cb_filterInput_00);

		Fl_Float_Input* filterInput_01 = new Fl_Float_Input(150, 50, 50, 20);
		filterInput_01->user_data((void*)(this)); 
		filterInput_01->callback(cb_filterInput_01);

		Fl_Float_Input* filterInput_02 = new Fl_Float_Input(200, 50, 50, 20);
		filterInput_02->user_data((void*)(this)); 
		filterInput_02->callback(cb_filterInput_02);

		Fl_Float_Input* filterInput_03 = new Fl_Float_Input(250, 50, 50, 20);
		filterInput_03->user_data((void*)(this)); 
		filterInput_03->callback(cb_filterInput_03);

		Fl_Float_Input* filterInput_04 = new Fl_Float_Input(300, 50, 50, 20);
		filterInput_04->user_data((void*)(this)); 
		filterInput_04->callback(cb_filterInput_04);

		Fl_Float_Input* filterInput_10 = new Fl_Float_Input(100, 70, 50, 20);
		filterInput_10->user_data((void*)(this)); 
		filterInput_10->callback(cb_filterInput_10);

		Fl_Float_Input* filterInput_11 = new Fl_Float_Input(150, 70, 50, 20);
		filterInput_11->user_data((void*)(this)); 
		filterInput_11->callback(cb_filterInput_11);

		Fl_Float_Input* filterInput_12 = new Fl_Float_Input(200, 70, 50, 20);
		filterInput_12->user_data((void*)(this)); 
		filterInput_12->callback(cb_filterInput_12);

		Fl_Float_Input* filterInput_13 = new Fl_Float_Input(250, 70, 50, 20);
		filterInput_13->user_data((void*)(this)); 
		filterInput_13->callback(cb_filterInput_13);

		Fl_Float_Input* filterInput_14 = new Fl_Float_Input(300, 70, 50, 20);
		filterInput_14->user_data((void*)(this)); 
		filterInput_14->callback(cb_filterInput_14);

		Fl_Float_Input* filterInput_20 = new Fl_Float_Input(100, 90, 50, 20);
		filterInput_20->user_data((void*)(this)); 
		filterInput_20->callback(cb_filterInput_20);

		Fl_Float_Input* filterInput_21 = new Fl_Float_Input(150, 90, 50, 20);
		filterInput_21->user_data((void*)(this)); 
		filterInput_21->callback(cb_filterInput_21);

		Fl_Float_Input* filterInput_22 = new Fl_Float_Input(200, 90, 50, 20);
		filterInput_22->user_data((void*)(this)); 
		filterInput_22->callback(cb_filterInput_22);

		Fl_Float_Input* filterInput_23 = new Fl_Float_Input(250, 90, 50, 20);
		filterInput_23->user_data((void*)(this)); 
		filterInput_23->callback(cb_filterInput_23);

		Fl_Float_Input* filterInput_24 = new Fl_Float_Input(300, 90, 50, 20);
		filterInput_24->user_data((void*)(this)); 
		filterInput_24->callback(cb_filterInput_24);

		Fl_Float_Input* filterInput_30 = new Fl_Float_Input(100, 110, 50, 20);
		filterInput_30->user_data((void*)(this)); 
		filterInput_30->callback(cb_filterInput_30);

		Fl_Float_Input* filterInput_31 = new Fl_Float_Input(150, 110, 50, 20);
		filterInput_31->user_data((void*)(this)); 
		filterInput_31->callback(cb_filterInput_31);

		Fl_Float_Input* filterInput_32 = new Fl_Float_Input(200, 110, 50, 20);
		filterInput_32->user_data((void*)(this)); 
		filterInput_32->callback(cb_filterInput_32);

		Fl_Float_Input* filterInput_33 = new Fl_Float_Input(250, 110, 50, 20);
		filterInput_33->user_data((void*)(this)); 
		filterInput_33->callback(cb_filterInput_33);

		Fl_Float_Input* filterInput_34 = new Fl_Float_Input(300, 110, 50, 20);
		filterInput_34->user_data((void*)(this)); 
		filterInput_34->callback(cb_filterInput_34);

		Fl_Float_Input* filterInput_40 = new Fl_Float_Input(100, 130, 50, 20);
		filterInput_40->user_data((void*)(this)); 
		filterInput_40->callback(cb_filterInput_40);

		Fl_Float_Input* filterInput_41 = new Fl_Float_Input(150, 130, 50, 20);
		filterInput_41->user_data((void*)(this)); 
		filterInput_41->callback(cb_filterInput_41);

		Fl_Float_Input* filterInput_42 = new Fl_Float_Input(200, 130, 50, 20);
		filterInput_42->user_data((void*)(this)); 
		filterInput_42->callback(cb_filterInput_42);

		Fl_Float_Input* filterInput_43 = new Fl_Float_Input(250, 130, 50, 20);
		filterInput_43->user_data((void*)(this)); 
		filterInput_43->callback(cb_filterInput_43);

		Fl_Float_Input* filterInput_44 = new Fl_Float_Input(300, 130, 50, 20);
		filterInput_44->user_data((void*)(this)); 
		filterInput_44->callback(cb_filterInput_44);


    m_filterDialog->end();	


}

void ImpressionistUI::cb_transparentBackground(Fl_Widget* o, void* v){
	((ImpressionistUI *)(o->user_data()))->m_paintView->setDrawTransparentBackground((unsigned char)((ImpressionistUI *)(o->user_data()))->m_nBackgroundAlpha);}

void ImpressionistUI::cb_edgeImage(Fl_Widget* o, void* v){
	((ImpressionistUI *)(o->user_data()))->m_origView->toggleEdge();}

void ImpressionistUI::cb_filterChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc* pDoc=pUI->getDocument();

	//	int type=(int)v;
	long long tmp = reinterpret_cast<long long>(v);
	int type = static_cast<int>(tmp);

	pDoc->setFilterChoice(type);
}

void ImpressionistUI::cb_apply_filter_button(Fl_Widget* o, void* v){
	((ImpressionistUI*)(o->user_data()))->m_pDoc->processFilterCall(1);}

void ImpressionistUI::cb_preview_filter_button(Fl_Widget* o, void* v){
	((ImpressionistUI*)(o->user_data()))->m_pDoc->processFilterCall(0);}

void ImpressionistUI::cb_cancel_filter_button(Fl_Widget* o, void* v){
	((ImpressionistUI*)(o->user_data()))->m_pDoc->processFilterCall(-1);}

void ImpressionistUI::cb_scaleInput(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_pDoc->setScale(atof(((Fl_Float_Input *)o)->value()));}

void ImpressionistUI::cb_offsetInput(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_pDoc->setOffset(atof(((Fl_Float_Input *)o)->value()));}

void ImpressionistUI::cb_filterInput_00(Fl_Widget* o, void* v)
{
	(((ImpressionistUI*)(o->user_data()))->fltKernel)[0] = atof(((Fl_Float_Input *)o)->value());}

void ImpressionistUI::cb_filterInput_01(Fl_Widget* o, void* v)
{
	(((ImpressionistUI*)(o->user_data()))->fltKernel)[1] = atof(((Fl_Float_Input *)o)->value());}

void ImpressionistUI::cb_filterInput_02(Fl_Widget* o, void* v)
{
	(((ImpressionistUI*)(o->user_data()))->fltKernel)[2] = atof(((Fl_Float_Input *)o)->value());}

void ImpressionistUI::cb_filterInput_03(Fl_Widget* o, void* v)
{
	(((ImpressionistUI*)(o->user_data()))->fltKernel)[3] = atof(((Fl_Float_Input *)o)->value());}

void ImpressionistUI::cb_filterInput_04(Fl_Widget* o, void* v)
{
	(((ImpressionistUI*)(o->user_data()))->fltKernel)[4] = atof(((Fl_Float_Input *)o)->value());}

void ImpressionistUI::cb_filterInput_10(Fl_Widget* o, void* v)
{
	(((ImpressionistUI*)(o->user_data()))->fltKernel)[5] = atof(((Fl_Float_Input *)o)->value());}

void ImpressionistUI::cb_filterInput_11(Fl_Widget* o, void* v)
{
	(((ImpressionistUI*)(o->user_data()))->fltKernel)[6] = atof(((Fl_Float_Input *)o)->value());}

void ImpressionistUI::cb_filterInput_12(Fl_Widget* o, void* v)
{
	(((ImpressionistUI*)(o->user_data()))->fltKernel)[7] = atof(((Fl_Float_Input *)o)->value());}

void ImpressionistUI::cb_filterInput_13(Fl_Widget* o, void* v)
{
	(((ImpressionistUI*)(o->user_data()))->fltKernel)[8] = atof(((Fl_Float_Input *)o)->value());}

void ImpressionistUI::cb_filterInput_14(Fl_Widget* o, void* v)
{
	(((ImpressionistUI*)(o->user_data()))->fltKernel)[9] = atof(((Fl_Float_Input *)o)->value());}

void ImpressionistUI::cb_filterInput_20(Fl_Widget* o, void* v)
{
	(((ImpressionistUI*)(o->user_data()))->fltKernel)[10] = atof(((Fl_Float_Input *)o)->value());}

void ImpressionistUI::cb_filterInput_21(Fl_Widget* o, void* v)
{
	(((ImpressionistUI*)(o->user_data()))->fltKernel)[11] = atof(((Fl_Float_Input *)o)->value());}

void ImpressionistUI::cb_filterInput_22(Fl_Widget* o, void* v)
{
	(((ImpressionistUI*)(o->user_data()))->fltKernel)[12] = atof(((Fl_Float_Input *)o)->value());}

void ImpressionistUI::cb_filterInput_23(Fl_Widget* o, void* v)
{
	(((ImpressionistUI*)(o->user_data()))->fltKernel)[13] = atof(((Fl_Float_Input *)o)->value());}

void ImpressionistUI::cb_filterInput_24(Fl_Widget* o, void* v)
{
	(((ImpressionistUI*)(o->user_data()))->fltKernel)[14] = atof(((Fl_Float_Input *)o)->value());}

void ImpressionistUI::cb_filterInput_30(Fl_Widget* o, void* v)
{
	(((ImpressionistUI*)(o->user_data()))->fltKernel)[15] = atof(((Fl_Float_Input *)o)->value());}

void ImpressionistUI::cb_filterInput_31(Fl_Widget* o, void* v)
{
	(((ImpressionistUI*)(o->user_data()))->fltKernel)[16] = atof(((Fl_Float_Input *)o)->value());}

void ImpressionistUI::cb_filterInput_32(Fl_Widget* o, void* v)
{
	(((ImpressionistUI*)(o->user_data()))->fltKernel)[17] = atof(((Fl_Float_Input *)o)->value());}

void ImpressionistUI::cb_filterInput_33(Fl_Widget* o, void* v)
{
	(((ImpressionistUI*)(o->user_data()))->fltKernel)[18] = atof(((Fl_Float_Input *)o)->value());}

void ImpressionistUI::cb_filterInput_34(Fl_Widget* o, void* v)
{
	(((ImpressionistUI*)(o->user_data()))->fltKernel)[19] = atof(((Fl_Float_Input *)o)->value());}

void ImpressionistUI::cb_filterInput_40(Fl_Widget* o, void* v)
{
	(((ImpressionistUI*)(o->user_data()))->fltKernel)[20] = atof(((Fl_Float_Input *)o)->value());}

void ImpressionistUI::cb_filterInput_41(Fl_Widget* o, void* v)
{
	(((ImpressionistUI*)(o->user_data()))->fltKernel)[21] = atof(((Fl_Float_Input *)o)->value());}

void ImpressionistUI::cb_filterInput_42(Fl_Widget* o, void* v)
{
	(((ImpressionistUI*)(o->user_data()))->fltKernel)[22] = atof(((Fl_Float_Input *)o)->value());}

void ImpressionistUI::cb_filterInput_43(Fl_Widget* o, void* v)
{
	(((ImpressionistUI*)(o->user_data()))->fltKernel)[23] = atof(((Fl_Float_Input *)o)->value());}

void ImpressionistUI::cb_filterInput_44(Fl_Widget* o, void* v)
{
	(((ImpressionistUI*)(o->user_data()))->fltKernel)[24] = atof(((Fl_Float_Input *)o)->value());}

double* ImpressionistUI::getFilterKernel(){
	return fltKernel;}

ImpressionistUI::~ImpressionistUI()
{
}
