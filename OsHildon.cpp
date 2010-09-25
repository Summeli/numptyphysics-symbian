/*
 * This file is part of NumptyPhysics
 * Copyright (C) 2008 Tim Edmonds
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */

#ifdef USE_HILDON

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <glib-object.h>
#include <glibconfig.h>
#include <glib/gmacros.h>
#include <libosso.h>

#include <gtk/gtk.h>

#include "Os.h"
#include "Http.h"
#include "Ui.h"
#include "Accelerometer.h"
#define Font __FONT_REDEF
#include "Config.h"

#define NP_NAME       "NumptyPhysics"
#define NP_SERVICE    "org.maemo.garage.numptyphysics"
#define NP_OBJECT     "org/maemo/garage/numptyphysics" /* / ?? */
#define NP_INTERFACE  "org.maemo.garage.numptyphysics"
#define NP_VERSION    "1.0"
#define MAX_FILES 32


static gint mime_handler(const gchar *interface,
                         const gchar *method,
                         GArray *arguments,
                         gpointer data,
                         osso_rpc_t *retval);


class HildonModEventMap : public EventMap
{
  EventMap* m_map;
  int m_mod;
public:
  HildonModEventMap(EventMap *m) : m_map(m), m_mod(0) {}

  virtual Event process(const SDL_Event& ev1)
  {
    SDL_Event ev = ev1;
    // \todo we should do a real event map instead of just modding
    // button clicks...
    switch( ev.type ) {      
    case SDL_KEYDOWN:
      if ( ev.key.keysym.sym == SDLK_F8 ) {
	m_mod = 1;  //zoom- == middle (delete)
	return Event();
      } else if ( ev.key.keysym.sym == SDLK_F7 ) {
	m_mod = 2;  //zoom+ == right (move)
	return Event();
      }
      break;
    case SDL_KEYUP:
      if ( ev.key.keysym.sym == SDLK_F7
	   || ev.key.keysym.sym == SDLK_F8 ) {
	m_mod = 0;     
	return Event();
      }
      break;
    case SDL_MOUSEBUTTONDOWN: 
    case SDL_MOUSEBUTTONUP: 
      if ( ev.button.button == SDL_BUTTON_LEFT ) {
	if ( m_mod == 1 ) {
	  ev.button.button = SDL_BUTTON_MIDDLE;
	} else if ( m_mod == 2 ) {
	  ev.button.button = SDL_BUTTON_RIGHT;
	}
      }
      break;
    default:
      break;
    }
    return m_map->process(ev);
  }
};




class OsHildon : public Os, private Accelerometer
{
  GMainContext   *m_gcontext;
  osso_context_t *m_osso;
  bool            m_useProximity;
  bool            m_proximityClosed;

 public:
  int             m_numFiles;
  char*           m_files[MAX_FILES];

  OsHildon()
    : m_useProximity(false),
      m_proximityClosed(false)
  {
    g_type_init();
    m_gcontext = g_main_context_new();

    m_osso = osso_initialize(NP_NAME, NP_VERSION, FALSE, m_gcontext);
    if (m_osso == NULL) {
      fprintf(stderr, "Failed to initialize libosso\n");
      return;
    }
    
    /* Set dbus handler to get mime open callbacks */
    if ( osso_rpc_set_cb_f(m_osso,
			   NP_SERVICE,
			   NP_OBJECT,
			   NP_INTERFACE,
			   mime_handler, NULL) != OSSO_OK) {
      fprintf(stderr, "Failed to set mime callback\n");
    }

#if MAEMO_VERSION >= 5
    m_useProximity = true;
#endif
  }

  ~OsHildon()
  {
    if ( m_osso ) {
      osso_deinitialize( m_osso );
    }
    if ( m_gcontext ) {
      g_main_context_unref( m_gcontext );
    }
  }

  virtual Accelerometer* getAccelerometer()
  {
    return this;
  }

  virtual bool poll( float32& gx, float32& gy, float32& gz )
  {
    bool ok = false;
    int x,y,z;
    FILE *f = fopen("/sys/class/i2c-adapter/i2c-3/3-001d/coord", "rt");
    //FILE *f = fopen("coord", "rt");
    if ( f ) {
      if ( fscanf(f,"%d %d %d",&x,&y,&z)==3 ) {
	gx = -(float32)x / 1000.0f;
	gy = -(float32)y / 1000.0f;
	gz =  (float32)z / 1000.0f;
	ok = true;
      }
      fclose(f);
    }
    return ok;
  }

  virtual void poll() 
  {
    // run the gobject main loop for dbus ops
    if ( g_main_context_iteration( m_gcontext, FALSE ) ) {
      fprintf(stderr, "Hildon::poll event!\n");
    }
#if MAEMO_VERSION >= 5
    // poll the proximity sensor to emulate esc key (undo)
    if ( m_useProximity ) {
      int fd = open("/sys/devices/platform/gpio-switch/proximity/state", 0);
      if ( fd >= 0 ) {
	char c;
	if ( read(fd,&c,1)==1 ) {
	  bool proxState = (c=='c');
	  if (proxState != m_proximityClosed) {
	    SDL_Event event;
	    event.key.type = proxState ? SDL_KEYDOWN : SDL_KEYUP;	
	    event.key.state = proxState ? SDL_PRESSED : SDL_RELEASED;
	    event.key.keysym.sym = SDLK_ESCAPE;
	    event.key.keysym.unicode = 27;
	    SDL_PushEvent(&event);
	    m_proximityClosed = proxState;
	  }
	}
	close(fd);
      } else {
	m_useProximity = false;
      }
#endif
    }
  }

