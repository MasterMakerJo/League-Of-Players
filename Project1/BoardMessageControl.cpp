#include "BoardMessageControl.h"
#include<string>
#include "windows.h"


int aiBid;
void printCard(int*);
int who = 0;//0表示上家为地主，1表示AI,2表示下家
int who2 = 0;

using namespace std;
BoardMessageControl::BoardMessageControl()
{
	//Test
	Pma = new PokerModelAnalysis();
}


BoardMessageControl::~BoardMessageControl()
{
	delete(engine);
}
void BoardMessageControl::InitTurn(struct Ddz * pDdz)
{
	engine = new MonteCarloSearchEngine();
	CInfo = CardsInfo::Instance();
	pDdz->iTurnTotal = 1;				//初始化总轮数
	pDdz->iTurnNow = 1;					//初始化当前轮次
	pDdz->iRoundTotal = 1;				//初始化总局数
	pDdz->iRoundNow = 1;				//初始化当前局次
	InitRound(pDdz);					//初始化本局数据
}
//P01-END

//M04-START重置本局数据初始值
//最后修订者:范晓梦 最后修订时间:16-07-31
//修改目的:otherPlayerCards初始化
void BoardMessageControl::InitRound(struct Ddz * pDdz)
{
	int i, j;
	pDdz->iStatus = 1;					//初始化本局引擎状态
	strcpy_s(pDdz->sCommandIn, "");		//初始化本局通信输入内容
	strcpy_s(pDdz->sCommandOut, "");		//初始化本局通信输出内容
	for (i = 0; i < 21; i++)
	{		//初始化本局手牌
		pDdz->iOnHand[i] = -1;
	}
	//初始化otherPlayerCards
	for (i = 0; i < 54; i++)
	{
		pDdz->iOnOtherHand[i] = i;
	}
	pDdz->iOnOtherHand[54] = -1;
	for (i = 0; i<162; i++)				//初始化本局桌面牌
		for (j = 0; j<21; j++)
			pDdz->iOnTable[i][j] = -2;
	for (i = 0; i < 21; i++)			//初始化本局出牌
		pDdz->iToTable[i] = -1;
	strcpy_s(pDdz->sVer, "");				//初始化本局协议版本号
	strcpy_s(pDdz->sVer, kPlayerName);		//初始化本局参赛选手称呼
	pDdz->cDir = 'B';						//初始化本局玩家方位编号
	pDdz->cLandlord = 'B';				//初始化本局地主方位编号
	pDdz->cWinner = 'B';					//初始化本局胜者方位编号
	for (i = 0; i < 3; i++)				//初始化本局叫牌
		pDdz->iBid[i] = -1;
	pDdz->iLastPassCount = 2;		//当前桌面连续PASS数（值域[0,2],初值2，正常出牌取0，一家PASS取1，两家PASS取2）
	pDdz->iLastTypeCount = 0;		//当前桌面牌型张数（值域[0,1108],初值0，iLastPassCount=0时更新值，=1时保留原值，=2时值为0）
	pDdz->iLastMainPoint = -1;		//当前桌面主牌点数（值域[0,15],初值-1，iLastPassCount=0时更新值，，=1时保留原值，=2时值为-1）
	pDdz->iBmax = 0;					//初始化本局叫牌分数
	pDdz->iOTmax = 0;					//初始化本局桌面牌手数
}
//MO4-END

