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
    interface_p    = NULL;
    master_voice_p = NULL;
    
    if(soundInterfaceLoadXAudio2())
    {
	// Init COM library
	CoInitializeEx(NULL, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE);
	
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

Sound::Sound(c_char* wav_path, SoundInterface& sound_interface)
{
    // Load audio data to buffer
    if(soundLoadWav(this, wav_path))
    {
	// Create source voice
	sound_interface.interface_p->CreateSourceVoice(&source_voice_p,
						      &waveFormat);
	if(!source_voice_p)
	{
	    OutputDebugStringA("ERROR: Failed to create source voice.\n");
	}

	return; // Success
    }
    
    OutputDebugStringA("\nERROR: Failed to load .wav file. Asset not found.\n"); // Failure
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
    fread(&sound->waveFormat.nChannels, sizeof(WORD), 1, file_p);
    // Read SampleRate
    fread(&sound->waveFormat.nSamplesPerSec, sizeof(DWORD), 1, file_p);
    // Read ByteRate
    fread(&sound->waveFormat.nAvgBytesPerSec, sizeof(DWORD), 1, file_p);
    // Read BlockAlign
    fread(&sound->waveFormat.nBlockAlign, sizeof(WORD), 1, file_p);
    // Read Bits Per Sample
    fread(&sound->waveFormat.wBitsPerSample, sizeof(WORD), 1, file_p);
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
    sound->buffer.PlayBegin = 0;
    sound->buffer.PlayLength = sound->waveFormat.nSamplesPerSec *
	                       (sound->buffer.AudioBytes / sound->waveFormat.nAvgBytesPerSec);
    // Close file
    fclose(file_p);
    
    return 1;
}

int soundPlay(Sound* sound)
{
    // Check buffer state
    XAUDIO2_VOICE_STATE state_p; 
    sound->source_voice_p->GetState(&state_p);
    if(!&state_p)
    {
	OutputDebugStringA("\nERROR: Failed to get buffer state from XAudio2.\n");
	return 0;
    }

    if(state_p.BuffersQueued == 0)
    {
	// Submit buffer to the source voice
	HRESULT hr;
	hr = sound->source_voice_p->SubmitSourceBuffer(&(sound->buffer));
	if(FAILED(hr))
	{
	    OutputDebugStringA("\nERROR: Failed to submit audio buffer to source voice.\n");
	    return 0;
	}
    }
    
    sound->source_voice_p->Start(0);
    return 1;
}

void soundPause(Sound* sound)
{
    sound->source_voice_p->Stop();
}

void soundStop(Sound* sound)
{
    sound->source_voice_p->Stop();
    sound->source_voice_p->FlushSourceBuffers();
}

void soundSetVolume(Sound* sound, int volume)
{
    // Assert value is >= 0 and <= 100
    _assert(volume >= 0 && volume <= 100);
    
    // Takes a value between 0 and 100, converts it to a float scale of 0 to 1.
    float value = (float)(volume * 0.01); 
    sound->source_voice_p->SetVolume(value);
}

////////////////////////
// Struct SoundStream //
////////////////////////

SoundStream::SoundStream(c_char* wav_path, SoundInterface& sound_interface)
{
    // Init member variables
    bytes_read = 0;
    cw_buffer  = 0;
    file_p = NULL;
    
    //Populate XAudio 2 structs:
    if(!soundStreamReadWavHeader(this, wav_path))
    {
	OutputDebugStringA("ERROR: Unable to populate XAudio2 structs.\n");
    }
    
    // Create source voice
    sound_interface.interface_p->CreateSourceVoice(&source_voice_p,
						  &waveFormat);
    if(!source_voice_p)
    {
	OutputDebugStringA("ERROR: Failed to create source voice.\n");
    }
}

SoundStream::~SoundStream()
{
    fclose(file_p);
    soundStreamStop(this);
}

int soundStreamReadWavHeader(SoundStream* sound_stream, c_char* wav_path)
{
    // Open file
    fopen_s(&sound_stream->file_p, wav_path, "rb");
    if(!sound_stream->file_p) {return 0;}
    
    // Verify first ChunkID is "RIFF" format
    char chunk_ID[5];
    chunk_ID[4] = '\0';
    fread(chunk_ID, sizeof(char), 4, sound_stream->file_p);
    if(strcmp(chunk_ID, "RIFF") != 0) {return 0;}

    //////////////////////////////////
    // Populate WAVEFORMATEX struct //
    //////////////////////////////////

    // Skip ChunkSize (4), Format (4), SubchunkID (4),
    // Subchunk1Size (4), AudioFormat (2)
    fseek(sound_stream->file_p, 18, SEEK_CUR);
    // Read NumChannels
    fread(&sound_stream->waveFormat.nChannels, sizeof(shint), 1, sound_stream->file_p);
    // Read SampleRate
    fread(&sound_stream->waveFormat.nSamplesPerSec, sizeof(uint), 1, sound_stream->file_p);
    // Read ByteRate
    fread(&sound_stream->waveFormat.nAvgBytesPerSec, sizeof(uint), 1, sound_stream->file_p);
    // Read BlockAlign
    fread(&sound_stream->waveFormat.nBlockAlign, sizeof(shint), 1, sound_stream->file_p);
    // Read Bits Per Sample
    fread(&sound_stream->waveFormat.wBitsPerSample, sizeof(shint), 1, sound_stream->file_p);
    // FormatTag
    sound_stream->waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    // cbSize
    sound_stream->waveFormat.cbSize = 0;

    ////////////////////////////////////
    // Populate XAUDIO2_BUFFER struct //
    ////////////////////////////////////
    
    // Skip Subchunk2ID (4)
    fseek(sound_stream->file_p, 4, SEEK_CUR);
    // Read Subchunk2Size into total_bytes
    fread(&sound_stream->total_bytes, sizeof(uint), 1, sound_stream->file_p);
    // Set AudioBytes to correct buffer size
    sound_stream->buffer.AudioBytes = BUFFER_SIZE;

    // Upate bytes_read to account for header
    sound_stream->bytes_read = 44;

    return 1;
}

int soundStreamUpdate(SoundStream* sound_stream)
{
    // Get sound state from XAudio2
    XAUDIO2_VOICE_STATE state_p; 
    sound_stream->source_voice_p->GetState(&state_p);
    if(!&state_p)
    {
	OutputDebugStringA("\nERROR: Failed to get buffer state from XAudio2.\n");
	return 0;
    }
    
    // Fill and submit buffers if they are unqueued
    if(state_p.BuffersQueued < NUM_BUFFERS && sound_stream->buffer.Flags != XAUDIO2_END_OF_STREAM)
    {
	// Offset file_p by amount of data already read
	fseek(sound_stream->file_p, sound_stream->bytes_read, SEEK_SET);
	
	// Determine amount to read on this pass
	uint bytes_left = sound_stream->total_bytes - sound_stream->bytes_read;
	uint bytes_to_read = min(bytes_left, BUFFER_SIZE);

	// If final pass, fill buffer with 0s and mark end of stream
	if(bytes_to_read < BUFFER_SIZE)
	{
	    memset(sound_stream->buffers[sound_stream->cw_buffer], 0, BUFFER_SIZE);
	    sound_stream->buffer.Flags = XAUDIO2_END_OF_STREAM;
	}
	
        // Fill unqueued buffer with data from the file
	fread((void *)&(sound_stream->buffers[sound_stream->cw_buffer]),
	      bytes_to_read, 1, sound_stream->file_p);

	// Submit buffer to queue
	sound_stream->buffer.pAudioData = sound_stream->buffers[sound_stream->cw_buffer];
        HRESULT hr;
	hr = sound_stream->source_voice_p->SubmitSourceBuffer(&(sound_stream->buffer));
	if(FAILED(hr))
	{
	    OutputDebugStringA("\nERROR: Failed to submit audio buffer to source voice.\n");
	    return 0;
	}

	// Update bytes_read for next pass
	sound_stream->bytes_read += bytes_to_read;

	// Update current write buffer for next pass
	sound_stream->cw_buffer++;
	sound_stream->cw_buffer %= NUM_BUFFERS;
    }
    else if(state_p.BuffersQueued == 0)
    {
	soundStreamStop(sound_stream);
    }

    return 1;
}

void soundStreamPlay(SoundStream* sound_stream)
{
    sound_stream->buffer.Flags = 0;
    sound_stream->source_voice_p->Start(0);
}

void soundStreamPause(SoundStream* sound_stream)
{
    // TODO: test
    sound_stream->source_voice_p->Stop();
}

void soundStreamStop(SoundStream* sound_stream)
{
    // TODO: test
    sound_stream->source_voice_p->Stop();
    // Clear XAudio2's queued buffers
    sound_stream->source_voice_p->FlushSourceBuffers();
    // Clear our temp buffers
    for(int i = 0; i < NUM_BUFFERS; i++)
    {
	memset(sound_stream->buffers[i], 0, BUFFER_SIZE);
    }
    // Reset our tracking values
    sound_stream->bytes_read = 44;
    sound_stream->cw_buffer  = 0;
}

void soundSetVolume(SoundStream* sound_stream, int volume)
{
    // Assert value is >= 0 and <= 100
    _assert(volume >= 0 && volume <= 100);    

    // Takes a value between 0 and 100, converts it to a float scale of 0 to 1.
    float value = (float)(volume * 0.01); 
    sound_stream->source_voice_p->SetVolume(value);
}
