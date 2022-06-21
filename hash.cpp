#include <random>
#include "bitboard.h"
#include "hash.h"

using namespace std;

namespace eia_v0_5
{
    Hash::Hash(HashStats * stats, MB size_mb)
        : age(0), stats(stats)
    {
        init(size_mb);
    }

    Hash::~Hash()
    {
        remove();
    }

    void Hash::init(MB size_mb)
    {
        count = (size_mb << 20) / sizeof(HashEntry);
	    count = msb(count);
	    table = new HashEntry[count];
	    clear();
    }

    void Hash::resize(MB size_mb)
    {
        remove();
        init(size_mb);
    }

    void Hash::clear()
    {
        for (HashEntry * the = table; the - table < count; the++)
        {
            the->key = EMPTY;
            the->move = None;
            the->type = HASH_EXACT;
            the->depth = 0;
            the->age = age;
            the->val = 0;
        }
    }

    void Hash::expire()
    {
        age++;
    }

    HashEntry * Hash::get(U64 key)
    {
        HashEntry * the = table + (key & (count - 1));
        if (the->key == key)
        {
            stats->hash_read++;
            return the;
        }
        return nullptr;
    }

    HashEntry * Hash::get(U64 key, int & alpha, int & beta, int depth, int ply)
    {
        HashEntry * the = table + (key & (count - 1));
        if (the->key == key)
        {
            stats->hash_read++;

            if (ply > 0)
            {
                if (the->depth >= depth)
                {
                    int val = static_cast<int>(the->val);
                    if      (val >  MATE) val -= ply;
                    else if (val < -MATE) val += ply;

                    // Exact score
                    if (the->type == HASH_EXACT) alpha = beta = val;
                    else if (the->type == HASH_ALPHA && val <= alpha) beta = alpha;
                    else if (the->type == HASH_BETA  && val >= beta) alpha = beta;
                }
            }
            return the;
        }
        return nullptr;
    }

    void Hash::set(U64 key, Move move, int val, int type, int depth, int ply)
    {
        stats->hash_write++;
        HashEntry * the = table + (key & (count - 1));
	
        //if (/*the->age == age && */the->depth > depth) return;

        if      (val >  MATE && val <=  INF) val += ply;
        else if (val < -MATE && val >= -INF) val -= ply;

        the->key   = key;
        the->move  = move;
        the->type  = type;
        the->depth = depth;
        the->age   = age;
        the->val   = val;
    }

    void Hash::remove()
    {
        delete[] table;
    }

    HashTables::HashTables()
    {
        mt19937_64 gen(0xC0FFEE);
        uniform_int_distribution<U64> distr;

        for (int i = 0; i < 12; i++)
            for (int j = 0; j < 64; j++)
                hash_key[i][j] = distr(gen);

        for (int i = 0; i < 16; i++)
            hash_castle[i] = i ? distr(gen) : EMPTY;

        for (int i = 0; i < 64; i++)
        {
            int y = y_(static_cast<SQ>(i));
            hash_ep[i] = (y == 2 || y == 5) ? distr(gen) : EMPTY;
        }

        hash_wtm = distr(gen);
    }
}
