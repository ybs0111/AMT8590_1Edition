// IO_Manager.h: interface for the IO_Manager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IO_MANAGER_H__EA6CB312_2364_436A_97C1_903DD2657C23__INCLUDED_)
#define AFX_IO_MANAGER_H__EA6CB312_2364_436A_97C1_903DD2657C23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct st_io_param
{
	//ASS'Y  : LOB BUFFER
	int o_density_f1_lockunlock_switch_lamp;								//S0000
	int o_density_f1_paper_pusher_switch_lamp;								//S0001
	int o_density_f2_lockunlock_switch_lamp;								//S0002
	int o_density_f2_paper_pusher_switch_lamp;								//S0003
	int o_density_f3_lockunlock_switch_lamp;								//S0003
	int o_density_f3_paper_pusher_switch_lamp;								//S0003
	
	int i_density_f1_lockunlock_switch_chk;									//PS000
	int i_density_f1_paper_pusher_switch_chk;								//PS0001
	int i_density_f2_lockunlock_switch_chk;									//PS0001
	int i_density_f2_paper_pusher_switch_chk;								//PS0001
	int i_density_f3_lockunlock_switch_chk;									//PS0001
	int i_density_f3_paper_pusher_switch_chk;								//PS0001

	int o_density_f4_lockunlock_switch_lamp;								//S0000
	int o_density_f4_paper_pusher_switch_lamp;								//S0001
	int o_density_f5_lockunlock_switch_lamp;								//S0002
	int o_density_f5_paper_pusher_switch_lamp;								//S0003
	int o_density_f6_lockunlock_switch_lamp;								//S0003
	int o_density_f6_paper_pusher_switch_lamp;								//S0003
	
	int i_density_f4_lockunlock_switch_chk;									//PS000
	int i_density_f4_paper_pusher_switch_chk;								//PS0001
	int i_density_f5_lockunlock_switch_chk;									//PS0001
	int i_density_f5_paper_pusher_switch_chk;								//PS0001
	int i_density_f6_lockunlock_switch_chk;									//PS0001
	int i_density_f6_paper_pusher_switch_chk;								//PS0001

	////2014,1203
	int i_density_left_label_limit1_chk;
	int i_density_right_label_limit2_chk;
	////

	int o_density_paper_pusher_switch[6];
	int o_density_lock_switch[6];
	int i_density_paper_pusher_switch_chk[6];
	int i_density_lock_switch_chk[6];


	int o_density_f1_lockunlock;											//S0000
	int o_density_f2_lockunlock;											//S0000
	int o_density_f3_lockunlock;											//S0000
	int o_density_f4_lockunlock;											//S0000
	int o_density_f5_lockunlock;											//S0000
	int o_density_f6_lockunlock;											//S0000
	int o_density_f1_motor_power;											//S0000
	int o_density_f2_motor_power;											//S0000
	int o_density_f3_motor_power;											//S0000
	int o_density_f4_motor_power;											//S0000
	int o_density_f5_motor_power;											//S0000
	int o_density_f6_motor_power;											//S0000

	int o_density_lockunlock[6];
	int o_density_motor_power[6];//2014.1017 feeder power on 

	int i_density_f1_lock_chk;												//S0000
	int i_density_f1_unlock_chk;											//S0000
	int i_density_f2_lock_chk;												//S0000
	int i_density_f2_unlock_chk;											//S0000
	int i_density_f3_lock_chk;												//S0000
	int i_density_f3_unlock_chk;											//S0000
	int i_density_f4_lock_chk;												//S0000
	int i_density_f4_unlock_chk;											//S0000
	int i_density_f5_lock_chk;												//S0000
	int i_density_f5_unlock_chk;											//S0000
	int i_density_f6_lock_chk;												//S0000
	int i_density_f6_unlock_chk;											//S0000

	int i_density_lock_chk[6];
	int i_density_unlock_chk[6];


//	int o_density_label_blow_onoff;
//	int o_density_label_plate_blow_onoff;
//
//	int o_density_f1_plate_btm_blow_onoff;
//	int o_density_f2_plate_btm_blow_onoff;
//	int o_density_f3_plate_btm_blow_onoff;
//	int o_density_f4_plate_btm_blow_onoff;
//	int o_density_f5_plate_btm_blow_onoff;
//	int o_density_f6_plate_btm_blow_onoff;
//	int o_density_f1_plate_blow_onoff;
//	int o_density_f2_plate_blow_onoff;
//	int o_density_f3_plate_blow_onoff;
//	int o_density_f4_plate_blow_onoff;
//	int o_density_f5_plate_blow_onoff;
//	int o_density_f6_plate_blow_onoff;
//
//	int o_density_plate_btm_blow_onoff[6];
//	int o_density_plate_blow_onoff[6];
//
//	int o_density_f1_paper_pusher_fwdback;
//	int o_density_f2_paper_pusher_fwdback;
//	int o_density_f3_paper_pusher_fwdback;
//	int o_density_f4_paper_pusher_fwdback;
//	int o_density_f5_paper_pusher_fwdback;
//	int o_density_f6_paper_pusher_fwdback;
//
//	int o_density_paper_pusher_fwdback[6];
//
//	int i_density_f1_sticker_limit_chk;
//	int i_density_f1_paper_pusher_forward_chk;
//	int i_density_f1_position_chk;
//	int i_density_f2_sticker_limit_chk;
//	int i_density_f2_paper_pusher_forward_chk;
//	int i_density_f2_position_chk;
//	int i_density_f3_sticker_limit_chk;
//	int i_density_f3_paper_pusher_forward_chk;
//	int i_density_f3_position_chk;

//	int i_density_f4_sticker_limit_chk;
//	int i_density_f4_paper_pusher_forward_chk;
//	int i_density_f4_position_chk;
//	int i_density_f5_sticker_limit_chk;
//	int i_density_f5_paper_pusher_forward_chk;
//	int i_density_f5_position_chk;
//	int i_density_f6_sticker_limit_chk;
//	int i_density_f6_paper_pusher_forward_chk;
//	int i_density_f6_position_chk;
//	int i_density_f1_plate_vacuun2_chk;
//	int i_density_f2_plate_vacuun2_chk;
//	int i_density_f3_plate_vacuun2_chk;
//	int i_density_f4_plate_vacuun2_chk;
//	int i_density_f5_plate_vacuun2_chk;
//	int i_density_f6_plate_vacuun2_chk;
//
//	int i_density_paper_pusher_fwdback_chk[6];
//	int i_density_sticker_limit_chk[6];
//	int i_density_plate_vacuum_chk[6];


	int o_density_lable1_start;
	int o_density_lable2_start;
	int o_density_lable3_start;
	int o_density_lable4_start;
	int o_density_lable5_start;
	int o_density_lable6_start;
	int o_density_lable_start[6];
	
	int o_density_lable1_pickon;
	int o_density_lable2_pickon;
	int o_density_lable3_pickon;
	int o_density_lable4_pickon;
	int o_density_lable5_pickon;
	int o_density_lable6_pickon;
	int o_density_lable_pickon[6];

	int o_density_lable1_length1;
	int o_density_lable1_length2;
	int o_density_lable2_length1;
	int o_density_lable2_length2;

	int o_density_lable3_length1;
	int o_density_lable3_length2;
	int o_density_lable4_length1;
	int o_density_lable4_length2;
	int o_density_lable5_length1;
	int o_density_lable5_length2;
	int o_density_lable6_length1;
	int o_density_lable6_length2;

	int o_density_lable_length1[6];
	int o_density_lable_length2[6];


	int i_density_f1_position_chk;
	int i_density_f2_position_chk;
	int i_density_f3_position_chk;
	int i_density_f4_position_chk;
	int i_density_f5_position_chk;
	int i_density_f6_position_chk;
	int i_density_position_chk[6];

	int i_density_label1_error_chk;
	int i_density_label2_error_chk;

	int i_density_label3_error_chk;
	int i_density_label4_error_chk;
	int i_density_label5_error_chk;
	int i_density_label6_error_chk;
	int i_density_label_error_chk[6];


	int i_density_label1_ready_chk;
	int i_density_label2_ready_chk;
	int i_density_label3_ready_chk;
	int i_density_label4_ready_chk;
	int i_density_label5_ready_chk;
	int i_density_label6_ready_chk;

	int i_density_label_ready_chk[6];

	int i_density_label1_complete_chk;
	int i_density_label2_complete_chk;
	int i_density_label3_complete_chk;
	int i_density_label4_complete_chk;
	int i_density_label5_complete_chk;
	int i_density_label6_complete_chk;
	int i_density_label_complete_chk[6];


	int o_density_rbt_left_vacuum1;
	int o_density_rbt_left_vacuum2;
	int o_density_rbt_right_vacuum1;
	int o_density_rbt_right_vacuum2;

	int o_density_rbt_left_blow1;
	int o_density_rbt_left_blow2;
	int o_density_rbt_right_blow1;
	int o_density_rbt_right_blow2;

	int i_density_rbt_left_vacuum1_chk;
	int i_density_rbt_left_vacuum2_chk;
	int i_density_rbt_right_vacuum1_chk;
	int i_density_rbt_right_vacuum2_chk;

	int i_bcr_label1_chk;
	int i_bcr_label2_chk;
	int i_bcr_label3_chk;
	int i_bcr_label4_chk;
	int i_bcr_label_chk[4];

	int o_distribute_p1_lockunlock_switch_lamp;
	int o_distribute_p2_lockunlock_switch_lamp;
	int o_distribute_p3_lockunlock_switch_lamp;
	int o_distribute_p4_lockunlock_switch_lamp;

	int i_distribute_p1_lockunlock_switch_chk;
	int i_distribute_p2_lockunlock_switch_chk;
	int i_distribute_p3_lockunlock_switch_chk;
	int i_distribute_p4_lockunlock_switch_chk;

	int o_distribute_lockunlock_switch_lamp[4];
	int i_distribute_lockunlock_switch_chk[4];

	int o_distribute_p1_lock;
	int o_distribute_p2_lock;
	int o_distribute_p3_lock;
	int o_distribute_p4_lock;

	int i_distribute_p1_lock_chk;
	int i_distribute_p1_unlock_chk;
	int i_distribute_p1_lock_position_chk;
	int i_distribute_p2_lock_chk;
	int i_distribute_p2_unlock_chk;
	int i_distribute_p2_lock_position_chk;
	int i_distribute_p3_lock_chk;
	int i_distribute_p3_unlock_chk;
	int i_distribute_p3_lock_position_chk;
	int i_distribute_p4_lock_chk;
	int i_distribute_p4_unlock_chk;
	int i_distribute_p4_lock_position_chk;

	int o_distribute_p_lock[4];
	int i_distribute_lock_chk[4];
	int i_distribute_unlock_chk[4];
	int i_distribute_lock_position_chk[4];

	int o_distribute_p1_plate_updown;
	int o_distribute_p2_plate_updown;
	int o_distribute_p3_plate_updown;
	int o_distribute_p4_plate_updown;

	int o_distribute_plate_updown[4];

	int i_distribute_p1_plate_up_chk;
	int i_distribute_p1_plate_down_chk;
	int i_distribute_p2_plate_up_chk;
	int i_distribute_p2_plate_down_chk;
	int i_distribute_p3_plate_up_chk;
	int i_distribute_p3_plate_down_chk;
	int i_distribute_p4_plate_up_chk;
	int i_distribute_p4_plate_down_chk;

	int i_distribute_plate_up_chk[4];
	int i_distribute_plate_down_chk[4];

	int o_distribute_p1_plate_vacuum;
	int o_distribute_p2_plate_vacuum;
	int o_distribute_p3_plate_vacuum;
	int o_distribute_p4_plate_vacuum;
	int o_distribute_p1_eject_vacuum;
	int o_distribute_p2_eject_vacuum;
	int o_distribute_p3_eject_vacuum;
	int o_distribute_p4_eject_vacuum;
	int i_distribute_p1_plate_vacuum_chk;
	int i_distribute_p2_plate_vacuum_chk;
	int i_distribute_p3_plate_vacuum_chk;
	int i_distribute_p4_plate_vacuum_chk;

	int o_distribute_plate_vacuum[4];
	int o_distribute_eject_vacuum[4];
	int i_distribute_plate_vacuum_chk[4];

	int o_distribute_rbt1_left_top_vacuum_fwdback;
	int o_distribute_rbt1_left_btm_vacuum_fwdback;
	int o_distribute_rbt1_left_box_guide_fwdback;
//	int o_distribute_rbt1_left_box_guide_updown;
	int o_distribute_rbt1_right_top_vacuum_fwdback;
	int o_distribute_rbt1_right_btm_vacuum_fwdback;
	int o_distribute_rbt1_right_box_guide_fwdback;
//	int o_distribute_rbt1_right_box_guide_updown;
	int o_distribute_rbt1_left_top_vacuum;
	int o_distribute_rbt1_left_top_eject;
	int o_distribute_rbt1_left_btm_vacuum;
	int o_distribute_rbt1_left_btm_eject;
	int o_distribute_rbt1_right_top_vacuum;
	int o_distribute_rbt1_right_top_eject;
	int o_distribute_rbt1_right_btm_vacuum;
	int o_distribute_rbt1_right_btm_eject;

//	int i_distribute_rbt1_left_top_vacuum_fwd_chk;
	int i_distribute_rbt1_left_top_vacuum_bwd_chk;
//	int i_distribute_rbt1_left_btm_vacuum_fwd_chk;
	int i_distribute_rbt1_left_btm_vacuum_bwd_chk;
//	int i_distribute_rbt1_left_box_guide_fwd_chk;
	int i_distribute_rbt1_left_box_guide_bwd_chk;
//	int i_distribute_rbt1_left_box_guide_up_chk;
//	int i_distribute_rbt1_left_box_guide_down_chk;
	int i_distribute_rbt1_left_top_vacuum_chk;
	int i_distribute_rbt1_left_btm_vacuum_chk;

	////2014,1203
	int i_distribute_rbt1_left_box_rotator_fwd_chk;
	int i_distribute_rbt1_left_box_rotator_bwd_chk;
	int o_distribute_rbt1_left_box_rotator_fwd_cylinder;
	int o_distribute_rbt1_left_box_rotator_bwd_cylinder;
	////


//	int i_distribute_rbt1_right_top_vacuum_fwd_chk;
	int i_distribute_rbt1_right_top_vacuum_bwd_chk;
//	int i_distribute_rbt1_right_btm_vacuum_fwd_chk;
	int i_distribute_rbt1_right_btm_vacuum_bwd_chk;
//	int i_distribute_rbt1_right_box_guide_fwd_chk;
	int i_distribute_rbt1_right_box_guide_bwd_chk;
//	int i_distribute_rbt1_right_box_guide_up_chk;
//	int i_distribute_rbt1_right_box_guide_down_chk;
	int i_distribute_rbt1_right_top_vacuum_chk;
	int i_distribute_rbt1_right_btm_vacuum_chk;
	
	////2014,1203
	int i_distribute_rbt1_right_box_rotator_fwd_chk;
	int i_distribute_rbt1_right_box_rotator_bwd_chk;
	int o_distribute_rbt1_right_box_rotator_fwd_cylinder;
	int o_distribute_rbt1_right_box_rotator_bwd_cylinder;
	////

	int o_distribute_rbt2_left_top_vacuum_fwdback;
	int o_distribute_rbt2_left_btm_vacuum_fwdback;
	int o_distribute_rbt2_left_box_guide_fwdback;
//	int o_distribute_rbt2_left_box_guide_updown;
	int o_distribute_rbt2_right_top_vacuum_fwdback;
	int o_distribute_rbt2_right_btm_vacuum_fwdback;
	int o_distribute_rbt2_right_box_guide_fwdback;
//	int o_distribute_rbt2_right_box_guide_updown;
	int o_distribute_rbt2_left_top_vacuum;
	int o_distribute_rbt2_left_top_eject;
	int o_distribute_rbt2_left_btm_vacuum;
	int o_distribute_rbt2_left_btm_eject;
	int o_distribute_rbt2_right_top_vacuum;
	int o_distribute_rbt2_right_top_eject;
	int o_distribute_rbt2_right_btm_vacuum;
	int o_distribute_rbt2_right_btm_eject;

//	int i_distribute_rbt2_left_top_vacuum_fwd_chk;
	int i_distribute_rbt2_left_top_vacuum_bwd_chk;
//	int i_distribute_rbt2_left_btm_vacuum_fwd_chk;
	int i_distribute_rbt2_left_btm_vacuum_bwd_chk;
//	int i_distribute_rbt2_left_box_guide_fwd_chk;
	int i_distribute_rbt2_left_box_guide_bwd_chk;
//	int i_distribute_rbt2_left_box_guide_up_chk;
//	int i_distribute_rbt2_left_box_guide_down_chk;
	int i_distribute_rbt2_left_top_vacuum_chk;
	int i_distribute_rbt2_left_btm_vacuum_chk;

	////2014,1203
	int i_distribute_rbt2_left_box_rotator_fwd_chk;
	int i_distribute_rbt2_left_box_rotator_bwd_chk;
	int o_distribute_rbt2_left_box_rotator_fwd_cylinder;
	int o_distribute_rbt2_left_box_rotator_bwd_cylinder;
	////


//	int i_distribute_rbt2_right_top_vacuum_fwd_chk;
	int i_distribute_rbt2_right_top_vacuum_bwd_chk;
//	int i_distribute_rbt2_right_btm_vacuum_fwd_chk;
	int i_distribute_rbt2_right_btm_vacuum_bwd_chk;
//	int i_distribute_rbt2_right_box_guide_fwd_chk;
	int i_distribute_rbt2_right_box_guide_bwd_chk;
//	int i_distribute_rbt2_right_box_guide_up_chk;
//	int i_distribute_rbt2_right_box_guide_down_chk;
	int i_distribute_rbt2_right_top_vacuum_chk;
	int i_distribute_rbt2_right_btm_vacuum_chk;

	////2014,1203
	int i_distribute_rbt2_right_box_rotator_fwd_chk;
	int i_distribute_rbt2_right_box_rotator_bwd_chk;
	int o_distribute_rbt2_right_box_rotator_fwd_cylinder;
	int o_distribute_rbt2_right_box_rotator_bwd_cylinder;
	////

	int o_distribute_rbt1_top_vacuum_fwdback[2];//left,right
	int o_distribute_rbt1_btm_vacuum_fwdback[2];
	int o_distribute_rbt2_top_vacuum_fwdback[2];
	int o_distribute_rbt2_btm_vacuum_fwdback[2];

	int o_distribute_rbt1_top_vacuum[2];//left,right
	int o_distribute_rbt1_btm_vacuum[2];
	int o_distribute_rbt2_top_vacuum[2];
	int o_distribute_rbt2_btm_vacuum[2];

	int o_distribute_rbt1_top_eject[2];//left,right
	int o_distribute_rbt1_btm_eject[2];
	int o_distribute_rbt2_top_eject[2];
	int o_distribute_rbt2_btm_eject[2];
	
	int o_distribute_rbt1_box_guide_fwdback[2];//left,right
//	int o_distribute_rbt1_box_guide_updown[2];
	int o_distribute_rbt2_box_guide_fwdback[2];//left,right
//	int o_distribute_rbt2_box_guide_updown[2];

	int i_distribute_rbt1_top_vacuum_fwd_chk[2];
	int i_distribute_rbt1_top_vacuum_bwd_chk[2];
	int i_distribute_rbt1_btm_vacuum_fwd_chk[2];
	int i_distribute_rbt1_btm_vacuum_bwd_chk[2];

	int i_distribute_rbt2_top_vacuum_fwd_chk[2];
	int i_distribute_rbt2_top_vacuum_bwd_chk[2];
	int i_distribute_rbt2_btm_vacuum_fwd_chk[2];
	int i_distribute_rbt2_btm_vacuum_bwd_chk[2];

	int i_distribute_rbt1_box_guide_fwd_chk[2];//left,right
	int i_distribute_rbt1_box_guide_back_chk[2];
	int i_distribute_rbt1_box_guide_up_chk[2];
	int i_distribute_rbt1_box_guide_down_chk[2];
	
	int i_distribute_rbt1_top_vacuum_chk[2];//left,right
	int i_distribute_rbt1_btm_vacuum_chk[2];
	int i_distribute_rbt2_top_vacuum_chk[2];
	int i_distribute_rbt2_btm_vacuum_chk[2];

	////2014,1203
	int i_distribute_rbt1_box_rotator_fwd_chk[2];
	int i_distribute_rbt1_box_rotator_bwd_chk[2];
	int o_distribute_rbt1_box_rotator_fwd_cylinder[2];
	int o_distribute_rbt1_box_rotator_bwd_cylinder[2];

	int i_distribute_rbt2_box_rotator_fwd_chk[2];
	int i_distribute_rbt2_box_rotator_bwd_chk[2];
	int o_distribute_rbt2_box_rotator_fwd_cylinder[2];
	int o_distribute_rbt2_box_rotator_bwd_cylinder[2];

	int	i_distribute_rbt_box_rotator_fwd_chk[4];
	int	i_distribute_rbt_box_rotator_bwd_chk[4];
	int o_distribute_rbt_box_rotator_fwd_cylinder[4];
	int	o_distribute_rbt_box_rotator_bwd_cylinder[4];
	////

	int i_distribute_rbt2_box_guide_fwd_chk[2];//left,right
	int i_distribute_rbt2_box_guide_back_chk[2];
//	int i_distribute_rbt2_box_guide_up_chk[2];
//	int i_distribute_rbt2_box_guide_down_chk[2];


	int o_reject_rbt_updown;
	int o_reject_rbt_vacuum;
	int o_reject_rbt_eject;

	int o_density_vision_trigger_onoff;
	int o_distribute_vision_trigger_onoff;
	int o_top_vision_trigger_onoff;
	int o_btm_vision_trigger_onoff;

	int i_reject_rbt_up_chk;
	int i_reject_rbt_down_chk;
	int i_reject_rbt_vacuum_chk;
	int i_reject_cv_full_chk;
	int i_reject_cv_1step_pos_chk;
	int i_reject_cv_full_pos_chk;

	int o_uld_rbt_left_updown;
	int o_uld_rbt_left_vacuum;
	int o_uld_rbt_right_updown;
	int o_uld_rbt_right_vacuum;
	int o_recycle_door_onoff_cylinder;//2014,1207

	int i_uld_rbt_left_up_chk;
	int i_uld_rbt_left_down_chk;
	int i_uld_rbt_left_vacuum_chk;
	int i_uld_rbt_right_up_chk;
	int i_uld_rbt_right_down_chk;
	int i_uld_rbt_right_vacuum_chk;
	int recycle_door_lock_chk;//2014,1207	
	int recycle_door_unlock_chk;//2014,1207

	int o_uld_rbt_picker[2];
	int i_uld_rbt_pickerup_chk[2];
	int i_uld_rbt_pickerdn_chk[2];

	int o_uld_rbt_vacuum[2];
	int i_uld_rbt_vacuum_chk[2];
	int o_uld_rbt_eject[2];

	int o_recycle_zone_box_pusher_fwd;
	int o_recycle_zone_box_pusher_back;
	int o_recycle_zone_box_vacuum;

	int i_recycle_zone_box_pusher_fwd_chk;
	int i_recycle_zone_box_pusher_bwd_chk;
	int i_recycle_zone_box_vacuum_chk;
	int i_recycle_zone_box_chk;
	int i_recycle_zone_box1_chk;
	int i_recycle_zone_box2_chk;
	int i_recycle_zone_light_curtain_chk;//2014,1207

	int i_uld_cv_left_box_chk;
	int i_uld_cv_right_box_chk;
	int i_uld_cv_box_out_chk;
	int i_uld_cv_box_full_chk;//2014,1207

	int i_uld_cv_box[3];

	int o_top_idbuffer_ld1_shifter_hook_fwd;
	int o_top_idbuffer_ld1_shifter_hook_bwd;
	int o_top_idbuffer_ld1_shifter_hook_clamp;
	int o_top_idbuffer_ld1_shifter_hook_unclamp;
	int o_top_idbuffer_ld2_shifter_left_hook_clamp;
	int o_top_idbuffer_ld2_shifter_left_hook_unclamp;
	int o_top_idbuffer_ld2_shifter_right_hook_clamp;
	int o_top_idbuffer_ld2_shifter_right_hook_unclamp;
	int i_top_idbuffer_ld1_shifter_hook_fwd_chk;
	int i_top_idbuffer_ld1_shifter_hook_bwd_chk;
	int i_top_idbuffer_ld1_shifter_hook_clamp_chk;
	int i_top_idbuffer_ld1_shifter_hook_unclamp_chk;
	int i_top_idbuffer_ld2_shifter_left_hook_clamp_chk;
	int i_top_idbuffer_ld2_shifter_left_hook_unclamp_chk;
	int i_top_idbuffer_ld2_shifter_right_hook_clamp_chk;
	int i_top_idbuffer_ld2_shifter_right_hook_unclamp_chk;


	int o_top_idbuffer_ld2_shifter_hook_fwd;
	int o_top_idbuffer_ld2_shifter_hook_bwd;
	int o_top_idbuffer12_clamp;
	int o_top_idbuffer12_unclamp;
	int o_top_idbuffer34_clamp;
	int o_top_idbuffer34_unclamp;
	int o_top_idbuffer56_clamp;
	int o_top_idbuffer56_unclamp;
	int o_top_idbuffer78_clamp;
	int o_top_idbuffer78_unclamp;
	int o_top_left_box_supportup_clamp;
	int o_top_left_box_supportdn_clamp;
	int o_top_right_box_supportup_clamp;
	int o_top_right_box_supportdn_clamp;


	int o_top_idbuffer_shifter_hook_fwd[2];
	int o_top_idbuffer_shifter_hook_bwd[2];
	int o_top_idbuffer_shifter_hook_clamp[3];
	int o_top_idbuffer_shifter_hook_unclamp[3];
	int o_top_idbuffer_clamp[4];
	int o_top_idbuffer_unclamp[4];
	int o_top_box_up_clamp[2];
	int o_top_box_dn_clamp[2];

	int i_top_idbuffer_shifter_hook_fwd[2];
	int i_top_idbuffer_shifter_hook_bwd[2];
	int i_top_idbuffer_shifter_hook_clamp[3];
	int i_top_idbuffer_shifter_hook_unclamp[3];
	int i_top_idbuffer_clamp[4];
	int i_top_idbuffer_unclamp[4];
	int i_top_idbuffer_chk[4][2];
	int i_top_idbuffer_box[4][2];


	int i_top_idbuffer_ld2_shifter_hook_fwd_chk;
	int i_top_idbuffer_ld2_shifter_hook_bwd_chk;
	int i_top_idbuffer12_clamp_chk;
	int i_top_idbuffer12_unclamp_chk;
	int i_top_idbuffer1_chk;
	int i_top_idbuffer2_chk;
	int i_top_idbuffer_box1_chk;
	int i_top_idbuffer_box2_chk;
	int i_top_idbuffer34_clamp_chk;
	int i_top_idbuffer34_unclamp_chk;
	int i_top_idbuffer3_chk;
	int i_top_idbuffer4_chk;
	int i_top_idbuffer_box3_chk;
	int i_top_idbuffer_box4_chk;

	////2014,1203
	int i_btm_idbuffer_uld1_shifter_buffer1_chk;
	int i_btm_idbuffer_uld1_shifter_buffer2_chk;
	////

	int i_top_idbuffer56_clamp_chk;
	int i_top_idbuffer56_unclamp_chk;
	int i_top_idbuffer5_chk;
	int i_top_idbuffer6_chk;
	int i_top_idbuffer_box5_chk;
	int i_top_idbuffer_box6_chk;
	int i_top_idbuffer78_clamp_chk;
	int i_top_idbuffer78_unclamp_chk;
	int i_top_idbuffer7_chk;
	int i_top_idbuffer8_chk;
	int i_top_idbuffer_box7_chk;
	int i_top_idbuffer_box8_chk;

//	int o_rolling_align_pusher_fwdback;
//	int o_rolling1_pusher_fwdback;
//	int o_rolling2_pusher_fwdback;
//
//	int o_rolling_push_fwdback[3];

	int o_rolling1_pusher_fwdback_left;
	int o_rolling1_pusher_fwdback_right;
	int o_rolling2_pusher_fwdback_left;
	int o_rolling2_pusher_fwdback_right;

	int o_rolling_push_fwdback[2][2];

	int i_rolling_align_pusher_left_fwd_chk;
	int i_rolling_align_pusher_left_back_chk;
	int i_rolling_align_pusher_right_fwd_chk;
	int i_rolling_align_pusher_right_back_chk;
	int i_rolling1_pusher_left_fwd_chk;
	int i_rolling1_pusher_left_back_chk;
	int i_rolling1_pusher_right_fwd_chk;
	int i_rolling1_pusher_right_back_chk;
	int i_rolling2_pusher_left_fwd_chk;
	int i_rolling2_pusher_left_back_chk;
	int i_rolling2_pusher_right_fwd_chk;
	int i_rolling2_pusher_right_back_chk;

	int i_rolling_push_fwd_chk[2][2];
	int i_rolling_push_bwd_chk[2][2];

	int o_idbuffer_elv_hook_clamp;
	int o_idbuffer_elv_hook_unclamp;
	int o_idbuffer_elv_pusher_fwd;
	int o_idbuffer_elv_pusher_bwd;

	int i_idbuffer_elv_hook_clamp_chk;
	int i_idbuffer_elv_hook_unclamp_chk;
	int i_idbuffer_elv_idbuffer1_chk;
	int i_idbuffer_elv_idbuffer2_chk;
	int i_idbuffer_elv_box1_chk;
	int i_idbuffer_elv_box2_chk;
	int i_idbuffer_elv_pusher_fwd_chk;
	int i_idbuffer_elv_pusher_bwd_chk;

	int o_btm_idbuffer_uld1_shifter_hook_fwd;
	int o_btm_idbuffer_uld1_shifter_hook_back;
	int o_btm_idbuffer12_clamp;
	int o_btm_idbuffer12_unclamp;
	int o_btm_idbuffer_uld2_shifter_hook_fwd;
	int o_btm_idbuffer_uld2_shifter_hook_back;
	int i_btm_idbuffer_uld1_shifter_hook_fwd_chk;
	int i_btm_idbuffer_uld1_shifter_hook_back_chk;
	int i_btm_idbuffer12_clamp_chk;
	int i_btm_idbuffer12_unclamp_chk;
	int i_btm_idbuffer1_chk;
	int i_btm_idbuffer2_chk;
	int i_top_left_box_supportup_chk;
	int i_top_left_box_supportdn_chk;
	int i_top_right_box_supportup_chk;
	int i_top_right_box_supportdn_chk;
//	int i_btm_idbuffer_uld2_shifter_hook_fwd1;
//	int i_btm_idbuffer_uld2_shifter_hook_back1;
//	int i_btm_idbuffer_uld2_shifter_hook_fwd2;
//	int i_btm_idbuffer_uld2_shifter_hook_back2;
	int i_top_box_up_clamp_chk[2];
	int i_top_box_dn_clamp_chk[2];

	int o_btm_idbuffer34_clamp;
	int o_btm_idbuffer34_unclamp;
	int o_btm_idbuffer_stopper_updown;

	int i_btm_idbuffer_uld2_shifter_hook_fwd1_chk;
	int i_btm_idbuffer_uld2_shifter_hook_back1_chk;
	int i_btm_idbuffer_uld2_shifter_hook_fwd2_chk;
	int i_btm_idbuffer_uld2_shifter_hook_back2_chk;
	int i_btm_idbuffer34_clamp_chk;
	int i_btm_idbuffer34_unclamp_chk;
	int i_btm_idbuffer3_chk;
	int i_btm_idbuffer4_chk;
	int i_btm_idbuffer_stopper_up_chk;
	int i_btm_idbuffer_stopper_down_chk;
	int i_btm_idbuffer_stopper_buffer_chk;
	int i_btm_idbuffer_8480_hook_in_chk;

	int o_btm_idbuffer_shifter_hook_fwd[2];
	int o_btm_idbuffer_shifter_hook_bwd[2];

	int o_btm_idbuffer_clamp[2];
	int o_btm_idbuffer_unclamp[2];

	int i_btm_idbuffer_shifter_hook_fwd[3];
	int i_btm_idbuffer_shifter_hook_bwd[3];

//	int i_btm_idbuffer_shifter_clamp[3];
//	int i_btm_idbuffer_shifter_unclamp[3];
	int i_btm_idbuffer_clamp[2];
	int i_btm_idbuffer_unclamp[2];
	int i_btm_idbuffer_chk[2][2];

	int i_top_idbuffer_exist_sensor_check[30]; //센서 체크는 총 20개 만 사용 
	int i_btm_idbuffer_exist_sensor_check[8];////센서 체크 4개

	//ASS'Y  : TL & DOOR
	int o_start_switch_lamp;									
	int o_stop_switch_lamp;										
	int o_alarm_clear_lamp;										
	int o_buzzer_off_lamp;										
	int o_reject_cv_motor_onoff;
	int o_uld_cv_motor_onoff;
	int o_recycle_zone_door_switch_lamp;//2014,1203
	int i_start_switch_chk;										
	int i_stop_switch_chk;										
	int i_alarm_clear_switch_chk;								
	int i_buzzer_switch_chk;									
	int i_recycle_zone_door_switch_chk;//2014,1203
// 	int i_auto_mode_switch_chk;									
// 	int i_manual_mode_switch_chk;								
	int i_front_select_switch_chk;								
	int i_rear_select_switch_chk;								

	int o_switch_lamp[4];

	//ASS'Y  : MC & EMO
	int o_main_air;												
	int o_towerlamp_green;										
	int o_towerlamp_yellow;										
	int o_towerlamp_red;										
	int o_buzzer_1;												
	int o_buzzer_2;												
	int o_buzzer_3;
	int o_door_lock;											
	int i_main_air_chk;											
	
	int i_mc1_chk;												
	int i_emo_1_chk;											
	int i_emo_2_chk;											
	int i_emo_3_chk;											
	int i_emo_4_chk;											

	int i_door1_chk;											
	int i_door2_chk;											
	int i_door3_chk;											
	int i_door4_chk;											
	int i_door5_chk;											
	int i_door6_chk;											
	int i_door7_chk;											
	int i_door8_chk;											
	int i_door9_chk;									
	int i_door10_chk;									

	int i_recycle_zone_door_chk;//2014,1203

	int i_bottom_door1;
	int i_bottom_door2;
	int i_bottom_door3;
	int i_bottom_door4;

	int o_buzzer[3];
	int i_door_chk[10];
	int i_emo_chk[4];

	//ASS'Y  : MC/EMO/SMEMA
	int o_front_top_machine_enter_signal;						
	int o_front_top_machine_transfer_signal;					
	int o_front_btm_machine_ready_signal;						
	int o_front_btm_machine_stop_signal;						
	int o_rear_ready_signal;
	int o_rear_enter_signal;

	int i_front_top_machine_ready_signal_chk;					
	int i_front_top_machine_stop_signal_chk;					
	int i_front_btm_machine_request_signal_chk;					
	int i_front_btm_machine_transfer_signal_chk;				
	int i_rear_req_signal_chk;	
	int i_rear_transfer_signal_chk;


	st_io_param();
};
extern st_io_param st_io;

