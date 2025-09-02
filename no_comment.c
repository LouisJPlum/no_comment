#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024
#define MAX_FILENAME_LENGTH 256

void filenameSplit(const char *fullname, char *filename, char *extension) {
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
    FILE *fileInput, *fileOutput;

    // prepare input file
    printf("input file name: ");
    char filenameInput[MAX_FILENAME_LENGTH];
    fgets(filenameInput, sizeof(filenameInput), stdin); // input file name using keyboard
    filenameInput[strcspn(filenameInput, "\n")] = '\0'; // remove trailing newline from fgets
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

    // edit
    char lineData[MAX_LINE_LENGTH];
    char *slashPosition;
    while (fgets(lineData, sizeof(lineData), fileInput) != NULL) { // read file by lines
        slashPosition = strstr(lineData, "//"); // find first "//"" in line
        if (slashPosition != NULL) {
            *slashPosition = '\n'; // replace >/</ with newline
            *(slashPosition + 1) = '\0'; // replace />/< with null
        } 
        fprintf(fileOutput, "%s", lineData); // write to file
    }

    // close files
    fclose(fileInput);
    fclose(fileOutput);
    printf("Operation successful\n");
    return 0;
}