#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ARRMAX 9
typedef struct task{
    char* text;
    int order;
} TASK;
int readNR(){ //Reads and returns the number of the command the user wants to execute
    int nbr = 0; 
    char* buffer = NULL;
    size_t size = 0;
    printf("Type 1 to add a task.\nType 2 to remove a task.\nType 3 to list the tasks.\n");
    printf("Type 4 to sort the tasks alphabetically.\nType 5 to sort the tasks via the order they came in.\n");
    printf("Type anything else to terminate the program.\n");
    getline(&buffer,&size,stdin);
    if(sscanf(buffer,"%d",&nbr) != 1 || nbr < 1 || nbr > 5){ //Input validation, the number has to be between 1 and 5
        free(buffer);
        return 0;
    }
    free(buffer);
    return nbr; 
}
int isInTasks(TASK* tasks, char* buffer, int max){ //Goes throught the array of tasks, returns 1 if the task is already there
    for(int i = 0;i < max; i++){
        if(strcmp(buffer,tasks[i].text) == 0){
            return 1;
        }
    }
    return 0;
}
void hasVulgarRec(char* text, const char** vulgar, int index){ //Recursively censors all the words containing vulgarities
    if(index == ARRMAX){
        return;
    }
    char* vlg = strstr(text,vulgar[index]); //I get pointer to a vulgar word in the text, if it is there
    int tmp = 0;
    if(vlg){
        for(unsigned long l = vlg-text;l<vlg-text + strlen(vulgar[index]);l++){ //I censor the vulgar part in the text   f.e. motherfucker -> mother****er
               text[l] = '*';
               tmp = l+1;
           }
           while(text[tmp] != ' ' && text[tmp] != '\0'){ //I censor the rest of the vulgar word (to the right)    f.e. mother****er -> mother******
               text[tmp] = '*';
               tmp++;
           }
           if(vlg-text > 0){
               tmp = vlg-text-1;
               while(tmp >= 0 && text[tmp] != ' ' ){  //I censor the rest of the vulgar word(to the left)    f.e mother****** -> ************
                    text[tmp] = '*';
                    tmp--;
                }
           }
           hasVulgarRec(text,vulgar,index+1);
    }else{
        hasVulgarRec(text,vulgar,index+1);
    }
}
void addTask(TASK** tasks,int* constanta,int* index){ //This function censors(if necessary),checks if duplicate and adds the task to my array
    const char* vulgar[] = {"fuck","shit","bitch","dick","cock","cunt","piss","pussy","ass"};
    char* buffer = NULL;
    size_t size = 0;
    size_t chars = 0;
    chars = getline(&buffer,&size,stdin);
    buffer[chars-1] = '\0';
    hasVulgarRec(buffer,vulgar,0);
    if(isInTasks(*tasks,buffer,*index)){
        printf("Already on the list.\n");
        free(buffer);
        return;
    }
    char* newText = strdup(buffer); //Create an independent string
    TASK newTask = {newText,*index+1};
    (*tasks)[*index] = newTask;
    (*index)++;
    if(*index >= *constanta){ //Make the array bigger when necessary
        (*constanta)*=2;
        *tasks = (TASK*) realloc (*tasks,(*constanta)*sizeof(TASK));
    }
    free(buffer);

}
int main() {
    int nr = 0; 
    int index = 0;
    TASK* tasks = (TASK*) malloc (sizeof(TASK)*10);
    int constanta = 10;
    printf("Welcome to the task list sorter.\n");
    while(true){
        nr = readNR();
        if(!(nr)){
            return 0;
        }
        switch(nr){
            case 1:
                addTask(&tasks,&constanta,&index);
                break;
        }
        
    }
}
