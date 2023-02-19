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

// Win libs
#include <windows.h>
#include <stdio.h>
#include <cstring>
#include <xaudio2.h>
#include <algorithm>

// My libs
#include "preprocessor.hpp"
#include "typedefs.hpp"

///////////////////////////
// Struct SoundInterface //
///////////////////////////

typedef struct SoundInterface
{
    IXAudio2*               interface_p;
    IXAudio2MasteringVoice* master_voice_p;
    SoundInterface();
    ~SoundInterface();
} SoundInterface;
int soundInterfaceLoadXAudio2();

//////////////////
// Struct Sound //
//////////////////

typedef struct Sound
{
    WAVEFORMATEX         waveFormat = {0};
    XAUDIO2_BUFFER       buffer = {0};
    IXAudio2SourceVoice* source_voice_p;
    Sound(c_char* wav_path, SoundInterface& soundInterface);
    ~Sound();
} Sound;
int  soundLoadWav(Sound* sound, c_char* wav_path);
int  soundPlay(Sound* sound);
void soundPause(Sound* sound);
void soundStop(Sound* sound);
void soundSetVolume(Sound* sound, int volume);

////////////////////////
// Struct SoundStream //
////////////////////////

#define NUM_BUFFERS 3
#define BUFFER_SIZE 65536

typedef struct SoundStream
{
    WAVEFORMATEX         waveFormat = {0};
    XAUDIO2_BUFFER       buffer = {0};
    IXAudio2SourceVoice* source_voice_p;
    FILE*                file_p;
    BYTE                 buffers[NUM_BUFFERS][BUFFER_SIZE];
    uint                 bytes_read;
    uint                 total_bytes;
    uint                 cw_buffer;
    SoundStream(c_char* wav_path, SoundInterface& soundInterface);
    ~SoundStream();
} SoundStream;
int  soundStreamReadWavHeader(SoundStream* soundStream, c_char* wav_path);
int  soundStreamUpdate(SoundStream* soundStream);
void soundStreamPlay(SoundStream* soundStream);
void soundStreamPause(SoundStream* soundStream);
void soundStreamStop(SoundStream* soundStream);
void soundSetVolume(SoundStream* sound, int volume);

#endif
