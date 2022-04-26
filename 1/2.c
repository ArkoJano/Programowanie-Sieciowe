#include <stdio.h>
#include <stdlib.h>

int main(){
    int* liczby;
    int i, tmp, count=0;

    liczby = calloc(50, sizeof(int));

    for(i=0; i<50; i++){
        printf("Liczba: ");
        scanf("%d", &tmp);
        if(tmp == 0){
            break;
        } else {
            *(liczby+i) = tmp;
            count++;
        }
        printf("Dodano %d do tablicy, counter: %d\n", liczby[i], count);

    }
    printf("Liczby: ");

    for(i=0; i<=count; i++){
        if(*(liczby+i)>10 && *(liczby+i)<100)
            printf("%d ", *(liczby+i) );
    }
    free(liczby);
    printf("\n");
    return 0;

}