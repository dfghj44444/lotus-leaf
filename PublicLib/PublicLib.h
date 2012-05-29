#ifndef _WXM_OTHER_H
#define _WXM_OTHER_H

enum STEP
{
	K_STEP_FIRST,
	K_STEP_SECOND,
	K_STEP_THIRD,
	K_STEP_FOUTH
};


/**
	设置矩形
	@param 不解释
	@return 一个RECT
*/
RECT GetRect(int x1,int y1,int x2,int y2);	//设置矩形

/**
	取得随机数
	@param nMax:范围最大值
	@return 一个随机数值
*/
int random(int nMax);						//随机数
/**
	按下键后松开
	@param Key:键值
	@param x:按了几个键
*/
void PressKey(DWORD Key,int x=0);			
/**
	等待按键
*/
void WaitKey(DWORD Key);
/**
	延时函数
	@param time:延迟时间
*/
//void Delay(long time);	//delete By OneDouble@20100112		
/**
	速度测试
	@param nFrames:帧数
	@return 帧速
*/
int CountSpeed(long& nFrames);			

/**
	获取文件长度
	@param name:文件名
*/
int GetFileLength(char *name);		
/**
	判断文件是否存在
	@param name:文件名
*/
bool CheckFile(char *name);			

/**
	弹出调试信息
*/
void ShowMessage(char *msg,...);	
/**
	输出调试信息
*/
void PutDebugString(char *msg);		

/**
	char to wchar
*/
WCHAR* WChar(char *string);			
/**
	wchar to char
*/
char* Char(WCHAR *string);			

/**
	从文件名含路径分离出文件名不含路径
	@param filename:含路径的文件名
	@return 不含路径的文件名
*/
char* GetFileName(char* filename);	
/**
	由文件名获取文件路径
	@param filename:文件名
	@return 文件路径
*/
char *GetFilePath(char *filename);	

#endif

#define ERROR_DATA -99999999
#define _RELEASE(A)			try{ if( (A) != NULL ) { (A) -> Release(); (A) = NULL; } } catch(...) { PutDebugString("error ->Release!"); }
#define _DELETE(X)			try{ if( (X) != NULL ) { delete (X); (X) = NULL; } } catch(...) { PutDebugString("error delete!"); }
#define _DELETE_ARRAY(X)	try{ if( (X) != NULL ) { delete[] (X); (X) = NULL; } } catch(...) { PutDebugString("error delete[]!"); }
#define _FREE(X)			try{ if( (X) != NULL ) { free(X); (X)=NULL; } } catch(...) { PutDebugString("error free()!"); }
#define KEYDOWN(name,key) (name[key] & 0x80)