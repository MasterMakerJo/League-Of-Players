#include "CallNum.h"

CallNum::CallNum()
{
	CInfo = CardsInfo::Instance();
	pSplitCards = new SplitCard();
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


	int*single = pSplitCards->startSplitCard(iCards, 7);

	for (int i = 0; single[i] != -1; i++)
	{
		if (single[i] != -2&&single[i]<44)
		{
			iSingle++;
		}
	}



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
		}
	}


	//第二次筛选
	for (int i = 1, j = 0, k = 0, l = 0; i < 26; i += 2)
	{	//炸弹计数j 对子计数k，三带一计数l
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



/*	这个有点问题，还是调用splitcard里的吧。。
//单牌的计算 (顺子中的单牌不算，大于等于A的单牌不算) 
	for (int i = 0; i < 17; i++)
	{
		if (iMyCards[i] != iMyCards[i - 1] && iMyCards[i] != iMyCards[i + 1] && iMyCards[i] < 14)
		{
			iSingle++;
			//有顺子，检查单牌是否在顺子中

			for (int j = 0; j < 9; j = j + 3)//iStraight数组的格式为 顺子起点，顺子结点，0，顺子起点，顺子结点，0
			{
				if (iStraight[j] != 0)
				{
					if (iMyCards[i] >= iStraight[j] && iMyCards[i] <= iStraight[j + 1])//如果单牌在顺子中 单牌--
					{
						iSingle--;
					}
				}
			}
		}
	}

	for (int j = 0; j < 9; j = j + 3)//iStraight数组的格式为 顺子起点，顺子结点，0，顺子起点，顺子结点，0
	{
		if (iStraight[j] != 0)
		{
				for (int k = 0; k < 9; k++)
				{
					if (iPair[k] != 0 && iPair[k] >= iStraight[j] && iPair[k] <= iStraight[j + 1])//如果对子在顺子中，单牌++
					{
						iSingle++;
					}
				}
		}
	}*/
	

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

		//QQ,KK,AA牌力值加1
		if (i < 9 && iPair[i] >=12&&iPair[i]!=22)
		{
			iForce += 1;
		}

		//3张J及以上计1个牌力
		if (i < 6 && iThree[i] > 10&& iThree[i]!=22)
		{
			iForce += 1;
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

	//进手张小于掉张,掉张不易被带走
	if (iEntry < iSingle)
	{
		int iTemp = iSingle - iEntry;
		iForce -= iTemp;
	}

	//如果单牌数小于3，增加牌力值
	if (iSingle < 3)
	{
		iForce +=3 - iSingle;
	}
	cout << " SingleNumber " << iSingle << endl;
	return iForce;
}




//判断叫牌点数
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

	int bomb=0;
	int one=0;
	//牌点判断
	for (int i = 0; i<17; i++)
	{
		iInHand = pDdz->iOnHand[i];
		if (iInHand >= 53)		//大王
			//iPip += 3;
			bomb += 2;
		else if (iInHand >= 52)	//小王
			bomb += 1;
		else if (iInHand >= 48)	//2
			iPip++;
		else if (iInHand >= 44)
			one++;
	} 

	if (bomb == 3)
		iPip += 7;
	if (bomb == 1)
		iPip += 2;
	if (bomb == 2)
		iPip += 3;
	if (one >= 2)
		iPip++;

	//叫牌分析
	//叫牌的基本条件需满足4个牌点
	if (iPip >=4)
	{
		int iPipNum = CardForce(pDdz->iOnHand) + iPip;
		//Test
		cout << "Card Force: " << iPipNum-iPip << " 牌点值： " << iPip << " iPipNum " << iPipNum << endl;
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
	{
		int iPipNum = CardForce(pDdz->iOnHand) + iPip;
		//Test
		cout << "Card Force: " << iPipNum - iPip << " 牌点值： " << iPip << " iPipNum " << iPipNum << endl;
		iCallPoint = 0;
	}
		


	//判断是否有2和王，叫三分的前提，至少要有1个王，2个二 或者两个王一个二
	int j = 0,k=0;
	if (iCallPoint == 3)
	{
		for (int i = 0; i < 17; i++)
		{
			if (pDdz->iOnHand[i]/4==12) 
			{
				j++;//2
			}
			else if (pDdz->iOnHand[i] / 4 == 13)
			{
				k++;//王
			}
		}
		if (j < 2 || k < 1) {
			iCallPoint = 2;
		}
		if (j >= 1 && k == 2)
		{
			iCallPoint = 3;
		}
	}

	//返回叫牌点数
	return iCallPoint;
}
