#pragma once
#include <ostream>
#include <string>
#include "utils.h"
#include "piece.h"

using std::ostream;
using std::string;

namespace eia_v0_5
{
    enum Flags
    {
        F_QUIET,     // PC12 (Promotion, Capture, ...)
        F_PAWN2,
        F_KCASTLE,   // 0010
        F_QCASTLE,   // 0011
        F_CAP,       // 0100
        F_EP,	     // 0101
        F_NPROM = 8, // 1000
        F_BPROM,     // 1001
        F_RPROM,     // 1010
        F_QPROM,     // 1011
        F_NCAPPROM,  // 1100
        F_BCAPPROM,  // 1101
        F_RCAPPROM,  // 1110
        F_QCAPPROM   // 1111
    };

    // | 0..5 | 6..11 | 12..15 | = 16 bits
    // | FROM |  TO   | FLAGS  |
    enum Move : U16 { None };

    class History
    {
        int table[PIECE_N][SQUARE_N];

    public:
        History() { clear(); }
        void clear();
        void add(Piece p, SQ s, int depth);
        int get(Piece p, SQ s) const;

    private:
        void shrink();
    };

    extern Flags recognize_prom(char ch, bool capture = false);
    
    extern inline Move build_move(SQ from, SQ to, Flags flags = F_QUIET)
    {
        return Move(from | (to << 6) | (flags << 12));
    }

    extern inline Move build_prom(SQ from, SQ to, PieceType prom)
    {
        const int flags = F_NPROM + prom - 1;
        return Move(from | (to << 6) | (flags << 12));
    }

    extern inline Move build_capprom(SQ from, SQ to, PieceType prom)
    {
        const int flags = F_NCAPPROM + prom - 1;
        return Move(from | (to << 6) | (flags << 12));
    }

    extern inline bool is_cap(Move move)         { return !!(move & (F_CAP << 12)); }
    extern inline bool is_prom(Move move)        { return !!(move & (F_NPROM << 12)); }
    extern inline bool is_cap_or_prom(Move move) { return !!(move & (F_NCAPPROM << 12)); }

    extern inline bool is_cap(Flags flags)         { return !!(flags & F_CAP); }
    extern inline bool is_prom(Flags flags)        { return !!(flags & F_NPROM); }
    extern inline bool is_cap_or_prom(Flags flags) { return !!(flags & F_NCAPPROM); }

    extern inline Flags get_flags(Move move) { return static_cast<Flags>(move >> 12); }
    extern inline SQ get_from(Move move) { return static_cast<SQ>(move & 077); }
    extern inline SQ get_to(Move move) { return static_cast<SQ>((move >> 6) & 077); }
    extern inline Piece get_prom(Move move, int wtm = 0)
    {
        return static_cast<Piece>(2 * ((move >> 12) & 3) + BN + wtm);
    }

    extern inline Piece get_prom(Flags flags, int wtm = 0)
    {
        return static_cast<Piece>(2 * (flags & 3) + BN + wtm);
    }
    
    ostream & operator << (ostream & os, const Move move);
}
