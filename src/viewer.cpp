#include "viewer.hpp"
#include "algebra.hpp"
#include <iostream>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "scene_lua.hpp"


// Quick and dirty global data structure
// Totally violates basic OOP principles, but I needed something to work
// fast and simple here. Time was running out!
std::vector<GeomNode*> gldata;


Viewer::Viewer()
{
	Glib::RefPtr<Gdk::GL::Config> glconfig;

	// Ask for an OpenGL Setup with
	//  - red, green and blue component colour
	//  - a depth buffer to avoid things overlapping wrongly
	//  - double-buffered rendering to avoid tearing/flickering
	glconfig = Gdk::GL::Config::create( Gdk::GL::MODE_RGB   |
										Gdk::GL::MODE_DEPTH |
										Gdk::GL::MODE_DOUBLE );
	if ( glconfig == 0 )
	{
		// If we can't get this configuration, die
		std::cerr << "Unable to setup OpenGL Configuration!" << std::endl;
		abort();
	}

	// Accept the configuration
	set_gl_capability( glconfig );

	// Register the fact that we want to receive these events
	add_events( Gdk::BUTTON1_MOTION_MASK    |
				Gdk::BUTTON2_MOTION_MASK    |
				Gdk::BUTTON3_MOTION_MASK    |
				Gdk::BUTTON_PRESS_MASK      |
				Gdk::BUTTON_RELEASE_MASK    |
				Gdk::BUTTON_MOTION_MASK		|
				Gdk::POINTER_MOTION_MASK	|
				Gdk::VISIBILITY_NOTIFY_MASK );


	m_mode        = POSITION;

	m_translation = Matrix4x4();
	m_rotation    = Matrix4x4();

	m_circ        = false;
	m_zbuf        = false;
	m_bcul        = false;
	m_fcul        = false;

	m_button1     = false;
	m_button2     = false;
	m_button3     = false;
	m_ixpos       = 0.0;
	m_xpos        = 0.0;
	m_iypos       = 0.0;
	m_ypos        = 0.0;
}

Viewer::~Viewer()
{
  // There's nothing really to clean up here
}

void Viewer::set_mode( Mode mode )
{
	m_mode = mode;
}

void Viewer::set_model( std::string filename )
{
	SceneNode* root = import_lua( filename );
	if ( !root )
	{
		// TODO
		// Proper code exiting on issue
		//std::cerr << "Could not open " << filename << std::endl;
	}
	m_model = root;
}

void Viewer::tog_circ()
{
	m_circ = !m_circ;
	invalidate();
}

void Viewer::tog_zbuf()
{
	m_zbuf = !m_zbuf;
	invalidate();
}

void Viewer::tog_bcul()
{
	m_bcul = !m_bcul;
	invalidate();
}

void Viewer::tog_fcul()
{
	m_fcul = !m_fcul;
	invalidate();
}

void Viewer::reset_pos()
{
	m_translation = Matrix4x4();
	invalidate();
}

void Viewer::reset_orient()
{
	m_rotation = Matrix4x4();
	invalidate();
}

void Viewer::reset_joint()
{
	// TODO
	// Code to reset all joint positions
	//invalidate();
}
void Viewer::reset_all()
{
	reset_pos();
	reset_orient();
	reset_joint();
}

void Viewer::undo()
{
	// TODO
	// Undo command
}

void Viewer::redo()
{
	// TODO
	// Redo command
}

void Viewer::invalidate()
{
	// Force a rerender
	Gtk::Allocation allocation = get_allocation();
	get_window()->invalidate_rect( allocation, false );
}

void Viewer::on_realize()
{
	// Do some OpenGL setup.
	// First, let the base class do whatever it needs to
	Gtk::GL::DrawingArea::on_realize();

	Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

	if ( !gldrawable )
	{
		return;
	}

	if ( !gldrawable->gl_begin(get_gl_context()) )
	{
		return;
	}

	glShadeModel( GL_SMOOTH );
	glClearColor( 0.4, 0.4, 0.4, 0.0 );
	glEnable( GL_DEPTH_TEST );

	gldrawable->gl_end();
}

