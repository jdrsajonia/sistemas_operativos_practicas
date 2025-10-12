sudo apt install libssl-dev
sudo apt install libxxhash-dev

gcc hashing_functions.c hash_database.c -o hash_database -lssl -lcrypto -lxxhash 

echo "Si todo salió bien, ejecute el archivo compilado generado "hash_database" con el comando --> ./hash_database"