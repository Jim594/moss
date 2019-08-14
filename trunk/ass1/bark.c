#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
        int x;
        int y;
        int num;
        char suit;
    } Card;

void initialize_board(int height, int width, char*** board);
int load_saved_board(int height, int width, char*** board, FILE* deckFile);
void display_empty_board(int height, int width, char*** board);
void display_board(int height, int width, char*** board);
void exit_with_error(int errorNum);
int read_deck(FILE* deckFile, int cardAmount, char** deck);
int initialize_game(char** deck, Card* p1Cards, Card* p2Cards, int n);
void play_game(char*** board, char** deck, Card* p1Cards, Card* p2Cards, int n, int height, int width, char* p1Type, char* p2Type);
void display_cards(Card* cards, int v, char* type);
int first_move(int height, int width, Card* p1Cards, char*** board);
void auto_first_move(int height, int width,Card* p1Cards, char*** board, int v);
int move(int height, int width,int v, Card* p1Cards, Card* p2Cards, char*** board);
void delete_board(int height, int width_1, char*** board);
void free_board(int height, int width_1, char*** board);
void free_deck(char** deck, int cardAmount);


int main(int argc, char** argv){
    if ((argc!=4) && (argc!=6)){
        exit_with_error(1);
    }
    
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
        int height, width, n, v;
        char firstLine[81];
        char secondLine[81];
        char thirdLine[81];
        char fourthLine[81];
        fgets(firstLine,81,saveFile);
        sscanf(firstLine,"%d %d %d %d", &width, &height, &n, &v);
        if (height < 3 || height > 100){
            exit_with_error(2);
        }
        if (width < 3 || width > 100){
            exit_with_error(2);
        }
        if (v <1 || v > 2){
            exit_with_error(4);
        }
        char* deckName = calloc(81,sizeof(char));
        fgets(secondLine,81,saveFile);
        sscanf(secondLine,"%s",deckName);
        FILE* deckFile = fopen(deckName,"r");
        if (deckFile == NULL){
            free(deckName);
            exit_with_error(3);
        }
        char* N = calloc(81,sizeof(char));
        fgets(N,81,deckFile); 
        int cardAmount = atoi(N);
        if (cardAmount < 1){
            free(N);
            exit_with_error(3);
        }
        fgets(thirdLine,81,saveFile);
        fgets(fourthLine,81,saveFile);
        //char savedBoard[height][width][2];
        char*** board = calloc(height,sizeof(char**));
        for (int i = 0; i < height; ++i){
            board[i] = calloc(width,sizeof(char*));
            for (int j = 0; j < width; ++j){
                board[i][j]= calloc(2,sizeof(char));
            }
        }  
        int error;
        error = load_saved_board(height,width,board,saveFile);
        if (error != 0){
            free(N);
            free_board(height, width, board);
            exit_with_error(error);
        }        
        fclose(saveFile);
        display_board(height, width, board);
        char** deck = calloc(cardAmount,sizeof(char*));
        for (int i = 0; i < cardAmount; ++i){
        deck[i] = calloc(3,sizeof(char));
        }
        error = read_deck(deckFile,cardAmount,deck);
        fclose(deckFile);
        if (error != 0){
            free(N);
            free_deck(deck,cardAmount);
            exit_with_error(error);
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
        char* N = calloc(81,sizeof(char));
        fgets(N,81,deckFile); 
        int cardAmount = atoi(N);
        if (cardAmount < 1){
            free(N);
            exit_with_error(3);
        }
        char** deck = calloc(cardAmount,sizeof(char*));
        for (int i = 0; i < cardAmount; ++i){
        deck[i] = calloc(3,sizeof(char));
        }
        int error = read_deck(deckFile,cardAmount,deck);
        fclose(deckFile);
        if (error != 0){
            free(N);
            free_deck(deck,cardAmount);
            exit_with_error(error);
        }
        //int width_1 = width + 1; // the width must be 1 larger than width
        char*** board = calloc(height,sizeof(char**));
        for (int i = 0; i < height; ++i){
            board[i] = calloc(width,sizeof(char*));
            for (int j = 0; j < width; ++j){
                board[i][j]= calloc(2,sizeof(char));
            }
        }  
        initialize_board(height, width, board);
        Card* p1Cards = calloc(6,sizeof(Card));
        Card* p2Cards = calloc(6,sizeof(Card));
        int n = 0; //number of cards that have been drawn
        n = initialize_game(deck,p1Cards,p2Cards,n);
        display_board(height, width, board);
        play_game(board,deck,p1Cards,p2Cards,n,height,width,p1Type,p2Type);
        free(p1Cards);
        free(p2Cards);
        //delete_board(height, width, board);
        free_board(height, width, board);
    }
}

