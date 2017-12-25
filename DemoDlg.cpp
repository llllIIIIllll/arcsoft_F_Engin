#include "stdafx.h"
#include "DemoDlg.h"


DemoDlg::DemoDlg()
{
}


DemoDlg::~DemoDlg()
{
}


bool DemoDlg::DemoInit()
{
	int ret = 0;
	mFDEngine = new FDEngine();
	ret = mFDEngine->init();
	if (ret != MOK)
	{
		std::cout << "FDEngine fail\n";
		return false;
	}

	mFREngine = new FREngine();
	ret = mFREngine->init();
	if (ret != MOK)
	{
		std::cout << "FREngine fail\n";
		return false;
	}

	mFTEngine = new FTEngine();
	ret = mFTEngine->init();
	if (ret != MOK)
	{
		std::cout << "FTEngine fail\n";
		return false;
	}

	capture.open(0);
	capture.set(CV_CAP_PROP_FRAME_WIDTH, 1080);//宽度 
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 960);//高度
	capture.set(CV_CAP_PROP_FPS, 30);//帧数
	capture.set(CV_CAP_PROP_BRIGHTNESS, 1);//亮度 1
	capture.set(CV_CAP_PROP_CONTRAST, 40);//对比度 40
	capture.set(CV_CAP_PROP_SATURATION, 50);//饱和度 50
	capture.set(CV_CAP_PROP_HUE, 50);//色调 50
	capture.set(CV_CAP_PROP_EXPOSURE, 50);//曝光 50
	if (!capture.isOpened())
	{
		std::cout << "video not open." << std::endl;
	}


	return true;
}

// get Face Position by detection
int DemoDlg::getFacePosByFD(const char *filename, LPAFD_FSDK_FACERES &faceRes, ASVLOFFSCREEN &offInput)
{
	offInput = bmp2ASVLOFFSCREEN(filename, offInput);
	int mRet = mFDEngine->FaceDetection(&offInput, &faceRes);
	if (faceRes->nFace <= 0 || mRet != MOK)
	{
		std::cout << "图片中没有检测到人脸！\n";
		return -1;
	}
	return 0;
}

// get Face Postion by Tracting             
int DemoDlg::getFacePosByFT(cv::Mat &frame, LPAFT_FSDK_FACERES &faceRes, ASVLOFFSCREEN &offInput)
{
	// 读取视频帧数据信息存入ASVLOFFSCREEN结构体
	offInput.u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;
	offInput.i32Width = frame.cols;
	offInput.i32Height = frame.rows;
	offInput.ppu8Plane[0] = (uint8_t *)frame.data;
	offInput.pi32Pitch[0] = offInput.i32Width * 3;
	// 进行FT，获取人脸框信息
	mFTEngine->FaceTracking(&offInput, &faceRes);

	return 0;
}

// get Face Feature
AFR_FSDK_FACEMODEL DemoDlg::getFaceFea(const char* filename, AFR_FSDK_FACEMODEL &LocalFaceModels, int getFaceByFDOrFT)
{
	AFR_FSDK_FACEMODEL faceModels;
	AFR_FSDK_FACEINPUT faceLocation;
	ASVLOFFSCREEN offInput = { 0 };

	// get face position
	if (getFaceByFDOrFT == 0)
	{
		// Face detection
		LPAFD_FSDK_FACERES faceRes;
		getFacePosByFD(filename, faceRes, offInput);

		faceLocation.rcFace = faceRes->rcFace[0];
		faceLocation.lOrient = faceRes->lfaceOrient[0];
	}
	else if (getFaceByFDOrFT == 1)
	{
		// Face tracing
		LPAFT_FSDK_FACERES faceRes;
		readMat(frame);
		getFacePosByFT(frame, faceRes, offInput);

		faceLocation.rcFace = faceRes->rcFace[0];
		faceLocation.lOrient = faceRes->lfaceOrient;

		// show face and trace
		cv::Point point1 = cv::Point(faceRes->rcFace[0].right, faceRes->rcFace[0].top);
		cv::Point point2 = cv::Point(faceRes->rcFace[0].left, faceRes->rcFace[0].bottom);
		cv::Rect  rect = cv::Rect(point1, point2);
		cv::rectangle(frame, rect, cv::Scalar(255, 0, 0), 3, 4, 0);
		imshow("test", frame);
		cv::waitKey(1);

	}
	else
	{
		std::cout << "please choose getFaceFea by FD or FT  0:= FD 1:=FT" << std::endl;
		exit(-1);
	}

	// 提取特征失败
	int mRet = mFREngine->ExtractFRFeature(&offInput, &faceLocation, &LocalFaceModels);
	if (mRet != MOK)
	{
		std::cout << "提取特征失败！\n";
	}
	
	// need to memcpy or only the input LocalFaceModels head will change
	// leed to the same AFR_FSDK_FACEMODEL whatever filename changes
	faceModels.lFeatureSize = LocalFaceModels.lFeatureSize;
	faceModels.pbFeature = (MByte*)malloc(faceModels.lFeatureSize);
	memcpy(faceModels.pbFeature, LocalFaceModels.pbFeature, faceModels.lFeatureSize);

	return faceModels;
}

int DemoDlg::DemoStart()
{

	// get 1 face feature
	//AFR_FSDK_FACEMODEL faceModels1 = { 0 };
	//const char* filename = "sample_2.bmp";
	//AFR_FSDK_FACEMODEL a = getFaceFea(filename, faceModels1, 0);

	AFR_FSDK_FACEMODEL faceModels1 = { 0 };
	AFR_FSDK_FACEMODEL a = getFaceFea(NULL, faceModels1, 1);


	// get 2 face feature
	AFR_FSDK_FACEMODEL faceModels2 = { 0 };
	const char* filename_2 = "sample/3.bmp";
	AFR_FSDK_FACEMODEL b = getFaceFea(filename_2, faceModels2, 0);

	// get the simil source
	MFloat fSimilScore = 0.0f;
	mFREngine->FacePairMatching(&a, &b, &fSimilScore);

	std::cout << fSimilScore << std::endl;



	return 0;
}

void DemoDlg::readMat(cv::Mat &frame)
{
	if (!capture.read(frame))
	{
		std::cout << "no video frame" << std::endl;
	}
}


/* face trace;

ASVLOFFSCREEN offInput = { 0 };
LPAFT_FSDK_FACERES faceRes;
readMat(frame);
getFacePosByFT(frame, faceRes, offInput);

std::cout << faceRes->rcFace[0].top << std::endl;
cv::Point point1 = cv::Point(faceRes->rcFace[0].right, faceRes->rcFace[0].top);
cv::Point point2 = cv::Point(faceRes->rcFace[0].left, faceRes->rcFace[0].bottom);
cv::Rect  rect   = cv::Rect(point1, point2);

cv::rectangle(frame, rect, cv::Scalar(255, 0, 0), 3, 4, 0);

imshow("test", frame);
cv::waitKey(1);

*/