//P02-START输入信息
//最后修订者:梅险,最后修订时间:15-02-08
void BoardMessageControl::InputMsg(struct Ddz * pDdz)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	//cin.getline(pDdz->sCommandIn, 80);
	cout <<endl<< " 开始斗地主 （输入出牌请用空格隔开,只输入0为PASS,J Q K A 小王 大王分别为11 12 13 1 14 15,最后用0结束）" << endl<<endl;
 {
		//生成牌
		int out[55];
		int temp[55];
		srand((unsigned)time(NULL));//以时间作为种子

		for (int i = 0; i < 54; i++)
		{
			temp[i] = i;//往数组里放进牌编号
		}
		for (int left = 53, i = 0; left>0; left--)//left为牌堆剩余牌数 
		{
			int b = rand() % left;//随机选取一张牌
			out[i++] = temp[b];
			if (b != left) {
				temp[b] = temp[left];
			}
		}
		out[53] = temp[0];


		/*for (int i = 0; i < 54; i++)
		{
			out[i] = i;
		}
		out[3] = 52;
		out[4] = 53;
		out[11] = 51;
		out[12] = 50;*/


		/*333 444 5 666 777 888 9
		int us[21] = {0,1,2,4,5,6,8,12,13,14,16,17,18,20,21,29,24,-1,-1,-1,-1};//我们的牌
		
		out[51] = 50;
		out[52] = 51;
		out[53] = 52;*/

		int us[21];//我们的牌
		int latter[21];//下家的牌
		int former[21];//上家的牌

		//初始化三个数组
		for (int i = 0; i < 21; i++)
		{
			us[i] = -1;
			latter[i] = -1;
			former[i] = -1;
		}

		//从随机生成的牌中取出各家的牌
		for (int i = 0; i < 51; i++)
		{
			if (i < 17) {
				us[i] = out[i];
			}
			else if(i<34){
				latter[i - 17] = out[i];
			}
			else {
				former[i - 34] = out[i];
			}
		}
		/*for (int i = 0; i < 18; i++)
		{
			cout << "us[" << i << "]=" << i << ";" << endl;
		}*/
		/*us[0] = 0;
		us[1] = 1;
		us[2] = 2;
		us[3] = 4;
		us[4] = 8;
		us[5] = 12;
		us[6] = 16;
		us[7] = 20;
		us[8] = 21;
		us[9] = 22;
		us[10] = 29;
		us[11] = 36;
		us[12] = 40;
		us[13] = 41;
		us[14] = 44;
		us[15] = 45;
		us[16] = 53;
		us[17] = -1;

		out[51] = 50;
		out[52] = 51;
		out[53] = 52;*/


		//用冒牌排序给三家的牌排个序
		for (int pass = 17,j; pass >0; pass--)
		{
			for (int i = 0; i < pass-1; i++)
			{
				if (us[i] > us[i + 1]) {
					j = us[i];
					us[i] = us[i + 1];
					us[i + 1] = j;
				}
				if (latter[i] > latter[i + 1]) {
					j = latter[i];
					latter[i] = latter[i + 1];
					latter[i + 1] = j;
				}
				if (former[i] > former[i + 1]) {
					j = former[i];
					former[i] = former[i + 1];
					former[i + 1] = j;
				}
			}
		}

	
		


		//把数字转换成字符串
		char sCard[4] = "";
		char sCardString[90] = "";
		strcat_s(sCardString, "DEAL B");
		for (int i = 0; i < 17; i++)
		{
			if (us[i] >= 10) {
				sCard[0] = us[i] / 10 + '0';
				sCard[1] = us[i] % 10 + '0';
				sCard[2] = ',';
				sCard[3] = '\0';
			}
			else {
				sCard[0] = us[i] % 10 + '0';
				sCard[1] = ',';
				sCard[2] = '\0';
			}
			strcat_s(sCardString, sCard);
		}
		sCardString[strlen(sCardString) - 1] = '\0';//去掉多余的尾部逗号
		strcat(pDdz->sCommandIn, sCardString);

		//输出三家的手牌
		cout << "AI的手牌  ";
		printCard(us);

		cout << "\n上家的手牌";
		printCard(former);
		
		cout << "\n下家的手牌";
		printCard(latter);

		cout << endl;

		//AI拿到牌后分析，存储，回答平台
		AnalyzeMsg(pDdz);
		OutputMsg(pDdz);


		cout << "上家叫分： ";
		int formerbid;
		cin >> formerbid;

		char a[2];
		a[0]= formerbid % 10 + '0';
		a[1] = '\0';
		strcpy(pDdz->sCommandIn,"BID A");
		strcat(pDdz->sCommandIn, a);
		pDdz->sCommandIn[6] = '\0';

		AnalyzeMsg(pDdz);
		OutputMsg(pDdz);


		cout << "\nAI叫分 ";
		strcpy(pDdz->sCommandIn, "BID WHAT");
		AnalyzeMsg(pDdz);
		OutputMsg(pDdz);


		cout << "\n下家叫分： ";
		int latterbid;
		cin >> latterbid;
		char aa[2];
		aa[1] = '\0';
		aa[0] = latterbid % 10 + '0';
		strcpy(pDdz->sCommandIn, "BID C");
		strcat(pDdz->sCommandIn, aa);
		pDdz->sCommandIn[6] = '\0';
		AnalyzeMsg(pDdz);
		OutputMsg(pDdz);


		//显示底牌，以及判断谁是地主 A B C
		int left[4];
		left[0] =out[51];
		left[1] = out[52];
		left[2] = out[53];
		left[3] = -1;

		//把底牌转换成字符串
		char info[90] = "";
		for (int i = 51; i < 54; i++)
		{
			if (out[i] >= 10) {
				sCard[0] = out[i] / 10 + '0';
				sCard[1] = out[i] % 10 + '0';
				sCard[2] = ',';
				sCard[3] = '\0';
			}
			else {
				sCard[0] = out[i] % 10 + '0';
				sCard[1] = ',';
				sCard[2] = '\0';
			}
			strcat_s(info, sCard);
		}

		if(strlen(info)>=1)
			info[strlen(info) - 1] = '\0';//去掉多余的尾部逗号

		

		if (aiBid >= latterbid && aiBid >= formerbid) {

			strcpy(pDdz->sCommandIn, "LEFTOVER B");
			strcat_s(pDdz->sCommandIn, info);
			
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << "								AI为地主，底牌为 ";
			
			printCard(left);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			AnalyzeMsg(pDdz);

			OutputMsg(pDdz);

			who = 1;
			who2 = 1;
			Pma->Gailvbiao(out, 2);
		
			
		}
		else if (latterbid >= aiBid && latterbid >= formerbid) {
			strcpy(pDdz->sCommandIn, "LEFTOVER C");
			strcat_s(pDdz->sCommandIn, info);

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << "								下家为地主，底牌为 ";
			
			printCard(left);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			AnalyzeMsg(pDdz);
			OutputMsg(pDdz);

			latter[17] = out[51];
			latter[18] = out[52];
			latter[19] = out[53];
			who = 2;
			who2 = 2;
			Pma->Gailvbiao(out, 3);
		}
		else if (formerbid >= aiBid && formerbid >= latterbid) {
			strcpy(pDdz->sCommandIn, "LEFTOVER A");
			strcat_s(pDdz->sCommandIn, info);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << "								上家为地主，底牌为 ";
			
			printCard(left);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			AnalyzeMsg(pDdz);
			OutputMsg(pDdz);

			former[17] = out[51];
			former[18] = out[52];
			former[19] = out[53];
			who = 0;
			who2 = 0;
			Pma->Gailvbiao(out, 1);
		}

		
		
		

		cout << "AI的手牌  ";
		printCard(pDdz->iOnHand);
		cout << "上家的手牌";
		printCard(former);
		cout << "下家的手牌";
		printCard(latter);





		SplitCard *pSplitCards = new SplitCard();
		int **result = pSplitCards->startSplitCard(pDdz->iOnHand);

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
					cout << result[j][i] / 4 + 3<<" ";
				}
				else
				{
					cout << ",";
				}
			}
			cout << endl << endl;
			}
		

		while (pDdz->iStatus != 0)
		{
			int outOfHand[21];//出的牌

			//初始化数组
			for (int i = 0; i < 21; i++)
			{
				outOfHand[i] = -1;
			}

			if (who == 0) {
				cout << "输入上家出牌： "<<endl;
				int a[21];
				for (int i = 0; i < 20; i++)
				{
					a[i] = -1;
				}

				int i = 0, b, k = 0;

				while (cin >> b) {
					if (b != 0 && b>0 && b<16) {
						a[i++] = b;
					}
					else {
						break;
					}
				}

				if (i == 0) {
					strcpy(pDdz->sCommandIn, "PLAY A-1");

					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);

					cout <<"								上家PASS  " << endl;
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
					//cout << " MSGtoAI " << pDdz->sCommandIn << endl;
					AnalyzeMsg(pDdz);		//分析处理信息
					OutputMsg(pDdz);		//输出信息

					who = 1;
					continue;
				}
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
				cout << "								上家出牌 ： ";
				

				for (int i = 0; a[i] != -1; i++) {
					if (a[i] == 1)cout << " A ";
					else if(a[i]==11)cout <<" J ";
					else if (a[i] == 12)cout << " Q ";
					else if (a[i] == 13)cout << " K ";
					else if (a[i] == 14)cout << " 小王 ";
					else if (a[i] == 15)cout << " 大王 ";
					else cout << a[i] << " ";
				}
				cout << endl;

				
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				
				/*cout << endl << "上家牌编码" << endl;
				for (int i = 0; former[i] != -1; i++)
				{
					cout << former[i] << " ";
				}*/

				for (int i = 0; a[i] != -1; i++) {
					for (int j = 0; former[j] != -1; j++) {
						if (former[j] / 4 == 0)
						{
							if (a[i] == 3) {
								outOfHand[k++] = former[j];
								//	cout << " find 3";
								former[j] = -1000;
								break;
								//cout << "catch 3";
							}
							//cout << 3 << " ";    //3
						}

						else if (former[j] / 4 == 1)
						{
							if (a[i] == 4) {
								outOfHand[k++] = former[j];
								//	cout << " find 4";
								former[j] = -1000;
								//cout << "catch 4";
								break;
							}
						}
						//cout << 4 << " ";   //4
						else if (former[j] / 4 == 2) {
							if (a[i] == 5) {
								outOfHand[k++] = former[j];
								//	cout << " find 5";
								former[j] = -1000;
								break;
								//cout << "catch 5";
							}
							//cout << 5 << " ";   //5
						}
						else if (former[j] / 4 == 3)
						{
							if (a[i] == 6) {
								outOfHand[k++] = former[j];
								//cout << " find 6";
								former[j] = -1000;
								break;
							}
							//cout << 6 << " ";    //6
						}
						else if (former[j] / 4 == 4) {
							if (a[i] == 7) {
								outOfHand[k++] = former[j];
								//cout << " find 7";
								former[j] = -1000;
								break;
							}
							//cout << 7 << " ";    //7
						}
						else if (former[j] / 4 == 5) {
							if (a[i] == 8) {
								outOfHand[k++] = former[j];
								//	cout << " find 8";
								former[j] = -1000;
								break;
							}
							//cout << 8 << " ";    //8
						}
						else if (former[j] / 4 == 6) {
							if (a[i] == 9) {
								outOfHand[k++] = former[j];
								//	cout << " find 9";
								former[j] = -1000;
								break;
							}
							//cout << 9 << " ";   //9
						}
						else if (former[j] / 4 == 7)
						{
							if (a[i] == 10) {
								outOfHand[k++] = former[j];
								//			cout << " find 10";
								former[j] = -1000;
								break;
							}
							//cout << 10 << " ";    //10
						}
						else if (former[j] / 4 == 8)
						{
							if (a[i] == 11) {
								outOfHand[k++] = former[j];
								//		cout << " find J";
								former[j] = -1000;
								break;
							}
							//cout << " J ";    //J
						}
						else if (former[j] / 4 == 9) {
							if (a[i] == 12) {
								outOfHand[k++] = former[j];
								//	cout << " find Q";
								former[j] = -1000;

								break;
							}
							//cout << " Q ";    //Q
						}
						else if (former[j] / 4 == 10) {
							if (a[i] == 13) {
								outOfHand[k++] = former[j];
								//			cout << " find K";
								former[j] = -1000;
								break;
							}
							//cout << " K ";   //K
						}
						else if (former[j] / 4 == 11)
						{
							if (a[i] == 1) {
								outOfHand[k++] = former[j];
								//			cout << " find A";
								former[j] = -1000;
								break;
							}
							//cout << " A ";    //A
						}
						else if (former[j] / 4 == 12)
						{
							if (a[i] == 2) {
								outOfHand[k++] = former[j];
								//				cout << " find 2";
								former[j] = -1000;
								break;
							}
							//cout << 2 << " ";   //2
						}
						else if (former[j] == 52)
						{
							if (a[i] == 14) {
								outOfHand[k++] = former[j];
								//					cout << " find 小王";
								former[j] = -1000;
								break;
							}
							//cout << " 小王 ";    //小王
						}
						else if (former[j] == 53)
						{
							if (a[i] == 15) {
								outOfHand[k++] = former[j];
								//					cout << " find 大王";
								former[j] = -1000;
								break;
							}
							//cout << " 大王 ";   //大王
						}
					}

				}

				/*cout << endl << "former out of hand code ";
				for (int i = 0; outOfHand[i]!=-1; i++)
				{
					cout << outOfHand[i] << " ";
				}*/

				/*for (int i = 0; i < 4; i++)
				{
					sCard[i] = '\0';
				}*/




				char info[90] = "";
				for (int i = 0; i <k; i++)
				{
					if (outOfHand[i] >= 10) {
						sCard[0] = outOfHand[i] / 10 + '0';
						sCard[1] = outOfHand[i] % 10 + '0';
						sCard[2] = ',';
						sCard[3] = '\0';
					}
					else {
						sCard[0] = outOfHand[i] % 10 + '0';
						sCard[1] = ',';
						sCard[2] = '\0';
					}
					strcat_s(info, sCard);
				}

				if (strlen(info) >= 1)
				info[strlen(info) - 1] = '\0';//去掉多余的尾部逗号

				strcpy(pDdz->sCommandIn, "PLAY A");
				strcat_s(pDdz->sCommandIn, info);

				//cout << " MSGtoAI " << pDdz->sCommandIn << endl;
				AnalyzeMsg(pDdz);		//分析处理信息
				OutputMsg(pDdz);		//输出信息
					who = 1;
					Pma->Change(outOfHand, 1);

			}
			else if (who == 1) {

			//	cout << pDdz->sCommandIn << endl;
				cout << "AI thinking..." << endl;
				
				strcpy(pDdz->sCommandIn, "PLAY WHAT");

			//	cout << " MSGtoAI " << pDdz->sCommandIn << endl;
				AnalyzeMsg(pDdz);		//分析处理信息
				OutputMsg(pDdz);		//输出信息

				who = 2;
			}
			else if (who == 2) {
					cout << "输入下家出牌： " << endl;
					int a[21];
					for (int i = 0; i < 20; i++)
					{
						a[i] = -1;
					}

					int i = 0, b, k = 0;

					while (cin >> b) {
						if (b != 0 && b>0 && b<16) {
							a[i++] = b;
						}
						else {
							break;
						}
					}

					if (i == 0) {
						strcpy(pDdz->sCommandIn, "PLAY C-1");
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
						cout << "								下家PASS " << endl;
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
						//cout << " MSGtoAI " << pDdz->sCommandIn << endl;
						AnalyzeMsg(pDdz);		//分析处理信息
						OutputMsg(pDdz);		//输出信息

						who = 0;
						continue;
					}

					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
					cout << "								下家出牌 ： ";
					for (int i = 0; a[i] != -1; i++) {
						if (a[i] == 1)cout << " A ";
						else if (a[i] == 11)cout << " J ";
						else if (a[i] ==12)cout << " Q ";
						else if (a[i] ==13)cout << " K ";
						else if (a[i] == 14)cout << " 小王 ";
						else if (a[i] == 15)cout << " 大王 ";
						else cout << a[i] << " ";
					}
					cout << endl;
					//printCard(a);
					/*for (int i = 0; a[i] != -1; i++) {
						cout << a[i] << " ";
					}*/
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				

					/*cout << endl<< "下家牌编码" << endl;
					for (int i = 0; latter[i]!=-1; i++)
					{
						cout << latter[i] << " ";
					}
					*/
					for (int i = 0; a[i] != -1; i++) {
						for (int j = 0; latter[j] != -1; j++) {
							if (latter[j] / 4 == 0)
							{
								if (a[i] == 3) {
									outOfHand[k++] = latter[j];
								//	cout << " find 3";
									latter[j] = -1000;
									break;
									//cout << "catch 3";
								}
								//cout << 3 << " ";    //3
							}

							else if (latter[j] / 4 == 1)
							{
								if (a[i] == 4) {
									outOfHand[k++] = latter[j];
								//	cout << " find 4";
									latter[j] = -1000;
									//cout << "catch 4";
									break;
								}
							}
							//cout << 4 << " ";   //4
							else if (latter[j] / 4 == 2) {
								if (a[i] == 5) {
									outOfHand[k++] = latter[j];
								//	cout << " find 5";
									latter[j] = -1000;
									break;
									//cout << "catch 5";
								}
								//cout << 5 << " ";   //5
							}
							else if (latter[j] / 4 == 3)
							{
								if (a[i] == 6) {
									outOfHand[k++] = latter[j];
									//cout << " find 6";
									latter[j] = -1000;
									break;
								}
								//cout << 6 << " ";    //6
							}
							else if (latter[j] / 4 == 4) {
								if (a[i] == 7) {
									outOfHand[k++] = latter[j];
									//cout << " find 7";
									latter[j] = -1000;
									break;
								}
								//cout << 7 << " ";    //7
							}
							else if (latter[j] / 4 == 5) {
								if (a[i] == 8) {
									outOfHand[k++] = latter[j];
								//	cout << " find 8";
									latter[j] = -1000;
									break;
								}
								//cout << 8 << " ";    //8
							}
							else if (latter[j] / 4 == 6) {
								if (a[i] == 9) {
									outOfHand[k++] = latter[j];
								//	cout << " find 9";
									latter[j] = -1000;
									break;
								}
								//cout << 9 << " ";   //9
							}
							else if (latter[j] / 4 == 7)
							{
								if (a[i] == 10) {
									outOfHand[k++] = latter[j];
						//			cout << " find 10";
									latter[j] = -1000;
									break;
								}
								//cout << 10 << " ";    //10
							}
							else if (latter[j] / 4 == 8)
							{
								if (a[i] == 11) {
									outOfHand[k++] = latter[j];
							//		cout << " find J";
									latter[j] = -1000;
									break;
								}
								//cout << " J ";    //J
							}
							else if (latter[j] / 4 == 9) {
								if (a[i] == 12) {
									outOfHand[k++] = latter[j];
								//	cout << " find Q";
									latter[j] = -1000;

									break;
								}
								//cout << " Q ";    //Q
							}
							else if (latter[j] / 4 == 10) {
								if (a[i] == 13) {
									outOfHand[k++] = latter[j];
						//			cout << " find K";
									latter[j] = -1000;
									break;
								}
								//cout << " K ";   //K
							}
							else if (latter[j] / 4 == 11)
							{
								if (a[i] == 1) {
									outOfHand[k++] = latter[j];
						//			cout << " find A";
									latter[j] = -1000;
									break;
								}
								//cout << " A ";    //A
							}
							else if (latter[j] / 4 == 12)
							{
								if (a[i] == 2) {
									outOfHand[k++] = latter[j];
					//				cout << " find 2";
									latter[j] = -1000;
									break;
								}
								//cout << 2 << " ";   //2
							}
							else if (latter[j] == 52)
							{
								if (a[i] == 14) {
									outOfHand[k++] = latter[j];
				//					cout << " find 小王";
									latter[j] = -1000;
									break;
								}
								//cout << " 小王 ";    //小王
							}
							else if (latter[j] == 53)
							{
								if (a[i] == 15) {
									outOfHand[k++] = latter[j];
				//					cout << " find 大王";
									latter[j] = -1000;
									break;
								}
								//cout << " 大王 ";   //大王
							}
						}

					}



				/*	cout << endl << "latter out of hand code ";
					for (int i = 0; outOfHand[i] != -1; i++)
					{
						cout << outOfHand[i] << " ";
					}*/


					char info[90] = "";

				/*	for (int i = 0; i < 4; i++)
					{
						sCard[i] = '\0';
					}
					*/
					for (int i = 0; i <k; i++)
					{
						if (outOfHand[i] >= 10) {
							sCard[0] = outOfHand[i] / 10 + '0';
							sCard[1] = outOfHand[i] % 10 + '0';
							sCard[2] = ',';
							sCard[3] = '\0';
						}
						else {
							sCard[0] = outOfHand[i] % 10 + '0';
							sCard[1] = ',';
							sCard[2] = '\0';
						}
						strcat_s(info, sCard);
					}

					if (strlen(info) >= 1)
					info[strlen(info) - 1] = '\0';//去掉多余的尾部逗号

					strcpy(pDdz->sCommandIn, "PLAY C");
					strcat_s(pDdz->sCommandIn, info);

					//cout << "final" << pDdz->sCommandIn << endl;
					//cout << " MSGtoAI " << pDdz->sCommandIn << endl;
					AnalyzeMsg(pDdz);		//分析处理信息
					OutputMsg(pDdz);		//输出信息
					Pma->Change(outOfHand, 2);
					who = 0;
				}
				
			cout << endl;
			cout << "AI的手牌   ";
			printCard(pDdz->iOnHand);
			cout << "上家的手牌 ";
			printCard(former);
			cout << "下家的手牌 ";
			printCard(latter);
			cout << endl;

			cout << "上家的牌数 ： " << pDdz->iUpPlayerCards << "  下家的牌数 ： " << pDdz->iDownPlayerCards;
			

			int usCardCount=0;
			for (int i = 0; pDdz->iOnHand[i]!=-1; i++)
			{
				usCardCount++;
			}

			cout << "    AI的牌数 : " << usCardCount ;

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
			if (who2 == 0)cout << "      上家为地主 "<<endl; else if (who2 == 1)cout << "     AI为地主 "<<endl;
			else if (who2 == 2)cout << "      下家为地主 " << endl;
			
		
			if (pDdz->iUpPlayerCards == 0) {
				cout << endl<<"  上家获胜  " << endl;

				strcpy(pDdz->sCommandIn, "GAMEOVER A");
				InitTurn(pDdz);

				AnalyzeMsg(pDdz);

				InitTurn(pDdz);
				break;
			}
			else if (pDdz->iDownPlayerCards == 0) {
				cout << endl << "  下家获胜  " << endl;
				strcpy(pDdz->sCommandIn, "GAMEOVER C");
				InitTurn(pDdz);
				AnalyzeMsg(pDdz);
				InitTurn(pDdz);
				break;
			}
			else if (usCardCount == 0) {
				cout << endl << "  AI获胜  " << endl;
				strcpy(pDdz->sCommandIn, "GAMEOVER B");
				InitTurn(pDdz);
				AnalyzeMsg(pDdz);
				InitTurn(pDdz);
				break;
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			//CalOthers(pDdz);		//计算其它数据
		}
		}
	


}	
//P02-END

