#include "PokerModelAnalysis.h"


void PokerModelAnalysis::Init()//初始化
{
	UpWrong = 0;
	DownWrong = 0;
	for (int i = 0; i < 8; i++)////一个二维数组，放牌型，可以增加，，不是说牌型不够吗
	{
		for (int j = 0; j < 40; j++)
		{
			iUtimate[i][j] = -1;////8行40列数组，所有元素内容赋初值为－1
		}
	}
	for (int i = 0; i < 21; i++)////最多20张，用来初始化上家下家的牌的数组
	{
		iOneUtimate[i] = -1;
	}
	for (int i = 0; i < 21; i++)////2是下家
	{
		iTwoUtimate[i] = -1;
	}
	for (int i = 0; i < 38; i++)////除开自家手牌17张，剩下的牌放入一个数组，对手的所有牌都在这个数组中
	{
		Buffer[i] = pMyDdz->iOnOtherHand[i];////把其他牌装入一个缓冲数组存放.不过没有看到这个数组的定义
	}
}//初始化牌型信息

PokerModelAnalysis::PokerModelAnalysis()////构造函数，ab变量的定义
{
	A = new SplitCard();
	B = new CardsInfo();
}
PokerModelAnalysis::~PokerModelAnalysis()////析构函数
{
}
void PokerModelAnalysis::GetOtherPokersCards(int *iUpCards, int *iDownCards, Ddz *pDdz)////获得其他玩家的牌
{
	pMyDdz = pDdz;////指针，我的指针

	int temp[18];//除去底牌

	Init();////调用初始化那个函数，将改建立的数组进行全元素为－1的初始化
	if (pMyDdz->iOTmax <= 2)//当前出牌手`数小于2
	{
		GameStart();////调用两个函数，一个是用来模拟，一个是真实的对手模型
		Gaming();
	}
	else
	{
		Gaming();////当出牌手数大于2后，就调用这一个
	}
	for (int i = 0; i < 21; i++)////循环，将上家的牌数组中的元素依次放入上家牌组
	{
		iUpCards[i] = iOneUtimate[i];////上家的牌，在recommend cards里面定义的
	}
	for (int i = 0; i < 21; i++)////循环，将下家的牌数组中的元素依次放入下家牌组
	{
		iDownCards[i] = iTwoUtimate[i];////下家的牌
	}
}

void PokerModelAnalysis::GameStart()
{
	//CardsValue(Hc);
	//int size = sizeof(iUtimate);
	//int i = 0;
	//memcpy(iMyUtimate, iUtimate, size);
	//if (i = 0)
	//{
	//	if (InitialSum > Standard2)//牌力值大于标准值2就是好牌
	//	{
	//		LordBest();
	//	}
	//	if (InitialSum >= Standard1 && InitialSum <= Standard2)//牌力值介于标准值1和标准值2之间就是一般的牌
	//	{
	//		Equality();
	//	}
	//	else
	//	{
	//		PartnerBest();
	//	}
	//}
	//else//烂牌
	//{
	//	Equality();
	//}
	for (int i = 0; i < 21; i++)////还是循环，循环两家必有的牌，将初值全都赋为－1
	{
		iOneDe[i] = -1;////上家必有得牌
	}
	for (int i = 0; i < 21; i++)
	{
		iTwoDe[i] = -1;////下家必有得牌
	}
	if (pMyDdz->cDir != pMyDdz->cLandlord)////当我不是地主的时候
	{
		Fendipai();////调这个函数
	}
}

void PokerModelAnalysis::Gaming()////执行这个函数，又是调一堆其他函数
{
	DeleteC();
	Analysis(2);
	Analysis(1);
	Random(Buffer);
}

