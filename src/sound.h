#ifndef WAVE_LOADER_H
#define WAVE_LOADER_H

#include <al.h>
#include <alc.h>

#include <stdint.h>
#include <stdio.h>
#include <malloc.h>

#include "logger.h"

//pragma pack aligns our structs to 4 bytes
#pragma pack(push, 1)

//12 bytes in size
struct RIFF_HEADER
{
	char chunk_id[4];
	uint32_t chunk_size;
	char format[4];
};

//24 bytes in size
struct WAVE_FORMAT
{
	char sub_chunk_id[4];
	uint32_t sub_chunk_1_size;
	uint16_t audio_format;
	uint16_t num_channels;
	uint32_t sample_rate;
	uint32_t byte_rate;
	uint16_t byte_align;
	uint16_t bits_per_sample;
};

//8 bytes in size
struct WAVE_DATA
{
	char sub_chunk_2_id[4];
	uint32_t sub_chunk_2_size;
};

#pragma pack(pop)

typedef struct
{
	ALenum format;
	ALsizei size;
	ALvoid* data;
	ALsizei frequency;
	ALboolean loop;
} wav_file;

wav_file* wav_load_file(char* _name);

typedef struct
{
	ALuint buffer;
	ALuint source;
	ALboolean playing;
	ALboolean looped;

	ALfloat pitch;
	ALfloat volume;
	ALfloat source_pos[3];
	ALfloat source_vel[3];
	ALfloat listener_pos[3];
	ALfloat listener_vel[3];
	ALfloat listener_ori[6];
} audio;

audio audio_create(char* _filename);
void audio_destroy(audio* _audio);
void audio_set_listener_position(ALfloat _new_pos[3]);
void audio_set_listener_orientation(ALfloat _new_ori[3]);
void audio_set_listener_velocity(ALfloat _new_vel[3]);
void audio_set_pitch(audio* _audio, ALfloat _pitch);
void audio_set_master_volume(audio* _audio, ALfloat _volume);
void audio_play(audio* _audio);
void audio_stop(audio* _audio);

typedef struct
{
	ALCdevice* device;
	ALCcontext* context;
} al_system;

al_system al_start();
void al_shutdown(al_system* _al_system);

#endif