/*
  Stockfish, a UCI chess playing engine derived from Glaurung 2.1
  Copyright (C) 2004-2022 The Stockfish developers (see AUTHORS file)

  Stockfish is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Stockfish is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// Definition of input features KP_hm of NNUE evaluation function

#ifndef NNUE_FEATURES_HALF_KA_V2_XQ_H_INCLUDED
#define NNUE_FEATURES_HALF_KA_V2_XQ_H_INCLUDED

#include "../nnue_common.h"

#include "../../evaluate.h"
#include "../../misc.h"

namespace Stockfish {
  struct StateInfo;
}

namespace Stockfish::Eval::NNUE::Features {

  // Feature KP_hm: Combination of the position of two kings and the position of pieces.
  // Position mirrored if white king is on file f or white king is on file e and black king is on file f
  class KP_hm {

    // unique number for each piece type on each square
    enum {
      PS_NONE      =  0,
      PS_W_ROOK    =  0,
      PS_B_ROOK    =  1  * SQUARE_NB,
      PS_W_ADVISOR =  2  * SQUARE_NB,
      PS_B_ADVISOR =  3  * SQUARE_NB,
      PS_W_CANNON  =  4  * SQUARE_NB,
      PS_B_CANNON  =  5  * SQUARE_NB,
      PS_W_PAWN    =  6  * SQUARE_NB,
      PS_B_PAWN    =  7  * SQUARE_NB,
      PS_W_KNIGHT  =  8  * SQUARE_NB,
      PS_B_KNIGHT  =  9  * SQUARE_NB,
      PS_W_BISHOP  =  10 * SQUARE_NB,
      PS_B_BISHOP  =  11 * SQUARE_NB,
      PS_NB        =  12 * SQUARE_NB
    };

    static constexpr IndexType PieceSquareIndex[PIECE_NB] = {
      // convention: W - us, B - them
      PS_NONE, PS_W_ROOK, PS_W_ADVISOR, PS_W_CANNON, PS_W_PAWN, PS_W_KNIGHT, PS_W_BISHOP, PS_NONE,
      PS_NONE, PS_B_ROOK, PS_B_ADVISOR, PS_B_CANNON, PS_B_PAWN, PS_B_KNIGHT, PS_B_BISHOP, PS_NONE
    };

    // Index of a feature for a given king bucket and another piece on some square
    static IndexType make_index(Square s, Piece pc, int bucket);

   public:
    // Feature name
    static constexpr const char* Name = "KP_hm";

    // Hash value embedded in the evaluation file
    static constexpr std::uint32_t HashValue = 0xd17b100;

    // Number of feature dimensions
    static constexpr IndexType Dimensions = (3 * 9 + 3 * 6) * PS_NB;

    static constexpr int KingMaps[SQUARE_NB] = {
         0,  0,  0,  0,  9, 18,  0,  0,  0,
         0,  0,  0, 27, 36, 45,  0,  0,  0,
         0,  0,  0, 54, 63, 72,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  6,  7,  8,  0,  0,  0,
         0,  0,  0,  3,  4,  5,  0,  0,  0,
         0,  0,  0,  0,  1,  2,  0,  0,  0,
    };

#define M(s) ((1 << 6) | s)
    // Stored as (mirror << 6 | bucket)
    static constexpr int KingBuckets[81] = {
           0 ,    1 ,    2 ,    3 ,    4 ,    5 ,    6 ,    7 ,    8 , // white king on  0, no mirror
           9 ,   10 , M( 9),   11 ,   12 , M(11),   13 ,   14 , M(13), // white king on  9, mirror for black
        M( 2), M( 1), M( 0), M( 5), M( 4), M( 3), M( 8), M( 7), M( 6), // white king on 18, mirror for white
          15 ,   16 ,   17 ,   18 ,   19 ,  20 ,    21 ,   22 ,   23 , // white king on 27, no mirror
          24 ,   25 , M(24),   26 ,   27 , M(26),   28 ,   29 , M(28), // white king on 36, mirror for black
        M(17), M(16), M(15), M(20), M(19), M(18), M(23), M(22), M(21), // white king on 45, mirror for white
          30 ,   31 ,   32 ,   33 ,   34 ,   35 ,   36 ,   37 ,   38 , // white king on 54, no mirror
          39 ,   40 , M(39),   41 ,   42 , M(41),   43 ,   44 , M(43), // white king on 63, mirror for black
        M(32), M(31), M(30), M(35), M(34), M(33), M(38), M(37), M(36), // white king on 72, mirror for white
    };
#undef M

    // Mirror a square
    static constexpr int Mirror[SQUARE_NB] = {
         8,  7,  6,  5,  4,  3,  2,  1,  0,
        17, 16, 15, 14, 13, 12, 11, 10,  9,
        26, 25, 24, 23, 22, 21, 20, 19, 18,
        35, 34, 33, 32, 31, 30, 29, 28, 27,
        44, 43, 42, 41, 40, 39, 38, 37, 36,
        53, 52, 51, 50, 49, 48, 47, 46, 45,
        62, 61, 60, 59, 58, 57, 56, 55, 54,
        71, 70, 69, 68, 67, 66, 65, 64, 63,
        80, 79, 78, 77, 76, 75, 74, 73, 72,
        89, 88, 87, 86, 85, 84, 83, 82, 81,
    };

    // Maximum number of simultaneously active features.
    static constexpr IndexType MaxActiveDimensions = 30;
    using IndexList = ValueList<IndexType, MaxActiveDimensions>;

    // Get the king bucket
    static int king_bucket(const Position& pos);

    // Get a list of indices for active features
    static void append_active_indices(
      const Position& pos,
      IndexList& active);

    // Get a list of indices for recently changed features
    static void append_changed_indices(
      int bucket,
      const DirtyPiece& dp,
      IndexList& removed,
      IndexList& added
    );

    // Returns the cost of updating one perspective, the most costly one.
    // Assumes no refresh needed.
    static int update_cost(const StateInfo* st);
    static int refresh_cost(const Position& pos);

    // Returns whether the change stored in this StateInfo means that
    // a full accumulator refresh is required.
    static bool requires_refresh(const StateInfo* st);
  };

}  // namespace Stockfish::Eval::NNUE::Features

#endif // #ifndef NNUE_FEATURES_HALF_KA_V2_XQ_H_INCLUDED
