#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


bool drukowalne(const void * buf, int len){
    int i;
    const char* char_buf = buf;
    for(i=0; i<len; i++){
        if(char_buf[i]<32 || char_buf[i]>126){
            return false;
        }
    }
    return true;
}

int main(){
    
    char buf_false[] = {10, 36, 55, -73};
    char buf_true[] = {75, 36, 55, 115};
    printf("%s\n", drukowalne(buf_false, 4) ? "true" : "false");
    printf("%s\n", drukowalne(buf_true, 4) ? "true" : "false");

    return 0;
}