#include "RecommendCards.h"
void print(int*temp);
//出牌生成器初始化
//优化了单牌牌型判断，增加了出炸弹的情况
//添加专为模拟出牌使用的出牌函数
int RecommendCards::InitPlaGenerate(fDdz* pDdz)
{
	int i, j;
	for (i = 0; i<PlaMax; i++)		//清空推荐出牌数组
		for (j = 0; j<21; j++)
			pDdz->iPlaArr[i][j] = -1;

	
	iPlaCount = 0;

	iHandNum = MinHandNum(pDdz->iOnHand);
	isSim = false;
	iUpHandNum = MinHandNum(iUpCards);
	iDownHandNum = MinHandNum(iDownCards);
	ifNeedPass = false;//是否需要考虑PASS
	iLastMaxPoint = CInfo->AnalyzeMainMaxPoint(pDdz->iLastOnTable);
	return 0;
}
void print(int*temp) {

	if (temp[0] == -2)
	{
		cout << "PASS ";
		return;
	}
	int j;
	int a = 0;
	for (int i = 0; temp[i] != -1; i++)
	{
		a++;
	}
	cout << "  ";
	//cout <<"还有"<< a<<"张牌 ";
	for (int pass = a; pass >0; pass--)
	{
		for (int i = 0; i < pass - 1; i++)
		{
			if (temp[i] > temp[i + 1]) {
				j = temp[i];
				temp[i] = temp[i + 1];
				temp[i + 1] = j;
			}
		}
	}

	for (int i = 0; i<a; i++)
	{
		if (temp[i] / 4 == 0)
			cout << 3 << " ";    //3
		else if (temp[i] / 4 == 1)
			cout << 4 << " ";   //4
		else if (temp[i] / 4 == 2)
			cout << 5 << " ";   //5
		else if (temp[i] / 4 == 3)
			cout << 6 << " ";    //6
		else if (temp[i] / 4 == 4)
			cout << 7 << " ";    //7
		else if (temp[i] / 4 == 5)
			cout << 8 << " ";    //8
		else if (temp[i] / 4 == 6)
			cout << 9 << " ";   //9
		else if (temp[i] / 4 == 7)
			cout << 10 << " ";    //10
		else if (temp[i] / 4 == 8)
			cout << " J ";    //J
		else if (temp[i] / 4 == 9)
			cout << " Q ";    //Q
		else if (temp[i] / 4 == 10)
			cout << " K ";   //K
		else if (temp[i] / 4 == 11)
		{
			cout << " A ";    //A
		}
		else if (temp[i] / 4 == 12)
		{
			cout << 2 << " ";   //2
		}
		else if (temp[i] == 52)
		{
			cout << " 小王 ";    //小王
		}
		else if (temp[i] == 53)
		{
			cout << " 大王 ";   //大王
		}
	}
	cout << "  ，，";
};
//帮助AI根据当前牌局生成当前推荐出牌
int RecommendCards::HelpPla(struct fDdz * pDdz, bool IsSim)
{
	//cout << "split card result" << endl;
	
	/*int **result= pSplitCards->startSplitCard(pDdz->iOnHand);
	//cout << " Rocket ";
	for (int j = 0; j<8; j++)
	{
		if (j == 0)cout << " Rocket ";
		if (j == 1)cout << " Bomb ";
		if (j == 2)cout << " Three ";
		if (j == 3)cout << " LinkThree ";
		if (j == 4)cout << " LinkSingle ";
		if (j == 5)cout << " LinkDouble ";
		if (j == 6)cout << " Double ";
		if (j == 7)cout << " Single ";
		for (int i = 0; result[j][i] != -1; i++)
		{
			if (result[j][i] != -2)
			{
				cout << result[j][i] / 4 + 3;
			}
			else
			{
				cout << " , ";
			}
		}
		cout << endl<<endl;
	}*/
	
	/*print(result[0]);
	cout << endl << " Bomb ";
	print(result[1]);
	cout << endl << " Three ";
	print(result[2]);
	cout << endl << " LinkThree ";
	print(result[3]);
	cout << endl << " LinkSingle ";
	print(result[4]);
	cout << endl << " LinkDouble ";
	print(result[5]);
	cout << endl << " Double ";
	print(result[6]);
	cout << endl << " Single ";
	print(result[7]);
	*/

	int iPass[3] = { -2, -1, -1 };
	bool isPassed = false;
	InitPlaGenerate(pDdz);
	isSim = IsSim;
	
	int playerType = GetPlayerType(pDdz);


	//是否前面两家pass
	if (IfPassTwice(pDdz))
	{
		//若是，需要重新出牌
		//必须有牌可出
		Help0Pass(pDdz, playerType);
	}
	else
	{
		int iTypeCount = pDdz->iLastTypeCount;

		if (301 == iTypeCount)//单张
		{
			if (IsSim)
				Help3SingleSim(pDdz);
			else
				Help3Single(pDdz);
		}
		else if (402 == iTypeCount)
		{
			if (IsSim)
				Help4DoubleSim(pDdz);
			else
				Help4Double(pDdz);
		}
		else if (503 == iTypeCount)
			Help5Three(pDdz);
		else if (604 == iTypeCount)
		{
			if (isSim)
				Help6ThreeOneSim(pDdz);
			else
				Help6ThreeOne(pDdz);
		}
		else if (605 == iTypeCount)
		{
			if (isSim)
				Help6ThreeDoubleSim(pDdz);
			else
				Help6ThreeDouble(pDdz);
		}
		else if (iTypeCount > 700 && iTypeCount < 800)
			Help7LinkSingle(pDdz);
		else if (iTypeCount > 800 && iTypeCount < 900)
			Help8LinkDouble(pDdz);
		else if (iTypeCount > 900 && iTypeCount < 1000)
			Help9LinkThree(pDdz);
		else if (iTypeCount > 1000 && iTypeCount < 1100)
		{
			if ((iTypeCount - 1000) % 4 == 0)
				Help10LinkThreeSingle(pDdz);
			else
				Help10LinkThreeDouble(pDdz);
		}
		else if (1106 == iTypeCount)
			Help11FourSingle(pDdz);
		else if (1108 == iTypeCount)
			Help11FourDouble(pDdz);


		//如果无牌可出，出炸弹
		if (iPlaCount == 0)
		{
			if (iHandNum <= 9)//OriginStrategy手牌小于等于9出炸弹
				Help2Bomb(pDdz);
		}
			
		//如果需要PASS且没有节点，加上
		if (ifNeedPass&&!isPassed)
		{
			if (pDdz->iPlaArr[iPlaCount][0] != -2)
			{
				AddInPlaArr(pDdz, iPass);
			}
		}
		if (iPlaCount == 0)
		{
			if (pDdz->iPlaArr[iPlaCount][0] != -2)
			{
				AddInPlaArr(pDdz, iPass);
			}
			isPassed = true;
		}

	}


	return iPlaCount;
}

RecommendCards::RecommendCards() : CInfo(CardsInfo::Instance())
{
	pSplitCards = new SplitCard();
	pMoldel = new PokerModelAnalysis();
}


RecommendCards::~RecommendCards()
{
	delete(pSplitCards);
	delete(pMoldel);
}


RecommendCards* RecommendCards::Instance()
{
	static RecommendCards instance;

	return &instance;
}

//重新出牌，从iOnHand中查询牌型到iPlaArr
//最后修订者:成文杰,最后修订时间:2017.7.13
//修改：首出时只出一种牌型，非首出时给出所有牌型可能性
//修改：防止拆牌出错，临时添加，保证有牌可出
int RecommendCards::Help0Pass(fDdz * pDdz, int playerType)
{


	//DEBUG:如果剩牌中只有四带二的组合，出牌会只出对牌
	//重新出牌应该没有优先级，由胜率决定
	//出牌的时候，首先出包含最小牌的组合

	////判断是否首出//
	int cardsCount = CInfo->CountCards(pDdz->iOnHand);


	//如果首出
	if ((cardsCount == 17 && playerType != 1) || cardsCount == 20)
	{
		
		if (isSim)
			Help3SingleSim(pDdz);

		if (!isSim)
			Help3Single(pDdz);

		if (isSim)
			Help4DoubleSim(pDdz);

		if (!isSim)
			Help4Double(pDdz);

		//Help5Three(pDdz);

		Help6ThreeOne(pDdz);
		Help6ThreeDouble(pDdz);

		if (isSim)
			Help6ThreeOneSim(pDdz);

		if (!isSim)
			Help6ThreeOne(pDdz);

		if (isSim)
			Help6ThreeDoubleSim(pDdz);

		if (!isSim)
			Help6ThreeDouble(pDdz);

		Help7LinkSingle(pDdz);

		Help8LinkDouble(pDdz);

		//Help9LinkThree(pDdz);

		Help10LinkThreeDouble(pDdz);

		Help10LinkThreeSingle(pDdz);

	}
	else
	{
		//方案2:出N种牌型，直到节点数到达上限

		if (isSim)
			Help4DoubleSim(pDdz);
		else
			Help4Double(pDdz);

		Help5Three(pDdz);

		if (isSim)
			Help6ThreeOneSim(pDdz);
		else
			Help6ThreeOne(pDdz);

		if (isSim)
			Help6ThreeDoubleSim(pDdz);
		else
			Help6ThreeDouble(pDdz);

		Help7LinkSingle(pDdz);//通过
		Help8LinkDouble(pDdz);//通过，但应该加上双顺模拟出牌语句使随机更随机
		Help9LinkThree(pDdz);//通过
		Help10LinkThreeSingle(pDdz);//通过
		Help10LinkThreeDouble(pDdz);//通过
		Help11FourSingle(pDdz);
		Help11FourDouble(pDdz);

		//地主下家，如果地主手上不是单牌
		if (playerType == 3)
		{
			if (isSim)
				Help3SingleSim(pDdz);
			else
				Help3Single(pDdz);
		}

		//如果地主出牌
		if (playerType == 1)
		{
			if (pDdz->iUpPlayerCards != 1 && pDdz->iDownPlayerCards != 1)
			{
				//如果其他玩家手上不是单牌，先出单牌
				if (isSim)
					Help3SingleSim(pDdz);
				else
					Help3Single(pDdz);
			}
		}
		//如果是地主上家出牌,如果地主手上不是单牌,先出单牌
		if (playerType == 2)
		{
			if (pDdz->iDownPlayerCards > 1)
			{
				//如果地主手上不是单牌，先出单牌
				if (isSim)
					Help3SingleSim(pDdz);
				else
					Help3Single(pDdz);
			}
		}

		if (iPlaCount == 0)
		{
			if (playerType == 2 || playerType == 1)
			{
				Help3Single(pDdz);
			}
		}
		if (iPlaCount == 0 && cardsCount==4)
			Help2Bomb(pDdz);

		if (iPlaCount == 0&&cardsCount==2)
			Help1Rocket(pDdz);
	}
	//防止拆牌出错，临时添加，保证有牌可出
	if (iPlaCount == 0)
	{
		//Help3SingleSim(pDdz);
		int iTmpCards[50];
		int t = 0;
		for (int i = 0; pDdz->iOnHand[i] != -1; i++)
		{
			if (pDdz->iOnHand[i] < 48)
			{
				iTmpCards[t] = pDdz->iOnHand[i];
				t++;
				iTmpCards[t] = -2;
				t++;
			}
		}
		iTmpCards[t] = -1;
		AddInPlaArr(pDdz, iTmpCards);
	}

	return iPlaCount;
}
//H00-END

//H00-END

//H01-START从iOnHand中查询组合火箭牌型到iPlaArr
//最后修订者:范晓梦，最后修订时间:2016-07-26
int RecommendCards::Help1Rocket(struct fDdz * pDdz)
{
	int *iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 0);
	if (iCards[0]>-1)
		AddInPlaArr(pDdz, iCards);
	return iPlaCount;
}
//H01-END

//H02-START推荐出牌应对炸弹
//最后修订者:范晓梦，最后修订时间:2016-07-26
int RecommendCards::Help2Bomb(struct fDdz * pDdz)
{
	int Cards = CInfo->CountCards(pDdz->iOnHand);
	if (!isSim&&Cards >= 17)
		return 0;
	int *iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 1);
	int iSave[21];
	memset(iSave, -1, sizeof(iSave));
	if (GetLegalCards(pDdz, iCards, iSave))
	{
		AddInPlaArr(pDdz, iSave);
		return iPlaCount;
	}
	//如果是跟牌
	if (!IfPassTwice(pDdz))
	{
		ifNeedPass = true;
		Help1Rocket(pDdz);
	}
	return 0;

}
//H02-END