void PokerModelAnalysis::Analysis(int t)
{
/*------------------------------------------------------
＊牌型分析：将所有的牌型添加进去，总共15种，包含pass
＊书写者：杨丽颖   2017－07－05
*///-----------------------------------------------------------
	int iCs[21];////放牌的数组啊，最大20张，这是一个拿去模拟出牌的数组，就是17张手牌加上3张底牌的组合
	for (int i = 0; i < 21; i++)
	{
		iCs[i] = -1;////还是依次循环然后全部赋值为－1，
	}
	if (t == 2)////看传的参数，等于2就是调用 Analysis(2);然后这是一个函数，1，2只是用来进入不同的if语句 等于2应该是介入上家的内容，等于1是介入下家的内容
	{
		for (int i = 0; i < 21; i++)
		{
			iCs[i] = pMyDdz->iOnTable[pMyDdz->iOTmax - 2][i];////已出牌数组（所有值初始化为-2）每行是一手牌，以-1结尾，Pass记为-1，162=54*3
		}
		DeleteB(iCs, 2);////然后调用这个delete函数

		if (B->IsType0Pass(iCs) == 1)////如果pass了就直接返回，这个是cardsinfo里面的，判断PASS，返回1是，0不是
		{
			return;
		}
		if (B->IsType1Rocket(iCs) == 1)
		{
			DeleteB(iCs, 1);/*???*/
			Type1(iCs, 2);
			return;
		}
		if (B->IsType2Bomb(iCs) == 1)
		{
			DeleteB(iCs, 1);
			Type2(iCs, 2);
			return;
		}
		if (B->IsType3Single(iCs) == 1)////是单牌 分析，就去调用后面的函数   判断单牌，返回1是，0不是
		{
			DeleteB(iCs, 1);
			Type3(iCs, 2);
			return;
		}
		if (B->IsType4Double(iCs) == 1)////是对牌 分析   判断对牌，返回1是，0不是
		{
			DeleteB(iCs, 1);
			Type4(iCs, 2);
			return;
		}
		if (B->IsType5Three(iCs) == 1)
		{
			DeleteB(iCs, 1);
			Type5(iCs, 2);
			return;
		}
		if (B->IsType6ThreeOne(iCs) == 1)////是三带一 分析   判断三带一单，返回1是，0不是
		{
			DeleteB(iCs, 1);
			Type6_11(iCs, 2);
			return;
		}
		if (B->IsType6ThreeDouble(iCs) == 1) {
			DeleteB(iCs, 1);
			Type6_11(iCs, 2);
			return;
		}
		if (B->IsType7LinkSingle(iCs) == 1)//是单顺 分析
		{
			DeleteB(iCs, 1);
			Type6_11(iCs, 2);
			return;
		}
		if (B->IsType8LinkDouble(iCs) == 1)
		{
			DeleteB(iCs, 1);
			Type6_11(iCs, 2);
			return;
		}
		if (B->IsType9LinkThree(iCs) == 1)
		{
			DeleteB(iCs, 1);
			Type6_11(iCs, 2);
			return;
		}
		if (B->IsType10LinkThreeSingle(iCs) == 1)
	    {
			DeleteB(iCs,1);
			Type6_11(iCs,2);
			return;
		}
		if (B->IsType10LinkThreeDouble(iCs) == 1)
		{
			DeleteB(iCs, 1);
			Type6_11(iCs, 2);
			return;
		}
		if (B->IsType11FourSingle(iCs) == 1)
		{
			DeleteB(iCs,1);
			Type6_11(iCs,2);
			return;
		}
		if (B->IsType11FourDouble(iCs) == 1) {
			DeleteB(iCs, 1);
			Type6_11(iCs, 2);
			return;
		}
	 }
	if (t == 1)//t＝1，进到这个if语句中的内容，是当前出牌手数减1的那一行的i个元素的值，放入这个cs数组中
	{
		for (int i = 0; i < 21; i++)
		{
			iCs[i] = pMyDdz->iOnTable[pMyDdz->iOTmax - 1][i];
		}
		DeleteB(iCs, 1);

		if (B->IsType0Pass(iCs) == 1)
			{
				return;
			}
		if (B->IsType1Rocket(iCs) == 1)
			{
				DeleteB(iCs, 2);
				Type1(iCs, 1);
				return;
			}
		if (B->IsType2Bomb(iCs) == 1)
			{
				DeleteB(iCs, 2);
				Type2(iCs, 1);
				return;
			}
		if (B->IsType3Single(iCs) == 1)
			{
				DeleteB(iCs, 2);
				Type3(iCs, 1);//这个type3是用来赋值的
				return;
			}
		if (B->IsType4Double(iCs) == 1)
			{
				DeleteB(iCs, 2);
				Type4(iCs, 1);
				return;
			}
		if (B->IsType5Three(iCs) == 1)
			{
				DeleteB(iCs, 2);
				Type5(iCs, 1);
				return;
			}
		if (B->IsType6ThreeOne(iCs) == 1)
			{
				DeleteB(iCs, 2);
				Type6_11(iCs, 1);
				return;
			}
		if (B->IsType6ThreeDouble(iCs) == 1)
			{
				DeleteB(iCs, 2);
				Type6_11(iCs, 1);
				return;
			}
		if (B->IsType7LinkSingle(iCs) == 1)//是单顺 分析
			{
				DeleteB(iCs, 2);
				Type6_11(iCs, 1);
				return;
			}
		if (B->IsType8LinkDouble(iCs) == 1)
			{
				DeleteB(iCs, 2);
				Type6_11(iCs, 1);
				return;
			}
		if (B->IsType9LinkThree(iCs) == 1)
			{
				DeleteB(iCs, 2);
				Type6_11(iCs, 1);
				return;
			}
		if (B->IsType10LinkThreeSingle(iCs) == 1)
			{
				DeleteB(iCs,2);
				Type6_11(iCs,1);
				return;
			}
		if (B->IsType10LinkThreeDouble(iCs) == 1)
			{
				DeleteB(iCs,2);
				Type6_11(iCs, 1);
				return;
			}
		if (B->IsType11FourSingle(iCs) == 1)
			{
			    DeleteB(iCs,2);
			    Type6_11(iCs,1);
				return;
			}
		if (B->IsType11FourDouble(iCs) == 1)
			{
				DeleteB(iCs, 2);
				Type6_11(iCs, 1);
				return;
			}
		}
	}