void printCard(int*temp) {

	int j;
	int a=0;
	for (int i = 0; temp[i]!=-1; i++)
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
	cout << endl<<endl;
}


//P0301-START获取并处理版本信息DOU
//最后修订者:张洪民&梅险,最后修订时间:15-02-10 21:04
//修订内容及目的:修订kPlayerNmae
void BoardMessageControl::GetDou(struct Ddz * pDdz)
{
	int i;								//简单循环变量
	for (i = 0; pDdz->sCommandIn[i] != '\0'; i++)
		pDdz->sVer[i] = pDdz->sCommandIn[i];
	pDdz->sVer[i] = '\0';
	strcpy_s(pDdz->sCommandOut, "NAME ");
	strcat_s(pDdz->sCommandOut, kPlayerName);
}
//P0301-END



//P0302-START获取并处理轮局信息INF
//最后修订者:张晨&梅险,最后修订时间:15-02-10
void BoardMessageControl::GetInf(struct Ddz * pDdz)		//轮次信息处理函数(处理数字信息，对应写入对象成员变量中):输入INFO 1/4 1/9 9 2450     输出OK INFO
{
	char c;					//存当前字节信息
	int iCount = 0;			//记录数据个数
	int iTemp = 0;			//中间变量
	int iMessage[7] = { 0 };		//记录数据数组
	int i;
	for (i = 5; pDdz->sCommandIn[i] != '\0'; i++)
	{
		c = pDdz->sCommandIn[i];
		if (c >= '0' && c <= '9')											//当前字符为0-9
		{
			iTemp = iTemp * 10 + c - '0';
			iMessage[iCount] = iTemp;								//晋级选手数
		}
		if (c == ',')														//当前字符为逗号,
		{
			iCount++;
			iTemp = 0;
		}
	}
	
	
	pDdz->iTurnNow = iMessage[0];						//当前轮次
	pDdz->iTurnTotal = iMessage[1];						//总轮数
	pDdz->iRoundNow = iMessage[2];						//当前局次
	pDdz->iRoundTotal = iMessage[3];						//总局数
	pDdz->iLevelUp = iMessage[4];							//晋级数量
	pDdz->iScoreMax = iMessage[5];						//得分界限
	InitRound(pDdz);//初始化数据
	strcpy_s(pDdz->sCommandOut, "OK INFO");
}
//P0302-END

