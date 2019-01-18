#include <SDL2/SDL_mixer.h>
#include "sound.h"
#include "memory.h"
#include "file.h"
#include "rwops.h"
#include "error.h"
#include "debug.h"

/* Audio format constants */
#define SAMPLE_DATE 44100
#define NUM_CHANNELS 2
#define BUFFER_SIZE 512

/* Sound struct */
struct Sound {
    Mix_Chunk *sample;
    const char *filename;
};

/* Internal helper functions */
static const char *get_audio_format_string(Uint16 format);

void sound_init(void) {
    int numtimesopened;
    int frequency;
    int channels;
    Uint16 format;
    SDL_version compile_version;
    const SDL_version *linked_version = Mix_Linked_Version();
    SDL_MIXER_VERSION(&compile_version);

    debug_printf("Initializing sound...\n");

    /* Initialize mixer */
    if (Mix_OpenAudio(SAMPLE_DATE, MIX_DEFAULT_FORMAT, NUM_CHANNELS,
                      BUFFER_SIZE) < 0) {
        error("Failed to initialize sound: %s\n", Mix_GetError());
    }

    /* Get audio format information */
    numtimesopened = Mix_QuerySpec(&frequency, &format, &channels);
    if (!numtimesopened) {
        error("Failed to query audio format: %s\n", Mix_GetError());
    }

    debug_printf("Listing audio details...\n");
    debug_printf("  Compiled version: %d.%d.%d\n", compile_version.major,
                 compile_version.minor, compile_version.patch);
    debug_printf("  Linked version: %d.%d.%d\n", linked_version->major,
                 linked_version->minor, linked_version->patch);
    debug_printf("  Frequency: %d Hz\n", frequency);
    debug_printf("  Format: %s\n", get_audio_format_string(format));
    debug_printf("  Channels: %d\n", channels);
    debug_printf("  Chunk decoders: %d\n", Mix_GetNumChunkDecoders());
    debug_printf("  Device opened: %d time(s)\n", numtimesopened);
    debug_printf("End of audio details.\n");

    debug_printf("Sound initialized.\n");
}

void sound_quit(void) {
    debug_printf("Shutting down sound...\n");
    Mix_CloseAudio();
    debug_printf("Sound shut down.\n");
}

void sound_play(Sound * sound) {
    if (Mix_PlayChannel(-1, sound->sample, 0) < 0) {
        error("Failed to play sound: %s\n", Mix_GetError());
    }
}

Sound *sound_load(const char *filename) {
    Sound *sound;
    Mix_Chunk *sample;
    SDL_RWops *rwops;

    if (!(rwops = rwops_open_read(filename))) {
        error("Failed to open %s for reading: %s\n",
              filename, SDL_GetError());
    }

    if (!(sample = Mix_LoadWAV_RW(rwops, 1))) {
        error("Failed to load sound: %s\n", Mix_GetError());
    }

    sound = memory_alloc(sizeof(Sound));
    sound->sample = sample;
    sound->filename = filename;

    debug_printf("Listing chunk data...\n");
    debug_printf("  Allocated: %s\n", sample->allocated ? "true" : "false");
    debug_printf("  Length: %lu bytes\n", (unsigned long) sample->alen);
    debug_printf("  Volume: %d/128\n", sample->volume);
    debug_printf("End of chunk data.\n");

    debug_printf("Sound %s loaded.\n", filename);

    return sound;
}

void sound_free(Sound *sound) {
    const char *filename;
    if (sound == NULL) {
        error("Attempting to free an already freed sound.\n");
    }
    filename = sound->filename;
    Mix_FreeChunk(sound->sample);
    memory_free(sound);
    debug_printf("Sound %s freed.\n", filename);
}

const char *get_audio_format_string(Uint16 format) {
    switch (format) {
        case AUDIO_U8: return "unsigned, 8-bit";
        case AUDIO_S8: return "signed, 8-bit";
        case AUDIO_U16LSB: return "unsigned, 16-bit, little-endian";
        case AUDIO_S16LSB: return "signed, 16-bit, little-endian";
        case AUDIO_U16MSB: return "unsigned, 16-bit, big-endian";
        case AUDIO_S16MSB: return "signed, 16-bit, big-endian";
        case AUDIO_S32LSB: return "signed, 32-bit, little-endian";
        case AUDIO_S32MSB: return "signed, 32-bit, big-endian";
        case AUDIO_F32LSB: return "float, 32-bit, little-endian";
        case AUDIO_F32MSB: return "float, 32-bit, big-endian";
    }
    return "unknown";
}
