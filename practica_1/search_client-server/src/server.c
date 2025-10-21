#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include "find_record.h"

#define SERVER_FIFO "/tmp/book_server_fifo"
#define CLIENT_FIFO_TEMPLATE "/tmp/book_client_%d_fifo"
#define MAX_BUFFER 1024




// Estructura para la respuesta del servidor
typedef struct {
    char response[MAX_BUFFER];
    double search_time_ms;
} ServerResponse;

int server_fifo_fd;
char *server_fifo = SERVER_FIFO;

void handle_sigint(int sig) {
    printf("\n[SERVIDOR] Cerrando pipes y saliendo...\n");
    close(server_fifo_fd);
    unlink(server_fifo);  // Remove the FIFO
    exit(0);
}

int main() {
    char buffer[MAX_BUFFER];
    ServerResponse resp;
    int client_pid, bytes_read;
    char client_fifo[256];
    
    // Set up signal handler
    signal(SIGINT, handle_sigint);
    
    // Create the server's FIFO if it doesn't exist
    if (mkfifo(server_fifo, 0666) == -1 && errno != EEXIST) {
        perror("[SERVIDOR] Error al crear el FIFO del servidor");
        exit(-1);
    }
    
    printf("[SERVIDOR] Iniciando servicio...\n");
    
    // Open the server's FIFO for reading
    server_fifo_fd = open(server_fifo, O_RDONLY);
    if (server_fifo_fd == -1) {
        perror("[SERVIDOR] Error al abrir el FIFO del servidor");
        unlink(server_fifo);
        exit(-1);
    }
    
    // Keep the FIFO open for writing to prevent EOF
    int dummy_fd = open(server_fifo, O_WRONLY);
    
    printf("[SERVIDOR] Esperando consultas...\n");
    
    while (1) {
        // Read request format: <pid>:<isbn>
        bytes_read = read(server_fifo_fd, buffer, MAX_BUFFER);
        if (bytes_read <= 0) continue;
        
        buffer[bytes_read] = '\0';
        
        // Parse client PID and ISBN
        char *pid_str = strtok(buffer, ":");
        char *isbn = strtok(NULL, ":");
        if (!pid_str || !isbn) continue;
        
        client_pid = atoi(pid_str);
        printf("[SERVIDOR] Consulta recibida del cliente %d: %s\n", client_pid, isbn);
        
        // Format client FIFO path
        snprintf(client_fifo, sizeof(client_fifo), CLIENT_FIFO_TEMPLATE, client_pid);
        
        // Open client FIFO for response
        int client_fd = open(client_fifo, O_WRONLY);
        if (client_fd == -1) {
            perror("[SERVIDOR] Error al abrir el FIFO del cliente");
            continue;
        }
        
        // Search for the record and measure time
        struct timespec tstart, tend;
        clock_gettime(CLOCK_MONOTONIC, &tstart);
        char *search_result = find_record(isbn);
        clock_gettime(CLOCK_MONOTONIC, &tend);
        
        // Calculate search time
        long elapsed_ns = (tend.tv_sec - tstart.tv_sec) * 1000000000L + (tend.tv_nsec - tstart.tv_nsec);
        double elapsed_ms = elapsed_ns / 1e6;
        
        // Prepare and send response
        strncpy(resp.response, search_result, MAX_BUFFER - 1);
        resp.response[MAX_BUFFER - 1] = '\0';
        resp.search_time_ms = elapsed_ms;
        
        write(client_fd, &resp, sizeof(resp));
        printf("[TIMING] Busqueda de '%s' tomo %.3f ms (%ld ns)\n", isbn, elapsed_ms, elapsed_ns);
        
        close(client_fd);
    }
    
    // Cleanup (though we should never reach here due to while(1))
    close(server_fifo_fd);
    close(dummy_fd);
    unlink(server_fifo);
    return 0;
}