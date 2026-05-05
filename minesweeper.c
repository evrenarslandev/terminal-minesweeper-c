#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define MAX_MOVES 100
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

void push(int move_rows[MAX_MOVES], int move_cols[MAX_MOVES], int row, int col, int *move_top)
{
    if (*move_top < MAX_MOVES)
    {
        move_rows[*move_top] = row;
        move_cols[*move_top] = col;
        (*move_top)++;
    }
}

void pop(int move_rows[MAX_MOVES], int move_cols[MAX_MOVES], int *move_top)
{
    if (*move_top > 0)
    {
        (*move_top)--;
    }
}

int isEmpty(int *move_top)
{
    return (*move_top) == 0;
}

void map(int **board, int *size)
{
    int i, j;
    FILE *map = fopen("map.txt", "w");
    fclose(map);
    map = fopen("map.txt", "a");
    for (i = 0; i < (*size); i++)
    {
        for (j = 0; j < (*size); j++)
        {
            if (board[i][j] == 9)
            {
                fprintf(map, "* ");
            }
            else
            {
                fprintf(map, ". ");
            }
        }
        fprintf(map, "\n");
    }
    fclose(map);
}

int **create_board(int *size)
{
    srand(time(NULL));
    *size = (rand() % 9) + 2;
    int i, j;
    int **board = (int **)malloc((*size) * sizeof(int *));
    for (i = 0; i < (*size); i++)
    {
        board[i] = (int *)malloc((*size) * sizeof(int));
    }
    int row, col, mines, x, y, counter;
    mines = (*size) * (*size) / 4;
    for (i = 0; i < mines; i++)
    {
        do
        {
            row = rand() % (*size);
            col = rand() % (*size);
        } 
        while (board[row][col] == 9);
        board[row][col] = 9;
    }
    for (i = 0; i < (*size); i++)
    {
        for (j = 0; j < (*size); j++)
        {
            counter = 0;
            for (x = -1; x <= 1; x++)
            {
                for (y = -1; y <= 1; y++)
                {
                    if (x == 0 && y == 0)
                    {
                        continue;
                    }
                    int newX = i + x;
                    int newY = j + y;
                    if (newX >= 0 && newX < (*size) && newY >= 0 && newY < (*size))
                    {
                        if (board[newX][newY] == 9)
                        {
                            counter++;
                        }
                    }
                }
            }
            if (board[i][j] != 9)
            {
                board[i][j] = counter;
            }
        }
    }
    map(board, size);
    return board;
}

char **ui_board(int *size)
{
    int i, j;
    char **board_ui = (char **)malloc((*size) * sizeof(char *));
    for (i = 0; i < (*size); i++)
    {
        board_ui[i] = (char *)malloc((*size) * sizeof(char));
    }
    for (i = 0; i < (*size); i++)
    {
        for (j = 0; j < (*size); j++)
        {
            board_ui[i][j] = '#';
        }
    }
    return board_ui;
}

void print_board(char **board, int *size, int *win_counter)
{
    int i, j;
    printf ("  ");
    for (i = 0; i < (*size); i++)
    {
        printf("%d ", i);
    }
    printf("\n");
    for (i = 0; i < (*size); i++)
    {
        printf("%d ", i);
        for (j = 0; j < (*size); j++)
        {
            if (board[i][j] == '#')
            {
                printf("%c ", board[i][j]);
            }
            else if (board[i][j] == 'X')
            {
                printf(RED"%c "RESET, board[i][j]);
            }
            else if (board[i][j] == '0')
            {
                printf(GREEN"%c "RESET, board[i][j]);
            }
            else
            {
                printf(YELLOW"%c "RESET, board[i][j]);
            }
        }
        printf("\n");
    }
}

