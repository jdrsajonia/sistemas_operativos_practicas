#include <sys/socket.h> 
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>


#define PORT 8080
#define LOCAL_ADDRESS "127.0.0.1"
#define REMOTE_ADDRESS "149.130.174.253"


int main(){

    struct sockaddr_in cliente;

    char value[15];
    char buffer[1025];
    int command;


    printf("%s","===============================\n\tBuscador Clinux\n===============================");
    
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd==-1){
        perror("Error al crear el socket");
        exit(-1);
    }
    // Inicializa el puerto
    cliente.sin_family=AF_INET;
    cliente.sin_port=htons(PORT);
    // Escoge la direccion a usar 
    while (1) {
        char optbuf[32];
        
        printf("%s","\nEscoga opción de conexión:\n1.Local (requiere inicialización de servidor local)\n2.Remoto\n");

        if (fgets(optbuf, sizeof(optbuf), stdin) == NULL) {
            printf("Entrada EOF, saliendo\n");
            exit(-1);
        }
        /* quitar salto de linea si existe */
        char *p = strchr(optbuf, '\n');
        if (p) *p = '\0';

        char *endptr;
        long val = strtol(optbuf, &endptr, 10);
        if (endptr == optbuf || *endptr != '\0') {
            printf("%s","===Inserte una opción valida===");
            continue;
        }
        command = (int)val;

        if (command == 1) {
            cliente.sin_addr.s_addr=inet_addr(LOCAL_ADDRESS);
            break;
        } else if (command == 2){
            cliente.sin_addr.s_addr=inet_addr(REMOTE_ADDRESS);
            break;
        } else {
            printf("%s","===Inserte una opción valida===");
            continue;
        }
    }
    
    memset(&(cliente.sin_zero), 0, 8);

    int r = connect(fd, (struct sockaddr *)&cliente, sizeof(struct sockaddr));
    if (r==-1){
        perror("Error al conectar con el servidor");
        close(fd);
        exit(-1);
    }


    while(1){
        printf("\n[*] Inserte ID del libro a buscar \n[ID] >> ");
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


