#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct {
    int size; // liczba napisów (długość tablicy)
    char **strings; // tablica wskaźników do napisów
} StringTable;


void st_append(StringTable *p_dst, const StringTable *p_src);


void st_qsort(StringTable *st, int (*cmp)(const void*, const void*));


void st_free(StringTable *st) {
    for (int i = 0; i < st->size; ++i) {
        free(st->strings[i]);
    }
    free(st->strings);
    st->size = 0;
}


// pomocnicza do b)
int compare(const void* a, const void* b) {
    return strcmp((char *) a, (char *) b);
}


void sort_increasing(StringTable *st) {
    st_qsort(st, compare);
}


void st_add(StringTable *st, const char *txt) {
    char *new_string = "";
    strcpy(new_string, txt);
    st->strings[st->size++] = new_string;
}


void st_split(StringTable *st, const char *txt, const char *sep) {
    char *new_string = "";
    char *buffer;
    strcpy(new_string, txt);
    buffer = strtok(new_string, sep);
    while (buffer != NULL) {
        st->strings[st->size++] = buffer;
        buffer = strtok(NULL, sep);
    }
}


void st_print(StringTable *st) {
    printf("[");
    for (int i = 0; i < st->size; ++i) {
        printf("\"%s\"", st->strings[i]);
        if (i != st->size - 1) printf(",");
    }
    printf("]");
}
