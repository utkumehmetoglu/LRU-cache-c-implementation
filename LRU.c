#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define CACHE_SIZE 4
#define LOAD_FACTOR 0.6

typedef struct Node
{
  char kimlik_no[12];
  char ad[20];
  char soyad[20];
  char dtarihi[20];
  char dyeri[20];
  struct Node *next;
}Node;

typedef struct element{
	int deleted;
	struct Node *sira;
	char kimlik_no[12];
} element;

typedef struct Hash {
    int capacity;
	int curr_size; 
    element** array; 
} Hash;


unsigned int horner(char str[12])
{
	int i;
    unsigned int result = 0; 
    int n = strlen(str);
 
    for (i=0; i<n; i++)
        result += pow(31,i) * (int)str[i];
 
    return result ;
}
int hash1(int key,int m){
	return (key%m);
}

int hash2(int key,int m){
	return 1 + (key % (m-1));
}
int h(int key, int m, int i){
	return ( (hash1(key,m) + i*hash2(key,m)) % m);
}
int isFull(Hash *h)
{
      if(h->curr_size >= h->capacity)
         return 1;
      return 0;
}
void insert(char *str, Hash* h,struct Node **s){
    if(isFull(h)){
	   puts("HATA! TABLO DOLDU LUTFEN CACHE BOYUTUNU ARTTIRIN!");
	   return;
	}
       
    unsigned int key=horner(str);
    int index=hash1(key, h->capacity);
    if(strcmp(h->array[index]->kimlik_no,"") ){
    	
    	int index2 = hash2(key,h->capacity);
    	int i=1;
    	while (1) {
                int newIndex = (index + i * index2) % h->capacity;
 

                if (!strcmp(h->array[newIndex]->kimlik_no,"")) {
                    strcpy(h->array[newIndex]->kimlik_no,str);
                    h->array[newIndex]->sira = *s;
                    break;
                }
                if(h->array[newIndex]->deleted == 1){
					//printf("DEBUG:%s %s\n",h->array[newIndex]->kimlik_no, str);
					//strcpy(h->array[newIndex]->kimlik_no,str); strcpy bozuluyor nedense?
					int t;
					for(t=0;t<0;t++){
						h->array[newIndex]->kimlik_no[t] = str[t];
					}
                	h->array[newIndex]->deleted = 0;
                	h->array[newIndex]->sira = *s;
                    break;
				}
                i++;
            }
    	
    	
	}
	else{
		strcpy(h->array[index]->kimlik_no,str);
		h->array[index]->sira = *s;
	}
 	h->curr_size++;
 	
 }


Hash* createHash(int capacity)
{
    Hash* hash = (Hash*)malloc(sizeof(Hash));
    hash->capacity = capacity;
 
    hash->array = (element**)malloc(hash->capacity * sizeof(element*));
    

    int i;
    for (i = 0; i < hash->capacity; ++i){
	    hash->array[i] = (element*) malloc(sizeof(element));
        strcpy(hash->array[i]->kimlik_no, "");
        hash->array[i]->deleted = 0;
	 }
    hash->curr_size = 0;
    return hash;
}

int search(char key[12],Hash *h)
{
    int index1 = hash1(horner(key),h->capacity);
    int index2 = hash2(horner(key),h->capacity);
    int i = 0;
    while (strcmp(h->array[(index1 + i * index2) % h->capacity]->kimlik_no,key) && i<h->capacity) {
        if (strcmp(h->array[(index1 + i * index2) % h->capacity]->kimlik_no,"") == 0) {
            return -1;
        }
        i++;
        
    }
    if(i==h->capacity)
      return -1;
    return (index1 + i * index2) % h->capacity;
} 
 

