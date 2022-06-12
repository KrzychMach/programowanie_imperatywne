#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define MAX_RATE 4
#define MEMORY_ALLOCATION_ERROR  -1
#define LIST_ERROR               -2
#define PROGRAM_ERROR            -3

typedef union data_union {
	int int_data;
	char char_data;
	void *ptr_data;
} data_union;

typedef void (*DataFp)(data_union);
typedef void (*DataPFp)(data_union*);
typedef  int (*CompareDataFp)(data_union, data_union);
typedef  int (*HashFp)(data_union, int);

typedef struct ht_element {
	struct ht_element *next;
	data_union data;
} ht_element;

typedef struct hash_table {
	int size;
	int no_elements;
	ht_element **ht;
	DataFp dump_data;
	DataFp free_data;
	CompareDataFp compare_data;
	HashFp hash_function;
	DataPFp modify_data;
} hash_table;

void *safe_malloc(size_t size) {
	void *ptr = malloc(size);
	if(ptr) return ptr;
	exit(MEMORY_ALLOCATION_ERROR);
}

// ---------------------- functions to implement

// initialize table fields
void init_ht(hash_table *p_table, int size, DataFp dump_data, DataFp free_data,
		 CompareDataFp compare_data, HashFp hash_function, DataPFp modify_data) {
    p_table->size = size;
    p_table->no_elements = 0;
    p_table->ht = safe_malloc(size * sizeof(ht_element*));
    for (int i = 0; i < p_table->size; ++i) {
        p_table->ht[i] = NULL;
    }
    p_table->dump_data = dump_data;
    p_table->free_data = free_data;
    p_table->compare_data = compare_data;
    p_table->hash_function = hash_function;
    p_table->modify_data = modify_data;
}

// print elements of the list with hash n
void dump_list(const hash_table* p_table, int n) {
    ht_element *element = p_table->ht[n];
    while (element != NULL) {
        p_table->dump_data(element->data);
        element = element->next;
    }
}

// Free element pointed by data_union using free_data() function
void free_element(DataFp free_data, ht_element *to_delete) {
    free_data(to_delete->data);
}

// free all elements from the table (and the table itself)
void free_table(hash_table* p_table) {
    for (int i = 0; i < p_table->size; ++i) {
        if (p_table->ht[i] == NULL) continue;
        ht_element *element = p_table->ht[i];
        while (element->next != NULL) {
            ht_element *next_element = element->next;
            if (p_table->free_data) p_table->free_data(element->data);
            free(element);
            element = next_element;
        }
        if (p_table->free_data) p_table->free_data(element->data);
        free(element);
    }
}

// calculate hash function for integer k
int hash_base(int k, int size) {
	static const double c = 0.618033988; // (sqrt(5.) – 1) / 2.;
	double tmp = k * c;
	return (int)floor(size * (tmp - floor(tmp)));
}


// TODO
//  rehash sie wywala
void rehash(hash_table *p_table) {
//    p_table->ht = (ht_element **)realloc(p_table->ht, sizeof(**(p_table->ht)) * 2);
    ht_element **new_ht = (ht_element **)safe_malloc(sizeof(**(p_table->ht)) * 2);
    for (int i = 0; i < (p_table->size * 2); ++i) {
        new_ht[i] = NULL;
    }

    for (int i = 0; i < p_table->size; ++i) {
        ht_element *element = p_table->ht[i];
        while (element != NULL) {
            ht_element *next_element = element->next;
            int new_i = p_table->hash_function(element->data, p_table->size * 2);
            element->next = new_ht[new_i];
            new_ht[new_i] = element;
            element = next_element;
        }
    }
    free(p_table->ht);
    p_table->ht = new_ht;
    p_table->size *= 2;
}

// find element; return pointer to previous
ht_element *find_previous(hash_table *p_table, data_union data, int *first) {
    return NULL;
}

// return pointer to element with given value
ht_element *get_element(hash_table *p_table, data_union *data) {
    int i = p_table->hash_function(*data, p_table->size);
    ht_element *element = p_table->ht[i];
    while (element != NULL) {
        if (p_table->compare_data(element->data, *data)) return element;
        element = element->next;
    }
    return NULL;
}

// insert element
void insert_element(hash_table *p_table, data_union *data) {
    ht_element *new_element = safe_malloc(sizeof(ht_element));
    new_element->data = *data;
    int i = p_table->hash_function(*data, p_table->size);
    new_element->next = p_table->ht[i];
    p_table->ht[i] = new_element;
    ++p_table->no_elements;
    if (p_table->no_elements  > MAX_RATE * p_table->size) rehash(p_table);
}

// remove element
void remove_element(hash_table *p_table, data_union data) {
    int i = p_table->hash_function(data, p_table->size);
    ht_element *element = p_table->ht[i];
    if (p_table->compare_data(element->data, data)) {
        p_table->ht[i] = element->next;
        if (p_table->free_data) p_table->free_data(element->data);
        free(element);
        --p_table->no_elements;
        return;
    }
    while (element->next != NULL) {
        if (p_table->compare_data(element->next->data, data)) {
            ht_element *temp = element->next;
            element->next = element->next->next;
            if (p_table->free_data) p_table->free_data(temp->data);
            free(temp);
            --p_table->no_elements;
            return;
        }
        element = element->next;
    }
}

