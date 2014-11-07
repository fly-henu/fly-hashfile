#ifndef _RECORD_H
#define _RECORD_H

#define REC_LEN 48

//¼ÇÂ¼¸ñÊ½
struct Record
{
	int key;
	char other_info[REC_LEN - sizeof(int)];
};


#endif