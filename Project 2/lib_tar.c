#include "lib_tar.h"

/*
CONSIGNES :
Implémente les fonctions reprises dans le squelette du code en te
basant sur la documentation trouvable sur https://www.gnu.org/software/tar/manual/html_node/Standard.html
*/

/**
 * Checks whether the archive is valid.
 *
 * Each non-null header of a valid archive has:
 *  - a magic value of "ustar" and a null,
 *  - a version value of "00" and no null,
 *  - a correct checksum
 *
 * @param tar_fd A file descriptor pointing to the start of a file supposed to contain a tar archive.
 *
 * @return a zero or positive value if the archive is valid, representing the number of non-null headers in the archive,
 *         -1 if the archive contains a header with an invalid magic value,
 *         -2 if the archive contains a header with an invalid version value,
 *         -3 if the archive contains a header with an invalid checksum value
 */
int check_archive(int tar_fd) {
    struct posix_header header;
    int num_headers = 0; //nombre d'headers au total que l'on va retourner

    while (true) {
        ssize_t bytes_read = read(tar_fd, &header, 512);

        if (bytes_read < 512) {
            if (bytes_read == 0) { //fin de l'archive tar
                break;
            }
            return -4; //archive corrompue/lecture incomplète
        }

        if (header.name[0] == '\0') {
            break; //fin du fichier tar
        }

        if (strncmp(header.magic, "ustar", 5) != 0) { //vérification valeur magique
            //printf("Invalid magic value: '%.*s' (expected 'ustar').\n", 6, header.magic);
            return -1;
        }

        if (strncmp(header.version, "00", 2) != 0) { //vérification de la version
            //printf("Invalid version value: '%.*s' (expected '00').\n", 2, header.version);
            return -2;
        }

        int checksum = calculate_checksum((char *)&header); //vérification du checksum
        //printf("Checksum OK: calculated=%d, stored=%ld\n", checksum, strtol(header.chksum, NULL, 8));
        if (checksum != strtol(header.chksum, NULL, 8)) {
            return -3;
        }

        //on ne prends pas en compte l'en-tête représentant la racine de l'archive
        if (strcmp(header.name, "./") != 0 && strcmp(header.name, ".") != 0) {
            num_headers++;
        }

        int file_size = TAR_INT(header.size);
        int blocks_to_skip = (file_size + 511) / 512; //nombre de blocs en octets à sauter

        lseek(tar_fd, blocks_to_skip * 512, SEEK_CUR); //on passe aux 512 octet suivants
    }

    return num_headers; //retourner le nombre d'headers dans l'archive
}
/*c'est (pour toutes les fonctions) :

    Si ils nous donnent un path qui ne finit pas par un "/", il faut pouvoir le comprendre soit comme un path qui ne finit en effet pas par un "/", soit par un path qui finit par un "/" en fonction de ce qui se trouve dans l'archive (donc "dir" peut être converti en "dir/" mais seulement si nécessaire).
    Si ils nous donnent un path qui finit par un "/", il faut ne le comprendre que comme un path qui finit en effet par un "/", et pas par un path qui ne finit pas par un "/" (donc on ne peut pas convertir "dir/" en "dir").

Et par ailleurs notre Makefile n'a pas l'air d'être exécuté du tout, ils utilisent le leur je pense.
 * @return The calculated checksum as an integer.
 */
int calculate_checksum(const char *header) {
    int checksum = 0;

    for (int i = 0; i < 512; i++) {
        if (i >= 148 && i < 156) {
            checksum += 32; //valeur ASCII d'un espace
        } else {
            checksum += ( char)header[i];
        }
    }
    return checksum;
}

/**
 * Checks whether an entry exists in the archive.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 *
 * @return zero if no entry at the given path exists in the archive,
 *         any other value otherwise.
 */
int exists(int tar_fd, char *path) {
    struct posix_header header;

    lseek(tar_fd, 0, SEEK_SET); //on revient au début de l'archive pour recommencer la recherche

    while (true) {
        ssize_t bytes_read = read(tar_fd, &header, 512);

        if (bytes_read < 512) {
            if (bytes_read == 0) { //fin de l'archive tar
                break;
            }
            return -4; //archive corrompue/lecture incomplète
        }

        if (strncmp(header.name, path, sizeof(header.name)) == 0) {
            return 1; //fichier existe
        }

        if (header.name[0] == '\0') {
            break;
        }

        int file_size = TAR_INT(header.size);
        int blocks_to_skip = (file_size + 511) / 512;
        lseek(tar_fd, blocks_to_skip * 512, SEEK_CUR);  //passer aux 512 octet suivants
    }

    return 0; //fichier n'existe pas
}

/**
 * Checks whether an entry exists in the archive and is a directory.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 *
 * @return zero if no entry at the given path exists in the archive or the entry is not a directory,
 *         any other value otherwise.
 */
