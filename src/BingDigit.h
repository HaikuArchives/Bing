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

#ifndef BINGDIGIT_H_
#define BINGDIGIT_H_

#include "BingFloatingWindow.h"

///////////////////////////////////////////////////////////////////////////////
// class : BingDigit
//
// a seven segment display, made of seven window
// 

class BingDigit {
public:
	BingDigit(BRect frame,float thickness,rgb_color);
	void Set(int);
private:
	BingFloatingWindow* segment[7];
};

#endif