//P0303-START获取并处理牌套信息DEA
//最后修订者:范晓梦 最后修订时间:16-07-31 17:03
//修订内容及目的:增加初始化上手和下手玩家手牌数和编码,更新其他玩家手牌信息
void BoardMessageControl::GetDea(struct Ddz * pDdz)
{
	int i;			      //简单循环变量
	int iNow = 0;		  //当前处理牌序号
	int iCardId = 0;	//当前处理牌编码
	char c;			      //当前指令字符
	pDdz->cDir = pDdz->sCommandIn[5];     //获取本家AI方位编号
	if (pDdz->cDir == 'B')
	{
		cUpPlayer = 'A';
		cDownPlayer = 'C';
	}
	if (pDdz->cDir == 'A')
	{
		cUpPlayer = 'C';
		cDownPlayer = 'B';
	}
	if (pDdz->cDir == 'C')
	{
		cUpPlayer = 'B';
		cDownPlayer = 'A';
	}
	pDdz->cLastPlay = cUpPlayer;
	pDdz->iUpPlayerCards = 0;
	pDdz->iDownPlayerCards = 0;
	for (i = 0; i < 21; i++)//清理iOnhand[]
	{
		pDdz->iOnHand[i] = -1;
	}

	for (i = 6; pDdz->sCommandIn[i] != '\0'; i++)	//依次读取牌码指令
	{
		c = pDdz->sCommandIn[i];			      //c为当前指令字符
		if (c >= '0' && c <= '9')				        //当前字符为0-9
		{
			iCardId = iCardId * 10 + c - '0';
			pDdz->iOnHand[iNow] = iCardId;
		}
		if (c == ',')							            //当前字符为逗号,
		{
			iCardId = 0;
			iNow++;
		}
	}
	CInfo->SortById(pDdz->iOnHand);
	OtherPlayerCards(pDdz, pDdz->iOnHand);
	strcpy_s(pDdz->sCommandOut, "OK DEAL");  //回复信息
	CInfo->SortById(pDdz->iOnHand);  //iOnHand[]从小到大排序
}
//P0303-END

