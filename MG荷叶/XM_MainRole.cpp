#include "StdAfx.h"
XM_MainRole::XM_MainRole():
_direct(DIR_NODIRECTION),
_bOnCol(false),
_bNearCol(false),
_activeLeaf(NULL),
_dspeed(100.0)
{

}

XM_MainRole::~XM_MainRole()
{
	_DELETE(_eastLeaf);
	_DELETE(_westLeaf);
	_DELETE(_northLeaf);
	_DELETE(_southLeaf);
}
void XM_MainRole::Update(double ElpasedTime)
{	
	if(_bOnCol==false)
	{
		if(_direct == DIR_NODIRECTION)	return;
		else if(_direct%2 == 1)//东西
			_vPosition.x += static_cast<float>( ( 2 - _direct ) * ElpasedTime * _dspeed); 
		else//南北
			_vPosition.y += static_cast<float>( ( 3 - _direct ) * ElpasedTime * _dspeed); 

		if (_activeLeaf!=NULL&&_bNearCol==false)
			_activeLeaf->SetPosition(_vPosition.x,_vPosition.y);
	}

}
bool XM_MainRole::Check(double x,double y,double ElpasedTime)
{
	if(_direct == DIR_NODIRECTION)
	{
		if(_vPosition.x==x&&_vPosition.y==y)
		{
			_activeLeaf=NULL;
			_bOnCol=true;
			return true;
		}

	}
	else if ( ( (_direct%2 == 1) && ((_vPosition.x+(2-_direct)*PIC_WIDTH-x)*(_vPosition.x+(2-_direct)*ElpasedTime*_dspeed+(2-_direct)*PIC_WIDTH-x)<=0) && fabs(_vPosition.y-y)<1)||((_direct%2 == 0)&&((_vPosition.y+(3-_direct)*PIC_WIDTH-y)*(_vPosition.y+(3-_direct)*PIC_WIDTH+(3-_direct)*ElpasedTime*_dspeed-y)<=0)&&fabs(_vPosition.x-x)<1))//距离小于30，叶子停下
	{
		_bNearCol = true;
	}
	else if ( ((_direct%2 == 1) &&((_vPosition.x-x)*(_vPosition.x+(2-_direct)*ElpasedTime*_dspeed-x)<=0) && fabs(_vPosition.y-y)<1 ) || ((_direct%2 == 0)&&((_vPosition.y-y)*(_vPosition.y+(3-_direct)*ElpasedTime*_dspeed-y)<=0)&&fabs(_vPosition.x-x)<1))
	{
		_activeLeaf=NULL;
		_bOnCol=true;
		_bNearCol=false;
		_direct = DIR_NODIRECTION;
		_vPosition.x=static_cast<float>( x);
		_vPosition.y=static_cast<float>( y);
		return true;
	}	


	
	return false;
}
void XM_MainRole::SetDirection(DIRECTION dir)
{
	_direct = dir;
	switch(dir)
	{
	case DIR_EAST:
		_activeLeaf = _eastLeaf;
		break;
	case DIR_WEST:
		_activeLeaf = _westLeaf;
		break;
	case DIR_NORTH:
		_activeLeaf = _northLeaf;
		break;
	case DIR_SOUTH:
		_activeLeaf = _southLeaf;
		break;
	default:
		_activeLeaf = NULL;
		break;
	}
}

void XM_MainRole::Draw()
{
	if (_bOnCol)//在柱子上显示四片叶子
	{
		_eastLeaf->SetPosition(_vPosition.x+30,_vPosition.y);
		if(!_eastLeaf->CheckFail())
			_eastLeaf->Draw();
		_westLeaf->SetPosition(_vPosition.x-30,_vPosition.y);
		if(!_westLeaf->CheckFail())
			_westLeaf->Draw();
		_northLeaf->SetPosition(_vPosition.x,_vPosition.y-30);
		if (!_northLeaf->CheckFail())
			_northLeaf->Draw();
		_southLeaf->SetPosition(_vPosition.x,_vPosition.y+30);
		if (!_southLeaf->CheckFail())
			_southLeaf->Draw();
	}
	else
	{
		if (_activeLeaf!=NULL)
		{
			if (_bNearCol==false)
				_activeLeaf->SetPosition(_vPosition.x,_vPosition.y);

			_activeLeaf->Draw();
		}		
	}
	if(_pSprite==NULL) return;
	_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	_pSprite->Draw(_pTextSprite,&_rcSource, 0, &_vPosition, 0x99ffffff);
	_pSprite->End();
}

void XM_MainRole::CreateLeaf(LPDIRECT3DDEVICE9	 g_pDevice,const char* filename,int width,int height)
{
	_eastLeaf  = new XM_sprite();
	_eastLeaf->LoadFromFile(g_pDevice,filename,width,height);
	_westLeaf  = new XM_sprite();
	_westLeaf->LoadFromFile(g_pDevice,filename,width,height);
	_northLeaf = new XM_sprite();
	_northLeaf->LoadFromFile(g_pDevice,filename,width,height);
	_southLeaf = new XM_sprite();
	_southLeaf->LoadFromFile(g_pDevice,filename,width,height);
}

void XM_MainRole::ClearLeaf()
{
	_DELETE(_eastLeaf);
	_DELETE(_westLeaf);
	_DELETE(_northLeaf);
	_DELETE(_southLeaf);
}

void XM_MainRole::SetActive( DIRECTION dir)
{
	switch(dir)
	{
	case DIR_NODIRECTION:
		_activeLeaf=NULL;
		break;
	case DIR_EAST:
		_activeLeaf = _eastLeaf;
		_direct=DIR_EAST;
		_bOnCol=false;
		_bNearCol=false;
		break;
	case DIR_WEST:
		_activeLeaf = _westLeaf;
		_direct = DIR_WEST;
		_bOnCol=false;
		_bNearCol=false;
		break;
	case DIR_NORTH:
		_activeLeaf = _northLeaf;
		_direct = DIR_NORTH;
		_bOnCol=false;
		_bNearCol=false;
		break;
	case DIR_SOUTH:
		_activeLeaf = _southLeaf;
		_direct = DIR_SOUTH;
		_bOnCol=false;
		_bNearCol=false;
		break;
	}
}

RECT XM_MainRole::getLeafRect( DIRECTION dir)
{
	switch(dir)
	{
	case DIR_NODIRECTION:
		return this->getRect();
		break;
	case DIR_EAST:
		return _eastLeaf->getRect();
		break;
	case DIR_WEST:
		return _westLeaf->getRect();
		break;
	case DIR_NORTH:
		return _northLeaf->getRect();
		break;
	case DIR_SOUTH:
		return _southLeaf->getRect();
		break;
	default:
		#ifdef _DEBUG
		assert(0);
		#endif
		return this->getRect();
		break;
	}
}


