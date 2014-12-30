#ifndef BOOTPARAMS_H
#define	BOOTPARAMS_H

typedef struct {
	char flags[12];
	unsigned int val;
} ParamItem;

typedef struct {
	ParamItem start;
	//ParamItem cpu_clk;
	ParamItem boot_delay;
	
	ParamItem serial_sel;
	ParamItem AppRun_addr;
	ParamItem serial_baud;
	
	ParamItem machine;
	ParamItem run_addr;
	ParamItem root_sel;
	ParamItem tty_sel;
	ParamItem display_sel;
	ParamItem display_mode;
	ParamItem initrd_addr;
	ParamItem initrd_len;
	ParamItem mem_cfg;
	//ParamItem devfs_sel;
	//ParamItem osstor;
	ParamItem appsize;	//NEW ADD FOR V3
	ParamItem debug;	//NEW ADD FOR V3
	ParamItem diagnose;	//NEW ADD FOR V3
	ParamItem comdown;
	ParamItem xosc;
	ParamItem user_params;
	char string[128];
	unsigned int bpage[50];

//每个ParamItem占用16字节  共占用 16*18 +128+50*4 =616
//800个字节最多可以储存29条ParamItem
} BootParams;

typedef struct {
	ParamItem vid;
	ParamItem pid;
	ParamItem ser_l;
	ParamItem ser_h;
	ParamItem user_params;
	char string[128];
} VenderParams;

extern int search_params(void);
extern int save_params(void);
extern int set_params(void);

#define	DEFAULT_USER_PARAMS	" "

#ifdef GLOBAL_PARAMS

//小于等于512个字节,最多保存24个ITEM和128字节用户定义的字符串
BootParams boot_params = {
	//启动设置
	{"auto-run", 1},	//0=boot without parameters,1=boot with parameters
	//{"cpuclk",   2},	//0=200M, 1=300M, 2=400M, 3=440M
	{"rundelay", 0},	//0 seconds
	// 串口设置
	{"serial",   0},	//0=serial port 0, 1=serial port 1
	{"AppRunAddr",   0x32000000},
	{"baudrate", 115200},
	//机器信息设置 for Linux 
	{"machine",  913},	
	{"runAddr",  0x30201000},
	{"rootfs",   3},
	{"tty",      0},
	
	{"displayS",  1},	//0=320*240  1=640*480	2 = 800*600
	{"displayM",  0},	//0= lcd	1=vga	2=tv
	{"initrdA",  0x30200000},	//RAM 初始化地址
	{"initrdL",  0x02000000},	//初始化RAM长度
	{"memsize",  0x04000000},	//RAM存储器大小 64M
	
	//PCB6V3
	
	{"AppSize", 0},			//NEW ADD FOR V3 	应用程序实际大小
	{"Debug", 0},			//NEW ADD FOR V3 	是否打印调试信息
	{"diagnose",0},		//NEW ADD FOR V3 	首次启动为0 自动进入测试模式 
	{"Com Usb",1},			//NEW ADD FOR V3    0 串口下载 1 usb下载
	{"16XOSC8",0},			// 					CAN 模块晶振 0 16M 、1 8M
	//{"devfs",    1},
	//{"ostore",   0},	//0=nand, 1=nor
	{"userpara", sizeof(DEFAULT_USER_PARAMS)},
	DEFAULT_USER_PARAMS,
	{0}
};

//小于等于256字节
VenderParams vend_params = {
	{"VendID",   0x76543210},
	{"ProdID",   0xfedcba98},
	{"Serial_L", 0x01234567},
	{"Serial_H", 0x89abcdef},
	{"userpara", 1},	//0=data, 1=string
	"www.witech.com.cn"
};

#else
//注意boot_params 不要大于0x800字节
extern BootParams boot_params;
extern VenderParams vend_params;

#endif

#endif