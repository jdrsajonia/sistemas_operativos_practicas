#define _POSIX_C_SOURCE 199309L
#include <sys/socket.h> //para los sockets
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#include "find_record.h"

#define PORT 3535
#define BACKLOG 10 //numero de conexiones permitidas en cola




int server_fd;

void handle_sigint(int sig){
    printf("\n[SERVIDOR] Cerrando socket y saliendo...");
    close(server_fd);
    exit(0);
}

int main(){

    struct sockaddr_in server, client;
    signal(SIGINT, handle_sigint); // ctrl + c
    char buffer[1025];


    // creacion del socket
    // ---------------------------------------------
    server_fd=socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd==-1){
        perror("Error al crear el socket");
        exit(-1);
    } 
    
    int opt=2;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server.sin_family=AF_INET;
    server.sin_port=htons(PORT); 
    server.sin_addr.s_addr=INADDR_ANY; 
    memset(&(server.sin_zero), 0, 8);
    // ---------------------------------------------


    //enlazar
    int r = bind(server_fd, (struct sockaddr *)&server, sizeof(struct sockaddr)); 
    if (r==-1){
        perror("Error al hacer el bind");
        close(server_fd);
        exit(-1);
    }

    //escuchar conexiones
    r=listen(server_fd,BACKLOG);
    if (r==-1){
        perror("Error al hacer Listen");
        close(server_fd);
        exit(-1);
    }

    printf("[SERVIDOR] Escuchando en el puerto %d . . . \n", PORT);


    while(1){
        socklen_t addrlen = sizeof(struct sockaddr_in);
        int client_fd = accept(server_fd, (struct sockaddr*)&client, &addrlen);
        if (client_fd==-1){
            perror("Error al aceptar la conexion");
            continue;
        }

        printf("[SERVIDOR] Cliente conectado\n");

        //While que corre indefinidamente para recibir mensajes del cliente
        while(1){
            int bytes=recv(client_fd, buffer, sizeof(buffer)-1, 0);
            if (bytes<=0){
                printf("[SERVIDOR] Cliente desconectado\n");
                close(client_fd);
                break;
            }

            buffer[bytes]='\0'; 
            printf("[CLIENTE] %s\n",buffer);


            //Aqui llamamos la funcion que consulta la base de datos, recuperar la info y enviarla en send

            // Cuenta el tiempo de busqueda
            struct timespec tstart, tend;
            clock_gettime(CLOCK_MONOTONIC, &tstart);
            char *server_response = find_record(buffer);
            clock_gettime(CLOCK_MONOTONIC, &tend);

            long elapsed_ns = (tend.tv_sec - tstart.tv_sec) * 1000000000L + (tend.tv_nsec - tstart.tv_nsec);
            double elapsed_ms = elapsed_ns / 1e6;
            printf("[TIMING] Busqueda de '%s' tomo %.3f ms (%ld ns)\n", buffer, elapsed_ms, elapsed_ns);

            send(client_fd, server_response, strlen(server_response), 0);
        }
    }

    close(server_fd);
    return 0;
}