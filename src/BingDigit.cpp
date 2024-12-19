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

#include "BingDigit.h"

///////////////////////////////////////////////////////////////////////////////
// function : BingDigit::BingDigit
//
// construct the seven windows.
//

BingDigit::BingDigit(BRect r,float t,rgb_color c) {
	float x=r.left;
	float y=r.top;
	float dx=r.Width()-2*t;
	float dy=(r.Height()-3*t)/2;
	float u=t*.75;
	segment[0]=new BingFloatingWindow(BRect(x,y,x+dx+u,y+t),c);
	segment[1]=new BingFloatingWindow(BRect(x,y,x+u,y+dy+t),c);
	segment[2]=new BingFloatingWindow(BRect(x+dx,y,x+dx+u,y+dy+t),c);
	segment[3]=new BingFloatingWindow(BRect(x,y+dy,x+dx+u,y+dy+t),c);
	segment[4]=new BingFloatingWindow(BRect(x,y+dy,x+u,y+2*dy+t),c);
	segment[5]=new BingFloatingWindow(BRect(x+dx,y+dy,x+dx+u,y+2*dy+t),c);
	segment[6]=new BingFloatingWindow(BRect(x,y+2*dy,x+dx+u,y+2*dy+t),c);
}

///////////////////////////////////////////////////////////////////////////////
// function : BingDigit::Set
// parameters : the new value for the digit. -1 means invisible
//
// changes the value displayed by the digit by hiding/showing the right window
//

void BingDigit::Set(int v) {
	for (int i=0;i<7;i++) {
		segment[i]->Lock();
	}
	switch(v) {
		case 0: {
			if (segment[0]->IsHidden()) segment[0]->Show();
			if (segment[1]->IsHidden()) segment[1]->Show();
			if (segment[2]->IsHidden()) segment[2]->Show();
			if (!segment[3]->IsHidden()) segment[3]->Hide();
			if (segment[4]->IsHidden()) segment[4]->Show();
			if (segment[5]->IsHidden()) segment[5]->Show();
			if (segment[6]->IsHidden()) segment[6]->Show();
			break;
		}
		case 1: {
			if (!segment[0]->IsHidden()) segment[0]->Hide();
			if (!segment[1]->IsHidden()) segment[1]->Hide();
			if (segment[2]->IsHidden()) segment[2]->Show();
			if (!segment[3]->IsHidden()) segment[3]->Hide();
			if (!segment[4]->IsHidden()) segment[4]->Hide();
			if (segment[5]->IsHidden()) segment[5]->Show();
			if (!segment[6]->IsHidden()) segment[6]->Hide();
			break;
		}
		case 2: {
			if (segment[0]->IsHidden()) segment[0]->Show();
			if (!segment[1]->IsHidden()) segment[1]->Hide();
			if (segment[2]->IsHidden()) segment[2]->Show();
			if (segment[3]->IsHidden()) segment[3]->Show();
			if (segment[4]->IsHidden()) segment[4]->Show();
			if (!segment[5]->IsHidden()) segment[5]->Hide();
			if (segment[6]->IsHidden()) segment[6]->Show();
			break;
		}
		case 3: {
			if (segment[0]->IsHidden()) segment[0]->Show();
			if (!segment[1]->IsHidden()) segment[1]->Hide();
			if (segment[2]->IsHidden()) segment[2]->Show();
			if (segment[3]->IsHidden()) segment[3]->Show();
			if (!segment[4]->IsHidden()) segment[4]->Hide();
			if (segment[5]->IsHidden()) segment[5]->Show();
			if (segment[6]->IsHidden()) segment[6]->Show();
			break;
		}
		case 4: {
			if (!segment[0]->IsHidden()) segment[0]->Hide();
			if (segment[1]->IsHidden()) segment[1]->Show();
			if (segment[2]->IsHidden()) segment[2]->Show();
			if (segment[3]->IsHidden()) segment[3]->Show();
			if (!segment[4]->IsHidden()) segment[4]->Hide();
			if (segment[5]->IsHidden()) segment[5]->Show();
			if (!segment[6]->IsHidden()) segment[6]->Hide();
			break;
		}
		case 5: {
			if (segment[0]->IsHidden()) segment[0]->Show();
			if (segment[1]->IsHidden()) segment[1]->Show();
			if (!segment[2]->IsHidden()) segment[2]->Hide();
			if (segment[3]->IsHidden()) segment[3]->Show();
			if (!segment[4]->IsHidden()) segment[4]->Hide();
			if (segment[5]->IsHidden()) segment[5]->Show();
			if (segment[6]->IsHidden()) segment[6]->Show();
			break;
		}
		case 6: {
			if (segment[0]->IsHidden()) segment[0]->Show();
			if (segment[1]->IsHidden()) segment[1]->Show();
			if (!segment[2]->IsHidden()) segment[2]->Hide();
			if (segment[3]->IsHidden()) segment[3]->Show();
			if (segment[4]->IsHidden()) segment[4]->Show();
			if (segment[5]->IsHidden()) segment[5]->Show();
			if (segment[6]->IsHidden()) segment[6]->Show();
			break;
		}
		case 7: {
			if (segment[0]->IsHidden()) segment[0]->Show();
			if (!segment[1]->IsHidden()) segment[1]->Hide();
			if (segment[2]->IsHidden()) segment[2]->Show();
			if (!segment[3]->IsHidden()) segment[3]->Hide();
			if (!segment[4]->IsHidden()) segment[4]->Hide();
			if (segment[5]->IsHidden()) segment[5]->Show();
			if (!segment[6]->IsHidden()) segment[6]->Hide();
			break;
		}
		case 8: {
			if (segment[0]->IsHidden()) segment[0]->Show();
			if (segment[1]->IsHidden()) segment[1]->Show();
			if (segment[2]->IsHidden()) segment[2]->Show();
			if (segment[3]->IsHidden()) segment[3]->Show();
			if (segment[4]->IsHidden()) segment[4]->Show();
			if (segment[5]->IsHidden()) segment[5]->Show();
			if (segment[6]->IsHidden()) segment[6]->Show();
			break;
		}
		case 9: {
			if (segment[0]->IsHidden()) segment[0]->Show();
			if (segment[1]->IsHidden()) segment[1]->Show();
			if (segment[2]->IsHidden()) segment[2]->Show();
			if (segment[3]->IsHidden()) segment[3]->Show();
			if (!segment[4]->IsHidden()) segment[4]->Hide();
			if (segment[5]->IsHidden()) segment[5]->Show();
			if (segment[6]->IsHidden()) segment[6]->Show();
			break;
		}
		case -1: {
			if (!segment[0]->IsHidden()) segment[0]->Hide();
			if (!segment[1]->IsHidden()) segment[1]->Hide();
			if (!segment[2]->IsHidden()) segment[2]->Hide();
			if (!segment[3]->IsHidden()) segment[3]->Hide();
			if (!segment[4]->IsHidden()) segment[4]->Hide();
			if (!segment[5]->IsHidden()) segment[5]->Hide();
			if (!segment[6]->IsHidden()) segment[6]->Hide();
			break;
		}
	}
	for (int i=0;i<7;i++) {
		segment[i]->Unlock();
	}
}
