#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "dico_private.h"
#include "dico.h"


// This is ridiculously low:
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

static dict_status_t internal_dict_equal_key(const dict_entry_t* a, uint32_t hash, const void* raw_key, size_t raw_key_len){
	if(a->hash == hash && a->raw_key_len == raw_key_len && !memcmp(a->raw_key, raw_key, raw_key_len))
		return DICT_OK;
	return DICT_NOK;
}

size_t dict_len(const dict_t* dict){
	return dict->key_nb;
}

 dict_entry_t** internal_dict_find_entry_ptr(const dict_t* dict, const void* raw_key,
	size_t raw_key_len, uint32_t hash) {
    dict_entry_t** ptr = &(dict->table[hash & (dict->table_len-1)]);
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
/**
 * @brief Procédure qui s'occupe de redimenssioner le tableau 
 * quand le facteur de charge est dépassé
 * 
 * @param dict 
 */
void dynamicResizing(dict_t* dict){
	
	size_t newSize  = dict->table_len*2;
	printf("On effectue un redimensionnement pour une nouvelle taille de %zu\n",newSize);
	dict_entry_t ** nouvelleTable = calloc(newSize,sizeof(dict_entry_t*));
	for(size_t i=0;i<dict->table_len;i++){
		dict_entry_t* parcour = dict->table[i];
		while(parcour){
			dict_entry_t* prochain = parcour->next;

			uint32_t newIndex = parcour->hash &(newSize-1);
			parcour->next = nouvelleTable[newIndex];
			nouvelleTable[newIndex] = parcour;

			parcour = prochain;
		}
	}
	free(dict->table);
	dict->table = nouvelleTable;
	dict->table_len = (dict->table_len)*2;

}


/**
 * @brief Fonction qui affiche un element du dictionnaire
 * On considère que notre valeur est toujours en entier 32 bit
 * @param cur 
 */
void afficheValue(dict_entry_t* cur){
	printf("clé : %s\n",(char*)cur->raw_key);
	printf("Son nombre d'occurences : %d\n",*(int*)cur->value);
}



/**
 * @brief Fonction qui affiche les paires clé valeur du dictionnaire
 * 
 * @param dict 
 */
void afficheDico(dict_t* dict,int uniq){
	for(size_t i = 0; i < dict->table_len; i++){
        dict_entry_t* cur = dict->table[i];
		if(cur!=NULL){
			printf("Paires clés valeur à l'index %zu:\n ",i);
		}
		while(cur){
			if(cur!=NULL){
            if (uniq == 1) {
                if (*(int*)cur->value == 1) {
                    afficheValue(cur);
                }
            } 
            else {
            
                afficheValue(cur);
            }
				
			}
			cur=cur->next;
		}
	}
}

int comparator(const void *a,const void* b){
	 dict_entry_t* elementA = *(dict_entry_t**)a;
	dict_entry_t *elementB = *(dict_entry_t **)b;
	// Si soustraction positif b retourné avant a
	int occurenceA = *(int*)elementA->value;
	int occurenceB = *(int*)elementB->value;

	return (occurenceA - occurenceB);
}

void trierOccurenceDecroissant(dict_t *dict){
	// On créer un tableau aussi grand que le nombre de clés pour une données de type dict_entry_t
	dict_entry_t** tableauTrie = calloc(dict->key_nb,sizeof(dict_entry_t *));

	int indexTrie = 0;
	for(size_t i =0;i<dict->table_len;i++){
		dict_entry_t* elementParcouru = dict->table[i];
		while(elementParcouru!=NULL){
			tableauTrie[indexTrie] = elementParcouru;
			indexTrie+=1;
			elementParcouru = elementParcouru->next;
		}
	}
	// On utilsie la fonction qsort
	qsort(tableauTrie,dict->key_nb,sizeof(dict_entry_t *),comparator);
	printf("Affichage décroissant des mots selon leurs occurences : \n");
	for (size_t j=0;j<dict->key_nb;j++){
		printf(" la Cle : %s | Occurences : %d\n",(char *)tableauTrie[j]->raw_key,*(int *)tableauTrie[j]->value);
	}


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

	if ((double)dict->key_nb / dict->table_len > 3.0)
		dynamicResizing(dict);

	return DICT_OK;
}
/**
 * @brief Fonction qui s'occupe de supprimer une clé avec sa valeur
 * en conservant la logigue de notre dictionnaire
 * 
 * @param dict 
 * @param raw_key 
 * @param key_len 
 * @return dict_status_t 
 */
dict_status_t dict_key_value_destroy(dict_t* dict,const void *raw_key,size_t key_len){
	uint32_t h = fnv1a_32(raw_key, key_len); 
	// On recupère l'enregistrement du dictionnaire
	dict_entry_t** ret = internal_dict_find_entry_ptr(dict,raw_key,key_len,h);
	// Si pas trouvé
	if (*ret==NULL)
	{
		return DICT_ERR_NOT_FOUND;
	}
	else{
		// On supprime la valeur et on remplace l'element par son next
		dict_entry_t *delete = *ret;
		*ret = delete->next;
		dict->key_nb--;
		dict_entry_destroy(delete);
		return DICT_OK;
	}
	
}

