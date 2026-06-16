

int main(void) {
    dict_t* set = dict_create();
    dict_t* dico = dict_create();

    FILE* f = fopen("mots_17479.txt", "r");
    if (!f) { perror("mots_17479.txt"); return 1; }

    char *line = NULL;
    size_t len = 0; // taille du buffer, getline s'en charge

    int line_nb = 1;
    while (getline(&line, &len, f) != -1) {
        // line contient la ligne complète, '\n' inclus
	size_t word_len = strcspn(line, "\r\n");
 	// on remplace \n par \0 (pour pas casser printf, sinon OSEF)
	line[word_len] = '\0';
	// on veut hasher toute la chaîne \0 compris
	
	// ici on construit un vrai dictionnaire clé -> valeur
	dict_add(dico, line, word_len + 1, &line_nb, sizeof(int));

	// là on fait un set, donc juste des clés
	dict_add(set, line, word_len + 1, NULL, 0);
	line_nb++;
    }

    printf("Le dictionnaire contient %ld clés\n", dict_len(dico));
    printf("Le set contient %ld clés\n", dict_len(set));

    char to_find[]="aplatissaient";

    // un essai avec le set
    printf("Recherche dans un set simple\n");
    uint64_t t0 = now_us();
    dict_status_t found = dict_contains(set, to_find, strlen(to_find) + 1);
    uint64_t t1 = now_us();
    printf("durée = %llu us\n",
    	(unsigned long long)(t1 - t0));
    if(found == DICT_OK){
	    printf("%s est dans la liste de mots\n", to_find);
    }
    else{
	    printf("%s n'est PAS dans la liste de mots\n", to_find);
    }

    // le même avec une recherche de clé dans le dictionnaire
    printf("Recherche dans un dictionnaire\n");
    char to_find2[]="qdsfjhqskjlhfk";
    t0 = now_us();
    found = dict_contains(dico, to_find2, strlen(to_find2) + 1);
    t1 = now_us();
    printf("durée = %llu us\n",
    	(unsigned long long)(t1 - t0));
    if(found == DICT_OK){
	    printf("%s est dans la liste de mots\n", to_find2);
    }
    else{
	    printf("%s n'est PAS dans la liste de mots\n", to_find2);
    }


    printf("Recherche clé/valeur dans un dictionnaire\n");
    char to_find3[]="impliquassent";
    const int* value;
    size_t value_len;
    t0 = now_us();
    found = dict_get_value(dico, to_find3, strlen(to_find3) + 1, (const void**)&value, &value_len);
    t1 = now_us();
    printf("durée = %llu us\n",
    	(unsigned long long)(t1 - t0));
    if(found == DICT_OK){
	    printf("%s est dans la liste de mots à la ligne %d\n", to_find3, *(int*)value);
    }
    else{
	    printf("%s n'est PAS dans la liste de mots\n", to_find3);
    }

    dict_destroy(dico);
    free(line);
    fclose(f);
    return 0;
}