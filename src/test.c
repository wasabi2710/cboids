#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {

    srand(time(NULL));
    //max
    int maxWidth = rand() / ((RAND_MAX + 1u) / 900);
    int maxHeight = rand() / ((RAND_MAX + 1u) / 400); 

    printf("width: %d, height: %d\n", maxWidth, maxHeight);

}

