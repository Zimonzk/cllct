#include "cllct.h"
#include "zimonzk/logger.h"
#include <stdlib.h>

int main() {
	cllct_ctx_t ctx;
	set_verbosity(100);
	zlog(50, "Writing all 16bit numbers");
	if(init_cllct_ctx(&ctx, "test.cllcf", 4) != 0) {
		fatal("Can't init");
	}
	zlog(10, "init fine");
	//system("pause");
	for(unsigned int i = 0; i < 65536; i++) {
		zlog(90, "Writing #%u", i);
		int ret = insert_chararray(&ctx, (unsigned char*)&i);
		if(ret != 0) {
			fatal("Insert #%u failed. errorcode: %i", i, ret);
		}
	}
	zlog(10, "All done.");
}
