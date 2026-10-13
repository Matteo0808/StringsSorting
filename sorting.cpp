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
    RETURN_ERROR_CONSOLE_INPUT = 13,
};

enum OutputMode{
    MODE_NONE = 0,
    MODE_SORT_UNSORTED = 1,
    MODE_SORT_BEGINNING = 2,
    MODE_SORT_ENDING = 3,
    MODE_SORT_ALL = 4,
    MODE_SORT_ONLY = 5
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
    OutputMode mode;
};

#include "QuickSort.cpp"

const char *fileInNameDefault = "onegin.txt";

const char *CheckConsoleArguments(int argc, char *argv[], Info *programInfo);
bool IsMode(char * consoleArg, Info *programInfo);

ErrSuc OpenInFile(Info *programInfo);
ErrSuc ReadInFile(Info *programInfo);
ErrSuc BufferToLinesFragmentation(Info *programInfo);
ErrSuc SortingByBeginnings(Info *programInfo);
ErrSuc SortingByEndings(Info *programInfo);
ErrSuc OutputUnsortedText(Info *programInfo);

void PrintfError(ErrSuc error);

void ChangeEndlsToNulls(Info *programInfo);
void ChangeNullsToEndls(Info *programInfo);

void ProgramDestroy(Info *programInfo DEBUG(, int fileOutUnsortedDescriptor, int fileOutInOneLineDescriptor));


//TODO - struct!!!!!!!!!!!!!!! 
///@note completed

//TODO - reverse sort
///@note completed

//TODO - virginity text output
///@note completed fuck you bitch

//TODO: main clear
///@note completed

//TODO: onegin 2 strings - WORKING!!!
///@note completed

//TODO: isolate comparator - WORKING!!!
///@note completed

//TODO: qsort() standart - my qsort ne workaet - vse, teper' workaet
///@note completed

//TODO: before write runtroughtbuffer \0 -> \n - found more optimal solution
///@note completed

// TODO: if style fix с пробелом везде сделай
///@note completed

//TODO: errors
///@note completed

//TODO: sprintf() вместо write / File* вместо descr, ask ded
///@note completed

// TODO: fix naming
///@note compleeted

// TODO: VLA - variable length array -- полное говно (budet crash), сделать calloc
///@note completed

// TODO: Buffer -> buffer
///@note completed

//TODO change strlen to ptr + 1 - ptr
///@note completed

//TODO: func args in one struct - in progress
///@note completed

//TODO - clear project
///@note completed

///TODO - destroy program func(), add poison value
///@note completed

//TODO - argc/argv - in progress
///@note completed

//TODO - README - in progress
///@note completed


int main(int argc, char *argv[]){
    
    Info env = {}; 

    DEBUG(printf("debug0\n");)
    
    env.fileName = CheckConsoleArguments(argc, argv, &env);
    
    env.error = OpenInFile(&env);
    if (env.error) {
        PrintfError(env.error);
        return RETURN_ERROR;
    }

    DEBUG(printf("debug1\n");)

    env.error = ReadInFile(&env);
    if (env.error) {
        PrintfError(env.error);
        return RETURN_ERROR;
    }

    DEBUG(int fileOutUnsortedDescriptor = open("onegin_out_unsorted.txt", O_CREAT | O_RDWR | O_TRUNC, 0666);
          write(fileOutUnsortedDescriptor, env.buffer, env.bufSize);)

    ChangeEndlsToNulls(&env);

    DEBUG(printf("debug2\n");)

    
    DEBUG(int fileOutInOneLineDescriptor = open("onegin_out_in_one_line.txt", O_CREAT | O_RDWR | O_TRUNC, 0666);
          write(fileOutInOneLineDescriptor, env.buffer, env.bufSize);)

    BufferToLinesFragmentation(&env);

    DEBUG(printf("debug3\n");)
    
    if (env.mode == MODE_SORT_BEGINNING || env.mode == MODE_SORT_ALL || env.mode == MODE_SORT_ONLY){
        env.error = SortingByBeginnings(&env);
        if (env.error) {
            PrintfError(env.error);
            return RETURN_ERROR;
        }
    }

    DEBUG(printf("debug4\n");)

    if (env.mode == MODE_SORT_ENDING || env.mode == MODE_SORT_ALL || env.mode == MODE_SORT_ONLY){
        env.error = SortingByEndings(&env);
        if (env.error) {
            PrintfError(env.error);
            return RETURN_ERROR;
        }
    }
    
    DEBUG(printf("degug5\n");)

    if (env.mode == MODE_SORT_UNSORTED || env.mode == MODE_SORT_ALL){
        env.error = OutputUnsortedText(&env);
        if (env.error) {
            PrintfError(env.error);
            return RETURN_ERROR;
        }
    }
    ProgramDestroy(&env DEBUG(, fileOutInOneLineDescriptor, fileOutUnsortedDescriptor));
}

