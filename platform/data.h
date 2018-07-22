#pragma once

#define CHAR_MAX_LENGTH 64
#define LABEL_CNT 3
#define DEFAULT_LABEL 1

using namespace std;



class CData
{
	// 생성자, 소멸자, 초기화
public:
	CData();
	~CData();
	void init(void);
	void clear(void);

	// group index 접근 관련 변수 // ("환자명"+"날짜" 가 같은 경우 같은 그룹)
	int m_nGroupCnt;					// Group 갯수 (=현재 열려있는 메모리(group)수)
	int m_nGroupAccumulatedCnt;			// Group 생성 누적합
	int* m_pnGroupIdx;					// Group idx (빈 배열: -1, 사용중: group 번호)
	char** m_ppcGroupPatientName;		// Patient Name
	char** m_ppcGroupSeriesDate;		// Data

	// data index 접근 관련 변수 //
	int m_nFrameCnt;					// Frame 갯수 (모든환자) (=현재 열려있는 메모리(frame)수)
	int m_nFrameAccumulatedCnt;			// Frame 생성 누적합
	int* m_pnFrameIdx;					// Frame Idx (화면에 보이는 순서와 일치) (빈 배열: -1, 사용중: frame 생성번호)
	int* m_pnFrameMemoryIdx;			// Frame Memory Idx (생성된 순서대로 배치) (빈 배열: -1, 사용중: frame 생성번호)
	int* m_pnFrameGroupIdx;				// Group 번호 (빈 배열: -1, 사용중: group 번호)

	// dicom header //
	char** m_ppcPatientName;			// 환자명
	char** m_ppcInstitutionName;		// 기관명
	char** m_ppcSeriesDate;				// 촬영날짜
	char** m_ppcManufacturerModelName;	// 기기명
	char** m_ppcModality;				// 기기 Modality
	float* m_pfPixelSpacing;			// PixelSpacing
	float* m_pfSliceThickness;			// SliceThickness
	int* m_pnInstanceNumber;			// InstanceNumber (DICOM Header에 기록되어 있는 frame 번호)
	
	// image //
	int* m_pnHeight;					// 세로 길이
	int* m_pnWidth;						// 가로 길이
	short** m_ppsRawImage;				// 이미지 (원형)
	unsigned char** m_ppucLabelImage;			// 이미지 Label
	unsigned char** m_ppucSelectedImage;	// 이미지 Selected

public:
	// get, set, copy //
	int getFrameGroupIdx(int nFrameIdx);
	int getGroupCnt();

	int getFrameCnt();
	bool isOccufiedFrame(int nFrameIdx);
	char* getPatientName(int nFrameIdx);
	char* getInstitutionName(int nFrameIdx);
	char* getSeriesDate(int nFrameIdx);
	char* getManufacturerModelName(int nFrameIdx);
	char* getModality(int nFrameIdx);
	float getPixelSpacing(int nFrameIdx);
	float getSliceThickness(int nFrameIdx); 
	int getInstanceNumber(int nFrameIdx);
	short* getRawImage(int nFrameIdx);
	bool copyRawImage(int nFrameIdx, int& nWidth, int& nHeight, short* &psRawImage);
	bool copyRawImages(int nStartFrameIdx, int nEndFrameIdx, int& nFrameCnt, int& nWidth, int& nHeight, short** &ppsRawImages);
	bool copyRawImages(int nGroupIdx, int& nFrameCnt, int& nWidth, int& nHeight, short** &ppsRawImages);
	bool setRawImage(int nFrameIdx, int nWidth, int nHeight, short* psRawImage);
	bool setRawImages(int nStartFrameIdx, int nEndFrameIdx, int nWidth, int nHeight, short** ppsRawImages);
	unsigned char* getLabelImage(int nFrameIdx);
	bool copyLabelImage(int nFrameIdx, int& nWidth, int& nHeight, unsigned char* &pusLabelImage);
	bool copyLabelImages(int nStartFrameIdx, int nEndFrameIdx, int& nFrameCnt, int& nWidth, int& nHeight, unsigned char** &ppucLabelImages);
	bool copyLabelImages(int nGroupIdx, int& nFrameCnt, int& nWidth, int& nHeight, unsigned char** &ppucLabelImages);
	bool setLabelImage(int nFrameIdx, int nWidth, int nHeight, unsigned char* pusLabelImage);
	bool setLabelImages(int nStartFrameIdx, int nEndFrameIdx, int nWidth, int nHeight, unsigned char** ppucLabelImages);
	unsigned char* getSelectedImage(int nFrameIdx);
	bool copySelectedImage(int nFrameIdx, int& nWidth, int& nHeight, unsigned char* &pusSelectedImage);
	bool copySelectedImages(int nStartFrameIdx, int nEndFrameIdx, int& nFrameCnt, int& nWidth, int& nHeight, unsigned char** &ppucSelectedImages);
	bool copySelectedImages(int nGroupIdx, int& nFrameCnt, int& nWidth, int& nHeight, unsigned char** &ppucSelectedImages);
	bool copySelectedImagesInUse(int nGroupIdx, int& nFrameCnt, int* &pnFrameIdx, int& nWidth, int& nHeight, unsigned char** &ppucSelectedImages);
	bool copySelectedImagesInUse(int nStartFrameIdx, int nEndFrameIdx, int& nFrameCnt, int* &pnFrameIdx, int& nWidth, int& nHeight, unsigned char** &ppucSelectedImages);
	bool setSelectedImage(int nFrameIdx, int nWidth, int nHeight, unsigned char* pusSelectedImage);
	bool setSelectedImages(int nStartFrameIdx, int nEndFrameIdx, int nWidth, int nHeight, unsigned char** ppucSelectedImages);
	int getWidth(int nFrameIdx);
	int getHeight(int nFrameIdx);
	int getGroupStartFrameIdx(int nGroupIdx);
	int getGroupEndFrameIdx(int nGroupIdx);

	////////
	int inputDICOM(char* pcImagePath);
	int inputMask(char* pcMaskPath);
	void deleteData(int nFrameIdx);

private:
	void resizeDataArray(int nDataCnt);
	void resizeGroupArray(int nGroupCnt);
	bool readImage(char* pcImagePath, int& nWidth, int& nHeight, short* &psRawImage, unsigned char* &psRawLabelImage, unsigned char* &psRawSelectedImage, bool bMakeNewArray);
	char* getFileExtension (char* pcFileName);
	char* getFileName(char* pcFilePath);
};