#include <iostream>
#include <cstdlib>
#include <vector>
#include <time.h>

using namespace std;

const int BOARD_WIDTH = 7;
const int BOARD_HEIGHT = 6;
const int CON_NUM_TO_WIN = 4;

enum Color
{
    EMPTY,
    BLACK,
    WHITE
};

typedef vector<vector<Color> > BOARD;

void dump_board( Color board[BOARD_HEIGHT][BOARD_WIDTH] )
{
    for ( int i = 0; i < BOARD_HEIGHT; ++i )
    {
        for ( int j = 0; j < BOARD_WIDTH; ++j )
        {
            switch ( board[BOARD_HEIGHT - i - 1][j] )
            {
                case EMPTY: cout << "-"; break;
                case BLACK: cout << "x"; break;
                case WHITE: cout << "o"; break;
            }
        }
        cout << endl;
    }
    cout << "0123456" << endl;
}

Color getLineCheck( Color board[BOARD_HEIGHT][BOARD_WIDTH], int y0, int x0, int dy, int dx )
{
    int con_num = 1;
    Color prev_grid = board[y0][x0];
    int r = y0;
    int c = x0;
    while ( true )
    {
        r += dy;
        c += dx;
        if ( r >= BOARD_HEIGHT || c >= BOARD_WIDTH || r < 0 || c < 0 )
        {
            break;
        }
        Color grid = board[r][c];
        if ( grid == prev_grid )
        {
            con_num++;
        }
        else
        {
            con_num = 1;
            prev_grid = grid;
        }
        if ( con_num == CON_NUM_TO_WIN && grid != EMPTY )
        {
            return grid;
        }
    }

    return EMPTY;
}

Color getWinner( Color board[BOARD_HEIGHT][BOARD_WIDTH] )
{
    // check horizontal
    for ( int y = 0; y < BOARD_HEIGHT; ++y )
    {
        Color c = getLineCheck( board, y, 0, 0, 1 );
        if ( c != EMPTY ) return c;
    }

    // check vertical
    for ( int x = 0; x < BOARD_WIDTH; ++x )
    {
        Color c = getLineCheck( board, 0, x, 1, 0 );
        if ( c != EMPTY ) return c;
    }

    // check slash
    for ( int y = 0; y < BOARD_HEIGHT; ++y )
    {
        Color c = getLineCheck( board, y, 0, -1, 1 );
        if ( c != EMPTY ) return c;
    }
    for ( int x = 1; x < BOARD_WIDTH; ++x )
    {
        Color c = getLineCheck( board, BOARD_HEIGHT - 1, x, -1, 1 );
        if ( c != EMPTY ) return c;
    }

    // check backslash
    for ( int y = 0; y < BOARD_HEIGHT; ++y )
    {
        Color c = getLineCheck( board, y, 0, 1, 1 );
        if ( c != EMPTY ) return c;
    }
    for ( int x = 1; x < BOARD_WIDTH; ++x )
    {
        Color c = getLineCheck( board, 0, x, 1, 1 );
        if ( c != EMPTY ) return c;
    }

    return EMPTY;
}

/*
Color getWinner( Color board[BOARD_HEIGHT][BOARD_WIDTH] )
{
    const int Dirc = 4;
    int dy[Dirc] = { 1,1,-1,0 };
    int dx[Dirc] = { 0,1, 1,1 };
    for ( int k = 0; k < Dirc; ++k )
    {
        for ( int y = 0; y < BOARD_HEIGHT; ++y )
        {
            for ( int x = 0; x < BOARD_WIDTH; ++x )
            {
                int con_num = 1;
                Color prev_grid = board[y][x];
                int r = y;
                int c = x;
                while ( true )
                {
                    r += dy[k];
                    c += dx[k];
                    if ( r >= BOARD_HEIGHT || c >= BOARD_WIDTH || r < 0 || c < 0 )
                    {
                        break;
                    }
                    Color grid = board[r][c];
                    if ( grid == prev_grid )
                    {
                        con_num++;
                    }
                    else
                    {
                        con_num = 1;
                        prev_grid = grid;
                    }
                    if ( con_num == CON_NUM_TO_WIN && grid != EMPTY )
                    {
                        return grid;
                    }
                }
            }
        }
    }

    return EMPTY;
}
*/

bool put( Color board[BOARD_HEIGHT][BOARD_WIDTH], int x, Color color )
{
    for ( int y = 0; y < BOARD_HEIGHT; ++y )
    {
        if ( board[y][x] == EMPTY )
        {
            board[y][x] = color;
            return true;
        }
    }

    return false;
}

bool pop( Color board[BOARD_HEIGHT][BOARD_WIDTH], int x )
{
    for ( int y = BOARD_HEIGHT - 1; y >= 0; --y )
    {
        if ( board[y][x] != EMPTY )
        {
            board[y][x] = EMPTY;
            return true;
        }
    }

    return false;
}

int getGoodX( Color board[BOARD_HEIGHT][BOARD_WIDTH], Color color )
{
    Color self_color = color;
    Color oppo_color = (Color)( BLACK + WHITE - color );

    for ( int x = 0; x < BOARD_WIDTH; ++x )
    {
        bool put_ok = put( board, x, self_color );
        bool is_win = ( getWinner( board ) == self_color );
        if ( put_ok )
            pop( board, x );
        if ( is_win )
        {
            // cout << "good " << x << endl;
            return x;
        }
    }

    for ( int x = 0; x < BOARD_WIDTH; ++x )
    {
        bool put_ok = put( board, x, oppo_color );
        bool is_lose = ( getWinner( board ) == oppo_color );
        if ( put_ok )
            pop( board, x );
        if ( is_lose )
        {
            // cout << "good " << x << endl;
            return x;
        }
    }

    return -1;
}

