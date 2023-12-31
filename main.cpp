#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <sstream>

#include "Cell.h"
#include "Life.h"

int main (int argc, char *argv[])
{
    Display                 *display;
    Visual                  *visual;
    int                     depth;
    int                     text_x;
    int                     text_y;
    XSetWindowAttributes    frame_attributes;
    Window                  frame_window;
    XFontStruct             *fontinfo;
    XGCValues               gr_values;
    GC                      graphical_context;
    XEvent                  event;
    char                    hello_string[] = "Hello World";
    int                     hello_string_length = strlen(hello_string);

    display = XOpenDisplay(NULL);
    visual = DefaultVisual(display, 0);
    depth  = DefaultDepth(display, 0);

    frame_attributes.background_pixel = XWhitePixel(display, 0);
    /* create the application window */
    frame_window = XCreateWindow(display, XRootWindow(display, 0),
                                 0, 0, 400, 400, 5, depth,
                                 InputOutput, visual, CWBackPixel,
                                 &frame_attributes);
    XStoreName(display, frame_window, "The Game of Life");
    XSelectInput(display, frame_window, ExposureMask | StructureNotifyMask);

    fontinfo = XLoadQueryFont(display, "10x20");
    gr_values.font = fontinfo->fid;
    gr_values.foreground = XBlackPixel(display, 0);
    graphical_context = XCreateGC(display, frame_window, 
                                  GCFont+GCForeground, &gr_values);
    XMapWindow(display, frame_window);

    Life <ConwayCell> aLife (21, 21);

    aLife.animate (10, 5, '*');
    aLife.animate (10, 6, '*');
    aLife.animate (10, 7, '*');
    aLife.animate (10, 8, '*');
    aLife.animate (10, 9, '*');
    aLife.animate (10, 10, '*');
    aLife.animate (10, 11, '*');
    aLife.animate (10, 12, '*');
    aLife.animate (10, 13, '*');
    aLife.animate (10, 14, '*');

    std::ostringstream outStream;
    outStream << aLife;
    string aString = outStream.str ();
    const char* aChar = aString.c_str ();
    int len = outStream.str ().size ();

    while ( 1 ) {
        XNextEvent(display, (XEvent *)&event);
        switch ( event.type ) {
            case Expose:
            {
                XWindowAttributes window_attributes;
                int font_direction, font_ascent, font_descent;
                XCharStruct text_structure;
                XTextExtents(fontinfo, aChar, len, 
                             &font_direction, &font_ascent, &font_descent, 
                             &text_structure);
                XGetWindowAttributes(display, frame_window, &window_attributes);
                text_x = (window_attributes.width - text_structure.width)/2;
                text_y = (window_attributes.height - 
                          (text_structure.ascent+text_structure.descent))/2;

                outStream << aLife;

                XDrawString(display, frame_window, graphical_context,
                            text_x, text_y, aChar, len);
                break;
            }
            default:
                break;
        }
    }
    return(0);
}
