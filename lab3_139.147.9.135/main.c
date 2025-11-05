#include <stdio.h>

// Prototypes for the four functions that will implement the state machines.
int FMS1(char *str);
int FMS2(char *str);
int FMS3(char *str);
int FMS4(char *str);

// enum representing the different states, this will be used
// by your state machines to specify the current state.
enum STATE {
    S0, S1, S2, S3
};

// global variables re  presenting the input alphabet
// this will be used by your state machines
char a = 'a';
char b = 'b';
char c = 'c';
char d = 'd';
char e = 'e';

int main() {
    char *input_str = NULL;
    
    // Array of test strings for comprehensive testing
    char *test_strings[] = {    
        "aa", "abc", "ace", "bcd", "abcde", "ed", "ddd", "ae"
    };
    
    // Test each string with all FSMs
    for(int i = 0; i < 8; i++) {
        /** modify the following for your input strings **/
        input_str = test_strings[i];
        
        printf("Testing string: \"%s\"\n", input_str);
        
        if(FMS1(input_str)) printf("FMS1: %s is valid\n", input_str);
        else printf("FMS1: %s is invalid\n", input_str);
        
        if(FMS2(input_str)) printf("FMS2: %s is valid\n", input_str);
        else printf("FMS2: %s is invalid\n", input_str);
        
        if(FMS3(input_str)) { printf("FMS3: %s is valid\n", input_str); }
        else                { printf("FMS3: %s is invalid\n", input_str); }
        
        if(FMS4(input_str)) { printf("FMS4: %s is valid\n", input_str); }
        else                { printf("FMS4: %s is invalid\n", input_str); }
        
        printf("\n");
    }
    
    return 0;
}
/*
table:
current, input, input
S0, a, S2
S0, b, S0

S1, a, S3
S1, b, S0

S2, a, S2
S2, b, S1

S3, a, S2
S3, b, S3

accepting state: S3

*/
int FMS1(char *str) { // ! if-else only implementation

    enum STATE curr_state = S0;
    int i = 0;
    while(str[i] != '\0'){
        char curr_char = str[i];
        if(curr_state == S0){
            if (curr_char == a) {
                curr_state = S2;
            }
            else if (curr_char == b) {
                curr_state = S0;
            }
    }
    else if (curr_state == S1){
            if (curr_char == a) {
                curr_state = S3;
            }
            else if (curr_char == b) {
                curr_state = S0;
            }
    }
    else if (curr_state == S2){
            if (curr_char == a) {
                curr_state = S2;
            }
            else if (curr_char == b) {
                curr_state = S1;
            }
    }
    else if (curr_state == S3){
            if (curr_char == a) {
                curr_state = S2;
            }
            else if (curr_char == b) {
                curr_state = S3;
            }
    }
        i++;
    }


    if (curr_state == S3) {
        return 1;
    } else {
        return 0;
    }
}
/*
table:
current, input, input
S0, a, S1
S0, b, S0
S0, c, S0

S1, a, S0
S1, b, S2
S1, c, S1

S2, a, S2
S2, b, S1
S2, c, S3

S3, a, S3
S3, b, S3
S3, c, S2

accepting state: S3
*/
int FMS2(char *str) {

    // there can only be one stored state
    enum STATE curr_state = S0;
    int  i = 0;


    while (str[i] != '\0') {
        char curr_char = str[i];

        if (curr_state == S0) {
            if (curr_char == c || curr_char == b) {
                curr_state = S0;
            } else if (curr_char == a) {
                curr_state = S1;
            }
        }
        else if (curr_state == S1) {
            if (curr_char == a) {
                curr_state = S0;
            } else if (curr_char == b) {
                curr_state = S2;
            } else if (curr_char == c) {
                curr_state = S1;
            }
        }
        else if (curr_state == S2) {
            if (curr_char == a) {
                curr_state = S2;
            } else if (curr_char == b) {
                curr_state = S1;
            } else if (curr_char == c) {
                curr_state = S3;
            }
        }
        else if (curr_state == S3) {
            if (curr_char == a || curr_char == b) {
                curr_state = S3;
            } else if (curr_char == c) {
                curr_state = S2;
            }
        }
        i++;
    }
    if (curr_state == S3) {
        return 1;
    } else {
        return 0;
    }
}

/*
table:
current, input, input
S0, a, S2
S0, b, S1
S0, c, S0

S1, a, S0
S1, b, S2
S1, c, S1

S2, a, S1
S2, b, S0
S2, c, S2

accepting state: S2
*/
int FMS3(char *str) {

    // there can only be one stored state
    enum STATE curr_state = S0;
    int i = 0;

    while (str[i] != '\0') {
        char curr_char = str[i];
        switch (curr_state) {
            case S0:
            if (curr_char == a) {
                curr_state = S2;
            } else if (curr_char == b) {
                curr_state = S1;
            } else if (curr_char == c) {
                curr_state = S0;
            }
            break;
            case S1:
            if (curr_char == a) {
                curr_state = S0;
            } else if (curr_char == b) {
                curr_state = S2;
            } else if (curr_char == c) {
                curr_state = S1;
            }
            break;
            case S2:
            if (curr_char == a) {
                curr_state = S1;
            } else if (curr_char == b) {
                curr_state = S0;
            } else if (curr_char == c) {
                curr_state = S2;
            }
            break;
            case S3:
            break;
        }
        i++;
    }

    if (curr_state == S2) {
        return 1;
    } else {
        return 0;
    }
}

/*
table:
current, input, input
S0, a, S3
S0, b, S0
S0, c, S2
S0, d, S2
S0, e, S0

S1, a, S2
S1, b, S1
S1, c, S0
S1, d, S3
S1, e, S1

S2, a, S2
S2, b, S1
S2, c, S3
S2, d, S0
S2, e, S2

S3, a, S3
S3, b, S0
S3, c, S1
S3, d, S2
S3, e, S3

accepting states: S3

*/
int FMS4(char *str) {

    // there can only be one stored state
    enum STATE curr_state = S0;
    int i = 0;

    while (str[i] != '\0') {
        char curr_char = str[i];
        switch (curr_state) {
            case S0:
                if (curr_char == a) {
                    curr_state = S3;
                } else if (curr_char == b || curr_char == e) {
                    curr_state = S0;
                } else if (curr_char == c || curr_char == d) {
                    curr_state = S2;
                }
                break;
            case S1:
                if (curr_char == a) {
                    curr_state = S2;
                } else if (curr_char == b || curr_char == e) {
                    curr_state = S1;
                } else if (curr_char == c) {
                    curr_state = S0;
                } else if (curr_char == d) {
                    curr_state = S3;
                }
                break;
            case S2:
                if (curr_char == a || curr_char == e) {
                    curr_state = S2;
                } else if (curr_char == b) {
                    curr_state = S1;
                } else if (curr_char == c) {
                    curr_state = S3;
                } else if (curr_char == d) {
                    curr_state = S0;
                }
                break;
            case S3:
                if (curr_char == a || curr_char == e) {
                    curr_state = S3;
                } else if (curr_char == b) {
                    curr_state = S0;
                } else if (curr_char == c) {
                    curr_state = S1;
                } else if (curr_char == d) {
                    curr_state = S2;
                }
                break;
        }
        i++;
    }

    if (curr_state == S3) {
        return 1;
    } else {
        return 0;
    }
}