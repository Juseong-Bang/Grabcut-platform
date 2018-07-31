#include "data.h"
#include "platform.h"

CData::CData()
{
	init();
}

CData::~CData()
{
	clear();
}

void CData::init()
{
	// group //
	m_nGroupCnt = 0;
	m_nGroupAccumulatedCnt = 0;
	m_pnGroupIdx = NULL;
	m_ppcGroupPatientName = NULL;
	m_ppcGroupSeriesDate = NULL;

	// data //
	m_nFrameCnt = 0;
	m_nFrameAccumulatedCnt = 0;
	m_pnFrameIdx = NULL;
	m_pnFrameMemoryIdx = NULL;
	m_pnFrameGroupIdx = NULL;
	m_ppcPatientName = NULL;
	m_ppcInstitutionName = NULL;
	m_ppcSeriesDate = NULL;
	m_ppcManufacturerModelName = NULL;
	m_ppcModality = NULL;
	m_pfPixelSpacing = NULL;
	m_pfSliceThickness = NULL;
	m_pnInstanceNumber = NULL;

	// image //
	m_pnHeight = NULL;
	m_pnWidth = NULL;
	m_ppsRawImage = NULL;
	m_ppucLabelImage = NULL;
	m_ppucSelectedImage = NULL;
}
void CData::clear()
{
	// group //
	SAFE_DELETE_ARRAY(m_pnGroupIdx);
	SAFE_DELETE_VOLUME(m_ppcGroupPatientName, m_nGroupCnt);
	SAFE_DELETE_VOLUME(m_ppcGroupSeriesDate, m_nGroupCnt);

	// data //
	SAFE_DELETE_ARRAY(m_pnFrameIdx);
	SAFE_DELETE_VOLUME(m_ppcPatientName, m_nFrameCnt);
	SAFE_DELETE_VOLUME(m_ppcInstitutionName, m_nFrameCnt);
	SAFE_DELETE_VOLUME(m_ppcSeriesDate, m_nFrameCnt);
	SAFE_DELETE_VOLUME(m_ppcManufacturerModelName, m_nFrameCnt);
	SAFE_DELETE_VOLUME(m_ppcModality, m_nFrameCnt);
	SAFE_DELETE_ARRAY(m_pfPixelSpacing);
	SAFE_DELETE_ARRAY(m_pfSliceThickness);
	SAFE_DELETE_ARRAY(m_pnInstanceNumber);

	// image //
	SAFE_DELETE_ARRAY(m_pnHeight);
	SAFE_DELETE_ARRAY(m_pnWidth);
	SAFE_DELETE_VOLUME(m_ppsRawImage, m_nFrameCnt);
	SAFE_DELETE_VOLUME(m_ppucLabelImage, m_nFrameCnt);
	SAFE_DELETE_VOLUME(m_ppucSelectedImage, m_nFrameCnt);
}

