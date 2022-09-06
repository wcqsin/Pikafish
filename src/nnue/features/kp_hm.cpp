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

#include "kp_hm.h"

#include "../../position.h"

namespace Stockfish::Eval::NNUE::Features {

  // Index of a feature for a given king bucket and another piece on some square
  inline IndexType KP_hm::make_index(Square s, Piece pc, int bucket) {
    return IndexType(PS_NB * (bucket & 63) + PieceSquareIndex[pc] + (bucket >> 6 ? Mirror[s] : s));
  }

  // Get the king bucket
  int KP_hm::king_bucket(const Position& pos) {
    return KingBuckets[KingMaps[pos.square<KING>(WHITE)] + KingMaps[pos.square<KING>(BLACK)]];
  }

  // Get a list of indices for active features
  void KP_hm::append_active_indices(
    const Position& pos,
    IndexList& active
  ) {
    int bucket = king_bucket(pos);
    Bitboard bb = pos.pieces() & ~pos.pieces(KING);
    while (bb)
    {
      Square s = pop_lsb(bb);
      active.push_back(make_index(s, pos.piece_on(s), bucket));
    }
  }

  // append_changed_indices() : get a list of indices for recently changed features

  void KP_hm::append_changed_indices(
    int bucket,
    const DirtyPiece& dp,
    IndexList& removed,
    IndexList& added
  ) {
    for (int i = 0; i < dp.dirty_num; ++i) {
      if (dp.from[i] != SQ_NONE)
        removed.push_back(make_index(dp.from[i], dp.piece[i], bucket));
      if (dp.to[i] != SQ_NONE)
        added.push_back(make_index(dp.to[i], dp.piece[i], bucket));
    }
  }

  int KP_hm::update_cost(const StateInfo* st) {
    return st->dirtyPiece.dirty_num;
  }

  int KP_hm::refresh_cost(const Position& pos) {
    return pos.count<ALL_PIECES>() - 2;
  }

  bool KP_hm::requires_refresh(const StateInfo* st) {
    return type_of(st->dirtyPiece.piece[0]) == KING;
  }

}  // namespace Stockfish::Eval::NNUE::Features
