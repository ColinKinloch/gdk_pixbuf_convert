#include <glib.h>
#include <gio/gio.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

static gint width = -1;
static gint height = -1;
static gboolean par = FALSE;
static gchar *format = "png";
static gchar *output = NULL;
static gchar **inputs = NULL;

GOptionEntry entries[] = {
  { "width", 'w', 0, G_OPTION_ARG_INT, &width, 0, "WIDTH" },
  { "height", 0, 0, G_OPTION_ARG_INT, &height, 0, "HEIGHT" },
  { "preserve-aspect-ratio", 'r', 0, G_OPTION_ARG_NONE, &par, 0, 0 },
  { "format", 'f', 0, G_OPTION_ARG_STRING, &format, 0, "FORMAT" },
  { "output", 'o', 0, G_OPTION_ARG_FILENAME, &output, 0, "OUTPUT"},
  { G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_FILENAME_ARRAY, &inputs, 0, "INPUT"},
  { NULL },
};

int main(int argc, char *argv[]) {
  GError *error = NULL;
  GOptionContext *context;
  GFile *in_file, *out_file;
  GdkPixbuf *pixbuf;
  
  context = g_option_context_new (
    "- see gdk_pixbuf_new_from_file_at_scale and gdk_pixbuf_save");
  g_option_context_add_main_entries (context, entries, NULL);
  
  if (!g_option_context_parse (context, &argc, &argv, &error))
    goto error;
  if (!(inputs && *inputs) || !output) {
    g_print ("Input or output missing\n"); 
    exit (1);
  }
  in_file = g_file_new_for_commandline_arg (*inputs);
  out_file = g_file_new_for_commandline_arg (output);
  g_print ("convert \"%s\" to \"%s\"\n", g_file_get_path (in_file), g_file_get_path (out_file));
  pixbuf = gdk_pixbuf_new_from_file_at_scale (g_file_get_path (in_file),
      width, height, par, &error);
  if (error)
    goto error;
  gdk_pixbuf_save(pixbuf, g_file_get_path (out_file), format, &error, NULL);
  if (error)
    goto error;
  g_print ("done\n");
  return 0;
error:
  g_print ("Failed with error: %s\n", error->message);
  exit (1);
}
