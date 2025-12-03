// wav.h - Operations on .WAV waveform-audio files for Tools, Play Sound

#ifndef WAV_H
#define WAV_H

#ifndef _MAC
BOOL bParseWaveLine( LPCSTR lpszWaveCommand, Str8& sFilePath, Str8& sFrom, Str8& sTo ); // 1.4sg Change From and To from numbers to strings
// Parses the wave line into its file path and possibly from and to positions.
// It returns FALSE if this is not a valid wave line as in no "." found anywhere.
// Note: It is possible to have a pdwFrom but no pdwTo, in which case pdwTo is zero.

DWORD dwPlayWAVEFile(LPCSTR lpszWAVEFileName, HWND hwndToCall, UINT* pwIDDevice, BOOL bHasFromTo, DWORD dwFrom, DWORD dwTo, Str8& sErr);
// Plays a given waveform-audio file using MCI_OPEN and MCI_PLAY. 
// Returns when playback begins. Returns 0L on success, otherwise 
// returns an MCI error code.
// pwIDDevice is the device ID of the media player.  Retain it for the
// call to bCloseWaveDevice.
// Note: when play is finished, the system will send a MM_MCINOTIFY
// message to hwndToCall, at which point you should call
// bCloseWaveDevice.

BOOL bCloseWaveDevice(UINT hWavDevice, Str8& sErr);
// Closes the wave device identified by hWavDevice.
#endif // not _MAC

#endif // not WAV_H