void PokerModelAnalysis::Random(int *iBuffer)
{
	int i = 0;
	int j = 0;
	int one = 0;
	int two = 0;
	int k = 0;
	while (iOneDe[j] != -1)////只要满足上家必有的牌值不等于－1，就循环啊循环，也就是有值的时候
	{
		j++;////其实就是算上家必有的牌的张数
	}
	if (j > pMyDdz->iUpPlayerCards)////如果上家必有的牌大于上家手中的牌数，就把j的值又改为0,就相当于知道所有的牌
	{
		j = 0;
	}
	one = pMyDdz->iUpPlayerCards - j;////如过不满足上家必有的牌大于上家手牌，就将两者的差赋给上家计数值

	j = 0;////完成了上家的部分后，就又置零，开始对于下家的计算

	while (iTwoDe[j] != -1)////同样的内容，这个部分是对于下家的
	{
		j++;////计算下家必有的牌的张数
	}
	if (j > pMyDdz->iDownPlayerCards)////如果下家必有的牌大于下家手中的牌数，就把j的值又改为0,就相当于知道所有的牌
		j = 0;
	two = pMyDdz->iDownPlayerCards - j;////记录除了必有的牌还有多少牌要分给下家，这是手牌减去必有的牌，就是目前未知的牌的张数

	i = one + two;//用一个变量i来存放两家总共未知的牌张数

	std::vector<int> vi(i);//这是一个容器
	//srand(unsigned(time(NULL)));
	for (int n = 0; n < i; n++)
	{
		vi[n] = iBuffer[n];
	}
	random_shuffle(vi.begin(), vi.end());
	j = 0;
	vector<int>::iterator it;
	for (it = vi.begin(); it != vi.begin() + one; it++)//给上家随机分配
	{
		iOneUtimate[j] = *it;
		j++;
	}
	for (k = 0; iOneDe[k] != -1; k++)//把上家必有的牌扔进去
	{
		iOneUtimate[j] = iOneDe[k];
		j++;
	}
	j = 0;
	for (it = vi.begin() + one; it != vi.end(); it++)//给下家随机分配
	{
		iTwoUtimate[j] = *it;
		j++;
	}
	k = 0;
	while (iTwoDe[k] != -1)//把下家必有的牌扔进去
	{
		iTwoUtimate[j] = iTwoDe[k];
		j++;
		k++;
	}
}

/*-------------------------------------------------------------------------------------
*对手模型判断  总共的牌型的所有分支
**  编写者：杨丽颖    2017-07-07
** 此处没有Type0,判断为pass后就直接返回了。不需要再使用函数对缓冲数组进行修改
*///------------------------------------------------------------------------------------
void PokerModelAnalysis::Type1(int *iCs, int k)//双王
{
	int a = iCs[0] / 4;//与4取整，判断当前牌面，这是桌面上出出来的牌
	for (int i = 0; Buffer[i] != -1; i++)//依次循环缓冲数组中有值的元素
	{
		if (a == Buffer[i] / 4){//当此时桌面上的这一张牌和缓冲数组中的一张牌相等时
			if (k == 2)//执行上家部分
			{
				int j = 0;//初始化一个j变量
				while (iOneDe[j] != -1)//只要上家必有的牌数组中有元素，就一直循环
				{
					j++;//这样计算上家必有的牌的张数，并将位置移到没有值得地方去，方便后面直接存入元素
				}
				iOneDe[j] = Buffer[i];//循环完了就到了空的 可以存新元素的位置了
				//然后就将这张已经确定的牌放入上家必有的牌数组当中
				for (int x = i; Buffer[x] != -1; x++)
				{
					Buffer[x] = Buffer[x + 1];//将这张已经出了的牌从缓冲数组中删除，元素依次往前移
				}
				i--;//缓冲数组长度减1
			}
			if (k == 1)//内容一致，只是操作数组不同，此处操作下家必有的牌数组
			{
				int j = 0;
				while (iTwoDe[j] != -1)
				{
					j++;
				}
				iTwoDe[j] = Buffer[i];
				for (int x = i; Buffer[x] != -1; x++)
				{
					Buffer[x] = Buffer[x + 1];
				}
				i--;
			}
		}
	}
}
void PokerModelAnalysis::Type2(int *iCs, int k)//炸弹
{
	int a = iCs[0] / 4;//同样判断此时桌面牌的牌面值
	for (int i = 0; Buffer[i] != -1; i++)//依次循环缓冲数组中的元素
	{
		if (a == Buffer[i] / 4)//当此时桌面上的牌和缓冲数组中的牌相同
		{
			if (k == 2)
			{
				int j = 0;
				while (iOneDe[j] != -1)
				{
					j++;//计数，并找取位置
				}
				iOneDe[j] = Buffer[i];//将满足条件的缓冲数组中的值赋给上家必有的牌
				for (int x = i; Buffer[x] != -1; x++)
				{
					Buffer[x] = Buffer[x + 1];//然后将赋了值的元素从缓冲数组中删除，其余元素依次往前移
				}
				i--;//数组长度减一
			}
			if (k == 1)//下家部分
			{
				int j = 0;
				while (iTwoDe[j] != -1)
				{
					j++;
				}
				iTwoDe[j] = Buffer[i];
				for (int x = i; Buffer[x] != -1; x++)
				{
					Buffer[x] = Buffer[x + 1];
				}
				i--;
			}
		}
	}
}
void PokerModelAnalysis::Type3(int *iCs, int k)//单牌
{
	     int a = iCs[0] / 4;
	for (int i = 0; Buffer[i] != -1; i++)//循环那个buffer数组，只要有值就一直循环，每一个元素都访问一遍
	{
		if (a == Buffer[i] / 4)//当之前判断的牌和缓冲数组里面的牌以此比较
		{
			if (k == 2)//等于2的时候是上家
			{
				int j = 0;
				while (iOneDe[j] != -1)
				{
					j++;//找到可以放牌的位置
				}
				iOneDe[j] = Buffer[i];//因为此时牌是相同的所以将缓冲数组中的牌放入上家必有的牌数组中
				for (int x = i; Buffer[x] != -1; x++)
				{
					Buffer[x] = Buffer[x + 1];//元素往前移
				}
				i--;//把提取出来的数据删除掉，然后数组长度就减1
				break;
			}
			if (k == 1)//等于1的时候是下家
			{
				int j = 0;
				while (iTwoDe[j] != -1)//只要不为－1，就一直往数组后续走
				{
					j++;//计算下家必有的牌的张数
				}
				iTwoDe[j] = Buffer[i];//将缓冲数组的值赋给下家必有的牌，是因为这一张已经判断相等了，所以放到下家必有的牌数组中
				for (int x = i; Buffer[x] != -1; x++)//内容同上面的一样
				{
					Buffer[x] = Buffer[x + 1];//还是往前移
				}
				i--;//数组长度减1
				break;
			}
		}
	}
	Random(Buffer);//是为了把数组打乱
}
void PokerModelAnalysis::Type4(int *iCs, int k){//对牌的情况
	
		int a = iCs[0] / 4;
		int count = 0;
		for (int i = 0; Buffer[i] != -1; i++)
		{
			if (a == Buffer[i] / 4)
			{
				if (k == 2)
				{
					int j = 0;
					while (iOneDe[j] != -1)
					{
						j++;
					}
					iOneDe[j] = Buffer[i];
					for (int x = i; Buffer[x] != -1; x++)
					{
						Buffer[x] = Buffer[x + 1];
					}
					i--;
					count++;
				    if(count==2) break;					
				}
				if (k == 1)
				{
					int j = 0;
					while (iTwoDe[j] != -1)
					{
						j++;
					}
					iTwoDe[j] = Buffer[i];
					for (int x = i; Buffer[x] != -1; x++)
					{
						Buffer[x] = Buffer[x + 1];
					}
					i--;
					count++;
					if (count == 2) break;
				}
			}
		}
}
void PokerModelAnalysis::Type5(int *iCs, int k)//三条
{
	int a = iCs[0] / 4;
	int count = 0;
	for (int i = 0; Buffer[i] != -1; i++)
	{
		if (a == Buffer[i] / 4)
		{
			if (k == 2)
			{
				int j = 0;
				while (iOneDe[j] != -1)
				{
					j++;
				}
				iOneDe[j] = Buffer[i];
				for (int x = i; Buffer[x] != -1; x++)
				{
					Buffer[x] = Buffer[x + 1];
				}
				i--;
				count++;
				if (count == 3) break;
			}
			if (k == 1)
			{
				int j = 0;
				while (iTwoDe[j] != -1)
				{
					j++;
				}
				iTwoDe[j] = Buffer[i];
				for (int x = i; Buffer[x] != -1; x++)
				{
					Buffer[x] = Buffer[x + 1];
				}
				i--;
				count++;
				if (count == 3) break;
			}
		}
	}
}
void PokerModelAnalysis::Type6_11(int *iCs, int k)
{
	for (int i = 0;iCs[i] != -1;i++)
	{
		int a = iCs[i] / 4;
		for (int i = 0; Buffer[i] != -1; i++)
		{
			if (a == Buffer[i] / 4)
			{
				if (k == 2)
				{
					int j = 0;
					while (iOneDe[j] != -1)
					{
						j++;
					}
					iOneDe[j] = Buffer[i];
					for (int x = i; Buffer[x] != -1; x++)
					{
						Buffer[x] = Buffer[x + 1];
					}
					i--;
					break;
				}
				if (k == 1)
				{
					int j = 0;
					while (iTwoDe[j] != -1)
					{
						j++;
					}
					iTwoDe[j] = Buffer[i];
					for (int x = i; Buffer[x] != -1; x++)
					{
						Buffer[x] = Buffer[x + 1];
					}
					i--;
					break;
				}
			}
		}
	}
}

