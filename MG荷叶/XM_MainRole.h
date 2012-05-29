#ifndef _XM_MAINROLE_H
#define _XM_MAINROLE_H
#include "xm_sprite.h"
enum CHECKTYPE
{
	CHECK_COL,
	CHECK_WIN,
	CHECK_COUNT
};
enum DIRECTION
{
	DIR_NODIRECTION,
	DIR_EAST,
	DIR_SOUTH,
	DIR_WEST,
	DIR_NORTH,
};
class XM_MainRole :
	public XM_sprite
{
public:


	XM_MainRole();
	~XM_MainRole();
	void Update(double ElpasedTime);//更新位置
	//碰撞检测
	bool Check(double x,double y,double ElpasedTime);//返回true表示赢
	//边界检查

	void SetDirection(DIRECTION);
	virtual void Draw();
	void CreateLeaf(LPDIRECT3DDEVICE9	 g_pDevice,const char* filename,int width,int height);
	void ClearLeaf();
	void SetActive(DIRECTION);
	//获取叶子的范围
	RECT getLeafRect(DIRECTION);
private:
	DIRECTION _direct;
	XM_sprite* _activeLeaf;
	XM_sprite* _eastLeaf;
	XM_sprite* _westLeaf;
	XM_sprite* _northLeaf;
	XM_sprite* _southLeaf;
	bool _bOnCol;//是否在柱子上
	bool _bNearCol;//荷叶被柱子挡住
	double _dspeed;

};
#endif