int is_dir(int tar_fd, char *path) {
    struct posix_header header;

    lseek(tar_fd, 0, SEEK_SET); //on revient au début de l'archive pour recommencer la recherche

    while (true) {
        ssize_t bytes_read = read(tar_fd, &header, 512);

        if (bytes_read < 512) {
            if (bytes_read == 0) { //fin de l'archive tar
                break;
            }
            return 0;
        }

        if (header.name[0] == '\0') {
            break;
        }

        if (strncmp(header.name, path, 100) == 0) { //si chemin correspond
            if (header.typeflag == '5') {
                return 1;
            }
            return 0;
        }

        int file_size = TAR_INT(header.size);
        int blocks_to_skip = (file_size + 511) / 512;
        lseek(tar_fd, blocks_to_skip * 512, SEEK_CUR); //passer aux 512 octet suivants
    }

    return 0;
}

/**
 * Checks whether an entry exists in the archive and is a file.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 *
 * @return zero if no entry at the given path exists in the archive or the entry is not a file,
 *         any other value otherwise.
 */
int is_file(int tar_fd, char *path) {
    struct posix_header header;

    lseek(tar_fd, 0, SEEK_SET); //on revient au début de l'archive pour recommencer la recherche

    while (true) {
        ssize_t bytes_read = read(tar_fd, &header, 512);

        if (bytes_read < 512) {
            if (bytes_read == 0) { //fin de l'archive tar
                break;
            }
            return 0;
        }

        if (header.name[0] == '\0') {
            break;
        }

        if (strncmp(header.name, path, 100) == 0) {
            if (header.typeflag == '0' || header.typeflag == '\0') {
                return 1;
            }
            return 0;
        }

        int file_size = TAR_INT(header.size);
        int blocks_to_skip = (file_size + 511) / 512;
        lseek(tar_fd, blocks_to_skip * 512, SEEK_CUR); //passer aux 512 octet suivants
    }

    return 0;
}


/**
 * Checks whether an entry exists in the archive and is a symlink.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 * @return zero if no entry at the given path exists in the archive or the entry is not symlink,
 *         any other value otherwise.
 */
int is_symlink(int tar_fd, char *path) {
    struct posix_header header;

    lseek(tar_fd, 0, SEEK_SET); //on revient au début de l'archive pour recommencer la recherche

    while (true) {
        ssize_t bytes_read = read(tar_fd, &header, 512);

        if (bytes_read < 512) {
            if (bytes_read == 0) { //fin de l'archive tar
                break;
            }
            return 0;
        }

        if (header.name[0] == '\0') {
            break;
        }

        if (strncmp(header.name, path, 100) == 0) {
            if (header.typeflag == '2') { //si symlink
                return 1;
            }
            return 0;
        }

        int file_size = TAR_INT(header.size);
        int blocks_to_skip = (file_size + 511) / 512;
        lseek(tar_fd, blocks_to_skip * 512, SEEK_CUR); //passer aux 512 octet suivants
    }

    return 0;
}

int path_matches(const char *archive_path, const char *input_path) {
    // Verifier si il y a un '/' à la fin
    size_t leng = strlen(input_path);
    if (leng > 0 && input_path[leng - 1] == '/') {
        // Si '/' vérifier si les 2 sont identiques
        return strncmp(archive_path, input_path,leng) == 0;
    }

    // Si pas de '/' alors on verifie si c'est un fichier et qu'il est compatible
    if (strncmp(archive_path, input_path, leng) == 0) {
        return 1; // Les chemins correspondent exactement
    }

    // Ajout de '/' pour le passer en répertoire et vérifier s'il est compatible
    char tmp_path[leng+2]; 
    strncpy(tmp_path, input_path, leng); 
    tmp_path[leng] = '/';
    tmp_path[leng+1] = '\0';             

    return strncmp(archive_path, tmp_path, leng+1) == 0;
}


/**
 * Lists the entries at a given path in the archive.
 * list() does not recurse into the directories listed at the given path.
 *
 * Example:
 *  dir/          list(..., "dir/", ...) lists "dir/a", "dir/b", "dir/c/" and "dir/e/"
 *   ├── a
 *   ├── b
 *   ├── c/
 *   │   └── d
 *   └── e/
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive. If the entry is a symlink, it must be resolved to its linked-to entry.
 * @param entries An array of char arrays, each one is long enough to contain a tar entry path.
 * @param no_entries An in-out argument.
 *                   The caller set it to the number of entries in `entries`.
 *                   The callee set it to the number of entries listed.
 *
 * @return zero if no directory at the given path exists in the archive,
 *         any other value otherwise.
 */
