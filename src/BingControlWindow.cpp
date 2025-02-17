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

#include "BingControlWindow.h"
#include "BingFloatingWindow.h"
#include "BingDigit.h"

#include <Application.h>
#include <stdlib.h>
#include <cstdio>

// the game settings

static const rgb_color thecolor={255,64,0,0};

static const float batxsize=0.02,batysize=0.2;
static const float batoffset=0.02;
static const float ballxsize=0.021,ballysize=0.028;

static const float batyextra=0.01;

static const float batyspeed=0.0125;
static const float ballinitspeed=0.02;
static const float ballspeedinc=0.0005;

static const bigtime_t timeslice=33333;
static const int freezetime=50;

///////////////////////////////////////////////////////////////////////////////
// function : BingControlWindow::BingControlWindow
//
// initialize the base class
// construct all the floating windows
// spawn the game logic thread
//

BingControlWindow::BingControlWindow()
			:BWindow(BRect(20,40,100,60),"Bing",B_TITLED_WINDOW_LOOK,B_NORMAL_WINDOW_FEEL,B_NOT_RESIZABLE|B_NOT_ZOOMABLE) {
	leftdigit10=new BingDigit(BRect(0.2,0.05,0.28,0.2),0.015,thecolor);
	leftdigit=new BingDigit(BRect(0.3,0.05,0.38,0.2),0.015,thecolor);
	rightdigit10=new BingDigit(BRect(0.62,0.05,0.7,0.2),0.015,thecolor);
	rightdigit=new BingDigit(BRect(0.72,0.05,0.8,0.2),0.015,thecolor);
	for (int i=0;i<30;i++) {
		(new BingFloatingWindow(BRect(0.5-0.006,(2*i+1)/60.-0.008,0.5+0.006,(2*i+1)/60.+0.008),thecolor))->SetTitle("Net");
	}
	theball=new BingFloatingWindow(BRect(0.6,0.6,0.6+ballxsize,0.6+ballysize),thecolor);
	leftbat=new BingFloatingWindow(BRect(batoffset,0.5-batysize/2,batoffset+batxsize,0.5+batysize/2),thecolor);
	rightbat=new BingFloatingWindow(BRect(1-batoffset-batxsize,0.5-batysize/2,1-batoffset,0.5+batysize/2),thecolor);
	semlock=create_sem(0,"Bing Lock");
	resume_thread(tid=spawn_thread(_Game,"Bing game",B_URGENT_DISPLAY_PRIORITY,this));
}

///////////////////////////////////////////////////////////////////////////////
// function : BingControlWindow::ScreenChanged
// virtual from : BWindow
// parameters : new screen frame (BRect)
//				new screen depth (color_space) (ignored)
//
// resize the floating windows (windows do not seem to get ScreenChanged
// messages when hidden). This code needs some big cleanup
//

void BingControlWindow::ScreenChanged(BRect r,color_space) {
	BWindow* window;
	int i=0;
	while ((window=be_app->WindowAt(i++))) {
		if (window!=this) {
			((BingFloatingWindow*)window)->NewScreen(r);
		}
	}
}

void BingControlWindow::WindowActivated(bool a) {
	if (a) {
		release_sem(semlock);
	} else {
		acquire_sem(semlock);
	}
}

///////////////////////////////////////////////////////////////////////////////
// function : BingControlWindow::QuitRequested
// virtual from : BWindow, BLooper
// return value : whether the window agrees to quit or not (always true)
//
// tell the application to quit (in case the user close the window)
//