  EventMap* getEventMap( EventMapType type )
  {
    static HildonModEventMap gameMap(Os::getEventMap(GAME_MAP));
    static HildonModEventMap editMap(Os::getEventMap(EDIT_MAP));

    switch (type) {
    case GAME_MAP:
      return &gameMap;
    case EDIT_MAP:
      return &editMap;
    default:
      return Os::getEventMap(type);
    }
  }

  virtual void decorateGame( WidgetParent* game )
  {
    Button *b;
    // play options on the right
    b = new Button("",Event(Event::OPTION,1));
    b->setBg(0x408040);
    game->add( b, Rect(SCREEN_WIDTH-10,0,
SCREEN_WIDTH,SCREEN_HEIGHT) );
    // palette/edit options on the left
    b = new Button("",Event(Event::OPTION,2));
    b->setBg(0x408040);
    game->add( b, Rect(0,0,10,SCREEN_HEIGHT));
  }

  virtual char *getLaunchFile() 
  {
    if ( m_numFiles > 0 ) {
      return m_files[--m_numFiles];
    }
    return NULL;
  }

  virtual bool openBrowser( const char* url )
  {
    if ( url && strlen(url) < 200 ) {
      char buf[256];
      snprintf(buf,256,"xdg-open %s",url);
      if ( system( buf ) == 0 ) {
	return true;
      }
    }
    return false;
  }

  virtual char* saveDialog( const char* path )
  {
#if 0
    static char buf[256];
    GtkWidget *dialog = hildon_file_chooser_dialog_new(NULL,GTK_FILE_CHOOSER_ACTION_SAVE);
    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), path);
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
      gchar *name;
      name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
      strncpy( buf, name, 256 );
      g_print("Saving as %s\n", name);
      g_free(name);
    }    
    return buf;    
#else
    return NULL;
#endif
  }

};


static gint mime_handler(const gchar *interface,
                         const gchar *method,
                         GArray *arguments,
                         gpointer data,
                         osso_rpc_t *retval)
{
  OsHildon* os = (OsHildon*)data;
  if (arguments == NULL) {
    return OSSO_OK;
  }

  if (g_ascii_strcasecmp(method, "mime_open") == 0) {
    for(unsigned i = 0; i < arguments->len; ++i) {
      osso_rpc_t val = g_array_index(arguments, osso_rpc_t, i);
      if (val.type == DBUS_TYPE_STRING && val.value.s != NULL) {
	char *f = NULL;
	fprintf(stderr,"hildon mime open \"%s\"\n",val.value.s);
	if ( strncmp(val.value.s,"file://",7)==0 
	     && os->m_numFiles < MAX_FILES ) {
	  f = val.value.s+7;
	} else if ( ( strncmp(val.value.s,"http://",7)==0 
		      || strncmp(val.value.s,"nptp://",7)==0 ) ) {
	  Http h;
	  std::string filename(val.value.s+7);
	  if ( filename.rfind('/') >= 0 ) {
	    filename = filename.substr( filename.rfind('/')+1 );
	  }
	  std::string filepath = Config::userDataDir() + Os::pathSep;
	  if ( h.get( val.value.s+7, HTTP_TEMP_FILE ) ) {
	    f = HTTP_TEMP_FILE;
	  }
	}
	if ( f ) {
	  if ( os->m_files[os->m_numFiles] ) {
	    g_free(os->m_files[os->m_numFiles]);
	  }
	  os->m_files[os->m_numFiles++] = g_strdup( f );
	}
      }
    }
  }

  return OSSO_OK;
}


/* cortex-a8 runfast mode to shovel vfp isntructions into the neon
   unit.  runfast implies:
   * Subnormal numbers are being flushed to zero
   * Default NaN mode is active
   * No floating point exceptions are enabled
 */
void enable_runfast()
{
#if __ARM
  static const unsigned int x = 0x04086060;
  static const unsigned int y = 0x03000000;
  int r;
  asm volatile ("fmrx   %0, fpscr   \n\t"   //r0 = FPSCR
                "and    %0, %0, %1  \n\t"   //r0 = r0 & x
                "orr    %0, %0, %2  \n\t"   //r0 = r0 | y
                "fmxr   fpscr, %0   \n\t"   //FPSCR = r0
                : "=r"(r)
                : "r"(x), "r"(y));
#endif
}


Os* Os::get()
{
  static OsHildon os;
  return &os;
}

const char Os::pathSep = '/';

int main(int argc, char** argv)
{
#if 0
  gtk_init(&argc, &argv);
#endif
  enable_runfast();
  npmain(argc,argv);
}

#endif
