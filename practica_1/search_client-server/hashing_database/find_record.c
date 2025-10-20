#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashing_functions.h" 

#define TABLE_SIZE 100000

typedef struct {
    char id[10];
    char rating_dist_total[20];
    char rating_dist5[20];
    char publish_day[20];
    char name[50];
    char publish_month[20];
    char rating_dist4[20];
    char rating_dist1[20];
    char rating_dist2[20];
    char counts_of_review[20];
    char authors[50];
    char rating_dist3[20];
    char publish_year[20];
    char source_file[50];
    char publisher[50];
    char language[20];
    char isbn[10];
    char description[100];
    char rating[10];
    char pages_number[10];
    char count_of_text_reviews[10];
    long next_offset;
} bookrecord;

typedef struct {
    long id;
    long first_offset;
} hash_table_struct;


char *find_record(const char *isbn) {
    static char not_found[] = "No match found\n";

    int id_hashed = hash_xxh64(isbn, TABLE_SIZE, 0);

    FILE *hash_file = fopen("hashing_database/hash_table.dat", "rb");
    FILE *data_file = fopen("hashing_database/database_indexed.dat", "rb");


    if (!hash_file || !data_file) {
        perror("Error al abrir archivos");
        if (hash_file) fclose(hash_file);
        if (data_file) fclose(data_file);
        return not_found;
    }

    hash_table_struct bucket;
    fseek(hash_file, id_hashed * sizeof(hash_table_struct), SEEK_SET);
    fread(&bucket, sizeof(hash_table_struct), 1, hash_file);

    if (bucket.first_offset == -1) {
        fclose(hash_file);
        fclose(data_file);
        return not_found;
    }

    // recorrer lista enlazada de colisiones
    bookrecord record;
    long offset = bucket.first_offset;
    while (offset != -1) {
        fseek(data_file, offset, SEEK_SET);
        fread(&record, sizeof(bookrecord), 1, data_file);

        if (strcmp(record.id, isbn) == 0) {
            // asignamos memoria dinámica para retornar los datos
            char *result = malloc(1024);
            if (!result) {
                perror("Error al asignar memoria");
                fclose(hash_file);
                fclose(data_file);
                return not_found;
            }

            snprintf(result, 1024,
                "\n\nRegistro encontrado:\n\n"
                "ID: %s\n"
                "Nombre: %s\n"
                "Autor: %s\n"
                "Editorial: %s\n"
                "Año: %s\n"
                "Descripción: %s\n"
                "Páginas: %s\n"
                "Rating: %s\n",
                record.id,
                record.name,
                record.authors,
                record.publisher,
                record.publish_year,
                record.description,
                record.pages_number,
                record.rating
            );

            fclose(hash_file);
            fclose(data_file);
            return result;
        }

        offset = record.next_offset;
    }

    fclose(hash_file);
    fclose(data_file);
    return not_found;
}
