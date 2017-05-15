// ShareMem.h: interface for the ShareMem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHAREMEM_H__3C625654_DC90_4555_BDDA_4AF6D32AB0A3__INCLUDED_)
#define AFX_SHAREMEM_H__3C625654_DC90_4555_BDDA_4AF6D32AB0A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <vector>

enum SM_TYPE 
{
	SMT_O_density_f1_lockunlock_switch_lamp							= 0,				
	SMT_O_density_f1_paper_pusher_switch_lamp							= 1,								
	SMT_O_density_f2_lockunlock_switch_lamp							= 2,								
	SMT_O_density_f2_paper_pusher_switch_lamp							= 3,								
	SMT_O_density_f3_lockunlock_switch_lamp							= 4,								
	SMT_O_density_f3_paper_pusher_switch_lamp							= 5,								
	
	SMT_I_density_f1_lockunlock_switch_chk							= 8,										
	SMT_I_density_f1_paper_pusher_switch_chk							= 9,								
	SMT_I_density_f2_lockunlock_switch_chk							= 10,									
	SMT_I_density_f2_paper_pusher_switch_chk							= 11,								
	SMT_I_density_f3_lockunlock_switch_chk							= 12,									
	SMT_I_density_f3_paper_pusher_switch_chk							= 13,
	SMT_O_density_f4_lockunlock_switch_lamp							= 100,								
	SMT_O_density_f4_paper_pusher_switch_lamp							= 101,								
	SMT_O_density_f5_lockunlock_switch_lamp							= 102,							
	SMT_O_density_f5_paper_pusher_switch_lamp							= 103,						
	SMT_O_density_f6_lockunlock_switch_lamp							= 104,								
	SMT_O_density_f6_paper_pusher_switch_lamp							= 105,								
	
	SMT_I_density_f4_lockunlock_switch_chk							= 108,									
	SMT_I_density_f4_paper_pusher_switch_chk							= 109,								
	SMT_I_density_f5_lockunlock_switch_chk							= 110,									
	SMT_I_density_f5_paper_pusher_switch_chk							= 111,								
	SMT_I_density_f6_lockunlock_switch_chk							= 112,									
	SMT_I_density_f6_paper_pusher_switch_chk							= 113,								


	SMT_I_density_left_label_limit1_chk								= 114,
	SMT_I_density_right_label_limit2_chk							= 115,

	SMT_O_density_f1_lockunlock										= 200,											
	SMT_O_density_f2_lockunlock										= 201,											
	SMT_O_density_f3_lockunlock										= 202,											
	SMT_O_density_f4_lockunlock										= 203,											
	SMT_O_density_f5_lockunlock										= 204,											
	SMT_O_density_f6_lockunlock										= 205,											
	SMT_O_density_f1_motor_power										= 206,											
	SMT_O_density_f2_motor_power										= 207,											
	SMT_O_density_f3_motor_power										= 208,											
	SMT_O_density_f4_motor_power										= 209,											
	SMT_O_density_f5_motor_power										= 210,											
	SMT_O_density_f6_motor_power										= 211,											

	SMT_I_density_f1_lock_chk											= 300,												
	SMT_I_density_f1_unlock_chk										= 301,											
	SMT_I_density_f2_lock_chk											= 302,												
	SMT_I_density_f2_unlock_chk										= 303,											
	SMT_I_density_f3_lock_chk											= 304,												
	SMT_I_density_f3_unlock_chk										= 305,											
	SMT_I_density_f4_lock_chk											= 306,												
	SMT_I_density_f4_unlock_chk										= 307,											
	SMT_I_density_f5_lock_chk											= 308,												
	SMT_I_density_f5_unlock_chk										= 309,											
	SMT_I_density_f6_lock_chk											= 310,												
	SMT_I_density_f6_unlock_chk										= 311,											

	SMT_O_density_label_blow_onoff									= 400,
	SMT_O_density_label_plate_blow_onoff								= 401,

	SMT_O_density_f1_plate_vacuun2_onoff								= 403,
	SMT_O_density_f2_plate_vacuun2_onoff								= 404,
	SMT_O_density_f3_plate_vacuun2_onoff								= 405,
	SMT_O_density_f4_plate_vacuun2_onoff								= 406,
	SMT_O_density_f5_plate_vacuun2_onoff								= 407,
	SMT_O_density_f6_plate_vacuun2_onoff								= 408,
	SMT_O_density_f1_plate_blow_onoff									= 409,
	SMT_O_density_f2_plate_blow_onoff									= 410,
	SMT_O_density_f3_plate_blow_onoff									= 411,
	SMT_O_density_f4_plate_blow_onoff									= 412,
	SMT_O_density_f5_plate_blow_onoff									= 413,
	SMT_O_density_f6_plate_blow_onoff									= 414,


	SMT_O_density_f1_paper_pusher_fwdback								= 500,
	SMT_O_density_f2_paper_pusher_fwdback								= 501,
	SMT_O_density_f3_paper_pusher_fwdback								= 502,
	SMT_O_density_f4_paper_pusher_fwdback								= 503,
	SMT_O_density_f5_paper_pusher_fwdback								= 504,
	SMT_O_density_f6_paper_pusher_fwdback								= 505,

