//types
typedef unsigned char byte;
typedef enum Bool {
	False, True
} Bool;

//structures
typedef struct {
	unsigned int time;
	byte da;
	byte sa;
	byte prio;
	byte data_length;
	byte* data;
	byte checksum;
} packet;
typedef struct pkt_node {
	packet* pkt;
	struct pkt_node* next;
} S_pkt;
typedef struct Out_Qs_mgr {
	struct pkt_node* head_p1;
	struct pkt_node* tail_p1;
	struct pkt_node* head_p0;
	struct pkt_node* tail_p0;
} S_Out_Qs_mgr;

//functions declarations
void packetRead(FILE* fp, packet* pkt);
void packetWrite(FILE* fp, const packet* pkt);
Bool checksumCheck(const packet* pkt);
void enqueue_pkt(S_Out_Qs_mgr* QM_ptr, packet* pkt);
S_pkt* getPktOneBeforeHead(S_pkt* tail);
packet* dequeue_pkt(S_Out_Qs_mgr* QM_ptr, char priority);