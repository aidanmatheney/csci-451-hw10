#include "../include/hw10-program1.h"

#include "../include/util/StringBuilder.h"

#include "../include/util/file.h"
#include "../include/util/semaphore.h"
#include "../include/util/guard.h"

#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>

/**
 * Run CSCI 451 HW10 program 1.
 */
void hw10Program1(
    char const * const inputFilePath,
    char const * const semaphoreName,
    int const pipe1WriteFileDescriptor
) {
    guardNotNull(inputFilePath, "inputFilePath", "hw10Program1");
    guardNotNull(semaphoreName, "semaphoreName", "hw10Program1");

    FILE * const inputFile = safeFopen(inputFilePath, "r", "hw10Program1");
    sem_t * const semaphore = safeSemOpen(semaphoreName, "hw10Program1");
    FILE * const pipe1WriteFile = safeFdopen(pipe1WriteFileDescriptor, "w", "hw10Program1");

    // The semaphore is already locked

    while (true) {
        StringBuilder const currentWordBuilder = StringBuilder_create();
        bool atEof = true;

        char currentCharacter;
        while (safeFgetc(&currentCharacter, inputFile, "hw10Program1")) {
            atEof = false;

            if (currentCharacter == ' ' || currentCharacter == '\r' || currentCharacter == '\n') {
                break;
            }

            if (currentCharacter == ',' || currentCharacter == '.') {
                if (StringBuilder_length(currentWordBuilder) > 0) {
                    StringBuilder_appendChar(currentWordBuilder, currentCharacter);
                }
                break;
            }

            StringBuilder_appendChar(currentWordBuilder, currentCharacter);
        }

        if (atEof) {
            StringBuilder_destroy(currentWordBuilder);
            break;
        }

        if (StringBuilder_length(currentWordBuilder) == 0) {
            StringBuilder_destroy(currentWordBuilder);
            continue;
        }

        char * const currentWord = StringBuilder_toStringAndDestroy(currentWordBuilder);
        safeFprintf(pipe1WriteFile, "hw10Program1", "%s\n", currentWord);
        free(currentWord);
    }

    safeFprintf(pipe1WriteFile, "hw10Program1", "\n");
    safeSemPost(semaphore, "hw10Program1");

    fclose(inputFile);
    fclose(pipe1WriteFile);
}
