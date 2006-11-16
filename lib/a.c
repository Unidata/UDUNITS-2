#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

int
main(const char* argv, int argc)
{
    short	shorts[argc-1];
    char*	charString = "kg\xb7""m\xb3";
    char*	utf8String1 = "kg\xc2\xb7m\xe2\x81\xbb\xe2\x81\xb4\xc2\xb3";
    char*	utf8String2 = "kg\xc2\xb7m\xe2\x81\xba\xe2\x81\xb4\xc2\xb3";

    (void)printf("MB_LEN_MAX = %d\n", MB_LEN_MAX);
    (void)printf("MB_CUR_MAX = %d\n", MB_CUR_MAX);
    (void)printf("mblen(NULL, 1) = %d\n", mblen(NULL, 1));
    (void)printf("mblen(\"\\xe2\\x81\\xb4\", 3) = %d\n",
	mblen("\xe2\x81\xb4", 3));
    (void)printf("%s\n", charString);
    (void)printf("%s\n", utf8String1);
    (void)printf("%s\n", utf8String2);

    return 0;
}
