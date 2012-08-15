#include "open_file_get_buffer.h"

char* get_file_name(GtkWidget *window,
                    char* filter_name,
                    char* extension)
{

    char* file_contents_local;

    GtkFileFilter  *filter;
    GtkWidget      *dialog;
    int            result;
    gchar          *filename;
    GError         *error = NULL;
    guint          nBytesInBuf;
    gchar          *contents;

    dialog = gtk_file_chooser_dialog_new ("Select File...",
                                    ///GTK_WINDOW (window),
                                    window,
                                    GTK_FILE_CHOOSER_ACTION_OPEN,
                                    GTK_STOCK_OK,
                                    GTK_RESPONSE_ACCEPT,
                                    GTK_STOCK_CANCEL,
                                    GTK_RESPONSE_CANCEL,
                                    NULL);

    gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog),
                                         g_get_home_dir());

    ///Run the dialog modally and get the user response
    result = gtk_dialog_run (GTK_DIALOG (dialog));
    switch (result) {
    case GTK_RESPONSE_ACCEPT:
         filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
         gtk_widget_destroy (dialog);
         break;
    case GTK_RESPONSE_DELETE_EVENT:
    case GTK_RESPONSE_CANCEL:
        gtk_widget_destroy (dialog);
        return NULL;
    case GTK_RESPONSE_NONE:
         gtk_widget_destroy (dialog);
         return NULL;
    }

    ///This should not happen, but to be safe
    if (NULL == filename) {
        GtkWidget *msg;
        msg = gtk_message_dialog_new (GTK_WINDOW (window),
                                      GTK_DIALOG_MODAL,
                                      GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                                      "Failed to get file");
        gtk_dialog_run (GTK_DIALOG (msg));
        gtk_widget_destroy (msg);
        return NULL;
    }

    return(filename);
}
