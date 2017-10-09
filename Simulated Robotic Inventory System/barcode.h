#ifndef BARCODE_H_
#define BARCODE_H_

#define MAX_CODE_LEN   24

extern unsigned char	__base64_codetable_initialized;
char *decode_cat(char *buf, int length);

#endif /* BARCODE_H_ */
