#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initialize_board(int width, int height, char*** board);
void display_board(int width, int height, char*** board);
void exit_with_error(int errorNum);

int main(int argc, char** argv){
    /* if (argc != 4 || argc !=6){
        exit_with_error(1);
    }*/
    if (argc == 4){
       char* p1Type = argv[2];
       char* p2Type = argv[3];
       if (strcmp(p1Type,"h") && strcmp(p1Type,"a")){
           exit_with_error(2);
       }
       if (strcmp(p2Type,"h") && strcmp(p2Type,"a")){
           exit_with_error(2);
       }
    }

    else if (argc == 6){
    char* p1Type = argv[4];
    char* p2Type = argv[5];
    if (strcmp(p1Type,"h") && strcmp(p1Type,"a")){
           exit_with_error(2);
       }
    if (strcmp(p2Type,"h") && strcmp(p2Type,"a")){
           exit_with_error(2);
    }
    int height = atoi(argv[3]);
    int width = atoi(argv[2]);
    int width_1 = width + 1; // the width must be 1 larger than width
    char*** board = calloc(height,sizeof(char**));
    for (int i = 0; i < height; ++i){
        board[i] = calloc(width_1,sizeof(char*));
         for (int j = 0; j < width_1; j++){
            board[i][j]= calloc(2,sizeof(char));
        }
    }
    initialize_board(width, height, board);
    display_board(width, height, board); 
    printf("\n");
    }else{
        exit_with_error(1);
    }
}

void initialize_board(int width, int height, char*** board){ //create and save the board 
    for (int i = 0; i < height; i++){
        for (int j = 0; j <= width; j++){
            if (i != height - 1 && j == width){
                board[i][j] = "\n";
            }else if(i == height - 1 && j == width){
                board[i][j] = "\0";
            }else{
                board[i][j] = ".."; 
            }
            //board[i][j] = "..";
        }
    }
}

void display_board(int width, int height, char*** board){ //display the board
    for (int i = 0; i < height; i++){
        for (int j = 0; j <= width; j++){
        printf("%s",board[i][j]);
        }
    }
}

void exit_with_error(int errorNum){
    switch (errorNum)
    {
    case 1:
        fprintf(stderr, "Usage: bark savefile p1type p2type\n"); 
        fprintf(stderr,"bark deck width height p1type p2type\n");
        exit(1);
        break;
    case 2:
        fprintf(stderr, "Incorrect arg types\n"); 
        exit(2);
        break;
    default:
        exit(0);
    }
}