#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

static void on_destroy_window (GtkWidget* widget, GtkWidget* window) {
  gtk_main_quit();
}

static gboolean on_close_web_view (WebKitWebView* web_view, GtkWidget* window) {
  gtk_widget_destroy(window);
  return TRUE;
}

char *get_current_directory () {
  char buffer[PATH_MAX];
  getcwd(buffer, sizeof(buffer));
  char *buffer_string = (char *) calloc(1, strlen(buffer) + 1); // + 1 for the '\0' character
  strcpy(buffer_string, buffer);
  return buffer_string;
}

char *get_resources_path () {
  char buffer[PATH_MAX];
  getcwd(buffer, sizeof(buffer));
  char *buffer_string = (char *) calloc(1, strlen(buffer) + 1); // + 1 for the '\0' character
  strcpy(buffer_string, "file://");
  strcat(buffer_string, buffer);
  strcat(buffer_string, "/index.html");
  return buffer_string;
}

char *get_file_contents (const char *file_path) {
  FILE *file = fopen(file_path, "rb");
  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  fseek(file, 0, SEEK_SET);
  char *string = (char *) malloc(file_size + 1);
  fread(string, 1, file_size, file);
  fclose(file);
  string[file_size] = 0;
  return string;
}

int main (int argc, char* argv[]) {
  // Initialize GTK+
  gtk_init(&argc, &argv);

  // Create an 800x600 window that will contain the browser instance
  GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 600);

  // Developer extras
  WebKitSettings *webkit_settings = webkit_settings_new();
  webkit_settings_set_enable_developer_extras(webkit_settings, TRUE);

  // Create a browser instance
  WebKitWebView *web_view = WEBKIT_WEB_VIEW(webkit_web_view_new_with_settings(webkit_settings));

  // Put the browser area into the main window
  gtk_container_add(GTK_CONTAINER(main_window), GTK_WIDGET(web_view));

  // Set up callbacks so that if either the main window or the browser instance is
  // closed, the program will exit
  g_signal_connect(main_window, "destroy", G_CALLBACK(on_destroy_window), NULL);
  g_signal_connect(web_view, "close", G_CALLBACK(on_close_web_view), main_window);

  // Load a web page into the browser instance
  webkit_web_view_load_uri(web_view, get_resources_path());

  // Make sure that when the browser area becomes visible, it will get mouse
  // and keyboard events
  gtk_widget_grab_focus(GTK_WIDGET(web_view));

  // Make sure the main window and all its contents are visible
  gtk_widget_show_all(main_window);

  // Run the main GTK+ event loop
  gtk_main();

  return 0;
}
