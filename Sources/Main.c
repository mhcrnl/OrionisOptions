#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>

/* TODO: Move to header file maybe. */
static GtkWidget *aboutItem, *audioBox, *fileItem, *fileMenu, *graphicsBox, *menuBar, *menuBox, *optionsBox, *optionsNotebook, *quitItem, *saveButton, *window, *windowBox;
static GtkWidget *musicRange, *sfxRange;
static GtkWidget *musicFrame, *sfxFrame;

void initGUI (void);
void loadAbout (void);
void loadSettings (void);
void saveSettings (void);
void showMessage (const gchar *message);

void initGUI ()
{
    /* Creates the window. */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "Orionis | Options");
    gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size (GTK_WINDOW (window), 800, 600);

    /* Connects the quit method to the window being destroyed. */
    g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);

    /* Creates a window-wide vertical container to contain each control indirectly. TODO: Rename */
    menuBox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add (GTK_CONTAINER (window), menuBox);
    
    /* Creates a menu bar and adds it as the first child of the window-wide (TODO: reference by name) container. */
    menuBar = gtk_menu_bar_new ();
    gtk_box_pack_start (GTK_BOX (menuBox), menuBar, FALSE, TRUE, 0);

    /* Creates a file menu to hold menu controls pertaining to the file. */
    fileMenu = gtk_menu_new ();

    /* Creates all of the file menu controls. */
    fileItem = gtk_menu_item_new_with_label ("File");
    aboutItem = gtk_menu_item_new_with_label ("About");
    quitItem = gtk_menu_item_new_with_label ("Quit");

    /* Connects the necessary methods to the respective file menu controls. */
    g_signal_connect (G_OBJECT (aboutItem), "activate", G_CALLBACK (loadAbout), NULL);
    g_signal_connect (G_OBJECT (quitItem), "activate", G_CALLBACK (gtk_main_quit), NULL);

    /* Sets the file menu to open via click, and appends it to the menu bar. */
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (fileItem), fileMenu);
    gtk_menu_shell_append (GTK_MENU_SHELL (menuBar), fileItem);
    
    /* Appends the file menu controls to the file menu. */
    gtk_menu_shell_append (GTK_MENU_SHELL (fileMenu), aboutItem);
    gtk_menu_shell_append (GTK_MENU_SHELL (fileMenu), quitItem);

    /* Creates a horizontal container to contain the rest of the controls. TODO: Rename */
    windowBox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start (GTK_BOX (menuBox), windowBox, TRUE, TRUE, 0);

    /* Creates a vertical container to contain the 'save settings' button. */
    optionsBox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start (GTK_BOX (windowBox), optionsBox, FALSE, FALSE, 1);

    /* Creates the 'save settings' button add connects the appropriate method to it. */
    saveButton = gtk_button_new_with_label ("Save Settings");
    g_signal_connect (G_OBJECT (saveButton), "clicked", G_CALLBACK (saveSettings), NULL);
    
    /* Create an "unused" vertical container that expands downward so that the 'save settings' button will be at the bottom of the application. A bit hacky. */
    gtk_box_pack_start (GTK_BOX (optionsBox), gtk_box_new (GTK_ORIENTATION_VERTICAL, 0), TRUE, TRUE, 1);
    
    /* Adds the 'save settings' button to the left side of the window (i.e. the 'optionsBox' container) TODO: Change once renamed */
    gtk_box_pack_start (GTK_BOX (optionsBox), saveButton, FALSE, TRUE, 1);

    /* Adds a vertical line between the 'optionsBox' and 'optionsNotebook' widgets. TODO: Change once renamed. */
    gtk_box_pack_start (GTK_BOX (windowBox), gtk_separator_new (GTK_ORIENTATION_VERTICAL), FALSE, TRUE, 1);

    /* Creates a notebook container to contain all of the audio and graphics setting controls. */
    optionsNotebook = gtk_notebook_new ();
    gtk_box_pack_start (GTK_BOX (windowBox), optionsNotebook, TRUE, TRUE, 1);

    /* Creates the vertical container to hold the audio setting controls. */
    audioBox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_notebook_append_page (GTK_NOTEBOOK (optionsNotebook), audioBox, gtk_label_new ("Audio"));

    /* Creates a frame with the title 'Music Volume' to hold the music volume control. */
    musicFrame = gtk_frame_new ("Music Volume");
    gtk_box_pack_start (GTK_BOX (audioBox), musicFrame, FALSE, TRUE, 5);
    
    /* Creates the range control for music volume and adds it to 'musicFrame.' */
    musicRange = gtk_scale_new_with_range (GTK_ORIENTATION_HORIZONTAL, 0, 100, 5);
    gtk_container_add (GTK_CONTAINER (musicFrame), musicRange);

    /* Creates a frame with the title 'SFX Volume' to hold the sound effects volume control. */
    sfxFrame = gtk_frame_new ("SFX Volume");
    gtk_box_pack_start (GTK_BOX (audioBox), sfxFrame, FALSE, TRUE, 5);

    /* Creates the range control for sound effects volume and adds it to 'sfxFrame.' */
    sfxRange = gtk_scale_new_with_range (GTK_ORIENTATION_HORIZONTAL, 0, 100, 5);
    gtk_container_add (GTK_CONTAINER (sfxFrame), sfxRange);

    /* Creates the vertical container to hold the graphics setting controls. */
    graphicsBox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_notebook_append_page (GTK_NOTEBOOK (optionsNotebook), graphicsBox, gtk_label_new ("Graphics"));

    gtk_widget_show_all (window);
}

void loadAbout ()
{
    system ("xdg-open 'https://aerodlyn.github.io/'");
}

void loadSettings ()
{
    FILE *file = fopen ("Settings.properties", "r");
    char *line = NULL;
    size_t len = 0;

    if (file)
    {
        while (getline (&line, &len, file) != -1)
        {
            if (strstr (line, "SFX") != NULL)
                gtk_range_set_value (GTK_RANGE (sfxRange), atoi (line + 12));

            if (strstr (line, "Music") != NULL)
                gtk_range_set_value (GTK_RANGE (musicRange), atoi (line + 14));
        }

        fclose (file);
    }

    else
    {
        showMessage ("'Settings.properties' couldn't be found. A new file was created.");
        saveSettings ();
    }
}

void saveSettings ()
{
    FILE *file = fopen ("Settings.properties", "w");

    if (!file)
    {
        showMessage ("'Settings.properties' couldn't be opened. The error code is '0x1'.");
        exit (0x1);
    }

    else
    {
        fprintf (file, "[Settings]\nSFX Volume: %u\nMusic Volume: %u", (int) gtk_range_get_value (GTK_RANGE (sfxRange)), (int) gtk_range_get_value (GTK_RANGE (musicRange)));
        fclose (file);
        
        showMessage ("Settings saved successfully.");
    }
}

void showMessage (const gchar *message)
{
    GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW (window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, message);

    gtk_window_set_title (GTK_WINDOW (dialog), "Notice");
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

int main (int argc, char **argv)
{
    gtk_init (&argc, &argv);

    initGUI ();
    loadSettings ();

    gtk_main ();

    return 0x0;
}