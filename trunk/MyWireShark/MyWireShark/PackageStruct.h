#include <pcap.h>
#define ETHERTYPE_IPV6 0x86dd /* ip protocol */
#define ETHERTYPE_IPV4 0x0800 /* ip protocol */
#define TCP_PROTOCOL 0x0600 /* tcp protocol */
#define UDP_PROTOCOL 0x1100 /* tcp protocol */
#define ARP_PROTOCOL 0x0806 /* arp protocol*/
#define ETHER_ADDR_LEN 6

typedef struct ip_address
{
    u_char byte1;
    u_char byte2;
    u_char byte3;
    u_char byte4;
}ip_address;

typedef struct ip_header
{
    u_char  ver_ihl;        // �汾 (4 bits) + �ײ����� (4 bits)
    u_char  tos;            // ��������(Type of service) 
    u_short tlen;           // �ܳ�(Total length) 
    u_short identification; // ��ʶ(Identification)
    u_short flags_fo;       // ��־λ(Flags) (3 bits) + ��ƫ����(Fragment offset) (13 bits)
    u_char  ttl;            // ���ʱ��(Time to live)
    u_char  proto;          // Э��(Protocol)
    u_short crc;            // �ײ�У���(Header checksum)
    ip_address  saddr;      // Դ��ַ(Source address)
    ip_address  daddr;      // Ŀ�ĵ�ַ(Destination address)
    u_int   op_pad;         // ѡ�������(Option + Padding)
}ip_header;
typedef struct ip6_header
{
	unsigned   char   first_4[4];	//ǰ4���ֽڣ������汾(4λ)����������(8λ)������ǩ(20λ)  
	unsigned   short  payload_len;  //�غɳ���16λ
	unsigned   char   next_hdr;		//��һ����ͷ8λ
	unsigned   char   hop_limit;	//��������8λ
	struct   in6_addr   ip_src;		//Դ��ַ128λ
	struct   in6_addr   ip_dst;		//Ŀ�ĵ�ַ128λ   
}ip6_header;

typedef struct udp_header
{
    u_short sport;          // Դ�˿�(Source port)
    u_short dport;          // Ŀ�Ķ˿�(Destination port)
    u_short len;            // UDP���ݰ�����(Datagram length)
    u_short crc;            // У���(Checksum)
}udp_header;

typedef struct tcp_header
{
    u_short th_sport;         /* source port */
    u_short th_dport;         /* destination port */
    u_int th_seq;             /* sequence number */
    u_int th_ack;             /* acknowledgement number */
    u_short th_len_resv_code; /* datagram length and reserved code */
    u_short th_window;        /* window */
    u_short th_sum;           /* checksum */
    u_short th_urp;           /* urgent pointer */
}tcp_header;

typedef struct ether_header 
{
	u_char ether_shost[ETHER_ADDR_LEN]; /* source ethernet address, 8 bytes */
	u_char ether_dhost[ETHER_ADDR_LEN]; /* destination ethernet addresss, 8 bytes */
    u_short ether_type;                 /* ethernet type, 16 bytes */
}ether_header;

typedef struct arp_header
{
	u_short hard_type;
	u_short protocol_type; 
	u_char	hard_size;
	u_char	protocol_size;
	u_short opcode;
	u_char ether_shost[ETHER_ADDR_LEN];
	ip_address  saddr;
	u_char ether_dhost[ETHER_ADDR_LEN];
	ip_address  daddr; 
}arp_header;