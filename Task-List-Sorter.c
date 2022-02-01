#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ARRMAX 9
typedef struct task{
    char* text;
    int order;
} TASK;
void deleteTasks(TASK* tasks,int index){
    for(int i = 0; i < index; i++){
        free(tasks[i].text);
    }
    free(tasks);
}
int readNR(){ //Reads and returns the number of the command the user wants to execute
    int nbr = 0; 
    char* buffer = NULL;
    size_t size = 0;
    printf("Type 1 to add a task.\nType 2 to remove a task.\nType 3 to list the tasks.\n");
    printf("Type 4 to sort and list the tasks alphabetically.\nType 5 to sort and list the tasks against the order they came in.\n");
    printf("Type anything else to terminate the program.\n");
    getline(&buffer,&size,stdin);
    if(sscanf(buffer,"%d",&nbr) != 1 || nbr < 1 || nbr > 5){ //Input validation, the number has to be between 1 and 5
        free(buffer);
        return 0;
    }
    free(buffer);
    return nbr; 
}
int isInTasks(TASK* tasks, char* buffer, int max, int* indexOfDup){ //Goes throught the array of tasks, returns 1 if the task is already there
    for(int i = 0;i < max; i++){
        if(strcmp(buffer,tasks[i].text) == 0){
            *indexOfDup = i;
            return 1;
        }
    }
    return 0;
}
void hasVulgarRec(char* text, const char** vulgar, int index){ //Recursively censors all the words containing vulgarities
    if(index == ARRMAX){
        return;
    }
    char* vlg = strcasestr(text,vulgar[index]); //I get pointer to a vulgar word in the text, if it is there
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
           hasVulgarRec(text,vulgar,index);
    }else{
        hasVulgarRec(text,vulgar,index+1);
    }
}
char* readTask(TASK* tasks,int index,int nr,int* indexOfDup){
    const char* vulgar[] = {"fuck","shit","bitch","dick","cock","cunt","piss","pussy","ass"};
    char* buffer = NULL;
    size_t size = 0;
    size_t chars = 0;
    int indexOfDuplicate = -1;
    chars = getline(&buffer,&size,stdin);
    buffer[chars-1] = '\0';
    hasVulgarRec(buffer,vulgar,0);
    if(isInTasks(tasks,buffer,index,&indexOfDuplicate)){
        if(nr == 2){ //If we want to remove a task, we want to know the index of the task to be removed
            *indexOfDup = indexOfDuplicate;
            free(buffer);
            return NULL;
        }
        printf("Already on the list.\n");
        free(buffer);
        return NULL;
    }
    char* newText = strdup(buffer); //Create an independent string
    free(buffer);
    return newText;
}
void addTask(TASK** tasks,int* constanta,int* index,int* order){ //This function censors(if necessary),checks if duplicate and adds the task to my array
    char* newText = readTask(*tasks,*index,1,NULL);
    if(newText == NULL){
        return;
    }
    TASK newTask = {newText,*order};
    (*order)++;
    (*tasks)[*index] = newTask;
    (*index)++;
    if(*index >= *constanta){ //Make the array bigger when necessary
        (*constanta)*=2;
        *tasks = (TASK*) realloc (*tasks,(*constanta)*sizeof(TASK));
    }

}
void listTasks(TASK* tasks, int index){
    for(int i = 0;i < index;i++){
            printf("%s\n",tasks[i].text);
        }
}
void removeTask(TASK** tasks,int* index){
    int indexOfDup = -1;
    readTask(*tasks,*index,2,&indexOfDup);
    if(indexOfDup == -1){ //If the task is not in the array
        printf("This task is not on the list.\n");
        return;
    }
    for(int i = indexOfDup;i < *index;i++){
        if(i + 1 == *index){ //The last task in the array will be there twice and will be waiting to be overwritten by a new one
            break;
        }
        if(i == indexOfDup){ //Free the text of the removed task
            free((*tasks)[i].text);
        }
        (*tasks)[i] = (*tasks)[i+1]; //Move all the tasks to the left in the array
    }
    (*index)--; 
}
void copyTasks(TASK* orig,TASK* newT,int index){ //Function that copies and array into a new one
    for(int i = 0;i < index;i++){
        char* copyText = strdup(orig[i].text);
        TASK copyTask = {copyText,orig[i].order};
        newT[i]  = copyTask;
    }
}
int cmpAlp(const void* a,const void* b){
    TASK* first = (TASK*)a;
    TASK* sec = (TASK*)b;
    return strcasecmp(first -> text,sec -> text); //Compare alphabetically

}
int cmpOrd(const void* a, const void* b){
    return (*(TASK*)b).order - (*(TASK*)a).order;
}
void listTasksAlp(TASK* tasks,int index){
    TASK* alpTasks = (TASK*) malloc (sizeof(TASK)*index); //A new independent array to sort alphabetically
    copyTasks(tasks,alpTasks,index);
    qsort(alpTasks,index,sizeof(TASK),cmpAlp);
    listTasks(alpTasks,index);
    deleteTasks(alpTasks,index); //Delete the alphabetically sorted array
}
void listTasksAgOrder(TASK* tasks,int index){
    TASK* ordTasks = (TASK*) malloc (sizeof(TASK)*index);
    copyTasks(tasks,ordTasks,index);
    qsort(ordTasks,index,sizeof(TASK),cmpOrd);
    listTasks(ordTasks,index);
    deleteTasks(ordTasks,index);
}
int main() {
    int nr = 0; 
    int index = 0;
    int order = 1;
    TASK* tasks = (TASK*) malloc (sizeof(TASK)*10);
    int constanta = 10;
    printf("Welcome to the task list sorter.\n");
    while(true){
        nr = readNR();
        if(!(nr)){
            deleteTasks(tasks,index);
            return 0;
        }
        switch(nr){
            case 1:
                addTask(&tasks,&constanta,&index,&order);
                break;
            case 2:
                removeTask(&tasks,&index);
                break;
            case 3:
                listTasks(tasks,index);
                break;
            case 4:
                listTasksAlp(tasks,index);
                break;
            case 5:
                listTasksAgOrder(tasks,index);
                break;
        }
        
    }
}
