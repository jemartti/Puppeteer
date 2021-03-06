#include "material.hpp"
#include <GL/gl.h>
#include <GL/glu.h>


Material::~Material()
{
}

PhongMaterial::PhongMaterial( const Colour& kd,
							  const Colour& ks,
							  double shininess )
	: m_kd( kd ), m_ks( ks ), m_shininess( shininess )
{
}

PhongMaterial::~PhongMaterial()
{
}

void PhongMaterial::apply_gl() const
{
	// Set up a basic colouring, shading, and lighting model
	GLfloat diffuse[]   = { m_kd.R(), m_kd.G(), m_kd.B(), 1.0 };
	GLfloat shininess[] = { m_shininess };

	glShadeModel( GL_SMOOTH );
	glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse );
	glMaterialfv( GL_FRONT, GL_SHININESS, shininess );

	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );
}
