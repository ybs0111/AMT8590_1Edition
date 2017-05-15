// IO_Manager.cpp: implementation of the IO_Manager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "IO_Manager.h"
#include "FAS_HSSI.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "..\common\ShareMem.h"
#include "FastechPublic_IO.h"
#include "Srcbase\ALocalization.h"
#include "SrcPart\APartHandler.h"

st_io_param st_io;
IO_Manager g_ioMgr;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IO_Manager::IO_Manager()
{

}

IO_Manager::~IO_Manager()
{

}

BOOL IO_Manager::Get_In_Bit( int iIoNum )
{
	BOOL bRtn = FALSE;
	if( st_handler.mn_virtual_mode == 1 )
	{
		std::string strGet = g_sm.GetData( (SM_TYPE)iIoNum );
		if( strGet == "1" )
			bRtn = TRUE;
	}
	else
	{
		bRtn = FAS_IO.Get_In_Bit( iIoNum );
	}

	return bRtn;
}

int IO_Manager::Get_In_Bit( int iIoNum, int iDummy )
{
	BOOL bRtn = FALSE;
	if( st_handler.mn_virtual_mode == 1 )
	{
		std::string strGet = g_sm.GetData( (SM_TYPE)iIoNum );
		if( strGet == "1" )
			bRtn = TRUE;
	}
	else
	{
		bRtn = FAS_IO.Get_In_Bit( iIoNum );
	}

// 	return Get_In_Bit( iIoNum ); //2012,1229
	return bRtn;
}

int IO_Manager::Set_Out_Bit( int nIONo, BOOL bOnOff ) 
{
	int iRtn = CTLBD_RET_GOOD;

	if( st_handler.mn_virtual_mode == 1 )
	{
		std::string strOnOff = "0";
		if( bOnOff == TRUE )
			strOnOff = "1";

		g_sm.SetData( (SM_TYPE)nIONo, strOnOff );
	}
	else
	{
		iRtn = FAS_IO.Set_Out_Bit( nIONo, bOnOff );
	}
	return iRtn;
}

int IO_Manager::Set_IO_PortAutoEnable(int nMasterID, int nPortNo)
{
	int iRtn = CTL_YES;
	if( st_handler.mn_virtual_mode == 1 )
	{

	}
	else
	{
		iRtn = FAS_IO.Set_IO_PortAutoEnable( nMasterID, nPortNo );
	}

	return iRtn;
}

int IO_Manager::Set_IO_SlaveEnable(int nMasterID, int nPortNo, int nSlaveNo, BOOL bEnable)
{
	int iRtn = CTL_YES;
	if( st_handler.mn_virtual_mode == 1 )
	{

	}
	else
	{
		iRtn = FAS_IO.Set_IO_SlaveEnable( nMasterID, nPortNo, nSlaveNo, bEnable );
	}

	return iRtn;
}

BOOL IO_Manager::Get_Out_Bit( int iIoNum )
{
	BOOL bRtn = FALSE;
	if( st_handler.mn_virtual_mode == 1 )
	{
		std::string strGet = g_sm.GetData( (SM_TYPE)iIoNum );
		if( strGet == "1" )
			bRtn = TRUE;
	}
	else
	{
		bRtn = FAS_IO.Get_Out_Bit( iIoNum );
	}
	
	return bRtn;	
}

BOOL IO_Manager::Get_Out_Bit( int iIoNum, int iDummy )
{
	return Get_Out_Bit( iIoNum );
}

WORD IO_Manager::Get_In_Word(int nIONo)
{
	WORD wRtn = 0x00;
	if( st_handler.mn_virtual_mode == 1 )
	{
		for( int i=0; i<16; i++ )
		{
			std::string strGet = g_sm.GetData( (SM_TYPE) (nIONo * 100 + i) );
			if( strGet == "1" )
				wRtn |= (1 << i);
		}
	}
	else
	{
		wRtn = FAS_IO.Get_In_Word( nIONo );
	}
	return wRtn;
}

WORD IO_Manager::Get_Out_Word(int nIONo)
{
	WORD wRtn = 0x00;
	if( st_handler.mn_virtual_mode ==  1 )
	{
		for( int i=0; i<16; i++ )
		{
			std::string strGet = g_sm.GetData( (SM_TYPE) (nIONo * 100 + i) );
			if( strGet == "1" )
				wRtn |= (1 << i);
		}
	}
	else
	{
		wRtn = FAS_IO.Get_Out_Word( nIONo );
	}
	return wRtn;
}


int IO_Manager::set_out_bit( int nIONo, BOOL bOnOff )
{
	int iRtn = 0;
	if( st_handler.mn_virtual_mode == 1 )
		iRtn = Set_Out_Bit( nIONo, bOnOff );
	else
		iRtn = FAS_IO.set_out_bit( nIONo, bOnOff );

	return iRtn;
}

BOOL IO_Manager::get_in_bit( int iIoNum, int iDummy )
{
	BOOL bRtn = FALSE;
	if( st_handler.mn_virtual_mode == 1 )
		bRtn = Get_In_Bit( iIoNum, iDummy );
	else
		bRtn = FAS_IO.get_in_bit( iIoNum, iDummy );

	return bRtn;
}

BOOL IO_Manager::get_in_bit( int iIoNum, int opt, int iDummy )
{
	BOOL bRtn = FALSE;
	if( st_handler.mn_virtual_mode == 1 )
		bRtn = Get_In_Bit( iIoNum, iDummy );
	else
		bRtn = FAS_IO.Get_In_Bit( iIoNum, iDummy );
	
	return bRtn;
}

BOOL IO_Manager::get_in_bit( int iIoNum )
{
	BOOL bRtn = FALSE;
	int iDummy = IO_OFF;
	if( st_handler.mn_virtual_mode == 1 )
		bRtn = Get_In_Bit( iIoNum, iDummy );
	else
		bRtn = FAS_IO.get_in_bit( iIoNum, iDummy );

	return bRtn;
}

BOOL IO_Manager::get_out_bit( int iIoNum, int iDummy )
{
	BOOL bRtn = FALSE;
	if( st_handler.mn_virtual_mode == 1 )
		bRtn = Get_Out_Bit( iIoNum, iDummy );
	else
		bRtn = FAS_IO.get_out_bit( iIoNum, iDummy );

	return bRtn;
}