void initialize_board(int height, int width, char*** board){ //create and save the board 
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            /*if (i != height - 1 && j == width){
                board[i][j] = "\n";
            }else if(i == height - 1 && j == width){
                board[i][j] = "\0";
            }else{*/
            for (int k = 0; k < 2; k++){
                board[i][j][k] = '.';
            }
                //board[i][j] = ".."; 
        }
    }
}

int load_saved_board(int height, int width, char*** board, FILE* saveFile){
    int c;
    char savedBoard[height][width][2];
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            for (size_t k = 0; k < 2; k++){
                c = fgetc(saveFile);
                if (c == '\n'){
                    c = fgetc(saveFile);
                }
                if (c == '*'){
                    savedBoard[i][j][k] = '.';
                    putchar(savedBoard[i][j][k]);
                }else{
                    savedBoard[i][j][k] = c;
                    putchar(savedBoard[i][j][k]);
                }
            }
        }
        printf("\n");
    }
    c = fgetc(saveFile);
    if (c != '\n'&& c != EOF){
        return 4;
    }
    printf("\n");
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if (savedBoard[i][j][0] != '.'){
                board[i][j][0] = (int)(savedBoard[i][j][0]-'0');
                board[i][j][1] = savedBoard[i][j][1];
            }else{
                board[i][j][0] = savedBoard[i][j][0];
                board[i][j][1] = savedBoard[i][j][1];
            }
        }
    }
    return 0;
}

void display_empty_board(int height, int width, char*** board){ //display the board
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            //printf("%s",board[i][j]);
            for (int k = 0; k < 2; k++){
                printf("%c", board[i][j][k]);
            }
        }
        printf("\n");
    }
    //printf("\n");
}

void display_board(int height, int width, char*** board){
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if (board[i][j][0] == '.'){
                printf("%c",board[i][j][0]);
            }else{
                printf("%d",board[i][j][0]);
            }
            printf("%c",board[i][j][1]);
        }
        printf("\n");
    }
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
    c = fgetc(deckFile);
    if (c != EOF){ // check if the number of card is more than N
        return 3;
    }
    return 0;   
}

int initialize_game(char** deck, Card* p1Cards, Card* p2Cards, int n){
    for (int i = 0; i < 5; i++){
        int index = i + n;
        p1Cards[i].num = (int)(deck[index][0]-'0');
        p1Cards[i].suit = deck[index][1];
    }
    n = n + 5;
    for (int i = 0; i < 5; i++){
        int index = i + n;
        p2Cards[i].num = (int)(deck[index][0]-'0');
        p2Cards[i].suit = deck[index][1];
    }
    n = n + 5;
    return n;
}

void play_game(char*** board, char** deck, Card* p1Cards, Card* p2Cards, int n, int height, int width, char* p1Type, char* p2Type){
    int v = 1; // indicating which player should have their turn now 
    int error; 
    p1Cards[5].num = (int)(deck[n][0]-'0');
    p1Cards[5].suit = deck[n][1];
    n = n + 1;
    display_cards(p1Cards,v,p1Type);
    if (!strcmp(p1Type,"h")){
        error = first_move(height,width,p1Cards,board);
        if (error != 0){
            exit_with_error(error);
        }  
    }else{
        auto_first_move(height,width,p1Cards,board,v);
    }
    v = 2;
    display_board(height, width, board);
    p2Cards[5].num = (int)(deck[n][0]-'0');
    p2Cards[5].suit = deck[n][1];
    n = n + 1;
    display_cards(p2Cards,v,p2Type);
    error = move(height,width,v,p1Cards,p2Cards,board);
    if (error != 0){
        exit_with_error(error);
    } 
    v = 1;
    display_board(height, width, board);
}

