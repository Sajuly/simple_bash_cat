#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "s21_cat.h"

void flag_up(int argc, char *argv[]);
void print_text();

int main(int argc, char *argv[]) {
    flag_up(argc, argv);
    print_text();
    return 0;
}

void flag_up(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == '-') {
                if (0 == strcmp("--number-nonblank", argv[i])) {
                    flag_b = 1;
                }
                if (0 == strcmp("--number", argv[i])) {
                    flag_n = 1;
                }
                if (0 == strcmp("--squeeze-blank", argv[i])) {
                    flag_s = 1;
                }
            } else {
                for (int j = 1; j < (int)strlen(argv[i]); j++) {
                    switch (argv[i][j]) {
                        case 'b': flag_b = 1; break;
                        case 'e': flag_e = 1; flag_v = 1; break;
                        case 'n': flag_n = 1; break;
                        case 's': flag_s = 1; break;
                        case 't': flag_t = 1; flag_v = 1; break;
                        case 'E': flag_e = 1; break;
                        case 'T': flag_t = 1; break;
                        case 'v': flag_v = 1; break;
                    }
                }
            }
        } else {
            strcpy(str[cnt], argv[i]);
            cnt++;
        }
    }
}

void print_text() {
    for (int i = 0; i < cnt; i++) {
        my_file = fopen(str[i], "r");
        if (my_file != NULL) {
            while ((char_from_file = fgetc(my_file)) != EOF) {
                if (new_str) {
                    if (char_from_file == '\n') {
                        empty_str = 1;
                    } else {
                        empty_str = 0;
                    }
                }
                if (flag_s) {
                    if (new_str && empty_str) {
                        empty_str_num++;
                    } else {
                        empty_str_num = 0;
                    }
                    if (empty_str_num == 2) {
                        empty_str_num = 1;
                        continue;
                    }
                }
                if (flag_n || flag_b) {
                    if (new_str) {
                        if (!(empty_str && flag_b)) {
                            printf("%6d\t", num);
                            new_str = 0;
                        } else {
                            if (flag_e) {
                                printf("$\n");
                            } else {
                                printf("\n");
                            }
                            continue;
                        }
                    }
                }
                if (char_from_file == '\n') {
                    new_str = 1;
                    if (!(empty_str && flag_b)) {
                        num++;
                    }
                } else {
                    new_str = 0;
                    empty_str = 0;
                }
                if (flag_e) {
                    if (char_from_file == '\n') {
                        printf("$\n");
                        continue;
                    }
                }
                if (flag_t) {
                    if (char_from_file == '\t') {
                        printf("^I");
                        continue;
                    }
                }
                if (flag_v) {
                    if (char_from_file < 32 && char_from_file !=9
                            && char_from_file != 10) {
                        char_from_file += 64;
                        printf("^");
                    }
                    if (char_from_file == 127) {
                        char_from_file -= 64;
                       printf("^");
                    }
                }
                printf("%c", char_from_file);
            }
            fclose(my_file);
        } else {
            printf("s21_cat: %s: %s\n", str[i], strerror(errno));
        }
    }
}
