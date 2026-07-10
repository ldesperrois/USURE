// Alexandre Chaussade
// Desperrois Lucas
#ifndef DICOPRIVATE_H      
#define DICOPRIVATE_H

#include <stdint.h>
#include <stddef.h>
#include "dico.h"

typedef struct dict_entry{
	uint32_t hash;
	
	void* raw_key;
	size_t raw_key_len;

	void* value;
	size_t value_len;

	struct dict_entry *next;
} dict_entry_t;


struct dict_s {
    dict_entry_t** table;
    size_t table_len;
    size_t key_nb;
};


// Fonctions privées (non exposées dans dico.h)
void dict_entry_destroy(dict_entry_t* entry);

#endif