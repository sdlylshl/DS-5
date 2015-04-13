#include "stm32f10x.h"
#include "stdio.h"

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

//***************************************************
typedef struct panel_node_s{
	uint32_t nodeID;
	uint16_t value;
	uint8_t nodeVID;	//传感器关联的网关 虚拟ID
	uint8_t nodeOP;		//备用传感器属性
	struct panel_node_s *node_next;
}panel_node_t;

typedef struct panel_policy_s{
	uint8_t  relation;	//策略关系 与或非	
	uint8_t  policyID; //策略ID
	uint8_t sensorNUM;
	uint8_t actuatorNUM;
	struct panel_node_s *sensor_head;
	struct panel_node_s *actuator_head;

	struct panel_policy_s *policy_next;
}panel_policy_t;	//策略

typedef struct panel_mode_s{
	uint8_t  modeID;		//模式ID
	uint8_t  modeUSED;		//启用状态	
	uint8_t  policyNUM;		//策略数量
	struct panel_policy_s  policy;
	struct panel_policy_s  *policy_end;
	struct panel_policy_s  *policy_current;
	struct panel_mode_s *mode_next;
}panel_mode_t;	//模式

typedef struct panel_scene_s{

	uint8_t  sceneID;					//场景ID
	uint8_t  modeNUM;					//模式数量
	struct panel_mode_s mode;			//模式链表头
	struct panel_mode_s *mode_end;			
	struct panel_mode_s *mode_current;		
	struct panel_scene_s *scene_next;
}panel_scene_t;	//场景

typedef struct panel_user_s{
	uint32_t userID;		//用户ID
	uint32_t userPASS;		//用户密码
	uint8_t login;			//登录标志
	uint8_t  sceneNUM;		//场景数量
	struct panel_scene_s scene;			//
	struct panel_scene_s *scene_end;	//
	struct panel_scene_s *scene_current;//当前使用的场景	
	struct panel_user_s *user_next;
}panel_user_t;	//用户

typedef struct panel_config_s{
	uint8_t  writeback;				//配置改动,回写保存后置0
	uint8_t  userNUM;				//可操作用户数
	struct panel_user_s user;		//用户
	struct panel_user_s *user_end;		//最后一个用户
	struct panel_user_s *user_current;	//当前登录用户
}panel_config_t;

//
panel_config_t config;

int8_t panel_config_init(){	
	//配置
	config.writeback = 0;
	config.userNUM = 0;
	config.user_current = &config.user;
	config.user_end = config.user_current;
	//用户
	config.user.user_next = NULL;
	config.user.userID = 0;
	config.user.userPASS = 0;
	config.user.login = 0;
	
	config.user.sceneNUM = 0;
	config.user.scene_current = &config.user.scene;
	config.user.scene_end = config.user.scene_current;
	//场景
	config.user.scene.scene_next = NULL;
	config.user.scene.sceneID = 0;

	config.user.scene.modeNUM = 0;
	config.user.scene.mode_current = &config.user.scene.mode;
	config.user.scene.mode_end = config.user.scene.mode_current;
	//模式
	config.user.scene.mode.mode_next = NULL;
	config.user.scene.mode.modeID = 0;
	config.user.scene.mode.modeUSED = 0;

	config.user.scene.mode.policyNUM = 0;
	config.user.scene.mode.policy_current = &config.user.scene.mode.policy;
	config.user.scene.mode.policy_end = config.user.scene.mode.policy_current;
	//策略
	config.user.scene.mode.policy.policy_next = NULL;
	config.user.scene.mode.policy.policyID = 0;
	config.user.scene.mode.policy.relation = 0;
	config.user.scene.mode.policy.sensorNUM = 0;
	config.user.scene.mode.policy.actuatorNUM = 0;

	config.user.scene.mode.policy.sensor_head = NULL;
	config.user.scene.mode.policy.actuator_head = NULL;

	//每条策略 最少有一个传感器和一个执行器
	/*config.user.scene.mode.policy.sensor.nodeID = 0;
	config.user.scene.mode.policy.sensor.nodeVID = 0;
	config.user.scene.mode.policy.sensor.nodeOP = 0;
	config.user.scene.mode.policy.sensor.value = 0;
	config.user.scene.mode.policy.sensor.node_next = NULL;

	config.user.scene.mode.policy.actuator.nodeID = 0;
	config.user.scene.mode.policy.actuator.nodeVID = 0;
	config.user.scene.mode.policy.actuator.nodeOP = 0;
	config.user.scene.mode.policy.actuator.value = 0;
	config.user.scene.mode.policy.actuator.node_next = NULL;*/
}
//******************************************************

panel_user_t * panel_create_user(panel_config_t *pconfig){
	panel_user_t *u = callc(sizeof(panel_user_t));
	if (u)
	{
		pconfig->user.user_next = u;
		
	}
	return u;
}


//
panel_policy_t * panel_init_policy(uint8_t sn,uint8_t an){	
	//1.创建传感器节点
	panel_node_t *node = collc(sizeof(panel_node_t)* (sn + an));

	if (node){
		//给节点赋值
	}
}

void  panel_chg_policy(){
	//1.get policy

}
typedef struct panel_id_s
{
	uint32_t userID;
	uint8_t sceneID;
	uint8_t  modeID;
	uint8_t  policyID;
}panel_id_t;