	SMT_I_density_f1_sticker_limit_chk								= 507,
	SMT_I_density_f1_paper_pusher_forward_chk							= 508,
	SMT_I_density_f1_position_chk										= 509,
	SMT_I_density_f2_sticker_limit_chk								= 510,
	SMT_I_density_f2_paper_pusher_forward_chk							= 511,
	SMT_I_density_f2_position_chk										= 512,
	SMT_I_density_f3_sticker_limit_chk								= 513,
	SMT_I_density_f3_paper_pusher_forward_chk							= 514,
	SMT_I_density_f3_position_chk										= 515,

	SMT_I_density_f4_sticker_limit_chk								= 600,
	SMT_I_density_f4_paper_pusher_forward_chk							= 601,
	SMT_I_density_f4_position_chk										= 602,
	SMT_I_density_f5_sticker_limit_chk								= 603,
	SMT_I_density_f5_paper_pusher_forward_chk							= 604,
	SMT_I_density_f5_position_chk										= 605,
	SMT_I_density_f6_sticker_limit_chk								= 606,
	SMT_I_density_f6_paper_pusher_forward_chk							= 607,
	SMT_I_density_f6_position_chk										= 608,
	SMT_I_density_f1_plate_vacuun2_chk								= 609,
	SMT_I_density_f2_plate_vacuun2_chk								= 610,
	SMT_I_density_f3_plate_vacuun2_chk								= 611,
	SMT_I_density_f4_plate_vacuun2_chk								= 612,
	SMT_I_density_f5_plate_vacuun2_chk								= 613,
	SMT_I_density_f6_plate_vacuun2_chk								= 614,


	SMT_O_density_rbt_left_vacuum1									= 700,
	SMT_O_density_rbt_left_vacuum2									= 701,
	SMT_O_density_rbt_right_vacuum1									= 702,
	SMT_O_density_rbt_right_vacuum2									= 703,

	SMT_O_density_rbt_left_blow1										= 704,
	SMT_O_density_rbt_left_blow2										= 705,
	SMT_O_density_rbt_right_blow1										= 706,
	SMT_O_density_rbt_right_blow2										= 707,


	SMT_I_density_rbt_left_vacuum1_chk								= 708,
	SMT_I_density_rbt_left_vacuum2_chk								= 709,
	SMT_I_density_rbt_right_vacuum1_chk								= 710,
	SMT_I_density_rbt_right_vacuum2_chk								= 711,

	SMT_O_distribute_p1_lockunlock_switch_lamp						= 1000,
	SMT_O_distribute_p2_lockunlock_switch_lamp						= 1001,
	SMT_O_distribute_p3_lockunlock_switch_lamp						= 1002,
	SMT_O_distribute_p4_lockunlock_switch_lamp						= 1003,

	SMT_I_distribute_p1_lockunlock_switch_chk							= 1008,
	SMT_I_distribute_p2_lockunlock_switch_chk							= 1009,
	SMT_I_distribute_p3_lockunlock_switch_chk							= 1010,
	SMT_I_distribute_p4_lockunlock_switch_chk							= 1011,

	SMT_O_distribute_p1_lock											= 1100,
	SMT_O_distribute_p2_lock											= 1101,
	SMT_O_distribute_p3_lock											= 1102,
	SMT_O_distribute_p4_lock											= 1103,

	SMT_I_distribute_p1_lock_chk										= 1104,
	SMT_I_distribute_p1_unlock_chk									= 1105,
	SMT_I_distribute_p1_lock_position_chk								= 1106,
	SMT_I_distribute_p2_lock_chk										= 1107,
	SMT_I_distribute_p2_unlock_chk									= 1108,
	SMT_I_distribute_p2_lock_position_chk								= 1109,
	SMT_I_distribute_p3_lock_chk										= 1110,
	SMT_I_distribute_p3_unlock_chk									= 1111,
	SMT_I_distribute_p3_lock_position_chk								= 1112,
	SMT_I_distribute_p4_lock_chk										= 1113,
	SMT_I_distribute_p4_unlock_chk									= 1114,
	SMT_I_distribute_p4_lock_position_chk								= 1115,


	SMT_O_distribute_p1_plate_updown									= 1200,
	SMT_O_distribute_p2_plate_updown									= 1201,
	SMT_O_distribute_p3_plate_updown									= 1202,
	SMT_O_distribute_p4_plate_updown									= 1203,
	SMT_I_distribute_p1_plate_up_chk									= 1204,
	SMT_I_distribute_p1_plate_down_chk								= 1205,
	SMT_I_distribute_p2_plate_up_chk									= 1206,
	SMT_I_distribute_p2_plate_down_chk								= 1207,
	SMT_I_distribute_p3_plate_up_chk									= 1208,
	SMT_I_distribute_p3_plate_down_chk								= 1209,
	SMT_I_distribute_p4_plate_up_chk									= 1210,
	SMT_I_distribute_p4_plate_down_chk								= 1211,

