BOARD_SIZE - a number from the range <1;11> indicating the size of the board.
PAWNS_NUMBER - a number from the range <0;121> indicating the number of pawns of both players on the board.
IS_BOARD_CORRECT - a YES/NO response indicating whether the state of the board is correct, in other words, whether the number of pawns of one player is appropriate relative to the number of pawns of the other player.
IS_GAME_OVER - a YES RED/YES BLUE/NO response indicating whether (and if so, which) player has ended the game, i.e., whether they have connected two edges of the board with an uninterrupted path of their color.
IS_BOARD_POSSIBLE - a YES/NO response indicating whether the state of the board is possible. This means that the board is correct and that achieving such a state did not result in overlooking an earlier win by one of the players.
CAN_RED_WIN_IN_N_MOVE_WITH_NAIVE_OPPONENT (...) - a YES/NO response indicating whether one of the players can end the game in N (where N belongs to <1;2>) moves when playing against a naive opponent (making the worst possible moves for themselves). The number of moves N indicates the number of moves by the player for whom we are considering victory. Victory must occur in the Nth move, which means that the player must have the ability to make such a number of moves.
For example, if we are checking whether the red player can win in N = 2 moves and the current move belongs to the blue player, both players will make two moves. In the case where the current move belongs to the red player, the red player will make two moves and the blue player only one. 


Example Input:
          ---
       --< b >--
    --< r >-< b >--
 --< r >-< r >-< b >--
< b >-< b >-< r >-< b >
 --< r >-< b >-< b >--
    --< r >-< r >--
       --< r >--
          ---
BOARD_SIZE

PAWNS_NUMBER

IS_BOARD_CORRECT

IS_GAME_OVER

IS_BOARD_POSSIBLE

CAN_RED_WIN_IN_1_MOVE_WITH_NAIVE_OPPONENT
CAN_BLUE_WIN_IN_1_MOVE_WITH_NAIVE_OPPONENT
CAN_RED_WIN_IN_2_MOVES_WITH_NAIVE_OPPONENT
CAN_BLUE_WIN_IN_2_MOVES_WITH_NAIVE_OPPONENT
