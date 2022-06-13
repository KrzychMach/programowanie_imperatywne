#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STR_LEN 64
#define MAX_PERSONS 1024

typedef struct Person {
	int age;
	char first_name[MAX_STR_LEN];
	char last_name[MAX_STR_LEN];
} Person;

// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int cmp_person(const void *p1, const void *p2) {
    Person person1 = *(Person *)p1;
    Person person2 = *(Person *)p2;

    if (person2.age - person1.age != 0) {
        return (person2.age - person1.age);
    } else {
        // Imiona
        int i = 0;
        while (person1.first_name[i] != '\0' || person2.first_name[i] != '\0') {
            if (person1.first_name[i] - person2.first_name[i] != 0) {
                return (person1.first_name[i] - person2.first_name[i]);
            } else {
                ++i;
            }
        }

        // Nazwiska
        i = 0;
        while (person1.last_name[i] != '\0' || person2.last_name[i] != '\0') {
            if (person1.last_name[i] - person2.last_name[i] != 0) {
                return (person1.last_name[i] - person2.last_name[i]);
            } else {
                ++i;
            }
        }
    }

    // Nie znalazło różnicy, czyli elementy równe
    return 0;
}

// Read data to Person array (till EOF)
int read_person_array(Person *persons) {
    char curr_string[MAX_STR_LEN];
    int index = 0;

    while (fgets(curr_string, MAX_STR_LEN - 1, stdin)) {
        if (strcspn(curr_string, "\n") == 0) {
            continue;
        }
        persons[index].age = atoi(strtok(curr_string, " \n"));
        strcat(persons[index].first_name, strtok(NULL, " \n"));
        strcat(persons[index].last_name, strtok(NULL, " \n"));
        ++index;
    }

    return index;
}

// Print Person array
void print_person_array(Person *persons, int n) {
    for (int i = 0; i < n; ++i) {
        printf("%d %s %s\n", persons[i].age, persons[i].first_name, persons[i].last_name);
    }
}

// Sort women first (woman's first name ends with 'a');
// Then sort women by age and men by last name
// Line consists of: age, first_name, last_name
// (int that order)
int cmp_lines(const void *l1, const void *l2) {
    char person1[MAX_STR_LEN] = "";
    char person2[MAX_STR_LEN] = "";
    char first_name_1[MAX_STR_LEN] = "";
    char last_name_1[MAX_STR_LEN] = "";
    char first_name_2[MAX_STR_LEN] = "";
    char last_name_2[MAX_STR_LEN] = "";
    strcpy(person1, (char *)l1);
    strcpy(person2, (char *)l2);

    int age1 = atoi(strtok(person1, " "));
    strcat(first_name_1, strtok(NULL, " "));
    strcat(last_name_1, strtok(NULL, " \n"));
    int age2 = atoi(strtok(person2, " "));
    strcat(first_name_2, strtok(NULL, " "));
    strcat(last_name_2, strtok(NULL, " \n"));

    // Określam płeć, 0 - kobieta, 1 - mężczyzna
    int i = 0;
    while (first_name_1[i + 1] != '\0') i++;
    int gender1 = first_name_1[i] == 'a' ? 0 : 1;
    i = 0;
    while (first_name_2[i + 1] != '\0') i++;
    int gender2 = first_name_2[i] == 'a' ? 0 : 1;

    // Porównanie płci
    if (gender1 - gender2) {
        return gender1 - gender2;
    } else if (gender1 == 0) {
        // Kobiety
        return (age1 - age2);
    } else {
        // Mężczyźni
        i = 0;
        while (last_name_1[i] != '\0' || last_name_2[i] != '\0') {
            if (last_name_1[i] - last_name_2[i] != 0) {
                return (last_name_1[i] - last_name_2[i]);
            } else {
                ++i;
            }
        }

        return 0;
    }
}

// Read lines with students' data (as text)
int read_lines(char lines[][MAX_STR_LEN]) {
    char curr_string[MAX_STR_LEN];
    int index = 0;

    while (fgets(curr_string, MAX_STR_LEN - 1, stdin)) {
        if (strcspn(curr_string, "\n") == 0) {
            continue;
        }
        curr_string[strcspn(curr_string, "\n")] = 0;
        strcat(lines[index++], curr_string);
    }

    return index;
}

// Print sorted lines
void print_lines(char lines[][MAX_STR_LEN], int n) {
    for (int i = 0; i < n; ++i) {
        printf("%s\n", lines[i]);
    }
}

// -------------------------------------------------

int read_int() {
	char buf[MAX_STR_LEN];
	int n;
	fgets(buf, MAX_STR_LEN, stdin);
	sscanf(buf, "%d", &n);
	return n;
}

int main(void) {
	int to_do = read_int();
	int n;
	Person persons[MAX_PERSONS];
	char lines[MAX_PERSONS][MAX_STR_LEN];
	switch (to_do) {
		case 1:
			n = read_person_array(persons);
			qsort(persons, (size_t)n, sizeof(Person), cmp_person);
			print_person_array(persons, n);
			break;
		case 2:
			n = read_lines(lines);
			qsort(lines, (size_t) n, MAX_STR_LEN, cmp_lines);
			print_lines(lines, n);
			break;
		default:
			printf("Nothing to do for %d\n", to_do);
			break;
	}
}

