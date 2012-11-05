#include <stdio.h>
#include <stdlib.h>
#define FALSE 0
#define TRUE 1
#define MAX_ORDER 4
#define MIN_ORDER 2
#define HEIGHT 10
#define WIDTH 50

typedef struct nodes{
  int count;
  int key[MAX_ORDER+1];
  struct nodes *branch[MAX_ORDER+1];
}Node;

int cnt[HEIGHT];
int levels;
int loc[HEIGHT][WIDTH];
Node *pointer[HEIGHT][WIDTH];
Node *root;
FILE *fp;
int key_pos;
void printerror(char *errormsg){
  printf("%s\n",errormsg);
}

void print_format(){
  int i,depth,pos,base,first,last,dist;
  int x;
  depth = levels;
  pos = 0;
  for (i=0; i<cnt[depth]; i++){
    loc[depth][i] = pos; 
    pos += (pointer[depth][i]->count)*2+2;
  }
  for (depth=levels-1; depth>=0; depth--){
    base = 0;
    for (i=0; i<cnt[depth]; i++){
       first = base;
       last  = first+(pointer[depth][i]->count);
       dist = (loc[depth+1][last] - loc[depth+1][first] + 2*pointer[depth+1][last]->count)/2;
       loc[depth][i] = loc[depth+1][first]+dist-pointer[depth][i]->count;
       base += pointer[depth][i]->count+1;
    }

  }
}

void print_node(Node *pnt, int depth, int i, int *pos,int signal){
  int j;
  if(signal)
  {
    while ((*pos)<loc[depth][i]){
      printf(" ");
      (*pos)++;
    }
  }
  for (j=1; j<pnt->count; j++){
    printf("%d ",pnt->key[j]);
    (*pos) += 2;
  }
  printf("%d |",pnt->key[j]);
  (*pos)+=3;
}

void depth_first_search(Node *pnt, int depth){
  int i;
  if (pnt != NULL) {
    if (depth>levels) levels=depth;
    pointer[depth][cnt[depth]] = pnt;
    cnt[depth]++;
    for (i=0; i<=pnt->count; i++)
      depth_first_search(pnt->branch[i],depth+1); 
  }
}

void print_allnodes(Node *root){
  int i,depth,pos,child;
  if (root==NULL)
    printf("EMPTY\n");
  else{
  for (depth=0; depth<HEIGHT; depth++) {
    cnt[depth]=0;
  }
  levels=0; 
  depth_first_search(root,0);
  print_format();
  for (depth=0; depth<=levels; depth++){
    pos=-1;
    for (i=0; i<cnt[depth]; i++)
      print_node(pointer[depth][i],depth,i,&pos,TRUE);
    printf("\n");
  } 
  }
}

int linear_search(int target,Node *pnt, int *k){
  if (target < pnt->key[1]) {  
    *k = 0;
    return FALSE;
  } 
   else {
     *k = pnt->count;
     while ((target<pnt->key[*k]) && *k > 1){
       (*k)--;
    }
      return (target==pnt->key[*k]);
	}
}

void putkey_in(int x,Node *xr,Node *pnt, int k){
   int i; 
   for (i = pnt->count; i > k; i--) {
     pnt->key[i+1] = pnt->key[i];
     pnt->branch[i+1] = pnt->branch[i];
   }
   pnt->key[k+1] = x;
   pnt->branch[k+1] = xr;
   pnt->count++;
}

Node *search_key(int target, Node *root, int *targetpos){
  if (root == NULL)
    return NULL;
  else 
    if (linear_search(target,root,targetpos))
      return root;
    else
      return search_key(target,root->branch[*targetpos],targetpos);
}

void cut_median(int x, Node *xr, Node *pnt, int k, int *y, Node **yr){
  int i;
  int median;
  if (k <= MIN_ORDER)
    median = MIN_ORDER;
  else
    median = MIN_ORDER + 1;
  *yr = (Node *)malloc(sizeof(Node));
  for (i = median+1; i <= MAX_ORDER; i++) {
    (*yr)->key[i-median] = pnt->key[i];
    (*yr)->branch[i-median] = pnt->branch[i];
  }
  (*yr)->count = MAX_ORDER - median;
  pnt->count = median;
  if (k <= MIN_ORDER)
   putkey_in(x,xr,pnt,k);
  else
    putkey_in(x,xr,*yr,k - median);
  *y = pnt->key[pnt->count];
  (*yr)->branch[0] = pnt->branch[pnt->count];
   pnt->count--;
}

int push_down(int newkey,Node *pnt,int *x,Node **xr){
   int k; 
   if (pnt == NULL) {
     *x = newkey;
     *xr = NULL;
     return TRUE;
   } else {  
      if (linear_search(newkey,pnt,&k))
      {
          printerror("Duplicate key");
          return FALSE;
      }
      if (push_down(newkey,pnt->branch[k],x,xr))
        if (pnt->count < MAX_ORDER) {
          putkey_in(*x,*xr,pnt,k);
          return FALSE;
        } else {
          cut_median(*x,*xr,pnt,k,x,xr);
          return TRUE;
        }
        return FALSE;
	}
}

Node *insert_main(int newkey, Node *root){
  int x;	
  Node *xr;
  Node *pnt;
  int pushup;
  pushup = push_down(newkey,root,&x,&xr);
  if (pushup) {
    pnt = (Node *)malloc(sizeof(Node)); 
    pnt->count = 1;
    pnt->key[1] = x;
    pnt->branch[0] = root;
    pnt->branch[1] = xr;
    return pnt;
  }
  return root;
}

