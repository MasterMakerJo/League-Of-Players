#include "CallNum.h"

CallNum::CallNum()
{
	CInfo = CardsInfo::Instance();
}

CallNum::~CallNum()
{
}

CallNum* CallNum::Instance()
{
	static CallNum instance;

	return &instance;
}



//牌力判断函数
//作者：刘云志
//时间：2016.4.1.13:11
//第一次修改：2016.7.12.17:13 逻辑修改
//第二次修改：2016.7.13.12:37 代码机构修改
//第三次修改：2016.7.16.15:04 修正牌型分析BUG
//第四次修改：2016.8.13.13:45 增加火箭追计4个牌力
//第五次修改：2016.8.15.15:53 取消火箭追计4个牌力
int CallNum::CardForce(int iCards[])
{
	int iMyCards[18] = { 0 };//手牌牌型

	//偶数为牌的大小，奇数为该牌的数量
	int iRepeat[27] = { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 };//重牌及张数
	
	int iOrder[18] = { 0 };//连牌 

	int iBomb[5] = { 0 };//炸弹
	int iPair[9] = { 0 };//对子
	int iThree[6] = { 0 };//三带一

	int iStraight[9] = { 0 };//顺子

	int iSingle = 0;//调张（单牌）

	int iEntry = 0;//进手张  ///A 2 大小王

	int iForce = 0;//牌力

	bool bLock = true;//锁机制

	//整理牌型，转换为实际点数，并计数进手张
	for (int i = 0; i < 17; i++)
	{
		if (iCards[i] / 4 == 0)
			iMyCards[i] = 3;    //3
		if (iCards[i] / 4 == 1)
			iMyCards[i] = 4;    //4
		if (iCards[i] / 4 == 2)
			iMyCards[i] = 5;    //5
		if (iCards[i] / 4 == 3)
			iMyCards[i] = 6;    //6
		if (iCards[i] / 4 == 4)
			iMyCards[i] = 7;    //7
		if (iCards[i] / 4 == 5)
			iMyCards[i] = 8;    //8
		if (iCards[i] / 4 == 6)
			iMyCards[i] = 9;    //9
		if (iCards[i] / 4 == 7)
			iMyCards[i] = 10;    //10
		if (iCards[i] / 4 == 8)
			iMyCards[i] = 11;    //J
		if (iCards[i] / 4 == 9)
			iMyCards[i] = 12;    //Q
		if (iCards[i] / 4 == 10)
			iMyCards[i] = 13;    //K
		if (iCards[i] / 4 == 11)
		{
			iMyCards[i] = 14;    //A
			iEntry++;
		}
		if (iCards[i] / 4 == 12)
		{
			iMyCards[i] = 22;    //2
			iEntry++;
		}
		if (iCards[i] == 52)
		{
			iMyCards[i] = 66;    //小王
			iEntry++;
		}
		if (iCards[i] == 53)
		{
			iMyCards[i] = 99;    //大王
			iEntry++;
		}
	}

	//对子、三带一、炸弹的第一次筛选
	for (int i = 0, j = 0; i < 17; i++)
	{
		if (iMyCards[i] == iMyCards[i + 1])
		{
			iRepeat[j] = iMyCards[i + 1];
			iRepeat[j + 1]++;
		}
		else
		{
			j += 2;
			///3～K才算单牌
			if (iMyCards[i] != iMyCards[i - 1] && iMyCards[i] != iMyCards[i + 1] && iMyCards[i] < 14)
			{
				iSingle++;
			}
		}
	}


	//第二次筛选
	for (int i = 1, j = 0, k = 0, l = 0; i < 26; i += 2)
	{	///炸弹计数j 对子计数k，三带一计数l
		//炸弹的计算
		if (iRepeat[i] == 4)
		{
			iBomb[j] = iRepeat[i - 1];
			j++;
		}
		//对子的计算
		if (iRepeat[i] == 2)
		{
			iPair[k] = iRepeat[i - 1];
			k++;
		}
		//三带一的计算
		if (iRepeat[i] == 3)
		{
			iThree[l] = iRepeat[i - 1];
			l++;
		}
	}


	//顺子的第一次筛选
	for (int i = 0, j = 0; i < 17; i++)
	{	
		///下标 0 1 2 3 4  5 6 7 8 9  10 11 12 13 14 15 16
		///牌   3 3 3 4 5  5 6 7 9 10 J  Q  K  A  2  2  2   iOrder[18]={0} iMyCards[18] = { 0 }
		///牌   4 6 7 9 10 J J J J Q  Q  Q  K  A  2  2  2
		if (iMyCards[i ] + 1 == iMyCards[i + 1])
		{	
			iOrder[j] = iMyCards[i];    ///如果两张相邻的牌递增，这里只记录了前一张
			j++;
		}
		///两张相邻的牌 后者大于前者2点及以上 或者为最后一张牌
		else if (iMyCards[i] + 1 < iMyCards[i + 1]|| i == 16)
		{
			///
			if (iOrder[j - 1] + 1 == iMyCards[i])
			{
				iOrder[j] = iMyCards[i];
				j += 2;
			}
		}
		else
		{
			continue;
		}
	}

	//顺子的第二次筛选
	for (int i = 0, j = 0; i < 17; i++)
	{
		if (iOrder[i] != 0)
		{
			if (bLock)
			{
				iStraight[j] = iOrder[i];
				j++;
				bLock = false;
			}
		}
		else
		{
			if (iOrder[i - 1] == 0 && iOrder[i + 1] == 0)
			{
				break;
			}
			else
			{
				if (iOrder[i - 1] - iStraight[j - 1] >= 4)
				{
					iStraight[j] = iOrder[i - 1];
					j++;
					iStraight[j] = 0;
					j++;
					bLock = true;
				}
				else
				{
					iStraight[j - 1] = 0;
					j--;
					bLock = true;
				}
			}
		}
	}


	//牌力的计算
	for (int i = 0; i < 17; i++)
	{
		//炸弹计4个牌力，如果是中间牌，就要减1。因为有时不得不拆掉炸弹去凑顺子。
		if (i < 4 && iBomb[i] != 0)
		{
			iForce += 4;
			if (iBomb[i] < 11 && iBomb[i] > 6)
			{
				iForce -= 1;
			}
		}

		//大王计2个牌力
		if (iMyCards[i] == 99)
		{
			iForce += 2;
		}
		//小王计1个牌力
		if (iMyCards[i] == 66)
		{
			iForce += 1;
		}

		//一对2计1个牌力
		if (i < 9 && iPair[i] == 2)
		{
			iForce += 1;
		}
		//3张J及以上计1个牌力
		if (i < 6 && iThree[i] > 10)
		{
			iForce += 1;
			//3张2加计2个牌力
			if (iThree[i] == 22)
			{
				iForce += 2;
			}
		}
		//顺子计1到2个牌力
		if (i < 9 && iStraight[i + 1] != 0 && iStraight[i] != 0)
		{
			if (iStraight[i + 1] - iStraight[i] > 8 || iStraight[i + 1] > 12)
			{
				iForce += 2;
			}
			else
			{
				iForce += 1;
			}
		}

	}

	//进手张大于掉张，掉张被容易带走  
	if (iEntry > iSingle)
	{
		iForce += 1;
	}
	//进手张小于掉张,掉张不易被带走
	if (iEntry < iSingle)
	{
		int iTemp = iSingle - iEntry;
		iForce -= iTemp;
	}

	return iForce;
}




