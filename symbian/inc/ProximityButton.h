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

#ifndef PROXIMITYBUTTON_H_
#define PROXIMITYBUTTON_H_

#include <e32base.h>
#include <sensrvchannel.h> 
#include <sensrvdatalistener.h> 
	

#include <sensrvproximitysensor.h>

class ProximityButton: public CBase, public MSensrvDataListener 
    {
public:
	ProximityButton( );
	static ProximityButton* NewL(  );
	~ProximityButton();

    void ConstructL( );
public:
    void requestMoreData();
    
public:
    void DataReceived( CSensrvChannel& aChannel, TInt aCount, TInt aDataLost );
    void DataError( CSensrvChannel& aChannel, TSensrvErrorSeverity aError );
    void GetDataListenerInterfaceL( TUid aInterfaceUid, TAny*& aInterface);
    
    
private:
    void PostKeyEvent( TBool aKeyDown );
private:
    CSensrvChannel* iSensrvChannel;
    TSensrvProximityData::TProximityState iLastState;
    CPeriodic* iSensorTimer;
    TBool dataRequested;
    };

#endif /* PROXIMITYBUTTON_H_ */
