/* Numptyphysics symbian port
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


#include "RemoteControlKeys.h"

#include <SDL_stdinc.h>
#include <SDL_events.h>

#define KTimeOut 200000

static TInt VolDownCallBack(TAny* ptr);
static TInt VolUpCallBack(TAny* ptr);

RemoteControlKeys::RemoteControlKeys()
	: CActive ( EPriorityNormal )
	{
	//TODO: two phased constructor
	CActiveScheduler::Add( this );
    iInterfaceSelector = CRemConInterfaceSelector::NewL();
    iRemConCore = CRemConCoreApiTarget::NewL(*iInterfaceSelector, *this);
    iInterfaceSelector->OpenTargetL(); 
    
    iVolUptimer =  CPeriodic::NewL( EPriorityNormal ); 
    iVolDowntimer =  CPeriodic::NewL( EPriorityNormal ); 
	}

RemoteControlKeys::~RemoteControlKeys()
	{
	// TODO Auto-generated destructor stub
    delete iInterfaceSelector;
    iRemConCore = NULL; //owned by interfaceselector
	Cancel();
	iResponseQ.Reset();
	iResponseQ.Close();
	}
/*
 * it seems that it takes about 600ms to get an update after buttonpress
 * */
void RemoteControlKeys::MrccatoCommand(TRemConCoreApiOperationId aOperationId, 
	TRemConCoreApiButtonAction aButtonAct)
	{
	TRequestStatus status;
       switch( aOperationId )
       {
       case ERemConCoreApiVolumeUp:
           {   
           switch (aButtonAct)
               {
           	   case ERemConCoreApiButtonPress:
           		  PostVolUpKeyEvent( ETrue );
				  break;
			   case ERemConCoreApiButtonRelease:
				   PostVolUpKeyEvent( EFalse );
				  break;
			   case ERemConCoreApiButtonClick:
				   if( !iVolUptimer->IsActive() )
					   {
					   PostVolUpKeyEvent( ETrue );
					   //with button click also start a timer
					   iVolUptimer->Start( KTimeOut, KTimeOut, 
						   TCallBack( VolUpCallBack, static_cast<RemoteControlKeys*>(this)) );
					   }
				   break;
			   default:      
				break; 
               }
           break;
           }       
       case ERemConCoreApiVolumeDown:
           {
           switch (aButtonAct)
               {
          	   case ERemConCoreApiButtonPress:
          		 PostVolDownKeyEvent( ETrue );
				  break;
			   case ERemConCoreApiButtonRelease:
				   PostVolDownKeyEvent( EFalse );
				  break;
			   case ERemConCoreApiButtonClick:
				   if( !iVolDowntimer->IsActive() )
					   {
					   PostVolDownKeyEvent( ETrue );
					   //with button click also start a timer
					   iVolDowntimer->Start( KTimeOut, KTimeOut, 
						   TCallBack( VolDownCallBack, static_cast<RemoteControlKeys*>(this)) );
					   }
			   default:      
				break; 
               }
           break;
           }  
 
       default:
           break;
       }

       CompleteMediaKeyEvent( aOperationId );
	}

void RemoteControlKeys::MrccatoPlay(TRemConCoreApiPlaybackSpeed aSpeed, 
	TRemConCoreApiButtonAction aButtonAct)
	{
	
	}

void RemoteControlKeys::MrccatoTuneFunction(TBool aTwoPart, 
	TUint aMajorChannel, 
	TUint aMinorChannel,
	TRemConCoreApiButtonAction aButtonAct)
	{
	
	}

void RemoteControlKeys::MrccatoSelectDiskFunction(TUint aDisk,
	TRemConCoreApiButtonAction aButtonAct)
	{
	
	}

void RemoteControlKeys::MrccatoSelectAvInputFunction(TUint8 aAvInputSignalNumber,
	TRemConCoreApiButtonAction aButtonAct)
	{
	
	}

void RemoteControlKeys::MrccatoSelectAudioInputFunction(TUint8 aAudioInputSignalNumber,
	TRemConCoreApiButtonAction aButtonAct)
	{
	
	}

