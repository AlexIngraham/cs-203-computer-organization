// The main method for implementing the three push down automata.
//
// Modified by <your-name-here>
//

#include <stdio.h>

void print_use(const char* program) {
    printf("Usage: %s <pda-number> <string>\n", program);
    printf("  <pda-number> should be 1, 2, or 3 to select the PDA to run.\n");
    printf("  <string> is the input string to be evaluated by the PDA.\n");
    printf("Example: %s 2 aaabbb\n", program);
    printf("Note: Input strings should not contain spaces.\n");
}
int main(int argc, char *argv[]) {

    // The program will look something like the following
    // ./lab4 2 aaabbb
    // This will be passed to argv as three strings.
    // String 1 will be the name of the program "./pda"
    // String 2 will identify what pda will be run.
    // String 3 will provide the string that is passed to the pda.

    /** You will create a switch structure that will use string two **/
    /** to select the appropriate PDA.  Then the appropriate PDA    **/
    /** function will be called passing the third string as input.  **/
    /** Depending on the return code from the PDA function, print   **/
    /** if the string was valid or invalid.                         **/

    /** Validating input strings is not necessary, but might save   **/
    /** you time debugging your code.                               **/

    /** Additionally, you might consider unit-testing your PDA in   **/
    /** a different source file similar to the unit-tests for the   **/
    /** stack and list.         
     * 
     * 
     *                                     **/


     if (argc != 3) {
         print_use(argv[0]);
         return 1;
     }
     int pda_number = atoi(argv[1]);
        char *input_string = argv[2];

        if (pda_number < 1 || pda_number > 3) {
            printf("Error: PDA number must be 1, 2, or 3.\n");
            print_use(argv[0]);
            return 1;
        }

        int result = 0;

        switch (pda_number) {
            case 1:
                result = PDA1(input_string);
                break;
            case 2:
                result = PDA2(input_string);
                break;
            case 3:
                result = PDA3(input_string);
                break;
            default:
                // This should never happen due to the earlier check
                printf("Error: Invalid PDA number.\n");
                return 1;
        }

        if (result) {
            printf("Result: valid\n");
        } else {
            printf("Result: invalid\n");
        }
    return 0;
}
