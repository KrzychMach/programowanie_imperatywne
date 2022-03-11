#include <stdio.h>
#include <math.h> // for NAN

#define STACK_SIZE 10

#define OK        0 
#define EMPTY    -1
#define FULL     -2
#define OVERFLOW -3


//TODO
//  cbuff


double stack[STACK_SIZE];
int top = 0;

int stack_push(double x){
    if (top == STACK_SIZE){
        return OVERFLOW;
    } else {
        stack[top++] = x;
        return OK;
    }
}

double stack_pop(void){
    if (top == 0){
        return NAN;
    } else {
        return stack[--top];
    }
}

int stack_state(void){
    if (top == STACK_SIZE){
        return FULL;
    } else {
        return top;
    }
}

// FIFO queue with shifts

#define QUEUE_SIZE 10

int queue[QUEUE_SIZE];
int in = 0, curr_nr = 0;

// ONE OVERFLOW for many resigning

void queue_push(int in_nr){
    for (int i = 0; i < in_nr; ++i) {
        if (in == QUEUE_SIZE){
            printf("%s ", "OVERFLOW");
            curr_nr += in_nr - i;
            return;
        } else {
            queue[in++] = ++curr_nr;
        }
    }
}

void queue_pop(int out_nr){
    if (out_nr > in) {
        printf("%s ", "UNDERFLOW");
        in = 0;
        return;
    } else {
        for (int i = 0; i < in; ++i) {
            queue[i] = queue[out_nr + i];
        }
        in = in - out_nr;
    }
}

void queue_state(void){
    if (in == 0){
        printf("%s ", "EMPTY");
    } else {
        for (int i = 0; i < in; ++i) {
            printf("%d ", queue[i]);
        }
    }
}

// Queue with cyclic buffer

#define CBUFF_SIZE 10

int cbuff[CBUFF_SIZE];
int out=0, len=0;


void cbuff_push(int cli_nr){
    if (len == CBUFF_SIZE){
        printf("%s ", "OVERFLOW");
    } else {
        cbuff[(out + len++) % CBUFF_SIZE] = cli_nr;
    }
}

void cbuff_pop(void){
    if (len == 0){
        printf("%s ", "UNDERFLOW");
    } else {
        out = ++out % CBUFF_SIZE;
        --len;
    }
}

void cbuff_state(void){
    if (len == 0){
        printf("%s ", "EMPTY");
    } else {
        for (int i = 0; i < len; ++i) {
            printf("%d ", cbuff[(out + i) % CBUFF_SIZE]);
        }
    }
}


int main(void) {
	int to_do, n, client_no, op_code;
	double x;
	scanf("%d",&to_do);
	switch(to_do) {
		case 1: // stack
			do {
				scanf("%lf",&x);
				if(x > 0) stack_push(x);
				else {
					if(x < 0) printf("%.2f ", stack_pop());
					else printf("\n%d\n", stack_state());
				} 
			} while(x != 0);
			break;
		case 2: // FIFO queue with shifts
			do {
				scanf("%d",&n);
				if(n > 0) queue_push(n);
				else {
					if(n < 0) queue_pop(-n);
					else queue_state();
				}
			} while(n != 0);
			break;
		case 3: // queue with cyclic buffer
			client_no = 0;
			do {
				scanf("%d",&op_code);
				if(op_code > 0) cbuff_push(++client_no);
				else {
					if(op_code < 0) cbuff_pop();
					else cbuff_state();
				}
			} while(op_code != 0);
			break;
		default: 
			printf("NOTHING TO DO!\n");
	}
	return 0;
}