// type-specific definitions

// int element

int hash_int(data_union data, int size) {
	return hash_base(data.int_data, size);
}

void dump_int(data_union data) {
    printf("%d ", data.int_data);
}

int cmp_int(data_union a, data_union b) {
    return a.int_data == b.int_data ? 1 : 0;
}

// read int value and insert to the union
data_union create_data_int() {
    int n = 0;
    scanf("%d", &n);
    data_union data;
    data.int_data = n;
    return data;
}

// char element

int hash_char(data_union data, int size) {
	return hash_base((int)data.char_data, size);
}

void dump_char(data_union data) {
    printf("%c ", data.char_data);
}

int cmp_char(data_union a, data_union b) {
    return a.char_data == b.char_data ? 1 : 0;
}

// read char value and insert to the union
data_union create_data_char() {
    char n;
    getchar(); // w celu pozbycia się whitespace
    n = getchar();
    data_union data;
    data.char_data = n;
    return data;
}

// Word element

typedef struct DataWord {
	char *word;
	int counter;
} DataWord;

void dump_word(data_union data) {
    printf("%s %d", ((DataWord *)data.ptr_data)->word, ((DataWord *)data.ptr_data)->counter);
}

void free_word(data_union data) {
    free(((DataWord *) data.ptr_data)->word);
    free(data.ptr_data);
}

int cmp_word(data_union a, data_union b) {
    return strcmp(((DataWord *)a.ptr_data)->word, ((DataWord *)b.ptr_data)->word) == 0 ? 1 : 0;
}

int hash_word(data_union data, int size) {
	int s = 0;
	DataWord *dw = (DataWord*)data.ptr_data;
	char *p = dw->word;
	while (*p) {
		s += *p++;
	}
	return hash_base(s, size);
}

// increase the counter
void modify_word(data_union *data) {
    DataWord *word = (DataWord *)data->ptr_data;
    ++word->counter;
}

// allocate DataWord structure and insert to the union
data_union create_data_word(char *value) {
    DataWord *new_elem = malloc(sizeof *new_elem);
    new_elem->word = malloc((strlen(value) + 1) * sizeof(char));
    for (int j = 0; j < strlen(value); ++j) {
        new_elem->word[j] = (char) tolower(value[j]);
    }
    new_elem->word[strlen(value)] = '\0';
    new_elem->counter = 1;
    data_union new_data;
    new_data.ptr_data = new_elem;
    return new_data;
}


// read text, parse it to words, and insert those words to the hashtable
void stream_to_ht(hash_table *p_table, FILE *stream) {
    int can_insert, i;
    char string[100];
    while (fgets(string, 99, stream)) {
        char *word = strtok(string, " \n\t\r.,?!:;-");
        while (word != NULL) {
            if (strlen(word) > 0) {
                // Tworzę struktury zawierające dane
                data_union new_data = create_data_word(word);

                // Sprawdzam czy słowo już istnieje i potencjalnie wstawiam do hash table
                i = p_table->hash_function(new_data, p_table->size);
                can_insert = 1;
                ht_element *element = p_table->ht[i];
                while (element != NULL) {
                    if (cmp_word(element->data, new_data)) {
                        can_insert = 0;
                        ++((DataWord *) element->data.ptr_data)->counter;
                        free_word(new_data);
                        break;
                    }
                    element = element->next;
                }

                // Mogę wstawić element
                if (can_insert) {
                    insert_element(p_table, &new_data);
                }
            }
            word = strtok(NULL, " \n\t\r.,?!:;-");
        }
    }
}

// test primitive type list
void table_test(hash_table *p_table, int n, data_union (*create_data)()) {
	char op;
	data_union data;
	for (int i = 0; i < n; ++i) {
		scanf(" %c", &op);
		data = create_data();
		switch (op) {
			case 'r':
				remove_element(p_table, data);
				break;
			case 'i':
				insert_element(p_table, &data);
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
}

int main(void) {
	int to_do, n, index;
	hash_table table;
	char buffer[BUFFER_SIZE];
	data_union data;

	scanf ("%d", &to_do);
	switch (to_do) {
		case 1: // test integer table
			scanf("%d %d",&n, &index);
			init_ht(&table, 4, dump_int,NULL,
					cmp_int, hash_int, NULL);
			table_test(&table, n, create_data_int);
			printf ("%d\n", table.size);
			dump_list(&table, index);
			free_table(&table);
			break;
		case 2: // test char table
			scanf("%d %d",&n, &index);
			init_ht(&table, 4, dump_char,NULL,
			        cmp_char, hash_char, NULL);
			table_test(&table, n, create_data_char);
			printf ("%d\n", table.size);
			dump_list(&table, index);
			free_table(&table);
			break;
		case 3: // read words from text, insert into table, and print
			scanf("%s", buffer);
			init_ht(&table, 8, dump_word, free_word,
			        cmp_word, hash_word, modify_word);
			stream_to_ht(&table, stdin);
			printf ("%d\n", table.size);
			data = create_data_word(buffer);
			ht_element *e = get_element(&table, &data);
			if (e) table.dump_data(e->data);
			if (table.free_data) table.free_data(data);
			free_table(&table);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}
