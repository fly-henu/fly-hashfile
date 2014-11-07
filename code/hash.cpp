#include "hash.h"

#include <malloc.h>
#include <string.h>

int rs_hash(int tot_rec, int key)
{
	int hash = 0;

	hash = key % tot_rec;
	return hash ;
}

void create_hash_file(const char* in_dir, int in_rec_len, int in_total_rec_num)
{
	HashFileHeader hfh;
	FILE* fp = NULL;
	char* buf = NULL;

	hfh.sign = 2434537;
	hfh.rec_len = in_rec_len;
	hfh.total_rec_num = in_total_rec_num;
	hfh.cur_rec_num = 0;

	if((fp= fopen(in_dir, "wb")) == NULL)
	{
		printf("cannot open file %s", in_dir);
		return;
	}

	fwrite(&hfh, sizeof(struct HashFileHeader), 1, fp);

	//格式化hash文件
	buf = (char*)malloc((hfh.rec_len + sizeof(struct ColFlag)) * hfh.total_rec_num);
	memset(buf, 0, (hfh.rec_len + sizeof(struct ColFlag)) * hfh.total_rec_num);
	if(fwrite(buf, 
		(hfh.rec_len + sizeof(struct ColFlag)) * hfh.total_rec_num, 1, fp) == 0)
	{
		printf("write file error!");
		return ;
	}
	free(buf);
	buf = NULL;
	if(fclose(fp) == EOF)
	{
		printf("close file error!");
		return ;
	}
	fp = NULL;

}

FILE* open_hash_file(const char* dir)
{
	FILE* fp;
	if((fp= fopen(dir, "rb+")) == NULL)
	{
		printf("cannot open file %s", dir);
		return NULL;
	}

	return fp;
}

int close_hash_file(FILE* fp)
{
	if(fclose(fp) == EOF)
	{
		printf("close file error!");
		return -1;
	}
	fp = NULL;
	return 0;
}

int read_hash_file_header(FILE* fp, HashFileHeader* p_hfh)
{
	fseek(fp, 0, SEEK_SET);
	fread(p_hfh, sizeof(HashFileHeader), 1, fp);

	return 0;
}

int write_hash_file(FILE* fp, void *rec_buf, int key)
{
	HashFileHeader hfh;
	int hash_addr_idx, offset;
	ColFlag flag;
	
	read_hash_file_header(fp, &hfh);
	if(hfh.total_rec_num == hfh.cur_rec_num)
	{
		printf("hash file full!");
		return -1;
	}

	hash_addr_idx = rs_hash(hfh.total_rec_num, key);

ReOffset:
	offset = sizeof(struct HashFileHeader) + (hfh.rec_len + sizeof(struct ColFlag)) * hash_addr_idx;
	fseek(fp, offset, SEEK_SET);
	fread(&flag, sizeof(struct ColFlag), 1, fp);
	if(flag.free == 0) //没有冲突产生
	{
		fseek(fp, offset, SEEK_SET);
		flag.free = 1;
		fwrite(&flag, sizeof(struct ColFlag), 1, fp);
		fwrite(rec_buf, hfh.rec_len, 1, fp);
	}
	else //产生冲突 因为hash表长和步长都为素数 故能遍历hash文件中的每条记录
	{
		flag.collision++;
		fwrite(&flag, sizeof(struct ColFlag), 1, fp);
		
		hash_addr_idx = (hash_addr_idx + STEPLEN) % hfh.total_rec_num;
		goto ReOffset;
	}

	return 0;
}

int find_hash_file_rec(FILE* fp, int key, void* buf, MyFun p_fun)
{
	HashFileHeader hfh;
	int hash_addr_idx, offset, comp_rtn;
	ColFlag flag;
	
	read_hash_file_header(fp, &hfh);

	hash_addr_idx = rs_hash(hfh.total_rec_num, key);
Re:
	offset = sizeof(struct HashFileHeader) + (hfh.rec_len + sizeof(struct ColFlag)) * hash_addr_idx;
	fseek(fp, offset, SEEK_SET);
	fread(&flag, sizeof(struct ColFlag), 1, fp);
	if(flag.free != 0)
	{
		fread(buf, hfh.rec_len, 1, fp);
		comp_rtn = (*p_fun)(key, buf);
		if(comp_rtn == -1)
		{
			hash_addr_idx = (hash_addr_idx + STEPLEN) % hfh.total_rec_num;
			goto Re;
		}
		return comp_rtn;
	}
	else
	{
		return -1;
	}
}
