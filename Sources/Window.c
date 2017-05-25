#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Window.h"

void initGUI ()
{
    /* Creates the window. */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), name);
    gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);

    /* Connects the quit method to the window being destroyed. */
    g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);

    /* Creates a window-wide horizontal container to contain each control indirectly. TODO: Rename */
    windowBox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add (GTK_CONTAINER (window), windowBox);

    /* Creates a vertical container to contain the 'save settings' button. */
    optionsBox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start (GTK_BOX (windowBox), optionsBox, FALSE, FALSE, 1);

    /* Creates the 'save settings' button and connects the appropriate method to it. */
    saveButton = gtk_button_new_with_label ("Save Settings");
    g_signal_connect (G_OBJECT (saveButton), "clicked", G_CALLBACK (saveSettings), NULL);
   
    /* Creates the 'about' button and connects the appropriate method to it. */
    aboutButton = gtk_button_new_with_label ("About");
    g_signal_connect (G_OBJECT (aboutButton), "clicked", G_CALLBACK (showAbout), NULL);

    /* Create an "unused" vertical container that expands downward so that the 'about' and 'save settings' buttons will be at the bottom of the application. A bit hacky. */
    gtk_box_pack_start (GTK_BOX (optionsBox), gtk_box_new (GTK_ORIENTATION_VERTICAL, 0), TRUE, TRUE, 1);
    
    /* Adds the 'about' and 'save settings' buttons to the left side of the window (i.e. the 'optionsBox' container) TODO: Change once renamed */
    gtk_box_pack_start (GTK_BOX (optionsBox), aboutButton, FALSE, TRUE, 1);
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
    gtk_container_set_border_width (GTK_CONTAINER (musicFrame), 5);
    gtk_box_pack_start (GTK_BOX (audioBox), musicFrame, FALSE, TRUE, 0);
    
    /* Creates the range control for music volume and adds it to 'musicFrame.' */
    musicRange = gtk_scale_new_with_range (GTK_ORIENTATION_HORIZONTAL, 0, 100, 5);
    gtk_container_add (GTK_CONTAINER (musicFrame), musicRange);

    /* Creates a frame with the title 'SFX Volume' to hold the sound effects volume control. */
    sfxFrame = gtk_frame_new ("SFX Volume");
    gtk_container_set_border_width (GTK_CONTAINER (sfxFrame), 5);
    gtk_box_pack_start (GTK_BOX (audioBox), sfxFrame, FALSE, TRUE, 0);

    /* Creates the range control for sound effects volume and adds it to 'sfxFrame.' */
    sfxRange = gtk_scale_new_with_range (GTK_ORIENTATION_HORIZONTAL, 0, 100, 5);
    gtk_container_add (GTK_CONTAINER (sfxFrame), sfxRange);

    /* Creates the vertical container to hold the graphics setting controls. */
    graphicsBox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_notebook_append_page (GTK_NOTEBOOK (optionsNotebook), graphicsBox, gtk_label_new ("Graphics"));

    /* Creates a frame with the title 'Resolution' to hold the screen resolution control. */
    resolutionFrame = gtk_frame_new ("Resolution");
    gtk_container_set_border_width (GTK_CONTAINER (resolutionFrame), 5);
    gtk_box_pack_start (GTK_BOX (graphicsBox), resolutionFrame, FALSE, TRUE, 0);

    /* Creates a combo box widget that holds all of the screen resolution options, and adds it to 'resolutionFrame.'*/
    resolutionChoices = gtk_combo_box_text_new ();

    for (size_t i = 0; i < sizeof (resolutions) / sizeof (resolutions [0]); i++)
        gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT (resolutionChoices), NULL, resolutions [i]);

    gtk_container_add (GTK_CONTAINER (resolutionFrame), resolutionChoices);

    /* Creates a frame with the title 'Window Mode' to hold the window display mode control. */
    windowFrame = gtk_frame_new ("Window Mode");
    gtk_container_set_border_width (GTK_CONTAINER (windowFrame), 5);
    gtk_box_pack_start (GTK_BOX (graphicsBox), windowFrame, FALSE, TRUE, 0);

    /* Creates a combo box widget that holds all of the window display mode options, and adds it to 'windowFrame.' */
    windowChoices = gtk_combo_box_text_new ();

    for (size_t i = 0; i < sizeof (displays) / sizeof (displays [0]); i++)
        gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT (windowChoices), NULL, displays [i]);
    
    gtk_container_add (GTK_CONTAINER (windowFrame), windowChoices);

    vsyncFrame = gtk_frame_new ("VSync");
    gtk_container_set_border_width (GTK_CONTAINER (vsyncFrame), 5);
    gtk_box_pack_start (GTK_BOX (graphicsBox), vsyncFrame, FALSE, TRUE, 0);

    vsyncChoice = gtk_check_button_new_with_label ("On/Off");
    gtk_container_add (GTK_CONTAINER (vsyncFrame), vsyncChoice);
     
    gtk_widget_show_all (window);
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
                gtk_combo_box_set_active (GTK_COMBO_BOX (resolutionChoices), atoi (line + 12));

            if (strstr (line, "Display") != NULL)
                gtk_combo_box_set_active (GTK_COMBO_BOX (windowChoices), atoi (line + 9));

            if (strstr (line, "VSync") != NULL)
                gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (vsyncChoice), atoi (line + 7));
        }

        fclose (file);
    }
    
    /* NOTE: fclose () is not needed (directly) below as it will be called in saveSettings () on the same file, or the program will exit (gracefully). */
    else
    {
        showMessage ("'Settings.properties' couldn't be found. A new file was created.");
        saveSettings ();
    }
}

/**
 * Shows a dialog with some information about the program.
 */
void showAbout ()
{
    GtkWidget *dialog = gtk_about_dialog_new ();
    
    gtk_about_dialog_set_program_name (GTK_ABOUT_DIALOG (dialog), name);
    gtk_about_dialog_set_website (GTK_ABOUT_DIALOG (dialog), website);
    gtk_about_dialog_set_comments (GTK_ABOUT_DIALOG (dialog), about);
    gtk_about_dialog_set_authors (GTK_ABOUT_DIALOG (dialog), authors);

    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
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
        int display     = (int) gtk_combo_box_get_active (GTK_COMBO_BOX (windowChoices));
        int music       = (int) gtk_range_get_value (GTK_RANGE (musicRange));
        int resolution  = (int) gtk_combo_box_get_active (GTK_COMBO_BOX (resolutionChoices));
        int sfx         = (int) gtk_range_get_value (GTK_RANGE (sfxRange));
        int vsync       = (int) gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (vsyncChoice));

        if (display < 0)
            display = 0;

        if (resolution < 0)
            resolution = 0;

        fprintf (file, settingsFormat, music, sfx, resolution, display, vsync);
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
