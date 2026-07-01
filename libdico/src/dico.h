#ifndef DICO_H       // 1. GARDE D'INCLUSION
#define DICO_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#define HASH_TABLE_DEFAULT_SIZE 512

typedef enum {
    DICT_NOK = 0,
    DICT_OK,
    DICT_VALUE_UPDATED,
    DICT_ERR_NOT_FOUND,
    DICT_ERR_MALLOC
} dict_status_t;

typedef struct dict_entry{
	uint32_t hash;
	
	void* raw_key;
	size_t raw_key_len;

	void* value;
	size_t value_len;

	struct dict_entry *next;
} dict_entry_t;



typedef struct dict_s dict_t;

// Méthodes accessibles publiquement

uint32_t fnv1a_32(const void *data, size_t len);
uint64_t now_us(void);
dict_t* dict_create(void);
void dict_destroy(dict_t* dict);
size_t dict_len(const dict_t* dict);
dict_status_t dict_key_value_destroy(dict_t* dict ,const void *raw_key,size_t key_len);

dict_status_t dict_contains(const dict_t* dict, const void* key, size_t key_len);
dict_status_t dict_get_value(const dict_t* dict, const void* key, size_t key_len,
const void** value_ptr, size_t* value_len);
dict_status_t dict_add(dict_t* dict, void* key, size_t key_len, void* value, size_t value_len);
void afficheDico(dict_t* dict);
#endif