st_io_param::st_io_param()
{
	int i=0,j=0,k=0;

	//ASS'Y  : LOB BUFFER
	st_io.o_density_f1_lockunlock_switch_lamp							= 0;				//S0000								
	st_io.o_density_f1_paper_pusher_switch_lamp							= 1;								
	st_io.o_density_f2_lockunlock_switch_lamp							= 2;								
	st_io.o_density_f2_paper_pusher_switch_lamp							= 3;								
	st_io.o_density_f3_lockunlock_switch_lamp							= 4;								
	st_io.o_density_f3_paper_pusher_switch_lamp							= 5;								
	
	st_io.i_density_f1_lockunlock_switch_chk							= 8;										
	st_io.i_density_f1_paper_pusher_switch_chk							= 9;								
	st_io.i_density_f2_lockunlock_switch_chk							= 10;									
	st_io.i_density_f2_paper_pusher_switch_chk							= 11;								
	st_io.i_density_f3_lockunlock_switch_chk							= 12;									
	st_io.i_density_f3_paper_pusher_switch_chk							= 13;								
	st_io.o_density_f4_lockunlock_switch_lamp							= 100;								
	st_io.o_density_f4_paper_pusher_switch_lamp							= 101;								
	st_io.o_density_f5_lockunlock_switch_lamp							= 102;								
	st_io.o_density_f5_paper_pusher_switch_lamp							= 103;								
	st_io.o_density_f6_lockunlock_switch_lamp							= 104;								
	st_io.o_density_f6_paper_pusher_switch_lamp							= 105;								
	
	st_io.i_density_f4_lockunlock_switch_chk							= 108;									
	st_io.i_density_f4_paper_pusher_switch_chk							= 109;								
	st_io.i_density_f5_lockunlock_switch_chk							= 110;									
	st_io.i_density_f5_paper_pusher_switch_chk							= 111;								
	st_io.i_density_f6_lockunlock_switch_chk							= 112;									
	st_io.i_density_f6_paper_pusher_switch_chk							= 113;	
	////2014,1203
	st_io.i_density_left_label_limit1_chk								= 114;
	st_io.i_density_right_label_limit2_chk								= 115;
	////

	st_io.o_density_paper_pusher_switch[0]								= 1;
	st_io.o_density_paper_pusher_switch[1]								= 3;
	st_io.o_density_paper_pusher_switch[2]								= 5;
	st_io.o_density_paper_pusher_switch[3]								= 101;
	st_io.o_density_paper_pusher_switch[4]								= 103;
	st_io.o_density_paper_pusher_switch[5]								= 105;
	
	st_io.o_density_lock_switch[0]										= 0;
	st_io.o_density_lock_switch[1]										= 2;
	st_io.o_density_lock_switch[2]										= 4;
	st_io.o_density_lock_switch[3]										= 100;
	st_io.o_density_lock_switch[4]										= 102;
	st_io.o_density_lock_switch[5]										= 104;


	st_io.i_density_paper_pusher_switch_chk[0]							= 9;
	st_io.i_density_paper_pusher_switch_chk[1]							= 11;
	st_io.i_density_paper_pusher_switch_chk[2]							= 13;
	st_io.i_density_paper_pusher_switch_chk[3]							= 109;
	st_io.i_density_paper_pusher_switch_chk[4]							= 111;
	st_io.i_density_paper_pusher_switch_chk[5]							= 113;

	st_io.i_density_lock_switch_chk[0]									= 8;
	st_io.i_density_lock_switch_chk[1]									= 10;
	st_io.i_density_lock_switch_chk[2]									= 12;
	st_io.i_density_lock_switch_chk[3]									= 108;
	st_io.i_density_lock_switch_chk[4]									= 110;
	st_io.i_density_lock_switch_chk[5]									= 112;


	st_io.o_density_f1_lockunlock										= 200;											
	st_io.o_density_f2_lockunlock										= 201;											
	st_io.o_density_f3_lockunlock										= 202;											
	st_io.o_density_f4_lockunlock										= 203;											
	st_io.o_density_f5_lockunlock										= 204;											
	st_io.o_density_f6_lockunlock										= 205;											
	st_io.o_density_f1_motor_power										= 206;											
	st_io.o_density_f2_motor_power										= 207;											
	st_io.o_density_f3_motor_power										= 208;											
	st_io.o_density_f4_motor_power										= 209;											
	st_io.o_density_f5_motor_power										= 210;											
	st_io.o_density_f6_motor_power										= 211;											

	st_io.i_density_f1_lock_chk											= 300;												
	st_io.i_density_f1_unlock_chk										= 301;											
	st_io.i_density_f2_lock_chk											= 302;												
	st_io.i_density_f2_unlock_chk										= 303;											
	st_io.i_density_f3_lock_chk											= 304;												
	st_io.i_density_f3_unlock_chk										= 305;											
	st_io.i_density_f4_lock_chk											= 306;												
	st_io.i_density_f4_unlock_chk										= 307;											
	st_io.i_density_f5_lock_chk											= 308;												
	st_io.i_density_f5_unlock_chk										= 309;											
	st_io.i_density_f6_lock_chk											= 310;												
	st_io.i_density_f6_unlock_chk										= 311;											

//	st_io.o_density_label_blow_onoff									= 400;
//	st_io.o_density_label_plate_blow_onoff								= 401;
//
//	st_io.o_density_f1_plate_btm_blow_onoff								= 403;
//	st_io.o_density_f2_plate_btm_blow_onoff								= 404;
//	st_io.o_density_f3_plate_btm_blow_onoff								= 405;
//	st_io.o_density_f4_plate_btm_blow_onoff								= 406;
//	st_io.o_density_f5_plate_btm_blow_onoff								= 407;
//	st_io.o_density_f6_plate_btm_blow_onoff								= 408;
//	st_io.o_density_f1_plate_blow_onoff									= 409;
//	st_io.o_density_f2_plate_blow_onoff									= 410;
//	st_io.o_density_f3_plate_blow_onoff									= 411;
//	st_io.o_density_f4_plate_blow_onoff									= 412;
//	st_io.o_density_f5_plate_blow_onoff									= 413;
//	st_io.o_density_f6_plate_blow_onoff									= 414;

	for ( i = 0; i < 6; i++)
	{
		st_io.o_density_lockunlock[i]									= 200 + i;
		st_io.i_density_lock_chk[i]										= 300 + i*2;
		st_io.i_density_unlock_chk[i]									= 301 + i*2;
//		st_io.o_density_plate_btm_blow_onoff[i]							= 403 + i;
//		st_io.o_density_plate_blow_onoff[i]								= 409 + i;
		st_io.o_density_motor_power[i]									= 206 + i;
	}

//	st_io.o_density_f1_paper_pusher_fwdback								= 500;
//	st_io.o_density_f2_paper_pusher_fwdback								= 501;
//	st_io.o_density_f3_paper_pusher_fwdback								= 502;
//	st_io.o_density_f4_paper_pusher_fwdback								= 503;
//	st_io.o_density_f5_paper_pusher_fwdback								= 504;
//	st_io.o_density_f6_paper_pusher_fwdback								= 505;
//
//	st_io.i_density_f1_sticker_limit_chk								= 507;
//	st_io.i_density_f1_paper_pusher_forward_chk							= 508;
//	st_io.i_density_f1_position_chk										= 509;
//	st_io.i_density_f2_sticker_limit_chk								= 510;
//	st_io.i_density_f2_paper_pusher_forward_chk							= 511;
//	st_io.i_density_f2_position_chk										= 512;
//	st_io.i_density_f3_sticker_limit_chk								= 513;
//	st_io.i_density_f3_paper_pusher_forward_chk							= 514;
//	st_io.i_density_f3_position_chk										= 515;
//
//	st_io.i_density_f4_sticker_limit_chk								= 600;
//	st_io.i_density_f4_paper_pusher_forward_chk							= 601;
//	st_io.i_density_f4_position_chk										= 602;
//	st_io.i_density_f5_sticker_limit_chk								= 603;
//	st_io.i_density_f5_paper_pusher_forward_chk							= 604;
//	st_io.i_density_f5_position_chk										= 605;
//	st_io.i_density_f6_sticker_limit_chk								= 606;
//	st_io.i_density_f6_paper_pusher_forward_chk							= 607;
//	st_io.i_density_f6_position_chk										= 608;
//	st_io.i_density_f1_plate_vacuun2_chk								= 609;
//	st_io.i_density_f2_plate_vacuun2_chk								= 610;
//	st_io.i_density_f3_plate_vacuun2_chk								= 611;
//	st_io.i_density_f4_plate_vacuun2_chk								= 612;
//	st_io.i_density_f5_plate_vacuun2_chk								= 613;
//	st_io.i_density_f6_plate_vacuun2_chk								= 614;

//	for ( i = 0; i < 6; i++)
//	{
//		st_io.o_density_paper_pusher_fwdback[i]							= 500 + i;
//		st_io.i_density_plate_vacuum_chk[i]								= 609 + i;
//	}
//	for ( i = 0; i < 3; i++)
//	{
//		st_io.i_density_sticker_limit_chk[i]							= 507 + (i*3);
//		st_io.i_density_paper_pusher_fwdback_chk[i]						= 508 + (i*3);
//	}
//	for ( i = 3; i < 6; i++)
//	{
//		st_io.i_density_sticker_limit_chk[i]							= 600 + ((i-3)*3);
//		st_io.i_density_paper_pusher_fwdback_chk[i]						= 601 + ((i-3)*3);
//	}

	st_io.o_density_lable1_start										= 400;
	st_io.o_density_lable2_start										= 401;
	st_io.o_density_lable3_start										= 402;
	st_io.o_density_lable4_start										= 403;
	st_io.o_density_lable5_start										= 404;
	st_io.o_density_lable6_start										= 405;

	st_io.o_density_lable1_pickon										= 406;
	st_io.o_density_lable2_pickon										= 407;
	st_io.o_density_lable3_pickon										= 408;
	st_io.o_density_lable4_pickon										= 409;
	st_io.o_density_lable5_pickon										= 410;
	st_io.o_density_lable6_pickon										= 411;
	st_io.o_density_lable1_length1										= 412;
	st_io.o_density_lable1_length2										= 413;
	st_io.o_density_lable2_length1										= 414;
	st_io.o_density_lable2_length2										= 415;

	for ( i = 0; i < 6; i++)
	{
		st_io.o_density_lable_start[i]									= 400 + i;
		st_io.o_density_lable_pickon[i]									= 406 + i;
	}
	for ( i = 0; i < 2; i++)
	{
		st_io.o_density_lable_length1[i]								= 412 + (i*2);
		st_io.o_density_lable_length2[i]								= 413 + (i*2);
	}

	st_io.o_density_lable3_length1										= 500;
	st_io.o_density_lable3_length2										= 501;
	st_io.o_density_lable4_length1										= 502;
	st_io.o_density_lable4_length2										= 503;
	st_io.o_density_lable5_length1										= 504;
	st_io.o_density_lable5_length2										= 505;
	st_io.o_density_lable6_length1										= 506;
	st_io.o_density_lable6_length2										= 507;

	for ( i = 2; i < 6; i++)
	{
		st_io.o_density_lable_length1[i]								= 500 + ((i-2)*2);
		st_io.o_density_lable_length2[i]								= 500 + ((i-2)*2);
	}

	st_io.i_density_f1_position_chk										= 508;
	st_io.i_density_f2_position_chk										= 509;
	st_io.i_density_f3_position_chk										= 510;
	st_io.i_density_f4_position_chk										= 511;
	st_io.i_density_f5_position_chk										= 512;
	st_io.i_density_f6_position_chk										= 513;
	st_io.i_density_label1_error_chk									= 514;
	st_io.i_density_label2_error_chk									= 515;
	
	for ( i = 0; i < 6; i++)
	{
		st_io.i_density_position_chk[i]									= 508 + i;
	}

	for ( i = 0; i < 2; i++)
	{
		st_io.i_density_label_error_chk[i]								= 514 + i;
	}


	st_io.i_density_label3_error_chk									= 600;
	st_io.i_density_label4_error_chk									= 601;
	st_io.i_density_label5_error_chk									= 602;
	st_io.i_density_label6_error_chk									= 603;
	for ( i = 2; i < 6; i++)
	{
		st_io.i_density_label_error_chk[i]								= 600 + (i-2);
	}

	st_io.i_density_label1_ready_chk									= 604;
	st_io.i_density_label2_ready_chk									= 605;
	st_io.i_density_label3_ready_chk									= 606;
	st_io.i_density_label4_ready_chk									= 607;
	st_io.i_density_label5_ready_chk									= 608;
	st_io.i_density_label6_ready_chk									= 609;
	st_io.i_density_label1_complete_chk									= 610;
	st_io.i_density_label2_complete_chk									= 611;
	st_io.i_density_label3_complete_chk									= 612;
	st_io.i_density_label4_complete_chk									= 613;
	st_io.i_density_label5_complete_chk									= 614;
	st_io.i_density_label6_complete_chk									= 615;

	for ( i = 0; i < 6; i++)
	{
		st_io.i_density_label_ready_chk[i]								= 604 + i;
		st_io.i_density_label_complete_chk[i]							= 610 + i;
	}

	st_io.o_density_rbt_left_vacuum1									= 700;
	st_io.o_density_rbt_left_vacuum2									= 701;
	st_io.o_density_rbt_right_vacuum1									= 702;
	st_io.o_density_rbt_right_vacuum2									= 703;

	st_io.o_density_rbt_left_blow1										= 704;
	st_io.o_density_rbt_left_blow2										= 705;
	st_io.o_density_rbt_right_blow1										= 706;
	st_io.o_density_rbt_right_blow2										= 707;


	st_io.i_density_rbt_left_vacuum1_chk								= 708;
	st_io.i_density_rbt_left_vacuum2_chk								= 709;
	st_io.i_density_rbt_right_vacuum1_chk								= 710;
	st_io.i_density_rbt_right_vacuum2_chk								= 711;

	st_io.i_bcr_label1_chk												= 712;
	st_io.i_bcr_label2_chk												= 713;
	st_io.i_bcr_label3_chk												= 714;
	st_io.i_bcr_label4_chk												= 715;

	st_io.o_distribute_p1_lockunlock_switch_lamp						= 1000;
	st_io.o_distribute_p2_lockunlock_switch_lamp						= 1001;
	st_io.o_distribute_p3_lockunlock_switch_lamp						= 1002;
	st_io.o_distribute_p4_lockunlock_switch_lamp						= 1003;

	st_io.i_distribute_p1_lockunlock_switch_chk							= 1008;
	st_io.i_distribute_p2_lockunlock_switch_chk							= 1009;
	st_io.i_distribute_p3_lockunlock_switch_chk							= 1010;
	st_io.i_distribute_p4_lockunlock_switch_chk							= 1011;

	for ( i = 0; i < 4; i++)
	{
		st_io.i_bcr_label_chk[i]										= 712 + i;
		st_io.o_distribute_lockunlock_switch_lamp[i]					= 1000 + i;
		st_io.i_distribute_lockunlock_switch_chk[i]						= 1008 + i;
	}

	st_io.o_distribute_p1_lock											= 1100;
	st_io.o_distribute_p2_lock											= 1101;
	st_io.o_distribute_p3_lock											= 1102;
	st_io.o_distribute_p4_lock											= 1103;

	st_io.i_distribute_p1_lock_chk										= 1104;
	st_io.i_distribute_p1_unlock_chk									= 1105;
	st_io.i_distribute_p1_lock_position_chk								= 1106;
	st_io.i_distribute_p2_lock_chk										= 1107;
	st_io.i_distribute_p2_unlock_chk									= 1108;
	st_io.i_distribute_p2_lock_position_chk								= 1109;
	st_io.i_distribute_p3_lock_chk										= 1110;
	st_io.i_distribute_p3_unlock_chk									= 1111;
	st_io.i_distribute_p3_lock_position_chk								= 1112;
	st_io.i_distribute_p4_lock_chk										= 1113;
	st_io.i_distribute_p4_unlock_chk									= 1114;
	st_io.i_distribute_p4_lock_position_chk								= 1115;

	for ( i = 0; i < 4; i++ )
	{
		st_io.o_distribute_p_lock[i]									= 1100 + i;//2014.1018 (i*3) -> i;
		st_io.i_distribute_lock_chk[i]									= 1104 + (i*3);
		st_io.i_distribute_unlock_chk[i]								= 1105 + (i*3);
		st_io.i_distribute_lock_position_chk[i]							= 1106 + (i*3);
	}

	st_io.o_distribute_p1_plate_updown									= 1200;
	st_io.o_distribute_p2_plate_updown									= 1201;
	st_io.o_distribute_p3_plate_updown									= 1202;
	st_io.o_distribute_p4_plate_updown									= 1203;
	st_io.i_distribute_p1_plate_up_chk									= 1204;
	st_io.i_distribute_p1_plate_down_chk								= 1205;
	st_io.i_distribute_p2_plate_up_chk									= 1206;
	st_io.i_distribute_p2_plate_down_chk								= 1207;
	st_io.i_distribute_p3_plate_up_chk									= 1208;
	st_io.i_distribute_p3_plate_down_chk								= 1209;
	st_io.i_distribute_p4_plate_up_chk									= 1210;
	st_io.i_distribute_p4_plate_down_chk								= 1211;

	for ( i = 0; i < 4; i++ )
	{
		st_io.o_distribute_plate_updown[i]								= 1200 + i;
		st_io.i_distribute_plate_up_chk[i]								= 1204 + (i*2);
		st_io.i_distribute_plate_down_chk[i]							= 1205 + (i*2);
	}

	st_io.o_distribute_p1_plate_vacuum									= 1300;
	st_io.o_distribute_p2_plate_vacuum									= 1301;
	st_io.o_distribute_p3_plate_vacuum									= 1302;
	st_io.o_distribute_p4_plate_vacuum									= 1303;
	st_io.o_distribute_p1_eject_vacuum									= 1304;
	st_io.o_distribute_p2_eject_vacuum									= 1305;
	st_io.o_distribute_p3_eject_vacuum									= 1306;
	st_io.o_distribute_p4_eject_vacuum									= 1307;
	st_io.i_distribute_p1_plate_vacuum_chk								= 1308;
	st_io.i_distribute_p2_plate_vacuum_chk								= 1309;
	st_io.i_distribute_p3_plate_vacuum_chk								= 1310;
	st_io.i_distribute_p4_plate_vacuum_chk								= 1311;

	for ( i = 0; i < 4; i++ )
	{
		st_io.o_distribute_plate_vacuum[i]								= 1300 + i;
		st_io.o_distribute_eject_vacuum[i]								= 1304 + i;
		st_io.i_distribute_plate_vacuum_chk[i]							= 1308 + i;
	}

	st_io.o_distribute_rbt1_left_top_vacuum_fwdback						= 1400;
	st_io.o_distribute_rbt1_left_btm_vacuum_fwdback						= 1401;
	st_io.o_distribute_rbt1_left_box_guide_fwdback						= 1402;
//	st_io.o_distribute_rbt1_left_box_guide_updown						= 1403;
	st_io.o_distribute_rbt1_right_top_vacuum_fwdback					= 1404;
	st_io.o_distribute_rbt1_right_btm_vacuum_fwdback					= 1405;
	st_io.o_distribute_rbt1_right_box_guide_fwdback						= 1406;
//	st_io.o_distribute_rbt1_right_box_guide_updown						= 1407;
	st_io.o_distribute_rbt1_left_top_vacuum								= 1408;
	st_io.o_distribute_rbt1_left_top_eject								= 1409;
	st_io.o_distribute_rbt1_left_btm_vacuum								= 1410;
	st_io.o_distribute_rbt1_left_btm_eject								= 1411;
	st_io.o_distribute_rbt1_right_top_vacuum							= 1412;
	st_io.o_distribute_rbt1_right_top_eject								= 1413;
	st_io.o_distribute_rbt1_right_btm_vacuum							= 1414;
	st_io.o_distribute_rbt1_right_btm_eject								= 1415;

//	st_io.i_distribute_rbt1_left_top_vacuum_fwd_chk						= 1500;
	st_io.i_distribute_rbt1_left_top_vacuum_bwd_chk						= 1501;
//	st_io.i_distribute_rbt1_left_btm_vacuum_fwd_chk						= 1502;
	st_io.i_distribute_rbt1_left_btm_vacuum_bwd_chk						= 1503;
//	st_io.i_distribute_rbt1_left_box_guide_fwd_chk						= 1504;
	st_io.i_distribute_rbt1_left_box_guide_bwd_chk						= 1505;
//	st_io.i_distribute_rbt1_left_box_guide_up_chk						= 1506;
//	st_io.i_distribute_rbt1_left_box_guide_down_chk						= 1507;
	st_io.i_distribute_rbt1_left_top_vacuum_chk							= 1508;
	st_io.i_distribute_rbt1_left_btm_vacuum_chk							= 1509;
	////2014,1203
	st_io.i_distribute_rbt1_left_box_rotator_fwd_chk					= 1510;
	st_io.i_distribute_rbt1_left_box_rotator_bwd_chk					= 1511;
	st_io.o_distribute_rbt1_left_box_rotator_fwd_cylinder				= 1512;
	st_io.o_distribute_rbt1_left_box_rotator_bwd_cylinder				= 1513;
	////

	
//	st_io.i_distribute_rbt1_right_top_vacuum_fwd_chk					= 1600;
	st_io.i_distribute_rbt1_right_top_vacuum_bwd_chk					= 1601;
//	st_io.i_distribute_rbt1_right_btm_vacuum_fwd_chk					= 1602;
	st_io.i_distribute_rbt1_right_btm_vacuum_bwd_chk					= 1603;
//	st_io.i_distribute_rbt1_right_box_guide_fwd_chk						= 1604;
	st_io.i_distribute_rbt1_right_box_guide_bwd_chk						= 1605;
//	st_io.i_distribute_rbt1_right_box_guide_up_chk						= 1606;
//	st_io.i_distribute_rbt1_right_box_guide_down_chk					= 1607;
	st_io.i_distribute_rbt1_right_top_vacuum_chk						= 1608;
	st_io.i_distribute_rbt1_right_btm_vacuum_chk						= 1609;
	////2014,1203
	st_io.i_distribute_rbt1_right_box_rotator_fwd_chk					= 1610;
	st_io.i_distribute_rbt1_right_box_rotator_bwd_chk					= 1611;
	st_io.o_distribute_rbt1_right_box_rotator_fwd_cylinder				= 1612;
	st_io.o_distribute_rbt1_right_box_rotator_bwd_cylinder				= 1613;
	////
	
	st_io.o_distribute_rbt2_left_top_vacuum_fwdback						= 1700;
	st_io.o_distribute_rbt2_left_btm_vacuum_fwdback						= 1701;
	st_io.o_distribute_rbt2_left_box_guide_fwdback						= 1702;
//	st_io.o_distribute_rbt2_left_box_guide_updown						= 1703;
	st_io.o_distribute_rbt2_right_top_vacuum_fwdback					= 1704;
	st_io.o_distribute_rbt2_right_btm_vacuum_fwdback					= 1705;
	st_io.o_distribute_rbt2_right_box_guide_fwdback						= 1706;
//	st_io.o_distribute_rbt2_right_box_guide_updown						= 1707;
	st_io.o_distribute_rbt2_left_top_vacuum								= 1708;
	st_io.o_distribute_rbt2_left_top_eject								= 1709;
	st_io.o_distribute_rbt2_left_btm_vacuum								= 1710;
	st_io.o_distribute_rbt2_left_btm_eject								= 1711;
	st_io.o_distribute_rbt2_right_top_vacuum							= 1712;
	st_io.o_distribute_rbt2_right_top_eject								= 1713;
	st_io.o_distribute_rbt2_right_btm_vacuum							= 1714;
	st_io.o_distribute_rbt2_right_btm_eject								= 1715;

//	st_io.i_distribute_rbt2_left_top_vacuum_fwd_chk						= 2000;
	st_io.i_distribute_rbt2_left_top_vacuum_bwd_chk						= 2001;
//	st_io.i_distribute_rbt2_left_btm_vacuum_fwd_chk						= 2002;
	st_io.i_distribute_rbt2_left_btm_vacuum_bwd_chk						= 2003;
//	st_io.i_distribute_rbt2_left_box_guide_fwd_chk						= 2004;
	st_io.i_distribute_rbt2_left_box_guide_bwd_chk						= 2005;
//	st_io.i_distribute_rbt2_left_box_guide_up_chk						= 2006;
//	st_io.i_distribute_rbt2_left_box_guide_down_chk						= 2007;
	st_io.i_distribute_rbt2_left_top_vacuum_chk							= 2008;
	st_io.i_distribute_rbt2_left_btm_vacuum_chk							= 2009;

	////2014,1203
	st_io.i_distribute_rbt2_left_box_rotator_fwd_chk					= 2010;
	st_io.i_distribute_rbt2_left_box_rotator_bwd_chk					= 2011;
	st_io.o_distribute_rbt2_left_box_rotator_fwd_cylinder				= 2012;
	st_io.o_distribute_rbt2_left_box_rotator_bwd_cylinder				= 2013;
	////

//	st_io.i_distribute_rbt2_right_top_vacuum_fwd_chk					= 2100;
	st_io.i_distribute_rbt2_right_top_vacuum_bwd_chk					= 2101;
//	st_io.i_distribute_rbt2_right_btm_vacuum_fwd_chk					= 2102;
	st_io.i_distribute_rbt2_right_btm_vacuum_bwd_chk					= 2103;
//	st_io.i_distribute_rbt2_right_box_guide_fwd_chk						= 2104;
	st_io.i_distribute_rbt2_right_box_guide_bwd_chk						= 2105;
//	st_io.i_distribute_rbt2_right_box_guide_up_chk						= 2106;
//	st_io.i_distribute_rbt2_right_box_guide_down_chk					= 2107;
	st_io.i_distribute_rbt2_right_top_vacuum_chk						= 2108;
	st_io.i_distribute_rbt2_right_btm_vacuum_chk						= 2109;

	////2014,1203
	st_io.i_distribute_rbt2_right_box_rotator_fwd_chk					= 2110;
	st_io.i_distribute_rbt2_right_box_rotator_bwd_chk					= 2111;
	st_io.o_distribute_rbt2_right_box_rotator_fwd_cylinder				= 2112;
	st_io.o_distribute_rbt2_right_box_rotator_bwd_cylinder				= 2113;
	////


	for ( i = 0;i < 2; i++)
	{
		st_io.o_distribute_rbt1_top_vacuum_fwdback[i]					= 1400 + (i*4);//left,right
		st_io.o_distribute_rbt1_btm_vacuum_fwdback[i]					= 1401 + (i*4);
		st_io.o_distribute_rbt1_box_guide_fwdback[i]					= 1402 + (i*4);
//		st_io.o_distribute_rbt1_box_guide_updown[i]						= 1403 + (i*4);
		st_io.o_distribute_rbt1_top_vacuum[i]							= 1408 + (i*4);
		st_io.o_distribute_rbt1_top_eject[i]							= 1409 + (i*4);
		st_io.o_distribute_rbt1_btm_vacuum[i]							= 1410 + (i*4);
		st_io.o_distribute_rbt1_btm_eject[i]							= 1411 + (i*4);
		
		st_io.o_distribute_rbt2_top_vacuum_fwdback[i]					= 1700 + (i*4);//left,right
		st_io.o_distribute_rbt2_btm_vacuum_fwdback[i]					= 1701 + (i*4);
		st_io.o_distribute_rbt2_box_guide_fwdback[i]					= 1702 + (i*4);
//		st_io.o_distribute_rbt2_box_guide_updown[i]						= 1703 + (i*4);
		st_io.o_distribute_rbt2_top_vacuum[i]							= 1708 + (i*4);
		st_io.o_distribute_rbt2_top_eject[i]							= 1709 + (i*4);
		st_io.o_distribute_rbt2_btm_vacuum[i]							= 1710 + (i*4);
		st_io.o_distribute_rbt2_btm_eject[i]							= 1711 + (i*4);

//		st_io.i_distribute_rbt1_top_vacuum_fwd_chk[i]					= 1500 + (i*100);
		st_io.i_distribute_rbt1_top_vacuum_bwd_chk[i]					= 1501 + (i*100);
//		st_io.i_distribute_rbt1_btm_vacuum_fwd_chk[i]					= 1502 + (i*100);
		st_io.i_distribute_rbt1_btm_vacuum_bwd_chk[i]					= 1503 + (i*100);
//		st_io.i_distribute_rbt1_box_guide_fwd_chk[i]					= 1504 + (i*100);
		st_io.i_distribute_rbt1_box_guide_back_chk[i]					= 1505 + (i*100);
//		st_io.i_distribute_rbt1_box_guide_up_chk[i]						= 1506 + (i*100);
//		st_io.i_distribute_rbt1_box_guide_down_chk[i]					= 1507 + (i*100);
		st_io.i_distribute_rbt1_top_vacuum_chk[i]						= 1508 + (i*100);
		st_io.i_distribute_rbt1_btm_vacuum_chk[i]						= 1509 + (i*100);

//		st_io.i_distribute_rbt2_top_vacuum_fwd_chk[i]					= 2000 + (i*100);
		st_io.i_distribute_rbt2_top_vacuum_bwd_chk[i]					= 2001 + (i*100);
//		st_io.i_distribute_rbt2_btm_vacuum_fwd_chk[i]					= 2002 + (i*100);
		st_io.i_distribute_rbt2_btm_vacuum_bwd_chk[i]					= 2003 + (i*100);
//		st_io.i_distribute_rbt2_box_guide_fwd_chk[i]					= 2004 + (i*100);
		st_io.i_distribute_rbt2_box_guide_back_chk[i]					= 2005 + (i*100);
//		st_io.i_distribute_rbt2_box_guide_up_chk[i]						= 2006 + (i*100);
//		st_io.i_distribute_rbt2_box_guide_down_chk[i]					= 2007 + (i*100);
		st_io.i_distribute_rbt2_top_vacuum_chk[i]						= 2008 + (i*100);
		st_io.i_distribute_rbt2_btm_vacuum_chk[i]						= 2009 + (i*100);

		////2014,1203
		st_io.i_distribute_rbt1_box_rotator_fwd_chk[i]    				= 1510 + (i*100);
		st_io.i_distribute_rbt1_box_rotator_bwd_chk[i]    				= 1511 + (i*100);
		st_io.o_distribute_rbt1_box_rotator_fwd_cylinder[i]    			= 1512 + (i*100);
		st_io.o_distribute_rbt1_box_rotator_bwd_cylinder[i]    			= 1513 + (i*100);

		st_io.i_distribute_rbt2_box_rotator_fwd_chk[i]    				= 2010 + (i*100);
		st_io.i_distribute_rbt2_box_rotator_bwd_chk[i]    				= 2011 + (i*100);
		st_io.o_distribute_rbt2_box_rotator_fwd_cylinder[i]    			= 2012 + (i*100);
		st_io.o_distribute_rbt2_box_rotator_bwd_cylinder[i]    			= 2013 + (i*100);
		////

	}
	
	st_io.o_reject_rbt_updown											= 2200;
	st_io.o_reject_rbt_vacuum											= 2201;
	st_io.o_reject_rbt_eject											= 2202;

	st_io.o_density_vision_trigger_onoff								= 2204;
	st_io.o_distribute_vision_trigger_onoff								= 2205;
	st_io.o_top_vision_trigger_onoff									= 2206;
	st_io.o_btm_vision_trigger_onoff									= 2207;

	st_io.i_reject_rbt_up_chk											= 2208;
	st_io.i_reject_rbt_down_chk											= 2209;
	st_io.i_reject_rbt_vacuum_chk										= 2210;
	st_io.i_reject_cv_full_chk											= 2211;
	st_io.i_reject_cv_1step_pos_chk										= 2212;
	st_io.i_reject_cv_full_pos_chk										= 2213;

	st_io.o_uld_rbt_left_updown											= 2300;
	st_io.o_uld_rbt_left_vacuum											= 2301;
	st_io.o_uld_rbt_right_updown										= 2302;
	st_io.o_uld_rbt_right_vacuum										= 2303;
	st_io.o_recycle_door_onoff_cylinder									= 2304;//2014,1207

	st_io.i_uld_rbt_left_up_chk											= 2308;
	st_io.i_uld_rbt_left_down_chk										= 2309;
	st_io.i_uld_rbt_left_vacuum_chk										= 2310;
	st_io.i_uld_rbt_right_up_chk										= 2311;
	st_io.i_uld_rbt_right_down_chk										= 2312;
	st_io.i_uld_rbt_right_vacuum_chk									= 2313;
    st_io.recycle_door_lock_chk											= 2314;//2014,1207
    st_io.recycle_door_unlock_chk										= 2315;//2014,1207

	for ( i = 0;i < 2; i++)
	{
		st_io.o_uld_rbt_picker[i]										= 2300 + (i*2);
		st_io.o_uld_rbt_vacuum[i]										= 2301 + (i*2);
		st_io.i_uld_rbt_pickerup_chk[i]									= 2308 + (i*3);
		st_io.i_uld_rbt_pickerdn_chk[i]									= 2309 + (i*3);
		st_io.i_uld_rbt_vacuum_chk[i]									= 2310 + (i*3);
	}

	

	st_io.o_recycle_zone_box_pusher_fwd									= 2400;
	st_io.o_recycle_zone_box_pusher_back								= 2401;
	st_io.o_recycle_zone_box_vacuum										= 2402;

	st_io.i_recycle_zone_box_pusher_fwd_chk								= 2405;
	st_io.i_recycle_zone_box_pusher_bwd_chk								= 2406;
	st_io.i_recycle_zone_box_vacuum_chk									= 2407;
	st_io.i_recycle_zone_box_chk										= 2408;
	st_io.i_recycle_zone_box1_chk										= 2409;
	st_io.i_recycle_zone_box2_chk										= 2410;
	st_io.i_recycle_zone_light_curtain_chk								= 2411;//2014,1207

	st_io.i_uld_cv_left_box_chk											= 2412;
	st_io.i_uld_cv_right_box_chk										= 2413;
	st_io.i_uld_cv_box_out_chk											= 2414;
    st_io.i_uld_cv_box_full_chk											= 2415;//2014,1207

	for( i = 0; i < 3; i++)
	{
		st_io.i_uld_cv_box[i]											= 2412 + i;
	}

	st_io.o_top_idbuffer_ld1_shifter_hook_fwd							= 2500;
	st_io.o_top_idbuffer_ld1_shifter_hook_bwd							= 2501;
	st_io.o_top_idbuffer_ld1_shifter_hook_clamp							= 2502;
	st_io.o_top_idbuffer_ld1_shifter_hook_unclamp						= 2503;
	st_io.o_top_idbuffer_ld2_shifter_left_hook_clamp					= 2504;
	st_io.o_top_idbuffer_ld2_shifter_left_hook_unclamp					= 2505;
	st_io.o_top_idbuffer_ld2_shifter_right_hook_clamp					= 2506;
	st_io.o_top_idbuffer_ld2_shifter_right_hook_unclamp					= 2507;
	st_io.i_top_idbuffer_ld1_shifter_hook_fwd_chk						= 2508;
	st_io.i_top_idbuffer_ld1_shifter_hook_bwd_chk						= 2509;
	st_io.i_top_idbuffer_ld1_shifter_hook_clamp_chk						= 2510;
	st_io.i_top_idbuffer_ld1_shifter_hook_unclamp_chk					= 2511;
	st_io.i_top_idbuffer_ld2_shifter_left_hook_clamp_chk				= 2512;
	st_io.i_top_idbuffer_ld2_shifter_left_hook_unclamp_chk				= 2513;
	st_io.i_top_idbuffer_ld2_shifter_right_hook_clamp_chk				= 2514;
	st_io.i_top_idbuffer_ld2_shifter_right_hook_unclamp_chk				= 2515;

	st_io.o_top_idbuffer_ld2_shifter_hook_fwd							= 2600;
	st_io.o_top_idbuffer_ld2_shifter_hook_bwd							= 2601;
	st_io.o_top_idbuffer12_clamp										= 2602;
	st_io.o_top_idbuffer12_unclamp										= 2603;
	st_io.o_top_idbuffer34_clamp										= 2604;
	st_io.o_top_idbuffer34_unclamp										= 2605;
	st_io.o_top_idbuffer56_clamp										= 2606;
	st_io.o_top_idbuffer56_unclamp										= 2607;
	st_io.o_top_idbuffer78_clamp										= 2608;
	st_io.o_top_idbuffer78_unclamp										= 2609;
	st_io.o_top_left_box_supportup_clamp								= 2610;
	st_io.o_top_left_box_supportdn_clamp								= 2611;
	st_io.o_top_right_box_supportup_clamp								= 2612;
	st_io.o_top_right_box_supportdn_clamp								= 2613;

	for ( i = 0; i < 2; i++ )
	{
		st_io.o_top_box_up_clamp[i]										= 2610 + (i*2);
		st_io.o_top_box_dn_clamp[i]										= 2611 + (i*2);
	}

	st_io.i_top_idbuffer_ld2_shifter_hook_fwd_chk						= 2700;
	st_io.i_top_idbuffer_ld2_shifter_hook_bwd_chk						= 2701;
	st_io.i_top_idbuffer12_clamp_chk									= 2702;
	st_io.i_top_idbuffer12_unclamp_chk									= 2703;
	st_io.i_top_idbuffer1_chk											= 2704;
	st_io.i_top_idbuffer2_chk											= 2705;
	st_io.i_top_idbuffer_box1_chk										= 2706;
	st_io.i_top_idbuffer_box2_chk										= 2707;
	st_io.i_top_idbuffer34_clamp_chk									= 2708;
	st_io.i_top_idbuffer34_unclamp_chk									= 2709;
	st_io.i_top_idbuffer3_chk											= 2710;
	st_io.i_top_idbuffer4_chk											= 2711;
	st_io.i_top_idbuffer_box3_chk										= 2712;
	st_io.i_top_idbuffer_box4_chk										= 2713;
	////2014,1203
	st_io.i_btm_idbuffer_uld1_shifter_buffer1_chk						= 2714;
	st_io.i_btm_idbuffer_uld1_shifter_buffer2_chk						= 2715;
	////
	st_io.i_top_idbuffer56_clamp_chk									= 3000;
	st_io.i_top_idbuffer56_unclamp_chk									= 3001;
	st_io.i_top_idbuffer5_chk											= 3002;
	st_io.i_top_idbuffer6_chk											= 3003;
	st_io.i_top_idbuffer_box5_chk										= 3004;
	st_io.i_top_idbuffer_box6_chk										= 3005;
	st_io.i_top_idbuffer78_clamp_chk									= 3006;
	st_io.i_top_idbuffer78_unclamp_chk									= 3007;
	st_io.i_top_idbuffer7_chk											= 3008;
	st_io.i_top_idbuffer8_chk											= 3009;
	st_io.i_top_idbuffer_box7_chk										= 3010;
	st_io.i_top_idbuffer_box8_chk										= 3011;

	st_io.i_top_idbuffer_shifter_hook_fwd[0]							= 2508;
	st_io.i_top_idbuffer_shifter_hook_bwd[0]							= 2509;
	st_io.i_top_idbuffer_shifter_hook_fwd[1]							= 2700;
	st_io.i_top_idbuffer_shifter_hook_bwd[1]							= 2701;

	for ( i = 0; i < 2; i++ )
	{
		st_io.o_top_idbuffer_shifter_hook_fwd[i]						= 2500 + (i*100);
		st_io.o_top_idbuffer_shifter_hook_bwd[i]						= 2501 + (i*100);
		st_io.o_uld_rbt_eject[i]										= 3014 + i;
	}
	st_io.i_top_idbuffer_shifter_hook_fwd[0]							= 2508;
	st_io.i_top_idbuffer_shifter_hook_bwd[0]							= 2509;
	st_io.i_top_idbuffer_shifter_hook_fwd[1]							= 2700;
	st_io.i_top_idbuffer_shifter_hook_bwd[1]							= 2701;

	for ( i = 0; i < HOOKCLAMP_CNT; i++)
	{
		st_io.o_top_idbuffer_shifter_hook_clamp[i]						= 2502 + (i*2);
		st_io.o_top_idbuffer_shifter_hook_unclamp[i]					= 2503 + (i*2);
		st_io.i_top_idbuffer_shifter_hook_clamp[i]						= 2510 + (i*2);
		st_io.i_top_idbuffer_shifter_hook_unclamp[i]					= 2511 + (i*2);
	}
	
	for ( i = 0; i < 4; i++)
	{
		st_io.o_top_idbuffer_clamp[i]									= 2602 + (i*2);
		st_io.o_top_idbuffer_unclamp[i]									= 2603 + (i*2);
	}
	for ( i = 0; i < 2; i++)
	{
		st_io.i_top_idbuffer_clamp[i]									= 2702 + (i*6);
		st_io.i_top_idbuffer_unclamp[i]									= 2703 + (i*6);
		st_io.i_top_idbuffer_chk[i][0]									= 2704 + (i*6);
		st_io.i_top_idbuffer_chk[i][1]									= 2705 + (i*6);
		st_io.i_top_idbuffer_box[i][0]									= 2706 + (i*6);
		st_io.i_top_idbuffer_box[i][1]									= 2707 + (i*6);
	}
	for ( i = 2; i < 4; i++)
	{
		st_io.i_top_idbuffer_clamp[i]									= 3000 + ((i-2)*6);
		st_io.i_top_idbuffer_unclamp[i]									= 3001 + ((i-2)*6);
		st_io.i_top_idbuffer_chk[i][0]									= 3002 + ((i-2)*6);
		st_io.i_top_idbuffer_chk[i][1]									= 3003 + ((i-2)*6);
		st_io.i_top_idbuffer_box[i][0]									= 3004 + ((i-2)*6);
		st_io.i_top_idbuffer_box[i][1]									= 3005 + ((i-2)*6);
	}

//	st_io.o_rolling_align_pusher_fwdback								= 3100;
//	st_io.o_rolling1_pusher_fwdback										= 3101;
//	st_io.o_rolling2_pusher_fwdback										= 3102;
	st_io.o_rolling1_pusher_fwdback_left								= 3100;
	st_io.o_rolling1_pusher_fwdback_right								= 3101;
	st_io.o_rolling2_pusher_fwdback_left								= 3102;
	st_io.o_rolling2_pusher_fwdback_right								= 3103;

	st_io.i_rolling_align_pusher_left_fwd_chk							= 3104;
	st_io.i_rolling_align_pusher_left_back_chk							= 3105;
	st_io.i_rolling_align_pusher_right_fwd_chk							= 3106;
	st_io.i_rolling_align_pusher_right_back_chk							= 3107;
	st_io.i_rolling1_pusher_left_fwd_chk								= 3108;
	st_io.i_rolling1_pusher_left_back_chk								= 3109;
	st_io.i_rolling1_pusher_right_fwd_chk								= 3110;
	st_io.i_rolling1_pusher_right_back_chk								= 3111;
	st_io.i_rolling2_pusher_left_fwd_chk								= 3112;
	st_io.i_rolling2_pusher_left_back_chk								= 3113;
	st_io.i_rolling2_pusher_right_fwd_chk								= 3114;
	st_io.i_rolling2_pusher_right_back_chk								= 3115;

	for ( i = 0; i<2; i++ )
	{
		for ( j = 0; j < 2; j++ )
		{
			st_io.o_rolling_push_fwdback[i][j]							= 3100 + (i*2) + j;
		}
	}

	for ( i = 0; i<2; i++ )
	{
		for ( j = 0; j < 2; j++ )
		{
			st_io.i_rolling_push_fwd_chk[i][j]							= 3108 + (i*4) + (j*2);
			st_io.i_rolling_push_bwd_chk[i][j]							= 3109 + (i*4) + (j*2);
		}
	}


	st_io.o_idbuffer_elv_hook_clamp										= 3200;
	st_io.o_idbuffer_elv_hook_unclamp									= 3201;
	st_io.o_idbuffer_elv_pusher_fwd										= 3202;
	st_io.o_idbuffer_elv_pusher_bwd										= 3203;


	st_io.i_idbuffer_elv_hook_clamp_chk									= 3208;
	st_io.i_idbuffer_elv_hook_unclamp_chk								= 3209;
	st_io.i_idbuffer_elv_idbuffer1_chk									= 3210;
	st_io.i_idbuffer_elv_idbuffer2_chk									= 3211;
	st_io.i_idbuffer_elv_box1_chk										= 3212;
	st_io.i_idbuffer_elv_box2_chk										= 3213;
	st_io.i_idbuffer_elv_pusher_fwd_chk									= 3214;
	st_io.i_idbuffer_elv_pusher_bwd_chk									= 3215;

	st_io.o_btm_idbuffer_uld1_shifter_hook_fwd							= 3300;
	st_io.o_btm_idbuffer_uld1_shifter_hook_back							= 3301;
	st_io.o_btm_idbuffer12_clamp										= 3302;
	st_io.o_btm_idbuffer12_unclamp										= 3303;
	st_io.o_btm_idbuffer_uld2_shifter_hook_fwd							= 3304;
	st_io.o_btm_idbuffer_uld2_shifter_hook_back							= 3305;
	st_io.i_btm_idbuffer_uld1_shifter_hook_fwd_chk						= 3306;
	st_io.i_btm_idbuffer_uld1_shifter_hook_back_chk						= 3307;
	st_io.i_btm_idbuffer12_clamp_chk									= 3308;
	st_io.i_btm_idbuffer12_unclamp_chk									= 3309;
	st_io.i_btm_idbuffer1_chk											= 3310;
	st_io.i_btm_idbuffer2_chk											= 3311;
	st_io.i_top_left_box_supportup_chk									= 3312;
	st_io.i_top_left_box_supportdn_chk									= 3313;
	st_io.i_top_right_box_supportup_chk									= 3314;
	st_io.i_top_right_box_supportdn_chk									= 3315;

//	st_io.i_btm_idbuffer_uld2_shifter_hook_fwd1							= 3312;
//	st_io.i_btm_idbuffer_uld2_shifter_hook_back1						= 3313;
//	st_io.i_btm_idbuffer_uld2_shifter_hook_fwd2							= 3314;
//	st_io.i_btm_idbuffer_uld2_shifter_hook_back2						= 3315;

	st_io.o_btm_idbuffer34_clamp										= 3400;
	st_io.o_btm_idbuffer34_unclamp										= 3401;
	st_io.o_btm_idbuffer_stopper_updown									= 3402;

	st_io.i_btm_idbuffer_uld2_shifter_hook_fwd1_chk						= 3404;
	st_io.i_btm_idbuffer_uld2_shifter_hook_back1_chk					= 3405;
	st_io.i_btm_idbuffer_uld2_shifter_hook_fwd2_chk						= 3406;
	st_io.i_btm_idbuffer_uld2_shifter_hook_back2_chk					= 3407;
	st_io.i_btm_idbuffer34_clamp_chk									= 3408;
	st_io.i_btm_idbuffer34_unclamp_chk									= 3409;
	st_io.i_btm_idbuffer3_chk											= 3410;
	st_io.i_btm_idbuffer4_chk											= 3411;
	st_io.i_btm_idbuffer_stopper_up_chk									= 3412;
	st_io.i_btm_idbuffer_stopper_down_chk								= 3413;
	st_io.i_btm_idbuffer_stopper_buffer_chk								= 3414;
	st_io.i_btm_idbuffer_8480_hook_in_chk								= 3415;

	for ( i = 0; i < 2; i++ )
	{
		st_io.o_btm_idbuffer_shifter_hook_fwd[i]						= 3300 + (i*4);
		st_io.o_btm_idbuffer_shifter_hook_bwd[i]						= 3301 + (i*4);

		st_io.i_btm_idbuffer_chk[i][0]									= 3310 + (i*100);
		st_io.i_btm_idbuffer_chk[i][1]									= 3311 + (i*100);

		st_io.i_top_box_up_clamp_chk[i]									= 3312 + (i*2);
		st_io.i_top_box_dn_clamp_chk[i]									= 3313 + (i*2);
	}
	st_io.i_btm_idbuffer_shifter_hook_fwd[0]							= 3306;
	st_io.i_btm_idbuffer_shifter_hook_bwd[0]							= 3307;
	for (i = 1; i < 3; i++)
	{
		st_io.i_btm_idbuffer_shifter_hook_fwd[i]						= 3404 + ((i-1)*2);
		st_io.i_btm_idbuffer_shifter_hook_bwd[i]						= 3405 + ((i-1)*2);
	}
	st_io.o_btm_idbuffer_clamp[0]										= 3302;
	st_io.o_btm_idbuffer_unclamp[0]										= 3303;
	st_io.o_btm_idbuffer_clamp[1]										= 3400;
	st_io.o_btm_idbuffer_unclamp[1]										= 3401;

//	st_io.i_btm_idbuffer_shifter_clamp									= 3306;
//	st_io.i_btm_idbuffer_shifter_unclamp								= 3307;
//	st_io.i_btm_idbuffer_shifter_clamp[1]								= 3312;
//	st_io.i_btm_idbuffer_shifter_unclamp[1]								= 3313;
//	st_io.i_btm_idbuffer_shifter_clamp[2]								= 3314;
//	st_io.i_btm_idbuffer_shifter_unclamp[2]								= 3315;

	st_io.i_btm_idbuffer_clamp[0]										= 3308;
	st_io.i_btm_idbuffer_unclamp[0]										= 3309;
// 	st_io.i_btm_idbuffer_clamp[1]										= 3400;
// 	st_io.i_btm_idbuffer_unclamp[1]										= 3401;//2016.0614
	st_io.i_btm_idbuffer_clamp[1]										= 3408;
	st_io.i_btm_idbuffer_unclamp[1]										= 3409;



	st_io.i_top_idbuffer_exist_sensor_check[0] = st_io.i_top_idbuffer1_chk;				
	st_io.i_top_idbuffer_exist_sensor_check[1] = st_io.i_top_idbuffer2_chk;				
	st_io.i_top_idbuffer_exist_sensor_check[2] = st_io.i_top_idbuffer3_chk;				
	st_io.i_top_idbuffer_exist_sensor_check[3] = st_io.i_top_idbuffer4_chk;				
	st_io.i_top_idbuffer_exist_sensor_check[4] = st_io.i_top_idbuffer5_chk;				
	st_io.i_top_idbuffer_exist_sensor_check[5] = st_io.i_top_idbuffer6_chk;				
	st_io.i_top_idbuffer_exist_sensor_check[6] = st_io.i_top_idbuffer7_chk;				
	st_io.i_top_idbuffer_exist_sensor_check[7] = st_io.i_top_idbuffer8_chk;				
	st_io.i_top_idbuffer_exist_sensor_check[8] = st_io.i_idbuffer_elv_idbuffer1_chk;				
	st_io.i_top_idbuffer_exist_sensor_check[9] = st_io.i_idbuffer_elv_idbuffer2_chk;
				
	st_io.i_top_idbuffer_exist_sensor_check[10] = st_io.i_top_idbuffer_box1_chk;				
	st_io.i_top_idbuffer_exist_sensor_check[11] = st_io.i_top_idbuffer_box2_chk;				
	st_io.i_top_idbuffer_exist_sensor_check[12] = st_io.i_top_idbuffer_box3_chk;				
	st_io.i_top_idbuffer_exist_sensor_check[13] = st_io.i_top_idbuffer_box4_chk;				
	st_io.i_top_idbuffer_exist_sensor_check[14] = st_io.i_top_idbuffer_box5_chk;				
	st_io.i_top_idbuffer_exist_sensor_check[15] = st_io.i_top_idbuffer_box6_chk;				
	st_io.i_top_idbuffer_exist_sensor_check[16] = st_io.i_top_idbuffer_box7_chk;				
	st_io.i_top_idbuffer_exist_sensor_check[17] = st_io.i_top_idbuffer_box8_chk;				
	st_io.i_top_idbuffer_exist_sensor_check[18] = st_io.i_idbuffer_elv_box1_chk;				
	st_io.i_top_idbuffer_exist_sensor_check[19] = st_io.i_idbuffer_elv_box2_chk;	

	st_io.i_btm_idbuffer_exist_sensor_check[0] = st_io.i_btm_idbuffer1_chk;
	st_io.i_btm_idbuffer_exist_sensor_check[1] = st_io.i_btm_idbuffer2_chk;
	st_io.i_btm_idbuffer_exist_sensor_check[2] = st_io.i_btm_idbuffer3_chk;
	st_io.i_btm_idbuffer_exist_sensor_check[3] = st_io.i_btm_idbuffer4_chk;
	//2014,1205
	st_io.i_btm_idbuffer_exist_sensor_check[4] = st_io.i_btm_idbuffer_uld1_shifter_buffer1_chk;
	st_io.i_btm_idbuffer_exist_sensor_check[5] = st_io.i_btm_idbuffer_uld1_shifter_buffer2_chk;



	//S'Y  : TL & DOOR
	st_io.o_start_switch_lamp											= 3500;									
	st_io.o_stop_switch_lamp											= 3501;										
	st_io.o_alarm_clear_lamp											= 3502;										
	st_io.o_buzzer_off_lamp												= 3503;										
	st_io.o_reject_cv_motor_onoff										= 3504;
	st_io.o_uld_cv_motor_onoff											= 3505;
	st_io.o_recycle_zone_door_switch_lamp								= 3506;//2014,1203

	st_io.i_start_switch_chk											= 3508;										
	st_io.i_stop_switch_chk												= 3509;										
	st_io.i_alarm_clear_switch_chk										= 3510;								
	st_io.i_buzzer_switch_chk											= 3511;									
	////2014,1203
	st_io.i_recycle_zone_door_switch_chk								= 3512;									
//	st_io.i_auto_mode_switch_chk										= 3512;									
//	st_io.i_manual_mode_switch_chk										= 3513;								
	////
	st_io.i_front_select_switch_chk										= 3514;								
	st_io.i_rear_select_switch_chk										= 3515;								

	//st_io.o_switch_lamp[4];

	//S'Y  : MC & EMO
	st_io.o_main_air													= 3600;												
	st_io.o_towerlamp_green												= 3601;										
	st_io.o_towerlamp_yellow											= 3602;										
	st_io.o_towerlamp_red												= 3603;										
	st_io.o_buzzer_1													= 3604;												
	st_io.o_buzzer_2													= 3605;												
	st_io.o_buzzer_3													= 3606;
	st_io.o_door_lock													= 3607;											
	st_io.i_main_air_chk												= 3608;											
	
	st_io.i_mc1_chk														= 3611;												
	st_io.i_emo_1_chk													= 3612;											
	st_io.i_emo_2_chk													= 3613;											
	st_io.i_emo_3_chk													= 3614;											
	st_io.i_emo_4_chk													= 3615;											

	st_io.i_door1_chk													= 3700;											
	st_io.i_door2_chk													= 3701;											
	st_io.i_door3_chk													= 3702;											
	st_io.i_door4_chk													= 3703;											
	st_io.i_door5_chk													= 3704;											
	st_io.i_door6_chk													= 3705;											
	st_io.i_door7_chk													= 3706;											
	st_io.i_door8_chk													= 3707;											
	st_io.i_door9_chk													= 3708;									
	st_io.i_door10_chk													= 3709;									
	st_io.i_recycle_zone_door_chk										= 3710;//2014,1203

	st_io.i_bottom_door1												= 3712;
	st_io.i_bottom_door2												= 3713;
	st_io.i_bottom_door3												= 3714;
	st_io.i_bottom_door4												= 3715;

	for ( i = 0; i < 3; i++ )
	{
		st_io.o_buzzer[i] = st_io.o_buzzer_1 + i;
	}
	for ( i = 0; i < 10; i++ )
	{
		st_io.i_door_chk[i]	= st_io.i_door1_chk + i;
	}
	for ( i = 0; i < 4; i++ )
	{
		st_io.i_emo_chk[i] = st_io.i_emo_1_chk + i;
	}

	//ASS'Y  : MC/EMO/SMEMA
	st_io.o_front_top_machine_enter_signal								= 4000;						
	st_io.o_front_top_machine_transfer_signal							= 4001;					
	st_io.o_front_btm_machine_ready_signal								= 4002;						
	st_io.o_front_btm_machine_stop_signal								= 4003;	
	st_io.o_rear_ready_signal											= 4004;
	st_io.o_rear_enter_signal											= 4005;

	st_io.i_front_top_machine_ready_signal_chk							= 4008;					
	st_io.i_front_top_machine_stop_signal_chk							= 4009;					
	st_io.i_front_btm_machine_request_signal_chk						= 4010;					
	st_io.i_front_btm_machine_transfer_signal_chk						= 4011;				
	st_io.i_rear_req_signal_chk											= 4012;				
	st_io.i_rear_transfer_signal_chk									= 4013;				

}

