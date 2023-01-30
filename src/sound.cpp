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

	// Start XAudio2 engine
	interface_p->StartEngine();
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

    // Stop the XAudio2 engine
    interface_p->StopEngine();
}

int soundInterfaceLoadXAudio2()
{
    // Windows 10 
    HMODULE x_audio_2_libs = LoadLibraryA("xaudio2_9.dll");
    if(x_audio_2_libs)
    {
	OutputDebugStringA("SUCCESS: Loaded xaudio2_9.dll.\n");
	return 1;
    }

    x_audio_2_libs = LoadLibraryA("xaudio2_9redist.dll");
    if(x_audio_2_libs)
    {
	OutputDebugStringA("SUCCESS: Loaded xaudio2_9redist.dll.\n");
	return 1;
    }

    // Windows 8
    x_audio_2_libs = LoadLibraryA("xaudio2_8.dll");
    if(x_audio_2_libs)
    {
	OutputDebugStringA("SUCCESS: Loaded xaudio2_8.dll.\n");
	return 1;
    }

    x_audio_2_libs = LoadLibraryA("xaudio2_8redist.dll");
    if(x_audio_2_libs)
    {
	OutputDebugStringA("SUCCESS: Loaded xaudio2_8redist.dll.\n");
	return 1;
    }

    // DirectX SDK
    x_audio_2_libs = LoadLibraryA("xaudio2_7.dll");
    if(x_audio_2_libs)
    {
	OutputDebugStringA("SUCCESS: Loaded xaudio2_7.dll.\n");
	return 1;
    }

    x_audio_2_libs = LoadLibraryA("xaudio2_7redist.dll");
    if(x_audio_2_libs)
    {
	OutputDebugStringA("SUCCESS: Loaded xaudio2_7redist.dll.\n");
	return 1;
    }
    
    return 0;
}

//////////////////
// Struct Sound //
//////////////////

Sound::Sound(c_char* wav_path, SoundInterface& soundInterface)
{
    // Load audio data to buffer
    if(!soundLoadWav(this, wav_path))
    {
	OutputDebugStringA("\nERROR: Failed to load .wav file.\n");
    }

    // Create source voice
    soundInterface.interface_p->CreateSourceVoice(&source_voice_p,
						  &waveFormat);
    if(!source_voice_p)
    {
	OutputDebugStringA("ERROR: Failed to create source voice.\n");
    }
}

Sound::~Sound()
{
    soundStop(this);
}

int soundLoadWav(Sound* sound, c_char* wav_path)
{
    // Open file
    FILE* file_p = NULL;
    fopen_s(&file_p, wav_path, "rb");
    if(!file_p) {return 0;}
    
    // Verify first ChunkID is "RIFF" format
    char chunk_ID[5];
    chunk_ID[4] = '\0';
    fread(chunk_ID, sizeof(char), 4, file_p);
    if(strcmp(chunk_ID, "RIFF") != 0) {return 0;}

    //////////////////////////////////
    // Populate WAVEFORMATEX struct //
    //////////////////////////////////

    // Skip ChunkSize (4), Format (4), SubchunkID (4),
    // Subchunk1Size (4), AudioFormat (2)
    fseek(file_p, 18, SEEK_CUR);
    // Read NumChannels
    fread(&sound->waveFormat.nChannels, sizeof(shint), 1, file_p);
    // Read SampleRate
    fread(&sound->waveFormat.nSamplesPerSec, sizeof(uint), 1, file_p);
    // Read ByteRate
    fread(&sound->waveFormat.nAvgBytesPerSec, sizeof(uint), 1, file_p);
    // Read BlockAlign
    fread(&sound->waveFormat.nBlockAlign, sizeof(shint), 1, file_p);
    // Read Bits Per Sample
    fread(&sound->waveFormat.wBitsPerSample, sizeof(shint), 1, file_p);
    // FormatTag
    sound->waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    // cbSize
    sound->waveFormat.cbSize = 0;

    ////////////////////////////////////
    // Populate XAUDIO2_BUFFER struct //
    ////////////////////////////////////
    
    // Skip Subchunk2ID (4)
    fseek(file_p, 4, SEEK_CUR);
    // Read Subchunk2Size
    fread(&sound->buffer.AudioBytes, sizeof(uint), 1, file_p);
    // Read Data
    BYTE *buffer = new BYTE[sound->buffer.AudioBytes]; 
    fread((void *)buffer, sizeof(char), sound->buffer.AudioBytes, file_p);
    sound->buffer.pAudioData = buffer;
    sound->buffer.Flags = XAUDIO2_END_OF_STREAM;
    
    // Close file
    fclose(file_p);
    
    return 1;
}

void soundPlay(Sound* sound)
{
    // Check buffer state
    XAUDIO2_VOICE_STATE state_p; 
    sound->source_voice_p->GetState(&state_p);
    if(!&state_p)
    {
	OutputDebugStringA("\nERROR: Failed to get buffer state from XAudio2.\n");
	return;
    }

    if(state_p.BuffersQueued == 0)
    {
	// Submit buffer to the source voice
	HRESULT hr;
	hr = sound->source_voice_p->SubmitSourceBuffer(&(sound->buffer));
	if(FAILED(hr))
	{
	    OutputDebugStringA("\nERROR: Failed to submit audio buffer to source voice.\n");
	    return;
	}
    }
    
    sound->source_voice_p->Start(0);
}

void soundStop(Sound* sound)
{
    sound->source_voice_p->Stop();
}
