#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 1000
#define MAX_RANGE 1000

typedef struct {
	int first;
	int second;
} pair;

void insertion_sort(int* array, int n) {
    for (int i = 1; i < n; ++i) {
        int j = i;
        while (j > 0) {
            if (array[j] < array[j - 1]) {
                int temp = array[j - 1];
                array[j - 1] = array[j];
                array[j] = temp;
                --j;
            } else {
                break;
            }
        }
    }
}

// Add pair to existing relation if not already there
int add_relation (pair* relation, int n, pair new) {
    for (int i = 0; i < n; ++i) {
        if (relation[i].first == new.first && relation[i].second == new.second) {
            return n;
        }
    }
    relation[n].first = new.first;
    relation[n].second = new.second;
    ++n;
    return n;
}

// Case 1:
// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair* relation, int n) {
    int curr_x, found;

    for (int i = 0; i < n; ++i) {
        if (relation[i].first == relation[i].second) {
            continue;
        }

        for (int j = 0; j < 2; ++j) {
            if (j == 0) { curr_x = relation[i].first; } else { curr_x = relation[i].second; }
            found = 0;

            for (int k = 0; k < n; ++k) {
                if (relation[k].first == curr_x && relation[k].second == curr_x) {
                    found = 1;
                    break;
                }
            }

            if (!found) {
                return 0;
            }
        }
    }

    return 1;
}
// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair* relation, int n) {
    for (int i = 0; i < n; ++i) {
        if (relation[i].first == relation[i].second) {
            return 0;
        }
    }

    return 1;
}
// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair* relation, int n) {
    int found;

    for (int i = 0; i < n; ++i) {
        found = 0;
        for (int j = 0; j < n; ++j) {
            if (relation[j].first == relation[i].second && relation[j].second == relation[i].first) {
                found = 1;
                break;
            }
        }

        if (!found) { return 0; }
    }

    return 1;
}
// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair* relation, int n) {
    int found;

    for (int i = 0; i < n; ++i) {
        found = 0;
        for (int j = 0; j < n; ++j) {
            if (relation[j].first == relation[i].second && relation[j].second == relation[i].first) {
                found = 1;
                break;
            }
        }

        if (found && relation[i].first != relation[i].second) {
            return 0;
        }
    }

    return 1;
}
// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair* relation, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (relation[j].first == relation[i].second && relation[j].second == relation[i].first) {
                return 0;
            }
        }
    }

    return 1;
}
// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair* relation, int n) {
    int x, y, z, found;

    for (int i = 0; i < n; ++i) {
        x = relation[i].first;
        y = relation[i].second;

        for (int j = 0; j < n; ++j) {
            if (relation[j].first == y) {
                z = relation[j].second;

                found = 0;
                for (int k = 0; k < n; ++k) {
                    if (relation[k].first == x && relation[k].second == z) {
                        found = 1;
                        break;
                    }
                }

                if (!found) {
                    return 0;
                }
            }
        }
    }

    return 1;
}

// Case 2:
int get_domain(pair* relation, int n, int* domain) {
    int current = 0, found;

    for (int i = 0; i < n; ++i) {

        found = 0;
        for (int j = 0; j < current; ++j) {
            if (relation[i].first == domain[j]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            domain[current++] = relation[i].first;
        }

        found = 0;
        for (int j = 0; j < current; ++j) {
            if (relation[i].second == domain[j]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            domain[current++] = relation[i].second;
        }
    }

    insertion_sort(domain, current);

    return current;
}
// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair* relation, int n) {
    int domain[MAX_REL_SIZE];
    int sizeof_domain = get_domain(relation, n, domain);
    for (int i = 0; i < sizeof_domain; ++i) {
        int x = domain[i];
        for (int j = i + 1; j < sizeof_domain; ++j) {
            int y = domain[j];

            int found = 0;
            for (int k = 0; k < n; ++k) {
                if ((relation[k].first == x && relation[k].second == y) ||
                    (relation[k].second == x && relation[k].first == y)) {
                    found = 1;
                    break;
                }
            }

            if (!found) {
                return 0;
            }
        }
    }

    return 1;
}
// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair* relation, int n) {
    if (is_reflexive(relation, n) && is_transitive(relation, n) && is_antisymmetric(relation, n)) {
        return 1;
    } else return 0;
}
// A total order relation is a partial order relation that is connected
int is_total_order(pair* relation, int n) {
    if (is_partial_order(relation, n) && is_connected(relation, n)) return 1; else return 0;
}
int find_max_elements(pair* relation, int n, int* max_elements) {
    int index = 0;

    for (int i = 0; i < n; ++i) {
        int curr_element = relation[i].second, is_max = 1;
        for (int j = 0; j < n; ++j) {
            if (relation[j].first == curr_element && relation[j].second != curr_element) {
                is_max = 0;
                break;
            }
        }

        if (is_max) {
            int found = 0;
            for (int j = 0; j < index; ++j) {
                if (max_elements[j] == curr_element) {
                    found = 1;
                    break;
                }
            }

            if (!found) max_elements[index++] = curr_element;
        }
    }

    insertion_sort(max_elements, index);

    return index;
}
int find_min_elements(pair* relation, int n, int* min_elements) {
    int index = 0;

    for (int i = 0; i < n; ++i) {
        int curr_element = relation[i].first, is_min = 1;
        for (int j = 0; j < n; ++j) {
            if (relation[j].second == curr_element && relation[j].first != curr_element) {
                is_min = 0;
                break;
            }
        }

        if (is_min) {
            int found = 0;
            for (int j = 0; j < index; ++j) {
                if (min_elements[j] == curr_element) {
                    found = 1;
                    break;
                }
            }

            if (!found) min_elements[index++] = curr_element;
        }
    }

    insertion_sort(min_elements, index);

    return index;
}


// Case 3:
int composition (pair* relation_1, int n1, pair* relation_2, int n2, pair* comp) {
    int n_comp = 0;
    for (int i = 0; i < n1; ++i) {
        int x = relation_1[i].first;
        int y = relation_1[i].second;
        for (int j = 0; j < n2; ++j) {
            int z = relation_2[j].second;
            if (y == relation_2[j].first) {
                comp[n_comp].first = x;
                comp[n_comp].second = z;
                ++n_comp;
                break;
            }
        }
    }

    return n_comp;
}

int cmp (pair p1, pair p2) {
	if (p1.first == p2.first) return p1.second - p2.second;
	return p1.first - p2.first;
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair* relation) {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &relation[i].first);
        scanf("%d", &relation[i].second);
    }
    return n;
}

void print_int_array(int *array, int n) {
	printf("%d\n", n);
	for (int i = 0; i < n; ++i) {
		printf("%d ", array[i]);
	}
	printf("\n");
}

int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];

	scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;

	switch (to_do) {
		case 1:
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size));
			break;
		case 2:
			ordered = is_partial_order(relation, size);
			n_domain = get_domain(relation, size, domain);
			printf("%d %d\n", ordered, is_total_order(relation, size));
			print_int_array(domain, n_domain);
			if (!ordered) break;
			int max_elements[MAX_REL_SIZE];
			int min_elements[MAX_REL_SIZE];
			int no_max_elements = find_max_elements(relation, size, max_elements);
			int no_min_elements = find_min_elements(relation, size, min_elements);
			print_int_array(max_elements, no_max_elements);
			print_int_array(min_elements, no_min_elements);
			break;
		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size,
			   relation_2, size_2, comp_relation));
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