//I02-START计算己方叫牌策略接口
int BoardMessageControl::CalBid(struct Ddz * pDdz)
{
	CallNum *CNum = new CallNum();
	int iMyBid = CNum->CallCardAnalyze(pDdz);		//叫牌
	aiBid = iMyBid;
	delete(CNum);
	return iMyBid;
}
//P0304-START获取并处理叫牌信息BID
//最后修订者:李思寒&梅险,最后修订时间:15-02-08
void BoardMessageControl::GetBid(struct Ddz * pDdz)
{
	if (pDdz->sCommandIn[4] == 'W')					//如果输入信息为BID WHAT
	{
		strcpy_s(pDdz->sCommandOut, "BID _0");
		pDdz->sCommandOut[4] = pDdz->cDir;
		pDdz->iBid[pDdz->cDir - 'A'] = CalBid(pDdz);//调用叫牌函数
		pDdz->sCommandOut[5] = pDdz->iBid[pDdz->cDir - 'A'] + '0';
		pDdz->sCommandOut[6] = '\0';
		return;
	}
	if (pDdz->sCommandIn[4] >= 'A'&&pDdz->sCommandIn[4] <= 'C')  //输入信息为BID **
	{
		pDdz->iBid[pDdz->sCommandIn[4] - 'A'] = pDdz->sCommandIn[5] - '0';
		strcpy_s(pDdz->sCommandOut, "OK BID");
		return;
	}
}
//P0304-END

