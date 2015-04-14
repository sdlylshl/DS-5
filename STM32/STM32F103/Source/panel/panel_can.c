#include "stm32f10x.h"
#include "stdio.h"
#include "stdlib.h"
#include "panel_queue.h"
//[
//	userNUM
//	userID1[
//		envNUM
//			envID1[
//				modeNUM
//					mode1[
//						modeUsed   //指示当前模式是否启用
//							policyNUM    //策略数
//							policyID1[
//								size
//									relation
//									sensorNUM
//									(
//									cer
//									ID
//									value
//									)sensor1;
//								actorNUM
//									(
//									cer
//									ID
//									value
//									)
//
//							]
//
//							policyID2[
//							]
//					]
//					mode2[]
//			]
//			envID2[]
//			envIDn[]
//	]
//	userID2[]
//]

#define panel_free free
#define panel_mallc malloc

#define  PANEL_READ			0x01
#define  PANEL_WRITE_ENABLE		0x02

//***************************************************
typedef struct panel_node_s{
	uint32_t nodeID;
	uint16_t value;
	uint8_t nodeVID;	//传感器关联的网关 虚拟ID
	uint8_t nodeOP;		//传感器>=< //执行器表示
	//struct panel_node_s *next;
}panel_node_t;


//虚拟有效传感设备表
// 模式切换的时候 重新 初始化 
typedef struct panel_sensor_device_s{

	struct panel_node_s  sensor;
	struct panel_sensor_device_s * next;
	//struct panel_sensor_device_s * prev;
}panel_sensor_device_t;
panel_sensor_device_t *psensor_device_head;

//虚拟执行设备表
typedef struct panel_actuator_device_s{

	struct panel_node_s  actuator;
	void(*run)(uint8_t value);
	struct panel_actuator_device_s * next;
	//struct panel_actuator_device_s * prev;
}panel_actuator_device_t;

typedef struct panel_actuator_s{
	struct panel_node_s  actuator;
	panel_actuator_device_t *actuator_device;
	struct panel_actuator_s *next;
}panel_actuator_t;
typedef struct panel_sensor_s{
	struct panel_node_s  sensor;
	panel_sensor_device_t *sensor_device;
	//uint8_t nodeVID;	//传感器关联的网关 虚拟ID
	struct panel_sensor_s *next;
}panel_sensor_t;
typedef struct panel_policy_s{
	uint8_t  relation;						//策略关系
	uint8_t  policyID;						//策略ID
	uint8_t sensorNUM;						//传感器数量
	uint8_t actuatorNUM;					//执行器数量
	struct panel_node_s *sensor_head;		//传感器头指针
	struct panel_node_s *actuator_head;		//执行器头指针
	struct panel_mode_s *parent;			//[父模式]<--策略
	struct panel_policy_s *prev;			//上一个策略
	struct panel_policy_s *next;			//下一个策略
}panel_policy_t;	

typedef struct panel_mode_s{
	uint8_t  modeID;						//模式ID
	uint8_t  modeUSED;						//模式启用状态	
	uint8_t  policyNUM;						//策略数量
	//struct panel_policy_s  policy;
	struct panel_policy_s  *child_current;	//模式-->[子策略]
	struct panel_scene_s *parent;			//[父场景]<--模式
	struct panel_mode_s *prev;				//上一个模式
	struct panel_mode_s *next;				//下一个模式
}panel_mode_t;	

typedef struct panel_scene_s{

	uint8_t  sceneID;						//场景ID
	uint8_t  modeNUM;						//模式数量
	//struct panel_mode_s mode;				//模式链表头
	struct panel_mode_s *child_current;		//场景-->[子模式]
	struct panel_user_s *parent;			//[父用户]<--场景
	struct panel_scene_s *prev;				//场景
	struct panel_scene_s *next;				//场景
}panel_scene_t;	

typedef struct panel_user_s{
	uint32_t userID;						//用户ID
	uint32_t userPASS;						//用户密码
	uint8_t login;							//登录标志
	uint8_t  sceneNUM;						//场景数量
	//struct panel_scene_s scene;			//
	struct panel_scene_s *child_current;	//用户-->[子场景]	
	//struct panel_config_t *parent;		//[父配置]<--用户
	struct panel_user_s *prev;				//用户
	struct panel_user_s *next;				//用户
}panel_user_t;	

typedef struct panel_config_s{
	uint8_t  writeback;						//配置改动,回写保存后置0
	uint8_t  userNUM;						//可操作用户数
	//struct panel_user_s user;				//用户
	struct panel_user_s *child_current;		//配置-->[子用户]
}panel_config_t;


//
panel_config_t config; //只有一个配置
//panel_user_t cur_user;
//panel_scene_t cur_scene;
//panel_mode_t cur_mode;
//panel_policy_t cur_policy;

