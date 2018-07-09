#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
int main(){
	char hex[3];
	unsigned uint8 my_byte[1] = {42};
	//printf("%02x", pub[i]);
	sprintf(hex,"%02X", my_byte);
	printf("%s",hex);
	return 0;
}