// - START从iOnHand中查询单张牌型到iPlaArr
//最后修订者:范晓梦,最后修订时间:16-08-11
//修订内容及目的:完善判断
//解决单王不出牌的情况
//优化单牌出牌策略
//最新修改:解决首牌出王和2的情况
int RecommendCards::Help3Single(struct fDdz * pDdz)
{
	//cout <<"Help3Single" << endl;
	//获取单牌牌型组合

	int iCopyOnHand[21];

	int iCount = iPlaCount;//出牌解
	//拆牌函数根本不会把王放进去，但为了代码强壮还是加入了判断语句
	int theLastDan = -1;
	int iTmpCards[60];
	int t = 0;//iTmpCards下标,或者各种临时变量
	int er[5] = { -1, -1, -1, -1, -1 };
	int dan2 = -1;
	int j = 0;
	int i = 0;
	int iHandCards = CInfo->CountCards(pDdz->iOnHand);
	int iTeamCards = -1;//队友手牌数
	bool ifPassTwice = IfPassTwice(pDdz);
	///如果只剩一张牌，且合法，直接出//
	if (pDdz->iOnHand[0] != -1 && pDdz->iOnHand[1] == -1)
	{
		iTmpCards[0] = pDdz->iOnHand[0];
		iTmpCards[1] = -2;
		iTmpCards[2] = -1;
		if (GetLegalCards(pDdz, iTmpCards))
			return iPlaCount-iCount;
	}
	/////////////////////////////////////
	//memset(iCopyOnHand, -1, sizeof(iCopyOnHand));
	bool Condition = AnalyzeCondition(pDdz);
	if (!Condition)
	{
		Help3SingleSim(pDdz);
		return iPlaCount - iCount;
	}

	memcpy(iCopyOnHand,pDdz->iOnHand,sizeof(iCopyOnHand));
	//排除炸弹，并记录炸弹点数
	int BombPoint[4] = { -1, -1, -1, -1 };
	int *iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 1);
	t = 0;
	for (i = 0; iCards[i] != -1; i++)
	{
		if (iCards[i] != -2)
		{
			DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
		}
		else
		{
			BombPoint[t] = iCards[i - 1] / 4;
			t++;
		}
	}
	//排除火箭
	for (i = 0; pDdz->iOnHand[i] != -1; i++);
	if (pDdz->iOnHand[i - 1] == 53 && pDdz->iOnHand[i - 2] == 52)
	{
		DeleteElementInCards(iCopyOnHand, 21, 53);
		DeleteElementInCards(iCopyOnHand, 21, 52);
	}
	/////////////////

	//排除2和王并登记2
	t = 0;
	for (i = 0; pDdz->iOnHand[i] != -1; i++)
	{
			if (pDdz->iOnHand[i] >=48)
			{
				DeleteElementInCards(iCopyOnHand, 21, pDdz->iOnHand[i]);
			}
			//记录2
			if (pDdz->iOnHand[i] >= 48 && pDdz->iOnHand[i] < 52)
			{
				t++;
				er[t] = pDdz->iOnHand[i];
			}
	}
	er[0] = t;//er数组第一个存放二的个数，后面四个存放每个二的编码


	int *iOnHand = iCopyOnHand;
	///查找排除2和王和炸的孤立合法单牌(测试通过)////
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 7);
	if (iCards[0] > -1)
	{
		i = CInfo->CountCards(pDdz->iOnHand);
		//首出，出最小牌
		if (i >= 17 && pDdz->iLastMainPoint<0)
		{
			iCards[2] = -1;
		}
		if (GetLegalCards(pDdz, iCards))
		{
			return iPlaCount - iCount;
		}
	}

	//如果没有合法单牌，也不是只剩一张牌
	//判断是出牌还是跟牌
	if (ifPassTwice)
	{
		//如果出牌

		return iPlaCount - iCount;
	}
	else
	{

		//PASS/////////////////////////
		////如果有1张以上的非炸弹2，拆2///
		if (er[0]>1 && er[0]<4)
		{
			iTmpCards[0] = er[1];
			iTmpCards[1] = -2;
			iTmpCards[2] = -1;
			if (GetLegalCards(pDdz, iTmpCards))
			{
				return iPlaCount - iCount;
			}
		}
		/////////////////////////////////


		//PASS//////////////////////////////
		//拆不在双顺组合里的对子牌,拆出单牌并保存到iSave里
		//双顺最大点用于检查对子是否是双顺内的组合

		//找双顺
		iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 5);
		//最多有三个双顺
		int shunPoint[3] = { -1, -1, -1 };//数据为对子数*100+最大点数
		int ShunNum = 0;
		j = 0;
		for (t = 0; iCards[t] != -1; t++)
		{
			if (iCards[0] == -2)
				break;
			if (iCards[t] == -2)
			{
				shunPoint[ShunNum] = (j / 2) * 100 + iCards[t - 1] / 4;
				ShunNum++;

				j = 0;
			}
			else
				j++;
		}
		int nodesNum = 0;
		int danIn2shun[12];
		iCards = pSplitCards->startSplitCard(iCopyOnHand, 6);
		//将对子改成单牌牌型
		t = 0;
		bool isIn = false;
		for (i = 0; iCards[i] != -1; i++)
		{
			isIn = false;
			if (iCards[0] <0)
				break;
			if (iCards[i] == -2)
			{
				int point = iCards[i - 1] / 4;
				//如果对子在某个双顺组合里面，不拆
				for (j = 0; j < ShunNum; j++)
				{
					int maxPoint = shunPoint[j] % 100;
					int minPoint = maxPoint - shunPoint[j] / 100 + 1;
					if (point <= maxPoint&&point >= minPoint)
					{
						danIn2shun[t] = iCards[i - 1];
						t++;
						isIn = true;
						break;
					}
				}
				if (!isIn)
				{
					iTmpCards[0] = iCards[i - 1];
					iTmpCards[1] = -2;
					iTmpCards[2] = -1;
					if (GetLegalCards(pDdz, iTmpCards))
					{
						nodesNum++;
					}
				}
			}
		}

		danIn2shun[t] = -1;
		if (nodesNum> 0)
		{
			return iPlaCount - iCount;
		}
		////////////////////////////////

		///////PASS//////////////
		////拆六连以上的单顺顶张跟之////
		//目前得到的情况是分牌时得到的第一个数组一定是最大点数组合
		nodesNum = 0;
		int DanShunNum;
		int MaxNum;
		int DanInShun[4] = { -1 };
		//找到六连及以上最大单顺，存到iTmpCards里
		for (i = 5; i < 13; i++)
		{
			//查询该长度的顺子
			DanShunNum = AnalyzeSingleLink(i, -1,pDdz->iOnHand, iTmpCards);
			if (DanShunNum != i)
				break;
			else
			{
				MaxNum = DanShunNum;
			}
		}
		if (MaxNum >= 6)
		{
			AnalyzeSingleLink(MaxNum, -1, pDdz->iOnHand, iTmpCards);
			j = 0;
			//得到最大长度顺子
			//检查顶张是否是炸弹点数,如果是，往前一级检查
			//检查顶张是否是双顺点数，如果是，往前一级检查
			for (i = MaxNum; i >=6; i--)
			{
				isIn = false;
				for (t = 0; BombPoint[t] != -1; t++)
				{
					if (iTmpCards[i - 1] / 4 == BombPoint[t])
					{
						isIn = true;
						break;
					}
				}
				for (t = 0; BombPoint[t] != -1; t++)
				{
					if (iTmpCards[i - 1] / 4 == danIn2shun[t]/4)
					{
						isIn = true;
						break;
					}
				}
				if (!isIn)
				{
					DanInShun[j] = iTmpCards[i - 1];
					j++;
					DanInShun[j] = -2;
					j++;
					DanInShun[j] = -1;
					if (GetLegalCards(pDdz, DanInShun))
					{
						nodesNum++;
					}
					break;
				}
			}
		}
		if (nodesNum>0)
		{
			return iPlaCount - iCount;
		}
		//否则拆三条跟之->DEBUG:但没有关心是三条是否在三顺里
		//即三顺和三条是同拆的
		nodesNum = 0;
		memset(iTmpCards, -1, sizeof(iTmpCards));
		iCards = pSplitCards->startSplitCard(iCopyOnHand, 2);
		t = 0;
		for (int i = 0; iCards[i] != -1; i++)
		{
			if (iCards[0] <0)
				break;
			if (iCards[i] == -2)
			{
				iTmpCards[t] = iCards[i - 1];
				t++;
				iTmpCards[t] = -2;
				t++;
			}
		}
		iTmpCards[t] = -1;
		if (GetLegalCards(pDdz, iTmpCards))
		{
			return iPlaCount-iCount;
		}

		/////PASS////////////
		////////////////////////////////
		///否则拆单顺///
		memset(iTmpCards, -1, sizeof(iTmpCards));
			iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 4);
			t = 0;

			for (int i = 0; iCards[i] != -1; i++)
			{
				if (iCards[i] != -2)
				{
					isIn = false;
					for (j = 0; BombPoint[j] > -1; j++)
					{
						if (iCards[i] / 4 == BombPoint[j])
						{
							isIn = true;
							break;
						}
					}
					if (isIn)
						continue;
					//排除双顺
					for (j = 0; danIn2shun[j] > -1; j++)
					{
						if (iCards[i] / 4 == danIn2shun[j] / 4)
						{
							isIn = true;
							break;
						}
					}
					if (isIn)
						continue;
					if (!isIn)
					{
						iTmpCards[t] = iCards[i];
						t++;
						iTmpCards[t] = -2;
						t++;
					}
				}
			}
			iTmpCards[t] = -1;
			if (GetLegalCards(pDdz, iTmpCards))
			{
				return iPlaCount - iCount;
			}
	    /////////////////

		////否则出对子里的合法双顺牌////
		//得到在双顺中的单牌
		//以下跟牌需要判断跟队友牌还是跟敌人牌
			memset(iTmpCards,-1,sizeof(iTmpCards));
			if (danIn2shun[0] != -1)
		{
			t = 0;
			for (int i = 0; danIn2shun[i] != -1; i++)
			{
				iTmpCards[t] = danIn2shun[i];
				t++;
				iTmpCards[t] = -2;
				t++;
			}
			iTmpCards[t] = -1;
			if (GetLegalCards(pDdz, iTmpCards))
			{
				return iPlaCount - iCount;
			}
		}
		//找不到任何可出的牌
		if (iPlaCount - iCount == 0)
		{
			//////////////////////////////////////////
			//如果有一张单2，如果局势不利或手数较小，将单2放入计算
			i = CInfo->CountCards(pDdz->iOnHand);
			if (er[0] == 1)
			{
				bool Condition = AnalyzeCondition(pDdz);
				if (!Condition || iHandNum <= 3)
				{
					iTmpCards[0] = er[1];
					iTmpCards[1] = -2;
					iTmpCards[2] = -1;
					if (GetLegalCards(pDdz, iTmpCards))
						return iPlaCount;
				}
			}
			/////////////////////////////////

			////找单牌王///
			iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 0);
			//如果没有火箭,即王不在火箭里，则有王只能是单牌王
			if (iCards[0] < 0)
			{
				bool condition = AnalyzeCondition(pDdz);
				for (t = 0; pDdz->iOnHand[t] != -1; t++);
				if (pDdz->iOnHand[t - 1] >= 52)
				{
					if (!condition || iHandNum <= 3)
					{
						iTmpCards[0] = pDdz->iOnHand[t - 1];
						iTmpCards[1] = -2;
						iTmpCards[2] = -1;
						if (GetLegalCards(pDdz, iTmpCards))
						{
							return iPlaCount - iCount;
						}
					}
				}
			}
			//有火箭，考虑出火箭或出小王,或PASS
			else
			{
				//出小王
				iTmpCards[0] = 52;
				iTmpCards[1] = -2;
				iTmpCards[2] = -1;
				GetLegalCards(pDdz, iTmpCards);
				//出火箭或炸弹
				Help2Bomb(pDdz);
				return iPlaCount - iCount;
			}
		}

	}
	return 0;
}
//H03-END

//查询所有不同点数单张牌型到iOnHand里面
int RecommendCards::Help3SingleSim(fDdz* pDdz)
{
	int i;
	int iTmpCards[60] = { -1 };

	int count = iPlaCount;
	int point = -1; 
	int t = 0;
	for (i = 0; pDdz->iOnHand[i] >= 0; i++)
	{
		if (t > 0)
		{
			if (pDdz->iOnHand[i] / 4 == point)
				continue;
		}
		iTmpCards[t] = pDdz->iOnHand[i];
		point = iTmpCards[t] / 4;
		t++;
		iTmpCards[t] = -2;
		t++;
	}
	iTmpCards[t] = -1;
	if (GetLegalCards(pDdz, iTmpCards))
	{
		return iPlaCount - count;
	}
	return 0;
}

