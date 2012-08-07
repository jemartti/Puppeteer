#include "scene.hpp"
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>


SceneNode::SceneNode( const std::string& name )
	: m_name( name )
{
}

SceneNode::~SceneNode()
{
}

void SceneNode::walk_gl( bool picking ) const
{
	// Simply walk through all the nodes
	ChildList::const_iterator i;
	for ( i = m_children.begin(); i != m_children.end(); ++i )
	{
		(*i)->set_transform( m_trans * (*i)->get_transform() );
		(*i)->walk_gl( picking );
		(*i)->set_transform( this->m_invtrans * (*i)->get_transform() );
	}
}

void SceneNode::rotate( char axis, double angle )
{
	Vector4D row1, row2, row3, row4;

	if ( axis )
	{
		// Do the bounds calculations for the joints, ensuring we don't go over
		// the defined min/max values
		if ( is_joint() )
		{
			JointNode* temp = dynamic_cast<JointNode*>(this);
			double rmin, rini, rmax, rcur;
			if ( axis == 'x' )
			{
				rmin = temp->get_joint_x().min;
				rini = temp->get_joint_x().init;
				rmax = temp->get_joint_x().max;
				rcur = temp->get_joint_x().cur;
			}
			if ( axis == 'y' )
			{
				rmin = temp->get_joint_y().min;
				rini = temp->get_joint_y().init;
				rmax = temp->get_joint_y().max;
				rcur = temp->get_joint_y().cur;
			}
			if ( (rcur + angle) <= rmin )
			{
				if ( rcur > rmin )
				{
					angle = rmin - rcur;
				}
				else
				{
					angle = 0;
				}
			}
			else if ( (rcur + angle) >= rmax )
			{
				if ( rcur < rmax )
				{
					angle = rmax - rcur;
				}
				else
				{
					angle = 0;
				}
			}
			rcur += angle;
			if ( axis == 'x' )
			{
				dynamic_cast<JointNode*>(temp)->set_joint_x(
						rmin, rini, rmax, rcur);
			}
			if ( axis == 'y' )
			{
				dynamic_cast<JointNode*>(temp)->set_joint_y(
						rmin, rini, rmax, rcur);
			}
		}

		angle = ( angle / 180.0 ) * M_PI;

		double   co = cos( angle );
		double   si = sin( angle );

		if ( axis == 'x' )
		{
			row1 = Vector4D( 1, 0,   0,  0 );
			row2 = Vector4D( 0, co, -si, 0 );
			row3 = Vector4D( 0, si,  co, 0 );
			row4 = Vector4D( 0, 0,   0,  1 );
		}
		else if ( axis == 'y' )
		{
			row1 = Vector4D(  co, 0, si, 0 );
			row2 = Vector4D(  0,  1, 0,  0 );
			row3 = Vector4D( -si, 0, co, 0 );
			row4 = Vector4D(  0,  0, 0,  1 );
		}
		else if ( axis == 'z' )
		{
			row1 = Vector4D( co, -si, 0, 0 );
			row2 = Vector4D( si,  co, 0, 0 );
			row3 = Vector4D( 0,   0,  1, 0 );
			row4 = Vector4D( 0,   0,  0, 1 );
		}
	}

	this->set_transform( m_trans * Matrix4x4(row1, row2, row3, row4) );
}

void SceneNode::scale( const Vector3D& amount )
{
	Vector4D  row1, row2, row3, row4;
	double    x = amount[0];
	double    y = amount[1];
	double    z = amount[2];

	row1 = Vector4D( x, 0, 0, 0 );
	row2 = Vector4D( 0, y, 0, 0 );
	row3 = Vector4D( 0, 0, z, 0 );
	row4 = Vector4D( 0, 0, 0, 1 );

	this->set_transform( m_trans * Matrix4x4(row1, row2, row3, row4) );
}

void SceneNode::translate( const Vector3D& amount )
{
	Vector4D  row1, row2, row3, row4;
	double    x = amount[0];
	double    y = amount[1];
	double    z = amount[2];

	row1 = Vector4D( 1, 0, 0, x );
	row2 = Vector4D( 0, 1, 0, y );
	row3 = Vector4D( 0, 0, 1, z );
	row4 = Vector4D( 0, 0, 0, 1 );

	this->set_transform( m_trans * Matrix4x4(row1, row2, row3, row4) );
}

bool SceneNode::is_joint() const
{
	return false;
}

JointNode::JointNode( const std::string& name )
	: SceneNode(name)
{
}

JointNode::~JointNode()
{
}

void JointNode::walk_gl( bool picking ) const
{
	// Simply walk through all the nodes
	ChildList::const_iterator i;
	for ( i = m_children.begin(); i != m_children.end(); ++i )
	{
		(*i)->set_transform( m_trans * (*i)->get_transform() );
		(*i)->walk_gl( picking );
		(*i)->set_transform( this->m_invtrans * (*i)->get_transform() );
	}
}

bool JointNode::is_joint() const
{
	return true;
}

void JointNode::set_joint_x( double min, double init, double max )
{
	m_joint_x.min  = min;
	m_joint_x.init = init;
	m_joint_x.max  = max;
	m_joint_x.cur  = init;
}

void JointNode::set_joint_y( double min, double init, double max )
{
	m_joint_y.min  = min;
	m_joint_y.init = init;
	m_joint_y.max  = max;
	m_joint_y.cur  = init;
}

void JointNode::set_joint_x( double min, double init, double max, double cur )
{
	m_joint_x.min  = min;
	m_joint_x.init = init;
	m_joint_x.max  = max;
	m_joint_x.cur  = cur;
}

void JointNode::set_joint_y( double min, double init, double max, double cur )
{
	m_joint_y.min  = min;
	m_joint_y.init = init;
	m_joint_y.max  = max;
	m_joint_y.cur  = cur;
}

GeometryNode::GeometryNode( const std::string& name, Primitive* primitive )
	: SceneNode( name ), m_primitive( primitive )
{
}

GeometryNode::~GeometryNode()
{
}

void GeometryNode::walk_gl( bool picking ) const
{
	// Walk through all the far down nodes and transform/draw them
	glPushMatrix();
	glMultMatrixd( this->m_trans.transpose().begin() );
	m_material->apply_gl();
	if(picking)
	{
		glPushName( m_id );
	}
	m_primitive->walk_gl( picking );
	if(picking)
	{
		glPopName();
	}
	glPopMatrix();
}
