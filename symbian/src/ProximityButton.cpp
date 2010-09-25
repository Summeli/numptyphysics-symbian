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

#include "ProximityButton.h"

#include <sensrvchannelfinder.h> 
#include <e32keys.h>
#include <SDL_stdinc.h>
#include <SDL_events.h>

static TInt SensorCallBack(TAny* ptr);

#define KCallBackInterval 200000
#define KCallBackDelayFromStart 1000000

ProximityButton::ProximityButton( )
    {
	iLastState = TSensrvProximityData::EProximityIndiscernible;
	dataRequested = EFalse;
    }
ProximityButton::~ProximityButton()
    {
    }

ProximityButton* ProximityButton::NewL( )
{
	ProximityButton* self = new (ELeave) ProximityButton( );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
}
 
void ProximityButton::ConstructL( )
    {
	CSensrvChannelFinder* SensrvChannelFinder = CSensrvChannelFinder::NewLC(); 
	RSensrvChannelInfoList ChannelInfoList;
	CleanupClosePushL( ChannelInfoList );
	 
	TSensrvChannelInfo mySearchConditions; // none, so matches all.
	mySearchConditions.iChannelDataTypeId = KSensrvChannelTypeIdProximityMonitor;
	SensrvChannelFinder->FindChannelsL(ChannelInfoList,mySearchConditions);
	 
	 // do something with the ChannelInfoList
	iSensrvChannel = CSensrvChannel::NewL( ChannelInfoList[0] );
	iSensrvChannel->OpenChannelL();
	 
	ChannelInfoList.Close();
	CleanupStack::Pop( &ChannelInfoList );
	CleanupStack::PopAndDestroy( SensrvChannelFinder );
	
	iSensorTimer = CPeriodic::NewL( EPriorityNormal ); 
	iSensorTimer->Start( KCallBackDelayFromStart, KCallBackInterval, TCallBack( SensorCallBack, static_cast<ProximityButton*>(this)) );
    }

void ProximityButton::DataReceived( CSensrvChannel& aChannel, TInt aCount, TInt aDataLost )
	{
    if ( aChannel.GetChannelInfo().iChannelType ==  KSensrvChannelTypeIdProximityMonitor )
    	{	
    	TSensrvProximityData data;
    	for( TInt i = 0; i < aCount; i++ )
    		{
    		TPckgBuf<TSensrvProximityData> dataBuf;
    		aChannel.GetData( dataBuf );
    		data = dataBuf();
    		TSensrvProximityData::TProximityState state = data.iProximityState;
    		if ( iLastState != state )
    			{
    			if( state == TSensrvProximityData::EProximityIndiscernible )
    				{
    				//button up
    				PostKeyEvent( EFalse );
					}
    			if( state == TSensrvProximityData::EProximityDiscernible )
    				{
    				//button down
    				PostKeyEvent( ETrue );
    				}
    			}
    		iLastState = state;
    		}
    	}
    dataRequested = EFalse;
	}

void ProximityButton::DataError( CSensrvChannel& aChannel, TSensrvErrorSeverity aError )
	{
	TInt error = aError;
	}

void ProximityButton::GetDataListenerInterfaceL( TUid aInterfaceUid, TAny*& aInterface)
	{
	TInt error;
	}
 
void ProximityButton::requestMoreData()
	{
	if( !dataRequested )
		{
		iSensrvChannel->StartDataListeningL( this, 1,1,0);
		dataRequested = ETrue;
		}
	}

static TInt SensorCallBack(TAny* ptr)
	{
	//ask for data
	ProximityButton* prox =static_cast<ProximityButton*>( ptr );
	prox->requestMoreData();
	}

void ProximityButton::PostKeyEvent( TBool aKeyDown )
	{
    SDL_Event event;
	SDL_memset(&event, 0, sizeof(event));
    event.key.type = aKeyDown ? SDL_KEYDOWN : SDL_KEYUP;	
    event.key.state = aKeyDown ? SDL_PRESSED : SDL_RELEASED;
    event.key.keysym.sym = SDLK_ESCAPE;
    event.key.keysym.unicode = 27;
   // event.key.keysym.sym = SDLK_m;
   // event.key.keysym.unicode = 109;
    SDL_PushEvent(&event);
	}