Node* push(struct Node** head_ref, char ad[20],char soyad[20],char dtarihi[20],char kimlik[20],char dyeri[20])
{
    struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));
 
    strcpy(new_node->ad,ad);
    strcpy(new_node->soyad,soyad);
    strcpy(new_node->dtarihi,dtarihi);
    strcpy(new_node->kimlik_no,kimlik);
    strcpy(new_node->dyeri,dyeri);
 
    new_node->next = (*head_ref);
 
    (*head_ref)    = new_node;
    return new_node;
}
int isCacheFull(struct Node *head)
{
    

   
    if(head == NULL)
    {
        return 0;
    }
    
    int i=0;
    while(head->next != NULL)
    {
    	head = head->next;
        i++;           
    }
    if(i>=CACHE_SIZE - 1)
     return 1;
    else 
      return 0;
}

void append(struct Node** head_ref,  struct Node *new_node)
{
    struct Node *last = *head_ref;  
    
    new_node->next = NULL;
 
    if (*head_ref == NULL)
    {
       *head_ref = new_node;
       return;
    }
 
    while (last->next != NULL)
        last = last->next;

    last->next = new_node;
    return;
}
void deleteNode(struct Node** head_ref, char key[12])
{
    struct Node *temp = *head_ref, *prev;
 
    if (temp != NULL && (strcmp(temp->kimlik_no,key)!= 0) ) {
        *head_ref = temp->next; 
        free(temp); 
        return;
    }
 

    while (temp != NULL && (strcmp(temp->kimlik_no,key)!= 0)) {
        prev = temp;
        temp = temp->next;
    }
 
    if (temp == NULL)
        return;
 
    prev->next = temp->next;
 
    free(temp);
}

void printList(struct Node *node)
{
  while (node != NULL)
  {
     printf(" %s ", node->ad);
     node = node->next;
  }
}
int calculate_table_size(){
	int sz = (int)ceil(CACHE_SIZE/LOAD_FACTOR ) *4;
	int *primes = (int*)calloc(sz,sizeof(int));
	primes[0] = 1; primes[1] = 1;  int i,j;
	    for (i = 2; i * i <= sz ; i ++) {
		   if(primes[i] == 0)     
            for (j = 2 * i; j <= sz; j += i)
                primes[j] = 1;
 
    }
    
	for (i = 2; i <= sz; i++) {
        if(primes[i] == 0){
        	if(i>=(int)ceil(CACHE_SIZE/LOAD_FACTOR)){
            	free(primes);
				return i;
			}
		}   
    }
    free(primes);
	return sz;
	
}
Node* find_latest(struct Node* head){
	int i=0;
	while(head->next != NULL  && i<CACHE_SIZE){
		
		head=head->next;
		i++;
	}
	return head;
}



int main()
{
	
    FILE *fp;
	fp = fopen("test.txt", "r");
  

    if (fp == NULL)
    {
        printf("Could not open file");
        return 0;
    }
	
    Hash* hash = createHash(calculate_table_size());
    Node* head = NULL; 
    char kimlik[12];
    char ad[20];char soyad[20];
	char dtarihi[20];
	char dyeri[20];
	while(fscanf(fp,"%s %s %s %s %s",kimlik,ad,soyad,dtarihi,dyeri) != EOF){
	int res = search(kimlik,hash);
    if(res != -1){
    	puts("kimlik bulundu\n");
		
		deleteNode(&head,kimlik);
		Node *new_node = push(&head,ad,soyad,dtarihi,kimlik,dyeri);
		hash->array[res]->sira = new_node;
		printf("kimlik no:%s\nisim:%s\nsoyisim:%s\ndogum tarihi:%s\ndogum yeri:%s\n\n",hash->array[res]->sira->kimlik_no,hash->array[res]->sira->ad,hash->array[res]->sira->soyad,hash->array[res]->sira->dtarihi,hash->array[res]->sira->dyeri);
	}
	else{
		printf("kimlik bulunamadi\n");
		
		if(isCacheFull(head)){
			
			struct Node *d = find_latest(head);
			hash->array[search(d->kimlik_no,hash)]->deleted = 1;
			deleteNode(&head,d->kimlik_no);
			
			puts("cache temizlendi");
		}
			
			Node *new_node = push(&head,ad,soyad,dtarihi,kimlik,dyeri);
			insert(kimlik,hash,&new_node);
			
		
	}
    }
    fclose(fp);
    free(hash);
    return 0;
}
