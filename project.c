#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <stdlib.h>

struct node{
  char statement[40];
  int next;
};

struct node textbuffer[25];
int free_head;
int inuse_head;
FILE *filep = NULL;
int x;
int y;

void save(char *filename){
  int i = inuse_head;
  //FILE *fp = NULL;
  filep = fopen(filename,"w");
  while(textbuffer[i].next != -1){
    fprintf(filep,"%s",textbuffer[i].statement);
    i++;
  }
  fclose(filep);
}


void insert(char *stat,int line){



  if(line == 0 || (inuse_head == -1 && free_head == 0)){//hep başa ekliceksek
    int tmp = free_head; // buraya ekledik aslında
    free_head = textbuffer[free_head].next; // free_head nextine geçti
    textbuffer[tmp].next = textbuffer[line].next;
    strcpy(textbuffer[tmp].statement, stat);
  }
  else if(line == free_head){ // sona eklicez
    int tmp = free_head; // buraya ekledik aslında
    free_head = textbuffer[free_head].next; // free_head nextine geçti
    textbuffer[tmp].next = -1;
    strcpy(textbuffer[tmp].statement, stat);

  }
  else{//between 2 nodes
    int tmp = free_head; // buraya ekledik aslında
    //textbuffer[tmp].next = free_head.next;
    free_head = textbuffer[free_head].next; // free_head nextine geçti
    strcpy(textbuffer[free_head].statement, stat);

    int i = inuse_head;
    while(textbuffer[i].next != line){
      i = textbuffer[i].next;
      i++;
    }
    textbuffer[i].next = tmp; // ekliceğimiz yerin bi öncesi eklediğimiz yeri
    textbuffer[tmp].next = textbuffer[line].next; //eklediğimiz ekliceğimiz yerin bi sonrasını gösteriyo
  }




}

void delete(int y){
  int temp = free_head;
  if(y == inuse_head){ // eğer inuse_head'i siliceksek
    inuse_head = textbuffer[inuse_head].next;
    free_head = y;
    textbuffer[free_head].next = temp;
  }
  else { // bu durumda inuse_head zaten değişmiyor.
    int tmp = free_head;
    textbuffer[y-1].next = textbuffer[y].next;
    free_head = y;
    textbuffer[free_head].next = temp;
  }
  

}
void initializetextbuffer(){ // en başta dosyayı açınca
    int i = 0;
    inuse_head = -1; // inuse_head boş
    free_head = 0; // herşey free_head ' de
    for(int i = 0; i < 24; i++){
      textbuffer[i].next = i + 1;
    }
    textbuffer[24].next = -1;

}

void edit(char *filename){
  char ch[40];
  initializetextbuffer();
  filep = fopen(filename,"r");
  if(filep != NULL){ // dosya var mı yok mu
    int i = 0;
    while(!feof(filep) && i < 25 ){//dosyanın sonuna kadar
      fgets(ch,40,filep);
      strcpy(textbuffer[i].statement, ch); // textbuffer'ı dolduruyoruz.
      textbuffer[i].next = i+1;
      printw("%s",ch); // char arrayi
      i++;
    }
    textbuffer[i].next = -1; // i kaçıncı indexte döngüden çıktıysa oranın nexti -1
    free_head = i+1; // bitince free_head de ordan sonra başlicak
    x = strlen(textbuffer[i].statement);
    //x = 0;
    y = i+1;
  }
  fclose(filep);
}

int main(){
  char in[20];//GİRİCEĞİMİZ LINELAR
  char filename[20];
  printf("Enter the file name: \n");
  scanf("%s",filename);
  //filename = &filenamee;
  refresh();
  char input;
  initscr();
  raw();
  noecho();

  edit(filename);

  while(true){
    input = getch();
    if(input == 'w' || input == 'W'){
      move(--y,x);//cursor up
      refresh();
    }
    else if(input == 'z' || input == 'Z'){
      move(++y,x);//cursor down
      refresh();

    }
    else if(input == 'I' || input == 'i'){
      printw("Enter a line: \n");
      scanw("%s",in);
      insert(in,y);
      refresh();

    }
    else if(input == 'D' || input == 'd'){
      delete(y);
      refresh();

    }
    else if(input == 'S' || input == 's'){
      save(filename);
      refresh();
    }
    else if(input == 'X' || input == 'x'){
      break;
    }

  }
  endwin();
}