bool Viewer::on_expose_event( GdkEventExpose* /*event*/ )
{
	Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

	if ( !gldrawable )
	{
		return false;
	}

	if ( !gldrawable->gl_begin(get_gl_context()) )
	{
		return false;
	}

	// Set up for perspective drawing
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glViewport( 0, 0, get_width(), get_height() );
	gluPerspective( 40.0, (GLfloat)get_width() / (GLfloat)get_height(),
			0.1, 1000.0 );

	// change to model view for drawing
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	// Clear framebuffer
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Set up lighting
	glEnable( GL_LIGHTING );
	GLfloat white_light[]    = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	glLightfv( GL_LIGHT0, GL_DIFFUSE,  white_light );
	glLightfv( GL_LIGHT0, GL_POSITION, light_position );
	glEnable( GL_LIGHT0 );

	// Set application state
	if ( m_bcul || m_fcul )
	{
		glEnable( GL_CULL_FACE );
		if ( m_bcul && m_fcul )
		{
			glCullFace( GL_FRONT_AND_BACK );
		}
		else if ( m_bcul )
		{
			glCullFace( GL_BACK );
		}
		else if ( m_fcul )
		{
			glCullFace( GL_FRONT );
		}
	}
	else
	{
		glDisable( GL_CULL_FACE );
	}
	if ( m_zbuf )
	{
		glEnable( GL_DEPTH_TEST );
		glDepthMask( GL_TRUE );
	}
	else
	{
		glDisable( GL_DEPTH_TEST );
	}

	// Draw stuff
	draw_model();
	draw_trackball_circle();

	// Swap the contents of the front and back buffers so we see what we
	// just drew. This should only be done if double buffering is enabled.
	gldrawable->swap_buffers();

	gldrawable->gl_end();

	return true;
}

bool Viewer::on_configure_event( GdkEventConfigure* event )
{
	Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

	if ( !gldrawable )
	{
		return false;
	}

	if ( !gldrawable->gl_begin(get_gl_context()) )
	{
		return false;
	}

	// Set up perspective projection, using current size and aspect
	// ratio of display
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glViewport( 0, 0, event->width, event->height );
	gluPerspective( 40.0, (GLfloat)event->width / (GLfloat)event->height,
			0.1, 1000.0 );

	// Reset to modelview matrix mode
	glMatrixMode( GL_MODELVIEW );

	gldrawable->gl_end();

	return true;
}

bool Viewer::on_button_press_event( GdkEventButton* event )
{
	// Which button(s) pressed?
	switch (event->button)
	{
	case 1:
		m_button1 = true;
		if ( m_mode == JOINTS )
		{
			picking( event->x, event->y );
		}
		break;
	case 2:
		m_button2 = true;
		break;
	case 3:
		m_button3 = true;
		break;
	default:
		break;
	}

	// Capture mouse position information
	m_ixpos = event->x;
	m_xpos  = event->x;
	m_iypos = event->y;
	m_ypos  = event->y;

	invalidate();
	return true;
}

bool Viewer::on_button_release_event( GdkEventButton* event )
{
	// Which button(s) released?
	switch (event->button)
	{
	case 1:
		m_button1 = false;
		break;
	case 2:
		m_button2 = false;
		break;
	case 3:
		m_button3 = false;
		break;
	default:
		break;
	}

	return true;
}

bool Viewer::on_motion_notify_event( GdkEventMotion* event )
{
	if ( m_button1 || m_button2 || m_button3 )
	{
		m_ixpos = m_xpos;
		m_iypos = m_ypos;
		m_xpos  = event->x;
		m_ypos  = event->y;

		switch ( m_mode )
		{
		case POSITION:
			// Which button(s) pressed?
			if ( m_button1 )
			{
				m_translation = m_translation *
						translation(
								Vector3D(( m_xpos - m_ixpos ) / 30.0, 0, 0) ) *
						translation(
								Vector3D(0, ( m_iypos - m_ypos ) / 30.0, 0) );
			}
			if ( m_button2 )
			{
				m_translation = m_translation * translation(
						Vector3D(0, 0, ( m_ypos - m_iypos ) / 30.0) );
			}
			if ( m_button3 )
			{
				// Do the trackball stuff
				int x_centre = get_width()  / 2;
				int y_centre = get_height() / 2;
				int diameter = get_width() < get_height() ?
				  (float)get_width() * 0.5 : (float)get_height() * 0.5;
				float x_fin, y_fin, z_fin;

				vCalcRotVec( m_xpos  - x_centre,
							 m_ypos  - y_centre,
							 m_ixpos - x_centre,
							 m_iypos - y_centre,
							 diameter,
							 &x_fin,
							 &y_fin,
							 &z_fin );

				Matrix4x4 newRot;
				vAxisRotMatrix( x_fin, -y_fin, z_fin, &newRot);
			    m_rotation = m_rotation * newRot;
			}
			break;
		case JOINTS:
			// Which button(s) pressed?
			// Standard joint movement
			if ( m_button2 )
			{
				std::vector<GeomNode*>::const_iterator i;
				for ( i = gldata.begin(); i != gldata.end(); ++i )
				{
					if ( (*i)->m_selected )
					{
						(*i)->m_parent->rotate( 'x',
								(m_iypos - m_ypos) / 10.0 );
						(*i)->m_parent->rotate( 'y',
								(m_iypos - m_ypos) / 10.0 );
					}
				}
			}
			// Special head movement
			if ( m_button3 )
			{
				// TODO
				// Code to rotate the head left and right with x motion
				/*
				if ( m_model )
				{
					m_model->rotate( 'x', (m_iypos - m_ypos) / 10.0 );
				}
				*/
			}
			break;
		default:
			break;
		}
		invalidate();
	}

	return true;
}