///////PASS//////////
//H04-START推荐出牌应对对牌
//最后修订者:范晓梦，最后修订时间:2016-07-26
int RecommendCards::Help4Double(struct fDdz * pDdz)
{
	//cout << "Help4Double" << endl;;
	//TODO:如果所有三条数量<=对子+单牌数量总和-2时，出对子，否则，出三带二
	//拆不在双顺组合里的对子牌
	//双顺最大点用于检查对子是否是双顺内的组合
	int i, j, t;
	int iCopyOnHand[21];
	int iCount = iPlaCount;
	int * iCards;
	int iTmpCards[50];
	//将手中的牌复制进iCopyOnHand
	memcpy(iCopyOnHand, pDdz->iOnHand, sizeof(pDdz->iOnHand));
	//是否可以直接出
	i = CInfo->CountCards(pDdz->iOnHand);
	if (i == 2)
	{
		iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 6);
		if (iCards[0] >= 0)
		{
			if (GetLegalCards(pDdz, iCards))
				return iPlaCount - iCount;
		}
	}
	bool  Condition = AnalyzeCondition(pDdz);
	if (!Condition)
	{
		Help4DoubleSim(pDdz);
		return iPlaCount - iCount;
	}
	//排除2和王
	int condition = AnalyzeCondition(pDdz);
	if (pDdz->iLastMainPoint<11)
	{
		for (i = 48; i <= 53; i++)
		{
			DeleteElementInCards(iCopyOnHand, 21, i);
		}
	}
	//判断炸弹,并排除
	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 1);
	if (iCards[0] > -1)
	{
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] != -2)
				DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
		}
	}
	//找双顺
	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 5);
	//排除在双顺里面的对子
	if (iCards[0] > -1)
	{
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] != -2)
				DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
		}
	}
	//排除在单顺里面的对子
	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 4);
	if (iCards[0] > -1)
	{
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] != -2)
				DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
		}
	}

	//分析对子
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 6);
	if (GetLegalCards(pDdz, iCards))
	{
		return iPlaCount - iCount;
	}
	//如果出牌，退出
	if (pDdz->iLastMainPoint < 0)
	{
		return 0;
	}

	memcpy(iCopyOnHand, pDdz->iOnHand, sizeof(pDdz->iOnHand));
	//根据情况排除2
	if (pDdz->iLastMainPoint<11)
	{
		for (i = 48; i <= 53; i++)
		{
			DeleteElementInCards(iCopyOnHand, 21, i);
		}
	}
	//判断炸弹,并排除
	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 1);
	if (iCards[0] > -1)
	{
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] != -2)
				DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
		}
	}
	////找三顺
	//iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 3);
	////排除在三顺里面的三条
	//if (iCards[0] > -1)
	//{
	//	for (i = 0; iCards[i] != -1; i++)
	//	{
	//		if (iCards[i] != -2)
	//			DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
	//	}
	//}
	//找三条
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 2);
	t = 0;
	if (iCards[0]>-1)
	{
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] == -2)
			{
				iTmpCards[t] = iCards[i - 1];
				t++;
				iTmpCards[t] = iCards[i - 2];
				t++;
				iTmpCards[t] = -2;
				t++;
			}
		}
	}
	iTmpCards[t] = -1;
	if (iTmpCards[0] != -1)
	{
		if (GetLegalCards(pDdz, iTmpCards))
		{
			return iPlaCount - iCount;
		}
	}

	iCards = pSplitCards->startSplitCard(iCopyOnHand, 6);
	if (GetLegalCards(pDdz, iCards))
	{
		return iPlaCount - iCount;
	}
	//如果依然没有牌，检查非炸弹2
	int dui2[4] = { -1, -1, -1, -1 };
	t = 0;
	for (i = 0; pDdz->iOnHand[i] != -1; i++)
	{
		if (pDdz->iOnHand[i] / 4 == 12)
		{
			if (t == 3)
			{
				dui2[0] = -1;
				break;
			}
			dui2[t] = pDdz->iOnHand[i];
			t++;
		}
	}
	i = CInfo->CountCards(pDdz->iOnHand);
	//如果有对2,不能在第一手出
	if (dui2[1] != -1)
	{
		if (pDdz->cDir == pDdz->cLandlord&&i == 20)
		{
			return 0;
		}
		//否则
		dui2[2] = -2;
		dui2[3] = -1;
		if (GetLegalCards(pDdz, dui2))
		{
			return iPlaCount - iCount;
		}
	}

	return iPlaCount - iCount;
}

/////PASS////
//无规则出对子
int RecommendCards::Help4DoubleSim(struct fDdz * pDdz)
{
	int iTmpCards[60] = { -1 };
	int t = 0;
	int i;
	int point = -1;
	int iCount = iPlaCount;

	//寻找对子，放进iTmpCards数组中
	for (i = 0; pDdz->iOnHand[i + 1] >= 0; i++)
	{
		if (pDdz->iOnHand[i] / 4 == pDdz->iOnHand[i + 1] / 4)
		{
			if (pDdz->iOnHand[i] / 4 != point)//防止三条及以上算两次对子
			{
				iTmpCards[t] = pDdz->iOnHand[i];
				point = pDdz->iOnHand[i] / 4;
				t++;
				iTmpCards[t] = pDdz->iOnHand[i + 1];
				t++;
				iTmpCards[t] = -2;
				t++;

			}
		}
	}
	iTmpCards[t] = -1;
	if (GetLegalCards(pDdz, iTmpCards))
	{
		return iPlaCount - iCount;
	}
	return iPlaCount - iCount;
}
//H05-START推荐出牌应对三条
//最后修订者:范晓梦，最后修订时间:2016-07-26
int RecommendCards::Help5Three(struct fDdz * pDdz)
{
	int iCopyOnHand[21];
	int i;
	memcpy(iCopyOnHand, pDdz->iOnHand, sizeof(pDdz->iOnHand));

	//判断炸弹,并排除
	int *iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 1);
	if (iCards[0] > -1)
	{
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] != -2)
				DeleteElementInCards(iCopyOnHand,21, iCards[i]);
		}
	}


	//去掉2
	if (pDdz->iLastMainPoint < 11)
	{
		for (i = 48; i < 52; i++)
		{
			DeleteElementInCards(iCopyOnHand, 21, i);
		}
	}

	//去掉三顺 ///如果是队友出的就去掉？地主出了而队友没出就应该接起？
	
	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 3);
	if (iCards[0] > -1)
	{
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] != -2)
				DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
		}
	}


	iCards = pSplitCards->startSplitCard(iCopyOnHand, 2);
	int iCount = iPlaCount;
	if (iCards[0] > -1)
	{
		if (GetLegalCards(pDdz, iCards))
		{
			return iPlaCount - iCount;
		}
	}

	//如果没有
	if (pDdz->iLastMainPoint < 0)
		return 0;
	else
	{
		memcpy(iCopyOnHand, pDdz->iOnHand, sizeof(pDdz->iOnHand));

		//判断炸弹,并排除
		int *iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 1);
		if (iCards[0] > -1)
		{
			for (i = 0; iCards[i] != -1; i++)
			{
				if (iCards[i] != -2)
					DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
			}
		}

		//去掉2
		if (pDdz->iLastMainPoint < 11)
		{
			for (i = 48; i < 52; i++)
			{
				DeleteElementInCards(iCopyOnHand, 21, i);
			}
		}
		iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 2);
		if (GetLegalCards(pDdz, iCards))
		{
			return iPlaCount - iCount;
		}
	}

	return 0;
}
//H05-END

//SplitCards拆牌要求：拆牌时单牌放入2不放入王
//H0601-START推荐出牌应对三带一单
//最后修订者:范晓梦，最后修订时间:2016-08-10
//优化三带单
int RecommendCards::Help6ThreeOne(struct fDdz * pDdz)
{
	int iTmpCards[60] = { -1 };
	int iCount = iPlaCount;

	//判断有无合法三条,并保存到iSave1里

	int t = 0;
	int i = 0;
	int *iCards;
	t = CInfo->CountCards(pDdz->iOnHand);
	int j;

	//如果可以直接出，则直接出（通过）
	if (t == 4)
	{
		j = 0;
		iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 2);
		if (iCards[0] > -1)
		{
			for (i = 0; iCards[i] != -1; i++)//把三条都赋值给iTempCaards
			{
				if (iCards[i] != -2)
				{
					iTmpCards[j] = iCards[i];
					j++;
				}
			}

			for (i = 0; pDdz->iOnHand[i] != -1; i++)//把另外一张牌找出来
			{
				if (pDdz->iOnHand[i] / 4 != iTmpCards[0] / 4)
				{
					iTmpCards[j] = pDdz->iOnHand[i];
					j++;
					break;
				}
			}
			iTmpCards[j] = -2;
			j++;
			iTmpCards[j] = -1;
			if (GetLegalCards(pDdz, iTmpCards))
			{
				return iPlaCount - iCount;
			}
		}
	}

	///////////////////////
	int iCopyOnHand[21];
	memcpy(iCopyOnHand, pDdz->iOnHand, sizeof(pDdz->iOnHand));
	//判断炸弹,并排除
	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 1);
	//排除炸弹
	for (i = 0; iCards[i] != -1; i++)
	{
		if (iCards[i] != -2)
		{
			DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
		}
	}

	iCards = pSplitCards->startSplitCard(iCopyOnHand, 2);

	//如果没有三条
	if (iCards[0] < 0)
	{
		return 0;
	}

	//如果有三条,保存三条到iTmpCards(通过)
	t = 0;
	int santiaoNum = 0;
	for (i = 0; iCards[i] != -1; i++)
	{
		if (iCards[0] == -2)
			break;
		if (iCards[i] == -2)
		{
			iTmpCards[t] = iCards[i - 3];
			t++;
			iTmpCards[t] = iCards[i - 2];
			t++;
			iTmpCards[t] = iCards[i - 1];
			t++;
			iTmpCards[t] = -2;
			t++;
			santiaoNum++;
		}
	}
	iTmpCards[t] = -1;

	if (santiaoNum)
	{
		int santiao[6];
		int danwang = -1;
		int er[4] = { -1, -1, -1, -1 };
		t = 0;
		j = 0;
		//找手上的单王和非炸弹2并保存起来(通过)
		int iHandCards = CInfo->CountCards(pDdz->iOnHand);
		for (i = iHandCards; i > iHandCards - 7; i--)
		{
			if (52 <= pDdz->iOnHand[i] && 53 >= pDdz->iOnHand[i])
			{
				j++;
				danwang = pDdz->iOnHand[i];

			}
			if (48 <= pDdz->iOnHand[i] && 51 >= pDdz->iOnHand[i])
			{
				er[t] = pDdz->iOnHand[i];
				t++;
			}
		}
		//如果2是炸弹,不保存
		if (er[3] >= 48)
			er[0] = -1;
		if (j > 1)//如果是王炸，不保存
			danwang = -1;
		//查找单牌
		//去掉2和王
		for (i = 48; i <=53; i++)
		{
			DeleteElementInCards(iCopyOnHand, 21,i);
		}
		iCards = pSplitCards->startSplitCard(iCopyOnHand, 7);
		int dan[21] = { -1, -1, -1, -1 };
		//如果有单,
		if (iCards[0] > -1)
		{
				AnalyzeSplitedCards(iCards, dan);
		}
		//如果没单且是出牌，离开
		if (iCards[0] <= -1 && pDdz->iLastMainPoint < 0)
		{
			return 0;
		}
		//如果没单，是跟牌，且三条数大于1，查找其余三条里的全部单牌
		if (dan[0] <= -1 && santiaoNum > 1)
		{
			t = 0;
			for (i = 0; iTmpCards[i] != -1; i++)
			{
				if (iTmpCards[i] == -2)
				{
					dan[t] = iTmpCards[i - 1];
					t++;
				}
			}
			dan[t] = -1;
		}
		//如果依然没单，查找去掉炸弹和2的对牌里面的全部单牌
		if (dan[0] <= -1)
		{
			iCards = pSplitCards->startSplitCard(iCopyOnHand, 6);
			if (iCards[0] >= 0)
			{
				t = 0;
				for (i = 0; iCards[i] != -1; i++)
				{
					if (iCards[i] == -2)
					{
						dan[t] = iCards[i - 1];
						t++;
					}
				}
				dan[t] = -1;
			}
		}
		//如果没有有效单牌
		if (dan[0] <= -1)
		{
			bool condition = AnalyzeCondition(pDdz);
			if (!condition || iHandNum <= 2)
			{
				t = 0;

				if (er[0] != -1)
				{
					dan[t] = er[0];
					t++;
				}
				if (danwang != -1)
				{
					dan[t] = danwang;
					t++;
				}

			}
			dan[t] = -1;
		}
		if (dan[0] == -1)
		{
			return 0;
		}
		//整理合并单牌和三条
		for (i = 0; iTmpCards[i] != -1; i++)
		{
			if (iTmpCards[i] == -2)
			{
				santiao[2] = iTmpCards[i - 1];
				santiao[1] = iTmpCards[i - 2];
				santiao[0] = iTmpCards[i - 3];
				santiao[3] = -1;
				int santiaopoint = CInfo->AnalyzeMainMaxPoint(santiao);
				for (int j = 0; dan[j] != -1; j++)
				{
					if (dan[j] / 4 != santiaopoint)
					{
						santiao[3] = dan[j];
						santiao[4] = -2;
						santiao[5] = -1;
						GetLegalCards(pDdz,santiao);
					}
				}

			}

		}
	}
	return iPlaCount - iCount;
}
//H0601-END