void panel_config_init(){
	//策略
	//policy.policyID = 0;
	//policy.relation = 0;
	//policy.sensorNUM = 0;
	//policy.actuatorNUM = 0;
	//policy.actuator_head = NULL;
	//policy.sensor_head = NULL;
	////policy.prev = &policy;
	////policy.next = &policy;
	//panel_queue_init(&policy);
	////模式
	//mode.policyNUM = 0;
	//mode.child_current = &policy;
	//mode.modeID = 0;
	//mode.modeUSED = 0;
	////mode.prev = &mode;
	////mode.next = &mode;
	//panel_queue_init(&mode);
	////场景
	//scene.modeNUM = 0;
	//scene.child_current = &mode;
	//scene.sceneID = 0;
	////scene.prev = &scene;
	////scene.next = &scene;
	//panel_queue_init(&scene);
	////用户
	//user.sceneNUM = 0;
	//user.child_current = &scene;
	//user.userID = 0;
	//user.userPASS = 0;
	//user.login = 0;
	////user.prev = &user;
	////user.next = &user;
	//panel_queue_init(&user);
	//配置
	config.userNUM = 0;
	config.writeback = 0;
	config.child_current = NULL;
}



// 0 没有找到 且创建失败 1 找到  2 创建   
uint8_t panel_get_user(uint8_t flag,uint32_t userID){
	//1.遍历当前配置 没有则创建 有则反悔 cur_usr
	uint8_t num = config.userNUM;
	panel_user_t * pu = config.child_current;

	for (; num;num--)
	{ 
		if (pu->userID == userID)
		{
			config.child_current = pu;
			return 1;
		}
		pu=pu->next;
	}
	if (flag&PANEL_WRITE_ENABLE){	
		//2.创建用户
		pu = (panel_user_t*)panel_mallc(sizeof(panel_user_t));
		if (pu)
		{
			pu->userPASS = 0;
			pu->login = 0;
			pu->sceneNUM = 0;
			pu->child_current = NULL;
		
			pu->userID = userID;
			config.userNUM++;
			panel_queue_insert_head(config.child_current, pu);
			config.child_current = pu;
			return 2;
		}
	}
	return 0;
}
uint8_t panel_get_scene(uint8_t flag, uint32_t userID, uint8_t sceneID){
	uint8_t num;
	panel_scene_t *ps ;
	if (panel_get_user(flag, userID))
	{ 
		ps = config.child_current->child_current;
		num =config.child_current->sceneNUM;
		for (;num; num--)
		{
			if (ps->sceneID == sceneID)
			{
				config.child_current->child_current = ps;
				return 1;
			}
			ps = ps->next;
		}

		if (flag&PANEL_WRITE_ENABLE){
		
			//创建场景
			ps = (panel_scene_t*)panel_mallc(sizeof(panel_scene_t));
			if (ps)
			{			
				ps->modeNUM = 0;
				ps->child_current = NULL;

				ps->sceneID = sceneID;
				ps->parent = config.child_current;
				config.child_current->sceneNUM++;
				panel_queue_insert_head(config.child_current->child_current, ps);
				config.child_current->child_current = ps;
				return 2;
			}		
		}
	}
	return 0;
}
uint8_t panel_get_mode(uint8_t flag, uint32_t userID, uint8_t sceneID, uint8_t modeID){
	uint8_t num;
	panel_mode_t * pm;
	if (panel_get_scene(flag, userID, sceneID))
	{
		num = config.child_current->child_current->modeNUM;
		pm = config.child_current->child_current->child_current;
		for (;num;num--)
		{
			if (pm->modeID == modeID)
			{
				config.child_current->child_current->child_current = pm;
				return 1;
			}
			pm = pm->next;
		}
		if (flag&PANEL_WRITE_ENABLE){
			//创建模式
			pm = (panel_mode_t*)panel_mallc(sizeof(panel_mode_t));
			if (pm)
			{
				pm->modeUSED = 0;
				pm->policyNUM = 0;
				pm->child_current = NULL;

				pm->modeID = modeID;
				pm->parent = config.child_current->child_current;
				config.child_current->child_current->modeNUM++;
				panel_queue_insert_head(config.child_current->child_current->child_current, pm);
				config.child_current->child_current->child_current = pm;
				return 2;
			}
		}
	}
	return 0;
}
uint8_t panel_get_policy(uint8_t flag, uint32_t userID, uint8_t sceneID, uint8_t modeID, uint8_t policyID){
	uint8_t num;
	panel_policy_t * pp;
	if (panel_get_mode(flag, userID, sceneID, modeID)){
		num = config.child_current->child_current->child_current->policyNUM;
		pp = config.child_current->child_current->child_current->child_current;
		for (; num; num--)
		{
			if (pp->policyID == policyID)
			{
				config.child_current->child_current->child_current->child_current = pp;
				return 1;
			}
			pp = pp->next;
		}
		if (flag&PANEL_WRITE_ENABLE){
			//创建策略
			pp = (panel_policy_t*)panel_mallc(sizeof(panel_policy_t));
			if (pp)
			{
				pp->relation = 0;
				pp->actuatorNUM = 0;
				pp->actuator_head = NULL;
				pp->sensorNUM = 0;
				pp->sensor_head = NULL;

				pp->policyID = policyID;
				pp->parent = config.child_current->child_current->child_current;
				config.child_current->child_current->child_current->policyNUM++;
				panel_queue_insert_head(config.child_current->child_current->child_current->child_current, pp);
				config.child_current->child_current->child_current->child_current = pp;
				return 2;
			}
		}
	}
	return 0;
}

