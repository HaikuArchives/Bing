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

#include "BingFloatingWindow.h"

#include <Screen.h>
#include <View.h>

///////////////////////////////////////////////////////////////////////////////
// function : BingFloatingWindow::BingFloatingWindow
// parameters : initial coordinates (in the 0-1 range)
//				initial color
//
// initalize the base class
// self-resize according to the screen size
// add a full-window view and set its color
//

BingFloatingWindow::BingFloatingWindow(BRect f,rgb_color c):BWindow(BRect(0,0,0,0),"Floating Window",
			B_NO_BORDER_WINDOW_LOOK,B_FLOATING_APP_WINDOW_FEEL,B_NOT_RESIZABLE|B_NOT_ZOOMABLE|B_NOT_CLOSABLE) {
	FFrame=f;
	SFrame=BScreen(this).Frame();
	MoveTo(SFrame.left+FFrame.left*(SFrame.Width()+1),SFrame.top+FFrame.top*(SFrame.Height()+1));
	ResizeTo(FFrame.Width()*SFrame.Width(),FFrame.Height()*SFrame.Height());
	BView* v=new BView(Bounds(),"floater_back_view",B_FOLLOW_ALL_SIDES,0);
	v->SetViewColor(c);
	AddChild(v);
	Show();
}

///////////////////////////////////////////////////////////////////////////////
// function : BingFloatingWindow::FMoveTo
// parameters : new top-left coordinates (0-1 range)
//
// move the window.
//

void BingFloatingWindow::FMoveTo(BPoint p) {
	FFrame.right=p.x+FFrame.Width();
	FFrame.bottom=p.y+FFrame.Height();
	FFrame.left=p.x;
	FFrame.top=p.y;
	MoveTo(SFrame.left+FFrame.left*(SFrame.Width()+1),SFrame.top+FFrame.top*(SFrame.Height()+1));
}

///////////////////////////////////////////////////////////////////////////////
// function : BingFloatingWindow::FResizeTo
// parameters : new size (0-1 range)
//
// resize the window.
//

void BingFloatingWindow::FResizeTo(BPoint p) {
	FFrame.right=FFrame.left+p.x;
	FFrame.bottom=FFrame.top+p.y;
	ResizeTo(FFrame.Width()*SFrame.Width(),FFrame.Height()*SFrame.Height());
}

///////////////////////////////////////////////////////////////////////////////
// function : BingFloatingWindow::NewScreen
// parameters : new screen size
//
// take the new screen size into account. cannot use ScreenChanged, because
// it is not called for a floater window.
//

void BingFloatingWindow::NewScreen(BRect r) {
	SFrame=r;
	MoveTo(SFrame.left+FFrame.left*(SFrame.Width()+1),SFrame.top+FFrame.top*(SFrame.Height()+1));
	ResizeTo(FFrame.Width()*SFrame.Width(),FFrame.Height()*SFrame.Height());
}