int CData::inputDICOM(char* pcDICOMPath)
{
	// 1. DICOM 파일 Open ////////////////////////////////////////////////////////////////////////
	/*
	char* pcPatientName = new char[MAX_FILE_LENGTH];
	char* pcInstanceNum = new char[MAX_FILE_LENGTH];
	char* pcInstitutionName =  new char[MAX_FILE_LENGTH];
	char* pcSeriesDate =  new char[MAX_FILE_LENGTH];
	char* pcManufacturerModelName = new char[MAX_FILE_LENGTH];
	char* pcModality =  new char[MAX_FILE_LENGTH];
	double fPixelSpacing = 0.0;
	double fSliceThickness = 0.0;
	int nRow = 0;
	int nCol = 0;
	int nSliceNum = 0;
	memset(pcPatientName, 0, sizeof(char)*MAX_FILE_LENGTH);
	memset(pcInstitutionName, 0, sizeof(char)*MAX_FILE_LENGTH);
	memset(pcSeriesDate, 0, sizeof(char)*MAX_FILE_LENGTH);
	memset(pcManufacturerModelName, 0, sizeof(char)*MAX_FILE_LENGTH);
	memset(pcModality, 0, sizeof(char)*MAX_FILE_LENGTH);
	*/

	char pcPatientName[MAX_FILE_LENGTH] = { 0 };
	char pcInstanceNum[MAX_FILE_LENGTH] = { 0 };
	char pcInstitutionName[MAX_FILE_LENGTH] = { 0 };
	char pcSeriesDate[MAX_FILE_LENGTH] = { 0 };
	char pcManufacturerModelName[MAX_FILE_LENGTH] = { 0 };
	char pcModality[MAX_FILE_LENGTH] = { 0 };
	double fPixelSpacing = 0.0;
	double fSliceThickness = 0.0;
	int nRow = 0;
	int nCol = 0;
	int nSliceNum = 0;

	bool bLoadSuccess = false;
	if (strcmp(getFileExtension(pcDICOMPath), "dcm") == 0) {
		// file load //
		DcmFileFormat *fileformat = new DcmFileFormat();
		fileformat->loadFile(pcDICOMPath);

		OFCondition status = fileformat->getDataset()->chooseRepresentation(EXS_LittleEndianExplicit, NULL);
		if (!status.good())
		{
			std::cerr << "DCMTK error: " << status.text() << std::endl;
			delete fileformat;
		}
		else {
			bLoadSuccess = true;
		}

		// declar var //
		OFString OFStrPatientName, OFStrInstitutionName, OFStrSeriesDate, OFStrManufacturerModelName,
			OFStrModality, OFStrPixelSpacing, OFStrSliceThickeness, OFStrRows, OFStrColumns, OFStrInstanceNumber;

		DcmDataset* dataSet = fileformat->getDataset();
		dataSet->findAndGetOFString(DcmTagKey(0x0010, 0x0010), OFStrPatientName);
		dataSet->findAndGetOFString(DcmTagKey(0x0008, 0x0080), OFStrInstitutionName);
		dataSet->findAndGetOFString(DcmTagKey(0x0008, 0x0021), OFStrSeriesDate);
		dataSet->findAndGetOFString(DcmTagKey(0x0008, 0x1090), OFStrManufacturerModelName);
		dataSet->findAndGetOFString(DcmTagKey(0x0008, 0x0060), OFStrModality);
		dataSet->findAndGetOFString(DcmTagKey(0x0028, 0x0030), OFStrPixelSpacing);
		dataSet->findAndGetOFString(DcmTagKey(0x0018, 0x0050), OFStrSliceThickeness);
		dataSet->findAndGetOFString(DcmTagKey(0x0028, 0x0010), OFStrRows);
		dataSet->findAndGetOFString(DcmTagKey(0x0028, 0x0011), OFStrColumns);
		dataSet->findAndGetOFString(DcmTagKey(0x0020, 0x0013), OFStrInstanceNumber);

		std::sprintf(pcPatientName, "%s", OFStrPatientName.c_str());
		std::sprintf(pcInstitutionName, "%s", OFStrInstitutionName.c_str());
		std::sprintf(pcSeriesDate, "%s", OFStrSeriesDate.c_str());
		std::sprintf(pcManufacturerModelName, "%s", OFStrManufacturerModelName.c_str());
		std::sprintf(pcModality, "%s", OFStrModality.c_str());
		fPixelSpacing = atof(OFStrPixelSpacing.c_str());
		fSliceThickness = atof(OFStrSliceThickeness.c_str());
		nRow = atoi(OFStrRows.c_str());
		nCol = atoi(OFStrColumns.c_str());
		nSliceNum = atoi(OFStrInstanceNumber.c_str());

		// delete memory //
		fileformat->clear();
		delete fileformat;
	}
	else if (strcmp(getFileExtension(pcDICOMPath), "jpg") == 0 || strcmp(getFileExtension(pcDICOMPath), "png") == 0) {
		// file load //
		IplImage* image = cvLoadImage(pcDICOMPath, false);
		if (image != NULL) {
			bLoadSuccess = true;

			// copy file information //
			char pcFilePathTemp[MAX_FILE_LENGTH] = { 0 };
			char* pcFileName = NULL;

			std::sprintf(pcFilePathTemp, "%s", pcDICOMPath);
			pcFileName = getFileName(pcFilePathTemp);

			char* token = NULL;
			token = strtok(pcFileName, ".");
			std::sprintf(pcFileName, "%s", token);

			int nTokenCnt = 0;
			int nFileNameLength = strlen(pcFileName);
			for (int i = 0; i < nFileNameLength; i++) {
				char* temp = pcFileName + i;
				if (*temp == '_') {
					nTokenCnt++;
				}
			}
			if (nTokenCnt >= 3) {
				// patientName_SeriesDate_SeriesDateNum_SliceNum//
				// patientName
				token = strtok(pcFileName, "_");
				std::sprintf(pcPatientName, "%s", token);

				// SeriesDate
				token = strtok(NULL, "_");
				std::sprintf(pcSeriesDate, "%s", token);

				token = strtok(NULL, "_");
				int nSeriesDateNum = atoi(token);
				std::sprintf(pcSeriesDate, "%s_%d", pcSeriesDate, nSeriesDateNum);

				token = strtok(NULL, "_");
				if (token != NULL) {
					std::sprintf(pcInstanceNum, "%s", token);
					nSliceNum = atoi(pcInstanceNum);
				}
			}
			else {
				// patientName
				token = strtok(pcFileName, "_");
				std::sprintf(pcPatientName, "%s", token);

				// SeriesDate
				token = strtok(NULL, "_");
				std::sprintf(pcSeriesDate, "%s", token);

				// instanceNum
				token = strtok(NULL, "_");
				if (token != NULL) {
					std::sprintf(pcInstanceNum, "%s", token);
					nSliceNum = atoi(pcInstanceNum);
				}
			}

			// row
			nRow = image->height;
			nCol = image->width;

			cvReleaseImage(&image);
		}
	}
	if (bLoadSuccess == false) {
		QMessageBox msgBox;
		msgBox.setText(QString::fromLocal8Bit("Image load 실패 - DICOM Header에 문제가 있습니다."));
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.exec();

		/*
		SAFE_DELETE_ARRAY(pcPatientName);
		SAFE_DELETE_ARRAY(pcInstanceNum);
		SAFE_DELETE_ARRAY(pcInstitutionName);
		SAFE_DELETE_ARRAY(pcSeriesDate);
		SAFE_DELETE_ARRAY(pcManufacturerModelName);
		SAFE_DELETE_ARRAY(pcModality);
		*/
		return -1;
	}

	// 2. 데이터 중복 검사 및 공간확보 ///////////////////////////////////////////////////////////////////
	bool bDuplicateHeader = false;
	bool bDuplicateGroup = false;
	int nFrameIdx = -1;
	int nGroupIdx = -1;

	// for dicom header data //
	for (int i = 0; i < m_nFrameCnt; i++) {
		if (m_pnFrameIdx[i] >= 0) {
			if (strcmp(m_ppcPatientName[i], pcPatientName) == 0 &&
				strcmp(m_ppcSeriesDate[i], pcSeriesDate) == 0 &&
				m_pnInstanceNumber[i] == nSliceNum)
			{
				bDuplicateHeader = true;
				nFrameIdx = i;
				break;
			}
		}
		else {
			nFrameIdx = i;
			break;
		}
	}
	if (bDuplicateHeader == false) {
		if (nFrameIdx == -1) {
			// 저장 공간이 부족할 경우, 배열 realloc
			resizeDataArray(m_nFrameCnt + 500);
			nFrameIdx = m_nFrameAccumulatedCnt;
		}
	}

	// 3. DICOM Header 정보 로드 ////////////////////////////////////////////////////////////////////////
	if (bDuplicateHeader == false) {
		// for dicom data //
		if (m_ppcPatientName[nFrameIdx] == NULL) { m_ppcPatientName[nFrameIdx] = new char[CHAR_MAX_LENGTH]; }
		if (m_ppcInstitutionName[nFrameIdx] == NULL) { m_ppcInstitutionName[nFrameIdx] = new char[CHAR_MAX_LENGTH]; }
		if (m_ppcSeriesDate[nFrameIdx] == NULL) { m_ppcSeriesDate[nFrameIdx] = new char[CHAR_MAX_LENGTH]; }
		if (m_ppcManufacturerModelName[nFrameIdx] == NULL) { m_ppcManufacturerModelName[nFrameIdx] = new char[CHAR_MAX_LENGTH]; }
		if (m_ppcModality[nFrameIdx] == NULL) { m_ppcModality[nFrameIdx] = new char[CHAR_MAX_LENGTH]; }

		m_pnFrameIdx[nFrameIdx] = m_nFrameAccumulatedCnt;
		m_pnFrameMemoryIdx[nFrameIdx] = m_nFrameAccumulatedCnt;
		m_pnFrameGroupIdx[nFrameIdx] = nGroupIdx;
		sprintf(m_ppcPatientName[nFrameIdx], "%s", pcPatientName);
		sprintf(m_ppcInstitutionName[nFrameIdx], "%s", pcInstitutionName);
		sprintf(m_ppcSeriesDate[nFrameIdx], "%s", pcSeriesDate);
		sprintf(m_ppcManufacturerModelName[nFrameIdx], "%s", pcManufacturerModelName);
		sprintf(m_ppcModality[nFrameIdx], "%s", pcModality);
		m_pfPixelSpacing[nFrameIdx] = fPixelSpacing;
		m_pfSliceThickness[nFrameIdx] = fSliceThickness;
		m_pnInstanceNumber[nFrameIdx] = nSliceNum;

		m_pnHeight[nFrameIdx] = nRow;
		m_pnWidth[nFrameIdx] = nCol;
	}

	// for group //
	for (int i = 0; i < m_nGroupCnt; i++) {
		if (m_pnGroupIdx[i] >= 0) {
			if (strcmp(m_ppcPatientName[nFrameIdx], m_ppcGroupPatientName[i]) == 0 &&
				strcmp(m_ppcSeriesDate[nFrameIdx], m_ppcGroupSeriesDate[i]) == 0)
			{
				bDuplicateGroup = true;
				nGroupIdx = i;
				break;
			}
		}
		else {
			nGroupIdx = i;
			break;
		}
	}
	if (bDuplicateGroup == false) {
		if (nGroupIdx == -1) {
			resizeGroupArray(m_nGroupCnt + 100);
			nGroupIdx = m_nGroupAccumulatedCnt;
		}
	}
	if (bDuplicateGroup == false) {
		if (m_ppcGroupPatientName[nGroupIdx] == NULL) { m_ppcGroupPatientName[nGroupIdx] = new char[CHAR_MAX_LENGTH]; }
		if (m_ppcGroupSeriesDate[nGroupIdx] == NULL) { m_ppcGroupSeriesDate[nGroupIdx] = new char[CHAR_MAX_LENGTH]; }

		m_pnGroupIdx[nGroupIdx] = m_nGroupAccumulatedCnt;
		sprintf(m_ppcGroupPatientName[nGroupIdx], "%s", pcPatientName);
		sprintf(m_ppcGroupSeriesDate[nGroupIdx], "%s", pcSeriesDate);
		m_nGroupAccumulatedCnt += 1;
	}



	// 4. DICOM 파일 이미지 로드 ////////////////////////////////////////////////////////////////////////
	bool bLoad = false;
	bLoad = readImage(pcDICOMPath, m_pnWidth[nFrameIdx], m_pnHeight[nFrameIdx], m_ppsRawImage[nFrameIdx], m_ppucLabelImage[nFrameIdx], m_ppucSelectedImage[nFrameIdx], false);

	if (bLoad) {
		m_pnFrameGroupIdx[nFrameIdx] = nGroupIdx;
		m_nFrameAccumulatedCnt += 1;
	}

	// 5. 메모리 소멸 ///////////////////////////////////////////////////////////////////////////////////
	/*
	SAFE_DELETE_ARRAY(pcPatientName);
	SAFE_DELETE_ARRAY(pcInstanceNum);
	SAFE_DELETE_ARRAY(pcInstitutionName);
	SAFE_DELETE_ARRAY(pcSeriesDate);
	SAFE_DELETE_ARRAY(pcManufacturerModelName);
	SAFE_DELETE_ARRAY(pcModality);
	*/

	return m_pnFrameIdx[nFrameIdx];
}
int CData::inputMask(char* pcMaskPath)
{
	// 같은 환자번호/날짜/frame 번호를 검색해서 input
	char* pcPatientName = new char[CHAR_MAX_LENGTH];
	char* pcSeriesDate = new char[CHAR_MAX_LENGTH];
	char* pcInstanceNumber = new char[CHAR_MAX_LENGTH];
	char* pcFilePathTemp = new char[MAX_FILE_LENGTH];
	int nImageWidth = 0, nImageHeight = 0;
	int nFrameIdx = -1;

	if (strcmp(getFileExtension(pcMaskPath), "bmp") == 0) {

		// file 명 ////
		char* pcFileName = NULL;
		std::sprintf(pcFilePathTemp, "%s", pcMaskPath);
		pcFileName = getFileName(pcFilePathTemp);

		char* token = NULL;
		token = strtok(pcFileName, ".");
		std::sprintf(pcFileName, "%s", token);

		// patientName
		int nTokenCnt = 0;
		int nFileNameLength = strlen(pcFileName);
		for (int i = 0; i < nFileNameLength; i++) {
			char* temp = pcFileName + i;
			if (*temp == '_') {
				nTokenCnt++;
			}
		}
		if (nTokenCnt >= 3) {
			// patientName_SeriesDate_SeriesDateNum_SliceNum//
			// patientName
			token = strtok(pcFileName, "_");
			std::sprintf(pcPatientName, "%s", token);

			// SeriesDate
			token = strtok(NULL, "_");
			std::sprintf(pcSeriesDate, "%s", token);

			token = strtok(NULL, "_");
			int nSeriesDateNum = atoi(token);
			std::sprintf(pcSeriesDate, "%s_%d", pcSeriesDate, nSeriesDateNum);

			token = strtok(NULL, "_");
			if (token != NULL) {
				std::sprintf(pcInstanceNumber, "%s", token);
			}
			else {
				std::sprintf(pcInstanceNumber, "%d", 0);
			}
		}
		else {
			// patientName
			token = strtok(pcFileName, "_");
			std::sprintf(pcPatientName, "%s", token);

			// SeriesDate
			token = strtok(NULL, "_");
			std::sprintf(pcSeriesDate, "%s", token);

			// InstanceNo (slice No.)
			token = strtok(NULL, "_");
			if (token != NULL) {
				std::sprintf(pcInstanceNumber, "%s", token);
			}
			else {
				std::sprintf(pcInstanceNumber, "%d", 0);
			}
		}

		bool isDup = false;
		for (int i = 0; i < m_nFrameCnt; i++) {
			if (m_pnFrameIdx[i] >= 0) {
				if (strcmp(m_ppcPatientName[i], pcPatientName) == 0 &&
					strcmp(m_ppcSeriesDate[i], pcSeriesDate) == 0 &&
					m_pnInstanceNumber[i] == atoi(pcInstanceNumber))
				{
					isDup = true;
					nImageWidth = m_pnWidth[i];
					nImageHeight = m_pnHeight[i];
					nFrameIdx = i;
					break;
				}
			}
		}

		SAFE_DELETE_ARRAY(pcPatientName);
		SAFE_DELETE_ARRAY(pcSeriesDate);
		SAFE_DELETE_ARRAY(pcInstanceNumber);
		SAFE_DELETE_ARRAY(pcFilePathTemp);

		if (isDup) {
			// image open해서 open한 이미지의 크기와 메모리의 크기가 일치하는지 확인
			IplImage* maskImage = cvLoadImage(pcMaskPath, false);

			if (maskImage != NULL) {
				int nMaskImageWidth = maskImage->width;
				int nMaskImageHeight = maskImage->height;

				if (nMaskImageWidth == nImageWidth && nMaskImageHeight == nImageHeight) {
					bool bBinaryMask = false;
					for (int row = 0; row < nImageHeight; row++) {
						for (int col = 0; col < nImageWidth; col++) {
							int index = row*nImageWidth + col;
							int maskIndex = maskImage->widthStep*row + col;
							unsigned char value = (unsigned char)maskImage->imageData[maskIndex];

							if (value == 255) {
								bBinaryMask = true;
								break;
							}
						}
						if (bBinaryMask) {
							break;
						}
					}

					if (bBinaryMask) {
						for (int row = 0; row < nImageHeight; row++) {
							for (int col = 0; col < nImageWidth; col++) {
								int index = row*nImageWidth + col;
								int maskIndex = maskImage->widthStep*row + col;
								unsigned char value = (unsigned char)maskImage->imageData[maskIndex];

								if (value > 125) {
									m_ppucLabelImage[nFrameIdx][row*nImageWidth + col] = DEFAULT_LABEL;
								}
							}
						}
					}
					else {
						for (int row = 0; row < nImageHeight; row++) {
							for (int col = 0; col < nImageWidth; col++) {
								int index = row*nImageWidth + col;
								int maskIndex = maskImage->widthStep*row + col;
								unsigned char value = (unsigned char)maskImage->imageData[maskIndex];

								m_ppucLabelImage[nFrameIdx][row*nImageWidth + col] = value;
							}
						}
					}
				}
				cvReleaseImage(&maskImage);
			}
			else {
				QMessageBox msgBox;
				msgBox.setText(QString::fromLocal8Bit("load된 영상과 maskImage의 가로/세로 크기가 다름"));
				msgBox.setIcon(QMessageBox::Warning);
				msgBox.exec();

				cvReleaseImage(&maskImage);
				return -1;
			}
		}
		else {
			return -1;
		}
	}

	return nFrameIdx;
}
void CData::deleteData(int nFrameIdx)
{

}

