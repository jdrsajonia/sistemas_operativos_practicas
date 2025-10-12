sudo apt install libssl-dev
sudo apt install libxxhash-dev

gcc hash_prueba.c -o hashing -lssl -lcrypto -lxxhash 