//无规则三带一单
int RecommendCards::Help6ThreeOneSim(struct fDdz * pDdz)
{
	int iCount = iPlaCount;
	int iTmpCards[21] = { -1 };

	int i, iTemp, j;
	int point = -1;
	int danNum = 0;
	int dan[21];
	int santiaoPoint = 0;
	int t = 0;
	//如果有单
	for (i = 0; pDdz->iOnHand[i + 2] >= 0; i++)
	{

		if (pDdz->iOnHand[i] / 4 == pDdz->iOnHand[i + 1] / 4
			&& pDdz->iOnHand[i] / 4 == pDdz->iOnHand[i + 2] / 4)
		{
			iTemp = i;

			for (j = 0; pDdz->iOnHand[j] >= 0; j++)
			{
				t = 0;
				//该单牌不能是三条中的数并且将炸弹排除
				if (j == iTemp || j == iTemp + 1 || j == iTemp + 2 || pDdz->iOnHand[j] / 4 == pDdz->iOnHand[i] / 4)
					continue;
				//该单牌也不能跟上一个同一个三条的单牌点数相同
				if (pDdz->iOnHand[j] / 4 == point)
					continue;
				iTmpCards[t] = pDdz->iOnHand[i];
				t++;
				iTmpCards[t] = pDdz->iOnHand[i + 1];
				t++;
				iTmpCards[t] = pDdz->iOnHand[i + 2];
				t++;
				iTmpCards[t] = pDdz->iOnHand[j];
				point = pDdz->iOnHand[j] / 4;
				t++;
				iTmpCards[t] = -1;
				santiaoPoint = CInfo->AnalyzeMainMaxPoint(iTmpCards);
				iTmpCards[t] = -2;
				t++;
				iTmpCards[t] = -1;
				GetLegalCards(pDdz,iTmpCards);
			}

		}
	}

	return iPlaCount - iCount;
}


//H0602-START推荐出牌应对三带一对
//最后修订者:范晓梦，最后修订时间:2016-08-10
//修改判断有无合法三条由GetLegalCards判断，避免混淆
int RecommendCards::Help6ThreeDouble(struct fDdz * pDdz)
{
	int iTmpCards[60] = { -1 };
	int iCount = iPlaCount;
	int t = 0;
	int i = 0;
	int *iCards;
	t = CInfo->CountCards(pDdz->iOnHand);
	//如果可以直接出，则直接出
	if (t == 5)
	{
		iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 2);
		if (iCards[0] > -1 && iCards[0] / 4 > iLastMaxPoint)
		{
			iTmpCards[0] = iCards[0];
			iTmpCards[1] = iCards[1];
			iTmpCards[2] = iCards[2];
			iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 6);
			if (iCards[0] > -1)
			{
				iTmpCards[3] = iCards[0];
				iTmpCards[4] = iCards[1];
				iTmpCards[5] = -2;
				iTmpCards[6] = -1;

				if (GetLegalCards(pDdz, iTmpCards))
				{
					return iPlaCount - iCount;
				}
			}
		}
	}
	//////////////////////////
	int iCopyOnHand[21];
	memcpy(iCopyOnHand, pDdz->iOnHand, sizeof(pDdz->iOnHand));
	//判断炸弹,并排除
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 1);
	if (iCards[0] > -1)
	{
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] != -2)
				DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
		}
	}
	i = CInfo->CountCards(pDdz->iOnHand);
	if (i==20)
	{
		for (i = 48; i < 52; i++)
		{
			DeleteElementInCards(iCopyOnHand, 21, i);
		}
	}

	/////////////////
	//判断有无三条
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 2);
	int santiaoNum = 0;

	t = 0;
	for (i = 0; iCards[i] != -1; i++)
	{
		if (iCards[0] == -2)
			break;
		if (iCards[i] == -2)
		{
			iTmpCards[t] = iCards[i - 3];
			t++;
			iTmpCards[t] = iCards[i - 2];
			t++;
			iTmpCards[t] = iCards[i - 1];
			t++;
			iTmpCards[t] = -2;
			t++;
			santiaoNum++;
		}
	}
	iTmpCards[t] = -1;

	//一副牌里 三条数*3+对子数*2<21
	//最多36个节点//TODO：后期可以处理节点数
	//如果没有三条，返回
	if (santiaoNum == 0)
	{
		return 0;

	}
	if (santiaoNum)
	{
		bool condition = AnalyzeCondition(pDdz);
		if (condition||iHandNum>2)
		{
			for (i = 48; i < 52; i++)
			{
				DeleteElementInCards(iCopyOnHand, 21, i);
			}
		}
		////包括双顺里面的对子////////
		int santiao[8];
		iCards = pSplitCards->startSplitCard(iCopyOnHand, 6);
		int duizi[60];
		if (iCards[0] <= -1)
		{
			//如果没对子且是出牌，离开
			if (pDdz->iLastMainPoint < 0)
				return 0;
			//否则查找三条里面的对子,并保存在iCard里
			int  t = 0;
			for (int i = 0; iTmpCards[i] != -1; i++)
			{
				if (iTmpCards[i] == -2)
				{
					duizi[t] = iTmpCards[i - 2];
					++t;
					duizi[t] = iTmpCards[i - 1];
					++t;
					duizi[t] = -2;
					++t;
				}
			}
			duizi[t] = -1;
			iCards = duizi;
		}
		//如果仍然没对子
		if (iCards[0] == -1)
			return 0;
		condition = AnalyzeCondition(pDdz);
		for (i = 0; iTmpCards[i] != -1; i++)
		{
			int santiaoPoint;
			if (iTmpCards[i] == -2)
			{
				santiao[0] = iTmpCards[i - 3];
				santiao[1] = iTmpCards[i - 2];
				santiao[2] = iTmpCards[i - 1];
				santiao[3] = -1;
				for (int j = 0; iCards[j] != -1; j++)
				{
					if (iCards[j] == -2)
					{
						santiaoPoint = CInfo->AnalyzeMainMaxPoint(santiao);
						if (iCards[j - 1] / 4 != santiaoPoint)
						{
							santiao[3] = iCards[j - 2];
							santiao[4] = iCards[j - 1];
							santiao[5] = -2;
							santiao[6] = -1;
							if (GetLegalCards(pDdz, santiao))
							{
								if (condition)
									return iPlaCount - iCount;
							}
						}
					}
				}
			}
		}
	}
	return iPlaCount - iCount;
}
//H0602-END

//无规则三带二
int RecommendCards::Help6ThreeDoubleSim(struct fDdz * pDdz)
{
	int num = 0;
	int iCount = iPlaCount;
	int i, iTemp, j;
	int iTmpCards[500] = { -1 };
	int point = -1;
	int t = 0;
	int santiaoPoint = -1;
	for (i = 0; pDdz->iOnHand[i + 2] >= 0; i++)
	{
		if (t + 5 + 1 > 500)
			break;
		if (num + 1<kPlaMax
			&& pDdz->iOnHand[i] / 4 > iLastMaxPoint
			&& pDdz->iOnHand[i] / 4 == pDdz->iOnHand[i + 1] / 4
			&& pDdz->iOnHand[i] / 4 == pDdz->iOnHand[i + 2] / 4)
		{
			iTemp = i;
			for (j = 0; pDdz->iOnHand[j + 1] >= 0; j++)
			{
				t = 0;
				//将对子在三条中的数排除
				if (j >= iTemp - 1 && j <= iTemp + 2)
					continue;
				//该对子也不能跟上一个同一个三条的对子点数相同
				if (pDdz->iOnHand[j] / 4 == point)
					continue;
				if (pDdz->iOnHand[j]>=52)
					continue;
				if (pDdz->iOnHand[j] / 4 == pDdz->iOnHand[j + 1] / 4)
				{
					iTmpCards[t] = pDdz->iOnHand[i];
					t++;
					iTmpCards[t] = pDdz->iOnHand[i + 1];
					t++;
					iTmpCards[t] = pDdz->iOnHand[i + 2];
					t++;
					iTmpCards[t] = pDdz->iOnHand[j];
					point = pDdz->iOnHand[j] / 4;
					t++;
					iTmpCards[t] = pDdz->iOnHand[j + 1];
					t++;
					iTmpCards[t] = -1;
					santiaoPoint = CInfo->AnalyzeMainMaxPoint(iTmpCards);
					iTmpCards[t] = -2;
					t++;
					iTmpCards[t] = -1;

					if (GetLegalCards(pDdz, iTmpCards))
						num++;

				}
			}
		}
	}
	return iPlaCount - iCount;
}
//H07-START推荐出牌应对单顺
//最后修订者:范晓梦，最后修订时间:2016-07-26
int RecommendCards::Help7LinkSingle(struct fDdz * pDdz)
{
	int i, j, k, iLength, iTemp,  t;
	int iCopyCards[21];
	int iCount = iPlaCount;
	CInfo->SortById(pDdz->iOnHand);
	int shunziCount = 0;
	int *iCards;

	//排除炸弹
	//如果首出，去掉炸弹(通过)
	memcpy(iCopyCards, pDdz->iOnHand, sizeof(pDdz->iOnHand));
	t = CInfo->CountCards(pDdz->iOnHand);
	iCards = pSplitCards->startSplitCard(iCopyCards, 1);



	for (i = 0; iCards[i] != -1; i++)
	{
		if (iCards[i] != -2)
		{
			DeleteElementInCards(iCopyCards, 21, iCards[i]);
		}
	}


	//去掉王和2
	for (i = 48; i <= 53; i++)
	{
		DeleteElementInCards(iCopyCards, 21, i);
	}
	//去除重牌并从小到大排序
	///CInfo->GetUnReaptedCards(iCopyCards);

	///////////////////////////////////

	int iTmpShunzi[21];
	memset(iTmpShunzi, -1, sizeof(iTmpShunzi));
	int NodesCount = 0;
	int MaxNum;
	int iDanShunNum = 0;
	int iDanShunNoBombNum = 0;
	int MaxNumNoBomb = 0;
	//由于炸弹可能影响是否能拆出单顺
	//给出两个节点：一个是去掉炸弹的最大单牌，一个是没去掉炸弹的

	//如果是出牌
	if (pDdz->iLastMainPoint < 0)
	{

		for (iLength = 5; iLength <= 13; iLength++)
		{
			//查询该长度的顺子
			iDanShunNoBombNum = AnalyzeSingleLink(iLength, -1, iCopyCards, iTmpShunzi);
			iDanShunNum = AnalyzeSingleLink(iLength, -1, pDdz->iOnHand, iTmpShunzi);
			//非模拟,且查询长度失败出前一个记录最长最小顺子
			if (!isSim)
			{
				if (iDanShunNum == iLength)
				{
					MaxNum = iLength;
				}
				if (iDanShunNoBombNum == iLength)
				{
					MaxNumNoBomb = iLength;
				}
			}
			else
			{
				if (iDanShunNum == iLength)
				{
					GetLegalCards(pDdz,iTmpShunzi);
				}
			}
		}
		if (MaxNumNoBomb >= 5)
		{
			memset(iTmpShunzi, -1, sizeof(iTmpShunzi));
			iDanShunNoBombNum = AnalyzeSingleLink(MaxNumNoBomb, -1, iCopyCards, iTmpShunzi);
			if (iDanShunNoBombNum == MaxNumNoBomb)
			GetLegalCards(pDdz, iTmpShunzi);
		}

		i = CInfo->CountCards(pDdz->iOnHand);
		//如果是首出或者长度与无炸弹时一致，则不用出
		if (MaxNumNoBomb != MaxNum&&MaxNum >= 5 && i != 20)
		{
			memset(iTmpShunzi, -1, sizeof(iTmpShunzi));
			iDanShunNum = AnalyzeSingleLink(MaxNum, -1, pDdz->iOnHand, iTmpShunzi);
			if (iDanShunNum==MaxNum)
			GetLegalCards(pDdz, iTmpShunzi);
		}
	}
	//如果跟牌，尽最大可能出跟牌节点
	else
	{
		iLength = pDdz->iLastTypeCount - 700;
		int point = pDdz->iLastMainPoint;
		while(point + iLength - 1 <= 11)
		{
			iDanShunNum=AnalyzeSingleLink(iLength, point, pDdz->iOnHand, iTmpShunzi);
			point=point+1;
			if (iDanShunNum == iLength)
				GetLegalCards(pDdz, iTmpShunzi);
		}
	}
	return iPlaCount - iCount;
}
//H07-END