void Viewer::draw_trackball_circle()
{
	if ( m_circ )
	{
		int current_width  = get_width();
		int current_height = get_height();

		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		glViewport( 0, 0, current_width, current_height );
		glOrtho( 0.0, (float)current_width, 0.0,
				(float)current_height, -0.1, 0.1 );

		// change to model view for drawing
		glMatrixMode( GL_MODELVIEW );

		// Reset modelview matrix
		glLoadIdentity();

		// draw a circle for use with the trackball
		glDisable( GL_LIGHTING );
		glEnable( GL_LINE_SMOOTH );
		glColor3f( 1.0, 1.0, 1.0 );
		double radius = current_width < current_height ?
				(float)current_width * 0.25 : (float)current_height * 0.25;
		glTranslated( (float)current_width * 0.5,
				(float)current_height * 0.5, 0 );
		glBegin( GL_LINE_LOOP );
		for( size_t i = 0; i < 40; ++i )
		{
			double cosine = radius * cos( i * 2 * M_PI / 40 );
			double sine   = radius * sin( i * 2 * M_PI / 40 );
			glVertex2f( cosine, sine );
		}
		glEnd();
		glColor3f( 0.0, 0.0, 0.0 );
		glDisable( GL_LINE_SMOOTH );
	}
}

// Do the actual drawing of the model
void Viewer::draw_model( bool picking )
{
	m_model->set_transform( m_model->get_transform() *
			m_rotation * m_translation );
	m_model->walk_gl( picking );
	m_model->set_transform( m_model->get_transform() *
			(m_rotation * m_translation).invert() );
}

