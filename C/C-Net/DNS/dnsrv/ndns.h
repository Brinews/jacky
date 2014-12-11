//Types of DNS resource records :)

#define T_A 1 //Ipv4 address
#define T_NS 2 //Nameserver
#define T_CNAME 5 // canonical name
#define T_SOA 6 /* start of authority zone */
#define T_PTR 12 /* domain name pointer */
#define T_MX 15 //Mail server

//DNS header structure
struct DNS_HEADER
{
	unsigned short id; // identification number

	char  rd :1; // recursion desired
	char  tc :1; // truncated message
	char  aa :1; // authoritive answer
	char  opcode :4; // purpose of message
	char  qr :1; // query/response flag

	char  rcode :4; // response code
	char  cd :1; // checking disabled
	char  ad :1; // authenticated data
	char  z :1; // its z! reserved
	char  ra :1; // recursion available

	unsigned short q_count; // number of question entries
	unsigned short ans_count; // number of answer entries
	unsigned short auth_count; // number of authority entries
	unsigned short add_count; // number of resource entries
};

//Constant sized fields of query structure
struct QUESTION
{
	unsigned short qtype;
	unsigned short qclass;
};

//Constant sized fields of the resource record structure
#pragma pack(push, 1)
struct R_DATA
{
	unsigned short type;
	unsigned short _class;
	unsigned int ttl;
	unsigned short data_len;
};
#pragma pack(pop)

//Pointers to resource record contents
struct RES_RECORD
{
	char  *name;
	struct R_DATA *resource;
	char  *rdata;
};

//Structure of a Query
typedef struct
{
	char  *name;
	struct QUESTION *ques;
} QUERY;
