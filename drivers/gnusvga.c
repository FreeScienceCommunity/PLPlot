/* $Id$
   $Log$
   Revision 1.9  1993/07/01 21:59:33  mjl
   Changed all plplot source files to include plplotP.h (private) rather than
   plplot.h.  Rationalized namespace -- all externally-visible plplot functions
   now start with "pl"; device driver functions start with "plD_".

 * Revision 1.8  1993/03/15  21:39:06  mjl
 * Changed all _clear/_page driver functions to the names _eop/_bop, to be
 * more representative of what's actually going on.
 *
 * Revision 1.7  1993/03/03  19:41:55  mjl
 * Changed PLSHORT -> short everywhere; now all device coordinates are expected
 * to fit into a 16 bit address space (reasonable, and good for performance).
 *
 * Revision 1.6  1993/02/27  04:46:32  mjl
 * Fixed errors in ordering of header file inclusion.  "plplotP.h" should
 * always be included first.
 *
 * Revision 1.5  1993/02/22  23:10:51  mjl
 * Eliminated the plP_adv() driver calls, as these were made obsolete by
 * recent changes to plmeta and plrender.  Also eliminated page clear commands
 * from plP_tidy() -- plend now calls plP_clr() and plP_tidy() explicitly.
 *
 * Revision 1.4  1993/01/23  05:41:40  mjl
 * Changes to support new color model, polylines, and event handler support
 * (interactive devices only).
 *
 * Revision 1.3  1992/11/07  07:48:38  mjl
 * Fixed orientation operation in several files and standardized certain startup
 * operations. Fixed bugs in various drivers.
 *
 * Revision 1.2  1992/09/29  04:44:40  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:32:33  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*
	gnusvga.c
	Geoffrey Furnish
	20 March 1992
	
	This file constitutes the driver for an SVGA display under DOS
	using the GNU CC compiler and grpahics library by DJ Delorie.

	There are some things to note:
	1)  DJ Delorie's DOS port of GNU CC 386 uses a VCPI DOS extender.
		This means, among other things, that it won't run under
		any flavor of OS/2.  A DPMI port was rumored to be
		underway, so it is possible that by the time you read
		this, this restriction will have been lifted.
	2)  The currently available video drivers only support Super VGA
		type display cards, and only in resolutions with 256
		colors.  Thus, if you only have regular VGA, you're gonna
		get 320 x 200 resolution.  On the other hand, if you do
		have a card with say, 800 x 600 x 256 colors, then you're
		gonna get a picture much better than what could be delivered
		by the standard DOS version of PLPLOT using Micrsoft
		graphics.
	3)  This driver supports the PLPLOT RGB escape function.  So, you
		can draw lines of any color you like, which is again
		superior to the standard DOS version based on MS graphics.
*/
#ifdef GNUSVGA			/* Only compile for DOS 386 with GNU CC
				   compiler */
#include "plplotP.h"
#include <stdio.h>
#include "drivers.h"
#include <graphics.h>

/* Prototypes:	Since GNU CC, we can rest in the safety of ANSI prototyping. */

static void pause(void);
static void init_palette(void);

static PLINT vgax = 639;
static PLINT vgay = 479;

/* A flag to tell us whether we are in text or graphics mode */

#define TEXT_MODE 0
#define GRAPHICS_MODE 1

/* gmf; should probably query this on start up... Maybe later. */

static int mode = TEXT_MODE;
static int col = 1;
static int totcol = 16;

#define CLEAN 0
#define DIRTY 1

static page_state;

/* (dev) will get passed in eventually, so this looks weird right now */

static PLDev device;
static PLDev *dev = &device;

typedef struct {
    int r;
    int g;
    int b;
} RGB;

static RGB colors[] = {
    {0, 0, 0},			/* coral */
    {255, 0, 0},		/* red */
    {255, 255, 0},		/* yellow */
    {0, 255, 0},		/* green */
    {127, 255, 212},		/* acquamarine */
    {255, 192, 203},		/* pink */
    {245, 222, 179},		/* wheat */
    {190, 190, 190},		/* grey */
    {165, 42, 42},		/* brown */
    {0, 0, 255},		/* blue */
    {138, 43, 226},		/* Blue Violet */
    {0, 255, 255},		/* cyan */
    {64, 224, 208},		/* turquoise */
    {255, 0, 255},		/* magenta */
    {250, 128, 114},		/* salmon */
    {255, 255, 255},		/* white */
    {0, 0, 0},			/* black */
};


/*----------------------------------------------------------------------*\
* splD_init_vga()
*
* Initialize device.
\*----------------------------------------------------------------------*/

