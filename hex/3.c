#include <stdint.h>
#include <stdio.h>
int main(){
	uint8_t buf[] = {18,10,11};
	printf("%s\n","run byte to hex");
		
	for (int i = 0; i < 3; ++i) {
		printf("%02x\n", buf[i]);
	}

}
