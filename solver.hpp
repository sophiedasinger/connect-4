//#include "stdafx.h"
#include <cassert>
#include "position.hpp"

using namespace GameSolver::Connect4;


namespace GameSolver {
	namespace Connect4 {
		struct Move {
			int Score;
			int Column;
		};

		/*
		* A class to solve Connect 4 position using Nagemax variant of min-max algorithm.
		*/
		class Solver {
		private:
			const int MAX_DEPTH = 8;
			
			unsigned long long nodeCount; // counter of explored nodes.

			int columnOrder[Position::WIDTH]; // column exploration order

											  /**
											  * Reccursively score connect 4 position using negamax variant of alpha-beta algorithm.
											  * @param: alpha < beta, a score window within which we are evaluating the position.
											  *
											  * @return the exact score, an upper or lower bound score depending of the case:
											  * - if actual score of position <= alpha then actual score <= return value <= alpha
											  * - if actual score of position >= beta then beta <= return value <= actual score
											  * - if alpha <= actual score <= beta then return value = actual score
											  */
			int negamax(const Position &P, int alpha, int beta, int depth) {
				assert(alpha < beta);
				nodeCount++; // increment counter of explored nodes

				if (P.nbMoves() == Position::WIDTH*Position::HEIGHT) // check for draw game
					return 0;

				for (int x = 0; x < Position::WIDTH; x++) // check if current player can win next move
					if (P.canPlay(x) && P.isWinningMove(x))
						return (Position::WIDTH*Position::HEIGHT + 1 - P.nbMoves()) / 2;

				int max = (Position::WIDTH*Position::HEIGHT - 1 - P.nbMoves()) / 2;	// upper bound of our score as we cannot win immediately
				if (beta > max) {
					beta = max;                     // there is no need to keep beta above our max possible score.
					if (alpha >= beta) return beta;  // prune the exploration if the [alpha;beta] window is empty.
				}

				if (depth > MAX_DEPTH) {
					return beta;
				}

				for (int x = 0; x < Position::WIDTH; x++) // compute the score of all possible next move and keep the best one
					if (P.canPlay(columnOrder[x])) {
						Position P2(P);
						P2.play(columnOrder[x]);               // It's opponent turn in P2 position after current player plays x column.
						int score = -negamax(P2, -beta, -alpha, depth+1); // explore opponent's score within [-beta;-alpha] windows:
																 // no need to have good precision for score better than beta (opponent's score worse than -beta)
																 // no need to check for score worse than alpha (opponent's score worse better than -alpha)

						if (score >= beta) return score;  // prune the exploration if we find a possible move better than what we were looking for.
						if (score > alpha) alpha = score; // reduce the [alpha;beta] window for next exploration, as we only 
														  // need to search for a position that is better than the best so far.
					}

				return alpha;
			}

		public:

			int solve(const Position &P)
			{
				nodeCount = 0;
				return negamax(P, -Position::WIDTH*Position::HEIGHT / 2, Position::WIDTH*Position::HEIGHT / 2, 0);
			}

			unsigned long long getNodeCount()
			{
				return nodeCount;
			}

			// Constructor
			Solver() : nodeCount(0) {
				for (int i = 0; i < Position::WIDTH; i++)
					columnOrder[i] = Position::WIDTH / 2 + (1 - 2 * (i % 2))*(i + 1) / 2; // initialize the column exploration order, starting with center columns
			}

		};


	}
} // namespace GameSolver::Connect4