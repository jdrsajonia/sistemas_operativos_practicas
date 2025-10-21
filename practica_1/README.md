# :mag: Search Client-Server

Este proyecto implementa un sistema **cliente-servidor en C** para realizar consultas sobre una **base de datos de libros** previamente indexada mediante un algoritmo de *hashing*.

* Indíce de contenidos:
     - [Estructura del proyecto](#jigsaw-estructura-del-proyecto)
     - [Requisitos previos](#gear-requisitos-previos)
     - [Preparación del dataset](#inbox_tray-preparación-del-dataset)
     - [Instalación y Compilación del proyecto](#hammer_and_wrench-instalación-y-compilación-del-proyecto)
     - [Hasheo de la base de datos](#abacus-hasheo-de-la-base-de-datos)
     - [Ejecución del sistema](#desktop_computer-ejecución-del-sistema-cliente-servidor)
     - [Ejemplos de uso](#bulb-ejemplos-de-uso)
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

La base de datos esta estructurada con los siguientes campos: `Id`, `RatingDistTotal`, `RatingDist5`, `PublishDay`, `Name`, `PublishMonth`, `RatingDist4`, `RatingDist1`, `RatingDist2`, `CountsOfReview`, `Authors`, `RatingDist3`, `PublishYear`, `source_file`, `Publisher`, `Language`, `ISBN`, `Description`, `Rating`, `pagesNumber`, `Count of text reviews`, `PagesNumber`

Dentro de estos campos, nos interesan especialmente los siguientes campos


| Columna                 | Descripción                                     | Tipo de dato |
|-------------------------|-------------------------------------------------|--------------|
| Id                      | identificador unico dentro de la base de datos  | int          |
| PublishDay              | dia del mes que se publico                      | int          |
| Name                    | nombre del libro                                | str          |
| PublishMonth            | mes en el que se publico                        | int          |
| Authors                 | autores                                         | str          |
| PublishYear             | año de publicación                              | int          |
| Publisher               | editorial que publico el libro                  | str          |
| Language                | lenguaje                                        | str          |
| ISBN                    | codigo ISBN                                     | int          |
| Rating                  | puntuación                                      | float        |
| pagesNumber             | numero de paginas del libro                     | float        |

Tuvimos que adaptar el parser de csv debido a que ciertos campos como `Name`, `Authors` y `Publisher` tienen comas como parte de sus campos, en esos casos los campos son encapsulados por "", una vez hecha esta adaptación tendremos un dataset
que es entendible y consultable

[Volver al principio](#mag-search-client-server)

---

## :hammer_and_wrench: Instalación y Compilación del proyecto

Clona el repositorio a la carpeta deseada dentro de tu computador, recuerda añadir la base de datos en caso de no haber sido descargada por git clone

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

[Volver al principio](#mag-search-client-server)

---

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
## :bulb: Ejemplos de uso

Una vez ejecutemos ambos procesos podremos entrar desde client y entrar un id para búsqueda

```bash
===============================
        Buscador Clinux
===============================
[*] Inserte ISBN del libro a buscar 
[ISBN] >> 200000
```
Y obtendremos la siguiente salida con los campos mas relevantes del registro

```bash
Registro encontrado:

ID: 200000
Nombre: Meet My Staff
Autor: Patricia Marx
Editorial: HarperCollins
Año: 1998
Páginas: 40.0
Rating: 3.52
```

[Volver al principio](#mag-search-client-server)

---

## :spiral_notepad: Notas importantes

- Algunos registros del archivo CSV pueden estar **ligeramente desorganizados** debido a **comas mal formateadas** en el dataset original. Tambien podremos encontrar campos vacios o cortados a la mitad, esto se debe a temas de optimizacion de memoria y espacio.
  Sin embargo, la **información relevante (título, autor, año, etc.) se mantiene íntegra y accesible**.

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
