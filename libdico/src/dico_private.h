#ifndef DICOPRIVATE_H      
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
void dict_entry_destroy(dict_entry_t* entry);

#endif