	SMT_O_distribute_p1_plate_vacuum									= 1300,
	SMT_O_distribute_p2_plate_vacuum									= 1301,
	SMT_O_distribute_p3_plate_vacuum									= 1302,
	SMT_O_distribute_p4_plate_vacuum									= 1303,
	SMT_O_distribute_p1_eject_vacuum									= 1304,
	SMT_O_distribute_p2_eject_vacuum									= 1305,
	SMT_O_distribute_p3_eject_vacuum									= 1306,
	SMT_O_distribute_p4_eject_vacuum									= 1307,
	SMT_I_distribute_p1_plate_vacuum_chk								= 1308,
	SMT_I_distribute_p2_plate_vacuum_chk								= 1309,
	SMT_I_distribute_p3_plate_vacuum_chk								= 1310,
	SMT_I_distribute_p4_plate_vacuum_chk								= 1311,

	SMT_O_distribute_rbt1_left_top_vacuum_fwdback						= 1400,
	SMT_O_distribute_rbt1_left_btm_vacuum_fwdback						= 1401,
	SMT_O_distribute_rbt1_left_box_guide_fwdback						= 1402,
	SMT_O_distribute_rbt1_left_box_guide_updown						= 1403,
	SMT_O_distribute_rbt1_right_top_vacuum_fwdback					= 1404,
	SMT_O_distribute_rbt1_right_btm_vacuum_fwdback					= 1405,
	SMT_O_distribute_rbt1_right_box_guide_fwdback						= 1406,
	SMT_O_distribute_rbt1_right_box_guide_updown						= 1407,
	SMT_O_distribute_rbt1_left_top_vacuum								= 1408,
	SMT_O_distribute_rbt1_left_top_eject								= 1409,
	SMT_O_distribute_rbt1_left_btm_vacuum								= 1410,
	SMT_O_distribute_rbt1_left_btm_eject								= 1411,
	SMT_O_distribute_rbt1_right_top_vacuum							= 1412,
	SMT_O_distribute_rbt1_right_top_eject								= 1413,
	SMT_O_distribute_rbt1_right_btm_vacuum							= 1414,
	SMT_O_distribute_rbt1_right_btm_eject								= 1415,

	SMT_I_distribute_rbt1_left_top_vacuum_fwd_chk						= 1500,
	SMT_I_distribute_rbt1_left_top_vacuum_bwd_chk						= 1501,
	SMT_I_distribute_rbt1_left_btm_vacuum_fwd_chk						= 1502,
	SMT_I_distribute_rbt1_left_btm_vacuum_bwd_chk						= 1503,
	SMT_I_distribute_rbt1_left_box_guide_fwd_chk						= 1504,
	SMT_I_distribute_rbt1_left_box_guide_bwd_chk						= 1505,
	SMT_I_distribute_rbt1_left_box_guide_up_chk						= 1506,
	SMT_I_distribute_rbt1_left_box_guide_down_chk						= 1507,
	SMT_I_distribute_rbt1_left_top_vacuum_chk							= 1508,
	SMT_I_distribute_rbt1_left_btm_vacuum_chk							= 1509,

	SMT_I_distribute_rbt1_left_box_rotator_fwd_chk					= 1510,
	SMT_I_distribute_rbt1_left_box_rotator_bwd_chk					= 1511,
	SMT_O_distribute_rbt1_left_box_rotator_fwd_cylinder				= 1512,
	SMT_O_distribute_rbt1_left_box_rotator_bwd_cylinder				= 1513,

	SMT_I_distribute_rbt1_right_top_vacuum_fwd_chk					= 1600,
	SMT_I_distribute_rbt1_right_top_vacuum_bwd_chk					= 1601,
	SMT_I_distribute_rbt1_right_btm_vacuum_fwd_chk					= 1602,
	SMT_I_distribute_rbt1_right_btm_vacuum_bwd_chk					= 1603,
	SMT_I_distribute_rbt1_right_box_guide_fwd_chk						= 1604,
	SMT_I_distribute_rbt1_right_box_guide_bwd_chk						= 1605,
	SMT_I_distribute_rbt1_right_box_guide_up_chk						= 1606,
	SMT_I_distribute_rbt1_right_box_guide_down_chk					= 1607,
	SMT_I_distribute_rbt1_right_top_vacuum_chk						= 1608,
	SMT_I_distribute_rbt1_right_btm_vacuum_chk						= 1609,
	
	SMT_I_distribute_rbt1_right_box_rotator_fwd_chk					= 1610,
	SMT_I_distribute_rbt1_right_box_rotator_bwd_chk					= 1611,
	SMT_O_distribute_rbt1_right_box_rotator_fwd_cylinder				= 1612,
	SMT_O_distribute_rbt1_right_box_rotator_bwd_cylinder				= 1613,

