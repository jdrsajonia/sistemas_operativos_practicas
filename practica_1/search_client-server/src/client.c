#include <sys/socket.h> 
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>


#define PORT 8080
#define ADDRESS "127.0.0.1"


int main(){

    struct sockaddr_in cliente;

    char value[15];
    char buffer[1025];



    printf("%s","===============================\n\tBuscador Clinux\n===============================");
    
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd==-1){
        perror("Error al crear el socket");
        exit(-1);
    }

    cliente.sin_family=AF_INET;
    cliente.sin_port=htons(PORT);
    cliente.sin_addr.s_addr=inet_addr(ADDRESS);
    memset(&(cliente.sin_zero), 0, 8);

    int r = connect(fd, (struct sockaddr *)&cliente, sizeof(struct sockaddr));
    if (r==-1){
        perror("Error al conectar con el servidor");
        close(fd);
        exit(-1);
    }


    while(1){
        printf("\n[*] Inserte ISBN del libro a buscar \n[ISBN] >> ");
        if (scanf("%127s", value)!=1){
            break;
        }

        send(fd, value, strlen(value), 0);

        int bytes=recv(fd, buffer, sizeof(buffer)-1, 0);
        if (bytes>0){
            buffer[bytes]='\0';
            printf("\n%s", buffer);
        }
        else{
            printf("[CLIENTE] Conexion cerrada por el servidor\n");
            break;
        }
    }
    

    close(fd);
    return 0;
}


