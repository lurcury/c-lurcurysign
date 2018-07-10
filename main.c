#include <stdio.h>
#include <string.h>

int main(){

    size_t i;
    char c;
    char v[2] = {0x01,0x02};
    int len;
    len = 1;
    for (i = 0; i < len; ++i) {
	char *b;
	char re[10];
        sprintf(b,"%02x", v[i]);
	strcat(re,b);
    }
    printf("\n");

}
