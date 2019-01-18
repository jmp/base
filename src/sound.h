#ifndef SOUND_H
#define SOUND_H

typedef struct Sound Sound;

extern Sound *sound_load(const char *filename);
extern void sound_free(Sound *sound);
extern void sound_init(void);
extern void sound_quit(void);

extern void sound_play(Sound *sound);

#endif
