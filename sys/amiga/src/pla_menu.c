/* $Id$
   $Log$
   Revision 1.2  1993/07/01 21:59:51  mjl
   Changed all plplot source files to include plplotP.h (private) rather than
   plplot.h.  Rationalized namespace -- all externally-visible plplot functions
   now start with "pl"; device driver functions start with "plD_".

 * Revision 1.1  1993/03/15  21:34:22  mjl
 * Reorganization and update of Amiga drivers.  Window driver now uses Amiga
 * OS 2.0 capabilities.
 *
*/

/*	pla_menu.c

	Functions for handling Amiga menu selections and other IDCMP events.
*/

#include "plplotP.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "drivers.h"
#include "plamiga.h"
#include "plevent.h"

extern PLStream *the_pls;

int saveiff(char *);

/*----------------------------------------------------------------------*\
* plamiga_Open()
*
* For opening a file.  User must enable by setting an open-file handler.
* See plrender.c for more detail.
\*----------------------------------------------------------------------*/

int
plamiga_Open(void)
{
    rtEZRequest ("Option currently unimplemented.\n", "OK", NULL, NULL);
    return(1);
}

/*----------------------------------------------------------------------*\
* plamiga_Save()
*
* Save to existing file.
\*----------------------------------------------------------------------*/

int
plamiga_Save(void)
{
    rtEZRequest ("Option currently unimplemented.\n", "OK", NULL, NULL);
    return(1);
}

/*----------------------------------------------------------------------*\
* plamiga_SaveAs_ILBM()
*
* Screen dump.  
\*----------------------------------------------------------------------*/

int
plamiga_SaveAs_ILBM(void)
{
    struct rtFileRequester *filereq;
    char filename[34];
    APTR windowlock;
    int status;

    if (filereq = rtAllocRequestA (RT_FILEREQ, NULL)) {
	filename[0] = 0;
	if (rtFileRequest (filereq, filename, "Enter IFF file name",
			   RT_LockWindow, 1, RTFI_Flags, FREQF_PATGAD,
			   TAG_END)) {

	    windowlock = rtLockWindow(pla->window);
	    status = saveiff(filename);
	    rtUnlockWindow(pla->window, windowlock);
	    if (status)
		rtEZRequest ("Unable to save bitmap.\n", "OK", NULL, NULL);
	}
    }

    return(1);
}

/*----------------------------------------------------------------------*\
* plamiga_Print_Bitmap()
*
* Self-explanatory.
\*----------------------------------------------------------------------*/

int
plamiga_Print_Bitmap(void)
{
    rtEZRequest ("Option currently unimplemented.\n", "OK", NULL, NULL);
    return(1);
}

/*----------------------------------------------------------------------*\
* plamiga_Print_landscape()
*
* Self-explanatory.
\*----------------------------------------------------------------------*/

int
plamiga_Print_landscape(void)
{
    rtEZRequest ("Option currently unimplemented.\n", "OK", NULL, NULL);
    return(1);
}

/*----------------------------------------------------------------------*\
* plamiga_Print_portrait()
*
* Self-explanatory.
\*----------------------------------------------------------------------*/

int
plamiga_Print_portrait(void)
{
    rtEZRequest ("Option currently unimplemented.\n", "OK", NULL, NULL);
    return(1);
}

/*----------------------------------------------------------------------*\
* plamiga_About()
*
* Self-explanatory.
\*----------------------------------------------------------------------*/

int
plamiga_About(void)
{
    rtEZRequest ("Option currently unimplemented.\n", "OK", NULL, NULL);
    return(1);
}

/*----------------------------------------------------------------------*\
* plamiga_Quit()
*
* Self-explanatory.
\*----------------------------------------------------------------------*/

int
plamiga_Quit(void)
{
    plspause(0);
    plexit("");
    return(0);
}

/*----------------------------------------------------------------------*\
* plamiga_Screenmode()
*
* Brings up ReqTools Screenmode requester.
\*----------------------------------------------------------------------*/

