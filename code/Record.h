#ifndef _RECORD_H
#define _RECORD_H

#define REC_LEN 48

//��¼��ʽ
struct Record
{
	int key;
	char other_info[REC_LEN - sizeof(int)];
};


#endif