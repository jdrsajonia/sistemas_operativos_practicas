
// este codigo si fue hecho con IA en su totalidad, por favor revisar o implementar una versión propia para que sea mas guapetón joder!
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashing_functions.h" // asegúrate de incluir tu hash_xxh64
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

int main() {
    char id_input[20];
    printf("Ingrese el ID del libro: ");
    fgets(id_input, sizeof(id_input), stdin);
    id_input[strcspn(id_input, "\n")] = '\0'; // quitar salto de línea

    int id_hashed = hash_xxh64(id_input, TABLE_SIZE, 0);

    FILE *hash_file = fopen("hash_table.dat", "rb");
    FILE *data_file = fopen("database_indexed.dat", "rb");

    if (!hash_file || !data_file) {
        perror("Error al abrir archivos");
        return 1;
    }

    hash_table_struct bucket;
    fseek(hash_file, id_hashed * sizeof(hash_table_struct), SEEK_SET);
    fread(&bucket, sizeof(hash_table_struct), 1, hash_file);

    if (bucket.first_offset == -1) {
        printf("❌ No se encontró ningún registro con ese ID.\n");
        fclose(hash_file);
        fclose(data_file);
        return 0;
    }

    // recorrer lista enlazada de colisiones
    bookrecord record;
    long offset = bucket.first_offset;
    while (offset != -1) {
        fseek(data_file, offset, SEEK_SET);
        fread(&record, sizeof(bookrecord), 1, data_file);

        if (strcmp(record.id, id_input) == 0) {
            printf("\n✅ Registro encontrado:\n");
            printf("ID: %s\n", record.id);
            printf("Nombre: %s\n", record.name);
            printf("Autor: %s\n", record.authors);
            printf("Editorial: %s\n", record.publisher);
            printf("Año: %s\n", record.publish_year);
            printf("Descripción: %s\n", record.description);
            printf("Páginas: %s\n", record.pages_number);
            printf("Rating: %s\n", record.rating);
            fclose(hash_file);
            fclose(data_file);
            return 0;
        }

        offset = record.next_offset;
    }

    printf("❌ No se encontró ningún registro con ese ID.\n");

    fclose(hash_file);
    fclose(data_file);
    return 0;
}
