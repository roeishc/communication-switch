#include "main.h"

//functions definitions
S_node* createNode(char da, char output_port) {
	S_node* ptr;
	assert(ptr = (S_node*)calloc(1, sizeof(S_node)));
	ptr->da = da;
	ptr->output_port = output_port;
	return ptr;
}
S_node* addRoute(S_node *root, char da, char output_port) {
	byte DA = (byte)da;
	//if root is NULL ptr, create a node
	if (!root) {
		root = createNode(DA, output_port);
		return root;
	}
	//search recursively for the placement and add a node
	//left subtree
	if ((byte)(root->da) > DA) {
		if (root->left)
			addRoute(root->left, DA, output_port);
		else
			root->left = createNode(DA, output_port);
	}
	//right subtree
	if ((byte)(root->da) < DA) {
		if (root->right)
			addRoute(root->right, DA, output_port);
		else
			root->right = createNode(DA, output_port);
	}
	//in case of repeats, no changes are made
	return root;
}
S_node* minimumInBST(S_node* root) {
	S_node* current = root;
	while (current && current->left != NULL)
		current = current->left;
	return current;
}
S_node* deleteRoute(S_node *root, char da) {
	//if root is NULL ptr, return
	if (!root)
		return NULL;
	byte DA = (byte)da;
	//search recursively for the address
	//left subtree
	if (root->da > DA)
		root->left = deleteRoute(root->left, DA);
	//right subtree
	if (root->da < DA)
		root->right = deleteRoute(root->right, DA);
	//da is found
	if (root->da == DA) {
		//right child only
		if (!(root->left)) {
			S_node* temp = root->right;
			free(root);
			return temp;
		}
		//left child only
		if (!(root->right)) {
			S_node* temp = root->left;
			free(root);
			return temp;
		}
		//two children - find minimum in the right subtree and replace
		//the original root's value with the minimum.
		//then, recursively search for the minimum in the subtree to remove it.
		S_node* temp = minimumInBST(root->right);
		root->da = temp->da;
		root->output_port = temp->output_port;
		root->right = deleteRoute(root->right, temp->da);
	}
	return root;
}
S_node* searchRoute(const S_node* root, char da) {
	//if root is NULL ptr, return
	if (!root)
		return NULL;
	byte DA = (byte)da;
	//root has value of da
	if (root->da == DA)
		return (S_node*)root;
	//search recursively for the address
	S_node* temp = NULL;
	//left subtree
	if (root->da > DA)
		temp = searchRoute(root->left, DA);
	//right subtree
	if (root->da < DA)
		temp = searchRoute(root->right, DA);
	//da is found
	if (temp && temp->da == DA)
		return temp;
	else
		return NULL;
}
void printRoutingTable(const S_node* root) {
	if (!root)
		return;
	printRoutingTable(root->left);
	printf("da: %d, portout: %c.\n", (byte)root->da, root->output_port);
	printRoutingTable(root->right);
}
S_node* buildRouteTable(FILE* fp, S_node* root) {
	if (!fp)
		return NULL;
	//check for operation
	operation op_node;
	while (fscanf(fp, "%c ", &op_node.command) != EOF) {
		//addition
		if (op_node.command == 'a') {
			fscanf(fp, "%d %c\n", &(op_node.da), &(op_node.port));
			root = addRoute(root, op_node.da, op_node.port);
		}
		//deletion
		else if (op_node.command == 'd') {
			fscanf(fp, "%d\n", &(op_node.da));
			root = deleteRoute(root, op_node.da);
		}
		//invalid input, no changes are made
		else
			return root;
	}
	printRoutingTable(root);
	return root;
}