	SMT_O_distribute_rbt2_left_top_vacuum_fwdback						= 1700,
	SMT_O_distribute_rbt2_left_btm_vacuum_fwdback						= 1701,
	SMT_O_distribute_rbt2_left_box_guide_fwdback						= 1702,
	SMT_O_distribute_rbt2_left_box_guide_updown						= 1703,
	SMT_O_distribute_rbt2_right_top_vacuum_fwdback					= 1704,
	SMT_O_distribute_rbt2_right_btm_vacuum_fwdback					= 1705,
	SMT_O_distribute_rbt2_right_box_guide_fwdback						= 1706,
	SMT_O_distribute_rbt2_right_box_guide_updown						= 1707,
	SMT_O_distribute_rbt2_left_top_vacuum								= 1708,
	SMT_O_distribute_rbt2_left_top_eject								= 1709,
	SMT_O_distribute_rbt2_left_btm_vacuum								= 1710,
	SMT_O_distribute_rbt2_left_btm_eject								= 1711,
	SMT_O_distribute_rbt2_right_top_vacuum							= 1712,
	SMT_O_distribute_rbt2_right_top_eject								= 1713,
	SMT_O_distribute_rbt2_right_btm_vacuum							= 1714,
	SMT_O_distribute_rbt2_right_btm_eject								= 1715,

	SMT_I_distribute_rbt2_left_top_vacuum_fwd_chk						= 2000,
	SMT_I_distribute_rbt2_left_top_vacuum_bwd_chk						= 2001,
	SMT_I_distribute_rbt2_left_btm_vacuum_fwd_chk						= 2002,
	SMT_I_distribute_rbt2_left_btm_vacuum_bwd_chk						= 2003,
	SMT_I_distribute_rbt2_left_box_guide_fwd_chk						= 2004,
	SMT_I_distribute_rbt2_left_box_guide_bwd_chk						= 2005,
	SMT_I_distribute_rbt2_left_box_guide_up_chk						= 2006,
	SMT_I_distribute_rbt2_left_box_guide_down_chk						= 2007,
	SMT_I_distribute_rbt2_left_top_vacuum_chk							= 2008,
	SMT_I_distribute_rbt2_left_btm_vacuum_chk							= 2009,

	SMT_I_distribute_rbt2_left_box_rotator_fwd_chk					= 2010,
	SMT_I_distribute_rbt2_left_box_rotator_bwd_chk					= 2011,
	SMT_O_distribute_rbt2_left_box_rotator_fwd_cylinder				= 2012,
	SMT_O_distribute_rbt2_left_box_rotator_bwd_cylinder				= 2013,

	SMT_I_distribute_rbt2_right_top_vacuum_fwd_chk					= 2100,
	SMT_I_distribute_rbt2_right_top_vacuum_bwd_chk					= 2101,
	SMT_I_distribute_rbt2_right_btm_vacuum_fwd_chk					= 2102,
	SMT_I_distribute_rbt2_right_btm_vacuum_bwd_chk					= 2103,
	SMT_I_distribute_rbt2_right_box_guide_fwd_chk						= 2104,
	SMT_I_distribute_rbt2_right_box_guide_bwd_chk						= 2105,
	SMT_I_distribute_rbt2_right_box_guide_up_chk						= 2106,
	SMT_I_distribute_rbt2_right_box_guide_down_chk					= 2107,
	SMT_I_distribute_rbt2_right_top_vacuum_chk						= 2108,
	SMT_I_distribute_rbt2_right_btm_vacuum_chk						= 2109,

	SMT_I_distribute_rbt2_right_box_rotator_fwd_chk					= 2110,
	SMT_I_distribute_rbt2_right_box_rotator_bwd_chk					= 2111,
	SMT_O_distribute_rbt2_right_box_rotator_fwd_cylinder				= 2112,
	SMT_O_distribute_rbt2_right_box_rotator_bwd_cylinder				= 2113,




	SMT_O_reject_rbt_updown											= 2200,
	SMT_O_reject_rbt_vacuum											= 2201,
	SMT_O_reject_rbt_eject											= 2202,

	SMT_O_density_vision_trigger_onoff								= 2204,
	SMT_O_distribute_vision_trigger_onoff								= 2205,
	SMT_O_top_vision_trigger_onoff									= 2206,
	SMT_O_btm_vision_trigger_onoff									= 2207,

	SMT_I_reject_rbt_up_chk											= 2208,
	SMT_I_reject_rbt_down_chk											= 2209,
	SMT_I_reject_rbt_vacuum_chk										= 2210,
	SMT_I_reject_cv_full_chk											= 2211,
	SMT_I_reject_cv_1step_pos_chk										= 2212,
	SMT_I_reject_cv_full_pos_chk										= 2213,

	SMT_O_uld_rbt_left_updown											= 2300,
	SMT_O_uld_rbt_left_vacuum											= 2301,
	SMT_O_uld_rbt_right_updown										= 2302,
	SMT_O_uld_rbt_right_vacuum										= 2303,

	SMT_I_uld_rbt_left_up_chk											= 2308,
	SMT_I_uld_rbt_left_down_chk										= 2309,
	SMT_I_uld_rbt_left_vacuum_chk										= 2310,
	SMT_I_uld_rbt_right_up_chk										= 2311,
	SMT_I_uld_rbt_right_down_chk										= 2312,
	SMT_I_uld_rbt_right_vacuum_chk									= 2313,