void PokerModelAnalysis::CardsValue(int *pCards)
{
	InitialSum = 0;//初始化和为0
	ConfirmCards(pCards);//调用确认牌函数

	if (iUtimate[0][0] != -1)//第一个位置有牌
	{
		InitialSum = InitialSum + 10;//增加牌力值，对王加10分
	}
	for (int i = 0; iUtimate[1][i] != -1; i++)//有没有有炸弹 有炸弹就往后推5格，前面四格放炸弹啊
	{
		if (iUtimate[1][i] == -2)//等于－2是什么状态，没有炸弹
		{
			i++;
			continue;
		}
		else
		{
			InitialSum = InitialSum + 9;//增加牌力值，炸弹加9分
			i = i + 4;//往后移，放炸弹，四个
		}
	}
	for (int i = 0; iUtimate[2][i] != -1; i = i + 4)//有没有三条 有三条往后推4格，就是三张相同的牌
	{
		InitialSum = InitialSum + 8;//增加牌力值，每循环一次就证明有一个三条，有一个加8分
	}
	for (int i = 0; iUtimate[3][i] != -1; i++)//飞机
	{
		int j;
		for (j = 0; iUtimate[3][j] != -2; j = j + 3)//这个for循环拿来干嘛，又没有内容，是不是应该将后面的计算牌力值的语句放进去
		{
		}
		InitialSum = InitialSum + 7;//根据j值和首值编码计算牌力值
		i = i + j;//是i跳到下一个三顺的区域
	}
	for (int i = 0; iUtimate[4][i] != -1; i++)//有没有顺子 有顺子就进入循环
	{
		int j;
		for (j = 0; iUtimate[4][j] != -2; j++)//有顺子 用J值来检测顺子的长度（至少5个）
		{
			//这个里面应该写什么啊
		}
		InitialSum = InitialSum + 6;//根据j值和首值编码计算牌力值
		i = i + j;//使i跳到下一个顺子的区域
	}
	for (int i = 0; iUtimate[5][i] != -1; i++)
	{
		int j;
		for (j = 0; iUtimate[5][j] != -2; j = j + 2)//判断连对
		{
		}
		InitialSum = InitialSum + 5;//根据j值和首值编码计算牌力值
		i = i + j;//使i跳到下一个连对的区域
	}
	for (int i = 0; iUtimate[6][i] != -1; i = i + 3)//有没有对子 有对子就往后推3格
	{
		InitialSum = InitialSum + 4;//增加牌力值
	}
	for (int i = 0; iUtimate[7][i] != -1; i = i + 2)//有没有单牌 有单牌就往后推2格
	{
		InitialSum = InitialSum + 3;//增加牌力值
	}
}

