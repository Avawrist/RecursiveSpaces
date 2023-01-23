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

    // Init riff structs
    ChunkDescriptor chunkDescriptor;
    FMTSubchunk     fmtSubchunk;
    DataSubchunk    dataSubchunk;
    
    // Open file
    FILE* file_p = NULL;
    fopen_s(&file_p, wav_path, "rb");
    if(!file_p) {return 0;}
    
    // Read chunks into RIFF structs

    /////////////////////////////////
    // Fill ChunkDescriptor struct //
    /////////////////////////////////
    
    // ChunkID
    fread(chunkDescriptor.chunk_ID, sizeof(char), 4, file_p);
    if(strcmp(chunkDescriptor.chunk_ID, "RIFF") != 0) {return 0;}
    // ChunkSize
    fread(&chunkDescriptor.chunk_size, sizeof(uint), 1, file_p);
    // Format
    fread(chunkDescriptor.format, sizeof(char), 4, file_p);

    /////////////////////////////////////////////
    // Fill FMTSubchunk & WAVEFORMATEX structs //
    /////////////////////////////////////////////

    // Subchunk1ID
    fread(fmtSubchunk.subchunk_1_ID, sizeof(char), 4, file_p);
    // Subchunk1Size
    fread(&fmtSubchunk.subchunk_1_size, sizeof(uint), 1, file_p);
    // AudioFormat
    fread(&fmtSubchunk.audio_format, sizeof(shint), 1, file_p);
    // NumChannels
    fread(&sound.waveFormat.nChannels, sizeof(shint), 1, file_p);
    // SampleRate
    fread(&sound.waveFormat.nSamplesPerSec, sizeof(uint), 1, file_p);
    // ByteRate
    fread(&sound.waveFormat.nAvgBytesPerSec, sizeof(uint), 1, file_p);
    // BlockAlign
    fread(&sound.waveFormat.nBlockAlign, sizeof(shint), 1, file_p);
    // BitsPerSample
    fread(&fmtSubchunk.bits_per_sample, sizeof(shint), 1, file_p);

    ////////////////////////////////////////////////
    // Fill DataSubchunk & XAUDIO2_BUFFER structs //
    ////////////////////////////////////////////////

    // Subchunk2ID
    fread(dataSubchunk.subchunk_2_ID, sizeof(char), 4, file_p);
    // Subchunk2Size
    fread(&sound.buffer.AudioBytes, sizeof(uint), 1, file_p);
    // Data
    BYTE *buffer = new BYTE[sound.buffer.AudioBytes]; 
    fread((void *)buffer, sizeof(char), sound.buffer.AudioBytes, file_p);
    sound.buffer.pAudioData = buffer;
    delete [] buffer;
    
    // Temp debug output
    //char msg[256];
    //sprintf_s(msg, "%u", dataSubchunk.subchunk_2_size);
    //OutputDebugStringA("\n");
    //OutputDebugStringA(msg);
    //OutputDebugStringA("\n\n");
    
    fclose(file_p);
    
    return 1;
}

void soundPlay(Sound& sound)
{
    
}

////////////////////////////////
// Struct RiffChunkDescriptor //
////////////////////////////////

ChunkDescriptor::ChunkDescriptor()
{
    chunk_ID[4] = '\0';
    format[4]   = '\0';
}

////////////////////////
// Struct FMTSubChunk //
////////////////////////

FMTSubchunk::FMTSubchunk()
{
    subchunk_1_ID[4]   = '\0';
}

/////////////////////////
// Struct DataSubChunk //
/////////////////////////

DataSubchunk::DataSubchunk()
{
    subchunk_2_ID[4] = '\0';
}
