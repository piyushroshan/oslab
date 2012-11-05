#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define ORDER_DEFAULT 5
#define ORDER_MIN 3
#define ORDER_MAX 20

typedef struct data {
	int value;
} data;

typedef struct nodet {
	void** pntrs;
	int* keys;
	bool is_leaf;
	int nkeys;
	struct nodet* parent;
	struct nodet* next;
} nodet;

int order = ORDER_DEFAULT;
nodet* queue = NULL;
FILE *fp;
nodet* insert_into_parent(nodet* root, nodet* left, int key, nodet* right);
nodet* delete_start( nodet* root, nodet* n, int key, void* pointr );

void addto_queue( nodet* nodet_new ) {
	nodet* c;
	if (queue == NULL) {
		queue = nodet_new;
		queue->next = NULL;
	}
	else {
		c = queue;
		while(c->next != NULL) {
			c = c->next;
		}
		c->next = nodet_new;
		nodet_new->next = NULL;
	}
}

nodet* deletefrm_queue( void ) {
	nodet* n = queue;
	queue = queue->next;
	n->next = NULL;
	return n;
}

int root_path( nodet* root, nodet* child ) {
	int length = 0;
	nodet* c = child;
	while (c != root) {
		c = c->parent;
		length++;
	}
	return length;
}

void print_tree( nodet* root ) {

	int rank = 0;
	int rank_new = 0;
	nodet* n = NULL;
	int i = 0;
	if (root == NULL) {
		printf("Empty tree.\n");
		return;
	}
	queue = NULL;
	addto_queue(root);
	while( queue != NULL ) {
		n = deletefrm_queue();
		if (n->parent != NULL && n == n->parent->pntrs[0]) {
			rank_new = root_path( root, n );
			if (rank_new != rank) {
				rank = rank_new;
				printf("\n");
			}
		}		
		for (i = 0; i < n->nkeys; i++) {
			printf("%d ", n->keys[i]);
		}
		if (!n->is_leaf)
			for (i = 0; i <= n->nkeys; i++)
				addto_queue(n->pntrs[i]);
		printf("| ");
	}
	printf("\n");
}

nodet* locate_leaf( nodet* root, int key) {
	int i = 0;
	nodet* c = root;
	if (c == NULL) {
		return c;
	}
	while (!c->is_leaf) {
		i = 0;
		while (i < c->nkeys) {
			if (key >= c->keys[i]) i++;
			else break;
		}
		c = (nodet*)c->pntrs[i];
	}
	return c;
}


data* locate( nodet* root, int key) {
	int i = 0;
	nodet* c = locate_leaf( root, key);
	if (c == NULL) return NULL;
	for (i = 0; i < c->nkeys; i++)
		if (c->keys[i] == key) break;
	if (i == c->nkeys) 
		return NULL;
	else
		return (data*)c->pntrs[i];
}

int cut( int length ) {
	if (length % 2 == 0)
		return length/2;
	else
		return length/2 + 1;
}

void locate_and_print(nodet* root, int key) {
	data* r = locate(root, key);
	if (r == NULL)
		printf("Key %d not found\n", key);
	else 
		printf("Key found at %lx -> key %d, value %d.\n",
				(unsigned long)r, key, r->value);
}

data* make_data(int value) {
	data* new_data = (data*)malloc(sizeof(data));
	if (new_data == NULL) {
		exit(EXIT_FAILURE);
	}
	else {
		new_data->value = value;
	}
	return new_data;
}

nodet* make_nodet( void ) {
	nodet* nodet_new;
	nodet_new = malloc(sizeof(nodet));
	if (nodet_new == NULL) {
		exit(EXIT_FAILURE);
	}
	nodet_new->keys = malloc( (order - 1) * sizeof(int) );
	if (nodet_new->keys == NULL) {
		exit(EXIT_FAILURE);
	}
	nodet_new->pntrs = malloc( order * sizeof(void*) );
	if (nodet_new->pntrs == NULL) {
		exit(EXIT_FAILURE);
	}
	nodet_new->is_leaf = false;
	nodet_new->nkeys = 0;
	nodet_new->parent = NULL;
	nodet_new->next = NULL;
	return nodet_new;
}