void PokerModelAnalysis::LordBest()//给地主分配最好的牌
{
	//ConfirmCards(pMyDdz->iOnOtherHand);
	//Undone
	int k = 0;
	if (k = 0)//k＝0是下家当地主吗
	{
		for (int i = 0; i < 37; i++)//除开我手上的牌，剩37张
		{
			if (i < 17)//然后前面的17张
			{
				iOneUtimate[i] = pMyDdz->iOnOtherHand[i];//将其他手上的牌的前面17张都赋给上家的牌
			}
			else
			{
				iTwoUtimate[k] = pMyDdz->iOnOtherHand[i];// 剩下的20张都赋给了下家
			}
		}
	}
	else//k不等于0的时候

	{
		for (int i = 0; i < 37; i++)
		{
			if (i < 20)
			{
				iOneUtimate[k] = pMyDdz->iOnOtherHand[i];
			}
			else
			{
				iTwoUtimate[i] = pMyDdz->iOnOtherHand[i];
				k++;
			}
		}
	}
}

void PokerModelAnalysis::PartnerBest()//给队友分配最好的牌
{
	int k = 0;
	if (k = 0)
	{
		for (int i = 0; i < 37; i++)
		{
			if (i < 20)
			{
				iTwoUtimate[i] = pMyDdz->iOnOtherHand[i];
			}
			else
			{
				iOneUtimate[k] = pMyDdz->iOnOtherHand[i];
				k++;
			}
		}
	}
	else
	{
		for (int i = 0; i < 37; i++)
		{
			if (i < 17)
			{
				iOneUtimate[i] = pMyDdz->iOnOtherHand[i];
			}
			else
			{
				iTwoUtimate[k] = pMyDdz->iOnOtherHand[i];
				k++;
			}
		}
	}
}

void PokerModelAnalysis::Equality()//给两家分配差不多的牌
{
	int k = 0;
	int j = 0;
	for (int i = 0; i < 37; i++)
	{
		if (i % 2 == 0 && k < 17)
		{
			iOneUtimate[k] = pMyDdz->iOnOtherHand[i];
			k++;
		}
		else
		{
			iTwoUtimate[j] = pMyDdz->iOnOtherHand[i];
			j++;
		}
	}
}

void PokerModelAnalysis::SaveCards(int j, int *pCards)
{
	int *abc = A->startSplitCard(pCards, j);
	int i;
	for (i = 0; abc[i] != -1; i++)
	{
		iUtimate[j][i] = abc[i];
	}
	if (abc[0] != -1)
	{
		DeleteCards(abc);
	}
}

void PokerModelAnalysis::DeleteCards(int *iResult)
{
	int k = 0;
	for (int i = 0; iResult[i] != -1; i++)
	{
		if (iResult[i] != -2)
		{
			for (k = 0; k < 21; k++)
			{
				if (iResult[i] == Hc[k])
				{
					for (; k < 20; k++)
					{
						Hc[k] = Hc[k + 1];
					}
					break;
				}
			}
		}
	}
}

void PokerModelAnalysis::ConfirmCards(int *pCards)
{
	SaveCards(0, pCards);
	SaveCards(1, pCards);
	SaveCards(2, pCards);
	SaveCards(3, pCards);
	SaveCards(4, pCards);
	SaveCards(5, pCards);
	SaveCards(6, pCards);
	SaveCards(62, pCards);
	SaveCards(7, pCards);
	SaveCards(8, pCards);
	SaveCards(9, pCards);
	SaveCards(10,pCards);
	SaveCards(102, pCards);
	SaveCards(11,pCards);
	SaveCards(1102, pCards);
}

void PokerModelAnalysis::DeleteB(int *iCs, int a)
{
	if (a == 2)//也是判断相等，然后将元素前移，同其他地方一样啊，有什么用
	{
		for (int i = 0; iTwoDe[i] != -1; i++)
		{
			for (int j = 0; iCs[j] != -1; j++)
			{
				if (iTwoDe[i] == iCs[j])
				{
					for (int k = i; iTwoDe[k] != -1; k++)
					{
						iTwoDe[k] = iTwoDe[k + 1];
					}
					i--;
					j = 0;
				}
			}
		}
		for (int i = 0; iOneDe[i] != -1; i++)
		{
			for (int j = 0; iCs[j] != -1; j++)
			{
				if (iOneDe[i] == iCs[j])
				{
					for (int k = i; iOneDe[k] != -1; k++)
					{
						iOneDe[k] = iOneDe[k + 1];
					}
					UpWrong++;
					i--;
					j = 0;
				}
			}
		}
	}
	if (a == 1)
	{
		for (int i = 0; iTwoDe[i] != -1; i++)
		{
			for (int j = 0; iCs[j] != -1; j++)
			{
				if (iTwoDe[i] == iCs[j])
				{
					for (int k = i; iTwoDe[k] != -1; k++)
					{
						iTwoDe[k] = iTwoDe[k + 1];
					}
					DownWrong++;
					i--;
					j = 0;
				}
			}
		}
		for (int i = 0; iOneDe[i] != -1; i++)
		{
			for (int j = 0; iCs[j] != -1; j++)
			{
				if (iOneDe[i] == iCs[j])
				{
					for (int k = i; iOneDe[k] != -1; k++)
					{
						iOneDe[k] = iOneDe[k + 1];
					}
					i--;
					j = 0;
				}
			}
		}
	}
}

