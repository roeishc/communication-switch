# communication-switch
Implemented a communication switch, with 4 input ports and 4 output ports, simulated by text files.

The main program receives the 5 files as input:<br>
In Visual Studio, right-click the project -> properties -> under configuration properties: debugging -> command arguments: route.txt port1.in.txt port2.in.txt port3.in.txt port4.in.txt

In the input data files, portX.in, each row represesnts a data packet in the following format:
Time, Da, Sa, Prio, Data_length, Data, Checksum
* Time - unsigned int. Packet's arrival time,
* Da - byte (unsgined char). Destination address.
* Sa - byte (unsgined char). Source address.
* Prio - byte (unsgined char). Packet's priority, with 0 being highest priority, 1 being lowest priority.
* Data_length - byte (unsgined char). Data array's length (number of elements in the array).
* Checksum - byte (unsgined char). Calculated as a bitwise XOR operation between all the bits in the packet, except for the Time field.

In each portX.in file, the data packets are sorted by time of arrival.

The switch routes the data packets from the input ports to the output ports by using a routing table represented as a binary search tree. Each node in the tree (S_node) is defined as a structure with the following fields: <br>
`typedef struct route_node{`<br>
  `char da;`
  `char output_port;`
  `struct route_node *left;`
  `struct route_node *right;`<br>
`}S_node;`

In each portX.out file,the data packets are primarily sorted by priority ("prio") and secondarily sorted by time.