nodet* make_leaf( void ) {
	nodet* leaf = make_nodet();
	leaf->is_leaf = true;
	return leaf;
}

int get_lindex(nodet* parent, nodet* left) {

	int lindex = 0;
	while (lindex <= parent->nkeys && 
			parent->pntrs[lindex] != left)
		lindex++;
	return lindex;
}

nodet* insert_to_leaf( nodet* leaf, int key, data* pointr ) {

	int i, insertion_point;

	insertion_point = 0;
	while (insertion_point < leaf->nkeys && leaf->keys[insertion_point] < key)
		insertion_point++;

	for (i = leaf->nkeys; i > insertion_point; i--) {
		leaf->keys[i] = leaf->keys[i - 1];
		leaf->pntrs[i] = leaf->pntrs[i - 1];
	}
	leaf->keys[insertion_point] = key;
	leaf->pntrs[insertion_point] = pointr;
	leaf->nkeys++;
	return leaf;
}

nodet* insert_leaf_split(nodet* root, nodet* leaf, int key, data* pointr) {
	nodet* new_leaf;
	int* temp_keys;
	void** temp_pntrs;
	int insertion_index, split, new_key, i, j;
	new_leaf = make_leaf();
	temp_keys = malloc( order * sizeof(int) );
	if (temp_keys == NULL) {
		exit(EXIT_FAILURE);
	}
	temp_pntrs = malloc( order * sizeof(void*) );
	if (temp_pntrs == NULL) {
		exit(EXIT_FAILURE);
	}
	insertion_index = 0;
	while (insertion_index < order - 1 && leaf->keys[insertion_index] < key)
		insertion_index++;
	for (i = 0, j = 0; i < leaf->nkeys; i++, j++) {
		if (j == insertion_index) j++;
		temp_keys[j] = leaf->keys[i];
		temp_pntrs[j] = leaf->pntrs[i];
	}
	temp_keys[insertion_index] = key;
	temp_pntrs[insertion_index] = pointr;
	leaf->nkeys = 0;
	split = cut(order - 1);
	for (i = 0; i < split; i++) {
		leaf->pntrs[i] = temp_pntrs[i];
		leaf->keys[i] = temp_keys[i];
		leaf->nkeys++;
	}
	for (i = split, j = 0; i < order; i++, j++) {
		new_leaf->pntrs[j] = temp_pntrs[i];
		new_leaf->keys[j] = temp_keys[i];
		new_leaf->nkeys++;
	}
	free(temp_pntrs);
	free(temp_keys);
	new_leaf->pntrs[order - 1] = leaf->pntrs[order - 1];
	leaf->pntrs[order - 1] = new_leaf;
	for (i = leaf->nkeys; i < order - 1; i++)
		leaf->pntrs[i] = NULL;
	for (i = new_leaf->nkeys; i < order - 1; i++)
		new_leaf->pntrs[i] = NULL;
	new_leaf->parent = leaf->parent;
	new_key = new_leaf->keys[0];
	return insert_into_parent(root, leaf, new_key, new_leaf);
}

nodet* insert_to_nodet(nodet* root, nodet* n, 
		int lindex, int key, nodet* right) {
	int i;

	for (i = n->nkeys; i > lindex; i--) {
		n->pntrs[i + 1] = n->pntrs[i];
		n->keys[i] = n->keys[i - 1];
	}
	n->pntrs[lindex + 1] = right;
	n->keys[lindex] = key;
	n->nkeys++;
	return root;
}

