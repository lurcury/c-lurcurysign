#include "base58.h"
#include "common.h"
#include "ec.h"
//#include "hex.c"

struct KeyData {

	char* privateKey;
	char* uncomppressed_publicKey;
	char* uncomppressed_address;
	char* compressed_publicKey;
	char* compressed_address;

};

char* toHex(uint8_t vpriv_bytes[],int size){
        char buffer [100];
	static char pubKey [2048];
        char target[2048] = "";
        for(int i =0;i < size; i++){
                sprintf (buffer, "%02X", vpriv_bytes[i]);
                strcat(target, buffer);
        }
	strcpy(pubKey,target);
        return pubKey;
}

char* publickeytoaddrsss(uint8_t pub_bytes[],size_t size){
	uint8_t address_bytes[210];
	char *address;
	static char addressSta [2048];
	const char address_exp[] = "mqMi3XYqsPvBWtrJTk8euPWDVmFTZ5jHuK";
	address_bytes[0] = 0x00;
	bbp_hash160(address_bytes+1, pub_bytes, size);
	address = bbp_base58check(address_bytes, 21);
	printf("address      : %s\n", address);
	strcpy(addressSta, address);
	free(address);
	return addressSta;
}

int keyPublish() {
    
    struct KeyData r;
    uint8_t priv_bytes[32] = {
        0x16, 0x26, 0x07, 0x83, 0xe4, 0x0b, 0x16, 0x73,
        0x16, 0x73, 0x62, 0x2a, 0xc8, 0xa5, 0xb0, 0x45,
        0xfc, 0x3e, 0xa4, 0xaf, 0x70, 0xf7, 0x27, 0xf3,
        0xf9, 0xe9, 0x2b, 0xdd, 0x3a, 0x1d, 0x01, 0x09
    };

    EC_KEY *key;
    uint8_t priv[32];
    uint8_t *pub;
    const BIGNUM *priv_bn;

    point_conversion_form_t conv_forms[] = {
        POINT_CONVERSION_UNCOMPRESSED,
        POINT_CONVERSION_COMPRESSED
    };
    const char *conv_forms_desc[] = {
        "uncompressed",
        "compressed"
    };
    int i;

    key = bbp_ec_new_keypair(priv_bytes);
    if (!key) {
        puts("Unable to create keypair");
        return -1;
    }
    bbp_print_hex("privateKey:", priv_bytes, sizeof(priv));
    priv_bn = EC_KEY_get0_private_key(key);
    if (!priv_bn) {
        puts("Unable to decode private key");
        return -1;
    }
    BN_bn2bin(priv_bn, priv);
    for (i = 0; i < sizeof(conv_forms) / sizeof(point_conversion_form_t); ++i) {
        size_t pub_len;
        uint8_t *pub_copy;

        EC_KEY_set_conv_form(key, conv_forms[i]);

        pub_len = i2o_ECPublicKey(key, NULL);
        pub = calloc(pub_len, sizeof(uint8_t));
        pub_copy = pub;
        if (i2o_ECPublicKey(key, &pub_copy) != pub_len) {
            puts("Unable to decode public key");
            return -1;
        }
	printf("publicKey:%s\n",toHex(pub,pub_len));
	publickeytoaddrsss(pub,pub_len);
        free(pub);
    }
    EC_KEY_free(key);

    return 0;
}
/*
int main(){
	keyPublish();

}
*/

struct KeyData go(){
	struct KeyData r;
	r.privateKey = "abc";
	printf("%s",r.privateKey);
	return r;
}


int main(){
	struct KeyData r;
	go();
	//printf("%s",r.privateKey);
	return 0;

}

