#ifndef DICO_H       // 1. GARDE D'INCLUSION
#define DICO_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>


typedef struct dict_entry{
	uint32_t hash;
	
	void* raw_key;
	size_t raw_key_len;

	void* value;
	size_t value_len;

	struct dict_entry *next;
} dict_entry_t;



typedef struct dict_s dict_t;
// Fonction de l'algo de HASH
uint32_t fnv1a_32(const void *data, size_t len);


uint64_t now_us(void);
// Création d'un dictionnaire
dict_t* dict_create(void);
// Suppression d'un element du dictionnaire( libération de mémoire)
void dict_entry_destroy(dict_entry_t* entry);
// Suppression de l'ensemble du dictionnaire 
void dict_destroy(dict_t* destroy);
// Obtenir la taille du dictionnaire
size_t dict_len(const dict_t* dict);
// vérifier si une clé existe dans le dictionnaire
dict_status_t dict_contains(const dict_t* dict, const void* key, size_t key_len){



#endif