void PokerModelAnalysis::Fendipai()
{
	if (pMyDdz->cLandlord + 1 == pMyDdz->cDir)////地主是我方上家，就是地主的方位再顺时针移动一个，等于我所在的方位，所以我的上家就是地主
	{
		for (int i = 0; i < 3; i++)
		{
			iOneDe[i] = pMyDdz->iLef[i];////依次将3张底牌放入上家必有的牌数组中
		}
		for (int i = 0; Buffer[i] != -1; i++)////只要缓冲区数组的值不为－1，就一直循环，二维数组的循环,是找到－1的位置后来放这三张底牌
		{
			for (int j = 0; j < 3; j++)////此处是循环三张底牌
			{
				if (Buffer[i] == pMyDdz->iLef[j])////当底牌数组中的元素等于缓冲区数组的元素时
				{
					for (int k = i; Buffer[k] != -1; k++)
					{
						Buffer[k] = Buffer[k + 1];////往前移
					}
					i--;
				}
			}
		}
	}
	if (pMyDdz->cLandlord - 1 == pMyDdz->cDir)////同理，当我的下家是地主的情况
	{
		for (int i = 0; i < 3; i++)////依次将3张底牌放入下家必有的牌数组中
		{
			iTwoDe[i] = pMyDdz->iLef[i];
		}
		for (int i = 0; Buffer[i] != -1; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (Buffer[i] == pMyDdz->iLef[j])
				{
					for (int k = i; Buffer[k] != -1; k++)
					{
						Buffer[k] = Buffer[k + 1];
					}
					i--;
				}
			}
		}
	}
}

void PokerModelAnalysis::DeleteC()
{
	for (int i = 0; iOneDe[i] != -1; i++)
	{
		for (int j = 0; Buffer[j] != -1; j++)
		{
			if (Buffer[j] == iOneDe[i])//二重循环嘛，当上家必有的牌和缓冲区的牌相等时，就将缓冲区的元素依次往前移
			{
				for (int k = j; Buffer[k] != -1; k++)
				{
					Buffer[k] = Buffer[k + 1];
				}
				i--;
				j = 0;
			}
		}
	}
	for (int i = 0; iTwoDe[i] != -1; i++)//内容同上，此处是针对下家而言的
	{
		for (int j = 0; Buffer[j] != -1; j++)
		{
			if (Buffer[j] == iTwoDe[i])
			{
				for (int k = j; Buffer[k] != -1; k++)
				{
					Buffer[k] = Buffer[k + 1];
				}
				i--;
				j = 0;
			}
		}
	}
}


/**
*  作者：祝瑾如  2017.7.12
*  初始化概率表
*/
void PokerModelAnalysis::Gailvbiao(int *Buffer)/*概率表的初始化*/
{
	/*共15张牌，3,4,5,6,7,8,9,10,J,Q,K,A,2,小王,大王。共0,1,2,3,4五种情况。*/
	
	int One[15] = { 0 };/*One为上家,存储除底牌外牌的张数*/
	int Dipai[15] = {0};/*装三张底牌,下标是底牌，值为底牌的张数*/
	int n; /*n为总张数=底牌+Buffer*/

	 /*计算buffer中分别多少张*/
	for (int i1 = 0; i1 < 34/*Buffer[i1]!=-1*/; i1++)
	{
		int j1;
		j1 = Buffer[i1] / 4;
		if (Buffer[i1] ==52) 
		{
			One[13]++;
		}
		if (Buffer[i1] == 53)
		{
			One[14]++;
		}
		else
		{
	      One[j1]++;
		}
	}

	/*计算底牌分别多少张*/
		for (int j2 = 0; j2< 3; j2++)
		{
			int i2 = pMyDdz->iLef[j2] / 4;
			if (pMyDdz->iLef[j2] ==52)
			{
				Dipai[13]++;
			}
			if (pMyDdz->iLef[j2] == 53)
			{
				Dipai[14]++;
			}
			else
			{
				Dipai[i2]++;
			}
		}


	if (pMyDdz->cLandlord + 1 == pMyDdz->cDir) /*地主是上家*/
	{
			/*概率表初始化*/
			for (int i3 = 0; i3<15; i3++)
			{
				n = One[i3]+Dipai[i3];
				int d = Dipai[i3];
				for (int j3 = 0; j3 < 5; j3++)
				{
					if (j3 <= n)
					{
						if (j3==d&&j3!=0) 
					{
							BiaoA[i3][j3] = 0.85;/*把底牌张数的概率设为0.85*/
						}
						if (j3<d) 
						{
							BiaoA[i3][j3] = 0;
						}
						else
						{
							BiaoA[i3][j3] = 0.5;
						}
						BiaoB[i3][j3 - Dipai[i3]] = 0.5;
						if (One[i3] = 0)/*如果buffer中牌数等于0，即底牌中张数等于总牌数*/
						{
							BiaoA[i3][j3] = 1;
							BiaoB[i3][j3] = 0;
						}
						}
					else
					{
						BiaoA[i3][j3] = 0;
						BiaoB[i3][j3] = 0;
					}
					}
				}
			}

	if (pMyDdz->cLandlord == pMyDdz->cDir)/*地主是“我”*/
	{
		for (int i4 = 0; i4<15; i4++)
		{
			for (int j4 = 0; j4 < 5; j4++)
			{
				if (j4 <= One[i4])
				{
					BiaoA[i4][j4] = 0.5;
					BiaoB[i4][j4] = 0.5;
				}
				else
				{
					BiaoA[i4][j4] = 0;
					BiaoB[i4][j4] = 0;
				}
			}
		}
	}

	if (pMyDdz->cLandlord - 1 == pMyDdz->cDir)/*地主是下家*/
	{
		for (int i5 = 0; i5<15; i5++)
		{
			n = One[i5] + Dipai[i5];
			int d = Dipai[i5];
			for (int j5 = 0; j5 < 5; j5++)
			{
				if (j5 <= n)
				{
					if (d==j5)
					{
						BiaoB[i5][j5] = 0.85;/*把底牌张数的概率设为0.85*/
					}
					if (j5<d)
					{
						BiaoB[i5][j5] = 0;
					}
					else
					{
						BiaoB[i5][j5] = 0.5;
					}
					BiaoA[i5][j5 - Dipai[i5]] = 0.5;
					if (One[i5] = 0)/*如果buffer中牌数等于0，即底牌中张数等于总牌数*/
					{
						BiaoB[i5][j5] = 1;
						BiaoA[i5][j5] = 0;
					}
				}
				else
				{	
					BiaoB[i5][j5] = 0;
					BiaoA[i5][j5] = 0;
				}
			}
		}
	}
}