//P0305-START获取并处理底牌信息LEF
//最后修订者:杨洋&梅险,最后修订时间:15-02-08
void BoardMessageControl::GetLef(struct Ddz * pDdz)
{
	int i, iCount = 0;
	char c;
	pDdz->cLandlord = pDdz->sCommandIn[9];    //确定地主方
	pDdz->iLef[0] = 0;
	pDdz->iLef[1] = 0;
	pDdz->iLef[2] = 0;
	for (i = 10; pDdz->sCommandIn[i] != '\0'; i++)
	{
		c = pDdz->sCommandIn[i];
		if (c >= '0' && c <= '9')
			pDdz->iLef[iCount] = pDdz->iLef[iCount] * 10 + c - '0';
		else
			iCount++;
	}
	if (pDdz->sCommandIn[9] == pDdz->cDir)
	{
		pDdz->iOnHand[17] = pDdz->iLef[0];
		pDdz->iOnHand[18] = pDdz->iLef[1];
		pDdz->iOnHand[19] = pDdz->iLef[2];
		pDdz->iOnHand[20] = -1;
		OtherPlayerCards(pDdz, pDdz->iLef);
		pDdz->iUpPlayerCards = 17;
		pDdz->iDownPlayerCards = 17;
	}
	if (pDdz->sCommandIn[9] == cUpPlayer)
	{
		pDdz->iUpPlayerCards = 20;
		pDdz->iDownPlayerCards = 17;
	}
	if (pDdz->sCommandIn[9] == cDownPlayer)
	{
		pDdz->iUpPlayerCards = 17;
		pDdz->iDownPlayerCards = 20;
	}
	if (pDdz->sCommandIn[9] == pDdz->cDir)
		strcpy_s(pDdz->sCommandOut, "OK LEFTOVER");
	CInfo->SortById(pDdz->iOnHand);					//iOnHand[]从小到大排序
}
//P0305-END

