#include <cassert>
#include <iostream>
#include "movelist.h"

using namespace std;

namespace eia_v0_5
{
    void MoveList::add(Move move, int val)
    {
        if (move == hashmove) return;
        assert(last - first < 256);
        last->move = move;
        last->val = val;
        ++last;
    }

    void MoveList::print() const
    {
        for (MoveVal * ptr = first; ptr != last; ptr++)
        {
            cout << ptr->move << " - " << ptr->val << endl;
        }
        cout << endl;
    }

    void MoveList::set_values()
    {
    }

    void MoveList::add_move(SQ from, SQ to, Flags flags)
    {
        add(build_move(from, to, flags));
    }

    void MoveList::add_prom(SQ from, SQ to)
    {
        add(build_move(from, to, F_QPROM));
        add(build_move(from, to, F_RPROM));
        add(build_move(from, to, F_BPROM));
        add(build_move(from, to, F_NPROM));
    }

    void MoveList::add_capprom(SQ from, SQ to)
    {
        add(build_move(from, to, F_QCAPPROM));
        add(build_move(from, to, F_RCAPPROM));
        add(build_move(from, to, F_BCAPPROM));
        add(build_move(from, to, F_NCAPPROM));
    }

    void MoveList::remove(MoveVal * ptr)
    {
        *ptr = *(last - 1);
        --last;
    }

    void MoveList::clear(Move hash_move)
    {
        last = first = moves;
        hashmove = None;
        if (hash_move != None)
        {
            add(hash_move, 100000);
            hashmove = hash_move;
        }
    }

    Move MoveList::get_next_move()
    {
        if (empty()) return None;
        return (first++)->move;
    }

    Move MoveList::get_best_move(int lower_bound)
    {
        if (empty()) return None;
        MoveVal * best = first;
        for (MoveVal * ptr = first; ptr != last; ++ptr)
        {
            if (ptr->val > best->val)
            {
                best = ptr;
            }
        }
        Move move = best->move;
        remove(best);
        return move;
    }
}