// get, set //
int CData::getFrameGroupIdx(int nFrameIdx)
{
	return m_pnFrameGroupIdx[nFrameIdx];
}
int CData::getGroupCnt()
{
	return m_nGroupCnt;
}
int CData::getFrameCnt()
{
	return m_nFrameCnt;
}
bool CData::isOccufiedFrame(int nFrameIdx)
{
	if (m_pnFrameIdx[nFrameIdx] == -1) {
		return false;
	}
	else {
		return true;
	}
}
char* CData::getPatientName(int nFrameIdx)
{
	return m_ppcPatientName[nFrameIdx];
}
char* CData::getInstitutionName(int nFrameIdx)
{
	return m_ppcInstitutionName[nFrameIdx];
}
char* CData::getSeriesDate(int nFrameIdx)
{
	return m_ppcSeriesDate[nFrameIdx];
}
char* CData::getManufacturerModelName(int nFrameIdx)
{
	return m_ppcManufacturerModelName[nFrameIdx];
}
char* CData::getModality(int nFrameIdx)
{
	return m_ppcModality[nFrameIdx];
}
float CData::getPixelSpacing(int nFrameIdx)
{
	return m_pfPixelSpacing[nFrameIdx];
}
float CData::getSliceThickness(int nFrameIdx)
{
	return m_pfSliceThickness[nFrameIdx];
}
int CData::getInstanceNumber(int nFrameIdx)
{
	return m_pnInstanceNumber[nFrameIdx];
}
short* CData::getRawImage(int nFrameIdx)
{
	return m_ppsRawImage[nFrameIdx];
}
bool CData::copyRawImage(int nFrameIdx, int& nWidth, int& nHeight, short* &psRawImage)
{
	// 0. input paremter check
	if (m_pnFrameIdx == NULL || m_pnFrameIdx[nFrameIdx] == -1) {
		return false;
	}
	if (psRawImage == NULL) {
		nWidth = m_pnWidth[nFrameIdx];
		nHeight = m_pnHeight[nFrameIdx];
		psRawImage = new short[nWidth*nHeight * 3];
	}

	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++)
			for (int ch = 0; ch < 3; ch++)
			{
				int idx = row*nWidth * 3 + col * 3;
				psRawImage[idx + ch] = m_ppsRawImage[nFrameIdx][idx + ch];
			}
	}

	return true;
}
bool CData::copyRawImages(int nStartFrameIdx, int nEndFrameIdx, int& nFrameCnt, int& nWidth, int& nHeight, short** &ppsRawImages)
{
	// 0. input parameter check
	if (m_pnFrameIdx == NULL || m_pnFrameIdx[nStartFrameIdx] == -1 || m_pnFrameIdx[nEndFrameIdx] == -1) {
		return false;
	}
	if (ppsRawImages != NULL) {
		return false;
	}
	if (nEndFrameIdx < nStartFrameIdx) {
		return false;
	}

	// 1. 
	nWidth = m_pnWidth[nStartFrameIdx];
	nHeight = m_pnHeight[nStartFrameIdx];
	nFrameCnt = nEndFrameIdx - nStartFrameIdx + 1;

	ppsRawImages = new short*[nFrameCnt];
	for (int i = 0; i < nFrameCnt; i++) {
		copyRawImage(i + nStartFrameIdx, nWidth, nHeight, ppsRawImages[i]);
	}

	return true;
}
bool CData::copyRawImages(int nGroupIdx, int& nFrameCnt, int& nWidth, int& nHeight, short** &ppsRawImages)
{
	if (m_pnFrameIdx == NULL) {
		return false;
	}

	// 해당 group에 속하는 frame 갯수
	int nStartFrameIdx = -1;
	int nEndFrameIdx = -1;

	for (int i = 0; i < m_nFrameCnt; i++) {
		if ((m_pnFrameIdx[i] != -1) && (m_pnFrameGroupIdx[i] == nGroupIdx)) {
			if (nStartFrameIdx == -1) { nStartFrameIdx = i; }
			nEndFrameIdx = i;
		}
	}

	return copyRawImages(nStartFrameIdx, nEndFrameIdx, nFrameCnt, nWidth, nHeight, ppsRawImages);
}
bool CData::setRawImage(int nFrameIdx, int nWidth, int nHeight, short* psRawImage)
{
	// input parameter check //
	if (psRawImage == NULL) { return false; }
	if (m_pnFrameIdx == NULL) { return false; }
	if (m_pnWidth == NULL || m_pnHeight == NULL || m_pnWidth[nFrameIdx] != nWidth || m_pnHeight[nFrameIdx] != nHeight) { return false; }

	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++)
			for (int ch = 0; ch < 3; ch++)
			{
				int idx = row*nWidth * 3 + col * 3;
				m_ppsRawImage[nFrameIdx][idx + ch] = psRawImage[idx + ch];
			}
	}

	return true;

}
bool CData::setRawImages(int nStartFrameIdx, int nEndFrameIdx, int nWidth, int nHeight, short** ppsRawImages)
{
	if (ppsRawImages == NULL) {
		return false;
	}
	int nCount = nEndFrameIdx - nStartFrameIdx + 1;

	for (int i = 0; i < nCount; i++) {
		int idx = nStartFrameIdx + i;
		setRawImage(idx, nWidth, nHeight, ppsRawImages[i]);
	}

	return true;
}
unsigned char* CData::getLabelImage(int nFrameIdx)
{
	return m_ppucLabelImage[nFrameIdx];
}
bool CData::copyLabelImage(int nFrameIdx, int& nWidth, int& nHeight, unsigned char* &pusLabelImage)
{
	// 0. input paremter check
	if (m_pnFrameIdx == NULL || m_pnFrameIdx[nFrameIdx] == -1) {
		return false;
	}
	if (pusLabelImage != NULL) {
		nWidth = m_pnWidth[nFrameIdx];
		nHeight = m_pnHeight[nFrameIdx];
		pusLabelImage = new unsigned char[nWidth*nHeight];
	}

	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			int idx = row*nWidth + col;
			pusLabelImage[idx] = m_ppucLabelImage[nFrameIdx][idx];
		}
	}

	return true;
}
bool CData::copyLabelImages(int nStartFrameIdx, int nEndFrameIdx, int& nFrameCnt, int& nWidth, int& nHeight, unsigned char** &ppucLabelImages)
{
	// 0. input parameter check
	if (m_pnFrameIdx == NULL || m_pnFrameIdx[nStartFrameIdx] == -1 || m_pnFrameIdx[nEndFrameIdx] == -1) {
		return false;
	}
	if (ppucLabelImages != NULL) {
		return false;
	}
	if (nEndFrameIdx < nStartFrameIdx) {
		return false;
	}

	// 1. 
	nWidth = m_pnWidth[nStartFrameIdx];
	nHeight = m_pnHeight[nStartFrameIdx];
	nFrameCnt = nEndFrameIdx - nStartFrameIdx + 1;

	ppucLabelImages = new unsigned char*[nFrameCnt];
	for (int i = 0; i < nFrameCnt; i++) {
		copyLabelImage(nStartFrameIdx + i, nWidth, nHeight, ppucLabelImages[i]);
	}

	return true;
}
bool CData::copyLabelImages(int nGroupIdx, int& nFrameCnt, int& nWidth, int& nHeight, unsigned char** &ppucLabelImages)
{
	if (m_pnFrameIdx == NULL) {
		return false;
	}

	// 해당 group에 속하는 frame 갯수
	int nStartFrameIdx = -1;
	int nEndFrameIdx = -1;

	for (int i = 0; i < m_nFrameCnt; i++) {
		if ((m_pnFrameIdx[i] != -1) && (m_pnFrameGroupIdx[i] == nGroupIdx)) {
			if (nStartFrameIdx == -1) { nStartFrameIdx = i; }
			nEndFrameIdx = i;
		}
	}

	return copyLabelImages(nStartFrameIdx, nEndFrameIdx, nFrameCnt, nWidth, nHeight, ppucLabelImages);
}
bool CData::setLabelImage(int nFrameIdx, int nWidth, int nHeight, unsigned char* pusLabelImage)
{
	// input parameter check //
	if (pusLabelImage == NULL) { return false; }
	if (m_pnFrameIdx == NULL) { return false; }
	if (m_pnWidth == NULL || m_pnHeight == NULL || m_pnWidth[nFrameIdx] != nWidth || m_pnHeight[nFrameIdx] != nHeight) { return false; }

	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			int idx = row*nWidth + col;
			m_ppucLabelImage[nFrameIdx][idx] = pusLabelImage[idx];
		}
	}

	return true;

}
bool CData::setLabelImages(int nStartFrameIdx, int nEndFrameIdx, int nWidth, int nHeight, unsigned char** ppucLabelImages)
{
	if (ppucLabelImages == NULL) {
		return false;
	}
	int nCount = nEndFrameIdx - nStartFrameIdx + 1;

	for (int i = 0; i < nCount; i++) {
		int idx = nStartFrameIdx + i;
		setLabelImage(idx, nWidth, nHeight, ppucLabelImages[i]);
	}

	return true;
}
unsigned char* CData::getSelectedImage(int nFrameIdx)
{
	return m_ppucSelectedImage[nFrameIdx];
}
bool CData::copySelectedImage(int nFrameIdx, int& nWidth, int& nHeight, unsigned char* &pusSelectedImage)
{
	// 0. input paremter check
	if (m_pnFrameIdx == NULL || m_pnFrameIdx[nFrameIdx] == -1) {
		return false;
	}
	if (pusSelectedImage != NULL) {
		nWidth = m_pnWidth[nFrameIdx];
		nHeight = m_pnHeight[nFrameIdx];
		pusSelectedImage = new unsigned char[nWidth*nHeight];
	}

	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			int idx = row*nWidth + col;
			pusSelectedImage[idx] = m_ppucSelectedImage[nFrameIdx][idx];
		}
	}

	return true;
}
bool CData::copySelectedImages(int nStartFrameIdx, int nEndFrameIdx, int& nFrameCnt, int& nWidth, int& nHeight, unsigned char** &ppucSelectedImages)
{
	// 0. input parameter check
	if (m_pnFrameIdx == NULL || m_pnFrameIdx[nStartFrameIdx] == -1 || m_pnFrameIdx[nEndFrameIdx] == -1) {
		return false;
	}
	if (ppucSelectedImages != NULL) {
		return false;
	}
	if (nEndFrameIdx < nStartFrameIdx) {
		return false;
	}

	// 1. 
	nWidth = m_pnWidth[nStartFrameIdx];
	nHeight = m_pnHeight[nStartFrameIdx];
	nFrameCnt = nEndFrameIdx - nStartFrameIdx + 1;

	ppucSelectedImages = new unsigned char*[nFrameCnt];
	for (int i = 0; i < nFrameCnt; i++) {
		copySelectedImage(nStartFrameIdx + i, nWidth, nHeight, ppucSelectedImages[i]);
	}

	return true;
}
bool CData::copySelectedImages(int nGroupIdx, int& nFrameCnt, int& nWidth, int& nHeight, unsigned char** &ppucSelectedImages)
{
	if (m_pnFrameIdx == NULL) {
		return false;
	}

	// 해당 group에 속하는 frame 갯수
	int nStartFrameIdx = -1;
	int nEndFrameIdx = -1;

	for (int i = 0; i < m_nFrameCnt; i++) {
		if ((m_pnFrameIdx[i] != -1) && (m_pnFrameGroupIdx[i] == nGroupIdx)) {
			if (nStartFrameIdx == -1) { nStartFrameIdx = i; }
			nEndFrameIdx = i;
		}
	}

	return copySelectedImages(nStartFrameIdx, nEndFrameIdx, nFrameCnt, nWidth, nHeight, ppucSelectedImages);
}
bool CData::copySelectedImagesInUse(int nStartFrameIdx, int nEndFrameIdx, int& nFrameCnt, int* &pnFrameIdx, int& nWidth, int& nHeight, unsigned char** &ppucSelectedImages)
{
	// 0. input parameter check
	if (m_pnFrameIdx == NULL || m_pnFrameIdx[nStartFrameIdx] == -1 || m_pnFrameIdx[nEndFrameIdx] == -1) {
		return false;
	}
	if (ppucSelectedImages != NULL) {
		return false;
	}
	if (pnFrameIdx != NULL) {
		return false;
	}
	if (nEndFrameIdx < nStartFrameIdx) {
		return false;
	}

	nWidth = m_pnWidth[nStartFrameIdx];
	nHeight = m_pnHeight[nStartFrameIdx];

	vector<int> frameIdxs;
	bool isUse = false;
	for (int i = nStartFrameIdx; i < nEndFrameIdx; i++) {
		for (int row = 0; row < nHeight; row++) {
			for (int col = 0; col < nWidth; col++) {
				int idx = row*nWidth + col;
				if (m_ppucSelectedImage[i][idx] > 0) {
					isUse = true;
					break;
				}
			}
			if (isUse) { break; }
		}
		if (isUse) {
			frameIdxs.push_back(i);
			break;
		}
	}

	nFrameCnt = frameIdxs.size();
	ppucSelectedImages = new unsigned char*[nFrameCnt];
	if (frameIdxs.size() > 0) {
		pnFrameIdx = new int[nFrameCnt];
	}
	for (int i = 0; i < nFrameCnt; i++) {
		pnFrameIdx[i] = frameIdxs[i];
		ppucSelectedImages[i] = new unsigned char[nWidth*nHeight];
		for (int row = 0; row < nHeight; row++) {
			for (int col = 0; col < nWidth; col++) {
				int idx = row*nWidth + col;
				ppucSelectedImages[i][idx] = m_ppucSelectedImage[frameIdxs[i]][idx];
			}
		}
	}

	return true;
}
bool CData::copySelectedImagesInUse(int nGroupIdx, int& nFrameCnt, int* &pnFrameIdx, int& nWidth, int& nHeight, unsigned char** &ppucSelectedImages)
{
	if (m_pnFrameIdx == NULL) {
		return false;
	}

	// 해당 group에 속하는 frame 갯수
	int nStartFrameIdx = -1;
	int nEndFrameIdx = -1;

	for (int i = 0; i < m_nFrameCnt; i++) {
		if ((m_pnFrameIdx[i] != -1) && (m_pnFrameGroupIdx[i] == nGroupIdx)) {
			if (nStartFrameIdx == -1) { nStartFrameIdx = i; }
			nEndFrameIdx = i;
		}
	}
	return copySelectedImagesInUse(nStartFrameIdx, nEndFrameIdx, nFrameCnt, pnFrameIdx, nWidth, nHeight, ppucSelectedImages);
}
bool CData::setSelectedImage(int nFrameIdx, int nWidth, int nHeight, unsigned char* pusSelectedImage)
{
	// input parameter check //
	if (pusSelectedImage == NULL) { return false; }
	if (m_pnFrameIdx == NULL) { return false; }
	if (m_pnWidth == NULL || m_pnHeight == NULL || m_pnWidth[nFrameIdx] != nWidth || m_pnHeight[nFrameIdx] != nHeight) { return false; }

	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			int idx = row*nWidth + col;
			m_ppucSelectedImage[nFrameIdx][idx] = pusSelectedImage[idx];
		}
	}

	return true;

}
bool CData::setSelectedImages(int nStartFrameIdx, int nEndFrameIdx, int nWidth, int nHeight, unsigned char** ppucSelectedImages)
{
	if (ppucSelectedImages == NULL) {
		return false;
	}
	int nCount = nEndFrameIdx - nStartFrameIdx + 1;

	for (int i = 0; i < nCount; i++) {
		int idx = nStartFrameIdx + i;
		setSelectedImage(idx, nWidth, nHeight, ppucSelectedImages[i]);
	}

	return true;
}
int CData::getWidth(int nFrameIdx)
{
	return m_pnWidth[nFrameIdx];
}
int CData::getHeight(int nFrameIdx)
{
	return m_pnHeight[nFrameIdx];
}
int CData::getGroupStartFrameIdx(int nGroupIdx)
{
	// 해당 group에 속하는 frame 갯수
	int nStartFrameIdx = -1;
	int nEndFrameIdx = -1;

	for (int i = 0; i < m_nFrameCnt; i++) {
		if ((m_pnFrameIdx[i] != -1) && (m_pnFrameGroupIdx[i] == nGroupIdx)) {
			if (nStartFrameIdx == -1) { nStartFrameIdx = i; }
			nEndFrameIdx = i;
		}
	}

	return nStartFrameIdx;
}
int CData::getGroupEndFrameIdx(int nGroupIdx)
{
	// 해당 group에 속하는 frame 갯수
	int nStartFrameIdx = -1;
	int nEndFrameIdx = -1;

	for (int i = 0; i < m_nFrameCnt; i++) {
		if ((m_pnFrameIdx[i] != -1) && (m_pnFrameGroupIdx[i] == nGroupIdx)) {
			if (nStartFrameIdx == -1) { nStartFrameIdx = i; }
			nEndFrameIdx = i;
		}
	}

	return nEndFrameIdx;
}