void reveal_zeros(int **board, char **board_ui, int *size, int row, int col, int move_rows[MAX_MOVES], int move_cols[MAX_MOVES], int *move_top, int *counter, int *win_counter)
{
    if (row < 0 || row >= *size || col < 0 || col >= *size || board_ui[row][col] != '#')
    {
        return;
    }

    if (board[row][col] != 0) 
    {
        board_ui[row][col] = board[row][col] + '0';
        (*counter)++;
        (*win_counter)--;
        push(move_rows, move_cols, row, col, move_top);
        return;
    }

    board_ui[row][col] = '0';
    (*counter)++;
    (*win_counter)--;
    push(move_rows, move_cols, row, col, move_top);

    reveal_zeros(board, board_ui, size, row - 1, col - 1, move_rows, move_cols, move_top, counter, win_counter);
    reveal_zeros(board, board_ui, size, row - 1, col, move_rows, move_cols, move_top, counter, win_counter);
    reveal_zeros(board, board_ui, size, row - 1, col + 1, move_rows, move_cols, move_top, counter, win_counter); 
    reveal_zeros(board, board_ui, size, row, col - 1, move_rows, move_cols, move_top, counter, win_counter);
    reveal_zeros(board, board_ui, size, row, col + 1, move_rows, move_cols, move_top, counter, win_counter);
    reveal_zeros(board, board_ui, size, row + 1, col - 1, move_rows, move_cols, move_top, counter, win_counter);
    reveal_zeros(board, board_ui, size, row + 1, col, move_rows, move_cols, move_top, counter, win_counter);
    reveal_zeros(board, board_ui, size, row + 1, col + 1, move_rows, move_cols, move_top, counter, win_counter);
}

void move(int **board, char **board_ui, int *size, int row, int col, int move_rows[MAX_MOVES], int move_cols[MAX_MOVES], int *move_top, int *counter, int *win_counter)
{
    if (board[row][col] == 9)
    {
        board_ui[row][col] = 'X';
        print_board(board_ui, size, win_counter);
        printf(RED"BOOM! You hit a mine. Game Over.\n"RESET);
        exit(1);
    }
    else if (board_ui[row][col] != '#')
    {
        printf(YELLOW"You already open this place.\n"RESET);
    }
    else
    {
        if (board[row][col] == 0)
        {
            reveal_zeros(board, board_ui, size, row, col, move_rows, move_cols, move_top, counter, win_counter);
        }
        else
        {
            board_ui[row][col] = board[row][col] + '0';
            (*win_counter)--;
            (*counter)++;
            push(move_rows, move_cols, row, col, move_top);
        }
    }
}

void undoing(char **board_ui, int *counter, int move_rows[MAX_MOVES], int move_cols[MAX_MOVES], int *move_top, int *win_counter, int *move_counter)
{
    int i;
    if (isEmpty(move_top))
    {
        printf("There is no move to undo.\n");
    }
    else
    {
        for (i = 0; i < (*counter); i++)
        {
            (*win_counter)++;
            pop(move_rows, move_cols, move_top);
            board_ui[move_rows[*move_top]][move_cols[*move_top]] = '#';
        }
        (*move_counter)++;
        FILE *moves = fopen("moves.txt", "a");
        fprintf(moves, "Move %d: Undo\n", *move_counter);
        fclose(moves);
        printf(GREEN"Last move undone.\n"RESET);
    }
}

int main()
{
    FILE *moves = fopen("moves.txt", "w");
    fclose(moves);
    int move_rows[MAX_MOVES];
    int move_cols[MAX_MOVES];
    int move_top = 0;
    int size, i, counter = 0, move_counter = 0;
    char input[100];
    int a, b;
    int **board = create_board(&size);
    char **board_ui = ui_board(&size);
    int win_counter = size * size;
    while (1)
    {
        print_board(board_ui, &size, &win_counter);
        printf("Enter move(row col) or 'undo': ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "undo") == 0) 
        {
            undoing(board_ui, &counter, move_rows, move_cols, &move_top, &win_counter, &move_counter);
        }
        else 
        {
            if (sscanf(input, "%d %d", &a, &b) == 2) 
            {
                if (a >= size || b >= size)
                {
                    printf(YELLOW"Wrong input!\n"RESET);
                    continue;
                }
                counter = 0;
                move_counter++;
                move(board, board_ui, &size, a, b, move_rows, move_cols, &move_top, &counter, &win_counter);
                moves = fopen("moves.txt", "a");
                fprintf(moves, "Move %d: (Row: %d, Col: %d)\n", move_counter, a, b);
                fclose(moves);
            } 
            else 
            {
                printf(YELLOW"Wrong input!\n"RESET);
            }
        }
        if (win_counter == (size*size)/4)
        {
            print_board(board_ui, &size, &win_counter); 
            moves = fopen("moves.txt", "a");
            fprintf(moves, "Total move: %d\n", move_counter);
            fclose(moves);
            printf(GREEN"Welldone! You win!\n"RESET);
            break;
        }
    }

    for (i = 0; i < size; i++) 
    {
        free(board[i]);
    }
    free(board);
    for (i = 0; i < size; i++) 
    {
        free(board_ui[i]);
    }
    free(board_ui);
    return 0;
}