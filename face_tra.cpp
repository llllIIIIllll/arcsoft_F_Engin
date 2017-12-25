// face_tra.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <Windows.h>
#include "arcsoft_fsdk_face_tracking.h"
#include "merror.h"

#include "DemoDlg.h"

#include <string>
using namespace std;


#pragma warning(disable:4996)
#pragma comment(lib,"libarcsoft_fsdk_face_tracking.lib")

#define WORKBUF_SIZE        (40*1024*1024)
#define APPID		"E86unJtextWQ58JyThB7MpBrKpzXt3yJBtUNvNDsksmk"			//APPID
#define SDKKey		"9pNT69S6QxJHGcFPKtUZgwyDZev3RVpMiVh2MPWizsvZ"			//SDKKey


/* 获取视频帧数据，并保存到ASVLOFFSCREEN结构体中 */
MRESULT GetPreviewData(ASVLOFFSCREEN *offInput)
{
	MInt32 res = MOK;

	/* get frame data. add your code here */
	/* ... ... */

	return res;
}

int main()
{
	using namespace cv;
	DemoDlg demo;
	demo.DemoInit();

	while (true)
	{
		demo.DemoStart();
	}




	
	
	return 0;
}

