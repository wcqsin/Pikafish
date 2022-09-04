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

#include <iostream>

#include "bitboard.h"
#include "position.h"
#include "search.h"
#include "thread.h"
#include "uci.h"

#include "tune.h"

using namespace Stockfish;

void updatePieveValue() {
  PieceValue[MG][ROOK] = RookValueMg;
  PieceValue[EG][ROOK] = RookValueEg;
  PieceValue[MG][BISHOP] = BishopValueMg;
  PieceValue[EG][BISHOP] = BishopValueEg;
  PieceValue[MG][KNIGHT] = KnightValueMg;
  PieceValue[EG][KNIGHT] = KnightValueEg;
  PieceValue[MG][ADVISOR] = AdvisorValueMg;
  PieceValue[EG][ADVISOR] = AdvisorValueEg;
  PieceValue[MG][CANNON] = CannonValueMg;
  PieceValue[EG][CANNON] = CannonValueEg;
  PieceValue[MG][PAWN] = PawnValueMg;
  PieceValue[EG][PAWN] = PawnValueEg;
}

TUNE(
RookValueMg, AdvisorValueMg, CannonValueMg, PawnValueMg, KnightValueMg, BishopValueMg,
RookValueEg, AdvisorValueEg, CannonValueEg, PawnValueEg, KnightValueEg, BishopValueEg,
updatePieveValue
);


int main(int argc, char* argv[]) {

  std::cout << engine_info() << std::endl;

  CommandLine::init(argc, argv);
  UCI::init(Options);
  Tune::init();
  Bitboards::init();
  Position::init();
  Threads.set(size_t(Options["Threads"]));
  Search::clear(); // After threads are up
  Eval::NNUE::init();

  UCI::loop(argc, argv);

  Threads.set(0);
  return 0;
}
