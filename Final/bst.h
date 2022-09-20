//libraries
#include "pkt.h"

//structures
typedef struct route_node {
	byte da;
	byte output_port;
	struct route_node *left;
	struct route_node *right;
} S_node;
typedef struct {
	byte command;
	int da;
	byte port;
} operation;

//functions declarations
S_node* createNode(char da, char output_port);
S_node* addRoute(S_node* root, char da, char output_port);
S_node* minimumInBST(S_node* root);
S_node* deleteRoute(S_node* root, char da);
S_node* searchRoute(const S_node* root, char da);
void printRoutingTable(const S_node* root);
S_node* buildRouteTable(FILE* fp, S_node* root);