	SMT_O_recycle_zone_box_pusher_fwd									= 2400,
	SMT_O_recycle_zone_box_pusher_back								= 2401,
	SMT_O_recycle_zone_box_vacuum										= 2402,

	SMT_I_recycle_zone_box_pusher_fwd_chk								= 2405,
	SMT_I_recycle_zone_box_pusher_bwd_chk								= 2406,
	SMT_I_recycle_zone_box_vacuum_chk									= 2407,
	SMT_I_recycle_zone_box_chk										= 2408,
	SMT_I_recycle_zone_box1_chk										= 2409,
	SMT_I_recycle_zone_box2_chk										= 2410,

	SMT_I_uld_cv_left_box_chk											= 2412,
	SMT_I_uld_cv_right_box_chk										= 2413,
	SMT_I_uld_cv_box_out_chk											= 2414,


	SMT_O_top_idbuffer_ld1_shifter_hook_fwd							= 2500,
	SMT_O_top_idbuffer_ld1_shifter_hook_bwd							= 2501,
	SMT_O_top_idbuffer_ld1_shifter_hook_clamp							= 2502,
	SMT_O_top_idbuffer_ld1_shifter_hook_unclamp						= 2503,
	SMT_O_top_idbuffer_ld2_shifter_left_hook_clamp					= 2504,
	SMT_O_top_idbuffer_ld2_shifter_left_hook_unclamp					= 2505,
	SMT_O_top_idbuffer_ld2_shifter_right_hook_clamp					= 2506,
	SMT_O_top_idbuffer_ld2_shifter_right_hook_unclamp					= 2507,
	SMT_I_top_idbuffer_ld1_shifter_hook_fwd_chk						= 2508,
	SMT_I_top_idbuffer_ld1_shifter_hook_bwd_chk						= 2509,
	SMT_I_top_idbuffer_ld1_shifter_hook_clamp_chk						= 2510,
	SMT_I_top_idbuffer_ld1_shifter_hook_unclamp_chk					= 2511,
	SMT_I_top_idbuffer_ld2_shifter_left_hook_clamp_chk				= 2512,
	SMT_I_top_idbuffer_ld2_shifter_left_hook_unclamp_chk				= 2513,
	SMT_I_top_idbuffer_ld2_shifter_right_hook_clamp_chk				= 2514,
	SMT_I_top_idbuffer_ld2_shifter_right_hook_unclamp_chk				= 2515,

	SMT_O_top_idbuffer_ld2_shifter_hook_fwd							= 2600,
	SMT_O_top_idbuffer_ld2_shifter_hook_bwd							= 2601,
	SMT_O_top_idbuffer12_clamp										= 2602,
	SMT_O_top_idbuffer12_unclamp										= 2603,
	SMT_O_top_idbuffer34_clamp										= 2604,
	SMT_O_top_idbuffer34_unclamp										= 2605,
	SMT_O_top_idbuffer56_clamp										= 2606,
	SMT_O_top_idbuffer56_unclamp										= 2607,
	SMT_O_top_idbuffer78_clamp										= 2608,
	SMT_O_top_idbuffer78_unclamp										= 2609,

	SMT_I_top_idbuffer_ld2_shifter_hook_fwd_chk						= 2700,
	SMT_I_top_idbuffer_ld2_shifter_hook_bwd_chk						= 2701,
	SMT_I_top_idbuffer12_clamp_chk									= 2702,
	SMT_I_top_idbuffer12_unclamp_chk									= 2703,
	SMT_I_top_idbuffer1_chk											= 2704,
	SMT_I_top_idbuffer2_chk											= 2705,
	SMT_I_top_idbuffer_box1_chk										= 2706,
	SMT_I_top_idbuffer_box2_chk										= 2707,
	SMT_I_top_idbuffer34_clamp_chk									= 2708,
	SMT_I_top_idbuffer34_unclamp_chk									= 2709,
	SMT_I_top_idbuffer3_chk											= 2710,
	SMT_I_top_idbuffer4_chk											= 2711,
	SMT_I_top_idbuffer_box3_chk										= 2712,
	SMT_I_top_idbuffer_box4_chk										= 2713,

	SMT_I_btm_idbuffer_uld1_shifter_buffer1_chk						= 2714,
	SMT_I_btm_idbuffer_uld1_shifter_buffer2_chk						= 2715,


	SMT_I_top_idbuffer56_clamp_chk									= 3000,
	SMT_I_top_idbuffer56_unclamp_chk									= 3001,
	SMT_I_top_idbuffer5_chk											= 3002,
	SMT_I_top_idbuffer6_chk											= 3003,
	SMT_I_top_idbuffer_box5_chk										= 3004,
	SMT_I_top_idbuffer_box6_chk										= 3005,
	SMT_I_top_idbuffer78_clamp_chk									= 3006,
	SMT_I_top_idbuffer78_unclamp_chk									= 3007,
	SMT_I_top_idbuffer7_chk											= 3008,
	SMT_I_top_idbuffer8_chk											= 3009,
	SMT_I_top_idbuffer_box7_chk										= 3010,
	SMT_I_top_idbuffer_box8_chk										= 3011,


