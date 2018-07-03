/*	FIT2049 - Example Code
 *	Main.cpp
 *	Created by Elliott Wilson - 2015 - Monash Univeristy
 *	Entry point and main driver for the application
 */

#include "Window.h"

//Windows API programs have a special Main method, WinMain!
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	CoInitialize(0);
	Window* win = new Window("Game", 1024, 768, false);	//We'll create our window object, set a size and if we want it fullscreen

	if(win->Initialise())	//If the window initialises...
	{
		win->Start();		//...then we start the message pump running!
	}
	else
	{
		MessageBox(NULL, "Could not create Window", "Error", MB_OK); //otherwise, show an error message
	}

	win->Shutdown();	//If we have an error, or the message pump stops then we Shutdown our window...
	delete win;			//...delete it...
	win = NULL;

	return 0;			//...and quit!
}