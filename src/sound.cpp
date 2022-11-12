// ======================================================================
// Title: sound.cpp
// Description: The source file for game sounds
// ======================================================================

#include "sound.hpp"

///////////////////////////
// Struct SoundInterface //
///////////////////////////

SoundInterface::SoundInterface()
{
    if(soundInterfaceLoadXAudio2())
    {
	// init COM
	
	// init xaudio interface

	// init sound master
    }
    else
    {
	OutputDebugStringA("Failed to load XAudio2\n");
    }    
}

int soundInterfaceLoadXAudio2()
{
    HMODULE x_audio_2_libs = LoadLibraryA("xaudio2_9.dll");
    if(x_audio_2_libs) {return 1;}

    x_audio_2_libs = LoadLibraryA("xaudio2_9redist.dll");
    if(x_audio_2_libs) {return 1;}
    
    return 0;
}