	SMT_O_rolling_align_pusher_fwdback								= 3100,
	SMT_O_rolling1_pusher_fwdback										= 3101,
	SMT_O_rolling2_pusher_fwdback										= 3102,

	SMT_I_rolling_align_pusher_left_fwd_chk							= 3104,
	SMT_I_rolling_align_pusher_left_back_chk							= 3105,
	SMT_I_rolling_align_pusher_right_fwd_chk							= 3106,
	SMT_I_rolling_align_pusher_right_back_chk							= 3107,
	SMT_I_rolling1_pusher_left_fwd_chk								= 3108,
	SMT_I_rolling1_pusher_left_back_chk								= 3109,
	SMT_I_rolling1_pusher_right_fwd_chk								= 3110,
	SMT_I_rolling1_pusher_right_back_chk								= 3111,
	SMT_I_rolling2_pusher_left_fwd_chk								= 3112,
	SMT_I_rolling2_pusher_left_back_chk								= 3113,
	SMT_I_rolling2_pusher_right_fwd_chk								= 3114,
	SMT_I_rolling2_pusher_right_back_chk								= 3115,


	SMT_O_idbuffer_elv_hook_clamp										= 3200,
	SMT_O_idbuffer_elv_hook_unclamp									= 3201,
	SMT_O_idbuffer_elv_pusher_fwd										= 3202,
	SMT_O_idbuffer_elv_pusher_bwd										= 3203,


	SMT_I_idbuffer_elv_hook_clamp_chk									= 3208,
	SMT_I_idbuffer_elv_hook_unclamp_chk								= 3209,
	SMT_I_idbuffer_elv_idbuffer1_chk									= 3210,
	SMT_I_idbuffer_elv_idbuffer2_chk									= 3211,
	SMT_I_idbuffer_elv_box1_chk										= 3212,
	SMT_I_idbuffer_elv_box2_chk										= 3213,
	SMT_I_idbuffer_elv_pusher_fwd_chk									= 3214,
	SMT_I_idbuffer_elv_pusher_bwd_chk									= 3215,

	SMT_O_btm_idbuffer_uld1_shifter_hook_fwd							= 3300,
	SMT_O_btm_idbuffer_uld1_shifter_hook_back							= 3301,
	SMT_O_btm_idbuffer12_clamp										= 3302,
	SMT_O_btm_idbuffer12_unclamp										= 3303,
	SMT_O_btm_idbuffer_uld2_shifter_hook_fwd							= 3304,
	SMT_O_btm_idbuffer_uld2_shifter_hook_back							= 3305,
	SMT_I_btm_idbuffer_uld1_shifter_hook_fwd_chk						= 3306,
	SMT_I_btm_idbuffer_uld1_shifter_hook_back_chk						= 3307,
	SMT_I_btm_idbuffer12_clamp_chk									= 3308,
	SMT_I_btm_idbuffer12_unclamp_chk									= 3309,
	SMT_I_btm_idbuffer1_chk											= 3310,
	SMT_I_btm_idbuffer2_chk											= 3311,
//	SMT_I_btm_idbuffer_uld2_shifter_hook_fwd1							= 3312,
//	SMT_I_btm_idbuffer_uld2_shifter_hook_back1						= 3313,
//	SMT_I_btm_idbuffer_uld2_shifter_hook_fwd2							= 3314,
//	SMT_I_btm_idbuffer_uld2_shifter_hook_back2						= 3315,

	SMT_O_btm_idbuffer34_clamp										= 3400,
	SMT_O_btm_idbuffer34_unclamp										= 3401,
	SMT_O_btm_idbuffer_stopper_updown									= 3402,

	SMT_I_btm_idbuffer_uld2_shifter_hook_fwd1_chk						= 3404,
	SMT_I_btm_idbuffer_uld2_shifter_hook_back1_chk					= 3405,
	SMT_I_btm_idbuffer_uld2_shifter_hook_fwd2_chk						= 3406,
	SMT_I_btm_idbuffer_uld2_shifter_hook_back2_chk					= 3407,
	SMT_I_btm_idbuffer34_clamp_chk									= 3408,
	SMT_I_btm_idbuffer34_unclamp_chk									= 3409,
	SMT_I_btm_idbuffer3_chk											= 3410,
	SMT_I_btm_idbuffer4_chk											= 3411,
	SMT_I_btm_idbuffer_stopper_up_chk									= 3412,
	SMT_I_btm_idbuffer_stopper_down_chk								= 3413,
	SMT_I_btm_idbuffer_stopper_buffer_chk								= 3414,
	SMT_I_btm_idbuffer_8480_hook_in_chk								= 3415,



	//ASS'Y  : TL & DOOR
	SMT_O_START_SWITCH_LAMP          = 3500,  //S1500
	SMT_O_STOP_SWITCH_LAMP          = 3501,  //S1501
	SMT_O_ALARM_CLEAR_LAMP          = 3502,  //S1502
	SMT_O_BUZZER_OFF_LAMP          = 3503,  //S1503
	SMT_O_reject_cv_motor_onoff										= 3504,
	SMT_O_uld_cv_motor_onoff											= 3505,

