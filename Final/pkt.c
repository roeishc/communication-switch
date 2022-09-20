#include "main.h"

//functions definitions
void packetRead(FILE *fp, packet *pkt) {
	if (!fp)
		return;
	if (!pkt)
		return;
	int num;
	//scan the fields
	fscanf(fp, "%u %d %d %d %d ", &(pkt->time), (int*)&(pkt->da), (int*)&(pkt->sa), (int*)&(pkt->prio), (int*)&(pkt->data_length));
	//allocate memory for data according to data length
	int size = pkt->data_length;
	assert(pkt->data = (byte*)malloc(size * sizeof(byte)));
	//finish scanning
	for (byte i = 0; i < pkt->data_length; i++) {
		fscanf(fp, "%d ", &num);
		pkt->data[i] = (byte)num;
	}
	fscanf(fp, "%d\n", (int*)&(pkt->checksum));
}
void packetWrite(FILE *fp, const packet *pkt) {
	if (!fp)
		return;
	if (!pkt)
		return;
	//print the fields to the file
	fprintf(fp, "%u %d %d %d %d ", pkt->time, pkt->da, pkt->sa, pkt->prio, pkt->data_length);
	//finish scanning
	for (byte i = 0; i < pkt->data_length; i++)
		fprintf(fp, "%d ", pkt->data[(int)i]);
	fprintf(fp, "%d\n", pkt->checksum);
}
Bool checksumCheck(const packet *pkt) {
	byte checksum_check;
	//bitwise xor with all elements
	checksum_check = pkt->da ^ pkt->sa;
	checksum_check ^= pkt->prio;
	checksum_check ^= pkt->data_length;
	for (byte i = 0; i < pkt->data_length; i++)
		checksum_check ^= pkt->data[i];
	//check if checksum is valid
	if (checksum_check == pkt->checksum)
		return True;
	return False;
}
void enqueue_pkt(S_Out_Qs_mgr* QM_ptr, packet* pkt) {
	if (!pkt)
		return;
	if (!QM_ptr)
		return;
	//check for checksum
	if (checksumCheck(pkt) == True) {
		//create new node for the queue
		S_pkt* new_node;
		assert(new_node = (S_pkt*)calloc(1, sizeof(S_pkt)));
		new_node->pkt = pkt;
		//prio 0 case
		if (pkt->prio == 0) {
			//p0 is an empty queue
			if (QM_ptr->head_p0 == NULL && QM_ptr->tail_p0 == NULL) {
				QM_ptr->head_p0 = new_node;
				QM_ptr->tail_p0 = new_node;
			}
			//p0 isn't empty
			else {
				new_node->next = QM_ptr->tail_p0;
				QM_ptr->tail_p0 = new_node;
			}
			return;
		}
		//prio 1 case
		else if (pkt->prio == 1) {
			//p1 is an empty queue
			if (QM_ptr->head_p1 == NULL && QM_ptr->tail_p1 == NULL) {
				QM_ptr->head_p1 = new_node;
				QM_ptr->tail_p1 = new_node;
			}
			//p1 isn't empty
			else {
				new_node->next = QM_ptr->tail_p1;
				QM_ptr->tail_p1 = new_node;
			}
		}
		else {
			free(new_node);
			return;
		}
	}
	//if checksum isn't valid, free the allocated memory
	else {
		free(pkt->data);
		free(pkt);
		return;
	}
}
S_pkt* getPktOneBeforeHead(S_pkt* tail) {
	S_pkt* temp = tail;
	while (temp->next->next)
		temp = temp->next;
	return temp;
}
packet* dequeue_pkt(S_Out_Qs_mgr* QM_ptr, char priority) {
	if (!QM_ptr)
		return NULL;
	//priority 1 queue
	packet* temp;
	if (priority) {
		//save the required packet
		if (QM_ptr->head_p1)
			temp = QM_ptr->head_p1->pkt;
		else
			return NULL;
		//prio 1 queue has 1 node
		if (QM_ptr->tail_p1 == QM_ptr->head_p1) {
			free(QM_ptr->head_p1);
			QM_ptr->tail_p1 = QM_ptr->head_p1 = NULL;
		}
		//prio 1 queue has more than 1 node
		else {
			//find new head
			S_pkt* new_head = getPktOneBeforeHead(QM_ptr->tail_p1);
			//free the node containing the required packet (the previous head)
			free(QM_ptr->head_p1);
			//attach new head
			QM_ptr->head_p1 = new_head;
			new_head->next = NULL;
		}
	}
	//priority 0 queue
	else {
		//save the required packet
		if (QM_ptr->head_p0)
			temp = QM_ptr->head_p0->pkt;
		else
			return NULL;
		//prio 0 queue has 1 node
		if (QM_ptr->tail_p0 == QM_ptr->head_p0) {
			free(QM_ptr->head_p0);
			QM_ptr->tail_p0 = QM_ptr->head_p0 = NULL;
		}
		//prio 0 queue has more than 1 node
		else {
			//find new head
			S_pkt* new_head = getPktOneBeforeHead(QM_ptr->tail_p0);
			//free the node containing the required packet (the previous head)
			free(QM_ptr->head_p0);
			//attach new head
			QM_ptr->head_p0 = new_head;
			new_head->next = NULL;
		}
	}
	return temp;
}