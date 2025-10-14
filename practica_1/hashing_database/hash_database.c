#include <stdio.h>
#include <stdlib.h>
#include "hashing_functions.h"

#define TABLE_SIZE 10000000

#define MAX_STR 256   // tamaño máximo para campos de texto normales
#define MAX_DESC 1024 // tamaño máximo para la descripción

typedef struct {
    // --- Campos para el sistema de indexación ---
    unsigned long hashValue;  // hash calculado con xxh64() u otro método
    long nextOffset;          // offset al siguiente registro (o -1 si no hay colisión)

    // --- Campos del libro ---
    int Id;                     // Identificador único
    char Name[MAX_STR];         // Nombre del libro
    char Authors[MAX_STR];      // Autor(es)
    char Publisher[MAX_STR];    // Editorial
    char Language[MAX_STR];     // Idioma
    char ISBN[32];              // Código ISBN
    char source_file[MAX_STR];  // Archivo fuente (si proviene de varios CSV)
    char Description[MAX_DESC]; // Descripción o sinopsis

    int PublishDay;             // Día de publicación
    int PublishMonth;           // Mes de publicación
    int PublishYear;            // Año de publicación

    int pagesNumber;            // Número de páginas
    int PagesNumber;            // Campo repetido (para compatibilidad con dataset)

    float Rating;               // Calificación promedio
    long RatingDistTotal;       // Total de calificaciones
    long RatingDist1;           // Distribución: 1 estrella
    long RatingDist2;           // Distribución: 2 estrellas
    long RatingDist3;           // Distribución: 3 estrellas
    long RatingDist4;           // Distribución: 4 estrellas
    long RatingDist5;           // Distribución: 5 estrellas

    long CountsOfReview;        // Total de reseñas
    long CountOfTextReviews;    // Total de reseñas escritas

} BookRecord;

int get_id_hashed(char *line);

// 1. Inicializar hash_table.dat con todos los offsets en -1

// 2. Para cada registro en CSV:
//     a. Leer datos
//     b. hash = xxh64(ID) % TABLE_SIZE
//     c. offset = posición actual del puntero en database_indexed.dat
//     d. Escribir registro con next_offset = -1 en database_indexed.dat

//     e. Leer offset_guardado = hash_table[hash]
//     f. Si offset_guardado == -1:
//           hash_table[hash] = offset
//        Else:
//           temp_offset = offset_guardado
//           Mientras (registro en temp_offset).next_offset != -1:
//               temp_offset = (registro en temp_offset).next_offset
//           Actualizar (registro en temp_offset).next_offset = offset

int main(){
    
    // leer csv
    FILE *csv_file=fopen("databases/books_unificado.csv", "r");
    if (csv_file==NULL){
        perror("error al leer el archivo");
    }

    char row[MAX_STR+MAX_DESC];
    fgets(row, sizeof(row), csv_file); // se salta la linea de los encabezados del csv

    while (fgets(row, sizeof(row), csv_file)){
        row[strcspn(row, "\n")] = '\0';
        // printf("%s\n", row);
        printf("%d\n", get_id_hashed(row));
      

    }

    fclose(csv_file);
    return 0;
}


int get_id_hashed(char *line){
    int column=0;
    int id=-1;
    char *token=strtok(line, ",");
    
    while (token!=NULL){
        column++;
        if(column==10){
            id=atoi(token);
            break;
        }
        token=strtok(NULL, ",");
    }

    return id;
}

