#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define DEFAULT_ORDER 4
#define MIN_ORDER 3
#define MAX_ORDER 20

typedef struct record {
	int value;
} record;

typedef struct node {
	void** pointers;
	int* keys;
	struct node* parent;
	bool is_leaf;
	int nkeys;
	struct node* next; // Used for queue.
} node;

int order = DEFAULT_ORDER;
node* queue = NULL;
FILE *fp;
node* insert_into_parent(node* root, node* left, int key, node* right);
node* delete_entry( node* root, node* n, int key, void* pointer );

void enqueue( node* new_node ) {
	node* c;
	if (queue == NULL) {
		queue = new_node;
		queue->next = NULL;
	}
	else {
		c = queue;
		while(c->next != NULL) {
			c = c->next;
		}
		c->next = new_node;
		new_node->next = NULL;
	}
}

node* dequeue( void ) {
	node* n = queue;
	queue = queue->next;
	n->next = NULL;
	return n;
}

int root_path( node* root, node* child ) {
	int length = 0;
	node* c = child;
	while (c != root) {
		c = c->parent;
		length++;
	}
	return length;
}

void print_tree( node* root ) {

	node* n = NULL;
	int i = 0;
	int rank = 0;
	int new_rank = 0;

	if (root == NULL) {
		printf("Empty tree.\n");
		return;
	}
	queue = NULL;
	enqueue(root);
	while( queue != NULL ) {
		n = dequeue();
		if (n->parent != NULL && n == n->parent->pointers[0]) {
			new_rank = root_path( root, n );
			if (new_rank != rank) {
				rank = new_rank;
				printf("\n");
			}
		}		
		for (i = 0; i < n->nkeys; i++) {
			printf("%d ", n->keys[i]);
		}
		if (!n->is_leaf)
			for (i = 0; i <= n->nkeys; i++)
				enqueue(n->pointers[i]);
		printf("| ");
	}
	printf("\n");
}

node* find_leaf( node* root, int key) {
	int i = 0;
	node* c = root;
	if (c == NULL) {
		return c;
	}
	while (!c->is_leaf) {
		i = 0;
		while (i < c->nkeys) {
			if (key >= c->keys[i]) i++;
			else break;
		}
		c = (node*)c->pointers[i];
	}
	return c;
}


record* find( node* root, int key) {
	int i = 0;
	node* c = find_leaf( root, key);
	if (c == NULL) return NULL;
	for (i = 0; i < c->nkeys; i++)
		if (c->keys[i] == key) break;
	if (i == c->nkeys) 
		return NULL;
	else
		return (record*)c->pointers[i];
}

int cut( int length ) {
	if (length % 2 == 0)
		return length/2;
	else
		return length/2 + 1;
}

void find_and_print(node* root, int key) {
	record* r = find(root, key);
	if (r == NULL)
		printf("Record not found under key %d.\n", key);
	else 
		printf("Record at %lx -> key %d, value %d.\n",
				(unsigned long)r, key, r->value);
}

record* make_record(int value) {
	record* new_record = (record*)malloc(sizeof(record));
	if (new_record == NULL) {
		perror("Record creation.");
		exit(EXIT_FAILURE);
	}
	else {
		new_record->value = value;
	}
	return new_record;
}

node* make_node( void ) {
	node* new_node;
	new_node = malloc(sizeof(node));
	if (new_node == NULL) {
		perror("Node creation.");
		exit(EXIT_FAILURE);
	}
	new_node->keys = malloc( (order - 1) * sizeof(int) );
	if (new_node->keys == NULL) {
		perror("New node keys array.");
		exit(EXIT_FAILURE);
	}
	new_node->pointers = malloc( order * sizeof(void*) );
	if (new_node->pointers == NULL) {
		perror("New node pointers array.");
		exit(EXIT_FAILURE);
	}
	new_node->is_leaf = false;
	new_node->nkeys = 0;
	new_node->parent = NULL;
	new_node->next = NULL;
	return new_node;
}

