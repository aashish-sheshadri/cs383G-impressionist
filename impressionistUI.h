//
// impressionistUI.h
//
// The header file for the UI part
//

#ifndef ImpressionistUI_h
#define ImpressionistUI_h

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_File_Chooser.H>		// FLTK file chooser
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Color_Chooser.H>
#include <FL/Fl_Float_Input.H>

#include "impressionist.h"
#include "originalView.h"
#include "paintView.h"

#include "impBrush.h"

#include <cstdlib>
#include <algorithm>

const int FLT_WIDTH = 5;
const int FLT_HEIGHT = 5;

class ImpressionistUI {
public:
	ImpressionistUI();
	~ImpressionistUI();

	// The FLTK widgets
	Fl_Window*			m_mainWindow;
	Fl_Menu_Bar*		m_menubar;
								
	PaintView*			m_paintView;
	OriginalView*		m_origView;

// for brush dialog
	Fl_Window*			m_brushDialog;
	Fl_Choice*			m_BrushTypeChoice;
	Fl_Choice*			m_AngleTypeSelector;
	Fl_Choice*			m_FilterTypeChoice;

	Fl_Slider*			m_BrushSizeSlider;
	Fl_Button*          m_ClearCanvasButton;

	Fl_Slider*			m_BrushAngleSlider;

	Fl_Button* 			m_transparentBackgroundButton;

// for filter dialog
	Fl_Window*			m_filterDialog;
	Fl_Button* 			m_ApplyFilterButton;
	Fl_Button* 			m_PreviewFilterButton;
	Fl_Button* 			m_CancelFilterButton;


	// Member functions
	void				setDocument(ImpressionistDoc* doc);
	ImpressionistDoc*	getDocument();

	void				show();
	void				resize_windows(int w, int h);

	// Interface to get attribute

	int					getSize();
	void				setSize(int size);

	int					getAngle();
	void				setAngle(int angle);

	int 				getAlpha();

	// Callbacks for the image filter dialogue (different from
	// the other callbacks because they aren't static)
	void				initFltDesignUI(void);
	void				updateFilter(void);

	void				cancelFilter(void);
	void				applyFilter(void);
	void				previewFilter(void);

	double*				getFilterKernel();

private:
	ImpressionistDoc*	m_pDoc;		// pointer to document to communicate with the document

	// All attributes here
	int		m_nSize;
	int 	m_nAngle;
	int 	m_nAlpha;

	// These attributes are set by the filter kernel UI
	double fltKernel[FLT_WIDTH*FLT_HEIGHT];		//the kernel of the image filter
	double m_nScale;
	double m_nOffset;						//the divisor and offset parameter, 
												//applied after convolving to transform 
												//the filtered image into the range [0,255]


	// Static class members
	static Fl_Menu_Item		menuitems[];
	static Fl_Menu_Item		brushTypeMenu[NUM_BRUSH_TYPE+1];
	static Fl_Menu_Item		angleChoiceMenu[3+1];

	static Fl_Menu_Item		filterChoiceMenu[2+1];

	static ImpressionistUI*	whoami(Fl_Menu_* o);

	// All callbacks here.  Callbacks are declared 
	// static
	static void	cb_load_image(Fl_Menu_* o, void* v);
	static void	cb_save_image(Fl_Menu_* o, void* v);
	static void	cb_brushes(Fl_Menu_* o, void* v);
	static void	cb_filter_kernel(Fl_Menu_* o, void* v);
	static void	cb_clear_canvas(Fl_Menu_* o, void* v);
	static void	cb_exit(Fl_Menu_* o, void* v);
	static void	cb_about(Fl_Menu_* o, void* v);
	static void	cb_brushChoice(Fl_Widget* o, void* v);
	static void	cb_clear_canvas_button(Fl_Widget* o, void* v);
	static void	cb_sizeSlides(Fl_Widget* o, void* v);
	static void	cb_angleSlides(Fl_Widget* o, void* v);
	static void	cb_angleChoice(Fl_Widget* o, void* v);
	static void cb_filter(Fl_Menu_* o, void* v);
	static void	cb_alphaSlides(Fl_Widget* o, void* v);
	static void	cb_filterChoice(Fl_Widget* o, void* v);
	static void	cb_transparentBackground(Fl_Widget* o, void* v);
	
	//filter callbacks
	static void	cb_apply_filter_button(Fl_Widget* o, void* v);
	static void	cb_preview_filter_button(Fl_Widget* o, void* v);
	static void	cb_cancel_filter_button(Fl_Widget* o, void* v);
	static void cb_scaleInput(Fl_Widget* o, void* v);
	static void cb_offsetInput(Fl_Widget* o, void* v);
	static void cb_filterInput_00(Fl_Widget* o, void* v);
	static void cb_filterInput_01(Fl_Widget* o, void* v);
	static void cb_filterInput_02(Fl_Widget* o, void* v);
	static void cb_filterInput_03(Fl_Widget* o, void* v);
	static void cb_filterInput_04(Fl_Widget* o, void* v);
	static void cb_filterInput_10(Fl_Widget* o, void* v);
	static void cb_filterInput_11(Fl_Widget* o, void* v);
	static void cb_filterInput_12(Fl_Widget* o, void* v);
	static void cb_filterInput_13(Fl_Widget* o, void* v);
	static void cb_filterInput_14(Fl_Widget* o, void* v);
	static void cb_filterInput_20(Fl_Widget* o, void* v);
	static void cb_filterInput_21(Fl_Widget* o, void* v);
	static void cb_filterInput_22(Fl_Widget* o, void* v);
	static void cb_filterInput_23(Fl_Widget* o, void* v);
	static void cb_filterInput_24(Fl_Widget* o, void* v);
	static void cb_filterInput_30(Fl_Widget* o, void* v);
	static void cb_filterInput_31(Fl_Widget* o, void* v);
	static void cb_filterInput_32(Fl_Widget* o, void* v);
	static void cb_filterInput_33(Fl_Widget* o, void* v);
	static void cb_filterInput_34(Fl_Widget* o, void* v);
	static void cb_filterInput_40(Fl_Widget* o, void* v);
	static void cb_filterInput_41(Fl_Widget* o, void* v);
	static void cb_filterInput_42(Fl_Widget* o, void* v);
	static void cb_filterInput_43(Fl_Widget* o, void* v);
	static void cb_filterInput_44(Fl_Widget* o, void* v);
};

#endif

