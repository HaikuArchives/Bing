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

#include "BingApplication.h"
#include "BingControlWindow.h"

///////////////////////////////////////////////////////////////////////////////
// function : main
// return value : program return code (always 0)
//
// create the application object on the stack, and run it
//
int margc;
char **margv;
int main(int argc, char **argv) {
	margc = argc;
	margv = argv;
	BingApplication().Run();
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// function : BingApplication::BingApplication
//
// initalize the base class
//

BingApplication::BingApplication():BApplication("application/x-vnd.djaybee.Bing") {
}

///////////////////////////////////////////////////////////////////////////////
// function : BingApplication::ReadyToRun
// virtual from : BApplication, BLooper
//
// create the main window, and show it
//

void BingApplication::ReadyToRun() {
	(new BingControlWindow)->Show();
}