node* make_leaf( void ) {
	node* leaf = make_node();
	leaf->is_leaf = true;
	return leaf;
}

int get_lindex(node* parent, node* left) {

	int left_index = 0;
	while (left_index <= parent->nkeys && 
			parent->pointers[left_index] != left)
		left_index++;
	return left_index;
}

node* insert_into_leaf( node* leaf, int key, record* pointer ) {

	int i, insertion_point;

	insertion_point = 0;
	while (insertion_point < leaf->nkeys && leaf->keys[insertion_point] < key)
		insertion_point++;

	for (i = leaf->nkeys; i > insertion_point; i--) {
		leaf->keys[i] = leaf->keys[i - 1];
		leaf->pointers[i] = leaf->pointers[i - 1];
	}
	leaf->keys[insertion_point] = key;
	leaf->pointers[insertion_point] = pointer;
	leaf->nkeys++;
	return leaf;
}

node* insert_leaf_split(node* root, node* leaf, int key, record* pointer) {
	node* new_leaf;
	int* temp_keys;
	void** temp_pointers;
	int insertion_index, split, new_key, i, j;
	new_leaf = make_leaf();
	temp_keys = malloc( order * sizeof(int) );
	if (temp_keys == NULL) {
		perror("Temporary keys array.");
		exit(EXIT_FAILURE);
	}
	temp_pointers = malloc( order * sizeof(void*) );
	if (temp_pointers == NULL) {
		perror("Temporary pointers array.");
		exit(EXIT_FAILURE);
	}
	insertion_index = 0;
	while (insertion_index < order - 1 && leaf->keys[insertion_index] < key)
		insertion_index++;
	for (i = 0, j = 0; i < leaf->nkeys; i++, j++) {
		if (j == insertion_index) j++;
		temp_keys[j] = leaf->keys[i];
		temp_pointers[j] = leaf->pointers[i];
	}
	temp_keys[insertion_index] = key;
	temp_pointers[insertion_index] = pointer;
	leaf->nkeys = 0;
	split = cut(order - 1);
	for (i = 0; i < split; i++) {
		leaf->pointers[i] = temp_pointers[i];
		leaf->keys[i] = temp_keys[i];
		leaf->nkeys++;
	}
	for (i = split, j = 0; i < order; i++, j++) {
		new_leaf->pointers[j] = temp_pointers[i];
		new_leaf->keys[j] = temp_keys[i];
		new_leaf->nkeys++;
	}
	free(temp_pointers);
	free(temp_keys);
	new_leaf->pointers[order - 1] = leaf->pointers[order - 1];
	leaf->pointers[order - 1] = new_leaf;
	for (i = leaf->nkeys; i < order - 1; i++)
		leaf->pointers[i] = NULL;
	for (i = new_leaf->nkeys; i < order - 1; i++)
		new_leaf->pointers[i] = NULL;
	new_leaf->parent = leaf->parent;
	new_key = new_leaf->keys[0];
	return insert_into_parent(root, leaf, new_key, new_leaf);
}

node* insert_into_node(node* root, node* n, 
		int left_index, int key, node* right) {
	int i;

	for (i = n->nkeys; i > left_index; i--) {
		n->pointers[i + 1] = n->pointers[i];
		n->keys[i] = n->keys[i - 1];
	}
	n->pointers[left_index + 1] = right;
	n->keys[left_index] = key;
	n->nkeys++;
	return root;
}

