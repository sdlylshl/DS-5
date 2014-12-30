#include "stdio.h"

#include "loopback_client.h"
// 宏定义
#include "./W5500/Ethernet_config.h"
// TIM4_GetCurrentTime()
#include "./System/System_config.h"

#define tick_second 100

uint32_t my_time;
uint32_t presentTime;
uint32_t keep_alive_time;
uint32_t cruise_interval;
uint32_t received_len;

uint32_t time_return(void) {
	return TIM4_GetCurrentTime();
}

//#define TX_RX_MAX_BUF_SIZE	300

extern uint8_t TX_BUF[]; // TX Buffer for applications
//extern uint8_t RX_BUF[]; // RX Buffer for applications

//1.获取剩余空间大小

//2.获取可写空间大小 （TX_RX_MAX_BUF_SIZE-head )|(tail-head) 较小值 【一次可填充大小】

//3.确保写操作不会造成head 跑到 tail的前面

//4.确保读操作不会造成tail 跑到 head的前面

unsigned char calcfcs(unsigned char *pmsg, unsigned char len) {
	unsigned char result = 0;
	while (len--) {
		result ^= *pmsg++;
	}
	return result;
}

extern uint16_t DEST_PORT; //DST_IP port

#define	MAX_SOCK_NUM		8	/**< Maxmium number of socket */
//socket状态机
extern uint8_t ch_status[MAX_SOCK_NUM]; /** 0:close, 1:ready, 2:connected 3:init*/
uint16_t any_port = 1000;

void loopback_tcpc(uint8_t s, uint8_t * dstip, uint16_t dstport) {
	uint16_t RSR_len;
	uint8_t * data_buf = TX_BUF;

	uint8_t res = getSn_SR(s);
	switch (res) {
	//Socket n的连接状态。在此状态下，可以使用 SEND 或者 RECV 命令进行数据包传输。
	case SOCK_ESTABLISHED: /* if connection is established */
		//printf("SOCK_ESTABLISHED\n");
		if (time_return() - presentTime >= (tick_second * 2)) {
			presentTime = time_return();
			//printf("SOCK_ESTABLISHED\n");
		}
		if (ch_status[s] == 1 || ch_status[s] == 4) {
			printf("\r\n%d : Connected", s);
			ch_status[s] = 2;
		}

#if 0
		if ((RSR_len = getSn_RX_RSR(s)) > 0)
		{
			printf("RSR_len: 0x%x\n", RSR_len);
			if (RSR_len > TX_RX_MAX_BUF_SIZE)
			RSR_len = TX_RX_MAX_BUF_SIZE;

			received_len = recv(s, data_buf, RSR_len);
			send(s, data_buf, received_len);
		}
#endif
		//接收数据
		//tcpc_RevcDatToBuff(s);
		//do_tcp_alive(s, mode);
		//cruise_io(s, mode);
		//recv_loop(s, mode);

		break;
		//4.Socket n 接收到了来自连接对方发来的断开连接请求
		//若要全部关闭，需要使用 DISCON 命令。
		//要关闭Socket，需要使用 CLOSE命令。
	case SOCK_CLOSE_WAIT:
		if (time_return() - presentTime >= (tick_second * 2)) {
			presentTime = time_return();
			printf("SOCK_CLOSE_WAIT\n");
		}
		/* If the client request to close */
		//printf("\r\n%d : CLOSE_WAIT", s);
		if ((RSR_len = getSn_RX_RSR(s)) > 0) /* check Rx data */
		{
			if (RSR_len > TX_RX_MAX_BUF_SIZE
				) RSR_len = TX_RX_MAX_BUF_SIZE; /* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */
			/* the data size to read is MAX_BUF_SIZE. */
			recv(s, data_buf, RSR_len); /* read the received data */
		}
		disconnect(s);
		ch_status[s] = 0;
		break;
		//0. Socket n处于关闭状态，资源被释放
	case SOCK_CLOSED:
		//printf("SOCK_CLOSED\n");
		if (time_return() - presentTime >= (tick_second * 2)) {
			presentTime = time_return();
			printf("SOCK_CLOSED\n");
		}
		/* if a socket is closed */
		if (!ch_status[s]) {
			printf("\r\n%d : Loop-Back TCP Client Started. port: %d", s,
					DEST_PORT);
			ch_status[s] = 1;
		}
		if (socket(s, Sn_MR_TCP, any_port++, 0x00) == 0) /* reinitialize the socket */
		{
			printf("\a%d : Fail to create socket.", s);
			ch_status[s] = 0;
		}
		break;
		//1. 0x13  SOCK_INIT 该位指示了 Socket n 端口打开并处于 TCP工作模式。
	case SOCK_INIT: /* if a socket is initiated */
		//printf("SOCK_INIT\n");
		if (ch_status[s] == 2)
			ch_status[s] = 4;
		if (time_return() - presentTime >= (tick_second * 2)) {
			connect(s, dstip, dstport);
			presentTime = time_return();
			printf("SOCK_INIT\n");
		}
		break;
	default:
		if (time_return() - presentTime >= (tick_second * 2)) {
			presentTime = time_return();
			printf("default\n");
		}
		break;
	}
}

