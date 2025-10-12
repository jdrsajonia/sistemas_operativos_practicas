#include <stdio.h>
#include <stdlib.h>
#include "hashing_functions.h"

#define TABLE_SIZE 10000000

int main(){
    
    char row_line[1000];

    FILE *file=fopen("databases/dormir.csv", "r");

    if (file==NULL){
        perror("error al leer el archivo");
    }
    
    fgets(row_line, sizeof(row_line), file); // se salta la linea de los encabezados del csv

    while (fgets(row_line, sizeof(row_line), file)){
        // printf("%s", row_line);
        row_line[strcspn(row_line, "\n")] = '\0';

        char *csv_record=strtok(row_line, ",");

        if (csv_record!=NULL){
            long hashed_record=hash_xxh64(csv_record, TABLE_SIZE, 0);
            printf("\"%s\" ---HASHED---> %lu (XXH64) \n", csv_record, hashed_record);
        }
    }

    pclose(file);
    return 0;
}


