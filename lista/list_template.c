#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024
#define MEMORY_ALLOCATION_ERROR  -1
#define LIST_ERROR               -2
#define PROGRAM_ERROR            -3

struct tagList;
typedef void (*ConstDataFp)(const void*);
typedef void (*DataFp)(void*);
typedef  int (*CompareDataFp)(const void*, const void*);
typedef void (*InsertInOrder)(struct tagList*, void*);

typedef struct tagListElement {
	struct tagListElement *next;
	void                  *data;
} ListElement;

typedef struct tagList {
	ListElement *head;
	ListElement *tail;
	ConstDataFp dump_data;
	DataFp      free_data;
	CompareDataFp compare_data;
	InsertInOrder insert_sorted;
} List;

// -----------------------------------------------------------------
// generic functions - they are common for all instances of the list
// (independent of the data type)
// -----------------------------------------------------------------

void init_list(List *p_list, ConstDataFp dump_data, DataFp free_data,
			   CompareDataFp compare_data, InsertInOrder insert_sorted) {
    p_list->head = NULL;
    p_list->tail = NULL;
    p_list->dump_data = dump_data;
    p_list->free_data = free_data;
    p_list->compare_data = compare_data;
    p_list->insert_sorted = insert_sorted;
}

// Print all elements of the list
void dump_list(const List* p_list) {
    ListElement *p = p_list->head;
    while (p != NULL) {
        p_list->dump_data(p->data);
        p = p->next;
    }
}

// Print elements of the list if comparable to data
void dump_list_if(List *p_list, void *data) {
    ListElement *p = p_list->head;
    while (p != NULL) {
        if (!p_list->compare_data(p->data, data)) p_list->dump_data(p->data);
        p = p->next;
    }
}

// Free all elements of the list
void free_list(List* p_list) {
    ListElement *p = p_list->head, *next;
    while (p != NULL) {
        p_list->free_data(p->data);
        next = p->next;
        free(p);
        p = next;
    }
}

// Push element at the beginning of the list
void push_front(List *p_list, void *data){
    ListElement *element = malloc(sizeof(ListElement));
    if(element == NULL) exit(MEMORY_ALLOCATION_ERROR);
    element->data = data;
    element->next = NULL;
    if(p_list->head) element->next = p_list->head;
    p_list->head = element;
    if(!p_list->tail) p_list->tail = p_list->head;
}

// Push element at the end of the list
void push_back(List *p_list, void *data) {
	ListElement *element = malloc(sizeof(ListElement));
	if(element == NULL) exit(MEMORY_ALLOCATION_ERROR);
	element->data = data;
	element->next = NULL;
	if(p_list->tail) p_list->tail->next = element;
	p_list->tail = element;
	if(!p_list->head) p_list->head = p_list->tail;
}

// Remove the first element
void pop_front(List *p_list) {
    ListElement *element = p_list->head;
    p_list->head = p_list->head->next;
    free(element);
}

// Reverse the list
void rec_reverse(List *p_list, ListElement *curr_head) {
    if (curr_head == p_list->tail) {
        p_list->head = curr_head;
        return;
    }
    rec_reverse(p_list, curr_head->next);
    curr_head->next->next = curr_head;
    p_list->tail = curr_head;
}

void reverse(List *p_list) {
    ListElement *element = p_list->head;
    rec_reverse(p_list, element);
    p_list->tail->next = NULL;
}

// insert element preserving the ordering (defined by insert_sorted function)
void insert_in_order(List *p_list, void *data) {
	p_list->insert_sorted(p_list, data);
}

// find element in sorted list after which to insert given element
ListElement* find_insertion_point(const List *p_list, ListElement *p_element) {
    ListElement *list_element = p_list->head;
    ListElement *previous_element = NULL;
    while (list_element) {
        int compare_result = p_list -> compare_data(list_element -> data, p_element -> data);
        if(compare_result == 0) return p_element;
        if(compare_result > 0) return previous_element;
        previous_element = list_element;
        list_element = list_element -> next;
    }
    return previous_element;
}

// Insert element after 'previous'
void push_after(List *p_list, void *data, ListElement *previous) {
    ListElement *element = malloc(sizeof(ListElement));
    element -> data = data;
    if (previous == NULL) {
        element->next = p_list->head;
        p_list->head = element;
    } else {
        element->next = previous->next;
        previous->next = element;
    }
    if (element->next == NULL) p_list->tail = element;
}

// Insert element preserving order (no counter)
void insert_elem(List *p_list, void *p_data) {
    ListElement *element = malloc(sizeof(ListElement));
    element->data = p_data;
    ListElement *insertion_point = find_insertion_point(p_list, element);
    if (insertion_point != element) push_after(p_list, p_data, insertion_point);
    free(element);
}

// ---------------------------------------------------------------
// type-specific definitions
// ---------------------------------------------------------------

// int element

typedef struct DataInt {
	int id;
} DataInt;

void dump_int(const void *d) {
    DataInt *element = (DataInt *)d;
    printf("%d ", element->id);
}

