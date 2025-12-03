// wav.cpp - Operations on .WAV waveform-audio files for Tools, Play Sound

#include "stdafx.h"
#include "toolbox.h"
#include "wav.h"
#include <ctype.h>  // isspace, isdigit


#undef _UNICODE // 1.4sc Turn Unicode off for play sound file
#undef UNICODE // 1.4sc

#ifndef _MAC
#include <mmsystem.h>  // MCI_
#endif

#ifndef _MAC
DWORD dwPlayWAVEFile(LPCSTR lpszWAVEFileName, HWND hwndToCall, UINT* pwIDDevice, BOOL bHasFromTo, DWORD dwFrom, DWORD dwTo, Str8& sErr)
{
    DWORD dwReturn;
    MCI_OPEN_PARMS  mciOpenParms;
    MCI_PLAY_PARMS  mciPlayParms;
    MCI_SET_PARMS   mciSetParms;

    sErr.Empty();
	
	mciSendCommand(MCI_ALL_DEVICE_ID, MCI_CLOSE, MCI_WAIT, NULL); // 1.4se Fix bug of error on play sound while playing

    // Open the device by specifying the device and filename.
    // MCI will choose a device capable of playing the given file.
    mciOpenParms.lpstrDeviceType = "waveaudio";
    mciOpenParms.lpstrElementName = lpszWAVEFileName;
    if (dwReturn = mciSendCommand(0, MCI_OPEN,
       MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)(LPVOID) &mciOpenParms))
    {
        // Failed to open device. Don't close it; just return error.
        char* pszErr = sErr.GetBuffer(256);
        mciGetErrorString(dwReturn, pszErr, 256);
        sErr.ReleaseBuffer();
        return (dwReturn);
    }

    // The device opened successfully; get the device ID.
    *pwIDDevice = mciOpenParms.wDeviceID;

    // First, set the time format.
    mciSetParms.dwTimeFormat = MCI_FORMAT_MILLISECONDS;
    if (dwReturn = mciSendCommand(*pwIDDevice, MCI_SET,
       MCI_SET_TIME_FORMAT , (DWORD)(LPVOID) &mciSetParms))
    {
        // Failed to set time format. Don't close it; just return error.
        char* pszErr = sErr.GetBuffer(256);
        mciGetErrorString(dwReturn, pszErr, 256);
        sErr.ReleaseBuffer();
        return dwReturn;
    }


    // Begin playback. The window procedure function for the parent 
    // window will be notified with an MM_MCINOTIFY message when 
    // playback is complete. At this time, the window procedure closes 
    // the device.
    mciPlayParms.dwCallback = (DWORD)hwndToCall;
    
    DWORD dwPlayFlag = 0L;
    if (bHasFromTo)
        {
        dwPlayFlag = MCI_FROM;
        mciPlayParms.dwFrom = dwFrom;
        if (dwFrom < dwTo)
            {
            dwPlayFlag |= MCI_TO;
            mciPlayParms.dwTo = dwTo;
            }
        }


    if (dwReturn = mciSendCommand(*pwIDDevice, MCI_PLAY, MCI_NOTIFY|dwPlayFlag, 
        (DWORD)(LPVOID) &mciPlayParms))
    {
        mciSendCommand(*pwIDDevice, MCI_CLOSE, 0, NULL);
        char* pszErr = sErr.GetBuffer(256);
        mciGetErrorString(dwReturn, pszErr, 256);
        sErr.ReleaseBuffer();
        return (dwReturn);
    }

    return (0L);
}
  

BOOL bCloseWaveDevice(UINT hWavDevice, Str8& sErr)
{
    sErr.Empty();
    DWORD dwErr = mciSendCommand(hWavDevice, MCI_CLOSE, 0, NULL);   
    if (dwErr != 0)
    {
        char* pszErr = sErr.GetBuffer(256);
        mciGetErrorString(dwErr, pszErr, 256);
        sErr.ReleaseBuffer();
        return FALSE;
    }

    return TRUE;
}


BOOL bParseWaveLine( LPCSTR lpszWaveCommand, Str8& sFilePath, Str8& sFrom, Str8& sTo ) // 1.4sg Change From and To from numbers to strings
{
    Str8 sWaveCommand = lpszWaveCommand;
    int idx = sWaveCommand.GetLength()-2;
	sFrom = ""; // 1.4sg
	sTo = ""; // 1.4sg
    for ( ; idx > 1; idx-- ) // Search backward for the last period not followed by space or digit // 1.4sf
        {
        if ( sWaveCommand[idx] == '.' && !( isspace( sWaveCommand[idx+1] ) || isdigit( sWaveCommand[idx+1] ) ) ) // 1.4sf Detect wave file names followed by decimal numbers
            { // Skip to the next space and assume that everything previous is the file name.
            for( ; idx < sWaveCommand.GetLength(); idx++ )
                if ( isspace( sWaveCommand[idx] ) )
                    break;
            sFilePath = sWaveCommand.Left( idx );
            break;
            }
        }
    if ( idx <= 1 ) // 1.4sf
        return FALSE;   // No period found, no possibility this is a valid wave line.
    for( ; idx < sWaveCommand.GetLength(); idx++ ) // Look for the next non-space.
        if ( !isspace( sWaveCommand[idx] ) )
            break;
    if (idx >= sWaveCommand.GetLength())
        return TRUE;
    int idxStart; // Look for first number
    for (idxStart = idx; idx < sWaveCommand.GetLength(); idx++)
        if ( !isdigit( sWaveCommand[idx] ) && !( sWaveCommand[idx] == '.' ) ) // 1.4sf
            break;
    if ( idxStart == idx )    // Then no first digit.
        return TRUE;
    else
        sFrom = sWaveCommand.Mid( idxStart, idx-idxStart ); // 1.4sg
    for( ; idx < sWaveCommand.GetLength(); idx++ ) // Look for the next non-space.for second number
        if ( !isspace( sWaveCommand[idx] ) )
            break;
    if ( idx >= sWaveCommand.GetLength() )
        return TRUE;
    for ( idxStart = idx; idx < sWaveCommand.GetLength(); idx++ ) // Look for the second number
        if ( !isdigit( sWaveCommand[idx] ) && !( sWaveCommand[idx] == '.' ) ) // 1.4sf
            break;
    if ( idxStart < idx )
        sTo = sWaveCommand.Mid( idxStart, idx - idxStart ); // 1.4sg
    return TRUE;
}

#endif // not _MAC
