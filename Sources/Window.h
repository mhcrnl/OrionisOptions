#include <gtk/gtk.h>

static const char   *about          = "Orionis Options is a simple external options application for Orionis, designed to help you change settings without having to open the game first or manually edit a file.";
static const char   *name           = "Orionis | Options";
static const char   *settingsFormat = "[Settings]\nMusic Volume: %u\nSFX Volume: %u\nResolution: %u\nDisplay: %u";
static const char   *website        = "https://aerodlyn.github.io/";

static const char   *authors []     = { "Patrick Jahnig" };
static const char   *displays []    = { "Fullscreen", "Borderless", "Bordered" };
static const char   *resolutions [] = { "1366 x 768", "1600 x 900", "1920 x 1080" };

static GtkWidget    *aboutItem, *audioBox, *fileItem, *fileMenu, *graphicsBox, *menuBar, *menuBox, *optionsBox, *optionsNotebook, *quitItem, *saveButton, *window, *windowBox;
static GtkWidget    *musicRange, *sfxRange;
static GtkWidget    *musicFrame, *sfxFrame;
static GtkWidget    *resolutionChoices, *windowChoices;
static GtkWidget    *resolutionFrame, *windowFrame;

void initGUI ();

/**
 * Opens the user's default internet browser to my webpage.
 */
void loadAbout ();

/**
 * Attempts to load the settings file for Orionis.
 *      NOTE: If the file cannot be found or can't be opened then the program will attempt to create a new settings file.
 */
void loadSettings ();

/**
 * Attempts to save the current selected settings to file.
 *      NOTE: If the method cannot save to file, then it will notify the user and then exit.
 */
void saveSettings ();

/**
 * Shows the given message to the user in the form of a message dialog.
 *
 * @param message The gchar (char) pointer (i.e. string) that represents the message to show to the user
 */
void showMessage (const gchar *message);