void shift_right(Node *pnt,int k){
  int c;
  Node *t;
  t = pnt->branch[k];
  for (c=t->count; c>0; c--){
     t->key[c+1]=t->key[c];
     t->branch[c+1]=t->branch[c];
  }
  t->branch[1] = t->branch[0];
  t->count++;
  t->key[1]=pnt->key[k];
  t=pnt->branch [k-1];
  pnt->key[k]=t->key[t->count];
  pnt->branch[k]->branch[0]=t->branch[t->count];
  t->count--;
}

void shift_left(Node *pnt,int k){
  int c;
  Node *t;
  t = pnt->branch[k-1];
  t->count++;
  t->key[t->count] = pnt->key[k];
  t->branch[t->count] = pnt->branch[k]->branch[0];
  t = pnt->branch[k];
  pnt->key[k] = t->key[1];
  t->branch[0] = t->branch[1];
  t->count--;
  for (c=1; c<=t->count; c++){
    t->key[c] = t->key[c+1];
    t->branch[c] = t->branch[c+1];
  }
}

void merge(Node *pnt,int k){
  int c;
  Node *q;
  Node *l;
  q = pnt->branch[k];
  l = pnt->branch[k-1];
  l->count++;
  l->key[l->count] = pnt->key[k];
  l->branch[l->count] = q->branch[0];
  for (c=1; c<=q->count; c++){
    l->count++;
    l->key[l->count] = q->key[c];
    l->branch[l->count] = q->branch[c];
  }
  for (c=k; c<pnt->count; c++){
    pnt->key[c] = pnt->key[c+1];
    pnt->branch[c] = pnt->branch[c+1];
  }
  pnt->count--;
  free(q);
}

void restore_key(Node *pnt, int k){
  if (k==0)
    if (pnt->branch[1]->count >MIN_ORDER)
      shift_left(pnt,1);
    else
      merge(pnt,1);
  else if (k==pnt->count)
    if (pnt->branch[k-1]->count >MIN_ORDER)
      shift_right(pnt,k);
    else
      merge(pnt,k);
  else if (pnt->branch[k-1]->count>MIN_ORDER)
         shift_right(pnt,k);
  else if (pnt->branch[k+1]->count>MIN_ORDER)
         shift_left(pnt,k+1);
  else 
      merge(pnt,k);
}

void remove_key(Node *pnt,int k){
  int i;
  for (i=k+1; i<=pnt->count; i++){
    pnt->key[i-1] = pnt->key[i];
    pnt->branch[i-1] = pnt->branch[i];
  }
  pnt->count--;
}

void find_successor(Node *pnt, int k){
  Node *temp;
  for (temp=pnt->branch[k]; temp->branch[0]; temp=temp->branch[0]);
  pnt->key[k]=temp->key[1];
}

int iter_delete(int target,Node *pnt){
  int k;
  int found;
  if (pnt==NULL)
    return FALSE;
  else{  
       found=linear_search(target,pnt,&k);
       if (found)
         if (pnt->branch[k-1]){
           find_successor(pnt,k);
           if (!(found=iter_delete(pnt->key[k],pnt->branch[k])))
             printerror("Key not found.");
         }else
              remove_key(pnt,k);
      else
           found=iter_delete(target,pnt->branch[k]);
      if (pnt->branch[k] != NULL)
        if (pnt->branch[k]->count<MIN_ORDER)
          restore_key(pnt,k);
      return found;
     }
}

Node *delete_key(int target, Node *root)
{
   Node *pnt,*t;
	
   t = root;
   if (!iter_delete(target, t))
     printerror("Target was not in the B-tree.");
   else 
     if (root->count == 0) {
       pnt = root;
       root = root->branch[0];
       free(pnt);
     }
  return root;
}

void usageinput( void ) {
  printf("B Tree. Enter choice > :\n");
  printf("\ti <k> -> insert_main <k> (an integer) as both key and value).\n");
  printf("\tf <k> -> Find the value under key <k>.\n");
  printf("\td <k> -> Delete key <k> and value.\n");
  printf("\tp -> Print.\n");
  printf("\tq -> Quit.\n");
}


void input_function(){
  char input,blank,dump;
  int position,i,key;
  Node *node;
  root = NULL;
  printf("> ");
  
  while (fscanf(fp,"%c",&input) != EOF) { 
    switch (input) {
      case 'q' : printf("\n");
                 exit(0);
      case 'f' : fscanf(fp,"%c%d%c",&blank,&key,&dump);
                 printf("FIND: %d\n",key);
                 node=search_key(key,root,&position);
                 if(node)
                  {
                    printf("Found key %d at position %d in node ",key,position);
                    int temps=-1;
                    print_node(node,0,0,&temps,FALSE);
                    printf("\n");
                  }
                else
                  printf("Not found %d\n",key);
                 break;
      case 'i' : fscanf(fp,"%c%d%c",&blank,&key,&dump);
                 printf("INSERT: %d\n",key); 
                 root=insert_main(key,root);
                 break;     
      case 'd' : fscanf(fp,"%c%d%c",&blank,&key,&dump);
                 printf("DELETE: %d\n",key); 
                 root=delete_key(key,root);
                 break;
      case 'p' : fscanf(fp,"%c",&dump);
                   printf("B-TREE:\n"); 
                   print_allnodes(root);
                 break;
      default:
               usageinput();
               break; 
    }
    printf("> ");
  }   
}

int main(int argc, char *argv[]) {
  if (argc == 1)
    fp = stdin;
  else {
    fp = fopen(argv[1],"r");
    if (fp == NULL) {
      printf("No File: %s\n",argv[1]);
      return 1;
    }
  }
  root = NULL;
  input_function();
  printf("Exiting\n");
  return 0; 
}