void
splD_init_vga(PLStream *pls)
{
    pls->termin = 1;		/* is an interactive terminal */
    pls->icol0 = 1;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;
    pls->graphx = TEXT_MODE;

    if (!pls->colorset)
	pls->color = 1;

/* Set up device parameters */

    splD_graph_vga(pls);		/* Can't get current device info unless in
				   graphics mode. */

    vgax = GrSizeX() - 1;	/* should I use -1 or not??? */
    vgay = GrSizeY() - 1;

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xmin = 0;
    dev->xmax = vgax;
    dev->ymin = 0;
    dev->ymax = vgay;

    plP_setpxl(2.5, 2.5);		/* My best guess.  Seems to work okay. */

    plP_setphy(0, vgax, 0, vgay);
}

/*----------------------------------------------------------------------*\
* splD_line_vga()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
splD_line_vga(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;

    if (pls->pscale)
	plSclPhy(pls, dev, &x1, &y1, &x2, &y2);

    y1 = vgay - y1;
    y2 = vgay - y2;

    GrLine(x1, y1, x2, y2, col);

    page_state = DIRTY;
}

/*----------------------------------------------------------------------*\
* splD_polyline_vga()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
splD_polyline_vga(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;

    for (i = 0; i < npts - 1; i++)
	splD_line_vga(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}

/*----------------------------------------------------------------------*\
* splD_eop_vga()
*
* End of page.
\*----------------------------------------------------------------------*/

void
splD_eop_vga(PLStream *pls)
{
    if (page_state == DIRTY)
	pause();

    GrSetMode(GR_default_graphics);
    init_palette();

    page_state = CLEAN;
}

/*----------------------------------------------------------------------*\
* splD_bop_vga()
*
* Set up for the next page.
* Advance to next family file if necessary (file output).
\*----------------------------------------------------------------------*/

void
splD_bop_vga(PLStream *pls)
{
    pls->page++;
    splD_eop_vga(pls);
}

/*----------------------------------------------------------------------*\
* splD_tidy_vga()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void
splD_tidy_vga(PLStream *pls)
{
    splD_text_vga(pls);
    pls->page = 0;
    pls->OutFile = NULL;
}

/*----------------------------------------------------------------------*\
* splD_color_vga()
*
* Set pen color.
\*----------------------------------------------------------------------*/

void
splD_color_vga(PLStream *pls)
{
    col = pls->icol0;
}

/*----------------------------------------------------------------------*\
* splD_text_vga()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void
splD_text_vga(PLStream *pls)
{
    if (pls->graphx == GRAPHICS_MODE) {
	if (page_state == DIRTY)
	    pause();
	GrSetMode(GR_default_text);
	pls->graphx = TEXT_MODE;
    }
}

/*----------------------------------------------------------------------*\
* splD_graph_vga()
*
* Switch to graphics mode.
*
* NOTE:  Moving to a new page causes the RGB map to be reset so that
* there will continue to be room.  This could conceivably cause a problem
* if an RGB escape was used to start drawing in a new color, and then
* it was expected that this would persist accross a page break.  If
* this happens, it will be necessary to rethink the logic of how this
* is handled.  Could wrap the index, for example.  This would require
* saving the RGB info used so far, which is not currently done.
\*----------------------------------------------------------------------*/

void
splD_graph_vga(PLStream *pls)
{
    if (pls->graphx == TEXT_MODE) {
	GrSetMode(GR_default_graphics);	/* Destroys the palette */
	init_palette();		/* Fix the palette */
	totcol = 16;		/* Reset RGB map so we don't run out of
				   indicies */
	pls->graphx = GRAPHICS_MODE;
	page_state = CLEAN;
    }
}

/*----------------------------------------------------------------------*\
* splD_width_vga()
*
* Set pen width.
\*----------------------------------------------------------------------*/

void
splD_width_vga(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* splD_esc_vga()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
splD_esc_vga(PLStream *pls, PLINT op, void *ptr)
{
    switch (op) {
	case PL_SET_RGB:{
	    pleRGB *cols = (pleRGB *) ptr;
	    int r = 255 * cols->red;
	    int g = 255 * cols->green;
	    int b = 255 * cols->blue;
	    if (totcol < 255) {
		GrSetColor(++totcol, r, g, b);
		col = totcol;
	    }
	}
	break;
    }
}

/*----------------------------------------------------------------------*\
* pause()
*
* Wait for a keystroke.
\*----------------------------------------------------------------------*/

static void
pause(void)
{
    GrTextXY(0, 0, "Pause->", 15, 0);
    getkey();
}

/*----------------------------------------------------------------------*\
* init_palette()
*
* Set up the nice RGB default color map.
\*----------------------------------------------------------------------*/

static void 
init_palette(void)
{
    int n;

    for (n = 0; n < sizeof(colors) / sizeof(RGB); n++)
	GrSetColor(n, colors[n].r, colors[n].g, colors[n].b);
}

#else
int 
pldummy_gnusvga()
{
    return 0;
}

#endif				/* GNUSVGA */
