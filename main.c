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

#include <netinet/ether.h>	
#include "get_interface.h"
#include "main.h"
#include "thread_pool.h"
#include "ip_thread.h"
#include "arp_thread.h"


INTERFACE_INF *interface_inf1,*head;
INTER *Infrom;
INTERFACE_SAVE *save_link;
pool_t pool;
void help()
{
	printf("=-------------------路由使用说明----------------—--------\n");
	printf("=---命令：setip +IP 此IP的数据包无法路由----------------=\n");
	printf("=---命令：delip +IP 删除此过滤IP，此IP正常发送数据包----=\n");
	printf("=---命令：saveset  保存设置-----------------------------=\n");
	printf("=---命令：showip 显示过滤IP-----------------------------=\n");
	printf("=---命令：showarp 显示ARP表-----------------------------=\n");
	printf("=---命令：help 现实此界面-------------------------------=\n");
	printf("=---命令：exit 退出进程---------------------------------=\n");
	printf("=-------------------------------------------------------=\n");
}
int link_delete(INTERFACE_SAVE**p_head,char *buf)
{
   INTERFACE_SAVE*p_mov=*p_head,*p_fount;
   int flag=0;
   if(p_mov==NULL)
   printf("链表为空!\n");
   else
   {
   while(p_mov!=NULL)
    {
	   if(strcmp(buf,p_mov->ip)==0) 
		 {
		   if(p_mov==*p_head)
			*p_head=(*p_head)->next;
		  else
		   p_fount->next=p_mov->next;
		   free(p_mov);
		   flag=1;
		   break;
		 }
		 p_fount=p_mov;
		 p_mov=p_mov->next;
	 }
	 printf("delete success\n");
   }
  return flag;
}

void link_insert(INTERFACE_INF **p_head,INTERFACE_INF *p_new)
{
   INTERFACE_INF *p_mov=*p_head;
 
   if(p_mov==NULL)
	{
	   *p_head=p_new;
	   p_new->next=NULL;
	   //	printf("insert success\n");

	}
	 else
	{
	   	while(p_mov->next!=NULL)
		{
		  	p_mov =  p_mov->next;
		}
		p_mov->next=p_new;
		p_new->next=NULL;
		//	printf("insert sucess\n");
	}
	

	
}
 void save_link_set(INTERFACE_SAVE*p_mov)
{
  FILE *fp;
  fp=fopen("save.txt","w");
  if(p_mov==NULL)
  printf("此链表为空!\n");
  while(p_mov!=NULL)
   {
	   fprintf(fp,"%s\n",p_mov->ip);
	   p_mov=p_mov->next;
   }
   fclose(fp);
}

void link_printf(INTERFACE_INF*p_mov)
{
printf("----------link_start-----------\n");
  if(p_mov==NULL)
  	printf("此链表为空!\n");
  while(p_mov!=NULL)
   {
   	char cli_ip[INET_ADDRSTRLEN]="";
	char mac[18] = "";
	sprintf(mac,"%02x:%02x:%02x:%02x:%02x:%02x",p_mov->mac[0],p_mov->mac[1],p_mov->mac[2],p_mov->mac[3],p_mov->mac[4],p_mov->mac[5]);
  	inet_ntop(AF_INET,p_mov->ip,cli_ip,INET_ADDRSTRLEN);
	printf("%s->->%s\n",cli_ip,mac);
   	p_mov=p_mov->next;
   }
  printf("----------link_end-----------\n");
}
void link_printf_save(INTERFACE_SAVE*p_mov)
{
printf("----------link_start-----------\n");
  if(p_mov==NULL)
  	printf("此链表为空!\n");
  while(p_mov!=NULL)
   {
	printf("IP->->%s\n",p_mov->ip);
   	p_mov=p_mov->next;
   }
  printf("----------link_end-----------\n");
}

INTERFACE_INF *link_search(INTERFACE_INF* head,unsigned char *buf)
{
   INTERFACE_INF *p_mov=head;
   // if(p_mov==NULL)
   	// printf("1111链表为空！\n");
   // else
   // {
		while(p_mov!=NULL)
	   {
		 if(memcmp(p_mov->ip,buf,4) == 0)
		  break;
		 p_mov=p_mov->next;
	 //}
    }
 return p_mov;
}
void link_insert_save(INTERFACE_SAVE **p_head,INTERFACE_SAVE *p_new)
{
   INTERFACE_SAVE *p_mov=*p_head;
 
   if(p_mov==NULL)
	{
	   *p_head=p_new;
	   p_new->next=NULL;

	}
	 else
	{
	   	while(p_mov->next!=NULL)
		{
		  	p_mov =  p_mov->next;
		}
		p_mov->next=p_new;
		p_new->next=NULL;
	}
	
}