nodet* insert_nodet_split(nodet* root, nodet* old_nodet, int lindex, 
		int key, nodet* right) {
	int i, j, split, k_prime;
	nodet* nodet_new, * child;
	int* temp_keys;
	nodet** temp_pntrs;
	temp_pntrs = malloc( (order + 1) * sizeof(nodet*) );
	if (temp_pntrs == NULL) {
		exit(EXIT_FAILURE);
	}
	temp_keys = malloc( order * sizeof(int) );
	if (temp_keys == NULL) {
		exit(EXIT_FAILURE);
	}
	for (i = 0, j = 0; i < old_nodet->nkeys + 1; i++, j++) {
		if (j == lindex + 1) j++;
		temp_pntrs[j] = old_nodet->pntrs[i];
	}
	for (i = 0, j = 0; i < old_nodet->nkeys; i++, j++) {
		if (j == lindex) j++;
		temp_keys[j] = old_nodet->keys[i];
	}
	temp_pntrs[lindex + 1] = right;
	temp_keys[lindex] = key; 
	split = cut(order);
	nodet_new = make_nodet();
	old_nodet->nkeys = 0;
	for (i = 0; i < split - 1; i++) {
		old_nodet->pntrs[i] = temp_pntrs[i];
		old_nodet->keys[i] = temp_keys[i];
		old_nodet->nkeys++;
	}
	old_nodet->pntrs[i] = temp_pntrs[i];
	k_prime = temp_keys[split - 1];
	for (++i, j = 0; i < order; i++, j++) {
		nodet_new->pntrs[j] = temp_pntrs[i];
		nodet_new->keys[j] = temp_keys[i];
		nodet_new->nkeys++;
	}
	nodet_new->pntrs[j] = temp_pntrs[i];
	free(temp_pntrs);
	free(temp_keys);
	nodet_new->parent = old_nodet->parent;
	for (i = 0; i <= nodet_new->nkeys; i++) {
		child = nodet_new->pntrs[i];
		child->parent = nodet_new;
	}
	return insert_into_parent(root, old_nodet, k_prime, nodet_new);
}

nodet* insert_to_new_root(nodet* left, int key, nodet* right) {
	nodet* root = make_nodet();
	root->keys[0] = key;
	root->pntrs[0] = left;
	root->pntrs[1] = right;
	root->nkeys++;
	root->parent = NULL;
	left->parent = root;
	right->parent = root;
	return root;
}

nodet* insert_into_parent(nodet* root, nodet* left, int key, nodet* right) {
	int lindex;
	nodet* parent;
	parent = left->parent;
	if (parent == NULL)
		return insert_to_new_root(left, key, right);
	lindex = get_lindex(parent, left);
	if (parent->nkeys < order - 1)
		return insert_to_nodet(root, parent, lindex, key, right);
	return insert_nodet_split(root, parent, lindex, key, right);
}

nodet* start_new(int key, data* pointr) {
	nodet* root = make_leaf();
	root->keys[0] = key;
	root->pntrs[0] = pointr;
	root->pntrs[order - 1] = NULL;
	root->parent = NULL;
	root->nkeys++;
	return root;
}

nodet* insert( nodet* root, int key, int value ) {
	data* pointr;
	nodet* leaf;
	if (locate(root, key) != NULL)
	{
		printf("Duplicate key\n");
		return root;
	}
	pointr = make_data(value);
	if (root == NULL) 
		return start_new(key, pointr);
	leaf = locate_leaf(root, key);
	if (leaf->nkeys < order - 1) {
		leaf = insert_to_leaf(leaf, key, pointr);
		return root;
	}
	return insert_leaf_split(root, leaf, key, pointr);
}

int get_neighbour( nodet* n ) {
	int i;
	for (i = 0; i <= n->parent->nkeys; i++)
		if (n->parent->pntrs[i] == n)
			return i - 1;
	printf("Search nonexistent pointer.\n");
	printf("Node:  %#lx\n", (unsigned long)n);
	exit(EXIT_FAILURE);
}

nodet* remove_entry_nodet(nodet* n, int key, nodet* pointr) {
	int i, num_pntrs;
	i = 0;
	while (n->keys[i] != key)
		i++;
	for (++i; i < n->nkeys; i++)
		n->keys[i - 1] = n->keys[i];
	num_pntrs = n->is_leaf ? n->nkeys : n->nkeys + 1;
	i = 0;
	while (n->pntrs[i] != pointr)
		i++;
	for (++i; i < num_pntrs; i++)
		n->pntrs[i - 1] = n->pntrs[i];
	n->nkeys--;
	if (n->is_leaf)
		for (i = n->nkeys; i < order - 1; i++)
			n->pntrs[i] = NULL;
	else
		for (i = n->nkeys + 1; i < order; i++)
			n->pntrs[i] = NULL;
	return n;
}

