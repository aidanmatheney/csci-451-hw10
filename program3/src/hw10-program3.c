#include "../include/hw10-program3.h"

#include "../include/util/file.h"
#include "../include/util/guard.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * Run CSCI 451 HW10 program 3.
 */
void hw10Program3(
    char const * const type1WordCountInputFilePath,
    char const * const type2WordCountInputFilePath,
    char const * const outputFilePath,
    int const pipe2ReadFileDescriptor
) {
    guardNotNull(type1WordCountInputFilePath, "type1WordCountInputFilePath", "hw10Program3");
    guardNotNull(type2WordCountInputFilePath, "type2WordCountInputFilePath", "hw10Program3");
    guardNotNull(outputFilePath, "outputFilePath", "hw10Program3");

    FILE * const type1WordCountInputFile = safeFopen(type1WordCountInputFilePath, "r", "hw10Program3");
    FILE * const type2WordCountInputFile = safeFopen(type2WordCountInputFilePath, "r", "hw10Program3");

    char * const type1WordCountString = readFileLine(type1WordCountInputFile);
    char * const type2WordCountString = readFileLine(type2WordCountInputFile);

    unsigned int const type1WordCount = (unsigned int)strtoul(type1WordCountString, NULL, 10);
    unsigned int const type2WordCount = (unsigned int)strtoul(type2WordCountString, NULL, 10);

    free(type1WordCountString);
    free(type2WordCountString);

    fclose(type1WordCountInputFile);
    fclose(type2WordCountInputFile);

    printf("Type 1: %u\nType 2: %u\n", type1WordCount, type2WordCount);

    FILE * const pipe2ReadFile = safeFdopen(pipe2ReadFileDescriptor, "r", "hw10Program1");
    FILE * const outputFile = safeFopen(outputFilePath, "w", "hw10Program3");

    bool wroteFirstWord = false;
    while (true) {
        char * const currentPigLatinWord = readFileLine(pipe2ReadFile);
        if (currentPigLatinWord == NULL) {
            break;
        }
        if (currentPigLatinWord[0] == '\0') {
            free(currentPigLatinWord);
            break;
        }

        safeFprintf(
            outputFile,
            "hw10Program3",
            wroteFirstWord ? " %s" : "%s",
            currentPigLatinWord
        );
        wroteFirstWord = true;

        free(currentPigLatinWord);
    }

    safeFprintf(outputFile, "hw10Program3", "\n");

    fclose(pipe2ReadFile);
    fclose(outputFile);
}
