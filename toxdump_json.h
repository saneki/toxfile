#ifndef TOXDUMP_JSON_H
#define TOXDUMP_JSON_H

#include "toxdump.h"

void toxdump_create_json_module(toxdump_module *module);
int toxdump_perform_json(toxfile_state_t *state, FILE *file);
void toxdump_teardown_json_module();

#endif