const char *CheckConsoleArguments(int argc, char *argv[], Info *programInfo){
    if(argc == 1){
        programInfo->mode = MODE_SORT_BEGINNING;
        return fileInNameDefault;
    }
    else if(argc == 2){
        if(IsMode(argv[1], programInfo)){
            return fileInNameDefault;
        }
        else{
            return argv[1];
        }
    }
    else if(argc == 3){
        if((IsMode(argv[2], programInfo))){
            return argv[1];
        }
        else{
            PrintfError(RETURN_ERROR_CONSOLE_INPUT);
            return NULL;
        }
    }
    else{
        PrintfError(RETURN_ERROR_CONSOLE_INPUT);
        return NULL;
    }
}

bool IsMode(char * consoleArg, Info *programInfo){
    if(strcmp(consoleArg, "--unsorted") == 0 || strcmp(consoleArg, "-u") == 0){
        programInfo->mode = MODE_SORT_UNSORTED;
        return 1;
    }
    if(strcmp(consoleArg, "--beginning") == 0 || strcmp(consoleArg, "-b") == 0){
        programInfo->mode = MODE_SORT_BEGINNING;
        return 1;
    }
    if(strcmp(consoleArg, "--ending") == 0 || strcmp(consoleArg, "-e") == 0){
        programInfo->mode = MODE_SORT_ENDING;
        return 1;
    }
    if(strcmp(consoleArg, "--all") == 0 || strcmp(consoleArg, "-a") == 0){
        programInfo->mode = MODE_SORT_ALL;
        return 1;
    }
    if(strcmp(consoleArg, "--sortsonly") == 0 || strcmp(consoleArg, "-s") == 0){
        programInfo->mode = MODE_SORT_ONLY;
        return 1;
    }
    else{
        programInfo->mode = MODE_SORT_UNSORTED;
        return 0;
    }
}

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
        case (RETURN_ERROR_CONSOLE_INPUT):
            printf("Error in console arguments\n");   
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

ErrSuc SortingByBeginnings(Info *programInfo){
    assert(programInfo != NULL);
    assert(programInfo->arrLines != NULL);

    DEBUG(printf("OpenOutFile1\n");)

    QuickSort(programInfo->arrLines, sizeof(Line), 0, programInfo->arrSize - 1, CompareStrOneginBeginning);

    programInfo->fileOut /*_Descriptor*/ = fopen("output.txt", "a"/*O_CREAT | O_RDWR | O_TRUNC, 0666*/);

    if (programInfo->fileOut /*_Descriptor*/ == NULL){
        fclose(programInfo->fileOut /*_Descriptor*/);

        return RETURN_ERROR_FILECREATE;
    }

    fputs("===Sorting=By=Beginnig===\n\n", programInfo->fileOut);
    
    for (size_t index = 0; index < programInfo->arrSize; index++){
        if (fprintf(programInfo->fileOut, "%s\n", programInfo->arrLines[index].line) <= 0){
            fclose(programInfo->fileOut /*_Descriptor*/);
            
            return RETURN_ERROR_FILEWRITE_SORT_BEG;
        }
    }

    fputs("\n", programInfo->fileOut);

    return RETURN_SUCCESS;
}

ErrSuc SortingByEndings(Info *programInfo){
    assert(programInfo != NULL);
    assert(programInfo->arrLines != NULL);

    DEBUG(printf("OpenOutFile1\n");)

    qsort(programInfo->arrLines, programInfo->arrSize, sizeof(Line), CompareStrOneginEnding);

    if(programInfo->fileOut == NULL){
        programInfo->fileOut /*_Descriptor*/ = fopen("output.txt", "a"/*O_CREAT | O_RDWR | O_TRUNC, 0666*/);
    }

    if (programInfo->fileOut /*_Descriptor*/ == NULL){
        fclose(programInfo->fileOut /*_Descriptor*/);
        DEBUG(printf("hui\n");)
        return RETURN_ERROR_FILEADD_SORT_END;
    }

    fputs("===Sorting=By=Ends===\n\n", programInfo->fileOut);

    for (size_t index = 0; index < programInfo->arrSize; index++){
        if (fprintf(programInfo->fileOut, "%s\n", programInfo->arrLines[index].line) <= 0){
            fclose(programInfo->fileOut /*_Descriptor*/);
            
            return RETURN_ERROR_FILEWRITE_SORT_END;
        }
    }

    fputs("\n", programInfo->fileOut);

    //fclose(fileOut /*_Descriptor*/);
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