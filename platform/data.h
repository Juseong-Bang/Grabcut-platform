#pragma once

#define CHAR_MAX_LENGTH 64
#define LABEL_CNT 3
#define DEFAULT_LABEL 1

using namespace std;



class CData
{
	// ������, �Ҹ���, �ʱ�ȭ
public:
	CData();
	~CData();
	void init(void);
	void clear(void);

	// group index ���� ���� ���� // ("ȯ�ڸ�"+"��¥" �� ���� ��� ���� �׷�)
	int m_nGroupCnt;					// Group ���� (=���� �����ִ� �޸�(group)��)
	int m_nGroupAccumulatedCnt;			// Group ���� ������
	int* m_pnGroupIdx;					// Group idx (�� �迭: -1, �����: group ��ȣ)
	char** m_ppcGroupPatientName;		// Patient Name
	char** m_ppcGroupSeriesDate;		// Data

	// data index ���� ���� ���� //
	int m_nFrameCnt;					// Frame ���� (���ȯ��) (=���� �����ִ� �޸�(frame)��)
	int m_nFrameAccumulatedCnt;			// Frame ���� ������
	int* m_pnFrameIdx;					// Frame Idx (ȭ�鿡 ���̴� ������ ��ġ) (�� �迭: -1, �����: frame ������ȣ)
	int* m_pnFrameMemoryIdx;			// Frame Memory Idx (������ ������� ��ġ) (�� �迭: -1, �����: frame ������ȣ)
	int* m_pnFrameGroupIdx;				// Group ��ȣ (�� �迭: -1, �����: group ��ȣ)

	// dicom header //
	char** m_ppcPatientName;			// ȯ�ڸ�
	char** m_ppcInstitutionName;		// �����
	char** m_ppcSeriesDate;				// �Կ���¥
	char** m_ppcManufacturerModelName;	// ����
	char** m_ppcModality;				// ��� Modality
	float* m_pfPixelSpacing;			// PixelSpacing
	float* m_pfSliceThickness;			// SliceThickness
	int* m_pnInstanceNumber;			// InstanceNumber (DICOM Header�� ��ϵǾ� �ִ� frame ��ȣ)
	
	// image //
	int* m_pnHeight;					// ���� ����
	int* m_pnWidth;						// ���� ����
	short** m_ppsRawImage;				// �̹��� (����)
	unsigned char** m_ppucLabelImage;			// �̹��� Label
	unsigned char** m_ppucSelectedImage;	// �̹��� Selected

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