int IO_Manager::IO_Board_Initialize()
{
	INT HSSI_speed = 0;
	int Ret = 0;
	INT pMasterNo;
	
	if( !st_handler.mn_virtual_mode &&
		FAS_IO.Set_IO_BoardOpen(&pMasterNo, START_NOTHING) != STATUS_SUCCESS )
		return FALSE;
	
	if( IO_Port_AutoEnable( 0, 0 ) == FALSE )		return FALSE;
	if( IO_Port_AutoEnable( 0, 1 ) == FALSE )		return FALSE;
	if( IO_Port_AutoEnable( 0, 2 ) == FALSE )		return FALSE;
	if( IO_Port_AutoEnable( 0, 3 ) == FALSE )		return FALSE;
	if( IO_Port_AutoEnable( 1, 0 ) == FALSE )		return FALSE;
	
	
	if( IO_SlaveEnable( 0, 0, 8, TRUE ) == FALSE )	return FALSE;
	if( IO_SlaveEnable( 0, 1, 8, TRUE ) == FALSE )	return FALSE;
	if( IO_SlaveEnable( 0, 2, 8, TRUE ) == FALSE )	return FALSE;
	if( IO_SlaveEnable( 0, 3, 8, TRUE ) == FALSE )	return FALSE;
	
	
	if( IO_SlaveEnable( 1, 0, 8, TRUE ) == FALSE )  return FALSE;
	////
	
	for (int i = 0; i < 4; i++)
	{
		Ret = FAS_IO.Set_IO_HSSISpeed(0, i, PORT_SPEED_10);
		Ret = FAS_IO.Get_IO_HSSISpeed(0, i, &HSSI_speed);
	}
	Ret = FAS_IO.Set_IO_HSSISpeed(1, 0, PORT_SPEED_10);
	Ret = FAS_IO.Get_IO_HSSISpeed(1, 0, &HSSI_speed);

	
	FAS_IO.Set_IO_DefineWord(0, 0, 0, 0x00ff); // 0번 Master, 0번 포트, 0번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 0, 1, 0x00ff); // 0번 Master, 0번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 0, 2, 0xffff); // 0번 Master, 0번 포트, 2번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 0, 3, 0x0000); // 0번 Master, 0번 포트, 3번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 0, 4, 0xffff); // 0번 Master, 0번 포트, 4번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 0, 5, 0x00ff); // 0번 Master, 0번 포트, 5번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 0, 6, 0x0000); // 0번 Master, 0번 포트, 6번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 0, 7, 0x00ff); // 0번 Master, 0번 포트, 7번 슬레이브 , 16bit 셋팅
	
	FAS_IO.Set_IO_DefineWord(0, 1, 0, 0x00ff); // 0번 Master, 1번 포트, 0번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 1, 1, 0x000f); // 0번 Master, 1번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 1, 2, 0x000f); // 0번 Master, 1번 포트, 2번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 1, 3, 0x00ff); // 0번 Master, 1번 포트, 3번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 1, 4, 0xffff); // 0번 Master, 1번 포트, 4번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 1, 5, 0xf000); // 0번 Master, 1번 포트, 5번 슬레이브 , 16bit 셋팅 //2014,1203 0x0000 - > 0xf000 
	FAS_IO.Set_IO_DefineWord(0, 1, 6, 0xf000); // 0번 Master, 1번 포트, 6번 슬레이브 , 16bit 셋팅 //2014,1203 0x0000 - > 0xf000 
	FAS_IO.Set_IO_DefineWord(0, 1, 7, 0xffff); // 0번 Master, 1번 포트, 7번 슬레이브 , 16bit 셋팅
	
	FAS_IO.Set_IO_DefineWord(0, 2, 0, 0xf000); // 0번 Master, 2번 포트, 0번 슬레이브 , 16bit 셋팅 //2014,1203 0x0000 - > 0xf000
	FAS_IO.Set_IO_DefineWord(0, 2, 1, 0xf000); // 0번 Master, 2번 포트, 1번 슬레이브 , 16bit 셋팅 //2014,1203 0x0000 - > 0xf000 
	FAS_IO.Set_IO_DefineWord(0, 2, 2, 0xffff); // 0번 Master, 2번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 2, 3, 0x00ff); // 0번 Master, 2번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 2, 4, 0x001f); // 0번 Master, 2번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 2, 5, 0x00ff); // 0번 Master, 1번 포트, 5번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 2, 6, 0xffff); // 0번 Master, 1번 포트, 6번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 2, 7, 0x0000); // 0번 Master, 1번 포트, 7번 슬레이브 , 16bit 셋팅

	FAS_IO.Set_IO_DefineWord(0, 3, 0, 0xe000); // 0번 Master, 2번 포트, 0번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 3, 1, 0x000f); // 0번 Master, 2번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 3, 2, 0x00ff); // 0번 Master, 2번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 3, 3, 0x003f); // 0번 Master, 2번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 3, 4, 0x000f); // 0번 Master, 2번 포트, 1번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 3, 5, 0x00ff); // 0번 Master, 1번 포트, 5번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 3, 6, 0x00ff); // 0번 Master, 1번 포트, 6번 슬레이브 , 16bit 셋팅
	FAS_IO.Set_IO_DefineWord(0, 3, 7, 0x0000); // 0번 Master, 1번 포트, 7번 슬레이브 , 16bit 셋팅

	FAS_IO.Set_IO_DefineWord(1, 0, 0, 0x00ff); // 0번 Master, 2번 포트, 0번 슬레이브 , 16bit 셋팅
	SetIOBoardInit( true );
	
	return TRUE;
}

