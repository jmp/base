#ifndef FILE_H
#define FILE_H

/*
 * Check if the given file exists.
 * Returns 1 if it exists, 0 otherwise.
 */
extern int file_exists(const char *path);

/*
 * Attempt to create the given directory.
 * Returns 1 if the creation was successful, 0 otherwise.
 */
extern int file_mkdir(const char *path);

/*
 * Returns the full path to the given path relative to the asset directory.
 * Obviously not thread-safe and the caller should make sure to make a copy
 * of the string if they want to keep it. Otherwise it'll be replaced later.
 */
extern const char *file_abspath(const char *path);

#endif
