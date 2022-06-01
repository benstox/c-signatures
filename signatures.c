#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#define DOC_PAGES_PER_SHEET 4
#define SHEETS_PER_SIGNATURE 4
#define SEPARATOR "###############################\n"
#define ALPHABET_LEN 26
#define ASCII_A 65


char * prepend_char(char * str, char letter) {
    // Prepend a char to the beginning of a string.
    size_t str_len = strlen(str) + 1;
    str = realloc(str, str_len + 1);
    for (int i = 0; i < str_len; i++) {
        str[str_len - i] = str[str_len - i - 1];
    }
    str[0] = letter;
    return str;
}


int main(int argc, char * argv[]) {
    if (argc < 3) {
        printf("Need at least two arguments:\n");
        printf("* a first page number,\n");
        printf("* a last page number,\n");
        printf("* and optionally, a number of sheets per signature.\n");
        return -1;
    }

    char * first_arg = argv[1];
    char * second_arg = argv[2];
    int sheets_per_signature;
    if (argc >= 4) {
        char * third_arg = argv[3];
        sheets_per_signature = strtol(third_arg, NULL, 10);
        if (sheets_per_signature <= 0) {
            printf("The number of sheets per signature (third argument) must be an integer greater than zero!\n");
            return -1;
        }
    } else {
        sheets_per_signature = SHEETS_PER_SIGNATURE;
    }
    int first_page_number = strtol(first_arg, NULL, 10);
    int last_page_number = strtol(second_arg, NULL, 10);
    printf("Document first page: %d\n", first_page_number);
    printf("Document last page: %d\n", last_page_number);
    printf("Number of sheets per signature: %d\n", sheets_per_signature);
    if (first_page_number <= 0) {
        printf("The first page number (first argument) must be an integer greater than zero!\n");
        return -1;
    }
    if (last_page_number < first_page_number) {
        printf("The last page number (second argument) must be an integer greater than or equal to the first page number!\n");
        return -1;
    }

    int num_pages = last_page_number - first_page_number + 1;
    printf("Number of pages:  %d\n", num_pages);

    int num_sheets = ceil(1.0 * num_pages / DOC_PAGES_PER_SHEET);
    printf("Number of sheets: %d\n", num_sheets);

    int num_signatures = ceil(1.0 * num_sheets / sheets_per_signature);
    printf("Number of signatures: %d\n", num_signatures);
    printf(SEPARATOR);

    for (int i = 0; i < num_signatures; ++i) {
        // Get the alphabetic code for this signature.
        // In an ASCII table 'A' is 65.
        // Initialize the code as a string of a single byte equal to the null terminator
        char * alphabetic_code = malloc(1);
        alphabetic_code[0] = '\0';
        int j = i;
        int offset;
        char new_char;
        while (1) {
            offset = j % ALPHABET_LEN;
            new_char = (char) ASCII_A + offset;
            alphabetic_code = prepend_char(alphabetic_code, new_char);
            j = j / 26;
            if (j == 0) {
                // the alphabetic code has been built, so...
                break;
            }
            j = j - 1;
        }
        // first page of this signature
        int sig_first_page = first_page_number + (i * DOC_PAGES_PER_SHEET * sheets_per_signature);
        // last page of this signature
        int sig_last_page = sig_first_page + (DOC_PAGES_PER_SHEET * sheets_per_signature) - 1;
        if (sig_last_page > last_page_number) {
            sig_last_page = last_page_number;
        }
        printf("Signature %s. First page: %d, last page %d\n", alphabetic_code, sig_first_page, sig_last_page);
        free(alphabetic_code);
    }
    printf(SEPARATOR);
    return 0;
}