void IO_Manager::OnSet_IO_Out_Port_Clear()
{
	g_ioMgr.set_out_bit(st_io.o_buzzer_1, IO_OFF);
	g_ioMgr.set_out_bit(st_io.o_buzzer_2, IO_OFF);
	g_ioMgr.set_out_bit(st_io.o_buzzer_3, IO_OFF);
	g_ioMgr.set_out_bit(st_io.o_start_switch_lamp, IO_OFF);
	g_ioMgr.set_out_bit(st_io.o_stop_switch_lamp, IO_OFF);
	g_ioMgr.set_out_bit(st_io.o_alarm_clear_lamp, IO_OFF);
	g_ioMgr.set_out_bit(st_io.o_buzzer_off_lamp, IO_OFF);

	g_ioMgr.set_out_bit(st_io.o_recycle_zone_door_switch_lamp, IO_OFF);//2014,1205
	
	g_ioMgr.set_out_bit(st_io.o_main_air, IO_ON);
	
	if (g_ioMgr.Get_In_Bit(st_io.i_main_air_chk) != IO_ON)
	{
		g_ioMgr.set_out_bit(st_io.o_main_air, IO_OFF);
		
		Sleep(300);
		
		g_ioMgr.set_out_bit(st_io.o_main_air, IO_ON);
	}
	g_ioMgr.Set_Out_Bit(st_io.o_uld_cv_motor_onoff, IO_OFF);

	//smema
	g_ioMgr.set_out_bit(st_io.o_front_top_machine_enter_signal, IO_OFF);
	g_ioMgr.set_out_bit(st_io.o_front_top_machine_transfer_signal, IO_OFF);
	g_ioMgr.set_out_bit(st_io.o_front_btm_machine_ready_signal, IO_OFF);
	g_ioMgr.set_out_bit(st_io.o_front_btm_machine_stop_signal, IO_OFF);
	g_ioMgr.set_out_bit(st_io.o_rear_ready_signal, IO_OFF);
	g_ioMgr.set_out_bit(st_io.o_rear_enter_signal, IO_OFF);
}


