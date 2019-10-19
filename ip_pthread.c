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
void ip_pthread_msg(int socket_raw_fd)
{
	INTERFACE_INF *p_search;
	int i=0;
	for(i=0;i<Infrom->n_num;i++)
	{	//printf("sendip=%s\n",net_interface[i].ip);
		if(memcmp(net_interface[i].ip,Infrom->recv_msg+30,3) == 0&& memcmp(Infrom->recv_msg+30,Infrom->recv_msg+26,3)!=0)
		{
			// printf("******************************\n");
		    // link_printf(head);
			// printf("******************************\n");
			interface_inf1=head;
			struct sockaddr_ll sll;
			struct ifreq ethreq;
			
			strncpy(ethreq.ifr_name,net_interface[i].name,IFNAMSIZ);
			ioctl(socket_raw_fd,SIOCGIFINDEX, (char *)&ethreq);
			
			p_search = link_search(interface_inf1,Infrom->recv_msg+30);
			if(p_search!=NULL)
			{	
				bzero(&sll, sizeof(sll));
				sll.sll_ifindex = ethreq.ifr_ifindex;
				memcpy(Infrom->recv_msg,p_search->mac,6);
				memcpy(Infrom->recv_msg+6,net_interface[i].mac,6);
			//	printf("Icmp转发\n");
				sendto(socket_raw_fd,Infrom->recv_msg,Infrom->len_buf, 0, (struct sockaddr *)&sll, sizeof(sll));  //转发icmp
			}
			else if(p_search==NULL)
			{
				   
					bzero(&sll, sizeof(sll));
					sll.sll_ifindex = ethreq.ifr_ifindex;
					//memcpy(Infrom->send_msg+32,Infrom->recv_msg+6,6);//目的mac
					//printf("mac==%02x:%02x:%02x:%02x:%02x:%02x\n",net_interface[i].mac[0],net_interface[i].mac[1],net_interface[i].mac[2],net_interface[i].mac[3],net_interface[i].mac[4],net_interface[i].mac[5]);
					//printf("%d.%d.%d.%d\n",Infrom->recv_msg[26],Infrom->recv_msg[27],Infrom->recv_msg[28],Infrom->recv_msg[29]);
					//printf("%d.%d.%d.%d\n",Infrom->recv_msg[30],Infrom->recv_msg[31],Infrom->recv_msg[32],Infrom->recv_msg[33]);
					//printf("wk=%s\n",net_interface[i].name);
					memcpy(Infrom->send_msg+6,net_interface[i].mac,6);//mac源mac
					memcpy(Infrom->send_msg+22,net_interface[i].mac,6);//ARP源mac
					memcpy(Infrom->send_msg+28,net_interface[i].ip,4);//源ip
					memcpy(Infrom->send_msg+38,Infrom->recv_msg+30,4);//目的ip
					//printf("name = %s\n", Infrom->recv_msg);
					
				
					
					//printf("arp请求\n");
					sendto(socket_raw_fd,Infrom->send_msg, 42, 0, (struct sockaddr *)&sll, sizeof(sll));  //发送arp请求
					
			}
			break;
		}
	}

}