	SMT_I_START_SWITCH_CHK          = 3508,  //PS1500
	SMT_I_STOP_SWITCH_CHK          = 3509,  //PS1501
	SMT_I_ALARM_CLEAR_CHK          = 3510,  //PS1502
	SMT_I_BUZZER_OFF_CHK          = 3511,  //PS1503
	
	SMT_I__recycle_zone_door_switch_chk	= 3512,
// 	SMT_I_AUTO_MODE_SWITCH_CHK         = 1512,  //PS1504
// 	SMT_I_MANUAL_MODE_SWITCH_CHK        = 1513,  //PS1505
	SMT_I_FRONT_SELECT_SWITCH_CHK        = 3514,  //PS1506
	SMT_I_REAR_SELECT_SWITCH_CHK        = 3515,  //PS1507
	

	//ASS'Y  : MC & EMO
	SMT_O_MAIN_AIR_ONOFF          = 3600,  //S1600
	SMT_O_TOWER_GREEN_LAMP          = 3601,  //S1601
	SMT_O_TOWER_YELLOW_LAMP          = 3602,  //S1602
	SMT_O_TOWER_RED_LAMP          = 3603,  //S1603
	SMT_O_BUZZER1_ONOFF           = 3604,  //S1604
	SMT_O_BUZZER2_ONOFF           = 3605,  //S1605
	SMT_O_BUZZER3_ONOFF           = 3606,  //S1605
	SMT_O_DOOR_LOCK_ONOFF          = 3607,  //S1606

	SMT_I_MAIN_AIR_CHK           = 3608,  //PS1600

	SMT_I_MC1_CHK														= 3611,												
	SMT_I_EMO_SWITCH1_CHK													= 3612,											
	SMT_I_EMO_SWITCH2_CHK													= 3613,											
	SMT_I_EMO_SWITCH4_CHK													= 3614,											
	SMT_I_EMO_SWITCH5_CHK													= 3615,											


	SMT_I_DOOR1_OPENCLOSE_CHK         = 3700,  //PS1601
	SMT_I_DOOR2_OPENCLOSE_CHK         = 3701,  //PS1602
	SMT_I_DOOR3_OPENCLOSE_CHK         = 3702,  //PS1603
	SMT_I_DOOR4_OPENCLOSE_CHK         = 3703,  //PS1604
	SMT_I_DOOR5_OPENCLOSE_CHK         = 3704,  //PS1604
	SMT_I_DOOR6_OPENCLOSE_CHK         = 3705,  //PS1604
	SMT_I_DOOR7_OPENCLOSE_CHK         = 3706,  //PS1604
	SMT_I_DOOR8_OPENCLOSE_CHK         = 3707,  //PS1604
	SMT_I_DOOR9_OPENCLOSE_CHK         = 3708,  //PS1604
	SMT_I_DOOR10_OPENCLOSE_CHK         = 3709,  //PS1604

	
	SMT_I_bottom_door1												= 3712,
	SMT_I_bottom_door2												= 3713,
	SMT_I_bottom_door3												= 3714,
	SMT_I_bottom_door4												= 3715,
	
//	//ASS'Y  : SMEMA/MC/EMO
//	SMT_O_FRONT_SHUTTLE_REQUEST_SIGNAL       = 1700,  //S1700
//	SMT_O_LOB_BUFFER_CONVEYOR_MOTOR_ONOFF      = 1703,  //S0103
//	SMT_O_LOADING_TRAY_CONVEYOR_MOTOR_ONOFF      = 1704,  //S0104
//	SMT_O_EMPTY_TRAY_CONVEYOR_MOTOR_ONOFF      = 1705,  //S0203
//	SMT_O_EMPTY_TRAY_CONVEYOR_CW_ONOFF       = 1706,  //S0204
//	SMT_O_EMPTY_TRAY_CONVEYOR_CCW_ONOFF       = 1707,  //S0205


	//ASS'Y  : MC/EMO/SMEMA
	SMT_O_front_top_machine_enter_signal							= 4000,						
	SMT_O_front_top_machine_transfer_signal							= 4001,					
	SMT_O_front_btm_machine_ready_signal								= 4002,						
	SMT_O_front_btm_machine_stop_signal								= 4003,						

