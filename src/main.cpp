#include <iostream>
#include <gtkmm.h>
#include <gtkglmm.h>
#include "appwindow.hpp"
#include "scene_lua.hpp"


int main(int argc, char** argv)
{
	// Construct our main loop
	Gtk::Main kit( argc, argv );

	// Initialize OpenGL
	Gtk::GL::init( argc, argv );

	std::string filename = "puppet.lua";
	if ( argc >= 2 )
	{
		filename = argv[1];
	}

	// Construct our (only) window, passing the input filename
	AppWindow window( filename );

	// And run the application!
	Gtk::Main::run( window );
}