void *pthread_rcv_key_msg()
{
	INTERFACE_SAVE *p_new=NULL;
	help();
 	FILE *fp;
	fp=fopen("save.txt","r+");
      while(1)
	    {
		   p_new=(INTERFACE_SAVE*)malloc(sizeof(INTERFACE_SAVE));
		   fscanf(fp,"%s",p_new->ip);
		   if(feof(fp)!=0)
		   {
		     fclose(fp);
		       break;
		 }
		   link_insert_save(&save_link,p_new); 
		 }
	while(1)
	{
		char buf_cmd[200]="";
		char buf[20]="";
		fgets(buf_cmd,sizeof(buf_cmd),stdin);
		buf_cmd[strlen(buf_cmd)-1]='\0';
		if(strncmp(buf_cmd,"setip",5)==0)
		{
			p_new=(INTERFACE_SAVE *)malloc(sizeof(INTERFACE_SAVE ));
			sscanf(buf_cmd, "setip %[0-9.] ",buf);
			strcpy(p_new->ip,buf);
			link_insert_save(&save_link,p_new);
		}
		else if(strncmp(buf_cmd,"delip",5)==0)
		{
			sscanf(buf_cmd, "delip %[0-9.] ",buf);
			printf("p_new====%s\n",buf);
			int flag = link_delete(&save_link,buf);
			if(flag==1)
				printf("delete success!\n");
		   	 if(flag==0)
		  		 printf("delete fail!\n");	
		}
		else if(strcmp(buf_cmd,"help")==0)
			help();
		else if(strcmp(buf_cmd,"showip")==0)
			link_printf_save(save_link);
		else if(strcmp(buf_cmd,"saveset")==0)
			save_link_set(save_link);
		else if(strcmp(buf_cmd,"showarp")==0)
			link_printf(interface_inf1);
		else if(strcmp(buf_cmd,"exit")==0)
			{
				printf("in exit_fun\n");
				pool_uninit(&pool);//删除线程池
				exit(0);
			}
	}
	return NULL;
}
int screen_link(unsigned char *buf,INTERFACE_SAVE* p_mov)  //查看过滤表
{
	int flag=0;
	 while(p_mov!=NULL)
	 {
		char cli_ip[INET_ADDRSTRLEN]="";
	  	inet_ntop(AF_INET,buf,cli_ip,INET_ADDRSTRLEN);
		if(strcmp(cli_ip,p_mov->ip)==0)
		{
			flag = 1;
			break;

		}
		p_mov=p_mov->next;
	 }
	 return flag;
}

void *pthread_rcv_msg(void *fd)
{
	int socket_raw_fd = *(int *)fd;
	Infrom=(INTER *)malloc(sizeof(INTER ));
	head=interface_inf1;
	Infrom->n_num  =  get_interface_num();
	unsigned char send_msg1[1024]= {  //arp组包格式  
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff,//目的mac
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//源mac
		0x08, 0x06,//协议arp
		0x00, 0x01, 0x08, 0x00,  
		0x06, 0x04, 0x00, 0x01,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		10, 7, 162, 72,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		10, 7, 162, 1
	};
	memcpy(Infrom->send_msg,send_msg1,42);
	while(1)
	{
			memset(Infrom->recv_msg,0,sizeof(Infrom->recv_msg));
			Infrom->len_buf = recvfrom(socket_raw_fd,Infrom->recv_msg,sizeof(Infrom->recv_msg),0,NULL,NULL);//接待接收
			if(ntohs(*(unsigned short *)(Infrom->recv_msg+12)) == 0x0806) //arp

				{	
					pool_add_task(&pool, arp_pthread, (void *)&socket_raw_fd);
				}

			else if(ntohs(*(unsigned short *)(Infrom->recv_msg+12)) == 0x0800)//ip
				{
					int flag = screen_link(Infrom->recv_msg+30,save_link);
					
					if(flag==0)
						 ip_pthread_msg(socket_raw_fd);	
				}
	}
	close(socket_raw_fd);
	return NULL;
}

int sockaddr_init()  //创建原始套接字
{
	int socket_raw_fd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
	return socket_raw_fd; 
}
int main(int argc, char *argv[])
{
	int socket_raw_fd;
	socket_raw_fd = sockaddr_init();
	getinterface();
	pool_init(&pool, 40);//初始化一个线程池
	sleep(1);
	pthread_t recv_msc,key_msc;
	pthread_create(&recv_msc,NULL,pthread_rcv_msg,(void *)&socket_raw_fd);//接受线程
	pthread_create(&key_msc,NULL,pthread_rcv_key_msg,NULL);//键盘处理线程
	pthread_detach(key_msc);
	pthread_join(recv_msc,NULL);
	return 0;
}

