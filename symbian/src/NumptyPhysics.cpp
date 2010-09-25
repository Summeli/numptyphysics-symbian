/* NumptyPhysics Symbian port
 *
 * Copyright (C) 2010 Summeli <summeli@summeli.fi>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */


#include <eikapp.h>
#include <e32base.h>
#include <sdlapp.h>
#include <stddef.h>
#include <string.h>
#include <eikappui.h>
#include <eikenv.h>
//#include <reent.h>
#include <bautils.h>
#include <stdlib.h>
#include <sdl.h>

#include "os.h"
#include "OsSymbian.h"
#include "ProximityButton.h"
#include "RemoteControlKeys.h"


static char KDataDir[256] = "e:\\resource\\apps\\numptyphysics\\";

char* g_ResourceDir = NULL;
char* g_SharedResourceDir = NULL;
_LIT(KSHAREDRESOURCEPATH, "\\data\\numptyphysics" );
_LIT(KRESOURCEPATH, "\\private\\E3E84F3E" );

static int KFilePathLength = 256; //doesn't really matter

struct SymbianDirent {
	TUint32 d_fileno;		/* file number of entry */
	TUint16 d_reclen;		/* length of this record */
	TUint8  d_type; 		/* file type, see below */
	TUint8  d_namlen;		/* length of string in d_name */
	char	d_name[255 + 1];	/* name must be no longer than this */
};

//from forum.nokia http://wiki.forum.nokia.com/index.php/How_to_Convert_TBuf_to_Char_and_Vice_Versa
char* descriptorToStringL(const TDesC& aDescriptor)
{
    TInt length = aDescriptor.Length();
 
    HBufC8* buffer = HBufC8::NewLC(length);
    buffer->Des().Copy(aDescriptor);
 
    char* str = new(ELeave) char[length + 1];
    Mem::Copy(str, buffer->Ptr(), length);
    str[length] = '\0';
 
    CleanupStack::PopAndDestroy(buffer);
 
    return str;
}

/*
 * File types
 */
#define	DT_UNKNOWN	 0
#define	DT_FIFO		 1
#define	DT_CHR		 2
#define	DT_DIR		 4
#define	DT_BLK		 6
#define	DT_REG		 8
#define	DT_LNK		10
#define	DT_SOCK		12
#define	DT_WHT		14

class CNumptyApp: public CSDLApp {
public:
	CNumptyApp();
   	~CNumptyApp();

	TUid AppDllUid() const;

	/**
	 * This has a default empty implementation.
	 * Is called just before SDL_Main is called to allow init of system vars
	 */
	void PreInitializeAppL()
	{
		KDataDir[0] = BitmapStoreName()[0];
	}
	
	static void createAppPath();
	static void createSharedAppPath();
	
private:
	ProximityButton* proxbutton;
	RemoteControlKeys* remKeys;
};

CApaApplication* NewApplication() {
	// Return pointer to newly created CQMApp
	return new CNumptyApp();
}

#include <eikstart.h>
// E32Main() contains the program's start up code, the entry point for an EXE.
GLDEF_C TInt E32Main() {
 	return EikStart::RunApplication(NewApplication);
}

CNumptyApp::CNumptyApp() {
   g_ResourceDir = NULL;
   g_SharedResourceDir = NULL;
   proxbutton = ProximityButton::NewL();
   remKeys = new (ELeave) RemoteControlKeys();
}

CNumptyApp::~CNumptyApp() {
	delete g_ResourceDir;
	delete proxbutton;
	delete remKeys;
}

/**
 *   Responsible for returning the unique UID of this application
 * @return unique UID for this application in a TUid
 **/
TUid CNumptyApp::AppDllUid() const {
	return TUid::Uid(0xE3E84F3E);
}

void CNumptyApp::createAppPath()
{
	//construct the resource path at start up	
   TFileName appname = CEikonEnv::Static()->EikAppUi()->Application()->AppFullName();

   TParse parse;

   //On WINS the application is on the z drive
#ifdef __WINS__
   parse.Set(_L("c:"), &appname, NULL);
#else
   parse.Set( appname, NULL, NULL);
#endif

   TFileName drive;   // contains application's path

   drive = parse.Drive();
	
   HBufC* resPath = HBufC::NewLC( KFilePathLength);
   TPtr ptr = resPath->Des();
   ptr.Copy( drive );
   ptr.Append( KRESOURCEPATH );
   g_ResourceDir = descriptorToStringL( ptr );
   CleanupStack::PopAndDestroy();

}
void CNumptyApp::createSharedAppPath()
	{
	//construct the resource path at start up	
   TFileName appname = CEikonEnv::Static()->EikAppUi()->Application()->AppFullName();

   TParse parse;

   //On WINS the application is on the z drive
#ifdef __WINS__
   parse.Set(_L("c:"), &appname, NULL);
#else
   parse.Set( appname, NULL, NULL);
#endif

   TFileName drive;   // contains application's path

   drive = parse.Drive();
	
   HBufC* resPath = HBufC::NewLC( KFilePathLength);
   TPtr ptr = resPath->Des();
   ptr.Copy( drive );
   ptr.Append( KSHAREDRESOURCEPATH );
   g_SharedResourceDir = descriptorToStringL( ptr );
   CleanupStack::PopAndDestroy();
	}
extern int npmain(int argc, char** argv);

class OsSymbian32 : public Os
{

 public:

  OsSymbian32()
  {

  }

  ~OsSymbian32()
  {
  }

  virtual bool openBrowser( const char* url )
  {
    //??
    return false;
  }
};



Os* Os::get()
{
  static OsSymbian32 os;
  return &os;
}

const char Os::pathSep = '\\';

int SDL_main(int argc, char** argv)
{	
   npmain(argc,argv);	
}

char* getResourcePath()
	{
	if( g_ResourceDir == NULL )
		CNumptyApp::createAppPath();
	return g_ResourceDir;
	}

char* getSharedResourcePath()
	{
	if( g_SharedResourceDir == NULL )
		CNumptyApp::createSharedAppPath();
	return g_SharedResourceDir;
	}

int symbianDebugScreenPrintf (const char *fmt,...)
	{
	//TODO: put printf's into a file in debug mode
	}
