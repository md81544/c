#ifndef FGETS_DYNAMIC_H
#define FGETS_DYNAMIC_H

/*
 * Note! The this requires the following standard libraries.
 * To avoid complex dependencies these should be included by 
 * the including translation unit.
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <string.h>
 */

/* MCD 2014-01-30
 * Replicates fgets's behaviour, which means NULL is returned
 * (a) if EOF is encountered and also (b) on error.
 * Caller should check feof(fp) to determine which.
 *
 * Caller owns buf (which may be realloc'd multiple times)
 * so should delete it when finished. 
 *
 * Note you need to pass *pointers* to allow the function
 * to modify *buf and buf_len.
 */
char *fgets_dynamic(char **buf, size_t *buf_len, FILE *fp) 
{
    long pos_from = ftell(fp);
    if (!fgets(*buf, *buf_len, fp)) {
        return NULL;
    }
    long pos_to = ftell(fp);
    unsigned long chars_read = pos_to - pos_from;
    while (chars_read == *buf_len - 1 && (*buf)[*buf_len - 1] != '\n') {
        // if we get here then we filled the buffer
        // but didn't get to the end of the file line.
        // So rewind back to the start of the line:
        fseek(fp, pos_from, SEEK_SET);
#ifdef DEBUG
        printf("** Resizing buffer from %zu to %zu\n",
               *buf_len, *buf_len * 2);
#endif
        *buf_len = *buf_len * 2;
        char *new_buf = realloc(*buf, *buf_len);
        if (!new_buf) {
            free(*buf);
            return NULL;
        }
        *buf = new_buf;
        if (!fgets(*buf, *buf_len, fp)) {
            // Should never get here as we've determined we've only
            // read part of a line. This is to avoid compiler warnings.
            break;
        }
        pos_to = ftell(fp);
        chars_read = pos_to - pos_from;
    }
    return *buf;
}

#endif
