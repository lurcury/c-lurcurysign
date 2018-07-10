#include "common.h"
#include "endian.h"
#include "tx.h"
#include "hash.h"
#include "ec.h"
#include "base58.h"

void hash1601(char * address, char tmp[41]){
        uint8_t address_bytes[21];
        address_bytes[0] = 0x00;
        bbp_hash160(address_bytes + 1, address, 33);
//        bbp_print_hex("hash160", address_bytes + 1, 20);
        bbp_to_hex(tmp,address_bytes + 1, 20);
}

char * bbp_bytesTostring(const uint8_t *msg, size_t msg_len){
    size_t i;
    char ptr[10];
    static char result[10000];
	strcpy(result, "");
    for (i = 0; i < msg_len; ++i) {
        //printf("%02x", msg[i]);
	sprintf (ptr, "%02x", msg[i]);
	strcat(result,ptr);
    }
	return result;
}

char * build_t(int op,int in,int out,char * array[][6]) {

printf("1.top:%s\n",array[0][4]);
printf("1.top:%s\n",array[1][4]);
printf("1.top:%s\n",array[2][4]);

    bbp_txout_t outs[10];
    bbp_txout_t prev_outs[10];
    bbp_txin_t ins_sign[10];
    bbp_outpoint_t outpoint;
    bbp_tx_t tx;
    uint8_t *msg;
    size_t msg_len;

	int inint;
	int outint;
	int opint;
	int x = atoi(array[0][1]);

	for (inint=0;inint<in;inint++){
                int z = atoi(array[inint][3]);
		bbp_outpoint_fill(&outpoint, array[inint][2], z );
		int p = atoi(array[inint][1]);
		bbp_txout_create_p2pkh(&prev_outs[inint], p, array[inint][0]);
		bbp_txin_create_signable(&ins_sign[inint], &outpoint, &prev_outs[inint]);

	}

	for (outint=0;outint<out;outint++){
		char b[41];
	        hash1601(array[outint][4],b);
		//printf("result:%s\n",b);
		int p = atoi(array[outint][5]);
		bbp_txout_create_p2pkh(&outs[outint], p, array[outint][4]);
		printf("1.top:%s\n",array[outint][4]);
	}
/*
        for (opint=0;opint<op;opint++){
                //bbp_txout_create_p2pkh(&outs[outint], atoi(array[outint][5]), array[outint][4]);
                bbp_txout_create_opreturn(&outs[opint+out], 0, array[opint+out][6]);
        }
*/

    tx.version = bbp_eint32(BBP_LITTLE, 1);
    tx.outputs_len = out;
    tx.outputs = outs;
    tx.inputs_len = in;
    tx.inputs = ins_sign;
    tx.locktime = 0;
    msg_len = bbp_tx_size(&tx, BBP_SIGHASH_ALL);
    msg = malloc(msg_len);
    bbp_tx_serialize(&tx, msg, BBP_SIGHASH_ALL);

    return bbp_bytesTostring(msg,msg_len);
/*
    free(msg);
    bbp_txout_destroy(&outs[0]);
    bbp_txout_destroy(&outs[1]);
    bbp_txout_destroy(&prev_outs[0]);
    bbp_txin_destroy(&ins_sign[0]);
*/
}

char * pack(char* signContent,int op,int in,int out,char * array[][6], char * pubkey){
    bbp_txin_t ins[in];
    bbp_txout_t outs[out];
    bbp_outpoint_t outpoint;
    bbp_tx_t tx;
    uint8_t *rawtx;
    size_t rawtx_len;
    uint8_t txid[32];

        int inint;
        int outint;
	int opint;

        for (inint=0;inint<in;inint++){
		int z = atoi(array[inint][3]);
                bbp_outpoint_fill(&outpoint, array[inint][2], z );
		bbp_txin_create_p2pkh(&ins[inint], &outpoint,signContent,pubkey, BBP_SIGHASH_ALL);
        }
        for (outint=0;outint<out;outint++){
		int p = atoi(array[outint][5]);
                bbp_txout_create_p2pkh(&outs[outint], p, array[outint][4]);
		//printf("2.top:%s\n",array[outint][4]);
        }
/*
        for (opint=0;opint<op;opint++){
                //bbp_txout_create_p2pkh(&outs[outint], atoi(array[outint][5]), array[outint][4]);
                bbp_txout_create_opreturn(&outs[opint+out], 0, array[opint+out][6]);
        }
*/

    tx.version = bbp_eint32(BBP_LITTLE, 1);
    tx.outputs_len = out;
    tx.outputs = outs;
    tx.inputs_len = in;
    tx.inputs = ins;
    tx.locktime = 0;
    rawtx_len = bbp_tx_size(&tx, 0);
    rawtx = malloc(rawtx_len);
    bbp_tx_serialize(&tx, rawtx, 0);
    //txid
    bbp_hash256(txid, rawtx, rawtx_len);
    bbp_reverse(txid, 32);

//    bbp_print_hex("txid      ", txid, 32);

    bbp_print_hex("rawtx", rawtx, rawtx_len);
    return bbp_bytesTostring(rawtx, rawtx_len);

}

