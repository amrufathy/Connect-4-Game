/*
Connect 4 project
Programming I
Fall 2014
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

int ROWS,COLS;

void gameLogic();
void PVP(char [][COLS],char [],int [],int );
void PVC(char [][COLS],char [],int [],int );
void printBoard(char [][COLS]);
int playTurnPVP(char [][COLS],int [],int ,char []);
int playTurnPVC(char [][COLS],int [],int ,char []);
int checkFour(char [][COLS],char ,int ,int ,int ,int ,int ,int ,int ,int );
int checkX(char [][COLS]);
int checkO(char [][COLS]);
void saveGame(char [][COLS],int );
void loadGame(char [][COLS], char [16]);
int startGameChecker(char [][COLS],int []);
int computerEasy();

int main()
{
    gameLogic();

    fflush(stdin);
    getchar();
    return 0;
}

void gameLogic(){
    printf("     ____   U  ___ u  _   _     _   _  U  _____  u   ____   _____      _  _\n"
           " U  /\"___|   \\/\"_ \\/ | \\ |\"|   | \\ |\"|  \\| ___\"|/ U /\"___| |_ \" _|    | ||\"|\n"
           "  \\| | u     | | | |<|  \\| |> <|  \\| |>  |  _|\"   \\| | u     | |      | || |_\n"
           "   | |/__.-,_| |_| |U| |\\  |u U| |\\  |u  | |___    | |/__   /| |\      |__   _|\n"
           "    \\____|\\_)-\\___/  |_| \\_|   |_| \\_|   |_____|    \\____| u |_|U       /|_|\\\n"
           "   _// \\\\      \\\\    ||   \\\\,-.||   \\\\,-.<<   >>   _// \\\\  _// \\\\_     u_|||_u\n"
           "  (__)(__)    (__)   (_\")  (_/ (_\")  (_/(__) (__) (__)(__)(__) (__)    (__)__)\n\n"

           "\t   (1) New Game\n"
           "\t   (2) Load Game\n"
           "\t   (3) Exit\n"
           "\t   >>>> "
          );
    char response = getch();
    system("CLS");
    switch(response){
        case '1':
            {
                printf("Please Enter number of rows: ");
                scanf("%d",&ROWS);
                printf("Please Enter number of columns: ");
                scanf("%d",&COLS);
                char board[ROWS][COLS],pieces[]= {'X','O'};
                int i,emptyFlag[COLS],mode;
                memset(board,' ', sizeof(board));
                for(i=0; i<COLS; i++)
                    emptyFlag[i] = ROWS;
                int filledCells = 0;
                printf("\nPlease select the mode:\n1) Player vs Player. \n2) Player vs Computer. \n");
                do
                    scanf("%d",&mode);
                while(!(mode==1 || mode==2));
                if(mode==1)
                    PVP(board,pieces,emptyFlag,filledCells);
                else
                    PVC(board,pieces,emptyFlag,filledCells);
                Sleep(3000);
                system("CLS");
                printf("\n\nScore(X): %d\tScore(O): %d\n",checkX(board),checkO(board));
                printf("\nGame Finished!\n\nPress any key to quit...\n");
                break;
            }
        case '2':
            {
                FILE *f;
                fflush(stdin);
                char fileName[16];
                printf("Enter name of file to load from:\n");
                gets(fileName);
                strcat(fileName,".txt");
                f = fopen(fileName, "r");
                if (filelength(f)!=0 && f!=NULL){
                    int mode;
                    fscanf(f,"%d %d %d",&ROWS,&COLS,&mode);
                    char board[ROWS][COLS],pieces[]= {'X','O'};
                    int i,j,emptyFlag[COLS];
                    memset(board,' ', sizeof(board));
                    for(i=0; i<COLS; i++)
                        emptyFlag[i] = ROWS;
                    loadGame(board, fileName);
                    int filledCells = startGameChecker(board,emptyFlag);
                    if(mode==1)
                        PVP(board,pieces,emptyFlag,filledCells);
                    else
                        PVC(board,pieces,emptyFlag,filledCells);
                    fclose(f);
                    Sleep(3000);
                    system("CLS");
                    printf("\n\nScore(X): %d\tScore(O): %d\n",checkX(board),checkO(board));
                    printf("\nGame Finished!\n\nPress any key to quit...\n");
                }
                break;
            }
        case '3': exit(0); break;
        default: printf("\n\tWrong Command!\n"); system("PAUSE");
    }

}
void PVP(char board[][COLS],char pieces[],int emptyFlag[],int availMoves){
    int turn;
    for(turn=0; turn<(ROWS*COLS-availMoves); turn++){
        printBoard(board);
        while(!playTurnPVP(board, emptyFlag, turn%2, pieces)){
            printBoard(board);
            printf("**Column full!**\n\n");
        }
    }
    printBoard(board);
}

void PVC(char board[][COLS],char pieces[],int emptyFlag[],int availMoves){
    int turn;
    for(turn=0; turn<(ROWS*COLS-availMoves); turn++){
        printBoard(board);
        while(!playTurnPVC(board, emptyFlag, turn%2, pieces)){
            printBoard(board);
            if(turn%2==0)
                printf("**Column full!**\n\n");
        }
    }
    printBoard(board);
}

void printBoard(char board[][COLS]){
    int i, j,count=0;
    system("CLS");
    printf("\n****Connect Four****\n\nYou can enter (0) to save game.\n\n");
    for(i=0; i<4*COLS+1; i++)
        printf("-");
    printf("\n");
    for(i=0; i<ROWS; i++){
        for(j=0; j<COLS; j++)
            printf("| %c ",  board[i][j]);
        printf("|\n");
        for(j=0; j<4*COLS+1; j++)
            printf("-");
        printf("\n");
    }
    for(i=0; i<COLS; i++)
        printf("%*d",4,i+1);
    printf("\n\nScore(X): %d\tScore(O): %d\n\n",checkX(board),checkO(board));
}

int playTurnPVP(char board[][COLS],int emptyFlag[],int player,char pieces[]){
    int column,row;
    printf("Player %d (%c), Enter number of column <%d-%d>: ",player+1,pieces[player],1,COLS);
    do{
        scanf("%d",&column);
        if (column == 0)
            saveGame(board,1);
    }while(column<1 || column>COLS);
    column--;
    if(emptyFlag[column] > 0){
        emptyFlag[column]--;
        row = emptyFlag[column];
        board[row][column] = pieces[player];
        return 1;
    }
    return 0;
}

int playTurnPVC(char board[][COLS],int emptyFlag[],int player,char pieces[]){
    int column,row;
    if(player == 0){
        printf("Player 1 (X), Enter number of column <%d-%d>: ",1,COLS);
        do{
            scanf("%d",&column);
            if (column == 0)
                saveGame(board,2);
        }while(column<1 || column>COLS);
    }
    else
        column = computerEasy();

    column--;
    if(emptyFlag[column] > 0){
        emptyFlag[column]--;
        row = emptyFlag[column];
        board[row][column] = pieces[player];
        return 1;
    }
    return 0;
}

int checkFour(char board[][COLS],char c,int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4){
    return (board[x1][y1] == c && board[x1][y1] == board[x2][y2] && board[x2][y2] == board[x3][y3] && board[x3][y3] == board[x4][y4]);
}

int checkX(char board[][COLS]){
    int i,j,count=0;
    // RIGHT DIAGONAL CHECK
    for(i=0; i<ROWS-3; i++)
        for(j=0; j<COLS-3; j++)
            if(checkFour(board,'X',i,j,i+1,j+1,i+2,j+2,i+3,j+3))
                count++;
    // LEFT DIAGONAL CHECK
    for(i=0; i<ROWS-3; i++)
        for(j=COLS; j>2; j--)
            if(checkFour(board,'X',i,j,i+1,j-1,i+2,j-2,i+3,j-3))
                count++;
    // VERTICAL CHECK
    for(i=0; i<ROWS-3; i++)
        for(j=0; j<COLS; j++)
            if(checkFour(board,'X',i,j,i+1,j,i+2,j,i+3,j))
                count++;
    // HORIZONTAL CHECK
    for(i=0; i<ROWS; i++)
        for(j=0; j<COLS-3; j++)
            if(checkFour(board,'X',i,j,i,j+1,i,j+2,i,j+3))
                count++;
    return count;
}

int checkO(char board[][COLS]){
    int i,j,count=0;
    // RIGHT DIAGONAL CHECK
    for(i=0; i<ROWS-3; i++)
        for(j=0; j<COLS-3; j++)
            if(checkFour(board,'O',i,j,i+1,j+1,i+2,j+2,i+3,j+3))
                count++;
    // LEFT DIAGONAL CHECK
    for(i=0; i<ROWS-3; i++)
        for(j=COLS; j>3; j--)
            if(checkFour(board,'O',i,j,i+1,j-1,i+2,j-2,i+3,j-3))
                count++;
    // VERTICAL CHECK
    for(i=0; i<ROWS-3; i++)
        for(j=0; j<COLS; j++)
            if(checkFour(board,'O',i,j,i+1,j,i+2,j,i+3,j))
                count++;
    // HORIZONTAL CHECK
    for(i=0; i<ROWS; i++)
        for(j=0; j<COLS-3; j++)
            if(checkFour(board,'O',i,j,i,j+1,i,j+2,i,j+3))
                count++;
    return count;
}

void saveGame(char board[][COLS],int mode){
    int i,j;
    fflush(stdin);
    char fileName[16];
    printf("Enter name of file to save in:\n");
    gets(fileName);
    strcat(fileName,".txt");
    FILE *f;
    f = fopen(fileName, "w");
    fprintf(f,"%d %d %d\n",ROWS,COLS,mode);
    for(i=0;i<ROWS;i++){
        for(j=0;j<COLS;j++)
            fprintf(f,"%c",board[i][j]);
        fprintf(f,"\n");
    }
    fclose(f);
    printf("Board saved.\n");
}

void loadGame(char board[][COLS], char fileName[16]){
    int i,j,g3;
    fflush(stdin);
    if(fileName == NULL){
        char fileName[16];
        printf("Enter name of file to load from:\n");
        gets(fileName);
        strcat(fileName,".txt");
    }
    FILE *f;
    f = fopen(fileName, "r");
    fscanf(f,"%d %d %d",&ROWS,&COLS,&g3);
    for(i=0;i<ROWS;i++)
        for(j=0;j<COLS;j++){
            fscanf(f,"%c",&board[i][j]);
            if(board[i][j]=='\n')
                j--;
        }

    printBoard(board);
    fclose(f);
}

int startGameChecker(char board[][COLS],int emptyFlag[]){
    int i,j,count=0;
    for(i=0;i<ROWS;i++)
        for(j=0;j<COLS;j++)
            if(board[i][j]=='X' || board[i][j]=='O'){
                emptyFlag[j]--;
                count++;
            }
    return count;
}

int computerEasy(){
    srand(time(0));
    printf("Computer's turn...");
    Sleep(500);
    return 1+(rand()%COLS);
}