int ai( Color board[BOARD_HEIGHT][BOARD_WIDTH] )
{
    const int AI_STRONG = 3000;
    Color temp_board[BOARD_HEIGHT][BOARD_WIDTH];

    int x_win_count[BOARD_WIDTH] = { 0 };
    for ( int x = 0; x < BOARD_WIDTH; ++x )
    {
        if ( board[BOARD_HEIGHT - 1][x] != EMPTY ) continue;
        for ( int try_idx = 0; try_idx < AI_STRONG; try_idx++ )
        {
            // init board and chess_count
            int chess_count = 0;
            for ( int i = 0; i < BOARD_HEIGHT; ++i )
            {
                for ( int j = 0; j < BOARD_WIDTH; ++j )
                {
                    temp_board[i][j] = board[i][j];
                    if ( temp_board[i][j] != EMPTY )
                    {
                        chess_count++;
                    }
                }
            }

            // put 1st at x
            if ( !put( temp_board, x, BLACK ) )
            {
                cout << "fatal error!" << endl;
                break;
            }
            Color winner = getWinner( temp_board );
            if ( winner == BLACK )
            {
                x_win_count[x]++;
                continue;
            }
            chess_count++;
            if ( chess_count == BOARD_HEIGHT * BOARD_WIDTH )
            {
                // cout << "1" << chess_count << endl;
                // dump_board( temp_board );
                continue;
            }

            // monte carlo
            while ( true )
            {
                // WHITE
                {
                    int put_WHITE_x = -1;
                    int good_WHITE_x = getGoodX( temp_board, WHITE );
                    // cout << good_WHITE_x << endl;
                    if ( good_WHITE_x != -1 )
                    {
                        put_WHITE_x = good_WHITE_x;
                        put( temp_board, put_WHITE_x, WHITE );
                    }
                    else
                    {
                        while ( true )
                        {
                            put_WHITE_x = rand() % BOARD_WIDTH;
                            if ( put( temp_board, put_WHITE_x, WHITE ) )
                            {
                                break;
                            }
                        }
                    }
                    chess_count++;
                    if ( chess_count == BOARD_HEIGHT * BOARD_WIDTH )
                    {
                        // cout << "2" << chess_count << endl;
                        // dump_board( temp_board );
                        break;
                    }
                    Color winner = getWinner( temp_board );
                    if ( winner == WHITE )
                    {
                        // x_win_count[x]--;
                        //dump_board( temp_board );
                        break;
                    }
                }

                // BLACK
                {
                    int put_BLACK_x = -1;
                    int good_BLACK_x = getGoodX( temp_board, BLACK );
                    if ( good_BLACK_x != -1 )
                    {
                        put_BLACK_x = good_BLACK_x;
                        put( temp_board, put_BLACK_x, BLACK );
                    }
                    else
                    {
                        while ( true )
                        {
                            put_BLACK_x = rand() % BOARD_WIDTH;
                            if ( put( temp_board, put_BLACK_x, BLACK ) )
                            {
                                break;
                            }
                        }
                    }
                    chess_count++;
                    if ( chess_count == BOARD_HEIGHT * BOARD_WIDTH )
                    {
                        // cout << "3" << chess_count << endl;
                        // dump_board( temp_board );
                        break;
                    }
                    Color winner = getWinner( temp_board );
                    if ( winner == BLACK )
                    {
                        x_win_count[x]++;
                        break;
                    }
                }
            }
        }
    }

    int max_i = 0;
    int max_v = x_win_count[0];
    for ( int i = 0; i < BOARD_WIDTH; ++i )
    {
        cout << x_win_count[i] << endl;
        if ( max_v < x_win_count[i] )
        {
            max_v = x_win_count[i];
            max_i = i;
        }
    }

    return max_i;
}

int main()
{
    srand (time(NULL));

    Color board[BOARD_HEIGHT][BOARD_WIDTH];
    for ( int i = 0; i < BOARD_HEIGHT; ++i )
        for ( int j = 0; j < BOARD_WIDTH; ++j )
            board[i][j] = EMPTY;

    /*
    board[3][1] = BLACK;
    board[3][2] = BLACK;
    board[3][3] = BLACK;
    board[3][4] = BLACK;
    */

    int you_first = 0;
    cout << "1:you first" << endl << "2:computer first" << endl;
    cin >> you_first;

    if ( you_first == 1 )
    {
        int x;
        dump_board( board );
        while ( true )
        {
            cout << "put 0-7" << endl;
            cin >> x;
            if ( put( board, x, WHITE ) )
            {
                dump_board( board );
                break;
            }
        }
    }
    while ( true )
    {
        put( board, ai( board ), BLACK );
        Color winner = getWinner( board );
        dump_board( board );
        if ( winner != EMPTY )
        {
            cout << ( winner == WHITE ? "you win!\n" : "comp win!\n" );
            return 0;
        }
        while ( true )
        {
            int x;
            cout << "put 0-7" << endl;
            cin >> x;
            if ( put( board, x, WHITE ) )
            {
                dump_board( board );
                break;
            }
        }
        winner = getWinner( board );
        if ( winner != EMPTY )
        {
            cout << ( winner == WHITE ? "you win!\n" : "comp win!\n" );
            return 0;
        }
    }

    // cout << getWinner( board );
    // cout << ai( board );

    return 0;
}
