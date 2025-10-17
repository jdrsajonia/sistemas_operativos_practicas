#include <stdio.h>
#include <stdlib.h>
#include "hashing_functions.h"

#define TABLE_SIZE 100000

#define MAX_STR 256   
#define MAX_DESC 1024 

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


typedef struct{
    long id;
    long first_offset;
} hash_table_struct;


int get_id_hashed(char *line, int index_id);
void init_hash_table(int size);
bookrecord map_csv_record_to_bin(const char *row);


int main(){

    //inicializar hash_table
    init_hash_table(TABLE_SIZE);

    FILE *csv_file=fopen("databases/books_unificado2.csv", "r");
    if (csv_file==NULL){
        perror("error al leer el archivo CSV");
    }

    FILE *hash_table_file=fopen("hash_table.dat", "r+b");
    if (hash_table_file==NULL){
        perror("error al leer el archivo hash_table.dat");
    }

    FILE *database_indexed_file=fopen("database_indexed.dat", "w+b");
    if (database_indexed_file==NULL){
        perror("error al leer el archivo database_indexed.dat");
    }

 

    char row[MAX_STR+MAX_DESC];
    fgets(row, sizeof(row), csv_file); // se salta la linea de los encabezados del csv

    int id_hashed=0;
    long current_offset=0;
    hash_table_struct temp_hash_bucket={.id=-1, .first_offset=-1};

    printf("\nHASHEANDO BASE DE DATOS EN database_indexed.dat\n(si esto tarda muy poco en ejecutarse, verificar la base de datos csv)\n");
    while (fgets(row, sizeof(row), csv_file)){
    
        row[strcspn(row, "\n")] = '\0';
        // printf("%s\n", row);
        id_hashed=get_id_hashed(row, 0);
        bookrecord record=map_csv_record_to_bin(row);

        // printf("record.next_offset antes de escribir: %ld\n", record.next_offset);


        fseek(database_indexed_file,0, SEEK_END); // poner puntero del archivo desde el final
        current_offset=ftell(database_indexed_file);
        fwrite(&record, sizeof(bookrecord), 1, database_indexed_file); 
        fflush(database_indexed_file);
        
        // buscar indice hash en hash_table.dat
        fseek(hash_table_file, id_hashed*sizeof(hash_table_struct), SEEK_SET);
        fread(&temp_hash_bucket,sizeof(hash_table_struct), 1, hash_table_file); // aqui es donde obtenemos la posicion del index hasheado y su first offset actual

        // validar si en hash_table first_offset es -1 o no
        if (temp_hash_bucket.first_offset==-1){
            // se actualiza el valor al offset = current_offset
            temp_hash_bucket.first_offset=current_offset;
            fseek(hash_table_file, -sizeof(hash_table_struct), SEEK_CUR);
            fwrite(&temp_hash_bucket, sizeof(hash_table_struct), 1, hash_table_file);
            fflush(hash_table_file);

            
        }
        else{
            
            bookrecord temp_record;
            long prev_offset=temp_hash_bucket.first_offset;
            
            while (1){
                
                fseek(database_indexed_file, prev_offset, SEEK_SET);
                fread(&temp_record, sizeof(bookrecord), 1, database_indexed_file);

                // printf("next_offset = %ld\n", temp_record.next_offset);
                
                if (temp_record.next_offset==-1){
                    temp_record.next_offset=current_offset;
                    fseek(database_indexed_file, prev_offset, SEEK_SET);
                    fwrite(&temp_record, sizeof(bookrecord), 1, database_indexed_file);
                    fflush(database_indexed_file);
                    break;
                }
                prev_offset=temp_record.next_offset;
            }
        }
    }

    fclose(csv_file);
    fclose(hash_table_file);
    fclose(database_indexed_file);

    printf("\nOK\n");

    return 0;
}



int get_id_hashed(char *row, int index_id){
    int column=0;
    int id_hashed=-1;

    // Crear copia del row para no modificar el original
    char *temp=strdup(row);  // reserva memoria y copia row AQUI TUVE QUE HACER COPIA DE LA FILA porque al hacer strtok mas adelante, la modifica globalmente
    if (temp==NULL) {
        perror("Error al copiar la cadena");
        exit(EXIT_FAILURE);
    }

    char *token=strtok(temp, ",");
    while (token!=NULL) {
        if (column==index_id) {
            id_hashed=hash_xxh64(token, TABLE_SIZE, 0);
            break;
        }
        column++;
        token=strtok(NULL, ",");
    }

    free(temp);  // liberar memoria
    return id_hashed;
}




void init_hash_table(int size){
    FILE *hash_table_file=fopen("hash_table.dat", "wb");
    if (hash_table_file==NULL){
        perror("Error al crear hash_table.dat");
    }

    hash_table_struct hash_bucket;

    for (int i=0; i<size; i++){
        hash_bucket.id=i;
        hash_bucket.first_offset=-1;
        // printf("\nHASH: %d       OFFSET: %d ",hash_bucket.id, hash_bucket.first_offset);
        fwrite(&hash_bucket, sizeof(hash_bucket), 1, hash_table_file);
    }

    fclose(hash_table_file);
}



bookrecord map_csv_record_to_bin(const char *row) {
    bookrecord record;
    memset(&record, 0, sizeof(record));
    record.next_offset=-1;

    // printf("token: %s\n", row);

    char temp[MAX_STR + MAX_DESC];
    strcpy(temp, row);

    char *token = strtok(temp, ",");
    int field = 0;

    while (token != NULL) {
        switch (field) {
            case 0:  strncpy(record.id, token, sizeof(record.id)); break;
            case 1:  strncpy(record.rating_dist_total, token, sizeof(record.rating_dist_total)); break;
            case 2:  strncpy(record.rating_dist5, token, sizeof(record.rating_dist5)); break;
            case 3:  strncpy(record.publish_day, token, sizeof(record.publish_day)); break;
            case 4:  strncpy(record.name, token, sizeof(record.name)); break;
            case 5:  strncpy(record.publish_month, token, sizeof(record.publish_month)); break;
            case 6:  strncpy(record.rating_dist4, token, sizeof(record.rating_dist4)); break;
            case 7:  strncpy(record.rating_dist1, token, sizeof(record.rating_dist1)); break;
            case 8:  strncpy(record.rating_dist2, token, sizeof(record.rating_dist2)); break;
            case 9:  strncpy(record.counts_of_review, token, sizeof(record.counts_of_review)); break;
            case 10: strncpy(record.authors, token, sizeof(record.authors)); break;
            case 11: strncpy(record.rating_dist3, token, sizeof(record.rating_dist3)); break;
            case 12: strncpy(record.publish_year, token, sizeof(record.publish_year)); break;
            case 13: strncpy(record.source_file, token, sizeof(record.source_file)); break;
            case 14: strncpy(record.publisher, token, sizeof(record.publisher)); break;
            case 15: strncpy(record.language, token, sizeof(record.language)); break;
            case 16: strncpy(record.isbn, token, sizeof(record.isbn)); break;
            case 17: strncpy(record.description, token, sizeof(record.description)); break;
            case 18: strncpy(record.rating, token, sizeof(record.rating)); break;
            case 19: strncpy(record.pages_number, token, sizeof(record.pages_number)); break;
            case 20: strncpy(record.count_of_text_reviews, token, sizeof(record.count_of_text_reviews)); break;
        }
        // printf("token: %s\n", token);
        field++;
        // printf("token: %s\n", row);
        token = strtok(NULL, ",");
    }
    // printf("\n\n");

    return record;
}