char* sign(char* txContent, char* privkey) {

    uint8_t priv[32];
    EC_KEY *key;
    uint8_t *msg;
    size_t msg_len;
    uint8_t digest[32];
    uint8_t *sig;
    unsigned int sig_len;

    /* */

    /* keypair */
    bbp_parse_hex(priv, privkey);
    key = bbp_ec_new_keypair(priv);

    /* message */
    msg = bbp_alloc_hex(txContent, &msg_len);

    /* signature */
    bbp_hash256(digest, msg, msg_len);
    sig_len = ECDSA_size(key);
    sig = malloc(sig_len);
    ECDSA_sign(0, digest, sizeof(digest), sig, &sig_len, key);

    bbp_print_hex("digest", digest, sizeof(digest));
    bbp_print_hex("signature", sig, sig_len);
/*
    free(sig);
    free(msg);
    EC_KEY_free(key);
*/
    return bbp_bytesTostring(sig, sig_len);
}
/*
struct RawtxStruct{
	const char in[][];
	const char out[][];
};
*/

void main() {
	int op = 0;
	int in = 1;
	int out = 3;
	char * array[41][6];
	char * txContent;
	char * signContent;
	char * priv = "KxegFjsrQqG849TC5a2KQbu7PFiKaWrP27anuxrQ58nH3DtpBwtm";
	char * pub = "03397400CF70FD2E6285299A200E0519CBF072ED5185043B8B8FDA872F84D73462";
	//from, balance, tx, num
	array[0][0] = "1Pi1Spap6vdfAWJPfMkYUCtG4EYM5fPWeR";
	array[0][1] = "7430";
	array[0][2] = "b5110aee256c189494653a0e20e28104f60b56bc3816d5b5bc49d56a5bc72dd6";
	array[0][3] = "2";
/*
        array[1][0] = "18ba14b3682295cb05230e31fecb000892406601";
        array[1][1] = "300000000";
        array[1][2] = "f34e1c37e736727770fed85d1b129713ef7f300304498c31c833985f487fa2f3";
        array[1][3] = "1";
*/
	// char* outAddress[out][40], char* outBalance[out][40],
//1Pi1Spap6vdfAWJPfMkYUCtG4EYM5fPWeR
	array[0][4] = "1Pi1Spap6vdfAWJPfMkYUCtG4EYM5fPWeR";
	array[1][4] = "1A1Um9vFzVm3dRgCdeZodH43dhMM1L1jP5";
	array[2][4] = "18U8TbSjyMmBgqxZKBW6uiq1X1R6aieR4P";
	array[0][5] = "546";
	array[1][5] = "100000000";
	array[2][5] = "100000000";
	//char* cicAddress[opreturn]
	array[0][6] = "40aafe9b86f2240b89bf8a6a096ea4556f2e97bf";
	//op,in,out

	int t;
        for(t=0;t<out;t++){
                char x[41];
		char *y;
		//memset(x, 0, sizeof(x));
                hash1601(array[t][4],x);
		printf("%itest%p",t, x);
		printf("hago:%s\n",x);
                //memcpy(y/*array[t][4]*/ , x, 40);
		memcpy(array[t][4], x, sizeof(x));
		//array[t][4] = y;
		printf("go:%s\n",array[t][4]);
                printf("go:%s\n",array[0][4]);
        }

                printf("array[0][4]:%s\n",array[0][4]);
	txContent = build_t(op,in,out,array);
	printf("%s\n",build_t(op,in,out,array));

	signContent = sign(txContent,priv);
	pack(signContent,op,in,out,array,pub);

/*

	char x[41];
	hash160("1Pi1Spap6vdfAWJPfMkYUCtG4EYM5fPWeR",x);
        printf("%s",x);

        char e[41];
        hash160("1Pi1Spap6vdfAWJPfMkYUCtG4EYM5fPWeR",e);
        printf("%s",e);
*/
/*
	uint8_t address_bytes[21];
	address_bytes[0] = 0x00;
	bbp_hash160(address_bytes + 1, "1Pi1Spap6vdfAWJPfMkYUCtG4EYM5fPWeR", 33);
	bbp_print_hex("hash160", address_bytes + 1, 20);
	char x[41] ;
	bbp_to_hex(x,address_bytes + 1, 20);
*/
//	printf("%s",x);
/*
	char *address;
	address = bbp_base58check(address_bytes, 21);
	printf("address      : %s\n", address);
*/
}

