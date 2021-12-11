#include "piece.h"
#include "utils.h"

namespace eia_v0_5
{
    const Rays wp_offset = {{-1, 1}, {1, 1}};
    const Rays bp_offset = {{-1,-1}, {1,-1}};

    const Rays n_offset =
	{
		{1, 2}, {1,-2}, {-1, 2}, {-1,-2},
		{2, 1}, {2,-1}, {-2, 1}, {-2,-1}
	};

    const Rays k_offset =
	{
		{-1, 1}, {0, 1}, {1, 1},
		{-1, 0},         {1, 0},
		{-1,-1}, {0,-1}, {1,-1}
	};

    const Rays diag_offset = {{-1,-1}, {-1, 1}, {1,-1}, {1, 1}};
	const Rays rook_offset = {{-1, 0}, {0, 1}, {1, 0}, {0,-1}};

    PieceTables::PieceTables()
    {
        for (int piece = 0; piece < PIECE_N; piece++)
            for (int sq = 0; sq < SQUARE_N; sq++)
                att[piece][sq] = EMPTY;

        init_piece(WP, wp_offset);
        init_piece(BP, bp_offset);
        init_piece(WN, n_offset);
        init_piece(BN, n_offset);
        init_piece(WK, k_offset);
        init_piece(BK, k_offset);
        init_piece(WB, diag_offset, true);
        init_piece(BB, diag_offset, true);
        init_piece(WQ, diag_offset, true);
        init_piece(BQ, diag_offset, true);
        init_piece(WR, rook_offset, true);
        init_piece(BR, rook_offset, true);
        init_piece(WQ, rook_offset, true);
        init_piece(BQ, rook_offset, true);

        for (SQ sq = A1; sq < SQUARE_N; sq++)
        {
            forward_one[0][sq] = EMPTY;
            forward_one[1][sq] = EMPTY;
            if (y_(sq) < 7) forward_one[0][sq] = (BIT << sq) >> 8;
            if (y_(sq) > 0) forward_one[1][sq] = (BIT << sq) << 8;
        }

        for (SQ i = A1; i < SQUARE_N; i++)
        {
            for (SQ j = i + 1; j < SQUARE_N; j++) // j > i
            {
                dir[i][j] = 0;
                between[i][j] = EMPTY;
                int dx = x_(j) - x_(i);
                int dy = y_(j) - y_(i);

                if (abs(dx) == abs(dy) // Diagonal
                ||  !dx || !dy)        // Orthogonal
                {
                    int sx = sgn(dx);
                    int sy = sgn(dy);
                    int dt = sgn(dx) + 8 * sgn(dy);
                    dir[i][j] = dt;

                    for (int k = i + dt; k < j; k += dt)
                        between[i][j] |= BIT << k;
                }

                dir[j][i] = -dir[i][j];
                between[j][i] = between[i][j];
            }
        }
    }

    void PieceTables::init_piece(Piece piece, Rays rays, bool slider)
    {
        for (SQ sq = A1; sq < SQUARE_N; sq++)
        {
            for (auto ray : rays)
            {
                int x = x_(sq);
                int y = y_(sq);

                do
                {
                    x += ray.first;
                    y += ray.second;

                    if (x < 0 || x > 7 || y < 0 || y > 7) break;

                    att[piece][sq] |= BIT << sq_(x, y);
                }
                while (slider);
            }
        }
    }
}