nodet* root_fix(nodet* root) {
	nodet* new_root;
	if (root->nkeys > 0)
		return root;
	if (!root->is_leaf) {
		new_root = root->pntrs[0];
		new_root->parent = NULL;
	}
	else
		new_root = NULL;
	free(root->keys);
	free(root->pntrs);
	free(root);
	return new_root;
}

nodet* merge_nodets(nodet* root, nodet* n, nodet* neighbour, int neighbour_i, int k_prime) {
	int i, j, neighbour_insertion_i, n_start, n_end, new_key_prime;
	nodet* tmp;
	bool split;
	if (neighbour_i == -1) {
		tmp = n;
		n = neighbour;
		neighbour = tmp;
	}
	neighbour_insertion_i = neighbour->nkeys;
	split = false;
	if (!n->is_leaf) {
		neighbour->keys[neighbour_insertion_i] = k_prime;
		neighbour->nkeys++;
		n_end = n->nkeys;
		n_start = 0; 
		if (n->nkeys + neighbour->nkeys >= order) {
			split = true;
			n_end = cut(order) - 2;
		}
		for (i = neighbour_insertion_i + 1, j = 0; j < n_end; i++, j++) {
			neighbour->keys[i] = n->keys[j];
			neighbour->pntrs[i] = n->pntrs[j];
			neighbour->nkeys++;
			n->nkeys--;
			n_start++;
		}
		neighbour->pntrs[i] = n->pntrs[j];
		if (split) {
			new_key_prime = n->keys[n_start];
			for (i = 0, j = n_start + 1; i < n->nkeys; i++, j++) {
				n->keys[i] = n->keys[j];
				n->pntrs[i] = n->pntrs[j];
			}
			n->pntrs[i] = n->pntrs[j];
			n->nkeys--;
		}
		for (i = 0; i < neighbour->nkeys + 1; i++) {
			tmp = (nodet*)neighbour->pntrs[i];
			tmp->parent = neighbour;
		}
	}
	else {
		for (i = neighbour_insertion_i, j = 0; j < n->nkeys; i++, j++) {
			neighbour->keys[i] = n->keys[j];
			neighbour->pntrs[i] = n->pntrs[j];
			neighbour->nkeys++;
		}
		neighbour->pntrs[order - 1] = n->pntrs[order - 1];
	}
	if (!split) {
		root = delete_start(root, n->parent, k_prime, n);
		free(n->keys);
		free(n->pntrs);
		free(n); 
	}
	else
		for (i = 0; i < n->parent->nkeys; i++)
			if (n->parent->pntrs[i + 1] == n) {
				n->parent->keys[i] = new_key_prime;
				break;
			}
	return root;
}

nodet* split_nodets(nodet* root, nodet* n, nodet* neighbour, int neighbour_i, 
		int k_prime_index, int k_prime) {  
	int i;
	nodet* tmp;
	if (neighbour_i != -1) {
		if (!n->is_leaf)
			n->pntrs[n->nkeys + 1] = n->pntrs[n->nkeys];
		for (i = n->nkeys; i > 0; i--) {
			n->keys[i] = n->keys[i - 1];
			n->pntrs[i] = n->pntrs[i - 1];
		}
		if (!n->is_leaf) {
			n->pntrs[0] = neighbour->pntrs[neighbour->nkeys];
			tmp = (nodet*)n->pntrs[0];
			tmp->parent = n;
			neighbour->pntrs[neighbour->nkeys] = NULL;
			n->keys[0] = k_prime;
			n->parent->keys[k_prime_index] = neighbour->keys[neighbour->nkeys - 1];
		}
		else {
			n->pntrs[0] = neighbour->pntrs[neighbour->nkeys - 1];
			neighbour->pntrs[neighbour->nkeys - 1] = NULL;
			n->keys[0] = neighbour->keys[neighbour->nkeys - 1];
			n->parent->keys[k_prime_index] = n->keys[0];
		}
	}
	else {  
		if (n->is_leaf) {
			n->keys[n->nkeys] = neighbour->keys[0];
			n->pntrs[n->nkeys] = neighbour->pntrs[0];
			n->parent->keys[k_prime_index] = neighbour->keys[1];
		}
		else {
			n->keys[n->nkeys] = k_prime;
			n->pntrs[n->nkeys + 1] = neighbour->pntrs[0];
			tmp = (nodet*)n->pntrs[n->nkeys + 1];
			tmp->parent = n;
			n->parent->keys[k_prime_index] = neighbour->keys[0];
		}
		for (i = 0; i < neighbour->nkeys; i++) {
			neighbour->keys[i] = neighbour->keys[i + 1];
			neighbour->pntrs[i] = neighbour->pntrs[i + 1];
		}
		if (!n->is_leaf)
			neighbour->pntrs[i] = neighbour->pntrs[i + 1];
	}
	n->nkeys++;
	neighbour->nkeys--;
	return root;
}

