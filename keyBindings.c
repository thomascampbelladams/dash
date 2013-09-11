#include "keyBindings.h"

/**
* Binds the up arrow key to be used to return previous commands
* ran in the shell.
* Note: Params and returned int are unused.
*/
int bindUp(int count, int key)
{
	HIST_ENTRY * prev = previous_history();
	printf(prev->line);
	return 0;
}

/**
* Binds the down arrow key to be used to return more recent commands
* ran in the shell.
* Note: Params and returned int are unused.
*/
int bindDown(int count, int key)
{
	HIST_ENTRY * next = next_history();
	printf(next->line);
	return 0;
}

/**
* Binds the up and down arrow keys to navigate commands
* ran in the dash
*/
void bindKeys()
{
	rl_generic_bind(ISFUNC, "\033k", (char*) bindUp, vi_movement_keymap);
	rl_generic_bind(ISFUNC, "\033j", (char*) bindDown, vi_movement_keymap);
}
