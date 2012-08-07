#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#include "scene.hpp"

#include <vector>
#include <stack>
#include <gtkmm.h>
#include <gtkglmm.h>


// Define a simple data structure used to process the nodes for picking
class GeomNode {
public:
	GeomNode( GeometryNode* node, JointNode* parent )
	{
		m_node     = node;
		m_parent   = parent;
		m_selected = false;
	}

	JointNode*    m_parent;
	GeometryNode* m_node;
	Material*     m_oldmat;
	bool          m_selected;
};
extern std::vector<GeomNode*> gldata;

// The "main" OpenGL widget
class Viewer : public Gtk::GL::DrawingArea {
public:
	// Mode enum
	enum Mode {
		POSITION,
		JOINTS
	};

	Viewer();
	virtual ~Viewer();

	// Setter functions
	void set_mode ( Mode        mode    );
	void set_model( std::string filename );

	// State modification functions
	void tog_circ();
	void tog_zbuf();
	void tog_bcul();
	void tog_fcul();
	void reset_pos();
	void reset_orient();
	void reset_joint();
	void reset_all();
	void undo();
	void redo();

	// A useful function that forces this widget to rerender. If you
	// want to render a new frame, do not call on_expose_event
	// directly. Instead call this, which will cause an on_expose_event
	// call when the time is right.
	void invalidate();
  
protected:
	// Events we implement
	// Note that we could use gtkmm's "signals and slots" mechanism
	// instead, but for many classes there's a convenient member
	// function one just needs to define that'll be called with the
	// event.

	// Called when GL is first initialized
	virtual void on_realize();
	// Called when our window needs to be redrawn
	virtual bool on_expose_event        ( GdkEventExpose*    event );
	// Called when the window is resized
	virtual bool on_configure_event     ( GdkEventConfigure* event );
	// Called when a mouse button is pressed
	virtual bool on_button_press_event  ( GdkEventButton*    event );
	// Called when a mouse button is released
	virtual bool on_button_release_event( GdkEventButton*    event );
	// Called when the mouse moves
	virtual bool on_motion_notify_event ( GdkEventMotion*    event );

	// Draw a circle for the trackball, with OpenGL commands.
	// Assumes the context for the viewer is active.
	void draw_trackball_circle();
  
private:
	// The puppet data structure
	SceneNode* m_model;

	// Current application mode
	Mode       m_mode;

	// Transformation matrices
	Matrix4x4  m_rotation;
	Matrix4x4  m_translation;

	// Application state variables
	bool       m_circ, m_zbuf, m_bcul, m_fcul;

	// Mouse information
	bool       m_button1, m_button2, m_button3;
	double     m_ixpos,   m_iypos;
	double     m_xpos,    m_ypos;

	// The main application functions
	void      draw_model( bool picking = false );

	void      picking( int x, int y );

	Matrix4x4 translation( const Vector3D& displacement );

	void      vCalcRotVec( float fNewX, float fNewY, float fOldX, float fOldY,
			float fDiameter, float *fVecX, float *fVecY, float *fVecZ );

	void      vAxisRotMatrix( float fVecX, float fVecY, float fVecZ,
			Matrix4x4* mNewMat );
};

#endif
