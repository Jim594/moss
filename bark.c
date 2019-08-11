#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initialize_board(int width, int height, char*** board);
void display_board(int width, int height, char*** board);
void exit_with_error(int errorNum);
int read_deck(FILE* deckFile, int cardAmount, char** deck);
void free_board(int height, int width_1, char*** board);
void free_deck(char** deck, int cardAmount);

int main(int argc, char** argv){
    if ((argc!=4) && (argc!=6)){
        exit_with_error(1);
    }
    struct card{
        int num;
        char suit;
    };
    if (argc == 4){
        char* p1Type = argv[2];
        char* p2Type = argv[3];
        if (strcmp(p1Type,"h") && strcmp(p1Type,"a")){
            exit_with_error(2);
        }
        if (strcmp(p2Type,"h") && strcmp(p2Type,"a")){
            exit_with_error(2);
        }
        char* saveFileName = argv[1];
        FILE* saveFile = fopen(saveFileName,"r");
        if (saveFile == NULL){
            exit_with_error(4);
        }
    }
    if (argc == 6){
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
        if (height < 3 || height > 100){
            exit_with_error(2);
        }
        if (width < 3 || width > 100){
        exit_with_error(2);
        }
        char* deckName = argv[1]; // check deckfile format
        FILE* deckFile = fopen(deckName,"r");
        if (deckFile == NULL){ //can not open deckfile
            exit_with_error(3);
        }
        char* N = calloc(83,sizeof(char));
        fgets(N,83,deckFile); 
        int cardAmount = atoi(N);
        char** deck = calloc(cardAmount,sizeof(char*));
        for (int i = 0; i < cardAmount; ++i){
        deck[i] = calloc(3,sizeof(char));
        }
        int error = read_deck(deckFile,cardAmount,deck);
        if (error != 0){
            free(N);
            free_deck(deck,cardAmount);
            exit_with_error(error);
        }
    /* for (int i = 0; i < cardAmount; i++){
        for (int j = 0; j < 3; j++){ 
            putchar(deck[i][j]);  
        }  
    }*/
        int width_1 = width + 1; // the width must be 1 larger than width
        char*** board = calloc(height,sizeof(char**));
        for (int i = 0; i < height; ++i){
            board[i] = calloc(width_1,sizeof(char*));
            for (int j = 0; j < width_1; ++j){
                board[i][j]= calloc(2,sizeof(char));
            }
        }   
        initialize_board(width, height, board);
        display_board(width, height, board);
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
    printf("\n");
}

int read_deck(FILE* deckFile, int cardAmount, char** deck){
    if (cardAmount < 11){ // check how many card in the deckfile
        return 5;
    }
    int c;
    for (int i = 0; i < cardAmount; i++){
        for (int j = 0; j < 3; j++){
            c = fgetc(deckFile);
            if (c == EOF){
                return 3;  
            }else{
                deck[i][j] = c;
            }
        }  
    }
    // int d;
    c = fgetc(deckFile);
    if (c != EOF){ // check if the number of card is more than N
        return 3;
    }
    /* for (int i = 0; i < cardAmount; i++){
        for (int j = 0; j < 3; j++){ 
            putchar(deck[i][j]);  
        }  
    }*/
    return 0;   
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
    case 3:
        fprintf(stderr, "Unable to parse deckfile\n"); 
        exit(3);
        break;
    case 4:
        fprintf(stderr, "Unable to parse savefile\n"); 
        exit(4);
        break;
    case 5:
        fprintf(stderr, "Short deck\n"); 
        exit(5);
        break;
    default:
        ;
    }
}
void free_board(int height, int width_1, char*** board){
    for (int i = 0; i < height; ++i){
         for (int j = 0; j < width_1; ++j){
            free(board[i][j]);
         }
         free(board[i]);
    }
    free(board);
}

void free_deck(char** deck, int cardAmount){
    for (int i = 0; i < cardAmount; ++i){
        free(deck[i]);
    }
    free(deck);
}