#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* NOTE: Respecting header order is important for portability.
 * Always put system first, then EFL, then your public header,
 * and finally your private one. */

#include <Ecore_Getopt.h>
#include <Elementary.h>

#include "gettext.h"

#include "eflife_private.h"

#define COPYRIGHT "Copyright © 2016 Andy Williams <andy@andywilliams.me> and various contributors (see AUTHORS)."

static void
_eflife_win_del(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   elm_exit();
}

static Eina_Bool
eflife_tick(void *data)
{
   Evas_Object *win = data;

   eflife_life_nextgen();
   eflife_life_render(win);

   return ECORE_CALLBACK_RENEW;
}

static Evas_Object *
eflife_win_setup(void)
{
   Evas_Object *win;
   Evas_Coord w = 320, h = 320;

   win = elm_win_util_standard_add("main", "EFLife");
   if (!win) return NULL;

   elm_win_focus_highlight_enabled_set(win, EINA_TRUE);
   evas_object_smart_callback_add(win, "delete,request", _eflife_win_del, NULL);
   evas_object_resize(win, w, h);

   eflife_life_nextgen();
   eflife_life_render(win);

   evas_object_show(win);

   return win;
}

static const Ecore_Getopt optdesc = {
  "eflife",
  "%prog [options]",
  PACKAGE_VERSION,
  COPYRIGHT,
  "BSD with advertisement clause",
  "An EFL Game of Life program",
  0,
  {
    ECORE_GETOPT_LICENSE('L', "license"),
    ECORE_GETOPT_COPYRIGHT('C', "copyright"),
    ECORE_GETOPT_VERSION('V', "version"),
    ECORE_GETOPT_HELP('h', "help"),
    ECORE_GETOPT_SENTINEL
  }
};

EAPI_MAIN int
elm_main(int argc EINA_UNUSED, char **argv EINA_UNUSED)
{
   Evas_Object *win;
   int args;
   Eina_Bool quit_option = EINA_FALSE;

   Ecore_Getopt_Value values[] = {
     ECORE_GETOPT_VALUE_BOOL(quit_option),
     ECORE_GETOPT_VALUE_BOOL(quit_option),
     ECORE_GETOPT_VALUE_BOOL(quit_option),
     ECORE_GETOPT_VALUE_BOOL(quit_option),
     ECORE_GETOPT_VALUE_NONE
   };

#if ENABLE_NLS
   setlocale(LC_ALL, "");
   bindtextdomain(PACKAGE, LOCALEDIR);
   bind_textdomain_codeset(PACKAGE, "UTF-8");
   textdomain(PACKAGE);
#endif

   args = ecore_getopt_parse(&optdesc, values, argc, argv);
   if (args < 0)
     {
	EINA_LOG_CRIT("Could not parse arguments.");
	goto end;
     }
   else if (quit_option)
     {
	goto end;
     }

   elm_app_info_set(elm_main, "eflife", "images/eflife.png");

   if (!(win = eflife_win_setup()))
     goto end;

   ecore_timer_add(0.25, eflife_tick, win);
   elm_run();

 end:
   elm_shutdown();

   return 0;
}
ELM_MAIN()