int
plamiga_Screenmode(void)
{
    struct rtScreenModeRequester *scrmodereq;

    if (scrmodereq = rtAllocRequestA (RT_SCREENMODEREQ, NULL)) {

	if (rtScreenModeRequest (scrmodereq, "Pick a screen mode:",
				 RT_LockWindow, 1, RTSC_Flags,
				 SCREQF_DEPTHGAD | SCREQF_SIZEGADS |
				 SCREQF_OVERSCANGAD,
				 TAG_END)) {

	    pla->scr_width	= scrmodereq->DisplayWidth;
	    pla->scr_height	= scrmodereq->DisplayHeight;

	    pla->scr_depth	= scrmodereq->DisplayDepth;
	    pla->maxcolors	= pow(2., (double) pla->scr_depth);

	    pla->scr_displayID	= scrmodereq->DisplayID;

/* Close and reopen screen, window. */

	    pla->restart = 1;
	    pla_CloseWindow();
	    pla_CloseScreen();
	    pla_SetFont();
	    pla_InitDisplay();

/* Set up plotting scale factors */

	    pla->xscale = (double) pla->cur_width / pla->init_width;
	    pla->yscale = (double) pla->cur_height / pla->init_height;

/* Redraw the plot */

	    plRemakePlot(the_pls);
	}
	rtFreeRequest (scrmodereq);
    }
    else
	rtEZRequest ("Out of memory!", "Oh boy!", NULL, NULL);

    return(1);
}

/*----------------------------------------------------------------------*\
* plamiga_Palette0()
*
* Brings up ReqTools palette requester for setting cmap0.
\*----------------------------------------------------------------------*/

int
plamiga_Palette0(void)
{
    (void) rtPaletteRequest ("Change Color Map 0", NULL, RT_LockWindow, 1,
			     TAG_END); 

    return(1);
}

/*----------------------------------------------------------------------*\
* plamiga_Palette1()
*
* Currently unimplemented.
\*----------------------------------------------------------------------*/

int
plamiga_Palette1(void)
{
    rtEZRequest ("Option currently unimplemented.\n", "OK", NULL, NULL);
    return(1);
}

/*----------------------------------------------------------------------*\
* plamiga_LoadConfig()
*
* Load configuration.
\*----------------------------------------------------------------------*/

int
plamiga_LoadConfig(void)
{
    rtEZRequest ("Option currently unimplemented.\n", "OK", NULL, NULL);
    return(1);
}

/*----------------------------------------------------------------------*\
* plamiga_SaveConfigAs()
*
* Save configuration to specified file.
\*----------------------------------------------------------------------*/

int
plamiga_SaveConfigAs(void)
{
    rtEZRequest ("Option currently unimplemented.\n", "OK", NULL, NULL);
    return(1);
}

/*----------------------------------------------------------------------*\
* plamiga_SaveConfig()
*
* Save configuration.
\*----------------------------------------------------------------------*/

int
plamiga_SaveConfig(void)
{
    rtEZRequest ("Option currently unimplemented.\n", "OK", NULL, NULL);
    return(1);
}

/*----------------------------------------------------------------------*\
* plamiga_DUMMY()
*
* User menus to go here.
\*----------------------------------------------------------------------*/

int
plamiga_DUMMY(void)
{
    rtEZRequest ("Option currently unimplemented.\n", "OK", NULL, NULL);
    return(1);
}

/*----------------------------------------------------------------------*\
* plamiga_KEY()
*
* Keypress handler.
\*----------------------------------------------------------------------*/

int
plamiga_KEY(void)
{
    PLKey key;
    int input_char;

/* Defaults */

    input_char = PlplotMsg.Code;
    key.code = 0;
    key.string[0] = '\0';

/* Translate keystroke into a PLKey */

    if (isprint(input_char)) {
	key.string[0] = input_char;
	key.string[1] = '\0';
    }
    else {
	switch (input_char) {
	case 0x08:
	    key.code = PLK_BackSpace;
	    break;

	case 0x09:
	    key.code = PLK_Tab;
	    break;

	case 0x0A:
	    key.code = PLK_Linefeed;
	    break;

	case 0x0D:
	    key.code = PLK_Return;
	    break;

	case 0x1B:
	    key.code = PLK_Escape;
	    break;

	case 0xFF:
	    key.code = PLK_Delete;
	    break;
	}
    }

#ifdef DEBUG
    printf("Keycode %x, string: %s\n", key.code, key.string);
#endif

/* Call user event handler */
/* Since this is called first, the user can disable all plplot internal
   event handling by setting key.code to 0 and key.string to '\0' */

    if (the_pls->KeyEH != NULL)
	(*the_pls->KeyEH) (&key, NULL, &pla->exit_eventloop);

/* Handle internal events */

/* Advance to next page (i.e. terminate event loop) on a <eol> */

    if (key.code == PLK_Linefeed || key.code == PLK_Return)
	pla->exit_eventloop = TRUE;

/* Terminate on a 'Q' (not 'q', since it's too easy to hit by mistake) */

    if (key.string[0] == 'Q') {
	plspause(0);
	plexit("");
    }
    return(!pla->exit_eventloop);
}
