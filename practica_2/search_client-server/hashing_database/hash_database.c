#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include "hashing_functions.h"
#include <time.h>

#define TABLE_SIZE 100000

#define MAX_STR 256   
#define MAX_DESC 1024 

/**
 * @brief Estructura para almacenar un registro de un libro
 * 
 * Esta estructura representa un registro de libro en la base de datos,
 * incluyendo campos como ID, nombre, autor, editorial, año, descripción,
 * número de páginas, rating y un puntero al siguiente registro en caso de
 * colisiones en la tabla hash.
 *  
  */ 
typedef struct {
    char id[10];
    char rating_dist_total[20];
    char rating_dist5[20];
    char publish_day[20];
    char name[100];
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

// Estructura para almacenar un bucket de la tabla hash
typedef struct{
    long id;
    long first_offset;
} hash_table_struct;


/**
 * @brief Analiza el siguiente campo de una fila CSV, manejando correctamente los campos entre comillas
 * 
 * @param input Puntero a la posición actual en la fila CSV
 * @param field Buffer para almacenar el campo analizado
 * @param field_size Tamaño del buffer del campo
 * @return char* Puntero al inicio del siguiente campo, o NULL si está al final de la fila
 */
char* get_next_csv_field(char* input, char* field, size_t field_size) {
    if (!input || !*input) return NULL;  // End of input
    
    // Skip leading whitespace
    while (*input == ' ') input++;
    
    field[0] = '\0';
    size_t pos = 0;
    
    // Handle quoted fields
    if (*input == '"') {
        input++;  // Skip the opening quote
        
        while (*input && pos < field_size - 1) {
            if (*input == '"') {
                if (*(input + 1) == '"') {  // Double quote = escaped quote
                    if (pos < field_size - 1) {
                        field[pos++] = '"';
                    }
                    input += 2;
                } else {  // End of quoted field
                    input++;
                    break;
                }
            } else {
                field[pos++] = *input++;
            }
        }
        
        // Skip to the next delimiter
        while (*input && *input != ',') input++;
        
    } else {
        // Unquoted field - read until comma or end
        while (*input && *input != ',' && pos < field_size - 1) {
            field[pos++] = *input++;
        }
    }
    
    field[pos] = '\0';  // Null terminate the field
    
    // Move past the delimiter if present
    return *input ? input + 1 : NULL;
}

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

    printf("[HASHING DATABASE] ...\n");
    struct timespec tstart, tend;
    clock_gettime(CLOCK_MONOTONIC, &tstart);
    fflush(stdout);

    while (fgets(row, sizeof(row), csv_file)){
    
        row[strcspn(row, "\n")] = '\0';
        // printf("%s\n", row);
        id_hashed=get_id_hashed(row, 0);
        bookrecord record=map_csv_record_to_bin(row);
        

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
    clock_gettime(CLOCK_MONOTONIC, &tend);

    long elapsed_s = (tend.tv_sec - tstart.tv_sec);
    double elapsed_ms = elapsed_s * 1e3 + (tend.tv_nsec - tstart.tv_nsec) / 1e6;
    printf("\nHECHO\n[TIMING] Hashing de la base de datos completo en %ld s (%.0f ms)\n", elapsed_s, elapsed_ms);
    return 0;
}



/**
 *  Extrae y hashea un ID desde una cadena de fila
 * @row: puntero a una cadena terminada en '\0' que contiene una fila de la base de datos
 * @index_id: desplazamiento en bytes o índice de carácter dentro de @row donde comienza el campo ID
 *
 * Analiza el campo ID que empieza en @index_id dentro de @row, convierte ese
 * campo a un entero y devuelve un valor hasheado derivado del entero parseado,
 * adecuado para usarse como índice en la base de datos hash.
 *
 * @return Un indice hasheado no negativo en caso de éxito.
 *   Un valor negativo en caso de error (por ejemplo: @row es NULL, @index_id
 *   está fuera de los límites de la cadena, o la subcadena en @index_id no puede
 *   convertirse a un entero válido).
 *
 * Notas:
 *   - La función no modifica el contenido de @row.
 *   - El algoritmo de hash exacto y el rango de valores devueltos son internos a
 *     esta función; los llamadores deben tratar el retorno como un índice hasheado
 *     y no como el ID original.
 *   - El llamador debe asegurar que @row es una cadena válida terminada en '\0'
 *     y que @index_id apunta al inicio del texto del ID dentro de esa cadena.
 */
int get_id_hashed(char *row, int index_id){
    int column = 0;
    int id_hashed = -1;
    char field[MAX_STR];
    
    // Create a copy of the row to avoid modifying the original
    char *temp = strdup(row);
    if (temp == NULL) {
        perror("Error al copiar la cadena");
        exit(EXIT_FAILURE);
    }
    
    char *current_pos = temp;
    while (current_pos != NULL) {
        current_pos = get_next_csv_field(current_pos, field, sizeof(field));
        if (column == index_id) {
            id_hashed = hash_xxh64(field, TABLE_SIZE, 0);
            break;
        }
        column++;
    }
    
    free(temp);
    return id_hashed;
}

/**
 * @brief Inicializa la tabla hash en un archivo
 *
 * Esta función crea un archivo llamado "hash_table.dat" y lo llena con
 * entradas iniciales para una tabla hash de tamaño especificado. Cada
 * entrada se inicializa con un ID correspondiente a su índice y un
 * desplazamiento inicial de -1, indicando que no hay registros asociados.
 *
 * @param size El número de entradas (buckets) a crear en la tabla hash
 *
 * @note El archivo "hash_table.dat" se crea en el directorio de trabajo
 *       actual del programa. Si el archivo ya existe, se sobrescribirá.
 *
 * @warning Si no se puede crear o escribir en el archivo, la función
 *          imprimirá un mensaje de error y terminará el programa.
 */
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


/**
 * @brief Mapea un registro de CSV a un registro binario
 * 
 * @param csv_record Registro CSV de entrada que se va a mapear
 * @param bin_record Registro binario de salida donde se almacenarán los datos mapeados
 * @param csv_record_len Longitud del registro CSV de entrada
 * 
 * @details Esta función convierte un registro en formato CSV a su representación binaria.
 *          Procesa el registro CSV campo por campo, realizando las conversiones necesarias
 *          y almacena los datos en el formato binario especificado.
 * 
 * @return void
 * 
 */
bookrecord map_csv_record_to_bin(const char *row) {
    // Inicializar estructura de registro de libro
    bookrecord record;
    memset(&record, 0, sizeof(record));
    record.next_offset = -1;

    char temp[MAX_STR + MAX_DESC];
    strcpy(temp, row);
    
    char field_value[MAX_STR + MAX_DESC];
    char *current_pos = temp;
    int field_num = 0;
    
    while (current_pos != NULL && field_num <= 20) {
        current_pos = get_next_csv_field(current_pos, field_value, sizeof(field_value));
        
        switch (field_num) {
            case 0:  strncpy(record.id, field_value, sizeof(record.id) - 1); break;
            case 1:  strncpy(record.rating_dist_total, field_value, sizeof(record.rating_dist_total) - 1); break;
            case 2:  strncpy(record.rating_dist5, field_value, sizeof(record.rating_dist5) - 1); break;
            case 3:  strncpy(record.publish_day, field_value, sizeof(record.publish_day) - 1); break;
            case 4:  strncpy(record.name, field_value, sizeof(record.name) - 1); break;
            case 5:  strncpy(record.publish_month, field_value, sizeof(record.publish_month) - 1); break;
            case 6:  strncpy(record.rating_dist4, field_value, sizeof(record.rating_dist4) - 1); break;
            case 7:  strncpy(record.rating_dist1, field_value, sizeof(record.rating_dist1) - 1); break;
            case 8:  strncpy(record.rating_dist2, field_value, sizeof(record.rating_dist2) - 1); break;
            case 9:  strncpy(record.counts_of_review, field_value, sizeof(record.counts_of_review) - 1); break;
            case 10: strncpy(record.authors, field_value, sizeof(record.authors) - 1); break;
            case 11: strncpy(record.rating_dist3, field_value, sizeof(record.rating_dist3) - 1); break;
            case 12: strncpy(record.publish_year, field_value, sizeof(record.publish_year) - 1); break;
            case 13: strncpy(record.source_file, field_value, sizeof(record.source_file) - 1); break;
            case 14: strncpy(record.publisher, field_value, sizeof(record.publisher) - 1); break;
            case 15: strncpy(record.language, field_value, sizeof(record.language) - 1); break;
            case 16: strncpy(record.isbn, field_value, sizeof(record.isbn) - 1); break;
            case 17: strncpy(record.description, field_value, sizeof(record.description) - 1); break;
            case 18: strncpy(record.rating, field_value, sizeof(record.rating) - 1); break;
            case 19: strncpy(record.pages_number, field_value, sizeof(record.pages_number) - 1); break;
            case 20: strncpy(record.count_of_text_reviews, field_value, sizeof(record.count_of_text_reviews) - 1); break;
        }
        field_num++;
    }

    return record;
}
