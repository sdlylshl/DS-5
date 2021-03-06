#ifndef __ALARM_H
#define __ALARM_H

u8 get_alarm_in(u8 in);
u8 get_alarm_out(u8 out);
u8 is_power_low(void);
u8 get_key(u8 key);
u8 led_off(u8 led);
u8 alarm_out_test(void);
u8 led_red(u8 led);
u8 led_green(u8 led);
u8 do_disarm_link(void);
u8 do_arm_link(void);
u8 do_alarm_link(u32 status);
u8 beep_off(void);


#endif
