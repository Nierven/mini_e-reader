# Carte SD

La carte SD est utilisée dans le projet pour stocker les livres et les icônes de l'application. Sa configuration se fait grâce au configurateur de STM32CubeIDE. Il faut d'abord s'assurer que les SPI ne sont pas tous utilisés puis configurer le SDMMC qui va donc utiliser l'un des SPI. À ce point, on communique correctement avec la carte SD mais on ne connaît pas encore sa structure interne, ce que l'on va faire maintenant.

## Configuration de la carte

En effet, la structure utilisée est un système de fichier type FAT32 ou NTFS par exemple. La carte STM que l'on possède ne supporte que le FAT par défaut. Il faut donc bien faire attention au système de la carte SD et la reformater si besoin est sur Windows ou sur la STM si une erreur à l'ouverture apparaît.

L'étape suivante consiste à monter la carte et donc interpréter le système de fichier présent dessus (`SDPath` est le nom du lecteur, soit `0:`) :

```cpp
f_mount(&SDFatFS, (TCHAR const*)SDPath, 0);
```

La carte restera montée pendant toute l'exécution de l'application pour réduire notamment les temps d'écriture (voir la [procédure de téléchargement](/guten/gutenberg) depuis [Gutenberg](https://www.gutenberg.org)).

## Lecture/Écriture

Deux fonctions sont définies pour lire et écrire rapidement sur la carte tout en prenant en compte plusieurs offsets et taille de buffer.

Lecture de `length` octets de `0:filename` à partir de `offset` dans `buffer` :

```cpp
void readFile(char *filename, uint8_t *buffer, uint32_t offset, uint32_t length, UINT *bytesRead)
{
	f_open(&SDFile, filename, FA_READ);
	f_lseek(&SDFile, offset);
	f_read(&SDFile, buffer, length, bytesRead);
	f_close(&SDFile);
}
```

Écriture de `length` octets sur `0:filename` à partir de `offset` dans `buffer` :

```cpp
void writeFile(char *filename, uint8_t *buffer, uint32_t offset, uint32_t length, UINT *bytesWritten)
{
	f_open(&SDFile, filename, FA_WRITE | FA_CREATE_ALWAYS);
	f_write(&SDFile, buffer + offset, length, bytesWritten);
	f_close(&SDFile);
}
```

## Gestion des fichiers

Puisque l'on va télécharger des fichiers, il faut pouvoir créer des fichiers, ce qui est réalisé dans la fonction `writeFile` grâce à l'argument `FA_CREATE_ALWAYS` qui créé un fichier même si un fichier du même nom existe déjà, et va donc l'écraser.

Pour la gestion de la bibliothèque, on veut aussi pouvoir supprimer des livres pour gagner de la place sur la carte. C'est réalisable grâce à la fonction ci-dessous qui supprime le fichier du système sans wipe tous les octets écrits :

```cpp
void deleteFile(char *filename)
{
	f_unlink(filename);
}
```