#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <libgen.h>
#include <gdk/gdkkeysyms.h>


/******************************************************************************

Modifictions:  Modified from professor Weiss code for text file opening.

*******************************************************************************/

/**
Precondition:   Takes a pointer to main program window (for open file dialog).
                Takes a pointer to a C string for file filter and mime.
Postcondition:  Returns path of file selected by user.
*/
char* get_file_name(GtkWidget *window,
                                      char* filter_name,
                                      char* mime_type);
