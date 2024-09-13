# Sistemas Operativos: Proyecto #1

## Integrantes
- Ariana Alvarado Molina.
- María Paula Bolaños Apú.
- Marco Herrera González.

## Descripción

El proyecto consiste en un programa para la compresión y descompresión de archivos de texto por medio del algoritmo de codificación
de Huffman. Tanto para la compresión como descompresión se tienen tres versiones que producen los mismos resultados: serial, con concurrencia (implementada con hilos) y con paralelismo (implementada con la función *fork*). El programa de compresión toma la ruta de un directorio con archivos de texto y crea un archivo .tar con todos los archivos de texto comprimidos. El programa de descompresión toma la ruta de un archivo .tar y reconstruye una carpeta con todos los archivos de texto descomprimidos. En ambos programas se puede indicar la versión a utilizar, y al final se muestra el tiempo en nanosegundos que tomó hacer la compresión o descompresión.

## Requerimientos

- Un entorno Linux Fedora Workstation 39.

## Instalación

Para utilizar los programas de compresión y descompresión debe descargar un archivo zip con todos los archivos en este repositorio.
Luego, descomprima el archivo zip, ingrese a la carpeta descomprimida y abra una terminal de comandos desde esta carpeta.
Luego, debe instalar las dependencias de los programas, para lo cual debe ejecutar el archivo [dependencies.sh](dependencies.sh) por
medio del siguiente comando en la terminal:

```bash
sudo ./dependencies.sh
```

Finalmente, debe ejecutar el archivo [compile.sh](compile.sh) para compilar el proyecto y obtener los ejecutables, lo cual se hace con el siguiente comando en la terminal:

```bash
sudo ./compile.sh
```
## Uso

Una vez que se haya completado la instalación se deben producir dos archivos ejecutables llamados *compressor* y *decompressor*. Para utilizar los programas, abra una
terminal de comandos en la carpeta donde se encuentran los ejecutables. Para usar el programa de compresión use el siguiente comando en la terminal:

```bash
./compressor
```
Para usar el programa de descompresión use el siguiente comando en la terminal:

```bash
./decompressor
```

Al ejecutar cada uno de estos comandos va a recibir instrucciones detalladas de cómo utilizar los programas.