void RemoteControlKeys::CompleteMediaKeyEvent( TRemConCoreApiOperationId aOperationId )
	{
	if	( !IsActive() )
		{
		switch ( aOperationId )
			{
			case ERemConCoreApiVolumeUp:
				{
				iRemConCore->VolumeUpResponse( iStatus, KErrNone );
				SetActive();
				}
				break;
	
			case ERemConCoreApiVolumeDown:
				{
				iRemConCore->VolumeDownResponse( iStatus, KErrNone );	
				SetActive();
				}
				break;
			case ERemConCoreApiPlay:
				{
				iRemConCore-> PlayResponse(iStatus, KErrNone);
				SetActive();
				}
				break;
			case ERemConCoreApiStop:
				{
				iRemConCore->StopResponse(iStatus, KErrNone);
				SetActive();
				}
				break;
			
			case ERemConCoreApiPause:
				{
				iRemConCore->PauseResponse(iStatus, KErrNone);
				SetActive();
				}
				break;
			case ERemConCoreApiRewind:
				{
				iRemConCore->RewindResponse(iStatus, KErrNone);
				SetActive();
				}
				break;
			case ERemConCoreApiFastForward:
				{
				iRemConCore->FastForwardResponse(iStatus, KErrNone);
				SetActive();
				}
				break;
			case ERemConCoreApiForward:
				{
				iRemConCore->ForwardResponse( iStatus, KErrNone );
				SetActive();
				}
				break;
			case ERemConCoreApiBackward:
				{
				iRemConCore->BackwardResponse(iStatus, KErrNone );
				SetActive();
				}
				break;
			default:
				{
				}
				break;
			}
		}
	else
		{
		//active, append to queue
		iResponseQ.Append( aOperationId );
		}
	}

void RemoteControlKeys::RunL()
	{
	 if ( iResponseQ.Count() )
		{
		CompleteMediaKeyEvent( iResponseQ[0] );
		//remove old response from que
		iResponseQ.Remove(0);
		iResponseQ.Compress();
		}
	}

void RemoteControlKeys::DoCancel()
	{
	}

void RemoteControlKeys::voluptimerexpired()
	{
	PostVolUpKeyEvent( EFalse );
	iVolUptimer->Cancel();
	}

void RemoteControlKeys::voldowntimerexpired()
	{
	PostVolDownKeyEvent( EFalse );
	iVolDowntimer->Cancel();
	}

void RemoteControlKeys::PostVolDownKeyEvent( TBool aKeyDown )
	{
    SDL_Event event;
	SDL_memset(&event, 0, sizeof(event));
    event.key.type = aKeyDown ? SDL_KEYDOWN : SDL_KEYUP;	
    event.key.state = aKeyDown ? SDL_PRESSED : SDL_RELEASED;
    event.key.keysym.sym = SDLK_r;
    event.key.keysym.unicode = 114;
    SDL_PushEvent(&event);
	}

void RemoteControlKeys::PostVolUpKeyEvent( TBool aKeyDown )
	{
    SDL_Event event;
	SDL_memset(&event, 0, sizeof(event));
    event.key.type = aKeyDown ? SDL_KEYDOWN : SDL_KEYUP;	
    event.key.state = aKeyDown ? SDL_PRESSED : SDL_RELEASED;
    //event.key.keysym.sym = SDLK_ESCAPE;
    //event.key.keysym.unicode = 27;
    event.key.keysym.sym = SDLK_m;
    event.key.keysym.unicode = 109;
    SDL_PushEvent(&event);
	}

static TInt VolDownCallBack(TAny* ptr)
	{
	RemoteControlKeys* keys =static_cast<RemoteControlKeys*>( ptr );
	keys->voldowntimerexpired();
	}

static TInt VolUpCallBack(TAny* ptr)
	{
	RemoteControlKeys* keys =static_cast<RemoteControlKeys*>( ptr );
	keys->voluptimerexpired();
	}

