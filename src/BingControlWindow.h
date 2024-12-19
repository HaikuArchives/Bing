/*
Bing, Copyright (C) 1998-2000 Jean-Baptiste M. Queru

This program is distributed under the terms of the enclosed
JBQ code license.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Contact the author at:
Jean-Baptiste M. Queru, 31 Camino Alto, Millbrae CA 94030, USA
or by e-mail at : jb@queru.com
*/

#ifndef BINGCONTROLWINDOW_H_
#define BINGCONTROLWINDOW_H_

#include <Window.h>

// forward declarations

class BingFloatingWindow;
class BingDigit;

///////////////////////////////////////////////////////////////////////////////
// class : BingApplication
//
// the control window. is the only non-floating window. gets the user
// input and controls the whole game logic.
// 

class BingControlWindow:public BWindow {
public:
	BingControlWindow();
private:
	void ScreenChanged(BRect,color_space); // virtual from BWindow
	void WindowActivated(bool); // virtual from BWindow
	bool QuitRequested(); // virtual from BWindow/BLooper
	void Quit(); // virtual from BWindow/BLooper

// entry function for the game logic
	static int32 _Game(void*p) {return ((BingControlWindow*)p)->Game();}
	int32 Game();

// the thread_id and the sync semaphore for the game logic
	thread_id tid;
	sem_id semlock;

// pointers to the small windows
	BingFloatingWindow *theball,*leftbat,*rightbat;
	BingDigit *leftdigit10,*leftdigit,*rightdigit10,*rightdigit;
};

#endif
