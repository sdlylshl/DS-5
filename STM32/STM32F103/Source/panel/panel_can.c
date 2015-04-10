#include "stm32f10x.h"
#define NUM 100

//
//typedef struct _sensor{
//	uint32_t id;
//	uint32_t value;
//}sensor_t;
//typedef struct _Actuator{
//	uint8_t VID;
//	uint32_t id;
//	uint32_t value;
//}Actuator_t;

typedef struct _node{
	struct _node *next;
	uint8_t VID;	//���������������� ����ID
	uint16_t value;
	uint32_t id;
}node_t;


typedef struct _policy{
	struct _policy *next;
	uint8_t sensornum;
	uint8_t actuatornum;
	uint8_t znum;
	node_t *actuator;
	node_t *sensor;

}policy_t;

typedef struct _config{
	uint32_t userID;	//�û�ID
	uint8_t  envID;		//����ID
	uint8_t  modeID;	//ģʽID
	uint8_t  policyID;	//����ID	
	uint8_t  policysize; //���Դ�С
	uint8_t  used;		//����״̬
	policy_t *policy;
	struct _config *next;
}config_t;

config_t config[NUM];

config_t* CreateConfig(){
	config_t *pconfig = mallc(sizeof(config_t));
	if (pconfig)
	{
		pconfig->userID = 0;
	}
	return pconfig;
}