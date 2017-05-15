// RUN_IO_ReadWrite.cpp: implementation of the CRUN_IO_ReadWrite class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "RUN_IO_ReadWrite.h"

#include "FAS_HSSI.h"
#include "Variable.h"
#include "FastechPublic_IO.h"
#include "SrcPart\APartHandler.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRUN_IO_ReadWrite		RUN_IO_ReadWrite;

CRUN_IO_ReadWrite::CRUN_IO_ReadWrite()
{
	
}

CRUN_IO_ReadWrite::~CRUN_IO_ReadWrite()
{

}

void CRUN_IO_ReadWrite::Init_Total_IORead(int n_mode)
{
	int i =0;
	
	FAS_IO.IO_ReadWrite(0, 0, 0, -1); //output port initialize
	
	for(i = 0; i < 8; i++)
	{
		FAS_IO.IO_ReadWrite(0, 0, i, 0); //초기상태 읽는다.
		FAS_IO.IO_ReadWrite(0, 1, i, 0); //초기상태 읽는다.
		FAS_IO.IO_ReadWrite(0, 2, i, 0); //초기상태 읽는다.
		FAS_IO.IO_ReadWrite(0, 3, i, 0); //초기상태 읽는다.		
	}
	FAS_IO.IO_ReadWrite(1, 0, 0, 0); //초기상태 읽는다.

}

void CRUN_IO_ReadWrite::Run_Total_IORead()
{
	int i =0;
	
	for(i = 0; i < 8; i++)
	{
		FAS_IO.IO_ReadWrite(0, 0, i, 1); //초기상태 읽는다.
		FAS_IO.IO_ReadWrite(0, 1, i, 1); //초기상태 읽는다.	
		FAS_IO.IO_ReadWrite(0, 2, i, 1); //초기상태 읽는다.	
		FAS_IO.IO_ReadWrite(0, 3, i, 1); //초기상태 읽는다.			
	}
	FAS_IO.IO_ReadWrite(1, 0, 0, 1); //초기상태 읽는다.
}
