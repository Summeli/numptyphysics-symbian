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

#ifndef REMOTECONTROLKEYS_H_
#define REMOTECONTROLKEYS_H_

#include <e32base.h>

#include <remconcoreapitargetobserver.h>    // link against RemConCoreApi.lib
#include <remconcoreapitarget.h>            // and
#include <remconinterfaceselector.h>        // RemConInterfaceBase.lib

class RemoteControlKeys: public CActive, public MRemConCoreApiTargetObserver
	{
public:
	RemoteControlKeys();
	virtual ~RemoteControlKeys();
	
	
public: //From MRemConCoreApiTargetObserver
	void MrccatoCommand(TRemConCoreApiOperationId aOperationId, 
		TRemConCoreApiButtonAction aButtonAct);
	
	void MrccatoPlay(TRemConCoreApiPlaybackSpeed aSpeed, 
		TRemConCoreApiButtonAction aButtonAct);

	void MrccatoTuneFunction(TBool aTwoPart, 
		TUint aMajorChannel, 
		TUint aMinorChannel,
		TRemConCoreApiButtonAction aButtonAct);

	void MrccatoSelectDiskFunction(TUint aDisk,
		TRemConCoreApiButtonAction aButtonAct);

	void MrccatoSelectAvInputFunction(TUint8 aAvInputSignalNumber,
		TRemConCoreApiButtonAction aButtonAct);

	void MrccatoSelectAudioInputFunction(TUint8 aAudioInputSignalNumber,
		TRemConCoreApiButtonAction aButtonAct);
    
private:
    void CompleteMediaKeyEvent( TRemConCoreApiOperationId aOperationId );
    void RunL();
    void DoCancel();
    
public:
	void voluptimerexpired();
	void voldowntimerexpired();
	
private:
	void PostVolDownKeyEvent( TBool aKeyDown ); 
	void PostVolUpKeyEvent( TBool aKeyDown ); 
private:

	RArray<TRemConCoreApiOperationId> iResponseQ; //response queue

	CRemConCoreApiTarget* iRemConCore; //the controller
    CRemConInterfaceSelector* iInterfaceSelector;
    
    CPeriodic* iVolUptimer;
    CPeriodic* iVolDowntimer;
	};

#endif /* RemoteControlKeys_H_ */
