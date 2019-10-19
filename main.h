#ifndef MAIN_H
#define MAIN_H


typedef struct interface_inf{
			//�ӿ�����
	unsigned char ip[4];		//IP��ַ
	unsigned char mac[6];		//MAC��ַ
	struct interface_inf *next;
}INTERFACE_INF;
typedef struct inter_inf{
			//�ӿ�����
	unsigned char recv_msg [1024];		//IP��ַ
	unsigned char send_msg[1024];
	int n_num;
	int len_buf;
}INTER;
typedef struct interface_save{
			//�ӿ�����
	 char ip[30];		//IP��ַ
	struct interface_save *next;
}INTERFACE_SAVE;

extern INTERFACE_INF *interface_inf1,*head;
extern INTER *Infrom;
INTERFACE_SAVE *save_link;


extern void link_insert(INTERFACE_INF **p_head,INTERFACE_INF *p_new);
extern INTERFACE_INF *link_search(INTERFACE_INF*head,unsigned char *buf);
extern void link_printf(INTERFACE_INF*p_mov);


#endif