//1.根据策略ID获取 一个config  
//2.如果没有 则创建一个新的策略
//3.成功返回 panel_config_t 失败返回NULL 属性送入user_current
panel_config_t * panel_get_policy(panel_config_t *pconfig, panel_id_t id){
	uint8_t i;
	panel_user_t *u;
	if (pconfig->user_current->scene_current->mode_current->policy_current->policyID == id.policyID)
	{
		return pconfig;
	}
	else if (pconfig->user_current->scene_current->mode_current->modeID == id.modeID)
	{
		//查找策略表

		//未找到创建新策略
		panel_policy_t * p = callc(sizeof(panel_policy_t));
		if (p)
		{
			pconfig->user_current->scene_current->mode_current->policy_end->policy_next = p;
			pconfig->user_current->scene_current->mode_current->policy_end = p;

			pconfig->user_current->scene_current->mode_current->policy_current = p;
			pconfig->user_current->scene_current->mode_current->policy_current->policyID = p;
			pconfig->user_current->scene_current->mode_current->policy_current->sensorNUM = 0;
			pconfig->user_current->scene_current->mode_current->policy_current->actuatorNUM = 0;
			return pconfig;
		}
		else{
			return NULL;
		}
	}
	else if (pconfig->user_current->scene_current->sceneID == id.sceneID)
	{
		//1.查找mode 

		//2.创建mode
		panel_mode_t * m = callc(sizeof(panel_node_t));
		if (m)
		{

			return pconfig;
		}
		else
		{
			return NULL;
		}
	}
	else if (pconfig->user_current->userID == id.userID)
	{
	
	}
	else
	{

	}
//******************************************************
	//1. 检索 pconfig 中是否存在 user 没有则创建
	if (pconfig->user_current->userID == id.userID)
	{
		goto second;
	}
	else
	{
		pconfig->user_current = &pconfig->user;
	}
	for (i = 0; i < pconfig->userNUM; i++)
	{
		if (pconfig->user.userID == id.userID)
		{
			goto second;
		}
		else
		{
			pconfig->user_current = pconfig->user.user_next;
		}

	}
	//没有找到则 创建user
	 u = callc(sizeof(panel_user_t));
	if (u)
	{
		
		pconfig->user_end->user_next = u;
		pconfig->user_end = u;
		pconfig->userNUM++;
		pconfig->user_end->userID = id.userID;
		pconfig->user_current = u;
	}
	else
	{
		return;
	}

	//2. 检索 场景
second:
	if (pconfig->user_current->scene_current->sceneID == id.sceneID)
	{
		goto third;
	}
	else
	{
		pconfig->user_current->scene_current = &pconfig->user_current->scene;
	}

	for (i = 0; i < pconfig->user_current->sceneNUM; i++)
	{
		if (pconfig->user_current->scene.sceneID == id.sceneID)
		{
			goto third;
		}
		pconfig->user_current->scene_current = pconfig->user_current->scene_current->scene_next;
	}
	
	panel_scene_t *s = callc(sizeof(panel_scene_t));
	if (s)
	{
		pconfig->user_current->scene_end->scene_next = s;
		pconfig->user_current->scene_end = s;
		pconfig->user_current->scene_current = s;
		pconfig->user_current->sceneNUM++;
		pconfig->user_current->scene_current->sceneID = id.sceneID;
	}
	else
	{
		return;
	}
	//3. 模式配置
third:
	if (pconfig->user_current->scene_current->mode_current->modeID == id.modeID)
	{
		goto forth;
	}
	else
	{
		pconfig->user_current->scene_current->mode_current = &pconfig->user_current->scene_current->mode;
	}

	for (i = 0; i < pconfig->user_current->scene_current->modeNUM; i++)
	{
		if (pconfig->user_current->scene_current->mode_current == id.modeID)
		{
			goto forth;
		}
		else
		{
			pconfig->user_current->scene_current->mode_current = pconfig->user_current->scene_current->mode_current->mode_next;
		}
	}

	panel_scene_t *s = callc(sizeof(panel_scene_t));
	if (s)
	{
		pconfig->user_current->scene_end->scene_next = s;
		pconfig->user_current->scene_end = s;
		pconfig->user_current->scene_current = s;
		pconfig->user_current->sceneNUM++;
		pconfig->user_current->scene_current->sceneID = id.modeID;
	}
	else
	{
		return;
	}
	// 策略
forth:
	if (pconfig->user_current->scene_current->mode_current->policy_current->policyID == id.policyID)
	{
		goto forth;
	}
	else
	{
		pconfig->user_current->scene_current->mode_current = &pconfig->user_current->scene_current->mode;
	}

	for (i = 0; i < pconfig->user_current->scene_current->modeNUM; i++)
	{
		if (pconfig->user_current->scene_current->mode_current == id.sceneID)
		{
			goto forth;
		}
		else
		{
			pconfig->user_current->scene_current->mode_current = pconfig->user_current->scene_current->mode_current->mode_next;
		}
	}

	panel_scene_t *s = callc(sizeof(panel_scene_t));
	if (s)
	{
		pconfig->user_current->scene_end->scene_next = s;
		pconfig->user_current->scene_end = s;
		pconfig->user_current->scene_current = s;
		pconfig->user_current->sceneNUM++;
		pconfig->user_current->scene_current->sceneID = id.sceneID;
	}
	else
	{
		return;
	}
}