/**
*  作者：祝瑾如  2017.7.12
*  每次其他玩家出牌后，计算每个玩家手牌的概率
*/
void PokerModelAnalysis::Change(int *iCs, int t)/*概率表的变化*/
{
	int i,j;
	int a[15] = {0};/*存储此轮出牌的张数*/
	double g = 0.2;/*概率的变化值*/

	for (int i1 = 0; iCs[i1] != -1; i1++)
	{
		for (int j1 = 0; j1<15; j1++)
		{
			if (iCs[i1] / 4 == j1)
			{
				a[j1]++;
			}
		}
	}

	if (t==1) /*上家出牌概率表的变化*/
	{
		for (int i2=0;i2<15;i2++)
		{
			for (int j2 = 0; BiaoA[i2][j2] !=0|| BiaoB[i2][j2] != 0;j2++)/*分别找到概率表AB中最后一个有值的张数*/
			{
				if (BiaoA[i2][j2] != 0)
				{
					i = j2;/*找出概率表A中此行的张数最大值*/
				}
				if (BiaoB[i2][j2] != 0) 
				{
					j = j2;/*找出概率表B中此行的张数最大值*/
				}
			}

			if (i== a[i2]) /*出牌数等于概率表A中最大张数*/
			{

				if(i>=j)/*出牌数等于概率表A中最大张数且A的最大张数大于B的最大张数，A、B此行所有概率修改为0*/
				{
			while(i>=0)
				{
				BiaoA[i2][i] == 0;
				BiaoB[i2][i] == 0;
				i--;
				}
			}
				if (i<j)/*出牌数等于概率表A中最大张数但A的最大张数小于B的最大张数，A此行所有概率修改为0，B的j-i张数改为1,其他改为0*/
				{
					int c = j - i;
					BiaoB[i2][c] = 1;
					while (i >= 0)
					{
						BiaoA[i2][i] == 0;
						i--;
					}
					while (j>= 0)
					{
						if (j != c) 
						{
							BiaoB[i2][j] = 0;
						}
					}
				}
			}

			if (i>a[i2])/*A的出牌数小于概率表A中最大张数*/
			{
			 if (i>j)/*A的出牌数小于概率表A中最大张数且A的最大张数大于B的最大张数*/
				{
					if (BiaoA[i2][a[i2]]==0.85) /*出牌数恰好等于底牌数，则在剩下的牌中降低概率表A的概率增加概率表B的概率*/
					{
						int i3 = i - a[i2];
						for (int j3=i3;j3>=0;j3--) 
						{  
							if (j3 == i3) 
							{	BiaoA[i2][j3] += g;	}
							else 
							{BiaoA[i2][j3] -= g;}

							if (j3 == 0) 
							{ BiaoA[i2][j3] += g; }
							else
							{	BiaoA[i2][j3] -= g;}

							if (j3==a[i2]) 
							{	BiaoA[i2][j3] -=2*g;}
						}
					}
				}

			 if (i == j) /*A的出牌数小于概率表A中最大张数且A的最大张数等于于B的最大张数，说明A是地主*/
			 {
				 int i3 = i - a[i2];
				 for (; i3 >=0; i3--)
				 {
					 if (i3==0) 
					 {
						 BiaoA[i2][i3] += g;
						 BiaoB[i2][i3] -= g;
					 }
					 else
					 {
                     BiaoA[i2][i3] -=g ;
					 BiaoB[i2][i3] += g;
					 }
				 }
			 }

			 if (i<j)/*A的出牌数小于概率表A中最大张数且A的最大张数小于B的最大张数,说明B是地主*/
			 {
				 if (BiaoB[i2][j-a[i2]] ==0.85)/*除去A的出牌数后恰好是B拥有的底牌数，除了B的底牌数概率为1后其他AB概率都设为0*/
				 {
					 for (int i4 = 0; i4 >= 0;i4++) 
					 {
						 BiaoA[i2][i4] = 0;
						 if (i4==j-a[i2])
						 { BiaoB[i2][i4] = 1;}
						 else
						 { BiaoB[i2][i4] = 0; }
					 }
				 }
				 else
				 {
					 BiaoA[i2][0] += g;
					 BiaoB[i2][j-a[i2]] += g;
					 for (int i5 = i - a[i2]; i5 > 0;i5--) 
					 {
						 BiaoA[i2][i5] -= g;
					 }
					 for (int j5 = i - a[i2]-1; j5 >= 0; j5--)
					 {
						 BiaoA[i2][j5] -= g;
					 }
				 }
			 }
			}
		}
	}

	if (t==2) /*下家出牌概率表的变化*/
	{
		for (int i2 = 0; i2<15; i2++)
		{
			for (int j2 = 0; BiaoB[i2][j2] != 0 || BiaoA[i2][j2] != 0; j2++)/*分别找到概率表AB中最后一个有值的张数*/
			{
				if (BiaoB[i2][j2] != 0)
				{
					i = j2;/*找出概率表B中此行的张数最大值*/
				}
				if (BiaoA[i2][j2] != 0)
				{
					j = j2;/*找出概率表A中此行的张数最大值*/
				}
			}

			if (i == a[i2]) /*出牌数等于概率表B中最大张数*/
			{

				if (i >= j)/*出牌数等于概率表B中最大张数且B的最大张数大于A的最大张数，A、B此行所有概率修改为0*/
				{
					while (i >= 0)
					{
						BiaoB[i2][i] == 0;
						BiaoA[i2][i] == 0;
						i--;
					}
				}
				if (i<j)/*出牌数等于概率表B中最大张数但B的最大张数小于A的最大张数，A此行所有概率修改为0，B的j-i张数改为1,其他改为0*/
				{
					int c = j - i;
					BiaoA[i2][c] = 1;
					while (i >= 0)
					{
						BiaoB[i2][i] == 0;
						i--;
					}
					while (j >= 0)
					{
						if (j != c)
						{
							BiaoA[i2][j] = 0;
						}
					}
				}
			}

			if (i>a[i2])/*B的出牌数小于概率表B中最大张数*/
			{
				if (i>j)/*B的出牌数小于概率表B中最大张数且B的最大张数大于A的最大张数*/
				{
					if (BiaoB[i2][a[i2]] == 0.85) /*出牌数恰好等于底牌数，则在剩下的牌中降低概率表B的概率增加概率表B的概率*/
					{
						int i3 = i - a[i2];
						for (int j3 = i3; j3 >= 0; j3--)
						{
							if (j3 == i3)
							{
								BiaoB[i2][j3] += g;
							}
							else
							{
								BiaoB[i2][j3] -= g;
							}

							if (j3 == 0)
							{
								BiaoB[i2][j3] += g;
							}
							else
							{
								BiaoB[i2][j3] -= g;
							}

							if (j3 == a[i2])
							{
								BiaoB[i2][j3] -= 2 * g;
							}
						}
					}
				}

				if (i == j) /*B的出牌数小于概率表B中最大张数且B的最大张数等于A的最大张数，说明B是地主*/
				{
					int i3 = i - a[i2];
					for (; i3 >= 0; i3--)
					{
						if (i3 == 0)
						{
							BiaoB[i2][i3] += g;
							BiaoA[i2][i3] -= g;
						}
						else
						{
							BiaoB[i2][i3] -= g;
							BiaoA[i2][i3] += g;
						}
					}
				}

				if (i<j)/*B的出牌数小于概率表B中最大张数且B的最大张数小于A的最大张数,说明A是地主*/
				{
					if (BiaoA[i2][j - a[i2]] == 0.85)/*除去B的出牌数后恰好是A拥有的底牌数，除了A的底牌数概率为1后其他AB概率都设为0*/
					{
						for (int i4 = 0; i4 >= 0; i4++)
						{
							BiaoB[i2][i4] = 0;
							if (i4 == j - a[i2])
							{
								BiaoA[i2][i4] = 1;
							}
							else
							{
								BiaoA[i2][i4] = 0;
							}
						}
					}
					else
					{
						BiaoB[i2][0] += g;
						BiaoA[i2][j - a[i2]] += g;
						for (int i5 = i - a[i2]; i5 > 0; i5--)
						{
							BiaoB[i2][i5] -= g;
						}
						for (int j5 = i - a[i2] - 1; j5 >= 0; j5--)
						{
							BiaoB[i2][j5] -= g;
						}
					}
				}
			}
		}
	}

	for (int i6=0;i6<15;i6++)
	{
		for (int j6 = 0; j6 < 5; j6++)
		{
			if (BiaoA[i6][j6] <= 0) 
			{
				BiaoA[i6][j6] <= 0;
			}
			if (BiaoA[i6][j6] >= 1)
			{
				BiaoA[i6][j6] = 1;
			}
			if (BiaoB[i6][j6] <= 0)
			{
				BiaoB[i6][j6] <= 0;
			}
			if (BiaoB[i6][j6] >= 1)
			{
				BiaoB[i6][j6] = 1;
			}
		}
	}
}