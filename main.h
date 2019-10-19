#ifndef MAIN_H
#define MAIN_H


typedef struct interface_inf{
			//接口名字
	unsigned char ip[4];		//IP地址
	unsigned char mac[6];		//MAC地址
	struct interface_inf *next;
}INTERFACE_INF;
typedef struct inter_inf{
			//接口名字
	unsigned char recv_msg [1024];		//IP地址
	unsigned char send_msg[1024];
	int n_num;
	int len_buf;
}INTER;
typedef struct interface_save{
			//接口名字
	 char ip[30];		//IP地址
	struct interface_save *next;
}INTERFACE_SAVE;

extern INTERFACE_INF *interface_inf1,*head;
extern INTER *Infrom;
INTERFACE_SAVE *save_link;


extern void link_insert(INTERFACE_INF **p_head,INTERFACE_INF *p_new);
extern INTERFACE_INF *link_search(INTERFACE_INF*head,unsigned char *buf);
extern void link_printf(INTERFACE_INF*p_mov);


#endif