// Implements picking algorithm described in course notes, p94
void Viewer::picking( int x, int y )
{
	GLuint pickingBuffer[512];
	GLint *viewport = new GLint[4];

	glSelectBuffer( 512, pickingBuffer );
	glRenderMode( GL_SELECT );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	glGetIntegerv( GL_VIEWPORT, viewport );

	glPushMatrix();
	glLoadIdentity();
	gluPickMatrix( x, viewport[3] - y, 1, 1, viewport );

	gluPerspective( 40.0, (GLfloat)get_width() / (GLfloat)get_height(),
			0.1, 1000.0 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	draw_model( true );
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	size_t hits = glRenderMode( GL_RENDER );

	glMatrixMode( GL_MODELVIEW );

	// It's ugly, but it finds our picked results
	size_t loc = 0;
	for( size_t idx = 0; idx < hits; ++idx ) {
		GLuint len = pickingBuffer[loc++];
		loc++;
		loc++;
		for( size_t jdx = 0; jdx < len; ++jdx ) {
			int tb = pickingBuffer[loc++];
			std::vector<GeomNode*>::const_iterator i;
			for ( i = gldata.begin(); i != gldata.end(); ++i )
			{
				if ( (*i)->m_node->get_id() == tb)
				{
					if ( !(*i)->m_selected )
					{
						(*i)->m_oldmat = (*i)->m_node->get_material();
						Material* tm = new PhongMaterial( Colour(0.5, 0.0, 0.8),
								Colour(0.5, 0, 0.8), 1 );
						(*i)->m_node->set_material( tm );
						(*i)->m_selected = true;
					}
					else
					{
						(*i)->m_node->set_material( (*i)->m_oldmat );
						(*i)->m_selected = false;
					}
				}
			}
		}
	}
}


// Return a matrix to represent a displacement of the given vector.
// Taken from A2 code
Matrix4x4 Viewer::translation( const Vector3D& displacement )
{
	Matrix4x4 t;
	Vector4D  row1, row2, row3, row4;
	double    x = displacement[0];
	double    y = displacement[1];
	double    z = displacement[2];

	row1 = Vector4D( 1, 0, 0, x );
	row2 = Vector4D( 0, 1, 0, y );
	row3 = Vector4D( 0, 0, 1, z );
	row4 = Vector4D( 0, 0, 0, 1 );
	t    = Matrix4x4( row1, row2, row3, row4 );

	return t;
}

// Taken from trackball.c
void Viewer::vCalcRotVec( float fNewX, float fNewY, float fOldX, float fOldY,
		float fDiameter, float *fVecX, float *fVecY, float *fVecZ )
{
	float fNewVecX, fNewVecY, fNewVecZ, fOldVecX, fOldVecY, fOldVecZ, fLength;

	fNewVecX = fNewX * 2.0 / fDiameter;
	fNewVecY = fNewY * 2.0 / fDiameter;
	fNewVecZ = ( 1.0 - fNewVecX * fNewVecX - fNewVecY * fNewVecY );

	if ( fNewVecZ < 0.0 )
	{
		fLength   = sqrt( 1.0 - fNewVecZ );
		fNewVecZ  = 0.0;
		fNewVecX /= fLength;
		fNewVecY /= fLength;
	}
	else
	{
		fNewVecZ = sqrt( fNewVecZ );
	}

	fOldVecX    = fOldX * 2.0 / fDiameter;
	fOldVecY    = fOldY * 2.0 / fDiameter;
	fOldVecZ    = ( 1.0 - fOldVecX * fOldVecX - fOldVecY * fOldVecY );

	if ( fOldVecZ < 0.0 )
	{
		fLength   = sqrt( 1.0 - fOldVecZ );
		fOldVecZ  = 0.0;
		fOldVecX /= fLength;
		fOldVecY /= fLength;
	}
	else
	{
		fOldVecZ = sqrt( fOldVecZ );
	}

	*fVecX = fOldVecY * fNewVecZ - fNewVecY * fOldVecZ;
	*fVecY = fOldVecZ * fNewVecX - fNewVecZ * fOldVecX;
	*fVecZ = fOldVecX * fNewVecY - fNewVecX * fOldVecY;
}

// Taken from trackball.c (slightly modified to fit Matrix structures)
void Viewer::vAxisRotMatrix( float fVecX, float fVecY,
		float fVecZ, Matrix4x4* mNewMat )
{
	float fRadians, fInvLength, fNewVecX, fNewVecY, fNewVecZ;

	fRadians = sqrt( fVecX * fVecX + fVecY * fVecY + fVecZ * fVecZ );

	if ( fRadians > -0.000001 && fRadians < 0.000001 )
	{
		*mNewMat = Matrix4x4();
		return;
	}

	fInvLength = 1 / fRadians;
	fNewVecX   = fVecX * fInvLength;
	fNewVecY   = fVecY * fInvLength;
	fNewVecZ   = fVecZ * fInvLength;

	double dSinAlpha = sin( fRadians );
	double dCosAlpha = cos( fRadians );
	double dT = 1 - dCosAlpha;

	double* mNewMatVec = new double[16];
	mNewMatVec[0]  = dCosAlpha + fNewVecX * fNewVecX * dT;
	mNewMatVec[1]  = fNewVecX * fNewVecY*dT + fNewVecZ * dSinAlpha;
	mNewMatVec[2]  = fNewVecX * fNewVecZ*dT - fNewVecY * dSinAlpha;
	mNewMatVec[3]  = 0;

	mNewMatVec[4]  = fNewVecX * fNewVecY * dT - dSinAlpha * fNewVecZ;
	mNewMatVec[5]  = dCosAlpha + fNewVecY * fNewVecY * dT;
	mNewMatVec[6]  = fNewVecY * fNewVecZ * dT + dSinAlpha * fNewVecX;
	mNewMatVec[7]  = 0;

	mNewMatVec[8]  = fNewVecZ * fNewVecX * dT + dSinAlpha * fNewVecY;
	mNewMatVec[9]  = fNewVecZ * fNewVecY * dT - dSinAlpha * fNewVecX;
	mNewMatVec[10] = dCosAlpha + fNewVecZ * fNewVecZ * dT;
	mNewMatVec[11] = 0;

	mNewMatVec[12] = 0;
	mNewMatVec[13] = 0;
	mNewMatVec[14] = 0;
	mNewMatVec[15] = 1;

	*mNewMat = Matrix4x4(mNewMatVec);
}