//H08-START推荐出牌应对双顺
//最后修订者:范晓梦，最后修订时间:2016-08-11
//修订出牌时出双顺的情况
int RecommendCards::Help8LinkDouble(struct fDdz * pDdz)
{

	int i, j, k, iLength;
	int iCopyOnHand[21];
	int n = 1;
	int iFlag = 0;
	int iCardsFlag = 0;
	int iCard = 0;
	int iTmpShuangshun[21];
	int iTmpCards[21];
	int iTypeCount;
	int iCount = iPlaCount;
	//int iSave[12 * 12];
	memset(iTmpShuangshun, -1, sizeof(iTmpShuangshun));
	CInfo->SortById(pDdz->iOnHand);
	//跟牌时
	for (i = 0; i < 21; i++)
		iCopyOnHand[i] = pDdz->iOnHand[i];
	//去掉王和2
	for (i = 0; iCopyOnHand[i] >= 0; i++)
		if (iCopyOnHand[i] / 4 >= 12)
			iCopyOnHand[i] = -1;
	//排除炸弹
	//如果地主首出，去掉炸弹(通过)
	memcpy(iCopyOnHand, pDdz->iOnHand, sizeof(pDdz->iOnHand));
	k = CInfo->CountCards(pDdz->iOnHand);
	if (k == 20&&!isSim)
	{
		int *iCards = pSplitCards->startSplitCard(iCopyOnHand, 1);
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] != -2)
			{
				DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
			}
		}
	}
	auto GetShuangShun = [&](int *iCards, int iLength,int iLastMainPoint)->int
	{
		int shuangshunNum = 0;
		int iCopyCards[21];
		for (i = 0; iCards[i] != -1; i++)
		{
			iCopyCards[i] = iCards[i];
		}
		iCopyCards[i] = -1;
		for (i = 0; i < CInfo->CountCards(iCopyCards); i++)
		{
			if (iCopyCards[i] / 4 != iCopyCards[i + 1] / 4)
			{
				for (j = i; j < CInfo->CountCards(iCopyCards); j++)
					iCopyCards[j] = iCopyCards[j + 1];
				i--;
			}

		}//四连变三连 三连变对子 对子变单排 单排变没有
		for (i = 0; i < CInfo->CountCards(iCopyCards); i++)
		{
			if (iCopyCards[i] / 4 == iCopyCards[i + 1] / 4)
			{
				for (j = i; j < CInfo->CountCards(iCopyCards); j++)
					iCopyCards[j] = iCopyCards[j + 1];
				i--;
			}
		}
		//三连和对子变成单牌
		//以上将数组iCopyCards[21]处理成能组成对子的单排数组且最大为A

		for (i = 0; iCopyCards[i] >= 0; i++)
		{
			if (iCopyCards[i] / 4 == iCopyCards[i + 1] / 4 - 1)
				n++;
			else
			{
				n = 1;
				i = iFlag;
				iFlag++;
			}
			if (n == iLength)
			{
				//int lastMinPoint = CInfo->AnalyzeMainPoint(pDdz->iLastOnTable);
				if (shuangshunNum + 1 < kPlaMax
					&& iCopyCards[i + 2 - iLength] / 4 > iLastMainPoint)
				{
					for (j = i + 2 - iLength; j <= i + 1; j++)
					{
						for (k = 0; k < CInfo->CountCards(pDdz->iOnHand); k++)
						{
							if (iCopyCards[j] / 4 == pDdz->iOnHand[k] / 4 && iCardsFlag < 2)
							{
								iTmpShuangshun[iCard] = pDdz->iOnHand[k];
								iCard++;
								iCardsFlag++;
							}
						}
						iCardsFlag = 0;
					}
					iTmpShuangshun[iCard] = -2;
					iTmpShuangshun[iCard + 1] = -1;
					if (GetLegalCards(pDdz, iTmpShuangshun))
						shuangshunNum++;
					iCard = 0;
				}
				n = 1;
				i = iFlag;
				iFlag++;
			}
		}
		return shuangshunNum;
	};
	if (pDdz->iLastTypeCount > 0)
	{
		iLength = (pDdz->iLastTypeCount - 800) / 2;
		GetShuangShun(iCopyOnHand,iLength,pDdz->iLastMainPoint);
	}
	else
	{
		for (iLength = 3; iLength <=11; iLength++)
		{
			GetShuangShun(iCopyOnHand, iLength,pDdz->iLastMainPoint);
		}
	}
	return iPlaCount-iCount;
}
//H08-END

//H09-START推荐出牌应对三顺
//最后修订者:范晓梦，最后修订时间:2016-08-11
//修正应对出牌出三顺的情况
int RecommendCards::Help9LinkThree(struct fDdz * pDdz)
{
	int iTmpSanshun[21];
	memset(iTmpSanshun, -1, sizeof(iTmpSanshun));
	int sanshunNum = 0;
	int i, iTemp, j, k, iLength = pDdz->iLastTypeCount - 900;
	int iTempArr[12][5];
	if (CInfo->CountCards(pDdz->iOnHand) < 6)
		return 0;
	int iTypeCount = 0;
	int iCount = iPlaCount;
	if (pDdz->iLastMainPoint>= 0)
	{
		// iTempArr[i][0]用来装点数为 i 有多少张牌，
		// iTempArr[i][1],iTempArr[i][2],iTempArr[i][3],iTempArr[i][4]用来装点数为 i 的牌的编码，
		memset(iTempArr, -1, 12 * 5 * 4);
		CInfo->SortById(pDdz->iOnHand);    //先将数组进行排序
		for (i = 0; pDdz->iOnHand[i] > -1; i++)
		{
			if (pDdz->iOnHand[i] < 48)
			{
				//出点数为 iCards[i]/4 的牌有多少张，用 j 来表示
				j = 1;
				while (-1 != iTempArr[pDdz->iOnHand[i] / 4][j])
					j++;

				//将编码放入iTempArr[iCards[i]/4][j]里面，因为该编码的点数为iCards[i]/4
				iTempArr[pDdz->iOnHand[i] / 4][j] = pDdz->iOnHand[i];

				//将点数为 iCards[i]/4 的牌的张数 改为 j ；
				iTempArr[pDdz->iOnHand[i] / 4][0] = j;
			}
		}
		//int lastMinPoint = CInfo->AnalyzeMainPoint(pDdz->iLastOnTable);
		for (i = pDdz->iLastMainPoint + 1; i < 12; i++)
		{
			iTemp = iLength - 3;
			if (iTempArr[i][0] >= 3)
			{
				j = i;
				while (iTemp)
				{
					if (iTempArr[j][0] >= 3)
					{
						iTemp -= 3;
						j++;
					}
					else break;
				}
				if (sanshunNum + 1 < kPlaMax
					&& iTemp == 0)
				{
					k = 0;
					while (k < iLength)
					{
						iTmpSanshun[k] = iTempArr[j][1];
						k++;
						iTmpSanshun[k] = iTempArr[j][2];
						k++;
						iTmpSanshun[k] = iTempArr[j][3];
						k++;
						j--;
					}
					iTmpSanshun[k] = -2;
					k++;
					iTmpSanshun[k] = -1;
					GetLegalCards(pDdz,iTmpSanshun);

				}
			}
		}
	}
	//出牌时,直接出拆牌后三顺
	if (pDdz->iLastMainPoint < 0)
	{
		int iCopyCards[21];
		memcpy(iCopyCards, pDdz->iOnHand, sizeof(pDdz->iOnHand));
		//排除炸弹
		i = CInfo->CountCards(pDdz->iOnHand);
		int *iCards;
		if (i == 20&&!isSim)
		{
			iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 1);
			for (i = 0; iCards[i] != -1; i++)
			{
				if (iCards[i] != -2)
				{
					DeleteElementInCards(iCopyCards, 21, iCards[i]);
				}
			}

		}
		iCards = pSplitCards->startSplitCard(iCopyCards, 3);
		//如果有三顺，直接出
		if (iCards[0] >= 0)
		{
			j = 0;
			for (i = 0; iCards[i] != -1; i++)
			{
				if (iCards[i] == -2)
				{
					iTmpSanshun[j] = -2;
					j++;
					iTmpSanshun[j] = -1;
					GetLegalCards(pDdz, iTmpSanshun);
				}
				else{
					iTmpSanshun[j] = iCards[i];
					j++;
				}
			}
		}
	}
	return iPlaCount-iCount;
}
//H09-END

//UNDONE
//H1001-START推荐出牌应对三顺带单
//最后修订者:范晓梦，最后修订时间:16-07-26
int RecommendCards::Help10LinkThreeSingle(struct fDdz * pDdz)
{
	int iCount = iPlaCount;
	int iTmpSanshundan[40];
	int sanshundanNum = 0;
	int i, iTemp, j, k, m;
	int iTempArr[12][5];
	int nFlag = 0;
	int iProtect = 0;
	int iLength = (pDdz->iLastTypeCount - 1000);
	// iTempArr[i][0]用来装点数为 i 有多少张牌，
	// iTempArr[i][1],iTempArr[i][2],iTempArr[i][3],iTempArr[i][4]用来装点数为 i 的牌的编码，
	memset(iTempArr, -1, 12 * 5 * 4);
	CInfo->SortById(pDdz->iOnHand);    //先将数组进行排序
	int iCopyOnHand[21] = { -1 };
	//排除炸弹
	int bombPoint[4] = { -1, -1, -1, -1 };
	j = 0;
	int t = 0;//炸弹数
	int *iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 1);
	j = 0;
	if (iCards[0] > -1)
	{
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] == -2)
			{
				bombPoint[j] = iCards[i - 1] / 4;
				j++;
			}
		}
	}
	j = 0;
	for (i = 0; pDdz->iOnHand[i] != -1; i++)
	{
		if (pDdz->iOnHand[i] / 4 != bombPoint[0] && pDdz->iOnHand[i] / 4 != bombPoint[1])
		{
			iCopyOnHand[j] = pDdz->iOnHand[i];
			j++;
		}
	}
	iCopyOnHand[j] = -1;
	int *iOnHand = pDdz->iOnHand;
	bool condition = AnalyzeCondition(pDdz);
	if (iLength < 0)
	{
		iLength = 8;
		i = CInfo->CountCards(pDdz->iOnHand);
		if (condition||i==20)
		iOnHand = iCopyOnHand;
		iCards = pSplitCards->startSplitCard(iOnHand, 3);
		if (iCards[0] >= 0)
		{
			int t = 0;//最大三顺数
			j = 0;
			for (i = 0; iCards[i] != -1; i++)
			{
				if (iCards[i] == -2)
				{
					if (j > t)
					{
						t = j;
					}
					j = 0;
				}
				else
					j++;//牌张数
			}
			iLength = t + t / 3;
		}
	}

	for (i = 0; iOnHand[i] > -1; i++)
	{
		if (iOnHand[i] < 48)
		{
			//出点数为 iCards[i]/4 的牌有多少张，用 j 来表示
			j = 1;
			while (-1 != iTempArr[iOnHand[i] / 4][j])
				j++;

			//将编码放入iTempArr[iCards[i]/4][j]里面，因为该编码的点数为iCards[i]/4
			iTempArr[iOnHand[i] / 4][j] = iOnHand[i];

			//将点数为 iCards[i]/4 的牌的张数 改为 j ；
			iTempArr[iOnHand[i] / 4][0] = j;
		}
	}

	CInfo->SortById(iOnHand);    //先将数组进行排序
	//int lastMinPoint = CInfo->AnalyzeMainPoint(pDdz->iLastOnTable);
	for (i = pDdz->iLastMainPoint + 1; i < 12; i++)
	{
		iTemp = iLength;
		if (iTempArr[i][0] >= 3)
		{
			j = i;
			while (iTemp)
			{
				if (iTempArr[j][0] >= 3)
				{
					iTemp -= 4;
					j++;
				}
				else break;
			}
			if (iTemp == 0)
			{
				k = 0;
				j--;
				while (k < iLength / 4 * 3)
				{
					iTmpSanshundan[k] = iTempArr[j][1];
					k++;
					iTmpSanshundan[k] = iTempArr[j][2];
					k++;
					iTmpSanshundan[k] = iTempArr[j][3];
					k++;
					j--;
				}

				//带牌不能带炸弹和王,出牌时不能带2,
				for (j = 0; iOnHand[j] >= 0; j++)
				{
					nFlag = 1;
					for (m = 0; m < k; m++)
					{
						if (iOnHand[j] / 4 == iTmpSanshundan[m] / 4)
						{
							nFlag = 0;
							break;
						}
						//手数太大不能带2
						if (iOnHand[j]/4==12 && iHandNum > 5)
						{
							nFlag = 0;
							break;
						}
						//除非直接出否则不能带王
						if (iOnHand[j] / 4 == 13 && iHandNum >3)
						{
							nFlag = 0;
							break;
						}
						if (iOnHand[j] / 4 == bombPoint[0] || iOnHand[j] / 4 == bombPoint[1])
						{
							nFlag = 0;
							break;
						}
					}
					if (sanshundanNum + 1 < kPlaMax
						&& nFlag)
					{
						iTmpSanshundan[k] = iOnHand[j];
						k++;
						if (k == iLength)
						{
							iTmpSanshundan[k] = -2;
							k++;
							iTmpSanshundan[k] = -1;
								//CInfo->SortByMuch(iTmpSanshundan);
								if (GetLegalCards(pDdz, iTmpSanshundan))
								{
									sanshundanNum++;
								}
								k = 0;
							break;
						}
					}
				}
			}
		}
	}

	return iPlaCount-iCount;

}
//H1001-END