	SMT_I_front_top_machine_ready_signal_chk							= 4008,					
	SMT_I_front_top_machine_stop_signal_chk							= 4009,					
	SMT_I_front_btm_machine_request_signal_chk						= 4010,					
	SMT_I_front_btm_machine_transfer_signal_chk						= 4011,				

	
	//MOTOR TARGET
 	SMT_M_D_FEEDER_1_TARGET,
 	SMT_M_D_FEEDER_2_TARGET,
	SMT_M_D_FEEDER_3_TARGET,
	SMT_M_D_FEEDER_4_TARGET,
	SMT_M_D_FEEDER_5_TARGET,
	SMT_M_D_FEEDER_6_TARGET,
	SMT_M_FEEDER_1_TARGET,
	SMT_M_FEEDER_2_TARGET,
	SMT_M_FEEDER_3_TARGET,
	SMT_M_FEEDER_4_TARGET,
	SMT_M_D_VISION_X_TARGET,
	SMT_M_D_ATTACH_X_TARGET,
	SMT_M_D_ATTACH_Y_TARGET,
	SMT_M_D_ATTACH_Z1_TARGET,
	SMT_M_D_ATTACH_Z2_TARGET,
	SMT_M_VISION_X_TARGET,
	SMT_M_L_REJECT_X_TARGET,
	SMT_M_ATTACH1_X_TARGET,
	SMT_M_ATTACH1_Z1_TARGET,
	SMT_M_ATTACH1_R1_TARGET,
	SMT_M_ATTACH1_Z2_TARGET,
	SMT_M_ATTACH1_R2_TARGET,
	SMT_M_ATTACH2_X_TARGET,
	SMT_M_ATTACH2_Z1_TARGET,
	SMT_M_ATTACH2_R1_TARGET,
	SMT_M_ATTACH2_Z2_TARGET,
	SMT_M_ATTACH2_R2_TARGET,
	SMT_M_TOP_VISION_X_TARGET,
	SMT_M_BTM_VISION_X_TARGET,
	SMT_M_ULD_Y_TARGET,
	SMT_M_ULD_Z_TARGET,
	SMT_M_ULD_CV_PUTCH_TARGET,
	SMT_M_IDBUFFER_EL_TARGET,
	SMT_M_TOPID_LD1_TARGET,
	SMT_M_TOPID_LD2_TARGET,
	SMT_M_BTMID_ULD1_TARGET,
	SMT_M_BTMID_ULD2_TARGET,
	SMT_M_INDEX_X_TARGET,
	SMT_M_INDEX_ROLLER1_TARGET,
	SMT_M_INDEX_ROLLER2_TARGET,
	SMT_M_ULD_STACKER_Z_TARGET,

	
	// MOTOR POSITION
 	SMT_M_D_FEEDER_1_POSITION,
 	SMT_M_D_FEEDER_2_POSITION,
	SMT_M_D_FEEDER_3_POSITION,
	SMT_M_D_FEEDER_4_POSITION,
	SMT_M_D_FEEDER_5_POSITION,
	SMT_M_D_FEEDER_6_POSITION,
	SMT_M_FEEDER_1_POSITION,
	SMT_M_FEEDER_2_POSITION,
	SMT_M_FEEDER_3_POSITION,
	SMT_M_FEEDER_4_POSITION,
	SMT_M_D_VISION_X_POSITION,
	SMT_M_D_ATTACH_X_POSITION,
	SMT_M_D_ATTACH_Y_POSITION,
	SMT_M_D_ATTACH_Z1_POSITION,
	SMT_M_D_ATTACH_Z2_POSITION,
	SMT_M_VISION_X_POSITION,
	SMT_M_L_REJECT_X_POSITION,
	SMT_M_ATTACH1_X_POSITION,
	SMT_M_ATTACH1_Z1_POSITION,
	SMT_M_ATTACH1_R1_POSITION,
	SMT_M_ATTACH1_Z2_POSITION,
	SMT_M_ATTACH1_R2_POSITION,
	SMT_M_ATTACH2_X_POSITION,
	SMT_M_ATTACH2_Z1_POSITION,
	SMT_M_ATTACH2_R1_POSITION,
	SMT_M_ATTACH2_Z2_POSITION,
	SMT_M_ATTACH2_R2_POSITION,
	SMT_M_TOP_VISION_X_POSITION,
	SMT_M_BTM_VISION_X_POSITION,
	SMT_M_ULD_Y_POSITION,
	SMT_M_ULD_Z_POSITION,
	SMT_M_ULD_CV_PUTCH_POSITION,
	SMT_M_IDBUFFER_EL_POSITION,
	SMT_M_TOPID_LD1_POSITION,
	SMT_M_TOPID_LD2_POSITION,
	SMT_M_BTMID_ULD1_POSITION,
	SMT_M_BTMID_ULD2_POSITION,
	SMT_M_INDEX_X_POSITION,
	SMT_M_INDEX_ROLLER1_POSITION,
	SMT_M_INDEX_ROLLER2_POSITION,
	SMT_M_ULD_STACKER_Z_POSITION,
	
	SMT_COUNT,
};





struct st_shareMem
{
	SM_TYPE smt;

	HANDLE hFMap;
	char* pInFile;
};

class ShareMem  
{
public:
	ShareMem();
	virtual ~ShareMem();

public:
	void Initialize();
	void UnInitialize();
	void SetData( SM_TYPE smt, std::string strData );
	std::string GetData( SM_TYPE smt );

protected:
	std::vector<st_shareMem> m_vecSM;
};

extern ShareMem g_sm;

#endif // !defined(AFX_SHAREMEM_H__3C625654_DC90_4555_BDDA_4AF6D32AB0A3__INCLUDED_)
