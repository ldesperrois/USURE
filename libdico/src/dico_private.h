#ifndef DICOPRIVATE_H       // 1. GARDE D'INCLUSION
#define DICOPRIVATE_H

#include <stdint.h>
#include <stddef.h>
#include "dico.h"



struct dict_s {
    dict_entry_t** table;
    size_t table_len;
    size_t key_nb;
};




// Fonctions privées (non exposées dans dico.h)
dict_status_t internal_dict_equal_key(const dict_entry_t* a, uint32_t hash, const void* raw_key, size_t raw_key_len);
static dict_entry_t** internal_dict_find_entry_ptr(const dict_t* dict, const void* raw_key,size_t raw_key_len, uint32_t hash);
static void* internal_helper_copy_or_null(const void* src, size_t len);
static void dict_entry_destroy(dict_entry_t* entry);


#endif