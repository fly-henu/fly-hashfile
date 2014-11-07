#ifndef _HASH_H
#define _HASH_H

#include <stdio.h>
/*
 * hash文件需要以二进制形式打开 文件开头首先需要写入头文件记录 
 * hash文件的记录是以什么方式存储在文件内的呢？
 *
 */

#define STEPLEN 13 //一般使用素数


//hash文件头
struct HashFileHeader
{
	int sign;
	int rec_len;
	int total_rec_num; //一般使用素数
	int cur_rec_num;
};

//冲突标记
struct ColFlag
{
	char collision; //冲突
	char free;// '0' 代表空闲 '1'代表占用
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