#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dico_private.h"
#include "dico.h"


// This is ridiculously low:
#define HASH_TABLE_DEFAULT_SIZE 503
//#define HASH_TABLE_DEFAULT_SIZE 100003





// FNV-1a 32 bits
uint32_t fnv1a_32(const void *data, size_t len) {
    uint32_t hash = 0x811c9dc5; // offset_basis 32 bits
    unsigned char *ptr = (unsigned char *)data; // cast pour avancer 1 octet à la fois

    for (size_t i = 0; i < len; i++) {
        hash ^= ptr[i];          // XOR avec l'octet courant
        hash *= 0x01000193;      // multiplication par FNV_prime 32 bits
    }

    return hash;
}

uint64_t now_us(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (uint64_t)ts.tv_sec * 1000000ULL
         + (uint64_t)ts.tv_nsec / 1000ULL;
}

dict_t* dict_create(void){
	dict_t* dict = calloc(1, sizeof(*dict));

	if(!dict)
	    return NULL;

	dict->table = calloc(HASH_TABLE_DEFAULT_SIZE, sizeof(dict_entry_t*));

	if (!dict->table) {
            free(dict); 
            return NULL;
    	}

	dict->table_len = HASH_TABLE_DEFAULT_SIZE;
	dict->key_nb = 0;
	return dict;
}

void dict_entry_destroy(dict_entry_t* entry){
	free(entry->value);
	free(entry->raw_key); 
	free(entry);
}

void dict_destroy(dict_t* dict){
    for(size_t i = 0; i < dict->table_len; i++){
        dict_entry_t* cur = dict->table[i];
	while(cur){
	    dict_entry_t* tmp = cur->next;
	    dict_entry_destroy(cur);
	    cur = tmp;
    	}
    }
    free(dict->table);
    free(dict);
}

dict_status_t internal_dict_equal_key(const dict_entry_t* a, uint32_t hash, const void* raw_key, size_t raw_key_len){
	if(a->hash == hash && a->raw_key_len == raw_key_len && !memcmp(a->raw_key, raw_key, raw_key_len))
		return DICT_OK;
	return DICT_NOK;
}

size_t dict_len(const dict_t* dict){
	return dict->key_nb;
}

static dict_entry_t** internal_dict_find_entry_ptr(const dict_t* dict, const void* raw_key,
	       	size_t raw_key_len, uint32_t hash) {
    dict_entry_t** ptr = &(dict->table[hash % HASH_TABLE_DEFAULT_SIZE]);
    while (*ptr) {
        if (internal_dict_equal_key(*ptr, hash, raw_key, raw_key_len) == DICT_OK) {
            return ptr;
        }
        ptr = &((*ptr)->next);
    }
    return ptr; 
}

dict_status_t dict_contains(const dict_t* dict, const void* key, size_t key_len){
    	uint32_t h = fnv1a_32(key, key_len);
	const dict_entry_t* ret = *internal_dict_find_entry_ptr(dict, key, key_len, h);
	if (!ret)
		return DICT_ERR_NOT_FOUND;
	return DICT_OK;
}

dict_status_t dict_get_value(const dict_t* dict, const void* key, size_t key_len,
		const void** value_ptr, size_t* value_len){
    	uint32_t h = fnv1a_32(key, key_len);
	const dict_entry_t* ret = *internal_dict_find_entry_ptr(dict, key, key_len, h);
	if (!ret){
		*value_ptr = NULL;
		*value_len = 0;
		return DICT_ERR_NOT_FOUND;
	}
	*value_ptr = ret->value;
	*value_len = ret->value_len;
	return DICT_OK;
}

static void* internal_helper_copy_or_null(const void* src, size_t len) {
    if (!src || len == 0) return NULL;
    void* dest = malloc(len);
    if (dest) memcpy(dest, src, len);
    return dest;
}

dict_status_t dict_add(dict_t* dict, void* key, size_t key_len, void* value, size_t value_len){
	// if data is NULL or data_len is zero no value will be associated
	// with the key, call this a poor man's set
	uint32_t h = fnv1a_32(key, key_len); 
	dict_entry_t** entry_ptr = internal_dict_find_entry_ptr(dict, key, key_len, h);
	if(*entry_ptr){ // we already know this key, update it
	    void* new_val = internal_helper_copy_or_null(value, value_len);

	    if (value && value_len && !new_val)
		return DICT_ERR_MALLOC;

	    free((*entry_ptr)->value);
	    (*entry_ptr)->value = new_val;
            (*entry_ptr)->value_len = value_len;
	    return DICT_VALUE_UPDATED;
	}
	// If we're here we either have a collision or it's a new key
	// In both cases we have to create a new dict_entry
	
	dict_entry_t* new_node;
	new_node = calloc(1, sizeof(*new_node));

	if(!new_node)
	    return DICT_ERR_MALLOC;

	new_node->raw_key = internal_helper_copy_or_null(key, key_len);
    	new_node->value   = internal_helper_copy_or_null(value, value_len);

	if (!new_node->raw_key || (value_len > 0 && !new_node->value)) {
       	    free((void*)new_node->raw_key);
            free(new_node->value);
            free(new_node);
            return DICT_ERR_MALLOC;
    	}

	new_node->hash = h;
	new_node->raw_key_len = key_len;
	new_node->value_len = value_len;

	// Append new data at the end of the list
	*entry_ptr = new_node;

	// Update dict length counter
	dict->key_nb++;
	return DICT_OK;
}

// ---------------------------------------------------------------------------
// PARTIE BENCHMARK 
// ---------------------------------------------------------------------------

// Helper pour charger tout le fichier en mémoire AVANT de lancer le chrono
// Cela évite de mesurer la vitesse du disque dur au lieu de l'algo
char** load_dataset(const char* filename, size_t* out_count) {
    FILE* f = fopen(filename, "r");
    if (!f) { perror(filename); return NULL; }

    // On alloue large pour pas s'embêter avec des reallocs complexes dans le bench
    // Disons max 100k mots pour ce TP
    size_t capacity = 100000; 
    char** words = malloc(capacity * sizeof(char*));
    size_t count = 0;

    char *line = NULL;
    size_t len = 0;

    while (getline(&line, &len, f) != -1) {
        if (count >= capacity) break;
        
        // Nettoyage \n
        line[strcspn(line, "\r\n")] = 0;
        
        // Copie propre pour stocker dans notre tableau
        words[count] = strdup(line);
        count++;
    }
    
    free(line);
    fclose(f);
    *out_count = count;
    printf("[INFO] %zu mots chargés en mémoire depuis %s\n", count, filename);
    return words;
}




