#include "common.h"
#include "endian.h"
#include "tx.h"
#include "hash.h"
#include "ec.h"



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

char * build_t(int in,int out,char * array[][5]) {
    bbp_txout_t outs[10];
    bbp_txout_t prev_outs[10];
    bbp_txin_t ins_sign[10];
    bbp_outpoint_t outpoint;
    bbp_tx_t tx;
    uint8_t *msg;
    size_t msg_len;

	int inint;
	int outint;
	int x = atoi(array[0][1]);
	for (inint=0;inint<in;inint++){
		bbp_outpoint_fill(&outpoint, array[inint][2], atoi(array[inint][3]) );
		bbp_txout_create_p2pkh(&prev_outs[inint], atoi( array[inint][1] ), array[inint][0]);
		//bbp_txout_create_opreturn(&prev_outs[inint], atoi( array[inint][1] ), array[inint][0]);
		bbp_txin_create_signable(&ins_sign[inint], &outpoint, &prev_outs[inint]);

	}
	for (outint=0;outint<out;outint++){
		bbp_txout_create_p2pkh(&outs[outint], atoi(array[outint][5]), array[outint][4]);
	}


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

char * pack(char* signContent,int in,int out,char * array[][5]){
    bbp_txin_t ins[in];
    bbp_txout_t outs[out];
    bbp_outpoint_t outpoint;
    bbp_tx_t tx;
    uint8_t *rawtx;
    size_t rawtx_len;
    uint8_t txid[32];

        int inint;
        int outint;


        for (inint=0;inint<in;inint++){
                bbp_outpoint_fill(&outpoint, array[inint][2], atoi(array[inint][3]) );
		bbp_txin_create_p2pkh(&ins[inint], &outpoint,signContent,"0282006e9398a6986eda61fe91674c3a108c399475bf1e738f19dfc2db11db1d28", BBP_SIGHASH_ALL);
        }
        for (outint=0;outint<out;outint++){
                bbp_txout_create_p2pkh(&outs[outint], atoi(array[outint][5]), array[outint][4]);
        }

    tx.version = bbp_eint32(BBP_LITTLE, 1);
    tx.outputs_len = out;
    tx.outputs = outs;
    tx.inputs_len = in;
    tx.inputs = ins;
    tx.locktime = 0;
    rawtx_len = bbp_tx_size(&tx, 0);
    rawtx = malloc(rawtx_len);
    bbp_tx_serialize(&tx, rawtx, 0);
    bbp_print_hex("rawtx", rawtx, rawtx_len);
    return bbp_bytesTostring(rawtx, rawtx_len);

}

char* sign(char* txContent) {

    uint8_t priv[32];
    EC_KEY *key;
    uint8_t *msg;
    size_t msg_len;
    uint8_t digest[32];
    uint8_t *sig;
    unsigned int sig_len;

    /* */

    /* keypair */
    bbp_parse_hex(priv, "16260783e40b16731673622ac8a5b045fc3ea4af70f727f3f9e92bdd3a1ddc42");
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
	char * array[40][5];
	char * txContent;
	char * signContent;
	//from, balance, tx, num
	array[0][0] = "18ba14b3682295cb05230e31fecb000892406601";
	array[0][1] = "300000000";
	array[0][2] = "f34e1c37e736727770fed85d1b129713ef7f300304498c31c833985f487fa2f3";
	array[0][3] = "0";
        array[1][0] = "18ba14b3682295cb05230e31fecb000892406601";
        array[1][1] = "300000000";
        array[1][2] = "f34e1c37e736727770fed85d1b129713ef7f300304498c31c833985f487fa2f3";
        array[1][3] = "1";
	// char* outAddress[out][40], char* outBalance[out][40],
	array[0][4] = "18ba14b3682295cb05230e31fecb000892406601";
	array[1][4] = "18ba14b3682295cb05230e31fecb000892406605";
	array[2][4] = "18ba14b3682295cb05230e31fecb000892406609";
	array[0][5] = "100000000";
	array[1][5] = "100000000";
	array[2][5] = "100000000";
	txContent = build_t(2,3,array);
	printf("%s\n",build_t(2,3,array));
	signContent = sign(txContent);
	pack(signContent,2,3,array);
}