//判断叫牌点数
//作者：刘云志
//时间：2016.3.31.12:52
//第一次修改：2016.4.1.13:10
//第二次修改：2016.4.20.23:36  校正上家叫牌点数计算方法
//第三次修改：2016.7.11.15:45  校正牌点判断
//第四次修改：2016.7.13.18:02  校正叫牌分析
//第五次修改：2016.7.16.15:04  校正叫牌精度
//第六次修改：2016.8.7.11:22  叫牌点数的修改
//第七次修改：2016.8.13.14:48  叫牌点数的修改
//第八次修改：2016.8.15.15:53  校正上家叫牌点数计算方法,修改是否是头家的判断方法
int CallNum::CallCardAnalyze(Ddz* pDdz)
{
	int iInHand = 0;//进手张
	int iPip = 0;//牌点
	int iCallPoint = -1;//最终叫牌数
	int iHeadCallPointSecond;//上家叫牌总点数
	int iHeadCallPointFirst;//上上家叫牌点数

	//获取上家叫牌数
	if (pDdz->iBid[0] == -1)
	{
		iHeadCallPointFirst = -1;//上上家叫牌点数
		iHeadCallPointSecond = -1;//上家叫牌点数
	}
	else if (pDdz->iBid[1] == -1)
	{
		iHeadCallPointFirst = -1;//上上家叫牌点数
		iHeadCallPointSecond = pDdz->iBid[0];//上家叫牌点数
	}
	else if (pDdz->iBid[2] == -1)
	{
		iHeadCallPointFirst = pDdz->iBid[0];//上上家叫牌点数
		iHeadCallPointSecond = pDdz->iBid[1];//上家叫牌点数
	}

	//牌点判断
	for (int i = 0; i<17; i++)
	{
		if (pDdz->iOnHand[i] >= 44)
		{
			iInHand = pDdz->iOnHand[i];
			if (iInHand >= 53)
				iPip += 4;
			else if (iInHand >= 52)
				iPip += 3;
			else if (iInHand >= 48)
				iPip += 2;
			else
				iPip += 1;
		}
	} 

	//叫牌分析
	//叫牌的基本条件需满足6个牌点
	if (iPip > 5)
	{
		int iPipNum = CardForce(pDdz->iOnHand) + iPip;

		if (iHeadCallPointFirst == -1 && iHeadCallPointSecond == -1)
		{
			if (iPipNum > 11)
			{
				iCallPoint = 3;
			}
			else if (iPipNum > 9)
			{
				iCallPoint = 2;
			}
			else if (iPipNum > 7)
			{
				iCallPoint = 1;
			}
			else
			{
				iCallPoint = 0;
			}
		}
		if (iHeadCallPointFirst == -1 && iHeadCallPointSecond != -1)
		{
			if (iPipNum > 11 && iHeadCallPointSecond != 3)
			{
				iCallPoint = 3;
			}
			else if (iPipNum > 9)
			{
				if (iHeadCallPointSecond <= 2)
				{
					iCallPoint = 3;
				}
				else
				{
					iCallPoint = 0;
				}
			}
			else if (iPipNum > 7)
			{
				if (iHeadCallPointSecond <= 1)
				{
					iCallPoint = 2;
				}
				else
				{
					iCallPoint = 0;
				}
			}
			else
			{
				if (iHeadCallPointSecond == 0)
				{
					iCallPoint = 1;
				}
				else
				{
					iCallPoint = 0;
				}
			}
		}
		if (iHeadCallPointFirst != -1 && iHeadCallPointSecond != -1)
		{
			if (iPipNum > 11 && iHeadCallPointFirst != 3 && iHeadCallPointSecond != 3)
			{
				iCallPoint = 3;
			}
			else if (iPipNum > 9)
			{
				if (iHeadCallPointSecond <= 2 && iHeadCallPointFirst <= 2)
				{
					iCallPoint = 3;
				}
				else
				{
					iCallPoint = 0;
				}
			}
			else if (iPipNum > 7)
			{
				if (iHeadCallPointSecond <= 1 && iHeadCallPointFirst <= 1)
				{
					iCallPoint = 2;
				}
				else
				{
					iCallPoint = 0;
				}
			}
			else
			{
				if (iHeadCallPointSecond == 0 && iHeadCallPointFirst == 0)
				{
					iCallPoint = 1;
				}
				else
				{
					iCallPoint = 0;
				}
			}
		}
	}
	else
		iCallPoint = 0;


	//判断是否有2和王，叫三分的前提，至少要有1个王，2个二
	int j = 0,k=0;
	if (iCallPoint == 3)
	{
		for (int i = 0; i < 17; i++)
		{
			if (pDdz->iOnHand[i]/4==12) 
			{
				j++;
			}
			else if (pDdz->iOnHand[i] / 4 == 13)
			{
				k++;
			}
		}
		if (j < 2 || k < 1) {
			iCallPoint = 2;
		}
	}

	//返回叫牌点数
	return iCallPoint;
}
