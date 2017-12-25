#pragma once

#include <string>
#include <vector>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <merror.h>
#include "FDEngine.h"
#include "FTEngine.h"
#include "FREngine.h"

#define MAX_FACEMODELS_NUM 12
#define MAX_INPUT_FACES_NUM 12

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>

typedef struct 
{
	const char* filename;
	cv::Rect rcFace;
	const char * name;
	AFR_FSDK_FACEMODEL faceModel;
} FACE;

typedef struct 
{
	int nFaceId;
	float fCompareResult;
} CompareResult;

class DemoDlg
{
protected:
	FDEngine*				mFDEngine;
	FTEngine*				mFTEngine;
	FREngine*				mFREngine;
	cv::VideoCapture		capture;
	cv::Mat                 frame;
public:
	DemoDlg();
	~DemoDlg();

	bool DemoInit();
	int DemoStart();

	void readMat(cv::Mat &frame);

	int getFacePosByFD(const char* filename, LPAFD_FSDK_FACERES &faceRes, ASVLOFFSCREEN &offInput);

	int getFacePosByFT(cv::Mat &frame, LPAFT_FSDK_FACERES &faceRes, ASVLOFFSCREEN &offInput);

	AFR_FSDK_FACEMODEL getFaceFea(const char* filename, AFR_FSDK_FACEMODEL &LocalFaceModels, int getFaceByFDOrFT);
};

