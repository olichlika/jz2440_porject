#ifndef _TIMER_H
#define _TIMER_H


typedef struct timer_desc {
	char *name;
	void (*timer_func)(void);
}timer_desc, *p_timer_desc;

#endif