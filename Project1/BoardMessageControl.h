#pragma once
//////////////////////////////////
//////牌局信息控制台///////////////
//功能：用于初始化轮局，更新牌局////
////////////////////////////////
#include "Ddz.h"
#include "CardsInfo.h"
#include "MonteCarloSearchEngine.h"
#include "CallNum.h"
#include<list>


class BoardMessageControl
{
public:	BoardMessageControl();
	~BoardMessageControl();
	void InitTurn(struct Ddz * pDdz);				//P01 初始化本轮数据
	void InputMsg(struct Ddz * pDdz);				//P02 输入信息

	void AnalyzeMsg(struct Ddz * pDdz);				//P03 分析处理信息

	void GetDou(struct Ddz * pDdz);					//P0301 获取并处理版本信息DOU

	void GetInf(struct Ddz * pDdz);					//P0302 获取并处理轮局信息INF

	void GetDea(struct Ddz * pDdz);					//P0303 获取并处理牌套信息DEA

	void GetBid(struct Ddz * pDdz);					//P0304 获取并处理叫牌信息BID
 
	void GetLef(struct Ddz * pDdz);					//P0305 获取并处理底牌信息LEF

	void GetPla(struct Ddz * pDdz);					//P0306 获取并处理出牌信息PLA

	void CalPla(struct Ddz * pDdz);					//P030601 计算己方出牌策略

	void UpdateMyPla(struct Ddz * pDdz);			//P030602 根据己方出牌更新数据

	void UpdateHisPla(struct Ddz * pDdz);			//P030603 根据他人出牌更新数据

	void GetGam(struct Ddz * pDdz);					//P0307 获取并处理胜负信息GAM

	void OutputMsg(struct Ddz * pDdz);				//P04 输出信息

	void CalOthers(struct Ddz * pDdz);				//P05 计算其它数据

	void InitRound(struct Ddz * pDdz);				//M04 初始化本局数据

	int CalBid(struct Ddz * pDdz);					//I02 计算己方叫牌策略

	void OtherPlayerCards(Ddz *pDdz, int *iCards);
	
	CardsInfo* CInfo; //在InitTurn中被初始化
	MonteCarloSearchEngine *engine;//在InitTurn中被初始化

	PokerModelAnalysis*Pma;

	char cUpPlayer;
	char cDownPlayer;

};

