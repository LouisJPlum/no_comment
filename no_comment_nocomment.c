#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024
#define MAX_FILENAME_LENGTH 256

void filenameSplit(const char *fullname, char *filename, char *extension) {
    const char *last_dot = strrchr(fullname, '.');
    if (last_dot != NULL && last_dot != fullname) {
        strncpy(filename, fullname, last_dot - fullname);
        filename[last_dot - fullname] = '\0';
        strcpy(extension, last_dot);
    } else {
        strcpy(filename, fullname);
        extension[0] = '\0';
    }
}

int main() {
    FILE *fileInput, *fileOutput;

    printf("input file name: ");
    char filenameInput[MAX_FILENAME_LENGTH];
    fgets(filenameInput, sizeof(filenameInput), stdin); 
    filenameInput[strcspn(filenameInput, "\n")] = '\0';
    fileInput = fopen(filenameInput, "r");
    if (fileInput == NULL) {
        perror("Error");
        return 1;
    }

    char filenameOutput[MAX_FILENAME_LENGTH];
    char extension[MAX_FILENAME_LENGTH];
    filenameSplit(filenameInput,filenameOutput,extension);
    strcat (filenameOutput, "-nocomment");
    strcat (filenameOutput, extension);
    fileOutput = fopen(filenameOutput, "w");
    if (fileOutput == NULL) {
        perror("Error");
        fclose(fileInput);
        return 1;
    }

    char lineData[MAX_LINE_LENGTH];
    char *slashPosition;
    while (fgets(lineData, sizeof(lineData), fileInput) != NULL) {
        slashPosition = strstr(lineData, "//");
        if (slashPosition != NULL) {
            *slashPosition = '\n';
            *(slashPosition + 1) = '\0';
        } 
        fprintf(fileOutput, "%s", lineData);
    }

    fclose(fileInput);
    fclose(fileOutput);
    printf("Operation successful\n");
    return 0;
}