//UNDONE
//加入放崩溃
//H1002-START推荐出牌应对三顺带双
//最后修订者:范晓梦 最后修订时间:16-07-26
int RecommendCards::Help10LinkThreeDouble(struct fDdz * pDdz)
{
	int iCount = iPlaCount;
	int iCardsNum = CInfo->CountCards(pDdz->iOnHand);
	if (iCardsNum >= 17)
		return 0;
	int iTmpSanshunshuang[70];
	int sanshunshuangNum = 0;
	int i, iTemp, j, k, m, iLength = (pDdz->iLastTypeCount - 1000);
	int iTempArr[12][5];
	int nFlag = 0;
	int iCopyOnHand[21] = { -1 };
	int bombPoint[4] = { -1, -1, -1, -1 };//保存炸弹点数
	int t = 0;//炸弹数
	int *iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 1);
	j = 0;
	if (iCards[0] > -1)
	{
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] == -2)
			{
				bombPoint[j] = iCards[i - 1] / 4;
				j++;
			}
		}
	}
	//排除炸弹和王
	j = 0;
	for (i = 0; pDdz->iOnHand[i] != -1; i++)
	{
		if (pDdz->iOnHand[i] / 4 != bombPoint[0] && pDdz->iOnHand[i] / 4 != bombPoint[1] && pDdz->iOnHand[i] / 4 != bombPoint[2]
			&& pDdz->iOnHand[i] / 4 != bombPoint[3])
		{
			if (pDdz->iOnHand[i] < 52)
			{
				iCopyOnHand[j] = pDdz->iOnHand[i];
				j++;
			}
		}
	}
	iCopyOnHand[j] = -1;
	int *iOnHand = pDdz->iOnHand;
	bool condition = AnalyzeCondition(pDdz);

		auto GetSanShunDouble = [&](int Length)
		{
			for (i = pDdz->iLastMainPoint + 1; i <12; i++)
		{
			iTemp = iLength;
			if (iTempArr[i][0] >= 3)
			{
				j = i;
				while (iTemp)
				{
					if (iTempArr[j][0] >= 3)
					{

						iTemp -= 5;
						j++;
					}
					else break;
				}
				if (iTemp == 0)
				{
					k = 0;
					j--;
					while (k < iLength / 5 * 3)
					{
						iTmpSanshunshuang[k] = iTempArr[j][1];
						k++;
						iTmpSanshunshuang[k] = iTempArr[j][2];
						k++;
						iTmpSanshunshuang[k] = iTempArr[j][3];
						k++;
						j--;
					}

					for (j = 0; pDdz->iOnHand[j] >= 0; j++)
					{
						nFlag = 1;
						for (m = 0; m < k; m++)
						{
							if (iOnHand[j] / 4 == iTmpSanshunshuang[m] / 4)
							{
								nFlag = 0;
								break;
							}
							if (iOnHand[j] / 4 == 12 && iHandNum > 4)
							{
								nFlag = 0;
								break;
							}
							//不能带王
							if (iOnHand[j] / 4 >= 13)
							{
								nFlag = 0;
								break;
							}
							if (iOnHand[j] / 4 == bombPoint[0] || iOnHand[j] / 4 == bombPoint[1])
							{
								nFlag = 0;
								break;
							}
						}
						if (sanshunshuangNum + 1<kPlaMax
							&& nFlag)
						{
							if (iOnHand[j] / 4 != iOnHand[j + 1] / 4)
								continue;
							iTmpSanshunshuang[k] = iOnHand[j];
							k++;
							iTmpSanshunshuang[k] = iOnHand[j + 1];
							k++;
							if (k == iLength)
							{
								iTmpSanshunshuang[k] = -2;
								k++;
								iTmpSanshunshuang[k] = -1;
								GetLegalCards(pDdz, iTmpSanshunshuang);
								//break;
							}
						}
					}
				}
			}
		}
		};
	// iTempArr[i][0]用来装点数为 i 有多少张牌，
	// iTempArr[i][1],iTempArr[i][2],iTempArr[i][3],iTempArr[i][4]用来装点数为 i 的牌的编码，
	memset(iTempArr, -1, 12 * 5 * 4);
	CInfo->SortById(iOnHand);    //先将数组进行排序
	if (pDdz->iLastMainPoint < 0)
	{
		iLength = 10;
		i = CInfo->CountCards(pDdz->iOnHand);
		if (condition || i == 20)
			iOnHand = iCopyOnHand;
	}
	else
	{
		iOnHand = pDdz->iOnHand;
	}
	for (i = 0; iOnHand[i] > -1; i++)
	{
		if (iOnHand[i] < 48)
		{
			//出点数为 iCards[i]/4 的牌有多少张，用 j 来表示
			j = 1;
			while (-1 != iTempArr[iOnHand[i] / 4][j])
				j++;

			//将编码放入iTempArr[iCards[i]/4][j]里面，因为该编码的点数为iCards[i]/4
			iTempArr[iOnHand[i] / 4][j] = iOnHand[i];

			//将点数为 iCards[i]/4 的牌的张数 改为 j ；
			iTempArr[iOnHand[i] / 4][0] = j;
		}
	}
	if (pDdz->iLastMainPoint < 0)
	{
		iLength = 10;
		int i = CInfo->CountCards(pDdz->iOnHand);
		if (condition || i == 20)
			iOnHand = iCopyOnHand;
		iCards = pSplitCards->startSplitCard(iOnHand, 3);
		//如果有三顺，直接出排除炸弹和王的最大三顺
		int maxSanshun = 6;//最大三顺数
		if (iCards[0] >= 0)
		{

			j = 0;
			for (i = 0; iCards[i] != -1; i++)
			{
				if (iCards[i] == -2)
				{
					if (j > maxSanshun)
					{
						maxSanshun = j;
					}
					j = 0;
				}
				else
					j++;//牌张数
			}

			iLength = maxSanshun + 2 * (maxSanshun / 3);
		}

		while (iLength >= 10)
		{
			GetSanShunDouble(iLength);
			maxSanshun -= 3;
			if (maxSanshun>0)
			iLength = maxSanshun + 2  * (maxSanshun / 3);
		}
	}
	else
	{
		GetSanShunDouble(iLength);
	}
	//int lastMinPoint = CInfo->AnalyzeMainPoint(pDdz->iLastOnTable);

	return iPlaCount-iCount;
}
//H1002-END

//UNDONE
//H1101-START推荐出牌应对四带二单
//最后修订者:夏侯有杰，最后修订时间:15-03-10 12:00
int RecommendCards::Help11FourSingle(struct fDdz * pDdz)
{
	int Cards = CInfo->CountCards(pDdz->iOnHand);
	//不首出
	if (!isSim&&Cards >= 17 && pDdz->iLastMainPoint<0)
		return 0;
	///这段代码最大的问题是可能得出的结果是拆出重要牌型里面的牌
	//最多生成5个四带2节点
	int MaxNode = 5;
	int iCount = iPlaCount;
	int iTmpsidaierdan[21];
	memset(iTmpsidaierdan, -1, sizeof(iTmpsidaierdan));
	int sidaierdanNum = 0;
	int i, iTempI, j, k;

	if (CInfo->CountCards(pDdz->iOnHand)<6)
		return 0;
	int *iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 7);
	CInfo->SortById(pDdz->iOnHand);
	for (i = 0; pDdz->iOnHand[i + 3] >= 0; i++)
	{
		//int lastMinPoint = CInfo->AnalyzeMainPoint(pDdz->iLastOnTable);
		if (pDdz->iOnHand[i] / 4 > pDdz->iLastMainPoint
			&& pDdz->iOnHand[i] / 4 == pDdz->iOnHand[i + 1] / 4
			&& pDdz->iOnHand[i] / 4 == pDdz->iOnHand[i + 2] / 4
			&& pDdz->iOnHand[i] / 4 == pDdz->iOnHand[i + 3] / 4)
		{
			//如果炸弹是2
			if (pDdz->iOnHand[i]/4 == 12)
			{
				//如果剩余手数较多，不出
				if (iHandNum > 3)
					continue;
			}
			iTempI = i;
			int theLastJ = 0;
			//查找是否有现成两张单牌,如果有，结合最小的两张
			if (iCards[0] >= 0 && iCards[2] >= 0)
			{
				if (iCards[0] >= 48)
				{
					//如果剩余手数较多，不出
					if (iHandNum > 3)
						continue;
				}
				iTmpsidaierdan[0] = pDdz->iOnHand[i];
				iTmpsidaierdan[1] = pDdz->iOnHand[i + 1];
				iTmpsidaierdan[2] = pDdz->iOnHand[i + 2];
				iTmpsidaierdan[3] = pDdz->iOnHand[i + 3];
				iTmpsidaierdan[4] = iCards[0];
				iTmpsidaierdan[5] = iCards[2];
				iTmpsidaierdan[6] = -2;
				iTmpsidaierdan[7] = -1;
				if (GetLegalCards(pDdz, iTmpsidaierdan))
				{
					sidaierdanNum++;
				}
			}
			else
			{
				for (j = 0; pDdz->iOnHand[j] >= 0; j++)
				{
					//该单牌不能是4条中的数
					if (j == iTempI || j == iTempI + 1 || j == iTempI + 2 || j == iTempI + 3)
						continue;

					if (j != theLastJ)
					{
						if (pDdz->iOnHand[j] / 4 == pDdz->iOnHand[theLastJ] / 4)
							continue;
					}
					if (pDdz->iOnHand[j] / 4 == pDdz->iOnHand[theLastJ] / 4 || pDdz->iOnHand[j] / 4==13)
						continue;
					if (pDdz->iOnHand[j] /4== 12)
					{
						//如果剩余手数较多，不出
						if (iHandNum > 3)
							continue;
					}
					int theLastK = 0;
					for (k = j + 1; pDdz->iOnHand[k] >= 0; k++)
					{

						//该单牌不能是4条中的数,也不能跟前一张单牌点数相同
						if (k == iTempI || k == iTempI + 1 || k == iTempI + 2 || k == iTempI + 3)
							continue;
						if (pDdz->iOnHand[k] / 4 == pDdz->iOnHand[j] / 4)
							continue;
						//更不能跟上一张单牌点数相同
						if (k != theLastK)
						{
							if (pDdz->iOnHand[k] / 4 == pDdz->iOnHand[theLastK] / 4 || pDdz->iOnHand[k] / 4==13)
								continue;
							if (pDdz->iOnHand[k]/4 == 12)
							{
								//如果剩余手数较多，不出
								if (iHandNum > 3)
									continue;
							}

						}


							iTmpsidaierdan[0] = pDdz->iOnHand[i];
							iTmpsidaierdan[1] = pDdz->iOnHand[i + 1];
							iTmpsidaierdan[2] = pDdz->iOnHand[i + 2];
							iTmpsidaierdan[3] = pDdz->iOnHand[i + 3];
							iTmpsidaierdan[4] = pDdz->iOnHand[j];
							iTmpsidaierdan[5] = pDdz->iOnHand[k];
							iTmpsidaierdan[6] = -2;
							iTmpsidaierdan[7] = -1;
							theLastK = k;
							theLastJ = j;
							if (GetLegalCards(pDdz, iTmpsidaierdan))
							{
								sidaierdanNum++;
							}
						}
					}
				}
			}
		}
		return iPlaCount - iCount;
}
//H1101-END

