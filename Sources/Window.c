#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Window.h"

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

    /* Creates a frame with the title 'Resolution' to hold the screen resolution control. */
    resolutionFrame = gtk_frame_new ("Resolution");
    gtk_box_pack_start (GTK_BOX (graphicsBox), resolutionFrame, FALSE, TRUE, 5);

    /* Creates a combo box widget that holds all of the screen resolution options, and adds it to 'resolutionFrame.' TODO: Store options in array. */
    resolutionChoices = gtk_combo_box_text_new ();
    gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT (resolutionChoices), "1366 x 768", "1366 x 768");
    gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT (resolutionChoices), "1600 x 900", "1600 x 900");
    gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT (resolutionChoices), "1920 x 1080", "1920 x 1080");
    gtk_container_add (GTK_CONTAINER (resolutionFrame), resolutionChoices);

    /* Creates a frame with the title 'Window Mode' to hold the window display mode control. */
    windowFrame = gtk_frame_new ("Window Mode");
    gtk_box_pack_start (GTK_BOX (graphicsBox), windowFrame, FALSE, TRUE, 5);

    /* Creates a combo box widget that holds all of the window display mode options, and adds it to 'windowFrame.' */
    windowChoices = gtk_combo_box_text_new ();
    gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT (windowChoices), NULL, "Fullscreen");
    gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT (windowChoices), NULL, "Borderless");
    gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT (windowChoices), NULL, "Bordered");
    gtk_container_add (GTK_CONTAINER (windowFrame), windowChoices);

    gtk_widget_show_all (window);
}

/**
 * Opens the user's default internet browser to my webpage.
 */
void loadAbout ()
{
    GtkWidget *dialog = gtk_about_dialog_new ();

    const gchar *authors[] = {"Patrick Jahnig"};

    gtk_about_dialog_set_website (GTK_ABOUT_DIALOG (dialog), "https://aerodlyn.github.io/");
    gtk_about_dialog_set_comments (GTK_ABOUT_DIALOG (dialog), "Orionis Options is a simple external options application for Orionis, designed to help you change settings without having to open the game first or manually edit a file.");
    gtk_about_dialog_set_authors (GTK_ABOUT_DIALOG (dialog), authors);

    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

/**
 * Attempts to load the settings file for Orionis.
 *      NOTE: If the file cannot be found or can't be opened then the program will attempt to create a new settings file.
 */
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

            if (strstr (line, "Resolution") != NULL)
                gtk_combo_box_set_active (GTK_COMBO_BOX (resolutionChoices), 1/*line + 12*/);
        }

        fclose (file);
    }

    else
    {
        showMessage ("'Settings.properties' couldn't be found. A new file was created.");
        saveSettings ();
    }
}

/**
 * Attempts to save the current selected settings to file.
 *      NOTE: If the method cannot save to file, then it will notify the user and then exit.
 */
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
        int music   = (int) gtk_range_get_value (GTK_RANGE (musicRange));
        int sfx     = (int) gtk_range_get_value (GTK_RANGE (sfxRange));
   
        fprintf (file, "[Settings]\nSFX Volume: %u\nMusic Volume: %u\nResolution: %u\nDisplay: %u", sfx, music, 1, 0/*resolution, display*/);
        fclose (file);
        
        showMessage ("Settings saved successfully.");
    }
}

/**
 * Shows the given message to the user in the form of a message dialog.
 *
 * @param message The gchar (char) pointer (i.e. string) that represents the message to show to the user
 */
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
