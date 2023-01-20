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

// My libs
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
    WAVEFORMATEX  waveformat;
    XAUDIO2_BUFFER buffer;
    Sound(c_char* wav_path);
    ~Sound();
} Sound;
int soundLoadWav(Sound& sound, c_char* wav_path);
void soundPlay(Sound& sound);
uint convertEndianUInt(uint value);
void convertEndianChar(char* buf, uint size);

//////////////////
// RIFF Structs //
//////////////////

typedef struct ChunkDescriptor
{
    char chunk_ID[5];
    uint chunk_size;
    char format[5];
    ChunkDescriptor();
} ChunkDescriptor;
/*
typedef struct FMTSubChunk
{

} FMTSubChunk;

typedef struct DataSubChunk
{

} DataSubChunk;
*/
#endif