// private //
void CData::resizeDataArray(int nDataCnt)
{
	if (nDataCnt > m_nFrameCnt) {
		int* pnFrameIdxNew = new int[nDataCnt];
		int* pnFrameMemoryIdxNew = new int[nDataCnt];
		int* pnFrameGroupIdxNew = new int[nDataCnt];
		char** ppcPatientNameNew = new char*[nDataCnt];
		char** ppcInstitutionNameNew = new char*[nDataCnt];
		char** ppcSeriesDateNew = new char*[nDataCnt];
		char** ppcManufacturerModelNameNew = new char*[nDataCnt];
		char** ppcModalityNew = new char*[nDataCnt];
		float* pfPixelSpacingNew = new float[nDataCnt];
		float* pfSliceThicknessNew = new float[nDataCnt];
		int* pnInstanceNumberNew = new int[nDataCnt];
		int* pnRowsNew = new int[nDataCnt];
		int* pnColsNew = new int[nDataCnt];

		short** ppsRawImageNew = new short*[nDataCnt];
		unsigned char** ppsRawLabelImageNew = new unsigned char*[nDataCnt];
		unsigned char** ppsRawSelectedImageNew = new unsigned char*[nDataCnt];

		// new data copy
		for (int i = 0; i < m_nFrameCnt; i++) {
			pnFrameIdxNew[i] = m_pnFrameIdx[i];
			pnFrameMemoryIdxNew[i] = m_pnFrameMemoryIdx[i];
			pnFrameGroupIdxNew[i] = m_pnFrameGroupIdx[i];
			ppcPatientNameNew[i] = m_ppcPatientName[i];
			ppcInstitutionNameNew[i] = m_ppcInstitutionName[i];
			ppcSeriesDateNew[i] = m_ppcSeriesDate[i];
			ppcManufacturerModelNameNew[i] = m_ppcManufacturerModelName[i];
			ppcModalityNew[i] = m_ppcModality[i];
			pfPixelSpacingNew[i] = m_pfPixelSpacing[i];
			pfSliceThicknessNew[i] = m_pfSliceThickness[i];
			pnInstanceNumberNew[i] = m_pnInstanceNumber[i];
			pnRowsNew[i] = m_pnHeight[i];
			pnColsNew[i] = m_pnWidth[i];
			ppsRawImageNew[i] = m_ppsRawImage[i];
			ppsRawLabelImageNew[i] = m_ppucLabelImage[i];
			ppsRawSelectedImageNew[i] = m_ppucSelectedImage[i];
		}
		for (int i = m_nFrameCnt; i < nDataCnt; i++) {
			pnFrameIdxNew[i] = -1;
			pnFrameMemoryIdxNew[i] = -1;
			pnFrameGroupIdxNew[i] = -1;
			ppcPatientNameNew[i] = NULL;
			ppcInstitutionNameNew[i] = NULL;
			ppcSeriesDateNew[i] = NULL;
			ppcManufacturerModelNameNew[i] = NULL;
			ppcModalityNew[i] = NULL;
			pfPixelSpacingNew[i] = 0;
			pfSliceThicknessNew[i] = 0;
			pnInstanceNumberNew[i] = 0;
			pnRowsNew[i] = 0;
			pnColsNew[i] = 0;
			ppsRawImageNew[i] = NULL;
			ppsRawLabelImageNew[i] = NULL;
			ppsRawSelectedImageNew[i] = NULL;
		}

		// old data delete
		SAFE_DELETE_ARRAY(m_pnFrameIdx);
		SAFE_DELETE_ARRAY(m_pnFrameMemoryIdx);
		SAFE_DELETE_ARRAY(m_pnFrameGroupIdx);
		SAFE_DELETE_ARRAY(m_ppcPatientName);
		SAFE_DELETE_ARRAY(m_ppcInstitutionName);
		SAFE_DELETE_ARRAY(m_ppcSeriesDate);
		SAFE_DELETE_ARRAY(m_ppcManufacturerModelName);
		SAFE_DELETE_ARRAY(m_ppcModality);
		SAFE_DELETE_ARRAY(m_pfPixelSpacing);
		SAFE_DELETE_ARRAY(m_pfSliceThickness);
		SAFE_DELETE_ARRAY(m_pnHeight);
		SAFE_DELETE_ARRAY(m_pnWidth);
		SAFE_DELETE_ARRAY(m_pnInstanceNumber);
		SAFE_DELETE_ARRAY(m_ppsRawImage);
		SAFE_DELETE_ARRAY(m_ppucLabelImage);
		SAFE_DELETE_ARRAY(m_ppucSelectedImage);

		// 멤버변수에 값 할당
		m_pnFrameIdx = pnFrameIdxNew;
		m_pnFrameMemoryIdx = pnFrameMemoryIdxNew;
		m_pnFrameGroupIdx = pnFrameGroupIdxNew;
		m_ppcPatientName = ppcPatientNameNew;
		m_ppcInstitutionName = ppcInstitutionNameNew;
		m_ppcSeriesDate = ppcSeriesDateNew;
		m_ppcManufacturerModelName = ppcManufacturerModelNameNew;
		m_ppcModality = ppcModalityNew;
		m_pfPixelSpacing = pfPixelSpacingNew;
		m_pfSliceThickness = pfSliceThicknessNew;
		m_pnInstanceNumber = pnInstanceNumberNew;
		m_pnHeight = pnRowsNew;
		m_pnWidth = pnColsNew;
		m_ppsRawImage = ppsRawImageNew;
		m_ppucLabelImage = ppsRawLabelImageNew;
		m_ppucSelectedImage = ppsRawSelectedImageNew;

		m_nFrameCnt = nDataCnt;
	}
}
void CData::resizeGroupArray(int nGroupCnt)
{
	if (nGroupCnt > m_nGroupCnt) {
		int* pnGroupIdxNew = new int[nGroupCnt];
		char** ppcGroupPatientNameNew = new char*[nGroupCnt];
		char** ppcGroupSeriesDateNew = new char*[nGroupCnt];

		for (int i = 0; i < m_nGroupCnt; i++) {
			pnGroupIdxNew[i] = m_pnGroupIdx[i];
			ppcGroupPatientNameNew[i] = m_ppcGroupPatientName[i];
			ppcGroupSeriesDateNew[i] = m_ppcGroupSeriesDate[i];
		}
		for (int i = m_nGroupCnt; i < nGroupCnt; i++) {
			pnGroupIdxNew[i] = -1;
			ppcGroupPatientNameNew[i] = NULL;
			ppcGroupSeriesDateNew[i] = NULL;
		}

		// old data delete 
		SAFE_DELETE_ARRAY(m_pnGroupIdx);
		SAFE_DELETE_ARRAY(m_ppcGroupPatientName);
		SAFE_DELETE_ARRAY(m_ppcGroupSeriesDate);

		// 멤버변수에 값 할당
		m_pnGroupIdx = pnGroupIdxNew;
		m_ppcGroupPatientName = ppcGroupPatientNameNew;
		m_ppcGroupSeriesDate = ppcGroupSeriesDateNew;

		m_nGroupCnt = nGroupCnt;
	}
}
bool CData::readImage(char* pcImagePath, int& nWidth, int& nHeight, short* &psRawImage, unsigned char* &psRawLabelImage, unsigned char* &psRawSelectedImage, bool bMakeNewArray)
{
	// 0. input parameter 예외처리
	if (bMakeNewArray) {
		if (psRawImage != NULL || psRawLabelImage != NULL || psRawSelectedImage != NULL) {
			return false;
		}
	}

	// 1. 변수선언
	int nImageWidth = 0;
	int nImageHeight = 0;

	if (strcmp(getFileExtension(pcImagePath), "dcm") == 0) {
		DcmFileFormat *fileformat = new DcmFileFormat();
		fileformat->loadFile(pcImagePath);

		OFCondition status = fileformat->getDataset()->chooseRepresentation(EXS_LittleEndianExplicit, NULL);
		if (!status.good())
		{
			std::cerr << "DCMTK error: " << status.text() << std::endl;
			delete fileformat;
		}

		// image width, height
		DcmDataset* dataSet = fileformat->getDataset();
		OFString OFStrRows, OFStrColumns;
		dataSet->findAndGetOFString(DcmTagKey(0x0028, 0x0010), OFStrRows);
		dataSet->findAndGetOFString(DcmTagKey(0x0028, 0x0011), OFStrColumns);
		nImageWidth = atoi(OFStrRows.c_str());
		nImageHeight = atoi(OFStrColumns.c_str());

		// image
		unsigned long count;
		const Uint16 *pixelData;
		status = fileformat->getDataset()->findAndGetUint16Array(DCM_PixelData, pixelData, &count, OFTrue);

		// -3024 범위를 -1024~ 범위로 변경
		unsigned short* ucCopyPixelData = new unsigned short[nImageWidth*nImageHeight];

		int nMin = INT_MAX;
		int nMax = INT_MIN;
		for (int i = 0; i < nImageWidth*nImageHeight; i++) {
			if (pixelData[i] < nMin) {
				nMin = pixelData[i];
			}
			if (pixelData[i] > nMax) {
				nMax = pixelData[i];
			}

			ucCopyPixelData[i] = pixelData[i];
		}

		if (nMax > 10000)
		{
			int* piPixelData = new int[nImageWidth * nImageHeight];

			for (int i = 0; i < nImageHeight; i++)
			{
				for (int j = 0; j < nImageWidth; j++)
				{
					int iIdx = i*nImageWidth + j;
					if (ucCopyPixelData[iIdx] > 10000)
						piPixelData[iIdx] = ucCopyPixelData[iIdx] - 65536;
					else
						piPixelData[iIdx] = ucCopyPixelData[iIdx];
				}
			}

			int iTempMin = 99999, iTempMax = -99999;
			for (int i = 0; i < nImageHeight; i++)
			{
				for (int j = 0; j < nImageWidth; j++)
				{
					int iIdx = i*nImageWidth + j;

					if (piPixelData[iIdx] > iTempMax)
						iTempMax = piPixelData[iIdx];
					if (piPixelData[iIdx] < iTempMin)
						iTempMin = piPixelData[iIdx];
				}
			}

			if (iTempMin < 0)
			{
				iTempMin *= -1;
				for (int i = 0; i < nImageHeight; i++)
				{
					for (int j = 0; j < nImageWidth; j++)
					{
						int iIdx = i*nImageWidth + j;
						int iTempVal = piPixelData[iIdx] + iTempMin;
						if (iTempVal > 4095)
							iTempVal = 4095;

						// 데이터와 intensity 범위를 맞추기 위한 값 보정
						iTempVal -= 2000;
						if (iTempVal < 0)
							iTempVal = 0;

						ucCopyPixelData[iIdx] = (unsigned short)iTempVal;
					}
				}
			}

			delete[] piPixelData;
		}
		else
		{
			for (int i = 0; i < nImageHeight; i++)
			{
				for (int j = 0; j < nImageWidth; j++)
				{
					int iIdx = i*nImageWidth + j;

					if (ucCopyPixelData[iIdx] >= 4096 && ucCopyPixelData[iIdx] <= 5000)
						ucCopyPixelData[iIdx] = 4095;
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////////////
		if (bMakeNewArray) {
			nWidth = nImageWidth;
			nHeight = nImageHeight;
			psRawImage = new short[nWidth*nHeight];
			psRawLabelImage = new unsigned char[nWidth*nHeight];
			psRawSelectedImage = new unsigned char[nWidth*nHeight];
		}
		else {
			if ((nWidth != nImageWidth) || (nHeight != nImageHeight) || (psRawImage == NULL)) {
				SAFE_DELETE_ARRAY(psRawImage);
				SAFE_DELETE_ARRAY(psRawLabelImage);
				SAFE_DELETE_ARRAY(psRawSelectedImage);

				nWidth = nImageWidth;
				nHeight = nImageHeight;
				psRawImage = new short[nWidth*nHeight];
				psRawLabelImage = new unsigned char[nWidth*nHeight];
				psRawSelectedImage = new unsigned char[nWidth*nHeight];
			}
		}

		for (int row = 0; row < nHeight; row++) {
			for (int col = 0; col < nWidth; col++) {
				int index = row*nWidth + col;
				psRawImage[index] = ucCopyPixelData[index];
				psRawLabelImage[index] = 0;
				psRawSelectedImage[index] = 0;
			}
		}

		// delete memory //
		fileformat->clear();
		dataSet->clear();
		delete fileformat;
		SAFE_DELETE_ARRAY(ucCopyPixelData);
	}
	/*else if(strcmp(getFileExtension(pcImagePath), "jpg") == 0 || strcmp(getFileExtension(pcImagePath), "png") == 0) {
		IplImage* image = cvLoadImage(pcImagePath, 0);
		nImageWidth = image->width;
		nImageHeight = image->height;

		if(bMakeNewArray) {
			nWidth = nImageWidth;
			nHeight = nImageHeight;
			psRawImage = new short[nWidth*nHeight];
			psRawLabelImage = new unsigned char[nWidth*nHeight];
			psRawSelectedImage = new unsigned char[nWidth*nHeight];
		}
		else {
			if((nWidth != nImageWidth) || (nHeight != nImageHeight) || (psRawImage == NULL)) {
				SAFE_DELETE_ARRAY(psRawImage);
				SAFE_DELETE_ARRAY(psRawLabelImage);
				SAFE_DELETE_ARRAY(psRawSelectedImage);

				nWidth = nImageWidth;
				nHeight = nImageHeight;
				psRawImage = new short[nWidth*nHeight];
				psRawLabelImage = new unsigned char[nWidth*nHeight];
				psRawSelectedImage = new unsigned char[nWidth*nHeight];
			}
		}

		for(int row=0; row<nHeight; row++) {
			for(int col=0; col<nWidth; col++) {
				int index = row*nWidth + col;
				unsigned char value = (unsigned char)image->imageData[image->widthStep*row + col];
				psRawImage[index] = value;
				psRawLabelImage[index] = 0;
				psRawSelectedImage[index] = 0;
			}
		}

		cvReleaseImage(&image);
	}*/
	else if (strcmp(getFileExtension(pcImagePath), "jpg") == 0 || strcmp(getFileExtension(pcImagePath), "png") == 0) {
		IplImage* image = cvLoadImage(pcImagePath, 1);
		nImageWidth = image->width;
		nImageHeight = image->height;

		if (bMakeNewArray) {
			nWidth = nImageWidth;
			nHeight = nImageHeight;
			psRawImage = new short[image->widthStep*nHeight];
			psRawLabelImage = new unsigned char[nWidth*nHeight];
			psRawSelectedImage = new unsigned char[nWidth*nHeight];
		}
		else {
			if ((nWidth != nImageWidth) || (nHeight != nImageHeight) || (psRawImage == NULL)) {
				SAFE_DELETE_ARRAY(psRawImage);
				SAFE_DELETE_ARRAY(psRawLabelImage);
				SAFE_DELETE_ARRAY(psRawSelectedImage);

				nWidth = nImageWidth;
				nHeight = nImageHeight;
				psRawImage = new short[image->widthStep*nHeight];
				psRawLabelImage = new unsigned char[nWidth*nHeight];
				psRawSelectedImage = new unsigned char[nWidth*nHeight];
			}
		}

		for (int row = 0; row < nHeight; row++) {
			for (int col = 0; col < nWidth; col++) {
				int index = row*image->widthStep + col *image->nChannels;
				for (int ch = 0; ch < 3; ch++)
				{
					unsigned char value = (unsigned char)image->imageData[index + ch];
					psRawImage[index + ch] = value;

				}
				psRawLabelImage[row*nWidth + col] = 0;
				psRawSelectedImage[row*nWidth + col] = 0;

			}
		}


		unsigned char * tempimg = new unsigned char[image->widthStep*nHeight];

		for (int row = 0; row < nHeight; row++)
			for (int col = 0; col < nWidth; col++)
				for (int ch = 0; ch < 3; ch++)
				{
					int index = row*image->widthStep + col*image->nChannels;

					tempimg[index + ch] = image->imageData[index + ch];
				}


		cv::Mat tt(nHeight, nWidth, CV_8UC3, tempimg);

		cvReleaseImage(&image);
	}
	else {
		QMessageBox msgBox;
		msgBox.setText(QString::fromLocal8Bit("처리할 수 없는 파일 확장자입니다."));
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.exec();

		return false;
	}

	return true;
}
char* CData::getFileExtension(char* pcFileName)
{
	int file_name_len = strlen(pcFileName);
	pcFileName += file_name_len;

	char *file_ext;
	for (int i = 0; i < file_name_len; i++)
	{
		if (*pcFileName == '.')
		{
			file_ext = pcFileName + 1;
			break;
		}
		pcFileName--;
	}
	return file_ext;
}
char* CData::getFileName(char* pcFilePath)
{
	char *file_name;

	while (*pcFilePath)
	{
		if (*pcFilePath == '/' && (pcFilePath + 1) != NULL)
		{
			file_name = pcFilePath + 1;
		}

		pcFilePath++; //mv pointer       
	}
	return file_name;
}