void display_cards(Card* cards, int v, char* type){
    if (!strcmp(type,"h")){
        switch (v){
        case 1:
            printf("Hand(1): ");
            break;
        case 2:
            printf("Hand(2): ");
        default:
            ;
        }
        for (int i = 0; i < 6; i++){
            printf("%d%c",cards[i].num,cards[i].suit);
            if (i == 5){
                printf("\n");
            }else{
                printf(" ");
            }
        }  
    }else{
        printf("Hand: ");
        for (int i = 0; i < 6; i++){
            printf("%d%c",cards[i].num,cards[i].suit);
            if (i == 5){
                printf("\n");
            }else{
                printf(" ");
            }
        }  
    }
}

int first_move(int height, int width, Card* p1Cards, char*** board){
    int card,c,r;
    char str[81];
    char* i;
    int j = 0;
    while (j == 0){
        printf("Move? ");
        i = fgets(str,81,stdin);
        if (str[0] == '\n'){
            i = fgets(str,81,stdin);
        }
        if (i == NULL){
            return 7;
        }
        sscanf(str,"%d %d %d", &card, &c, &r);
        if(card <= 0 || card > 6){
            j = 0;
            continue;
        }
        if(c <= 0 || c > width){
            j = 0;
            continue;
        }
        if(r <= 0 || r > height){
            j = 0;
            continue;
        }
        board[r-1][c-1][0] = p1Cards[card-1].num;
        board[r-1][c-1][1] = p1Cards[card-1].suit;
        j = 1;
    }
    /*if(c <= width && c > 0){
        if (r <= height && r >0){
            board[r-1][c-1][0] = p1Cards[card-1].num;
            board[r-1][c-1][1] = p1Cards[card-1].suit;
        }
    }*/
    return 0;
}

void auto_first_move(int height, int width,Card* p1Cards, char*** board, int v){
    int x = (height + 1)/2;
    int y = (width + 1)/2;
    board[x-1][y-1][0] = p1Cards[0].num;
    board[x-1][y-1][1] = p1Cards[0].suit;
    printf("Player %d plays %d%c in column %d row %d\n",v,p1Cards[0].num,p1Cards[0].suit,x,y);
}

int move(int height, int width,int v, Card* p1Cards, Card* p2Cards, char*** board){
    int card,c,r;
    char str[81];
    char* i;
    int j = 0;
    int a = -1;
    while (j == 0){
        printf("Move? ");
        i = fgets(str,81,stdin);
        if (str[0] == '\n'){
            i = fgets(str,81,stdin);
        }
        if (i == NULL){
            return 7;
        }
        sscanf(str,"%d %d %d", &card, &c, &r);
        if(card <= 0 || card > 6){
            j = 0;
            continue;
        }
        if(c <= 0 || c > width){
            j = 0;
            continue;
        }
        if(r <= 0 || r > height){
            j = 0;
            continue;
        }
        if (board[r-1][c-1][0] != '.'){
            j = 0;
            continue;
        }
        int x = r+a;
        int y = c;
        if (x > height ){
            x = x - height;
        }
        if (x < 1){
            x = x + height;
        }
        if (board[x-1][y-1][0] == '.' && board[x-1][y-1][1] == '.'){
            j = 0;
            continue;
        }
        //board[r-1][c-1][0] = p1Cards[card-1].num;
        //board[r-1][c-1][1] = p1Cards[card-1].suit;
        j = 1;  
    }
    if (v == 1){
        board[r-1][c-1][0] = p1Cards[card-1].num;
        board[r-1][c-1][1] = p1Cards[card-1].suit;
    }
    if (v == 2){
        board[r-1][c-1][0] = p2Cards[card-1].num;
        board[r-1][c-1][1] = p2Cards[card-1].suit;
    }
    return 0;
}

void exit_with_error(int errorNum){
    switch (errorNum){
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
    case 7:
        fprintf(stderr, "End of input\n"); 
        exit(7);
        break;
    default:
        ;
    }
}

void delete_board(int height, int width, char*** board){
    for (int i = 0; i < height; ++i){
        for (int j = 0; j < width; ++j){
            for (int k = 0; i < 2; ++k){
                board[i][j][k] = '.';
            }
        }
    }
}

void free_board(int height, int width, char*** board){
    for (int i = 0; i < height; ++i){
         for (int j = 0; j < width; ++j){
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