//P0305-START 申请出牌
//最后修订者:范晓梦,最后修订时间:16-07-31
//修订内容:更新iOnOtherHand
void BoardMessageControl::CalPla(struct Ddz * pDdz)
{
	//int iMax = 0;
	//SearchEngine *engine = new SearchEngine();

	///////清空itoTable/////
	memset(pDdz->iToTable,-1,sizeof(pDdz->iToTable));
	///////////////
	engine->SearchAGoodMove(pDdz);
	int i=0;
	//更新己方要出的牌
	CInfo->SortById(engine->ibestMove);
	for (i = 0; engine->ibestMove[i] >= 0; i++)
		pDdz->iToTable[i] = engine->ibestMove[i];
	pDdz->iToTable[i] = -1;//以-1作为间隔.


}
//P030602-START根据己方出牌更新数据
//最后修订者:梅险&夏侯有杰,最后修订时间:15-03-01
//修订内容及目的:修改减少手中牌
void BoardMessageControl::UpdateMyPla(struct Ddz * pDdz)
{
	int i, j, k;
	if (pDdz->iToTable[0] == -1)	//Pass
	{
		pDdz->iOnTable[pDdz->iOTmax][0] = -1;
		pDdz->iLastPassCount++;
		if (pDdz->iLastPassCount >= 2)	//连续两家PASS
		{
			pDdz->iLastPassCount = 0;
			pDdz->iLastTypeCount = 0;
			pDdz->iLastMainPoint = -1;
		}
		pDdz->iOTmax++;
	}
	else						//不是PASS
	{
		//增加桌面牌
		for (i = 0; pDdz->iToTable[i] >= 0; i++)
			pDdz->iOnTable[pDdz->iOTmax][i] = pDdz->iToTable[i];
		pDdz->iOnTable[pDdz->iOTmax][i] = -1;
		pDdz->iOTmax++;
		//减少手中牌

		for (j = 0; pDdz->iToTable[j] >= 0; j++)
		{
			for (i = 0; pDdz->iOnHand[i] >= 0; i++)
				if (pDdz->iOnHand[i] == pDdz->iToTable[j])
				{
					for (k = i; pDdz->iOnHand[k] >= 0; k++)
						pDdz->iOnHand[k] = pDdz->iOnHand[k + 1];
					break;
				}
		}
		pDdz->iLastPassCount = 0;
		pDdz->iLastTypeCount = CInfo->AnalyzeTypeCount(pDdz->iToTable);
		pDdz->iLastMainPoint = CInfo->AnalyzeMainPoint(pDdz->iToTable);
		pDdz->cLastPlay = pDdz->cDir;
	}
}
//PO30602-END

