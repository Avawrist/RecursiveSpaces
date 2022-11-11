// =====================================================================================================
// Title: sound.hpp
// Description: The header file for game sounds, built on Microsoft XAudio2.9
// Will require a Win redistributable for users without XAudio2.9. From MSDN: "The XAudio 2.9 NuGet
// package (Microsoft.XAudio2.Redist.*.nupkg) includes a 32-bit and a 64-bit version of a DLL that
// implements the XAudio 2.9 API. The DLL is called XAUDIO2_9REDIST.DLL. This DLL will work on
// Windows 7 SP1, Windows 8, Windows 8.1 and Windows 10."
// =====================================================================================================

#ifndef SOUND_H
#define SOUND_H

#include <windows.h>
#include <xaudio2.h>>

int loadXAudio2();

#endif
