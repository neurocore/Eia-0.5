#pragma once
#include "moves.h"

namespace eia_v0_5
{
    enum HashType : U8 { HASH_ALPHA, HASH_EXACT, HASH_BETA };

    struct HashEntry // POD - 16 bytes
    {
        U64 key;   // 8
        Move move; // 2
        U8 type;   // 1
        U8 depth;  // 1
        U16 age;   // 2
        S16 val;   // 2
    };

    struct HashTables
    {
        U64 hash_key[12][64];
        U64 hash_castle[16];
        U64 hash_ep[64];
        U64 hash_wtm;

        HashTables();
    };

    struct HashStats
    {
        int hash_read;
        int hash_write;
    };

    class Hash
    {
        U16 age;
        U64 count;
        HashEntry * table;
        HashStats * stats;

    public:
        explicit Hash(HashStats * stats, MB size_mb = HASH_SIZE);
        ~Hash();

        void resize(MB size_mb = HASH_SIZE);
        void expire();

        HashEntry * get(U64 key);
        HashEntry * get(U64 key, int & alpha, int & beta, int depth, int ply);
        void set(U64 key, Move move, int val, int type, int depth, int ply);

    private:
        void init(MB size_mb = HASH_SIZE);
        void remove();
        void clear();
    };

    extern HashTables * HT;
    extern Hash * H;
}
