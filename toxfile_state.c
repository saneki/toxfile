#include <string.h>
#include <tox/tox.h>
#include "toxfile_state.h"

int toxfile_load_state(Tox *tox, toxfile_state_t *state, size_t flags)
{
	if(tox == NULL || state == NULL)
	{
		return -1;
	}

	memset(state, 0, sizeof(&state));

	tox_get_address(tox, state->address);
	state->name_len = tox_get_self_name_size(tox);
	tox_get_self_name(tox, state->name);
	state->status_message_len = tox_get_self_status_message_size(tox);
	tox_get_self_status_message(tox, state->status_message, state->status_message_len);
	state->status = tox_get_self_user_status(tox);

	if((flags & TOXFILE_LOAD_PRIVKEY) == TOXFILE_LOAD_PRIVKEY)
	{
		tox_get_keys(tox, state->public_key, state->private_key);
		state->has_private_key = 1;
	}
	else
	{
		tox_get_keys(tox, state->public_key, NULL);
		state->has_private_key = 0;
	}

	return 0;
}