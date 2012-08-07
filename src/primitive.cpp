#include "primitive.hpp"
#include <GL/gl.h>
#include <GL/glu.h>


Primitive::~Primitive()
{
}

Sphere::~Sphere()
{
}

void Sphere::walk_gl( bool /*picking*/ ) const
{
	// Draw a sphere in just about the simplest way possible
	GLUquadric* quadric = gluNewQuadric();
	gluQuadricNormals( quadric, GLU_SMOOTH );
	gluQuadricTexture( quadric, GL_TRUE );
	gluQuadricDrawStyle( quadric, GLU_FILL );
	gluSphere( quadric, 1.0f, 15, 15 );
}