//UNDONE
//H1102-START推荐出牌应对四带二对
//最后修订者:夏侯有杰，最后修订时间:15-03-23 12:00
int RecommendCards::Help11FourDouble(struct fDdz * pDdz)
{
	int Cards = CInfo->CountCards(pDdz->iOnHand);
	if (!isSim&&Cards >= 17)
		return 0;
	//最多生成5个四带2节点
	int MaxNode = 5;
	int iCount = iPlaCount;
	int iTmpsidai[21];
	memset(iTmpsidai, -1, sizeof(iTmpsidai));
	int sidaiNum = 0;
	int i, iTempI, j, k;
	if (CInfo->CountCards(pDdz->iOnHand)<8)
		return 0;
	CInfo->SortById(pDdz->iOnHand);
	for (i = 0; pDdz->iOnHand[i + 3] >= 0; i++)
	{
		if (pDdz->iOnHand[i] / 4 > pDdz->iLastMainPoint
			&& pDdz->iOnHand[i] / 4 == pDdz->iOnHand[i + 1] / 4
			&& pDdz->iOnHand[i] / 4 == pDdz->iOnHand[i + 2] / 4
			&& pDdz->iOnHand[i] / 4 == pDdz->iOnHand[i + 3] / 4)
		{
			//如果炸弹是2
			if (pDdz->iOnHand[i] >= 48)
			{
				//如果剩余手数较多，不出
				if (iHandNum > 3)
					continue;
			}
			iTempI = i;
			for (j = 0; pDdz->iOnHand[j] >= 0; j++)
			{
				if (j == iTempI || j == iTempI + 1 || j == iTempI + 2 || j == iTempI + 3 || pDdz->iOnHand[j] / 4 != pDdz->iOnHand[j + 1] / 4)
					continue;
				if (pDdz->iOnHand[j] >= 48)
				{
					//如果剩余手数较多，不出
					if (iHandNum > 3)
						continue;
				}
				for (k = j + 2; pDdz->iOnHand[k] >= 0; k++)
				{
					//该单牌不能是4条中的数且把大小王排除了
					if (k == iTempI || k == iTempI + 1 || k == iTempI + 2 || k == iTempI + 3 || pDdz->iOnHand[j] >= 52 || pDdz->iOnHand[k] / 4 != pDdz->iOnHand[k + 1] / 4)
						continue;
					if (pDdz->iOnHand[k] >= 48)
					{
						//如果剩余手数较多，不出
						if (iHandNum > 3)
							continue;
					}
					if (sidaiNum + 1<MaxNode)
					{
						iTmpsidai[0] = pDdz->iOnHand[i];
						iTmpsidai[1] = pDdz->iOnHand[i + 1];
						iTmpsidai[2] = pDdz->iOnHand[i + 2];
						iTmpsidai[3] = pDdz->iOnHand[i + 3];
						iTmpsidai[4] = pDdz->iOnHand[j];
						iTmpsidai[5] = pDdz->iOnHand[j + 1];
						iTmpsidai[6] = pDdz->iOnHand[k];
						iTmpsidai[7] = pDdz->iOnHand[k + 1];
						iTmpsidai[8] = -2;
						iTmpsidai[9] = -1;

							if (GetLegalCards(pDdz, iTmpsidai))
							{
								sidaiNum++;
							}
					}
				}
			}
		}
	}

	return iPlaCount-iCount;
}
//H1102-END


//获取玩家类型，1为AI是地主，2为下家是地主,3为上家是地主
//最后修订者:成文杰，最后修订时间:2017.7.13
int RecommendCards::GetPlayerType(fDdz *pDdz)
{
	if (pDdz->cDir == pDdz->cLandlord)
		return 1;
	else if (pDdz->cLastPlay == pDdz->cLandlord)
		return 3;
	else
		return 2;
}

//计算其他玩家手中剩牌数
int RecommendCards::OtherPlayersCards(fDdz *pDdz, Ddz *rDdz)
{
	int i;
	//如果是AI
	if (pDdz->cDir == rDdz->cDir)
	{
		pDdz->iUpPlayerCards = rDdz->iUpPlayerCards;//上手玩家手牌数
		pDdz->iDownPlayerCards = rDdz->iDownPlayerCards;//下手玩家手牌数
	}
	//如果是AI上手玩家
	else if (pDdz->cDir == rDdz->cLastPlay)
	{

		for (i = 0; rDdz->iOnHand; i++);
		pDdz->iUpPlayerCards = rDdz->iDownPlayerCards;//上手玩家手牌数
		pDdz->iDownPlayerCards = i;
	}
	//如果是AI下手玩家
	else
	{
		for (i = 0; rDdz->iOnHand; i++);
		pDdz->iUpPlayerCards = i;//上手玩家手牌数
		pDdz->iDownPlayerCards = rDdz->iUpPlayerCards;
	}
	return 0;
}

////计算两位玩家手中剩牌，存储在iOnOtherHand[]里,并按从小到大排序,返回两位玩家剩牌数
////iOnOtherHand[]以-1结尾
////最后修订者：范晓梦
////最后修改时间：2016/4/1
//int RecommendCards::OtherPlayersCards(fDdz* pDdz)
//{
//
//	//若牌面编码既没有出现在iOnhand[]和iOnTable[]里面，则牌面在其他玩家手中
//
//	int allCards[54];//建立一个能装下所有牌面编码的数组,初始化为1表示所有牌都有
//	for (int i = 0; i < 54; i++)
//	{		//初始化本局手牌
//		allCards[i] = 1;
//	}
//	int cards = 0;
//	//排除iOnHand[]中的牌
//
//	for (int i = 0; pDdz->iOnHand[i] > -1; i++)
//	{
//
//		if (cards != pDdz->iOnHand[i])
//			cards = pDdz->iOnHand[i];
//		allCards[cards] = 0;
//		cards++;
//	}
//
//
//	//排除iOnTable[]中的牌,iontable已有的牌对应的值设为0；
//	for (int i = 0; pDdz->iOnTable[i][0] != -2; i++)
//	{
//		cards = 0;
//		CInfo->SortById(pDdz->iOnTable[i]);
//		for (int j = 0; pDdz->iOnTable[i][j] >-1; i++)
//		{
//			if (cards != pDdz->iOnTable[i][j])
//				cards = pDdz->iOnTable[i][j];
//			allCards[cards] = 0;
//			cards++;
//		}
//
//	}
//	//将结果登记到iOnOtherHand[]中
//	cards = 0;
//	for (int i = 0; i<54; i++)
//	{
//		if (allCards[i] == 1)
//		{
//			pDdz->iOnOtherHand[cards] = i;
//			cards++;
//		}
//
//	}
//	cards++;
//	pDdz->iOnOtherHand[cards] = -1;
//	CInfo->SortById(pDdz->iOnOtherHand);
//	cards--;
//	return cards;
//
//}


////UNDO：如果为地主，则固定三张公共牌
////为其他玩家随机生成一副牌
////最后修订者：范晓梦
////最后修改时间：2016/4/19
//void RecommendCards::RandomHandCards(fDdz* pDdz)
//{
//	int count = 0;//OtherPlayersCards(pDdz);
//	int count1 = 0;//LastPlayerCardsCount(pDdz);
//	int count2 = count - count1;
//
//	std::vector<int> vi(count); // 定义一个大小为N的vector
//	for (int i = 0; i<count; ++i)
//	{
//		vi[i] = pDdz->iOnOtherHand[i];
//	}
//	srand((unsigned)time(NULL));
//	std::random_shuffle(vi.begin(), vi.end());
//
//	int i = 0;
//	//生成己方上一手玩家的随机牌
//	for (vector<int>::iterator it = vi.begin(); it != vi.begin() + count1; it++)
//	{
//
//		pDdz->iRandomOnHand[0][i] = *it;
//		i++;
//	}
//
//	pDdz->iRandomOnHand[0][i] = -1;
//	i = 0;
//	cout << "iRandomOnHand[0]:";
//	while (pDdz->iRandomOnHand[0][i] != -1)
//	{
//		cout << pDdz->iRandomOnHand[0][i] << ",";
//		i++;
//	}
//	cout << '\n';
//	CInfo->SortById(pDdz->iRandomOnHand[0]);
//	//生成另一个非己方玩家的随机牌
//	i = 0;
//	for (vector<int>::iterator it = vi.begin() + count1; it != vi.end(); it++)
//	{
//		pDdz->iRandomOnHand[1][i] = *it;
//		i++;
//	}
//
//	pDdz->iRandomOnHand[1][i] = -1;
//
//	i = 0;
//	cout << "iRandomOnHand[1]:";
//	while (pDdz->iRandomOnHand[1][i] != -1)
//	{
//		cout << pDdz->iRandomOnHand[1][i] << ",";
//		i++;
//	}
//	cout << '\n';
//	cout << "iOnHand";
//	i = 0;
//	while (pDdz->iOnHand[i] != -1)
//	{
//		cout << pDdz->iOnHand[i] << ",";
//		i++;
//	}
//	CInfo->SortById(pDdz->iRandomOnHand[1]);
//}

////M07-START从手牌模拟移除一组牌
////最后修订者:梅险,最后修订时间:15-03-01
//void RecommendCards::HelpTakeOff(fDdz* pDdz, int num)
//{
//	int i, j = 0, k, same;
//	for (i = 0; pCardsOnHand[i] >= 0; i++)
//	{
//		same = 0;
//		for (k = 0; pDdz->iPlaArr[num][k] >= 0; k++)
//		{
//			if (pCardsOnHand[i] == pDdz->iPlaArr[num][k])
//			{
//				same = 1;
//			}
//		}
//		if (same != 1)
//		{
//			pDdz->iPlaOnHand[j] = pCardsOnHand[i];
//			j++;
//		}
//	}
//	pDdz->iPlaOnHand[j] = -1;
//}



// 判断前面两家是否PASS两次
bool RecommendCards::IfPassTwice(fDdz *pDdz)
{
	if (pDdz->iLastTypeCount <= 0)
	{//连续2家PASS,出任意牌型
		return true;
	}
	return false;

}

//DEBUG:所有这里面的判断都不能简单解决首次出牌时的随意出牌问题
//BUG
// 配合SpitCard的返回格式拆出牌型
// 从iCards里面分离出能打过上家出牌的解,保存到参数iSave里面,iSave必须初始化为-1
//最后修订者:范晓梦，最后修订时间:16-07-28 23:50
//修订内容：增加判断出牌时面对的情况
//2016/8/8:增加判断炸弹，火箭情况
//2016/8/10 :取消传入带牌主牌可以判断主牌的语句，避免混淆
int RecommendCards::GetLegalCards(fDdz *pDdz, int *iCards, int *iSave)
{
	//对牌、三条、单顺、双顺、三顺以最大牌张比较大小；三带一、三顺带牌、四带二仅按其中三条、三顺、四条的牌张比较大小，与带牌大小无关。
	//火箭大于炸弹，火箭及炸弹均大于其他牌型，炸弹之间按牌张大小进行比较。
	int iCardsTmp[50];
	int t = 0;//用于指定iCards下标
	int start;//每个牌型开始的下标
	int iNodesCount = 0;
	//把数组改成“xxx -2 xxxx -2 -1”结构
	int num;
	for (num = 0; iCards[num] != -1; num++);
	if (iCards[num - 1] != -2)
	{
		iCards[num] = -2;
		iCards[num + 1] = -1;
	}
	/////
	for (int i = 0; iCards[i] != -1; i++)
	{
		start = 0;
		if (iCards[i] == -2)
		{
			iCardsTmp[t] = -1;
			CInfo->SortById(iCardsTmp);
			int iTypeCount = CInfo->AnalyzeTypeCount(iCardsTmp);
			int iType = iTypeCount / 100;
			int iCardsCount = iTypeCount % 100;
			int myPoint = 0;
			//如果是三顺
			//if (iType == 9 && pDdz->iLastTypeCount / 100 == 10)
			//{
			//但要求带牌
			//iType = 10;//等同判断带牌
			//}
			//如果传入是三条但要对的是三带一
			//if (iType == 5 && pDdz->iLastTypeCount / 100 == 6)
			//{
			//但要求的是带牌
			//iType = 6;//等同判断三条
			//}


			if (pDdz->iLastMainPoint == -1 || pDdz->iLastTypeCount / 100 == iType)
			{
				if (pDdz->iLastTypeCount % 100 == iCardsCount || pDdz->iLastMainPoint == -1)
				{
					iCardsTmp[iCardsCount] = -1;
					//如果合法,添加到iSave
					myPoint = CInfo->AnalyzeMainMaxPoint(iCardsTmp);
					if (iLastMaxPoint<myPoint)
					{
						iCardsTmp[t] = -1;
						AppendCards(iCardsTmp, iSave);
						iNodesCount++;
					}
				}
			}
			//如果出炸弹且上家不是炸弹也不是火箭
			if (iType == 2 && pDdz->iLastTypeCount / 100>2)
			{
				iCardsTmp[t] = -1;
				AppendCards(iCardsTmp, iSave);
				iNodesCount++;
			}
			//如果火箭
			if (iType == 1)
			{
				iCardsTmp[t] = -1;
				AppendCards(iCardsTmp, iSave);
				iNodesCount++;
			}

			t = 0;
		}
		else
		{
			iCardsTmp[t] = iCards[i];
			++t;
		}
	}
	return iNodesCount;
}

//传入单个套牌信息并判断是否合法，如果合法，加入iplaarr
int RecommendCards::GetLegalCards(fDdz *pDdz, int *iCards)
{
	//对牌、三条、单顺、双顺、三顺以最大牌张比较大小；三带一、三顺带牌、四带二仅按其中三条、三顺、四条的牌张比较大小，与带牌大小无关。
	//火箭大于炸弹，火箭及炸弹均大于其他牌型，炸弹之间按牌张大小进行比较。
	int iCount = iPlaCount;
	int iCardsTmp[30];
	int t = 0;//用于指定iCards下标
	int iTypeCount;
	int iType;
	int iCardsCount;
	int myPoint;
	for (int i = 0; iCards[i] != -1; i++)
	{
		/////////处理数据//////////////////
		if (iCards[i] == -2)// 等于-2时，
		{
			iCardsTmp[t] = -1;
			CInfo->SortById(iCardsTmp);
			iTypeCount = CInfo->AnalyzeTypeCount(iCardsTmp);
				iType = iTypeCount / 100;
			iCardsCount = iTypeCount % 100;
			myPoint = CInfo->AnalyzeMainMaxPoint(iCardsTmp);
			iCardsTmp[t] = -2;
			t++;
			iCardsTmp[t] = -1;
			t = 0;
		////////////判断////////////////////
		if (pDdz->iLastTypeCount / 100 == iType)
		{
			if (pDdz->iLastTypeCount % 100 == iCardsCount)
			{
				if (iLastMaxPoint < myPoint)
				{
					AddInPlaArr(pDdz, iCardsTmp);
				}
			}
		}
		if (pDdz->iLastMainPoint == -1)
		{
			if (iTypeCount > 0)
			{
				AddInPlaArr(pDdz, iCardsTmp);
			}
		}

		//如果出炸弹且上家不是炸弹也不是火箭
		if (iType == 2 && pDdz->iLastTypeCount / 100>2)
		{
			AddInPlaArr(pDdz, iCardsTmp);
		}
		//如果火箭
		if (iType == 1)
		{
			AddInPlaArr(pDdz, iCardsTmp);
		}
	}
	else
	{
		iCardsTmp[t] = iCards[i];
		++t;
	}
	}

	return iPlaCount - iCount;


}


