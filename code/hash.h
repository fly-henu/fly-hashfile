#ifndef _HASH_H
#define _HASH_H

#include <stdio.h>
/*
 * hash�ļ���Ҫ�Զ�������ʽ�� �ļ���ͷ������Ҫд��ͷ�ļ���¼ 
 * hash�ļ��ļ�¼����ʲô��ʽ�洢���ļ��ڵ��أ�
 *
 */

#define STEPLEN 13 //һ��ʹ������


//hash�ļ�ͷ
struct HashFileHeader
{
	int sign;
	int rec_len;
	int total_rec_num; //һ��ʹ������
	int cur_rec_num;
};

//��ͻ���
struct ColFlag
{
	char collision; //��ͻ
	char free;// '0' ������� '1'����ռ��
};

typedef int (*MyFun)(int key, void* buf);


int rs_hash(int tot_rec, int key);

void create_hash_file(const char* dir, int rec_len, int total_rec_num);

FILE* open_hash_file(const char* dir);

int close_hash_file(FILE* fp);

int read_hash_file_header(FILE* fp, HashFileHeader* p_hfh);

int write_hash_file(FILE* fp, void *rec_buf, int key);

int find_hash_file_rec(FILE* fp, int key, void* buf, MyFun p_fun);

#endif