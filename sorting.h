#ifndef SORTING_H
#define SORTING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>

#define POISON -1
//#define DEBUG_MODE

#ifdef DEBUG_MODE
#define DEBUG(...) __VA_ARGS__
#else
#define DEBUG(...)
#endif

enum ErrSuc{
    RETURN_POISON = -1,
    RETURN_SUCCESS = 0,
    RETURN_ERROR = 1,
    RETURN_ERROR_FILEOPEN = 2,
    RETURN_ERROR_FSTAT = 3,
    RETURN_ERROR_FILEREAD = 4,
    RETURN_ERROR_FILECREATE = 5,
    RETURN_ERROR_FILEWRITE_SORT_BEG = 6,
    RETURN_ERROR_TOO_LARGE_FILE_SIZE = 7,
    RETURN_ERROR_MEMORY_ALLOCATION = 8,
    RETURN_ERROR_FILEADD_SORT_END = 9,
    RETURN_ERROR_FILEWRITE_SORT_END = 10,
    RETURN_ERROR_FILEADD_UNS = 11,
    RETURN_ERROR_FILEWRITE_UNS = 12,
};

struct Line{
    char *line;
    size_t len;
};


struct Info{
    const char *fileName;
    int fileInDescriptor;
    FILE *fileOut;
\
    char *buffer;
    size_t bufSize;
\
    Line* arrLines;
    size_t arrSize;
\
    ErrSuc error;
};

#include "QuickSort.cpp"


ErrSuc OpenInFile(Info *programInfo);
ErrSuc ReadInFile(Info *programInfo);
ErrSuc BufferToLinesFragmentation(Info *programInfo);

void PrintfError(ErrSuc error);

void ChangeEndlsToNulls(Info *programInfo);
void ChangeNullsToEndls(Info *programInfo);

void ProgramDestroy(Info *programInfo DEBUG(, int fileOutUnsortedDescriptor, int fileOutInOneLineDescriptor));

void PrintfError(ErrSuc error){
    switch (error){
        case (RETURN_POISON):
            printf("Poison value, the struct has been destroyed\n");
            break;
        case (RETURN_ERROR_FILEOPEN):
            printf("Error in input file opening in fuction open(), maybe you should check console arguments\n");
            break;
        case (RETURN_ERROR_FSTAT):
            printf("Error in input file opening in fuction fstat()\n");
            break;
        case (RETURN_ERROR_FILEREAD):
            printf("Error in input file reading in fuction read()\n");
            break;
        case (RETURN_ERROR_FILECREATE):
            printf("Error in input file creating or opening in fuction fopen() in sorting by beginning, maybe you should check console arguments\n");
            break;
        case (RETURN_ERROR_FILEWRITE_SORT_BEG):
            printf("Error in input file writing in fuction fprintf() in sorting by beginning\n");
            break;
        case (RETURN_ERROR_TOO_LARGE_FILE_SIZE):
            printf("Input file is too large\n");
            break;
        case (RETURN_ERROR_MEMORY_ALLOCATION):
            printf("Error in memory allocation\n");
            break;
        case (RETURN_ERROR_FILEADD_SORT_END):
            printf("Error in input file opening in fuction fopen() in sorting by ending, maybe you should check console arguments\n");
            break;
        case (RETURN_ERROR_FILEWRITE_SORT_END):
            printf("Error in input file writing in fuction fprintf() in sorting by ending\n");
            break;
        case (RETURN_ERROR_FILEADD_UNS):
            printf("Error in input file opening in fuction fopen() in outputting unsorted text, maybe you should check console arguments\n");
            break;
        case (RETURN_ERROR_FILEWRITE_UNS):
            printf("Error in input file writing in fuction fwrite() outputting unsorted text\n");
            break;
        default:
            printf("no errors:)");
            break;
    }
}

ErrSuc OpenInFile(Info *programInfo){
    assert(programInfo != NULL);
    assert(&(programInfo->bufSize) != 0);
    
    programInfo->fileInDescriptor = open(programInfo->fileName, O_RDONLY);
    if (programInfo->fileInDescriptor == -1) {return RETURN_ERROR_FILEOPEN;}

    struct stat fileStat = {};
    if (fstat(programInfo->fileInDescriptor, &fileStat) == -1) {return RETURN_ERROR_FSTAT;}

    programInfo->bufSize = fileStat.st_size;
    if (programInfo->bufSize >= (size_t)1<<32) {return RETURN_ERROR_TOO_LARGE_FILE_SIZE;};

    return RETURN_SUCCESS;
}

