#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

int index_cmp(const void*, const void*);
int cmp(const void*, const void*);

char tab[MAX_IDS][MAX_ID_LEN];

char *keywords[] = {
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"
};


int find_idents(){
    int in_block_comment=0, in_line_comment=0, in_parentheses=0, in_apostrophes=0, after_hash=0,
            in_brackets=0, in_id=0, num_of_ids=0, found_or_keyword=0;
    char curr_id[MAX_ID_LEN], all_ids[MAX_IDS][MAX_ID_LEN], string[513];

    while ((fgets(string, 511, stdin))) {

        // Sprawdzam, czy jestem w komentarzu/stałej lub po hashu
        for (int i = 0; i < strlen(string); ++i) {
            if (in_block_comment) {
                if (i == strlen(string) - 1) continue;
                if (string[i] == '*' && string[i + 1] == '/') {
                    in_block_comment = 0;
                }
            } else if (in_line_comment) {
                if (string[i] == '\n') {
                    in_line_comment = 0;
                }
            } else if (in_apostrophes) {
                if (string[i] == '\'') {
                    in_apostrophes = 0;
                }
            } else if (in_parentheses) {
                if (string[i] == '"') {
                    in_parentheses = 0;
                }
            } else if (after_hash) {
                if (isspace(string[i])) {
                    after_hash = 0;
                }
//            } else if (in_brackets) {
//                if (string[i] == '>') {
//                    in_brackets = 0;
//                }

            // Sprawdzam, czy "wchodzę" do komentarza/stałej lub hash
            } else if (string[i] == '/' && string[i + 1] == '*') {
                in_block_comment = 1;
                ++i;
            } else if (string[i] == '/' && string[i + 1] == '/') {
                in_line_comment = 1;
            } else if (string[i] == '\'') {
                in_apostrophes = 1;
            } else if (string[i] == '"') {
                in_parentheses = 1;
            } else if (string[i] == '#') {
                after_hash = 1;
//            } else if (string[i] == '<') {
//                in_brackets = 1;

            } else if (string[i] == '\\') {
                ++i;

            //Sprawdzam, czy curr_char jest elementem id
            } else if (!in_id) {
                if (isalpha(string[i]) || string[i] == '_') {
                    strncat(curr_id, &string[i], 1);
                    in_id = 1;
                }
            } else if (in_id) {
                if (isalnum(string[i]) || string[i] == '_') {
                    strncat(curr_id, &string[i], 1);
                } else {
                    // Znaleziony koniec id
                    for (int i = 0; i < num_of_ids; ++i) {
                        if (!strcmp(curr_id, all_ids[i])) {  // Zwraca 0 jak równe
                            found_or_keyword = 1;
                            break;
                        }
                    }
                    for (int i = 0; i < 32; ++i) {
                        if (!strcmp(curr_id, keywords[i])) {  // Zwraca 0 jak równe
                            found_or_keyword = 1;
                            break;
                        }
                    }

                    if (!found_or_keyword) {
                        strcat(all_ids[num_of_ids++], curr_id);
                    }
                    found_or_keyword = 0;
                    in_id = 0;
                    curr_id[0] = '\0';
                }
            }
        }
    }
    return num_of_ids;
}


int cmp(const void* first_arg, const void* second_arg) {
	char *a = *(char**)first_arg;
	char *b = *(char**)second_arg;
	return strcmp(a, b);
}

int index_cmp(const void* first_arg, const void* second_arg) {
	int a = *(int*)first_arg;
	int b = *(int*)second_arg;
	return strcmp(tab[a], tab[b]);
}

int main(void) {
	printf("%d\n", find_idents());
	return 0;
}