//***********************接口********************************************
typedef struct panel_policy_data_s{
	uint8_t complete;
	uint32_t userID;
	uint8_t sceneID;
	uint8_t modeID;
	uint8_t policyID;
	uint8_t relation;
	uint8_t sensorNUM;
	uint8_t actuatorNUM;
	struct panel_node_s *sensor_head;
	struct panel_node_s *actuator_head;

}panel_policy_data_t;
panel_policy_data_t policy_data;

uint8_t panel_add_policy( panel_policy_data_t * policy_data){
	uint8_t num;
	panel_node_t *pn;
	panel_node_t *ppn;
	panel_policy_t * pp;
	num = panel_get_policy(PANEL_WRITE_ENABLE, policy_data->userID, policy_data->sceneID, policy_data->modeID, policy_data->policyID);
	if (num){
		pp = config.child_current->child_current->child_current->child_current;
		if (num == 1)
		{
			//表中已存在，修改当前模式
			pn = config.child_current->child_current->child_current->child_current->sensor_head;
			for (ppn=pn ; ppn;ppn = pn->next)
			{
				panel_free(pn);
				pn = ppn;
			}
			
			pn = config.child_current->child_current->child_current->child_current->actuator_head;
			for (ppn=pn ; ppn;ppn = pn->next)
			{
				panel_free(pn);
				pn = ppn;
			}
			
		}

		pp->relation = policy_data->relation;
		pp->actuatorNUM = policy_data->actuatorNUM;
		pp->actuator_head = policy_data->actuator_head;
		pp->sensorNUM = policy_data->sensorNUM;
		pp->sensor_head = policy_data->sensor_head;

		policy_data->relation = 0;
		policy_data->actuatorNUM = 0;
		policy_data->actuator_head = NULL;
		policy_data->sensorNUM = 0;
		policy_data->sensor_head = NULL;
		policy_data->complete = 0;
	}
	return num;
}
void panel_parse_policy_data(){


}
void panel_create_policy_data(){
	//panel_policy_data_t *policy_data = (panel_policy_data_t *)panel_mallc(sizeof());
	if (policy_data.complete)
	{
		panel_add_policy(&policy_data);
	}
	else{
	
		panel_parse_policy_data();
	
	}

}



panel_actuator_device_t *pactuator_device_head;

panel_sensor_device_t * find_sensor_device_by_id(uint32_t sensorID ){
	panel_sensor_device_t * psd = psensor_device_head;
	for (; psd; psd->next)
	{
		if (psd->devID == sensorID)
		{
			return psd;
		}
	}
	return NULL;
}
panel_actuator_device_t * find_actuator_device_by_id(uint32_t actuatorID){
	panel_actuator_device_t * pad = pactuator_device_head;
	for (; pad; pad->next)
	{
		if (pad->devID == actuatorID)
		{
			return pad;
		}
	}
	return NULL;
}

void policy_operate(){
	uint8_t i;
	panel_user_t * puser;
	panel_scene_t * pscene;
	panel_mode_t * pmode;
	panel_policy_t * ppolicy;
	panel_node_t *pnode;
	panel_sensor_device_t * psd;
	for (puser = config.child_current; puser; )
	{
		for (pscene = config.child_current->child_current; pscene;)
		{
			for (pmode = config.child_current->child_current->child_current; pmode;)
			{

				for (ppolicy = config.child_current->child_current->child_current->child_current; ppolicy;)
				{
					//策略执行
					pnode = ppolicy->sensor_head;
					if (ppolicy->relation){
					//或执行
						for (i= 0; i < ppolicy->sensorNUM; i++)
						{
							
							psd = find_sensor_device_by_id(pnode->nodeID);
							if (psd)
							{
								if (psd->value == pnode->value)
								{
									//
									


								}
							}
							pnode = pnode->next;
						}
					}
					else{
					
					
					}

					ppolicy = ppolicy->next;
					if (config.child_current->child_current->child_current->child_current == ppolicy)
					{
						break;
					}
				}

				pmode = pmode->next;
				if (config.child_current->child_current->child_current == pmode)
				{
					break;
				}

			}
			pscene = pscene->next;
			if (config.child_current->child_current == pscene)
			{
				break;
			}

		}
		puser = puser->next;
		if (config.child_current == puser){
			break;
		}
	}



}