node* insert_node_split(node* root, node* old_node, int left_index, 
		int key, node* right) {
	int i, j, split, k_prime;
	node* new_node, * child;
	int* temp_keys;
	node** temp_pointers;
	temp_pointers = malloc( (order + 1) * sizeof(node*) );
	if (temp_pointers == NULL) {
		perror("Temporary pointers array for splitting nodes.");
		exit(EXIT_FAILURE);
	}
	temp_keys = malloc( order * sizeof(int) );
	if (temp_keys == NULL) {
		perror("Temporary keys array for splitting nodes.");
		exit(EXIT_FAILURE);
	}
	for (i = 0, j = 0; i < old_node->nkeys + 1; i++, j++) {
		if (j == left_index + 1) j++;
		temp_pointers[j] = old_node->pointers[i];
	}
	for (i = 0, j = 0; i < old_node->nkeys; i++, j++) {
		if (j == left_index) j++;
		temp_keys[j] = old_node->keys[i];
	}
	temp_pointers[left_index + 1] = right;
	temp_keys[left_index] = key; 
	split = cut(order);
	new_node = make_node();
	old_node->nkeys = 0;
	for (i = 0; i < split - 1; i++) {
		old_node->pointers[i] = temp_pointers[i];
		old_node->keys[i] = temp_keys[i];
		old_node->nkeys++;
	}
	old_node->pointers[i] = temp_pointers[i];
	k_prime = temp_keys[split - 1];
	for (++i, j = 0; i < order; i++, j++) {
		new_node->pointers[j] = temp_pointers[i];
		new_node->keys[j] = temp_keys[i];
		new_node->nkeys++;
	}
	new_node->pointers[j] = temp_pointers[i];
	free(temp_pointers);
	free(temp_keys);
	new_node->parent = old_node->parent;
	for (i = 0; i <= new_node->nkeys; i++) {
		child = new_node->pointers[i];
		child->parent = new_node;
	}
	return insert_into_parent(root, old_node, k_prime, new_node);
}

node* insert_into_new_root(node* left, int key, node* right) {
	node* root = make_node();
	root->keys[0] = key;
	root->pointers[0] = left;
	root->pointers[1] = right;
	root->nkeys++;
	root->parent = NULL;
	left->parent = root;
	right->parent = root;
	return root;
}

node* insert_into_parent(node* root, node* left, int key, node* right) {
	int left_index;
	node* parent;
	parent = left->parent;
	if (parent == NULL)
		return insert_into_new_root(left, key, right);
	left_index = get_lindex(parent, left);
	if (parent->nkeys < order - 1)
		return insert_into_node(root, parent, left_index, key, right);
	return insert_node_split(root, parent, left_index, key, right);
}

node* start_new(int key, record* pointer) {
	node* root = make_leaf();
	root->keys[0] = key;
	root->pointers[0] = pointer;
	root->pointers[order - 1] = NULL;
	root->parent = NULL;
	root->nkeys++;
	return root;
}

node* insert( node* root, int key, int value ) {
	record* pointer;
	node* leaf;
	if (find(root, key) != NULL)
		return root;
	pointer = make_record(value);
	if (root == NULL) 
		return start_new(key, pointer);
	leaf = find_leaf(root, key);
	if (leaf->nkeys < order - 1) {
		leaf = insert_into_leaf(leaf, key, pointer);
		return root;
	}
	return insert_leaf_split(root, leaf, key, pointer);
}

int get_neighbor( node* n ) {
	int i;
	for (i = 0; i <= n->parent->nkeys; i++)
		if (n->parent->pointers[i] == n)
			return i - 1;
	printf("Search for nonexistent pointer to node in parent.\n");
	printf("Node:  %#lx\n", (unsigned long)n);
	exit(EXIT_FAILURE);
}

node* remove_entry_node(node* n, int key, node* pointer) {
	int i, num_pointers;
	i = 0;
	while (n->keys[i] != key)
		i++;
	for (++i; i < n->nkeys; i++)
		n->keys[i - 1] = n->keys[i];
	num_pointers = n->is_leaf ? n->nkeys : n->nkeys + 1;
	i = 0;
	while (n->pointers[i] != pointer)
		i++;
	for (++i; i < num_pointers; i++)
		n->pointers[i - 1] = n->pointers[i];
	n->nkeys--;
	if (n->is_leaf)
		for (i = n->nkeys; i < order - 1; i++)
			n->pointers[i] = NULL;
	else
		for (i = n->nkeys + 1; i < order; i++)
			n->pointers[i] = NULL;
	return n;
}

