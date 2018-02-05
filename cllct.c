include "cllct.h"

int init_cllct_ctx(cllctx_t* ctx, char* filepath, size_t number_total_units)
{
	ctx->tablefile = fopen(filepath, "ab+"); /* create file if it doesn't exist */
	uint64_t file_n_total_units = (uint64_t) number_total_units;
	if(ctx->tablefile == 0) {
		return CLLCT_ERROR_CANTOPEN;
	} else {
		fclose(ctx->tablefile);
		ctx->tablefile = fopen(filepath, "rb+");
		if(ctx->tablefile == 0) {
			return CLLCT_ERROR_CANTOPEN;
		}
	}
	fseeko(ctx->tablefile, 0, SEEK_END);
	if(ftello(ctx->tablefile) == 0) { /* this is an empty file */
		/* the header is exactly 128 Bytes big and the first linking entry also */
		char* zeros = (char*) calloc(256, 1);
		if(zeros == 0) {
			return CLLCT_ERROR_MEM;
		}
		if(fwrite(zeros, 256, 1, ctx->tablefile) != 1) {
			free(zeros);
			return CLLCT_ERROR_CANTWRITE;
		}
		free(zeros);
		fseeko(ctx->tablefile, 0, SEEK_SET); /* write stuff in the header now */
		/* The header for now only tells the length of the data (in nibbles)
		 * and the version of the file format. */
		if(fwrite(CLLCT_VERSION, strlen(CLLCT_VERSION)+1, 1, ctx->tablefile) != 1) {
			return CLLCT_ERROR_CANTWRITE;
		}	
		if(fwrite(&file_n_total_units, sizeof(uint64_t), 1, ctx->tablefile) != 1) {
			return CLLCT_ERROR_CANTWRITE;
		}
		fseeko(ctx->tablefile, 0, SEEK_END); /* header done */
		if(ftello(ctx->tablefile) != 256) {
			return CLLCT_ERROR_FILE;
		} else {
			ctx->last_fileend = 256;
		}
 		/* the file should now have ist header complete */
	} else {
		/* read the header to ensure the file is useable */
		char version[] = CLLCT_VERSION;
		uint64_t units;
		fseeko(ctx->tablefile, 0, SEEK_SET);
		for(int i = 0; i < 120; i++) {
			if(fgetc(ctx->tablefile) != version[i]) {
				return CLLCT_ERROR_INVAL_FILE;
			}
			if(version[i] == '\0') {
			 break;
			}
		}
		if(fread(&units, sizeof(uint64_t), 1, ctx->tablefile) != 1) {
			return CLLCT_ERROR_CANTREAD;
		}
		if(units != file_n_total_units) {
			return CLLCT_ERROR_INVAL_FILE;
		}
		ctx->numunits = number_total_units;
	}

	return 0;
}

off_t get_link(char index, off_t offset, cllct_ctx* ctx)
{
	off_t addr;
	fseeko(ctx->tablefile, offset+index, SEEK_SET);
	if(fread(&addr, sizeof(uint64_t), 1, ctx->tablefile) != 1) {
		exit(CLLCT_ERROR_CANTREAD); /*TODO do not exit here!
																 * find a way to report the error (errno?) */
	}
	return addr;
}

int write_link(char index, off_t offset, uint64_t link_dest, cllct_ctx* ctx) {
	fseeko(ctx->tablefile, offset+index, SEEK_SET);
	if(fwrite(&link_dest, sizeof(uint64_t), 1, ctx->tablefile) != 1) {
		return CLLCT_ERROR_CANTWRITE;														  
	}
}

int insert_chararray(cllct_ctx* ctx, unsigned char* chars)
{
	char unitval;
	char creation_mode = 0;
	off_t offset, tryoffset;
	if(chars == 0) {
		return ERROR_INVALARG;
	}
	offset = 128;
	for(int i = 0; i < numunits; i++) {
		/* for each unit */
		if((i+1) % 2) {
			/* if it is the more significant nibble of the byte */
			unitval = (chars[i/2] >> 4);
		} else {
			/* or the other one */
			unitval = (chars[i/2]);
		}
		unitval &= 0xF /* need only the lower 4 bits for a nibble */
		
		if(creation_mode == 0) {
			tryoffset = get_link(unitval, offset, ctx);
			if(tryoffset == 0) {
				/* no link make it and all the following entries */
				creation_mode = 1;
				if(write_link(unitval, offset, (uint64_t)ctx->last_fileend, ctx) != 0) {
					return CLLCT_ERROR_CANTWRITE;
				}
			} else {
				/* link exists, follow it! */
				offset = tryoffset;
			}
		} else {
			/* TODO make a new entry for links */

			/* TODO also write the link to the next entry there */
		}
	}


	return 0;
}

int probe_chararray(cllct_ctx* ctx, unsigned char* chars);
int remove_chararray(cllct_ctx* ctx, unsigned char* chars);
int insert_hexchararray(cllct_ctx* ctx, unsigned char* digits);
int probe_hexchararray(cllct_ctx* ctx, unsigned char* digits);
int remove_hexchararray(cllct_ctx* ctx, unsigned char* digits);
