En este documento solo nos interesará ejecutar dos programas:

1) hash_database.c --> hasheo de la base de datos (se crea un archivo binario de la base de datos indexada) 
2) find_record.c   --> consulta por ID de un registro de la base de datos  


Para compilar estos archivos, basta con ejecutar el siguiente comando dentro de este mismo directorio:

> make

Esto generará dos archivos ejecutables. "hashing_table" y "find"

Para generar la base de datos hasheada, se tiene que ejecutar hashing_table de la siguiente forma:
> ./hashing_table

Este proceso de hashing puede durar aproximadamente medio minuto.

Posteriormente con la base de datos ya hasheada, podemos empezar a buscar sobre ella con el siguiente comando:

> ./find

Esto abrirá el programa que permite consultar registros de la base de datos a traves de un ID, devolviendo información asociada del campo.


