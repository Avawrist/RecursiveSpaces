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
    interface_p    = nullptr;
    master_voice_p = nullptr;
    
    if(soundInterfaceLoadXAudio2())
    {
	// Init COM
	CoInitializeEx(nullptr, COINIT_MULTITHREADED); // wtf is this?
	
	// Init xaudio interface
	XAudio2Create(&interface_p, 0, XAUDIO2_DEFAULT_PROCESSOR);
	
	// Init sound master
	interface_p->CreateMasteringVoice(&master_voice_p);
    }
    else
    {
	OutputDebugStringA("Failed to load XAudio2\n");
    }    
}

SoundInterface::~SoundInterface()
{
    delete interface_p;
    delete master_voice_p;
}

int soundInterfaceLoadXAudio2()
{
    HMODULE x_audio_2_libs = LoadLibraryA("xaudio2_9.dll");
    if(x_audio_2_libs) {return 1;}

    x_audio_2_libs = LoadLibraryA("xaudio2_9redist.dll");
    if(x_audio_2_libs) {return 1;}
    
    return 0;
}
