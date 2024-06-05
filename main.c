#include <stdio.h>
#include <stdlib.h>

/*** Stealth Game ***/
char player;

int main(int argc, char **argv)
{
    system("cls");

    printf("Choose your character (*smash bros theme in the background*): ");
    player = getchar(); // Let player choose their own character

    printf("this u -> %c\n", player);

    return 0;
}