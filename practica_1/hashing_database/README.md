Este directorio contiene varios codigos.
El que nos interesa ejecutar por el momento es hash_database.c, el cual usa las funciones de hashing_functions.c para hashear los registros del csv apuntado

Para compilar el proyecto, primero hay que instalar las librerias libssl-dev y libxxhash-dev y luego hacer uso del siguiente comando:
> gcc hashing_functions.c hash_database.c -o hash_database -lssl -lcrypto -lxxhash 

Si no queremos hacer esto, simplemente cambia los permisos de launch.sh para que sea un ejecutable con "chmod 777 launch.sh" y ejecutalo con
> bash launch.sh

ó

> ./launch.sh

Esto generará el archivo binario a ejecutar.

Después hago un readme mas bonito
