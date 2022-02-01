#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ARRMAX 10
void hasVulgarRec(char* text, const char** vulgar, int index){ //Recursive, works everytime
    if(index == 9){
        return;
    }
    char* vlg = strstr(text,vulgar[index]);
    int tmp = 0;
    if(vlg){
        for(unsigned long l = vlg-text;l<vlg-text + strlen(vulgar[index]);l++){
               text[l] = '*';
               tmp = l+1;
           }
           while(text[tmp] != ' ' && text[tmp] != '\0'){
               text[tmp] = '*';
               tmp++;
           }
           if(vlg-text > 0){
               tmp = vlg-text-1;
                while(tmp >= 0 && text[tmp] != ' ' ){
                    text[tmp] = '*';
                    tmp--;
                }
           }
           hasVulgarRec(text,vulgar,index+1);
    }else{
        hasVulgarRec(text,vulgar,index+1);
    }

}
int hasVulgar(char* text, const char** vulgar){    //Non-recursive, does not work everytime
    int cnt = 0;
    int tmp = 0;
    for(int i = 0;i < ARRMAX;i++){
       char* vlg = strstr(text,vulgar[i]);
       if(vlg){
           for(unsigned long l = vlg-text;l<vlg-text + strlen(vulgar[i]);l++){
               text[l] = '*';
               tmp = l+1;
           }
           while(text[tmp] != ' ' && text[tmp] != '\0'){
               text[tmp] = '*';
               tmp++;
           }
           cnt++;
       }
    }

    return cnt;
}
int main() {
    const char* vulgar[] = {"fuck","shit","bitch","dick","cock","cunt","piss","pussy","nigger","ass"};
    char* buffer = NULL;
    size_t size = 0;
    size_t chars = 0;
    while(true){
        buffer = NULL;
        printf("Napište větu:\n");
        chars = getline(&buffer,&size,stdin);
        if(chars == EOF){
            break;
        }
        buffer[chars-1] = '\0';
        hasVulgarRec(buffer,vulgar,0);
        printf("%s\n",buffer);
    }
    free(buffer);
}