//把一套分好牌型的牌解析出牌存储在IPLARR数组里面
//TODO:修改局面信息
//最后修订者:范晓梦，最后修订时间:16-08-1 23:50
//修订内容：增加节点限制
int RecommendCards::AddInPlaArr(fDdz *pDdz, int *iCards)
{
	for (int i = 0; iCards[i] != -1; i++)
	{
		if (iCards[i] != -2)
		{
			bool isIn = false;
			for (int j = 0; pDdz->iOnHand[j] != -1; j++)
			{
				if (iCards[i] == pDdz->iOnHand[j])
				{
					isIn = true;
					break;
				}
			}
			if (!isIn)
				return iPlaCount;
		}
	}
	int num = iPlaCount;
	int t = 0;//第二维数组下标
	if (iCards[0] != -2)
	{
		for (int i = 0; iCards[i] != -1; i++)
		{

			if (iCards[i] != -2)
			{
				//如果节点数已经到达上限，直接返回
				if (num + 2 > PlaMax)
					return num;
				pDdz->iPlaArr[num][t] = iCards[i];
				t++;
			}
			else
			{
				pDdz->iPlaArr[num][t] = -1;
				t = 0;
				num++;
			}
		}
	}
	else
	{
		//如果节点数已经到达上限，直接返回
		if (num + 2 > PlaMax)
			return num;
		pDdz->iPlaArr[num][t] = -2;
		t++;
		pDdz->iPlaArr[num][t] = -1;
		num++;
	}
	iPlaCount = num;
	return num;
}




int RecommendCards::IfHelpEnemyCard(fDdz* pDdz)
{
	//是地主出牌
	if (pDdz->cDir == pDdz->cLandlord)
		return 1;
	//不是地主出牌
	if (pDdz->iLastPassCount == 1)
	{
		if (pDdz->cLastPlay == pDdz->cLandlord)
			return 0;
		else
			return 1;
	}
	if (pDdz->iLastPassCount == 0)
	{
		if (pDdz->cLastPlay == pDdz->cLandlord)
			return 1;
		else
			return 0;
	}
	return 1;
}


// 解析拆牌类产生的牌并存在iSave里面
int RecommendCards::AnalyzeSplitedCards(int* iCards, int *iSave)
{
	int t = 0;//用于指定iCards下标
	for (int i = 0; iCards[i] != -1; i++)
	{

		if (iCards[i] != -2)
		{
			iSave[t] = iCards[i];
			++t;
		}
	}
	iSave[t] = -1;
	return t;
}


//将from数组加到to数组后面，to数组分隔符为-2，结束符为-1
//从结束符开始写入数据，并以-2，-1结尾
void RecommendCards::AppendCards(int* from, int* to)
{
	int i;
	//读到to数组-1部分
	for (i = 0; to[i] != -1; i++);
	int j = i;

	for (i = 0; from[i] != -1; i++, j++)
	{
		to[j] = from[i];
	}
	//改变后缀
	if (to[j - 1] != -2)
	{
		to[j] = -2;
		to[j + 1] = -1;
	}
}




// 计算手牌最小手数
int RecommendCards::MinHandNum(int* iOnHand)
{
	int iCopyOnHand[21];
	//memcpy(iCopyOnHand, iOnHand, sizeof(iOnHand));//莫名失效
	for (int i = 0; i < 21; i++)
	{
		iCopyOnHand[i] = iOnHand[i];
	}
	int num = 0;//手数
	int t = 0;
	int j = 0;
	int bestStart = 0;
	int i = 0;
	int iTmpCards[21];
	memset(iTmpCards, -1, sizeof(iTmpCards));
	//从拆牌数组中，找寻最大牌张数的牌型,并从icopyHand中排除,并增加手数
	auto findMaxNumCards = [&](int *iSplitedCards)
	{
		if (iSplitedCards[0] > -1)
		{
			t = 0;//最大牌张数
			j = 0;
			bestStart = 0;//最大牌张起始位置
			for (i = 0; iSplitedCards[i] != -1; i++)
			{
				if (iSplitedCards[i] == -2)
				{
					if (j > t)
					{
						t = j;
						bestStart = i - t;
					}
					j = 0;
				}
				else
					j++;//计算牌张数
			}
		//	memcpy(iTmpCards, &iSplitedCards[bestStart], t*sizeof(int));
			t = 0;
			for (i = bestStart; iSplitedCards[i] != -2; i++)
			{
				iTmpCards[t] = iSplitedCards[i];
				t++;
			}
			iTmpCards[t] = -1;
			num++;
			//从iCopyOnHand中排除
			if (iTmpCards[0] > -1)
			{
				for (i = 0; iTmpCards[i] != -1; i++)
				{
					DeleteElementInCards(iCopyOnHand,21, iTmpCards[i]);
				}
			}
		}
	};
	//判断牌型数并排除,并增加手数
	auto DeleteTypeCards = [&](int *iCards)
	{
		if (iCards[0] > -1)
		{
			t = 0;
			for (i = 0; iCards[i] != -1; i++)
			{
				if (iCards[i] == -2)
				{
					iTmpCards[t] = -1;
					for (j = 0; iTmpCards[j] != -1; j++)
					{
						DeleteElementInCards(iCopyOnHand, 21,iTmpCards[j]);
					}
					num++;
					t = 0;
					iTmpCards[0] = -1;
				}
				else
				{
					iTmpCards[t] = iCards[i];
					t++;
				}
			}
		}
	};

	//首先判断火箭
	int *iCards = pSplitCards->startSplitCard(iOnHand, 0);
	if (iCards[0] > -1)
	{
		DeleteElementInCards(iCopyOnHand,21,52);
		DeleteElementInCards(iCopyOnHand, 21, 53);	
		num++;
	}
	//判断炸弹
	iCards = pSplitCards->startSplitCard(iOnHand, 1);
	//判断有几个炸弹,并排除(通过)
	DeleteTypeCards(iCards);

	//找到所有三顺,并排除
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 3);
	DeleteTypeCards(iCards);

	//判断双顺,找到所有双顺,并排除
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 5);
	DeleteTypeCards(iCards);

	//判断单顺,找到所有单顺,并排除
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 4);
	DeleteTypeCards(iCards);

	//判断三条
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 2);
	//判断有几个三条,并从iCopyOnHand中排除
	DeleteTypeCards(iCards);
	//判断对子
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 6);
	//判断有几个对子,并排除
	DeleteTypeCards(iCards);

	//判断单牌
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 7);

	DeleteTypeCards(iCards);
	//判断王(剩余牌必为王或2，拆牌不清楚)
	for (i = 0; iCopyOnHand[i] != -1; i++)
	{
		if (iCopyOnHand[i] >= 48)
			num++;
	}
	
	return num;
	
 }




bool RecommendCards::GetModel(Ddz * realDdz)
{
	pRealDdz = realDdz;
	pMoldel->GetOtherPokersCards(iUpCards, iDownCards, pRealDdz);
	//分析手牌数
	int i = CInfo->CountCards(iUpCards);
	int j = CInfo->CountCards(iDownCards);
	if(pRealDdz->iUpPlayerCards != i || pRealDdz->iDownPlayerCards != j)
		return false;
	CInfo->SortById(iUpCards);
	CInfo->SortById(iDownCards);
	return true;
}

//不利局势返回false,非不利返回true
bool RecommendCards::AnalyzeCondition(fDdz* pDdz)
{
	int sort = 0;//因素
	//从手牌数入手
	if (pDdz->cLandlord == pDdz->cDir)
	{
		if (pDdz->iUpPlayerCards <= 6 || pDdz->iDownPlayerCards <= 6)
			sort++;
	}
	else//AI不是地主
	{
		//上家是地主且牌数小于等于6
		if (pDdz->cLastPlay == pDdz->cLandlord&&pDdz->iUpPlayerCards <= 6)
			sort++;
		//上家不是地主（即下家为地主），下家牌数小于等于6
		if (pDdz->cLastPlay != pDdz->cLandlord&&pDdz->iDownPlayerCards <= 6)
			sort++;
	}

	//从手数入手
	if (pDdz->cLandlord == pDdz->cDir)
	{
		if (iUpHandNum <= 3 || iDownHandNum <= 3)
			sort++;
		if (iUpHandNum < iHandNum&&iDownHandNum < iHandNum)
			sort++;
	}
	else//AI不是地主
	{
		//上家是地主，且上家的手数小于等于3
		if (pDdz->cLastPlay == pDdz->cLandlord&&iUpHandNum <= 3)
			sort++;
		if (pDdz->cLastPlay != pDdz->cLandlord&&iDownHandNum <= 3)
			sort++;
	}
	//从牌价值入手
	int MyValue = CardsValue(pDdz->iOnHand);
	int UpValue = CardsValue(iUpCards);
	int DownValue = CardsValue(iDownCards);
	if (pDdz->cLandlord == pDdz->cDir)
	{
		if (MyValue <UpValue&& MyValue <DownValue)
			sort++;

	}
	else
	{
		if (pDdz->cLastPlay == pDdz->cLandlord&&UpValue >MyValue&&UpValue>DownValue)
			sort++;
		if (pDdz->cLastPlay != pDdz->cLandlord&&DownValue >MyValue&&DownValue >UpValue)
			sort++;
	}
	if (sort >= 2)
		return false;
	else
	return true;
}


void RecommendCards::DeleteElementInCards(int* iCards, int iCardsCount, int iElement)
{
	int i, j;
	i = j = 0;
	for (j = 0; j < iCardsCount; j++)
	{
		if (iCards[j] == iElement)
			i++;
		else
			iCards[j - i] = iCards[j];

	}
}

//
int RecommendCards::AnalyzeSingleLink(int Length, int iLastMainPoint, int *Cards, int* iTmpShunzi)
{
	if (Length < 5||Length>12)
		return 0;
	//去除重牌并从小到大排序
	int i, j, k;
	int iCards[21];
	for (i = 0; i < 21; i++)
	{
		iCards[i] = Cards[i];
	}
	CInfo->GetUnReaptedCards(iCards);
		int iBiaoji, iFlag, num;
		num = 0;
		for (i = 0; iCards[i + Length - 1] >= 0; i++)
		{
			num = 0;//牌张数
			iBiaoji = 0;
			iFlag = 0;
			//如果大于于上一张牌的最小点数
			if (iCards[i] / 4 > iLastMainPoint)
			{
				for (j = i; j < Length - 1 + i; j++)
				{
					//如果与后面的牌点不连序，标记并退出循环
					if (iCards[j] / 4 != iCards[j + 1] / 4 - 1)
					{
						iBiaoji = 1;
						break;
					}
				}
			}
			else
				continue;
			if (iBiaoji == 0)
			{
				for (k = i; k < Length + i; k++)
				{
					if (iCards[k] < 0)
						break;
					iTmpShunzi[iFlag] = iCards[k];
					iFlag++;
					if (k == Length - 1 + i)
					{
						num = iFlag;
						iTmpShunzi[iFlag++] = -2;
						iTmpShunzi[iFlag++] = -1;
						return num;
					}
				}
			}
		}
		return num;
}


int RecommendCards::CardsValue(int* iCards)
{
	int i;
	int dSum = 100;			//估值
	for (i = 0; iCards[i] >= 0; i++)
	{
		dSum = dSum - 5;			//手牌越少越好每多一张牌优势减5分
		if (i >= 1 && iCards[i - 1] / 4 == iCards[i] / 4)
			dSum = dSum + 2;	//相邻两牌同点加2分
		if (i >= 4 && iCards[i - 4] / 4 <= 7
			&& iCards[i - 4] / 4 + 1 == iCards[i - 3] / 4
			&& iCards[i - 3] / 4 + 1 == iCards[i - 2] / 4
			&& iCards[i - 2] / 4 + 1 == iCards[i - 1] / 4
			&& iCards[i - 1] / 4 + 1 == iCards[i] / 4)
			dSum = dSum + 6;	//2以下相邻五牌单顺加6分
	}
	return dSum;
}
