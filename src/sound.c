#include "sound.h"

#if ANKH2D_WIN32
	wav_file* wav_load_file(char* _name)
	{
		FILE* soundFile = NULL;

		RIFF_HEADER riff_header;
		WAVE_FORMAT wave_format;
		WAVE_DATA wave_data;
		unsigned char* data;

		//open w/ read & binary flags
		soundFile = fopen(_name, "rb");

		//if our sound file does not exist, tell the end user
		if (soundFile == NULL)
			log_fprint("ERROR: %s doesn't exist", _name);

		//read in the riff_header from file
		fread(&riff_header, sizeof(RIFF_HEADER), 1, soundFile);

		if ((riff_header.chunk_id[0] != 'R' ||
			riff_header.chunk_id[1] != 'I' ||
			riff_header.chunk_id[2] != 'F' ||
			riff_header.chunk_id[3] != 'F') || 
			(riff_header.format[0] != 'W' ||
			riff_header.format[1] != 'A' ||
			riff_header.format[2] != 'V' ||
			riff_header.format[3] != 'E'))
		{
			log_fprint("ERROR: wave RIFF header chunk_id or format is invalid");
		}

		fread(&wave_format, sizeof(WAVE_FORMAT), 1, soundFile);

		if (wave_format.sub_chunk_id[0] != 'f' ||
			wave_format.sub_chunk_id[1] != 'm' ||
			wave_format.sub_chunk_id[2] != 't' ||
			wave_format.sub_chunk_id[3] != ' ')
		{
			log_fprint("ERROR: wave format is invalid");
		}
	
		//check for extra wave params
		if (wave_format.sub_chunk_1_size > 16)
			fseek(soundFile, sizeof(uint16_t), SEEK_CUR);

		fread(&wave_data, sizeof(WAVE_DATA), 1, soundFile);

		if (wave_data.sub_chunk_2_id[0] != 'd' || 
			wave_data.sub_chunk_2_id[1] != 'a' ||
			wave_data.sub_chunk_2_id[2] != 't' ||
			wave_data.sub_chunk_2_id[3] != 'a')
		{
			log_fprint("ERROR: wave data header is invalid");
		}

		//create the size of the data
		data = (unsigned char*)malloc(wave_data.sub_chunk_2_size);
		//push the data into our data container
		fread(data, wave_data.sub_chunk_2_size, 1, soundFile);

		//used for OpenAL
		wav_file* wav = (wav_file*)malloc(sizeof(wav_file));
		wav->size = wave_data.sub_chunk_2_size;
		wav->frequency = wave_format.sample_rate;
		wav->data = data;
	
		//get our format via channels and bits/sample
		if (wave_format.num_channels == 1)
		{
			if (wave_format.bits_per_sample == 8)
				wav->format = AL_FORMAT_MONO8;

			else if (wave_format.bits_per_sample == 16)
				wav->format = AL_FORMAT_MONO16;
		}

		else if (wave_format.num_channels == 2)
		{
			if (wave_format.bits_per_sample == 8)
				wav->format = AL_FORMAT_STEREO8;

			else if (wave_format.bits_per_sample == 16)
				wav->format = AL_FORMAT_STEREO16;
		}

		fclose(soundFile);

		log_fprint("'%s' successfully created", _name);

		return wav;
	}

	audio audio_create(char* _filename)
	{
		audio audio_new;
	
		// Position of the source sound.
		audio_new.source_pos[0] =  0.0f;
		audio_new.source_pos[1] = 0.0f;
		audio_new.source_pos[2] = 0.0f;

		// Velocity of the source sound.
		audio_new.source_vel[0] =  0.0f;
		audio_new.source_vel[1] = 0.0f;
		audio_new.source_vel[2] = 0.0f;

		// Position of the listener.
		audio_new.listener_pos[0] =  0.0f;
		audio_new.listener_pos[1] = 0.0f;
		audio_new.listener_pos[2] = 0.0f;

		// Velocity of the listener.
		audio_new.listener_vel[0] =  0.0f;
		audio_new.listener_vel[1] = 0.0f;
		audio_new.listener_vel[2] = 0.0f;

		// Orientation of the listener. (first 3 elements are "at", second 3 are "up")
		audio_new.listener_ori[0] =  0.0f;
		audio_new.listener_ori[1] = 0.0f;
		audio_new.listener_ori[2] = -1.0f;
		audio_new.listener_ori[3] =  0.0f;
		audio_new.listener_ori[4] = 1.0f;
		audio_new.listener_ori[5] = 0.0f;

		alGenBuffers(1, &audio_new.buffer);
		if (alGetError() != AL_NO_ERROR)
			log_fprint("ERROR: openal buffer creation failed");

		//load our wav file
		wav_file* wav = wav_load_file(_filename);
		alBufferData(audio_new.buffer, wav->format, (void*)wav->data, wav->size, wav->frequency);

		alGenSources(1, &audio_new.source);
		if (alGetError() != AL_NO_ERROR)
			log_fprint("ERROR: openal source creation failed");

		audio_new.pitch = 1.0f;
		audio_new.volume = 1.0f;
		audio_new.playing = false;
		audio_new.looped = false;

		alSourcei(audio_new.source, AL_BUFFER, audio_new.buffer);
		alSourcef(audio_new.source, AL_PITCH, audio_new.pitch);
		alSourcef(audio_new.source, AL_GAIN, audio_new.volume);
		alSourcefv(audio_new.source, AL_POSITION, audio_new.source_pos);
		alSourcefv(audio_new.source, AL_VELOCITY, audio_new.source_vel);
		alSourcei(audio_new.source, AL_LOOPING, audio_new.looped);

		return audio_new;
	}

	void audio_destroy(audio* _audio)
	{
		alSourceStop(_audio->source);
		alDeleteSources(1, &_audio->source);
		alDeleteBuffers(1, &_audio->buffer);
	}

	void audio_set_listener_position(ALfloat _new_pos[3])
	{
		alListener3f(AL_POSITION, _new_pos[0], _new_pos[1], _new_pos[2]);
	}

	void audio_set_listener_orientation(ALfloat _new_ori[3])
	{
		alListener3f(AL_ORIENTATION, _new_ori[0], _new_ori[1], _new_ori[2]);
	}

	void audio_set_listener_velocity(ALfloat _new_vel[3])
	{
		alListener3f(AL_VELOCITY, _new_vel[0], _new_vel[1], _new_vel[2]);
	}

	void audio_set_pitch(audio* _audio, ALfloat _pitch)
	{
		_audio->pitch = _pitch;
		alSourcef(_audio->source, AL_PITCH, _pitch);
	}

	void audio_set_master_volume(audio* _audio, ALfloat _volume)
	{
		_audio->volume = _volume;
		alSourcef(_audio->source, AL_GAIN, _volume);
	}

	void audio_play(audio* _audio)
	{
		if (!_audio->playing)
		{
			_audio->playing = true;
			alSourcePlay(_audio->source);
		}
	}

	void audio_stop(audio* _audio)
	{
		if (_audio->playing)
		{
			_audio->playing = false;
			alSourceStop(_audio->source);
		}
	}

	al_system al_start()
	{
		al_system al_sys;

		al_sys.device = alcOpenDevice(NULL);
		if (al_sys.device == NULL)
			log_fprint("ERROR: alcdevice creation failed");
	
		al_sys.context = alcCreateContext(al_sys.device, NULL);
		alcMakeContextCurrent(al_sys.context);
	
		ALCenum error = alGetError();
		if (error != AL_NO_ERROR)
			log_fprint("ERROR: %s", error);

		log_fprint("openal device and context successfully created");

		return al_sys;
	}

	void al_shutdown(al_system* _al_system)
	{
		//Disable context
		alcMakeContextCurrent(NULL);
 
		//Release context(s)
		alcDestroyContext(_al_system->context);
 
		//Close device
		alcCloseDevice(_al_system->device);

		log_fprint("destroyed openal device and context");
	}

#elif ANK2D_PSX

#endif