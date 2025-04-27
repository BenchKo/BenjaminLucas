#pragma once

#include "hash.h"

#include <time.h>
#define PIN_MAX 9999


struct nlist* install_hashval(char* name, char* defn);

struct nlist* lookup(char* str);

unsigned djb2_hash(char* in_str);

void DisplayTable(struct NetworkContext* netCtx);

int delete_entry(char* key);

struct nlist* modify_value(char* key, char* new_value);

int generate_pin(char* pinBuf);