int list(int tar_fd, char *path, char **entries, size_t *no_entries) {

    //si répertoire n'existe pas ou n'en est pas un, erreur
    if (!exists(tar_fd, path) || !is_dir(tar_fd, path)) {
        return 0;
    }

    struct posix_header header;
    //lseek(tar_fd, 0, SEEK_SET);
    size_t listed_count = 0;

    if (is_symlink(tar_fd, path)) { //si c'est un symlink

        while (read(tar_fd, &header, 512) == 512) {
            if (strncmp(header.name, path, 100) == 0) {
                strncpy(path, header.linkname, 100);
                path[99] = '\0'; //string termine bien par '\0' comme chaque str en C
                break;
            }
            size_t size = strtol(header.size, NULL, 8);
            size_t skip_blocks = (size + 511) / 512;
            lseek(tar_fd, skip_blocks * 512, SEEK_CUR);
        }
    }

    if (lseek(tar_fd, 0, SEEK_SET) < 0) {
        perror("lseek");
        return 0; //erreur de lecture
    }

    while (read(tar_fd, &header, sizeof(header)) == sizeof(header)) { //lis les en-têtes des fichiers de l'archive

        if (header.name[0] == '\0') {
            break;
        }

        if (path_matches(header.name, path)) {

            char *relative_path = header.name + strlen(path); //obtient le chemin relatif du fichier par rapport au répertoire ou chemin spécifié
            char *slash_pos = strchr(relative_path, '/');

            //si l’élément trouvé est un fichier
            if (slash_pos == NULL || slash_pos == relative_path + strlen(relative_path) - 1) {
                
                if (header.typeflag == '5' && strchr(relative_path, '/') == NULL) { //si c'est le répertoire lui-même, on ignore
                    strncat(relative_path, "/", sizeof(relative_path) - strlen(relative_path) - 1);
                    continue;
                }

                if (listed_count < *no_entries) { //vérifie si on peut encore ajouter un fichier à la liste
                    strncpy(entries[listed_count], header.name, 100); //copie avec strncpy le nom du fichier dans entries
                    entries[listed_count][99] = '\0'; //string termine bien par '\0' comme chaque str en C
                    listed_count++;
                }
            }
        }

        size_t size = strtol(header.size, NULL, 8);
        size_t skip_blocks = (size + 511) / 512;

        if (lseek(tar_fd, skip_blocks * 512, SEEK_CUR) < 0) { //si erreur de lors du saut avec lseek
            perror("lseek");
            return 0;
        }
    }

    *no_entries = listed_count;

    return 1;
}

/**
 * Reads a file at a given path in the archive.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive to read from.  If the entry is a symlink, it must be resolved to its linked-to entry.
 * @param offset An offset in the file from which to start reading from, zero indicates the start of the file.
 * @param dest A destination buffer to read the given file into.
 * @param len An in-out argument.
 *            The caller set it to the size of dest.
 *            The callee set it to the number of bytes written to dest.
 *
 * @return -1 if no entry at the given path exists in the archive or the entry is not a file,
 *         -2 if the offset is outside the file total length,
 *         zero if the file was read in its entirety into the destination buffer,
 *         a positive value if the file was partially read, representing the remaining bytes left to be read to reach
 *         the end of the file.
 *
 */
ssize_t read_file(int tar_fd, char *path, size_t offset, uint8_t *dest, size_t *len) {
    struct posix_header header;

    lseek(tar_fd, 0, SEEK_SET); //on revient au début de l'archive pour recommencer la recherche

    while (read(tar_fd, &header, sizeof(header)) == sizeof(header)) {

        if (header.name[0] == '\0') {
            break;
        }

        if (path_matches(header.name, path)) { //vérifie si l'entrée correspond au chemin donné en argument

            if (header.typeflag == '2') { // symlink
                char *link_target = header.linkname;
                return read_file(tar_fd, link_target, offset, dest, len);
            }

            if (header.typeflag == '2') { //gérer le cas de si c'est un symlink
                char *linked_path = header.linkname;
                return read_file(tar_fd, linked_path, offset, dest, len); //on suit le lien du symlink avec un appel récursif
            }

            if (header.typeflag != '0' && header.typeflag != '\0') { //si l'entrée n'est pas un fichier
                printf("%d\n", -1);
                return -1;
            }

            size_t file_size = strtol(header.size, NULL, 8); //taille du fichier en octets

            if (offset >= file_size) {
                printf("%d\n", -2);
                return -2; //offset dépasse la taille du fichier
            }

            size_t bytes_to_read = file_size - offset;
            if (bytes_to_read > *len) {
                bytes_to_read = *len;
            }

            if (lseek(tar_fd, offset, SEEK_CUR) < 0) { 
                perror("lseek");
                printf("%d\n", -1);
                return -1;
            }

            ssize_t read_bytes = read(tar_fd, dest, bytes_to_read); //lire les données du fichier dans le buffer
            if (read_bytes < 0) {
                perror("read");
                printf("%d\n", -1);
                return -1;
            }

            *len = read_bytes;

            return file_size - offset - read_bytes; //on retourne le nombre d'octets restants
        }

        //si pas le fichier recherché, on le saute
        size_t size = strtol(header.size, NULL, 8);
        size_t skip_blocks = (size + 511) / 512;
        if (lseek(tar_fd, skip_blocks * 512, SEEK_CUR) < 0) { //si erreur de lors du saut avec lseek
            perror("lseek");
            printf("%d\n", -1);
            return -1;
        }
    }
    printf("%d\n", -1);
    return -1;
}