nodet* delete_start( nodet* root, nodet* n, int key, void* pointr ) {
	int min_keys;
	nodet* neighbour;
	int neighbour_i;
	int k_prime_index, k_prime;
	int capacity;
	n = remove_entry_nodet(n, key, pointr);
	if (n == root) 
		return root_fix(root);
	min_keys = n->is_leaf ? cut(order - 1) : cut(order) - 1;
	if (n->nkeys >= min_keys)
		return root;
	neighbour_i = get_neighbour( n );
	k_prime_index = neighbour_i == -1 ? 0 : neighbour_i;
	k_prime = n->parent->keys[k_prime_index];
	neighbour = neighbour_i == -1 ? n->parent->pntrs[1] : 
		n->parent->pntrs[neighbour_i];
	capacity = n->is_leaf ? order : order - 1;
	if (neighbour->nkeys + n->nkeys < capacity)
		return merge_nodets(root, n, neighbour, neighbour_i, k_prime);
	else
		return split_nodets(root, n, neighbour, neighbour_i, k_prime_index, k_prime);
}

nodet* delete(nodet* root, int key) {
	nodet* kleaf;
	data* kdata;
	kdata = locate(root, key);
	kleaf = locate_leaf(root, key);
	if (kdata != NULL && kleaf != NULL) {
		root = delete_start(root, kleaf, key, kdata);
		free(kdata);
	}
	return root;
}

void destroy_nodes_tree(nodet* root) {
	int i;
	if (root->is_leaf)
		for (i = 0; i < root->nkeys; i++)
			free(root->pntrs[i]);
	else
		for (i = 0; i < root->nkeys + 1; i++)
			destroy_nodes_tree(root->pntrs[i]);
	free(root->pntrs);
	free(root->keys);
	free(root);
}

nodet* destroy_tree(nodet* root) {
	destroy_nodes_tree(root);
	return NULL;
}

void usagecmd( void ) {
	printf("B+ Tree. Enter choice > :\n");
	printf("\ti <k> -> Insert <k> (an integer) as both key and value).\n");
	printf("\tf <k> -> Find the value under key <k>.\n");
	printf("\td <k> -> Delete key <k> and value.\n");
	printf("\tp -> Print\n");
	printf("\tq -> Quit.\n");
}



void input_function(){
  char input,blank,dump;
  int position,i,key;
  nodet *root;
  root = NULL;
  printf("> ");
  
  while (fscanf(fp,"%c",&input) != EOF) { 
    switch (input) {
      case 'q' : printf("\n");
      			 exit(0);
      case 'f' : fscanf(fp,"%c%d%c",&blank,&key,&dump);
                 printf("FIND: %d\n",key);
                 locate_and_print(root, key);
                 break;
      case 'i' : fscanf(fp,"%c%d%c",&blank,&key,&dump);
                 printf("INSERT: %d\n",key); 
                 root=insert(root, key, key);
                 break;     
      case 'd' : fscanf(fp,"%c%d%c",&blank,&key,&dump);
                 printf("DELETE: %d\n",key); 
                 root=delete(root, key);
                 break;
      case 'p' : fscanf(fp,"%c",&dump);
                   printf("B+ TREE:\n"); 
                   print_tree(root);
                 break;
      default:
               usagecmd();
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
  	nodet* root;
	int input;
	char instruction;
	root = NULL;
	if(order==5)
	printf("Default order = 5\n");
	input_function();
  	printf("Exiting\n");
  	return 0; 
}
