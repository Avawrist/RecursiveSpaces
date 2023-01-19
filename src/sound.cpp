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
	// Init COM library
	CoInitializeEx(nullptr, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE);
	
	// Init xaudio interface
	XAudio2Create(&interface_p, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if(!interface_p)
	{
	    OutputDebugStringA("Failed to initialize XAudio2 interface.\n");
	}
	
	// Init sound master
	interface_p->CreateMasteringVoice(&master_voice_p);
	if(!master_voice_p)
	{
	    OutputDebugStringA("Failed to initialize XAudio2 Master Voice\n");
	}
    }
    else
    {
	OutputDebugStringA("Failed to load XAudio2 dynamic library.\n");
    }    
}

SoundInterface::~SoundInterface()
{
    // Uninitialize COM library
    CoUninitialize();
}

int soundInterfaceLoadXAudio2()
{
    // Windows 10 
    HMODULE x_audio_2_libs = LoadLibraryA("xaudio2_9.dll");
    if(x_audio_2_libs) {return 1;}

    x_audio_2_libs = LoadLibraryA("xaudio2_9redist.dll");
    if(x_audio_2_libs) {return 1;}

    // Windows 8
    x_audio_2_libs = LoadLibraryA("xaudio2_8.dll");
    if(x_audio_2_libs) {return 1;}

    x_audio_2_libs = LoadLibraryA("xaudio2_8redist.dll");
    if(x_audio_2_libs) {return 1;}

    // DirectX SDK
    x_audio_2_libs = LoadLibraryA("xaudio2_7.dll");
    if(x_audio_2_libs) {return 1;}

    x_audio_2_libs = LoadLibraryA("xaudio2_7redist.dll");
    if(x_audio_2_libs) {return 1;}
    
    return 0;
}

//////////////////
// Struct Sound //
//////////////////

Sound::Sound(c_char* wav_path)
{
    if(!soundLoadWav(*this, wav_path))
    {
	OutputDebugStringA("\nERROR: Failed to load .wav file.\n");
    }
}

Sound::~Sound()
{
    
}

int soundLoadWav(Sound& sound, c_char* wav_path)
{
    // Populate WAVEFORMATEX struct
    // Populate XAUDIO2_BUFFER struct

    FILE* file_p = NULL;
    fopen_s(&file_p, wav_path, "rb");
    if(!file_p) {return 0;}

    // Check file header for RIFF
    char f_type[5];
    f_type[4] = '\0';
    fread(f_type, sizeof(char), 4, file_p);
    if(strcmp(f_type, "RIFF") != 0) {return 0;}
    
    // fread chunks into RIFF structs
    
    fclose(file_p);
    
    return 1;
}

void soundPlay(Sound& sound)
{
    
}

void convertEndian(char* buf, uint size)
{
    char* temp_buf = new char[size]; // dynamic char array

    // copy buffer to temp buffer
    for(int i = 0; i < size; i++)
    {
	temp_buf[i] = buf[i];
    }
    
    // copy temp buffer back to buffer, in reverse order
    for(int i = 0; i < size - 1; i++)
    {
	buf[i] = temp_buf[size - 2 - i];
    }
    
    delete [] temp_buf;
}
