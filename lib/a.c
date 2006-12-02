#include <limits.h>
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

int
main()
{
    char*	charString = "kg\xb7""m\xb3";
    char*	utf8String1 = "kg\xc2\xb7m\xe2\x81\xbb\xe2\x81\xb4\xc2\xb3";
    char*	utf8String2 = "kg\xc2\xb7m\xe2\x81\xba\xe2\x81\xb4\xc2\xb3";

    (void)printf("setlocale(LC_ALL,NULL): %s\n", setlocale(LC_ALL,NULL));
    (void)printf("setlocale(LC_ALL,\"\"): %s\n", setlocale(LC_ALL,""));

    (void)printf("MB_LEN_MAX = %d\n", MB_LEN_MAX);
    (void)printf("MB_CUR_MAX = %d\n", MB_CUR_MAX);
    (void)printf("mblen(NULL, 1) = %d\n", mblen(NULL, 1));
    (void)printf("mblen(\"\\xe2\\x81\\xb4\", 3) = %d\n",
	mblen("\xe2\x81\xb4", 3));
    (void)printf("%s\n", charString);
    (void)printf("%s\n", utf8String1);
    (void)printf("%s\n", utf8String2);
    (void)printf("UTF-8 exponent 4: \"%s\"\n", "\xe2\x81\xb4");

    return 0;
}
