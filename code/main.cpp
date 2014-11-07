#include <stdio.h>
#include <string.h>
#include "hash.h"
#include "Record.h"

int judge_key(int key, void* buf)
{
	Record* rec = (Record*)buf;
	if(rec->key == key)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int main(int argc, char* argv)
{
	Record rec[7] = 
	{
		{1, "zhao"},
		{12, "qian"},
		{34, "sun"},
		{56, "li"},
		{23, "wang"},
		{89, "wu"},
		{101, "zhou"}
	};
	int i;
	Record aim_rec;
	char* dir = "C:\\record.hash";

	create_hash_file(dir, REC_LEN, 7);
	printf("hash file create successfull!\n");

	FILE* fp = NULL;
	fp = open_hash_file(dir);
	for(i = 0; i < sizeof(rec)/sizeof(Record); i++)
	{
		write_hash_file(fp, &rec[i], rec[i].key);
	}
	close_hash_file(fp);

   	fp = open_hash_file(dir);
	find_hash_file_rec(fp, 56, &aim_rec, &judge_key);
	printf("\n\t%d,%s\n", aim_rec.key, aim_rec.other_info);
	close_hash_file(fp);


	//printf("%d\n", strlen((void*)&rec[i]));
	for(i = 0; i < sizeof(rec)/sizeof(Record); i++)
	{
		printf("<%d, %d>\t", rec[i].key, rs_hash(6 ,rec[i].key));
	}

	return 0;
}