bool BingControlWindow::QuitRequested() {
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// function : BingControlWindow::Quit
// virtual from : BWindow, BLooper
//
// kill the game thread
// call the base version (very important)
//

void BingControlWindow::Quit() {
	kill_thread(tid);
	BWindow::Quit();
}

///////////////////////////////////////////////////////////////////////////////
// function : BingControlWindow::Game
// return value : never returns
//
// the game engine. should be divided into small parts.
//
extern int margc;
extern char **margv;
int32 BingControlWindow::Game() {
	srand(system_time());
	bigtime_t targettime;
	key_info ki;
	float yleftbat=0.5-batysize/2,yrightbat=yleftbat;
	float newyleftbat=yleftbat,newyrightbat=yrightbat;
	float ballspeed=ballinitspeed;
	float ballxspeed=-0.7*ballspeed,ballyspeed=0.7*ballspeed;
	float ballx=1-batxsize-batoffset-ballxsize;
	float bally=0.5-ballysize/2;
	int leftscore=0,rightscore=0;
	int timenomove=-1;
	theball->FMoveTo(BPoint(ballx,bally));
	int mov;
	targettime=system_time()+timeslice;
	enum
	{
		GAME_MODE_DEFAULT = 0,
		GAME_MODE_PLAYER_VS_PLAYER = 0,
		GAME_MODE_PLAYER_VS_COMPUTER,
		GAME_MODE_COMPUTER_VS_COMPUTER,
		GAME_MODE__count,
	};
	int gamemode;

	const char *gamemode_names[GAME_MODE__count] = {
		"PLAYER_VS_PLAYER",
		"PLAYER_VS_COMPUTER",
		"COMPUTER_VS_COMPUTER",
	};

	{
		int n = -1;
		if (margc == 3) {
			if ((strcmp(margv[1], "-g") == 0) || (strcmp(margv[1], "--gamemode") == 0)) {
				n = atoi(margv[2]);
			}
		}

		if (n < GAME_MODE__count && n >= 0) {
			printf("Selecting gamemode %s\n", gamemode_names[n]);
			gamemode = n;
		}
		else
		{
			printf("No gamemode specified\nSelecting gamemode %s\nUsage: %s -g NUM\nNUM can be from 0 to %d\n",
					gamemode_names[GAME_MODE_DEFAULT], margv[0], GAME_MODE__count - 1);
			gamemode = GAME_MODE_DEFAULT;
		}
	}


	while(true) {
		acquire_sem(semlock);
		get_key_info(&ki);
		mov=0;

		switch (gamemode) {
			case GAME_MODE_PLAYER_VS_PLAYER: /*fallthrough*/
			case GAME_MODE_PLAYER_VS_COMPUTER:
				if (ki.key_states[7]&8) {
					mov--;
				}
				if (ki.key_states[9]&8) {
					mov++;
				}
			break;
			case GAME_MODE_COMPUTER_VS_COMPUTER:
				if (timenomove > 0)
					break;
				if (ballxspeed >= 0)
					break;
				if (yleftbat > bally) {
					mov--;
				}
				if ((yleftbat + batysize) < bally) {
					mov++;
				}
			break;
			default:
			/*Not Handled*/
			break;
		}

		if (mov==1) {
			newyleftbat=yleftbat+batyspeed;
			if (newyleftbat>1-batysize) {
				newyleftbat=1-batysize;
			}
		}			
		if (mov==-1) {
			newyleftbat=yleftbat-batyspeed;
			if (newyleftbat<0) {
				newyleftbat=0;
			}
		}

		mov=0;

		switch (gamemode) {
			case GAME_MODE_PLAYER_VS_PLAYER:
				if (ki.key_states[10]&1) {
					mov--;
				}
				if (ki.key_states[12]&32) {
					mov++;
				}
			break;
			case GAME_MODE_PLAYER_VS_COMPUTER: /*fallthrough*/
			case GAME_MODE_COMPUTER_VS_COMPUTER:
				if (timenomove > 0)
					break;
				if (ballxspeed <= 0)
					break;
				if ((yrightbat + 0.5*batysize - 0.1*batysize) > bally) {
					mov--;
				}
				if ((yrightbat + 0.5*batysize + 0.1*batysize) < bally) {
					mov++;
				}
			break;
			default:
			/*Not Handled*/
			break;
		}

		if (mov==1) {
			newyrightbat=yrightbat+batyspeed;
			if (newyrightbat>1-batysize) {
				newyrightbat=1-batysize;
			}
		}			
		if (mov==-1) {
			newyrightbat=yrightbat-batyspeed;
			if (newyrightbat<0) {
				newyrightbat=0;
			}
		}

		if (timenomove>0) {
			timenomove--;
		} else if (timenomove==-1) {
			if (ki.key_states[11]&2) {
				timenomove=0;
				leftscore=rightscore=0;
				leftdigit10->Set(-1);
				leftdigit->Set(0);
				rightdigit10->Set(-1);
				rightdigit->Set(0);
			}
		} else {
			if ((ballx>batoffset+batxsize)&&(ballx+ballxspeed<=batoffset+batxsize)) {
				float propmvt=(batoffset+batxsize-ballx)/ballxspeed;
				float yballcross=bally+ballyspeed*propmvt;
				float ybatcross=yleftbat+propmvt*(newyleftbat-yleftbat);
				if (yballcross<0) {
					yballcross=-yballcross;
				} else if (yballcross+ballysize>1) {
					yballcross=(2-2*ballysize)-yballcross;
				}
				if ((yballcross+ballysize>ybatcross-batyextra)&&(yballcross<ybatcross+batysize+batyextra)) {
					ballx=batoffset+batxsize;
					bally=yballcross;
					float angle=1.2*(2*(yballcross-(ybatcross-ballysize))/(batysize+ballysize)-1);
					ballxspeed=ballspeed*cos(angle);
					ballyspeed=ballspeed*sin(angle);
					ballx+=(1-propmvt)*ballxspeed;
					bally+=(1-propmvt)*ballyspeed;
					ballspeed+=ballspeedinc;
					goto balldone;
				}
			}

			if ((ballx+ballxsize<1-batoffset-batxsize)&&(ballx+ballxsize+ballxspeed>=1-batoffset-batxsize)) {
				float propmvt=(1-(batoffset+batxsize)-(ballx+ballxsize))/ballxspeed;
				float yballcross=bally+ballyspeed*propmvt;
				float ybatcross=yrightbat+propmvt*(newyrightbat-yrightbat);
				if (yballcross<0) {
					yballcross=-yballcross;
				} else if (yballcross+ballysize>1) {
					yballcross=(2-2*ballysize)-yballcross;
				}
				if ((yballcross+ballysize>ybatcross-batyextra)&&(yballcross<ybatcross+batysize+batyextra)) {
					ballx=1-batoffset-batxsize-ballxsize;
					bally=yballcross;
					float angle=1.2*(2*(yballcross-(ybatcross-ballysize))/(batysize+ballysize)-1);
					ballxspeed=ballspeed*-cos(angle);
					ballyspeed=ballspeed*sin(angle);
					ballx+=(1-propmvt)*ballxspeed;
					bally+=(1-propmvt)*ballyspeed;
					ballspeed+=ballspeedinc;
					goto balldone;
				}
			}
	
			ballx+=ballxspeed;
			if (ballx<0) {
				ballx=batoffset+batxsize;
				bally=0.5-ballysize/2;
				ballspeed=ballinitspeed;
				float angle=(2.*rand())/RAND_MAX-1;
				ballxspeed=ballspeed*cos(angle);
				ballyspeed=ballspeed*sin(angle);
				rightscore++;
				rightdigit->Set(rightscore%10);
				if (rightscore==10) {
					rightdigit10->Set(1);
				}
				if (rightscore==11) {
					timenomove=-1;
				} else {
					timenomove=freezetime;
				}
			} else if (ballx+ballxsize>1) {
				ballx=1-batoffset-batxsize-ballxsize;
				bally=0.5-ballysize/2;
				ballspeed=ballinitspeed;
				float angle=(2.*rand())/RAND_MAX-1;
				ballxspeed=-ballspeed*cos(angle);
				ballyspeed=ballspeed*sin(angle);
				leftscore++;
				leftdigit->Set(leftscore%10);
				if (leftscore==10) {
					leftdigit10->Set(1);
				}
				if (leftscore==11) {
					timenomove=-1;
				} else {
					timenomove=freezetime;
				}
			}
	
	
			bally+=ballyspeed;
			if (bally<0) {
				bally=-bally;
				ballyspeed=-ballyspeed;
			} else if (bally+ballysize>1) {
				bally=(2-2*ballysize)-bally;
				ballyspeed=-ballyspeed;
			}
		}
balldone:

		if (newyleftbat!=yleftbat) {
			leftbat->FMoveTo(BPoint(batoffset,yleftbat=newyleftbat));
		}
		if (newyrightbat!=yrightbat) {
			rightbat->FMoveTo(BPoint(1-batoffset-batxsize,yrightbat=newyrightbat));
		}

		theball->FMoveTo(BPoint(ballx,bally));

		bigtime_t timenow=system_time();
		if (timenow<targettime) {
			snooze(targettime-timenow);
			targettime+=timeslice;
		} else {
			targettime=timenow+timeslice;
		}
		release_sem(semlock);
	}
}
