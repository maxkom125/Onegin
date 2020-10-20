#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#define FOR_STR_CHR(s, val) for (; (s) != NULL && *((s) + 1) != '\0'; (s) = strchr(s, val))
#define POINTER_ON_FUNCTION(func) int (*func)(const char* x, const char* y)

void WriteToFile(FILE* file, char** lines, int colStr) {
    for (int i = 0; i < colStr; i++) {
        fprintf(file, "%s \n", lines[i]);
    }
    fprintf(file, "\n \n");
}

long getNumberOfSymbols(FILE* fil) //returns quantity of strings from FILE
{
    if (fil == NULL)
    {
        printf("/n ERROR Text file is not open in file %s, line: %d /n", __FILE__, __LINE__);
        exit(-1);
    }

    long currentIndex = ftell(fil);
    fseek(fil, 0, SEEK_END);

    long ans = ftell(fil);
    fseek(fil, currentIndex, SEEK_SET);

    return ans;
}

int GetNumberOfStrings(const char* fileInString){
    int colStr = 0;
    const char* s = fileInString - 1;
    //for (; (s) != NULL && *((s) + 1) != '\0'; (s) = strchr(s, '\n'))
    FOR_STR_CHR(s, '\n')
    {
        s++;
        if (*s != '\n')
            colStr++;
        //printf(s);
        //printf("\n !!!! %d\n", s - fileInString);
    }
    return colStr;
}

void SplitToLines(char* line, char** arrayOfLines){
    int currLine = 0;
    char* s = line - 1;

    FOR_STR_CHR(s, '\n') {
        if (currLine != 0)
            *(s) = '\0';

        s++;

        if (*(s) != '\n') {
            arrayOfLines[currLine] = s;
            currLine++;
        }
    }
}

int GetRightPointer(char** array, int rightPointer, int pivot, POINTER_ON_FUNCTION(CompareFunc)){
    while(rightPointer > pivot) {
        if ((*CompareFunc)(*(array + pivot), *(array + rightPointer))) //if x rather y
            return rightPointer;
        else
            rightPointer--;
    }
    return pivot;
}

void Swap(char** array, int pointer1, int pointer2){
    assert(array[pointer1]);
    if (array[pointer2] == NULL)
        printf("!!!!!!!!!!!!!!!!!!!!!!! %s %d %s %d", __PRETTY_FUNCTION__, pointer2, array[pointer1], pointer1);
    assert(array[pointer2]);
    char* x = array[pointer1];
    array[pointer1] = array[pointer2];
    array[pointer2] = x;
}

void ReplacePivot(int* pivot, char** array, int leftPointer, int rightPointer){

    int newPivot = (leftPointer + rightPointer + 1) / 2;
    if (newPivot != *pivot) {
        Swap(array, *pivot, newPivot);
        *pivot = newPivot;
    }
}

void Quicksort(char** array, int colStr, POINTER_ON_FUNCTION(CompareFunc)) {
    int leftPointer = 0;
    int rightPointer = colStr - 1;
    int pivot = colStr / 2;

    while (1) {
        if (*(array + pivot) == NULL)
            printf("!!!!!!!!!!!!! %d", pivot);
        if ((*CompareFunc)(*(array + leftPointer), *(array + pivot))) { //if x rather y
            //fix leftPointer
            while (1) {
                rightPointer = GetRightPointer(array, rightPointer, pivot, *CompareFunc);
                if (rightPointer == pivot) {
                    ReplacePivot(&pivot, array, leftPointer, rightPointer);
                    if (pivot == rightPointer) {//leftPointer + 1 == rightPointer
                        Swap(array, leftPointer, pivot);
                        pivot = leftPointer;
                        rightPointer = pivot;
                        leftPointer--; //next operation ++
                        break;
                    }
                }
                else {
                    Swap(array, leftPointer, rightPointer);
                    break;
                }
            }
        }
        leftPointer++;
        if (leftPointer == pivot) {
            ReplacePivot(&pivot, array, leftPointer, rightPointer);
            if (pivot == leftPointer) {
                if (pivot > 1)
                    Quicksort(array, pivot, *CompareFunc);

                if (colStr - pivot - 1 > 1)
                    Quicksort(array + pivot + 1, colStr - pivot - 1, *CompareFunc);
                return;
            }
        }
    }
}

int CompareStrBegins(const char* str1, const char* str2){
    assert(str1);
    if (str2 == NULL)
        printf("!!!!!!!!!!!!!!!!!!!!!!!");
    assert(str2);
    if (strcmp(str1, str2) <= 0)
        return 0;
    else
        return 1;
}
int CompareStrEnds(const char* str1, const char* str2){
    assert(str1);
    assert(str2);
    char s1[strlen(str1)];
    char s2[strlen(str2)];

    strcpy(s1, str1);
    strcpy(s2, str2);

    strrev(s1);
    strrev(s2);

    if (strcmp(s1, s2) <= 0)
        return 0;
    else
        return 1;
}

int main() {
    FILE *poem;
    poem = fopen("EvgeniyOnegin.txt", "r");
    //poem = fopen("EvgOneg.txt", "r");

    assert(poem != NULL);

    long numberOfSymbols = getNumberOfSymbols(poem);
    printf("\n NUMSYM %ld \n", numberOfSymbols);

    char* fileInString = (char*)calloc(sizeof(char)*(numberOfSymbols + 1), sizeof(char)); //REALLOC
    fread(fileInString, sizeof(char), numberOfSymbols, poem);
    fclose(poem);

    *(fileInString + numberOfSymbols) = '\0';

    int colStr = GetNumberOfStrings(fileInString);

    char* poemLines[colStr];
    char* poemLines1[colStr];
    char* poemLines2[colStr];

    SplitToLines(fileInString, poemLines);

    for (int i = 0; i < colStr; i++) {
        poemLines1[i] = poemLines[i];
        poemLines2[i] = poemLines[i];
    }

    printf("\n colStr: %d; poemLines: \n", colStr);
    for (int i = 0; i < colStr; i++) {
        printf("%s \n", poemLines[i]);
        if (*poemLines[i] == NULL)
            printf("!!! \n !!! \n !!! \n !!! %d", i);
    }

    Quicksort(poemLines1, colStr, CompareStrBegins);
    Quicksort(poemLines2, colStr, CompareStrEnds);

    printf("\n poemLines CompareBegins: \n");
    for (int i = 0; i < colStr; i++)
        printf("%s \n", poemLines1[i]);

    printf("\n poemLines CompareEnds: \n");
    for (int i = 0; i < colStr; i++)
        printf("%s \n", poemLines2[i]);

    poem = fopen("EvgOnegSorted.txt", "w");
    WriteToFile(poem, poemLines1, colStr);
    //WriteToFile(poem, poemLines2, colStr);
    WriteToFile(poem, poemLines, colStr);

    fclose(poem);

    free(fileInString);
    return 0;
}

