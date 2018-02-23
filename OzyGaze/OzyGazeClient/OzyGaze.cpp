#include <io.h> // _setmode
#include <fcntl.h> // _O_U8TEXT
#include <cstdlib> //system("pause");
#include <cstdio> //_fileno , stdout
#include "Statistics/Application/AppStatGrabber.h" // AppStatGrabber

int main(void)
{
	_setmode(_fileno(stdout), _O_U8TEXT);
	
	//AppStatGrabber appStatGrabber;

	///*
	//dispather
	// */

	//appStatGrabber.Start();

	ActiveWindowTracker wtr;

	wtr.Start();


	system("pause");

	wtr.Pause();
		
	wtr.Stop();

	system("pause");

	wtr.Start();

	system("pause");

	return 0;
}