node* root_fix(node* root) {
	node* new_root;
	if (root->nkeys > 0)
		return root;
	if (!root->is_leaf) {
		new_root = root->pointers[0];
		new_root->parent = NULL;
	}
	else
		new_root = NULL;
	free(root->keys);
	free(root->pointers);
	free(root);
	return new_root;
}

node* coalesce_nodes(node* root, node* n, node* neighbor, int neighbor_index, int k_prime) {
	int i, j, neighbor_insertion_index, n_start, n_end, new_k_prime;
	node* tmp;
	bool split;
	if (neighbor_index == -1) {
		tmp = n;
		n = neighbor;
		neighbor = tmp;
	}
	neighbor_insertion_index = neighbor->nkeys;
	split = false;
	if (!n->is_leaf) {
		neighbor->keys[neighbor_insertion_index] = k_prime;
		neighbor->nkeys++;
		n_end = n->nkeys;
		n_start = 0; 
		if (n->nkeys + neighbor->nkeys >= order) {
			split = true;
			n_end = cut(order) - 2;
		}
		for (i = neighbor_insertion_index + 1, j = 0; j < n_end; i++, j++) {
			neighbor->keys[i] = n->keys[j];
			neighbor->pointers[i] = n->pointers[j];
			neighbor->nkeys++;
			n->nkeys--;
			n_start++;
		}
		neighbor->pointers[i] = n->pointers[j];
		if (split) {
			new_k_prime = n->keys[n_start];
			for (i = 0, j = n_start + 1; i < n->nkeys; i++, j++) {
				n->keys[i] = n->keys[j];
				n->pointers[i] = n->pointers[j];
			}
			n->pointers[i] = n->pointers[j];
			n->nkeys--;
		}
		for (i = 0; i < neighbor->nkeys + 1; i++) {
			tmp = (node*)neighbor->pointers[i];
			tmp->parent = neighbor;
		}
	}
	else {
		for (i = neighbor_insertion_index, j = 0; j < n->nkeys; i++, j++) {
			neighbor->keys[i] = n->keys[j];
			neighbor->pointers[i] = n->pointers[j];
			neighbor->nkeys++;
		}
		neighbor->pointers[order - 1] = n->pointers[order - 1];
	}
	if (!split) {
		root = delete_entry(root, n->parent, k_prime, n);
		free(n->keys);
		free(n->pointers);
		free(n); 
	}
	else
		for (i = 0; i < n->parent->nkeys; i++)
			if (n->parent->pointers[i + 1] == n) {
				n->parent->keys[i] = new_k_prime;
				break;
			}
	return root;
}

node* redistribute_nodes(node* root, node* n, node* neighbor, int neighbor_index, 
		int k_prime_index, int k_prime) {  
	int i;
	node* tmp;
	if (neighbor_index != -1) {
		if (!n->is_leaf)
			n->pointers[n->nkeys + 1] = n->pointers[n->nkeys];
		for (i = n->nkeys; i > 0; i--) {
			n->keys[i] = n->keys[i - 1];
			n->pointers[i] = n->pointers[i - 1];
		}
		if (!n->is_leaf) {
			n->pointers[0] = neighbor->pointers[neighbor->nkeys];
			tmp = (node*)n->pointers[0];
			tmp->parent = n;
			neighbor->pointers[neighbor->nkeys] = NULL;
			n->keys[0] = k_prime;
			n->parent->keys[k_prime_index] = neighbor->keys[neighbor->nkeys - 1];
		}
		else {
			n->pointers[0] = neighbor->pointers[neighbor->nkeys - 1];
			neighbor->pointers[neighbor->nkeys - 1] = NULL;
			n->keys[0] = neighbor->keys[neighbor->nkeys - 1];
			n->parent->keys[k_prime_index] = n->keys[0];
		}
	}
	else {  
		if (n->is_leaf) {
			n->keys[n->nkeys] = neighbor->keys[0];
			n->pointers[n->nkeys] = neighbor->pointers[0];
			n->parent->keys[k_prime_index] = neighbor->keys[1];
		}
		else {
			n->keys[n->nkeys] = k_prime;
			n->pointers[n->nkeys + 1] = neighbor->pointers[0];
			tmp = (node*)n->pointers[n->nkeys + 1];
			tmp->parent = n;
			n->parent->keys[k_prime_index] = neighbor->keys[0];
		}
		for (i = 0; i < neighbor->nkeys; i++) {
			neighbor->keys[i] = neighbor->keys[i + 1];
			neighbor->pointers[i] = neighbor->pointers[i + 1];
		}
		if (!n->is_leaf)
			neighbor->pointers[i] = neighbor->pointers[i + 1];
	}
	n->nkeys++;
	neighbor->nkeys--;
	return root;
}

