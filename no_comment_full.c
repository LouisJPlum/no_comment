#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 1024
#define MAX_FILENAME_LENGTH 64

void filenameSplit(const char* fullname, char* filename, char* extension) {
    const char *last_dot = strrchr(fullname, '.');
    if (last_dot != NULL && last_dot != fullname) { // a dot was found, and it's not the first character
        strncpy(filename, fullname, last_dot - fullname);
        filename[last_dot - fullname] = '\0'; // null-terminate the filename
        strcpy(extension, last_dot);
    } else { // no dot found, or .file
        strcpy(filename, fullname);
        extension[0] = '\0'; // set extension to empty string
    }
}

int main() {
    FILE* fileInput;
    FILE* fileOutput;

    // prepare input file
    printf("input file name: ");
    char filenameInput[MAX_FILENAME_LENGTH];
    fgets(filenameInput, sizeof(filenameInput), stdin); // input file name using keyboard
    filenameInput[strcspn(filenameInput, "\n")] = '\0'; // remove trailing \n from fgets
    fileInput = fopen(filenameInput, "r"); // open input file in read mode
    if (fileInput == NULL) {
        perror("Error");
        return 1;
    }

    // prepare output file
    char filenameOutput[MAX_FILENAME_LENGTH];
    char extension[MAX_FILENAME_LENGTH];
    filenameSplit(filenameInput,filenameOutput,extension); // call function
    strcat (filenameOutput, "-nocomment"); // add "-nocomment" to file name
    strcat (filenameOutput, extension); // add extension to filename
    fileOutput = fopen(filenameOutput, "w"); // create & open output file in write mode
    if (fileOutput == NULL) {
        perror("Error");
        fclose(fileInput);
        return 1;
    }

    //edit
    char lineData[MAX_LINE_LENGTH];
    char key[] = "//";
    if (strcmp(extension, ".py") == 0) {strcpy(key, "#\0");} // python does have a different syntax on commenting
    char startKey[] = "/*";
    char endKey[] = "*/";
    char* comment;
    char* start;
    char* end;
    bool inPara = false;
    while(fgets(lineData, sizeof(lineData), fileInput) != NULL) {
        comment = strstr(lineData, key); // find first "//" in line (for python is "#")
        start = strstr(lineData, startKey); // find first "/*" in line
        end = strstr(lineData, endKey); // find first "*/" in line
        printf("%s", lineData);
        if (inPara == false) {
            if (comment != NULL) {
                *comment = '\n'; // replace with \n
                *(comment + 1) = '\0'; // null-ternimate the lineData
                fprintf(fileOutput, "%s", lineData);
                printf("1\n%s\n",lineData);
            }
            else if (start != NULL && end != NULL) { // single line case
                *start = '\n';
                *(start + 1) = '\0';
                fprintf(fileOutput, "%s", lineData);
                printf("2\n%s\n",lineData);
            }
            else if (start != NULL && end == NULL) {
                inPara = true;
                *start = '\0';
                printf("3\n%s\n",lineData);} // paragraph starting line located
            else {
                fprintf(fileOutput, "%s", lineData);
                printf("4\n%s\n",lineData); // no comment found, write original
            }
        } else {
            if (end != NULL) { // paragraph ending line located
                inPara = false;
                fprintf(fileOutput, "\n"); // write to file
                printf("5\n\n");
            }
            else{
                printf("6\n\n");
            }
        }
    }

    // close files
    fclose(fileInput);
    fclose(fileOutput);
    printf("Operation successful\n");
    return 0;
}