void free_int(void *d) {
    free(d);
}

int cmp_int(const void *a, const void *b) {
    DataInt data_a = *(DataInt *)a;
    DataInt data_b = *(DataInt *)b;
    return data_a.id - data_b.id;
}

DataInt *create_data_int(int v) {
    DataInt *data = malloc(sizeof(DataInt));
    data->id = v;
    return data;
}

// Word element

typedef struct DataWord {
	char *word;
	int counter;
} DataWord;

void dump_word (const void *d) {
    DataWord *element = (DataWord *)d;
    printf("%s\n", element->word);
}

void dump_word_lowercase (const void *d) {
    DataWord *element = (DataWord *)d;
    for (int i = 0; i < strlen(element->word); ++i) {
        printf("%c", tolower((element->word)[i]));
    }
    printf("\n");
}

void free_word(void *d) {
    free(((DataWord *)d)->word);
    free(d);
}

// compare words case insensitive
int cmp_word_alphabet(const void *a, const void *b) {
    DataWord x = *(DataWord *)a;
    DataWord y = *(DataWord *)b;
    int i = 0;
    while (1) {
        if (tolower(x.word[i]) - tolower(y.word[i])) return tolower(x.word[i]) - tolower(y.word[i]);
        if (x.word[i] == 0 && y.word[i] == 0) return 0;
        ++i;
    }
}

int cmp_word_counter(const void *a, const void *b) {
    DataWord *x = (DataWord *)a;
    DataWord *y = (DataWord *)b;
    return x->counter - y->counter;
}

// insert element; if present increase counter
void insert_elem_counter(List *p_list, void *data) {
    ListElement *element = p_list->head;
    if (!element) {
        push_front(p_list, data);
        return;
    }
    int cmp = p_list->compare_data(element->data, data);
    if (cmp > 0) {
        push_front(p_list, data);
        return;
    }
    while (element->next) {
        if (cmp == 0) {
            ++(((DataWord *)element->data)->counter);
            return;
        }
        int cmp_next = p_list->compare_data(element->next->data, data);
        if (cmp < 0 && cmp_next > 0) {
            push_after(p_list, data, element);
            return;
        }
        cmp = cmp_next;
        element = element->next;
    }
    if (cmp != 0) {
        push_after(p_list, data, element);
    } else {
        ++(((DataWord *)element->data)->counter);
    }
}

// read text, parse it to words, and insert those words to the list
// in order given by the last parameter (0 - read order,
// 1 - alphabetical order)
void stream_to_list(List *p_list, FILE *stream, int order) {
    char string[100];
    while (fgets(string, 100, stream)) {
        char *word = strtok(string, " \n\t\r.,?!:;-");
        while (word != NULL) {
            if (strlen(word) >0) {
                DataWord *data = malloc(sizeof(DataWord));
                data->word = strdup(word);
                data->counter = 1;
                if (order) {
//                    insert_elem_counter(p_list, data);
                    p_list->insert_sorted(p_list, data);
                } else {
                    push_back(p_list, data);
                }
                word = strtok(NULL, " \n\t\r.,?!:;-");
            } else {
                word = strtok(NULL, " \n\t\r.,?!:;-");
            }
        }
    }
}

// test integer list
void list_test(List *p_list, int n) {
	char op[2];
	int v;
	for (int i = 0; i < n; ++i) {
		scanf("%s", op);
		switch (op[0]) {
			case 'f':
				scanf("%d", &v);
				push_front(p_list, create_data_int(v));
				break;
			case 'b':
				scanf("%d", &v);
				push_back(p_list, create_data_int(v));
				break;
			case 'd':
				pop_front(p_list);
				break;
			case 'r':
				reverse(p_list);
				break;
			case 'i':
				scanf("%d", &v);
				insert_in_order(p_list, create_data_int(v));
				break;
			default:
				printf("No such operation: %s\n", op);
				break;
		}
	}
}

int main(void) {
	int to_do, n;
	List list;

	scanf ("%d", &to_do);
	switch (to_do) {
		case 1: // test integer list
			scanf("%d",&n);
			init_list(&list, dump_int, free_int,
					  cmp_int, insert_elem);
			list_test(&list, n);
			dump_list(&list);
			free_list(&list);
			break;
		case 2: // read words from text, insert into list, and print
			init_list(&list, dump_word, free_word,
					  cmp_word_alphabet, insert_elem_counter);
			stream_to_list(&list, stdin, 0);
			dump_list(&list);
			free_list(&list);
			break;
		case 3: // read words, insert into list alphabetically, print words encountered n times
			scanf("%d",&n);
			init_list(&list, dump_word_lowercase, free_word,
			          cmp_word_alphabet, insert_elem_counter);
			stream_to_list(&list, stdin, 1);
			list.compare_data = cmp_word_counter;
			DataWord data = { NULL, n };
//			list.dump_data = dump_word_lowercase;
			dump_list_if(&list, &data);
			printf("\n");
			free_list(&list);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

