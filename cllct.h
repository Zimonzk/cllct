ifndef __CLLCT_H_INCLUDED__
#define __CLLCT_H_INCLUDED__
#define _FILE_OFFSET_BITS 64
#define CLLCT_VERSION "0.1"
#include <features.h>
#include <stdio.h>
#include <string.h>

#include "zimonzk/logger.h"


#define CLLCT_ERROR_INVALARG -1 /*at least one argument is nor valid*/
#define CLLCT_ERROR_CANTOPEN -2
#define CLLCT_ERROR_CANTWRITE -3
#define CLLCT_ERROR_CANTREAD -4
#define CLLCT_ERROR_FILE	-5 /* some other error on a file */
#define CLLCT_ERROR_INVAL_FILE -6
#define CLLCT_ERROR_MEM -7

typedef struct cllct_ctx {
	FILE* tablefile;
	off_t last_fileend /* i keep this here so i have to seek less */
	size_t numunits; /* Each unit is a nibble.
										* (representable by a single hex digit)
										* Might add dynamic unit sizes in the future.
										* Or maybe you could do that for me.*/
} cllct_ctx_t;

/* all numbers (addresses, numbers in the header
 * , ...) are binary little endian */

int init_cllct_ctx(char* filepath, unsigned short int unit_size, size_t number_total_units);

/* when spliting the data firs the msb or unit which contains the msb is used
 * and the one containing the lsb is last. */

int insert_chararray(cllct_ctx* ctx, unsigned char* chars);
int probe_chararray(cllct_ctx* ctx, unsigned char* chars);
int remove_chararray(cllct_ctx* ctx, unsigned char* chars);
int insert_hexchararray(cllct_ctx* ctx, unsigned char* digits);
int probe_hexchararray(cllct_ctx* ctx, unsigned char* digits);
int remove_hexchararray(cllct_ctx* ctx, unsigned char* digits);

#endif
