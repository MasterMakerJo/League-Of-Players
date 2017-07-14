#define kPlayerName "参赛选手名称"
#define kPlaMax 500
#include "BoardMessageControl.h"
void TestRecommendCard();
int	main()
{
	struct Ddz tDdz, *pDdz = &tDdz;
	BoardMessageControl * pBoardControl = new BoardMessageControl();
	pBoardControl->InitTurn(pDdz);			//初始化数据
	while (pDdz->iStatus != 0)
	{
		pBoardControl->InputMsg(pDdz);			//输入信息
		pBoardControl->AnalyzeMsg(pDdz);		//分析处理信息
		pBoardControl->OutputMsg(pDdz);		//输出信息
		pBoardControl->CalOthers(pDdz);		//计算其它数据
	}
	delete(pBoardControl);
	return 0;
}
	