int IO_Manager::IO_Port_AutoEnable( int iMasterID, int iPortNo )
{
	int nRtn = Set_IO_PortAutoEnable(iMasterID, iPortNo);
	if ( nRtn != CTL_YES)
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg, "%d Port, not enable", iPortNo);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}

		SetIOBoardInit( false );
		return FALSE;
	}

	return TRUE;
}

int IO_Manager::IO_SlaveEnable(int nMasterID, int nPortNo, int nSlaveCnt, BOOL bEnable)
{
	int nFailCnt = 0;
	for (int i = 0; i < nSlaveCnt; i++)
	{
		int nRet = Set_IO_SlaveEnable( nMasterID, nPortNo, i, bEnable);
		
		if (nRet != CTL_YES)
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "Slave #%d, not enable", i);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			i = -1;
			
			if (nFailCnt < 3)
			{
				nFailCnt++;
			}
			else
			{
				SetIOBoardInit( false );
				return FALSE;
			}
			continue;
		}
		
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			sprintf(st_msg.c_normal_msg, "Slave #%d, enable success", i);
			st_handler.cwnd_list->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 성공 출력 요청
		}
		Sleep(100);
	}

	return TRUE;
}

void IO_Manager::set_out_reverse( int nIONo )
{
	BOOL bOn = get_out_bit( nIONo, IO_ON );

	if( bOn == IO_ON )
	{
//		if( nIONo == st_io.o_bin_stacker1_rail_guide_fwdbwd &&
//			g_ioMgr.get_in_bit( st_io.i_bin_stacker1_tray_chk ) == IO_ON )
//		{
//			if ( g_local.GetLocalType() == LOCAL_ENG )//20131029
//			{
//				DoModal_Msg( "Tray Check." );
//			}
//			else
//			{
//				DoModal_Msg( "트레이가 있습니다." );
//			}
//			return;
//		}
//		else if( nIONo == st_io.o_bin_stacker2_rail_guide_fwdbwd &&
//			g_ioMgr.get_in_bit( st_io.i_bin_stacker2_tray_chk ) == IO_ON )
//		{
//			if ( g_local.GetLocalType() == LOCAL_ENG )//20131029
//			{
//				DoModal_Msg( "Tray Check." );
//			}
//			else
//			{
//				DoModal_Msg( "트레이가 있습니다." );
//			}
//			return;
//		}
//		else if( nIONo == st_io.o_bin_stacker3_rail_guide_fwdbwd &&
//			g_ioMgr.get_in_bit( st_io.i_bin_stacker3_tray_chk ) == IO_ON )
//		{
//			if ( g_local.GetLocalType() == LOCAL_ENG )//20131029
//			{
//				DoModal_Msg( "Tray Check." );
//			}
//			else
//			{
//				DoModal_Msg( "트레이가 있습니다." );
//			}
//			return;
//		}
	}
	
	set_out_bit( nIONo, !bOn );
}

void IO_Manager::set_out_reverse( int nFwd, int nBwd )
{
	BOOL bFwd = get_out_bit( nFwd, IO_ON );
	BOOL bBwd = get_out_bit( nBwd, IO_ON );

	if( bFwd && !bBwd )
	{
		set_out_bit( nFwd, IO_OFF );
		set_out_bit( nBwd, IO_ON );
	}
	else
	{
		set_out_bit( nFwd, IO_ON );
		set_out_bit( nBwd, IO_OFF );
	}
}
