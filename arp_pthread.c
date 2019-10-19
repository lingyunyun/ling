#include <arpa/inet.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/in.h>
#include <stdio.h>
#include<netpacket/packet.h>
#include<pthread.h>
#include<netinet/ether.h>
#include<net/if.h>
#include<sys/ioctl.h>

#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "get_interface.h"


void *arp_pthread(void *fd)//将ARP请求的mac地址存到链表中
{
	int socket_raw_fd = (int)fd;
	INTERFACE_INF *p_new=NULL,*p_search;
	int i=0;
	for(i=0;i<Infrom->n_num;i++)
	{
		interface_inf1=head;
		
		if(memcmp(net_interface[i].ip,Infrom->recv_msg+38,3) == 0&&Infrom->recv_msg[21] == 2)
		{
			//printf("%d.%d.%d.%d\n",Infrom->recv_msg[28],Infrom->recv_msg[29],Infrom->recv_msg[30],Infrom->recv_msg[31]);
			//printf("ARP应答\n");
			struct sockaddr_ll sll;
			struct ifreq ethreq;
			strncpy(ethreq.ifr_name,net_interface[i].name,IFNAMSIZ);
		
			ioctl(socket_raw_fd,SIOCGIFINDEX, (char *)&ethreq);
			bzero(&sll, sizeof(sll));
			sll.sll_ifindex = ethreq.ifr_ifindex;
			p_new=(INTERFACE_INF *)malloc(sizeof(INTERFACE_INF ));
			memcpy(p_new->ip,Infrom->recv_msg+28,4);
			memcpy(p_new->mac,Infrom->recv_msg+22,6);
			//printf("insert ip=%d.%d.%d.%d\n",Infrom->recv_msg[28],Infrom->recv_msg[29],Infrom->recv_msg[30],Infrom->recv_msg[31]);
			p_search = link_search(interface_inf1,p_new->ip);
			if(p_search==NULL)
				{
				    
					link_insert(&head,p_new);
					//link_printf(head);
					
					
					
				}
		}
	}

return NULL;
}
