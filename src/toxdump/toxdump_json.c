/*
 * This file is part of toxfile.
 *
 * toxfile is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * toxfile is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with toxfile. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdint.h>
#include <jansson.h>
#include <tox/tox.h>
#include <libsy.h>
#include "toxdump.h"
#include "toxdump_json.h"
#include "../toxfile_state.h"
//#include "../hex.h"

int toxdump_perform_json(toxfile_state_t *state, FILE *file, toxdump_args_t *args)
{
	json_t *j_root = json_object();

	int hex_flags = (args->hex_uppercase ? HEX_UPPERCASE : 0);

	// Binary -> Hex strings
	uint8_t t_address_hex[sizeof(state->address) * 2];
	uint8_t t_public_key_hex[sizeof(state->public_key) * 2];
	hexx(t_address_hex, (char*)state->address, sizeof(state->address), hex_flags);
	hexx(t_public_key_hex, (char*)state->public_key, sizeof(state->public_key), hex_flags);

	// Initialize json_t variables
	json_t *j_str_name = json_stringn((char*)state->name, state->name_len);
	json_t *j_str_status_msg = json_stringn((char*)state->status_message, state->status_message_len);
	json_t *j_str_address = json_stringn((char*)t_address_hex, sizeof(t_address_hex));
	json_t *j_int_status = json_integer(state->status);
	json_t *j_str_public_key = json_stringn((char*)t_public_key_hex, sizeof(t_public_key_hex));
	json_t *j_str_secret_key = NULL;

	// Set fields to root variable
	json_object_set(j_root, "address", j_str_address);
	json_object_set(j_root, "name", j_str_name);
	json_object_set(j_root, "public_key", j_str_public_key);
	json_object_set(j_root, "status", j_int_status);
	json_object_set(j_root, "status_message", j_str_status_msg);

	if(state->has_secret_key == 1)
	{
		uint8_t t_secret_key_hex[sizeof(state->secret_key) * 2];
		hexx(t_secret_key_hex, state->secret_key, sizeof(state->secret_key), 0);
		j_str_secret_key = json_stringn((char*)t_secret_key_hex, sizeof(t_secret_key_hex));
		json_object_set(j_root, "secret_key", j_str_secret_key);
	}

	// Write to file
	int success = json_dumpf(j_root, file, JSON_SORT_KEYS);

	// Free json_t variables
	free(j_str_name);
	free(j_str_status_msg);
	free(j_str_address);
	free(j_int_status);
	free(j_str_public_key);

	if(j_str_secret_key != NULL)
	{
		free(j_str_secret_key);
	}

	free(j_root);

	return success;
}