ErrSuc ReadInFile(Info *programInfo){
    assert(programInfo != NULL);

    char *bufferBeta = (char *)calloc(programInfo->bufSize + 2, sizeof(char));

    if (bufferBeta == NULL) {return RETURN_ERROR_MEMORY_ALLOCATION;};

    DEBUG(printf("bufBeta = %p\n", bufferBeta);)

    programInfo->buffer = bufferBeta + 1;
    
    DEBUG(printf("buf = %p\n", programInfo->buffer);)

    if (read(programInfo->fileInDescriptor, programInfo->buffer, programInfo->bufSize) <= 0){
        close(programInfo->fileInDescriptor);

        return RETURN_ERROR_FILEREAD;
    }
    
    close(programInfo->fileInDescriptor);

    return RETURN_SUCCESS;
}

ErrSuc OutputUnsortedText(Info *programInfo){
    assert(programInfo != NULL);
    assert(programInfo->buffer != NULL);

    if(programInfo->fileOut == NULL){
        programInfo->fileOut /*_Descriptor*/ = fopen("output.txt", "a"/*O_CREAT | O_RDWR | O_TRUNC, 0666*/);
    }

    if (programInfo->fileOut /*_Descriptor*/ == NULL){
        fclose(programInfo->fileOut /*_Descriptor*/);
        return RETURN_ERROR_FILEADD_UNS;
    }

    ChangeNullsToEndls(programInfo);

    fputs("===Unsorted=Text===\n\n", programInfo->fileOut);

    fwrite(programInfo->buffer, sizeof(char), programInfo->bufSize, programInfo->fileOut);

    fputs("\n", programInfo->fileOut);

    //fclose(programInfo->fileOut /*_Descriptor*/);
    return RETURN_SUCCESS;
}

void ChangeEndlsToNulls(Info *programInfo){
    assert(programInfo != NULL);

    size_t endlCount = 0;

    for (size_t index = 0; index < programInfo->bufSize; index++){
        if (programInfo->buffer[index] == '\r'){

            programInfo->buffer[index] = '\0';
        }
        if (programInfo->buffer[index] == '\n'){

            endlCount++;
            programInfo->buffer[index] = '\0';

            while((index + 1) < programInfo->bufSize && programInfo->buffer[index + 1] == '\n'){

                programInfo->buffer[index + 1] = '\0';
                index++;
            }
        }
    }
    programInfo->arrSize = endlCount;
}

void ChangeNullsToEndls(Info *programInfo){

    for (size_t index = 0; index < programInfo->bufSize; index++){
        if (programInfo->buffer[index] == '\0'){

            programInfo->buffer[index] = '\n';
        }
    }
}



ErrSuc BufferToLinesFragmentation(Info *programInfo){
    assert(programInfo->buffer != NULL);

    DEBUG(printf("buffer = [%s]\n", programInfo->buffer);
          printf("arrLines ptr = %p\n", programInfo->arrLines);
          printf("bufSize = %llu\n", programInfo->bufSize);
         )

    programInfo->arrLines = (Line *)calloc(programInfo->arrSize, sizeof(Line));
    
    if(programInfo->arrLines == NULL){
        return RETURN_ERROR_MEMORY_ALLOCATION;
    }

    int linesIndex = 0;

    for (ssize_t index = -1; index < (ssize_t)(programInfo->bufSize - 1); index++){
        
        if (programInfo->buffer[index] == '\0'){
            while((index + 1) < (ssize_t)programInfo->bufSize && programInfo->buffer[index + 1] == '\0'){
                index++;
            }
            programInfo->arrLines[linesIndex].line = programInfo->buffer + (index + 1);
            linesIndex++;
        }
    }
    for (size_t index = 0; index < programInfo->arrSize; index++){
        if (programInfo->arrLines[index].line == 0){

            programInfo->arrLines[index].len = 0;
        }
        else{
            if(index == 0){
                programInfo->arrLines[index].len = strlen(programInfo->arrLines[index].line);
            }//TODO - 
            else{
                programInfo->arrLines[index].len = programInfo->arrLines[index + 1].line - programInfo->arrLines[index].line - 1;
            }
        }
    }
    return RETURN_SUCCESS;
}

void ProgramDestroy(Info *programInfo DEBUG(, int fileOutInOneLineDescriptor, int fileOutUnsortedDescriptor)){
    DEBUG(close(fileOutInOneLineDescriptor);)
    DEBUG(close(fileOutUnsortedDescriptor);)
    
    fclose(programInfo->fileOut);

    free(programInfo->buffer - 1);
    free(programInfo->arrLines);

    programInfo->arrSize = POISON;
    programInfo->fileInDescriptor = POISON;
    programInfo->bufSize = POISON;
    programInfo->error = RETURN_POISON;
    programInfo->fileOut = NULL;
}

#endif