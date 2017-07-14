#include <stdio.h>
#include <string.h>
#include <iostream>
#define kPlayerName "成会玩"
#define kPlaMax 500
#pragma once


using namespace std;

//修改人：杨丽颖
//存储各种信息
//日期：2017/7/13
//取消iPlaCount在模拟出牌中被使用改变的设定,iPlaArr中PASS首项为-3
struct Ddz
{
	int  iStatus;			//引擎状态-1错误,0结束,1开始
	char sCommandIn[80];	//通信输入内容
	char sCommandOut[80];	//通信输出内容
	int iOnHand[21];		//手中牌（所有值初始化为-1）
	int iOnTable[162][21];	//已出牌数组（所有值初始化为-2）每行是一手牌，以-1结尾，Pass记为-1
	int iUpPlayerCards;		//上家手中牌数
	int iDownPlayerCards;	//下家手中牌数
	int iToTable[21];		//要出的牌
	int iOnOtherHand[55];   //在其他玩家手中的牌
	char sVer[80];			//协议版本号
	char sName[80];			//参赛选手称呼
	char cDir;				//玩家方位编号
	char cLandlord;			//地主玩家方位编号
	char cLastPlay;         //非模拟出牌时上位出牌玩家方位编号
	char cWinner;			//胜利者方位编号
	int iBid[3];			//叫牌过程
	int iBmax;				//当前叫牌数，值域{0,1,2,3}
	int iOTmax;				//当前出牌手数
	int iRoundNow;			//当前局次
	int iRoundTotal;		//和总局数
	int iTurnNow;			//当前轮次
	int iTurnTotal;			//总轮数
	int iLevelUp;			//晋级选手数
	int iScoreMax;			//转换得分界限
	int iVoid;				//闲暇并行计算参数
	int iLef[3];			//本局底牌
	int iLastPassCount;		//当前桌面连续PASS数（值域[0,2],初值2，正常出牌取0，一家PASS取1，两家PASS取2）
	int iLastTypeCount;		//当前桌面牌型张数（值域[0,1108],初值0，iLastPassCount=0时更新值，=1时保留原值，=2时值为0）//最新出牌
	int iLastMainPoint;		//当前桌面主牌点数（值域[0,15],初值-1，iLastPassCount=0时更新值，，=1时保留原值，=2时值为-1）
};

