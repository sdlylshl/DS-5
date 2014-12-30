#ifndef __NANDLV_H
#define __NANDLV_H

#include "def.h"
#include "2440addr.h"
#include "2440lib.h"

extern U32 EraseBlock(U32 addr);
extern void ReadPage(U32 addr, U8 *buf);
extern void NF_ReadPageBP(U32 Block,U32 Page,U8 *buf);
extern int CheckBadBlk(U32 addr);
extern U32 WritePage(U32 addr, U8 *buf);
extern void NF_WritePageBP(U32 Block,U32 Page, U8 *buf);
extern int CheckBadBlk(U32 addr);

extern void WrFileToNF(void);
extern void WrFileToNF_COM(void);

extern void InitNandFlash(int info);
extern void NandErase(void);

extern void NandWrite(void);
extern void usbNandWrite(void);

extern int search_params(void);
extern int save_params(void);
extern int set_params(void);

#endif