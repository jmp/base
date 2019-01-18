#ifndef STATE_H
#define STATE_H

typedef struct {
	void (*init)(void);
	void (*quit)(void);
	void (*update)(void);
	void (*draw)(float fraction);
} State;

extern void state_set(State *state);
extern int state_update(void);
extern void state_draw(float fraction);
extern void state_quit(void);

#endif
