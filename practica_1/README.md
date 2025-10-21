# :mag: Search Client-Server

Este proyecto implementa un sistema **cliente-servidor en C** para realizar consultas sobre una **base de datos de libros** previamente indexada mediante un algoritmo de *hashing*.

* Indíce de contenidos:
     - [Estructura del proyecto](#jigsaw-estructura-del-proyecto)
     - [Requisitos previos](#gear-requisitos-previos)
     - [Preparación del dataset](#inbox_tray-preparación-del-dataset)
     - [Compilación del proyecto](#hammer_and_wrench-compilación-del-proyecto)
     - [Hasheo de la base de datos](#abacus-hasheo-de-la-base-de-datos)
     - [Ejecución del sistema](#desktop_computer-ejecución-del-sistema-cliente-servidor)
     - [Notas importantes](#spiral_notepad-notas-importantes)
     - [Créditos](#-créditos)

---

## :jigsaw: Estructura del proyecto

```
search_client-server/
├── hashing_database/
│   ├── databases/
│   │   └── books_unificado2.csv   ⬅️ (debe agregarse manualmente)
│   ├── hash_database.c
│   ├── hashing_functions.c
│   ├── find_record.c
│   └── ...
├── include/
│   ├── find_record.h
│   └── hashing_functions.h
├── src/
│   ├── server.c
│   └── client.c
├── Makefile
└── README.md
```
[Volver al principio](#mag-search-client-server)

---

## :gear: Requisitos previos

Antes de compilar y ejecutar el proyecto, asegúrate de tener:

- Sistema operativo **Linux** o compatible con GCC  
- **gcc** y **make** instalados  
- Librerías: `libssl`, `libcrypto` y `libxxhash`  
  en Debian, puedes instalarlas con:  
  ```bash
  sudo apt install libssl-dev libxxhash-dev
  ```
  en Arch, puedes usar el comando:
  ```bash
  yay -S openssl xxhash

  ```

[Volver al principio](#mag-search-client-server)
  
---

## :inbox_tray: Preparación del dataset

⚠️ **IMPORTANTE:**  
Debido a limitaciones de GitHub, el archivo `books_unificado2.csv` (≈1 GB) **no se incluye** en el repositorio.
> Usando git LFS u otros metodos podremos descargar el archivo al clonar el repositorio.

Debes colocar manualmente el archivo original dentro de:
```
hashing_database/databases/books_unificado2.csv
```

Si el archivo no está presente, el sistema no podrá generar ni consultar la base de datos indexada.

[Volver al principio](#mag-search-client-server)

---

## :hammer_and_wrench: Compilación del proyecto

Desde la carpeta raíz del proyecto (`search_client-server`):

```bash
make
```

Esto compilará todos los módulos y generará los binarios necesarios (`server`, `client`, y `hashdb`).

[Volver al principio](#mag-search-client-server)

---

## :abacus: Hasheo de la base de datos

Para generar la base de datos indexada (solo se hace una vez o cuando cambie el CSV):

```bash
cd hashing_database
./hashdb
```

Esto procesará `books_unificado2.csv` y generará:
- `database_indexed.dat`
- `hash_table.dat`

En un portatíl con un procesador Ryzen 5 2500U se demora en promedio ≈1m 30s para finalizar el proceso de hashing.

---

[Volver al principio](#mag-search-client-server)

## :desktop_computer: Ejecución del sistema cliente-servidor

1. **Ejecuta el servidor** (mantendrá las conexiones activas):
   ```bash
   ./server
   ```

2. **En otra terminal**, ejecuta el cliente:
   ```bash
   ./client
   ```

3. Desde el cliente, podrás ingresar un ISBN de libro para consultar su información indexada.

[Volver al principio](#mag-search-client-server)

---

## :spiral_notepad: Notas importantes

- Algunos registros del archivo CSV pueden estar **ligeramente desorganizados** debido a **comas mal formateadas** en el dataset original.  
  Sin embargo, la **información relevante (título, autor, año, descripción, etc.) se mantiene íntegra y accesible**.

- Si realizas modificaciones en los archivos fuente, puedes recompilar con:
  ```bash
  make rebuild
  ```

[Volver al principio](#mag-search-client-server)

---

## 📚 Créditos

Proyecto desarrollado en C como implementación práctica de:
- Algoritmos de **hashing**
- **Sistemas cliente-servidor TCP**
- Manejo de **archivos binarios** e indexación

Por:
- Juan Diego Rozo Álvarez
  jurozoa@unal.edu.co
- Daniel Alonso Gracia Pina
  dagraciap@unal.edu.co
- Felipe Rojas Marin 
  ferojasm@unal.edu.co

[Volver al principio](#mag-search-client-server)
