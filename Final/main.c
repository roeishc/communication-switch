#include "main.h"

void main(int argc, char* argv[]) {
	//define required variables and arrays
	ports_manager ports_arr[PORTS];
	S_Out_Qs_mgr queues_arr[PORTS] = { NULL };
	FILE* route;
	packet* new_packet;
	int times_size = 0, i, j, k = 0, m, n;
	unsigned int* times_arr;

	//opening route file, create a BST accordingly and print
	assert(route = fopen(argv[1], "r"));
	S_node* root = NULL;
	printf("Binary sorted tree according to route.txt:\n\n");
	root = buildRouteTable(route, root);

	//opening ports and creating arrays for the times
	for (i = 0; i < PORTS; i++) {
		assert(ports_arr[i].fp = fopen(argv[i + 2], "r"));
		ports_arr[i].times_arr = getTimesArr(ports_arr[i].fp, &(ports_arr[i].times_arr_size));
		times_size += ports_arr[i].times_arr_size;
	}

	//create one array for all times and sort
	assert(times_arr = (unsigned int*)malloc(sizeof(unsigned int)*times_size));
	for (i = 0; i < PORTS; i++) {
		for (j = 0; j < ports_arr[i].times_arr_size; j++) {
			times_arr[k] = ports_arr[i].times_arr[j];
			k++;
		}
	}
	sortArray(times_arr, times_size);

	//enqueue according to times to each queue
	S_node* temp = NULL;
	int idx;
	i = j = k = m = n = 0;

	while (i < times_size) {
		//create new packet to insert
		assert(new_packet = (packet*)malloc(sizeof(packet)));
		//checking for port1
		if (times_arr[i] == ports_arr[0].times_arr[j]) {
			fseek(ports_arr[0].fp, SEEK_SET, 0);
			//find the required packet in the port
			while (times_arr[i] != new_packet->time)
				packetRead(ports_arr[0].fp, new_packet);
			//if da is in the BST, enqueue
			temp = searchRoute(root, new_packet->da);
			if (temp) {
				idx = atoi(&(temp->output_port));
				idx--;
				enqueue_pkt(&(queues_arr[idx]), new_packet);
			}
			//if da isn't in the BST, free the allocated memory
			else {
				free(new_packet->data);
				free(new_packet);
			}
			j++;
			i++;
			continue;
		}
		//checking for port2
		if (times_arr[i] == ports_arr[1].times_arr[k]) {
			fseek(ports_arr[1].fp, SEEK_SET, 0);
			//find the required packet in the port
			while (times_arr[i] != new_packet->time)
				packetRead(ports_arr[1].fp, new_packet);
			//if da is in the BST, enqueue
			temp = searchRoute(root, new_packet->da);
			if (temp) {
				idx = atoi(&(temp->output_port));
				idx--;
				enqueue_pkt(&(queues_arr[idx]), new_packet);
			}
			//if da isn't in the BST, free the allocated memory
			else {
				free(new_packet->data);
				free(new_packet);
			}
			k++;
			i++;
			continue;
		}
		//checking for port3
		if (times_arr[i] == ports_arr[2].times_arr[m]) {
			fseek(ports_arr[2].fp, SEEK_SET, 0);
			//find the required packet in the port
			while (times_arr[i] != new_packet->time)
				packetRead(ports_arr[2].fp, new_packet);
			//if da is in the BST, enqueue
			temp = searchRoute(root, new_packet->da);
			if (temp) {
				idx = atoi(&(temp->output_port));
				idx--;
				enqueue_pkt(&(queues_arr[idx]), new_packet);
			}
			//if da isn't in the BST, free the allocated memory
			else {
				free(new_packet->data);
				free(new_packet);
			}
			m++;
			i++;
			continue;
		}
		//checking for port4
		if (times_arr[i] == ports_arr[3].times_arr[n]) {
			fseek(ports_arr[3].fp, SEEK_SET, 0);
			//find the required packet in the port
			while (times_arr[i] != new_packet->time)
				packetRead(ports_arr[3].fp, new_packet);
			//if da is in the BST, enqueue
			temp = searchRoute(root, new_packet->da);
			if (temp) {
				idx = atoi(&(temp->output_port));
				idx--;
				enqueue_pkt(&(queues_arr[idx]), new_packet);
			}
			//if da isn't in the BST, free the allocated memory
			else {
				free(new_packet->data);
				free(new_packet);
			}
			n++;
			i++;
			continue;
		}
	}

	//create 4 new files for ports out
	FILE* new_ports[PORTS] = { NULL };
	assert(new_ports[0] = fopen("port1.out.txt", "w"));
	assert(new_ports[1] = fopen("port2.out.txt", "w"));
	assert(new_ports[2] = fopen("port3.out.txt", "w"));
	assert(new_ports[3] = fopen("port4.out.txt", "w"));

	packet* temp1 = (packet*)1, *temp2 = (packet*)1;
	for (i = 0; i < PORTS; i++) {
		temp1 = (packet*)1;
		//prio 0 queue
		while (temp2) {
			temp2 = dequeue_pkt(&queues_arr[i], 0);
			if (temp2 == NULL) break;
			packetWrite(new_ports[i], temp2);
			free(temp2->data);
			free(temp2);
		}
		temp2 = (packet*)1;
		//prio 1 queue
		while (temp1) {
			temp1 = dequeue_pkt(&queues_arr[i], 1);
			if (temp1 == NULL) break;
			packetWrite(new_ports[i], temp1);
			free(temp1->data);
			free(temp1);
		}
	}
	

	//closing files, freeing allocated memory
	for (i = 0; i < PORTS; i++) {
		//ports in
		fclose(ports_arr[i].fp);
		//ports out
		fclose(new_ports[i]);
		//times arrays
		free(ports_arr[i].times_arr);
	}
	//BST
	while (root)
		root = deleteRoute(root, root->da);
	//route file
	fclose(route);
	//array of all times
	free(times_arr);
}