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
    u_char  ver_ihl;        // 版本 (4 bits) + 首部长度 (4 bits)
    u_char  tos;            // 服务类型(Type of service) 
    u_short tlen;           // 总长(Total length) 
    u_short identification; // 标识(Identification)
    u_short flags_fo;       // 标志位(Flags) (3 bits) + 段偏移量(Fragment offset) (13 bits)
    u_char  ttl;            // 存活时间(Time to live)
    u_char  proto;          // 协议(Protocol)
    u_short crc;            // 首部校验和(Header checksum)
    ip_address  saddr;      // 源地址(Source address)
    ip_address  daddr;      // 目的地址(Destination address)
    u_int   op_pad;         // 选项与填充(Option + Padding)
}ip_header;
typedef struct ip6_header
{
	unsigned   char   first_4[4];	//前4个字节，包含版本(4位)、流量类型(8位)、流标签(20位)  
	unsigned   short  payload_len;  //载荷长度16位
	unsigned   char   next_hdr;		//下一个报头8位
	unsigned   char   hop_limit;	//跳数限制8位
	struct   in6_addr   ip_src;		//源地址128位
	struct   in6_addr   ip_dst;		//目的地址128位   
}ip6_header;

typedef struct udp_header
{
    u_short sport;          // 源端口(Source port)
    u_short dport;          // 目的端口(Destination port)
    u_short len;            // UDP数据包长度(Datagram length)
    u_short crc;            // 校验和(Checksum)
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