class IO_Manager  
{
public:
	IO_Manager();
	virtual ~IO_Manager();

	BOOL Get_In_Bit( int iIoNum );
	BOOL Get_In_Bit( int iIoNum, int iDummy );
	BOOL Get_Out_Bit( int iIoNum );
	BOOL Get_Out_Bit( int iIoNum, int iDummy );
	int Set_Out_Bit( int nIONo, BOOL bOnOff ) ;
	int Set_IO_PortAutoEnable(int nMasterID, int nPortNo);
	int Set_IO_SlaveEnable(int nMasterID, int nPortNo, int nSlaveNo, BOOL bEnable);

	WORD Get_In_Word(int nIONo);
	WORD Get_Out_Word(int nIONo);

	int set_out_bit( int nIONo, BOOL bOnOff );
	BOOL get_in_bit( int iIoNum, int opt, int iDummy );
	BOOL get_in_bit( int iIoNum, int iDummy );
	BOOL get_in_bit( int iIoNum);
	BOOL get_out_bit( int iIoNum, int iDummy );

	void set_out_reverse( int nIONo );
	void set_out_reverse( int nFwd, int nBwd );

public:
	int IO_Board_Initialize();
	void OnSet_IO_Out_Port_Clear();


	void SetIOBoardInit( bool bVal )	{ m_bIOBoardInit = bVal; }
	bool GetIOBoardInit()	{ return m_bIOBoardInit; }

protected:
	int IO_Port_AutoEnable( int iMasterID, int iPortNo );
	int IO_SlaveEnable(int nMasterID, int nPortNo, int nSlaveCnt, BOOL bEnable);

protected:
	bool	m_bIOBoardInit;
};

extern IO_Manager g_ioMgr;

#endif // !defined(AFX_IO_MANAGER_H__EA6CB312_2364_436A_97C1_903DD2657C23__INCLUDED_)
