#include "common.h"
#include "endian.h"
#include "tx.h"
#include "hash.h"
#include "ec.h"



int main() {
    bbp_txin_t ins[1];
    bbp_txout_t outs[2];
    bbp_outpoint_t outpoint;
    bbp_tx_t tx;
    uint8_t *rawtx;
    size_t rawtx_len;
    uint8_t txid[32];
    bbp_outpoint_fill(&outpoint, "f34e1c37e736727770fed85d1b129713ef7f300304498c31c833985f487fa2f3", 0);
    bbp_txin_create_p2pkh(&ins[0], &outpoint, "30440220111a482aba6afba12a6f27de767dd4d06417def665bd100bc68c42845c752a8f02205e86f5e054b2c6cac5d663664e35779fb034387c07848bc7724442cacf659324", "0282006e9398a6986eda61fe91674c3a108c399475bf1e738f19dfc2db11db1d28", BBP_SIGHASH_ALL);
    bbp_txout_create_p2pkh(&outs[0], 25100000, "18ba14b3682295cb05230e31fecb000892406608");
    bbp_txout_create_p2pkh(&outs[1], 61900000, "6bf19e55f94d986b4640c154d864699341919511");
    tx.version = bbp_eint32(BBP_LITTLE, 1);
    tx.outputs_len = 2;
    tx.outputs = outs;
    tx.inputs_len = 1;
    tx.inputs = ins;
    tx.locktime = 0;
    rawtx_len = bbp_tx_size(&tx, 0);
    rawtx = malloc(rawtx_len);
    bbp_tx_serialize(&tx, rawtx, 0);
    bbp_print_hex("rawtx", rawtx, rawtx_len);
    return 0;
}
