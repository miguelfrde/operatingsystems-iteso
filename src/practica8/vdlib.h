#include "global.h"

#ifndef _VD_LIB_H_
#define _VD_LIB_H_

int vdwriteseclog(int sec_log, char *buffer);
int vdreadseclog(int sec_log, char *buffer);

int isblockfree(int block);
int nextfreeblock();
int assignblock(int block);
int unassignblock(int block);
int writeblock(int block, char *buffer);
int readblock(int block, char *buffer);

unsigned int datetoint(DATE date);
int inttodate(DATE *date, unsigned int val);
unsigned int currdatetimetoint();

int setninode(int num, char *filename, unsigned short atribs, int uid, int gid);
int searchinode(char *filename);
int removeinode(int numinode);
int isinodefree(int numinode);

#endif