//P030603-START根据他人出牌更新数据
//最后修订者:范晓梦,最后修订时间:16-07-31
//修订内容：记录牌到iOnOtherHand
void BoardMessageControl::UpdateHisPla(struct Ddz * pDdz)
{
	int i;
	int iCardId;
	int iNow;
	char c;
	pDdz->cLastPlay = pDdz->sCommandIn[5];
	//减少手中牌
	if (pDdz->sCommandIn[6] == '-')// PLAY ?-1 即PASS
	{
		pDdz->iOnTable[pDdz->iOTmax][0] = -1;
		pDdz->iLastPassCount++;
		if (pDdz->iLastPassCount >= 2)	//连续两家PASS
		{
			pDdz->iLastPassCount = 0;
			pDdz->iLastTypeCount = 0;
			pDdz->iLastMainPoint = -1;
		}
		pDdz->iOTmax++;
	}
	else						// PLAY 出牌
	{
		for (i = 0; i<21; i++)							//清理iToTable[]
			pDdz->iToTable[i] = -1;
		iCardId = 0;
		iNow = 0;
		for (i = 6; pDdz->sCommandIn[i] != '\0'; i++)		//依次读取牌码
		{
			c = pDdz->sCommandIn[i];					//c为当前指令字符
			if (c >= '0' && c <= '9')				        //当前字符为0-9
			{
				iCardId = iCardId * 10 + c - '0';
				pDdz->iToTable[iNow] = iCardId;
			}
			if (c == ',')									//当前字符为逗号,
			{
				iCardId = 0;
				iNow++;
			}
		}
		//增加桌面牌,减少iOnOtherHand牌
		for (i = 0; pDdz->iToTable[i] >= 0; i++)
		{
			pDdz->iOnTable[pDdz->iOTmax][i] = pDdz->iToTable[i];
		}
		pDdz->iOnTable[pDdz->iOTmax][i] = -1;
		if (pDdz->cLastPlay == cUpPlayer)
			pDdz->iUpPlayerCards -= i;
		else if (pDdz->cLastPlay == cDownPlayer)
			pDdz->iDownPlayerCards -= i;

		OtherPlayerCards(pDdz, pDdz->iToTable);
		pDdz->iLastPassCount = 0;
		pDdz->iLastTypeCount = CInfo->AnalyzeTypeCount(pDdz->iToTable);
		pDdz->iLastMainPoint = CInfo->AnalyzeMainPoint(pDdz->iToTable);
		pDdz->iOTmax++;
	}
}
//PO30603-END
//P0306-START获取并处理出牌信息PLA
//最后修订者:梅险,最后修订时间:15-02-08
void BoardMessageControl::GetPla(struct Ddz * pDdz)
{
	if (pDdz->sCommandIn[5] == 'W')					//接收信息为PLAY WHAT：应调用出牌计算函数计算出牌
	{
		//cout << "calPla" << endl;
		CalPla(pDdz);					//调用出牌计算函数计算出牌
		strcpy_s(pDdz->sCommandOut, "PLAY _");
		pDdz->sCommandOut[5] = pDdz->cDir;		//输出命令的预备信息准备到sCommandOut数组

		CInfo->AppendCardsToS(pDdz->iToTable, pDdz->sCommandOut);		//要出牌数组iToTable[]中的数字转化为字符并连接到sCommandOut中
		
		//ADDtoDebug
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "								AI 出牌 ";
		printCard(pDdz->iToTable);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);


		UpdateMyPla(pDdz);		//根据己方出牌更新数据
	}
	else										//否则收到信息为其它玩家出牌
	{
		if (pDdz->sCommandIn[5] == pDdz->cDir)//数据输错
			return;
		UpdateHisPla(pDdz);		//根据他人出牌更新数据
		strcpy_s(pDdz->sCommandOut, "OK PLAY");//回复收到
	}
	//当前手数加1
	//pDdz->iOTmax++;
}
//P0306-END

//P0307-START获取并处理胜负信息GAM
//最后修订者:梅险,最后修订时间:15-02-08 00:13
void BoardMessageControl::GetGam(struct Ddz * pDdz)
{
	pDdz->cWinner = pDdz->sCommandIn[9];			//胜利者方位编号
	if (pDdz->iRoundNow == pDdz->iRoundTotal)		//如果当前局数与每轮局相等时
	{
		pDdz->iStatus = 0;							//引擎状态变量设为结束
	}
	else											//否则
	{
		pDdz->iRoundNow++;							//当前局次加1
		InitRound(pDdz);							//引擎状态变量设为重新开始下一局
	}
	strcpy_s(pDdz->sCommandOut, "OK GAMEOVER");
}
//PO307-END


//P03-START分析处理信息
//最后修订者:梅险,最后修订时间:15-02-08 00:13
void BoardMessageControl::AnalyzeMsg(struct Ddz * pDdz)
{
	char sShort[4];
	int i;
	for (i = 0; i<3; i++)
		sShort[i] = pDdz->sCommandIn[i];
	sShort[3] = '\0';
	if (strcmp(sShort, "DOU") == 0)					//版本信息
	{
		GetDou(pDdz);
		return;
	}
	if (strcmp(sShort, "INF") == 0)					//轮局信息
	{
		GetInf(pDdz);
		return;
	}
	if (strcmp(sShort, "DEA") == 0)					//牌套信息
	{
		GetDea(pDdz);
		return;
	}
	if (strcmp(sShort, "BID") == 0)					//叫牌过程
	{
		GetBid(pDdz);
		return;
	}
	if (strcmp(sShort, "LEF") == 0)					//底牌信息
	{
		GetLef(pDdz);
		return;
	}
	if (strcmp(sShort, "PLA") == 0)					//出牌过程
	{
		GetPla(pDdz);
		return;
	}
	if (strcmp(sShort, "GAM") == 0)					//胜负信息
	{
		GetGam(pDdz);
		return;
	}
	if (strcmp(sShort, "EXI") == 0)					//强制退出
	{
		exit(0);
	}
	strcpy_s(pDdz->sCommandOut, "ERROR at module AnalyzeMsg,sCommandIn without match");
	return;
}
//P03-END





//P04-START输出信息
//最后修订者:梅险,最后修订时间:15-02-08 00:13
void BoardMessageControl::OutputMsg(struct Ddz * pDdz)
{
	cout << pDdz->sCommandOut << endl;
}
//P04-END

//P05-START计算其它数据
//最后修订者:梅险,最后修订时间:15-02-08
void BoardMessageControl::CalOthers(struct Ddz * pDdz)
{
	pDdz->iVoid = 0;
}
//P05-END


//传入要删除的牌，计算两位玩家手中剩牌，并存储在iOnOtherHand中
//最后修订者：范晓梦
//最后修改时间：2016/7/31
void BoardMessageControl::OtherPlayerCards(Ddz *pDdz, int *iCards)
{
	auto deleteElem = [](int *iOnHand, int elem){
		int i = 0, j = 0;
		for (j = 0; j < 55; j++)
		{

			if (iOnHand[j] == elem)
				i++;
			else
			{
				iOnHand[j - i] = iOnHand[j];
			}
		}
	};
	int i, j, k;

	if (iCards == pDdz->iLef)
	{
		for (j = 0; j < 3; j++)
		{
			deleteElem(pDdz->iOnOtherHand, iCards[j]);
		}
	}
	else
	{
		for (j = 0; iCards[j] >= 0; j++)
		{
			deleteElem(pDdz->iOnOtherHand, iCards[j]);
		}
	}
}