node* delete_entry( node* root, node* n, int key, void* pointer ) {
	int min_keys;
	node* neighbor;
	int neighbor_index;
	int k_prime_index, k_prime;
	int capacity;
	n = remove_entry_node(n, key, pointer);
	if (n == root) 
		return root_fix(root);
	min_keys = n->is_leaf ? cut(order - 1) : cut(order) - 1;
	if (n->nkeys >= min_keys)
		return root;
	neighbor_index = get_neighbor( n );
	k_prime_index = neighbor_index == -1 ? 0 : neighbor_index;
	k_prime = n->parent->keys[k_prime_index];
	neighbor = neighbor_index == -1 ? n->parent->pointers[1] : 
		n->parent->pointers[neighbor_index];
	capacity = n->is_leaf ? order : order - 1;
	if (neighbor->nkeys + n->nkeys < capacity)
		return coalesce_nodes(root, n, neighbor, neighbor_index, k_prime);
	else
		return redistribute_nodes(root, n, neighbor, neighbor_index, k_prime_index, k_prime);
}

node* delete(node* root, int key) {
	node* kleaf;
	record* krecord;
	krecord = find(root, key);
	kleaf = find_leaf(root, key);
	if (krecord != NULL && kleaf != NULL) {
		root = delete_entry(root, kleaf, key, krecord);
		free(krecord);
	}
	return root;
}

void destroy_tree_nodes(node* root) {
	int i;
	if (root->is_leaf)
		for (i = 0; i < root->nkeys; i++)
			free(root->pointers[i]);
	else
		for (i = 0; i < root->nkeys + 1; i++)
			destroy_tree_nodes(root->pointers[i]);
	free(root->pointers);
	free(root->keys);
	free(root);
}

node* destroy_tree(node* root) {
	destroy_tree_nodes(root);
	return NULL;
}

void usagecmd( void ) {
	printf("Enter any of the following commands after the prompt > :\n");
	printf("\ti <k> -> Insert <k> (an integer) as both key and value).\n");
	printf("\tm <k> -> Insert <k> integers as both key and value).\n");
	printf("\tf <k> -> Find the value under key <k>.\n");
	printf("\td <k> -> Delete key <k> and its associated value.\n");
	printf("\tt -> Print the B+ tree.\n");
	printf("\tq -> Quit. (Or use Ctl-D.)\n");
	printf("\tEnter to continue\n");
}



void input_function(){
  char input,blank,dump;
  int position,i,key;
  node *root;
  root = NULL;
  printf("> ");
  
  while (fscanf(fp,"%c",&input) != EOF) { 
    switch (input) {
      case 'q' : exit(0);
      case 'f' : fscanf(fp,"%c%d%c",&blank,&key,&dump);
                 printf("FIND: %d\n",key);
                 find_and_print(root, key);
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
  	node* root;
	int input;
	char instruction;
	root = NULL;
	printf("\nDefault order = 4\nEnter order where %d <= order <= %d :  ", MIN_ORDER, MAX_ORDER);
	scanf("%d",&order);
	if (order < MIN_ORDER || order > MAX_ORDER) {
		fprintf(stderr, "Invalid order: %d .\n\n", order);
		exit(EXIT_FAILURE);
	}
	input_function();
  	printf("Exiting\n");
  	return 0; 
}
