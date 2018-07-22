#include <image.h>
#include "platform.h"

// 생성자, 소멸자, 초기화 //
CImage::CImage(QWidget* parent)
	: QWidget(parent)
{
	setMouseTracking(true);

	m_qImageScreenDrawingPosition = new QLabel(this);
    m_qImageScreenDrawingPosition->setObjectName(QStringLiteral("label"));
    m_qImageScreenDrawingPosition->setGeometry(QRect(0, 0, 512, 512));
    m_qImageScreenDrawingPosition->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
	m_qImageScreenDrawingPosition->setMouseTracking(true);

	init();
}
CImage::~CImage()
{
	clear();
}
void CImage::init()
{
	// 멤버변수 //
	m_parent = NULL;
	m_pucImage = NULL;

	m_nActivatedFrameIdx = -1;
	m_nImageWidth = 0;
	m_nImageHeight = 0;
	m_nImageScreenWidth = 628;
	m_nImageScreenHeight = 628;

	// mouse
	m_bLMouseDown = false;
	m_bRMouseDown = false;
}
void CImage::init(CPlatform* parent)
{
	// 멤버변수 //
	setParent(parent);
	m_nActivatedFrameIdx = m_parent->m_nActivatedFrameIdx;

	int nWidth = m_parent->m_ciData.getWidth(m_nActivatedFrameIdx);
	int nHeight = m_parent->m_ciData.getHeight(m_nActivatedFrameIdx);
	if(nWidth != m_nImageWidth || nHeight != m_nImageHeight) {
		m_qImage = QImage(nWidth, nHeight, QImage::Format_RGB32);
		SAFE_DELETE_ARRAY(m_pucImage);

		m_nImageWidth = nWidth;
		m_nImageHeight = nHeight;
		short* psImage = m_parent->m_ciData.getRawImage(m_nActivatedFrameIdx);
		normalizeImage(psImage, nWidth, nHeight, m_pucImage);
	}

	// mouse
	m_bLMouseDown = false;
	m_bRMouseDown = false;
}
void CImage::clear()
{
	if(m_qImageScreenDrawingPosition != NULL) {
		delete m_qImageScreenDrawingPosition;
	}
	SAFE_DELETE_ARRAY(m_pucImage);

	m_qpMouseTrackingPoints.clear();
}

// Setting //
void CImage::setParent(CPlatform* parent)
{
	m_parent = parent;
}
void CImage::setSignalSlot()
{
	
}
void CImage::setImageScreenSize(int nScreenWidth, int nScreenHeight)
{
	this->setMaximumWidth(nScreenWidth);
	this->setMaximumHeight(nScreenHeight);

	// XA Copy
	// 기본 XA 영상
	for(int row=0; row< m_nImageHeight; row++) {
		for(int col=0; col<m_nImageWidth; col++) {
			int index = row*m_nImageWidth + col;
			int value = 0;

			m_qImage.setPixel(col, row, qRgb(value, value, value));
		}
	}
	m_nImageScreenWidth = nScreenWidth;
	m_nImageScreenHeight = nScreenHeight;

	m_qImageScreenDrawingPosition->setGeometry(QRect(0, 0, m_nImageScreenWidth, m_nImageScreenHeight));
	redraw(false);
}
bool CImage::checkImageRange(int nX, int nY, int nMargin)
{
	if( (nX>=nMargin) && (nX<m_nImageWidth-nMargin) && (nY>=nMargin) && (nY<m_nImageHeight-nMargin) ) {
		return true;
	}
	return false;
}
void CImage::normalizeImage(short* psInImage, int nWidth, int nHeight, unsigned char* &pucOutImage)
{
	if(pucOutImage != NULL) {
		SAFE_DELETE_ARRAY(pucOutImage);
	}
	pucOutImage = new unsigned char[nWidth * nHeight];

	int nMaxValue = INT_MIN;
	int nMinValue = INT_MAX;

	for(int row=0; row< nHeight; row++) {
		for(int col=0; col<nWidth; col++) {
			int index = row*nWidth + col;
			int value = psInImage[index];

			if(value > nMaxValue) {
				nMaxValue = value;
			}
			else if(value < nMinValue) {
				nMinValue = value;
			}
		}
	}

	int nWindowWidth = nMaxValue-nMinValue;
	int nWindowLevel = (nWindowWidth/2) + (nMinValue);
	int nWindowMax = nWindowLevel + (int)nWindowWidth/2;
	int nWindowMin = nWindowLevel - (int)nWindowWidth/2;

	for(int row=0; row< nHeight; row++) {
		for(int col=0; col< nWidth; col++) {
			int index = row*nWidth + col;
			int value = (int)psInImage[index];

			// 
			if(nWindowMin > value) {
				pucOutImage[index] = 0;
			}
			else if(nWindowMax < value) {
				pucOutImage[index] = 255;
			}
			else {
				int normValue = (float)(value-nWindowMin)* (255) / (float)(nWindowMax-nWindowMin+1);
				pucOutImage[index] = normValue;
			}
		}
	}
}
bool CImage::setImage(unsigned char* pusImage, int nWidth, int nHeight)
{
	if(pusImage == NULL) {return false;}
	if(m_pucImage == NULL) {return false;}

	for(int row=0; row< nHeight; row++) {
		for(int col=0; col< nWidth; col++) {
			int index = row*nWidth + col;
			m_pucImage[index] = pusImage[index];
		}
	}

	redraw(false);
	return true;
}

// point transform //
QPoint CImage::pointToWindowPoint(QPoint point)
{
	QPoint windowPoint;

	windowPoint.setX( ((point.x() / (float)m_nImageWidth * (float)m_nImageScreenWidth) + 0.5)   );
	windowPoint.setY( ((point.y() / (float)m_nImageHeight * (float)m_nImageScreenHeight) + 0.5) );

	return windowPoint;
}
QPoint CImage::windowPointToPoint(QPoint windowPoint)
{
	QPoint point;

	point.setX( ((windowPoint.x() / (float)m_nImageScreenWidth * (float)m_nImageWidth) + 0.5)   );
	point.setY( ((windowPoint.y() / (float)m_nImageScreenHeight * (float)m_nImageHeight) + 0.5) );

	return point;
}

QPoint CImage::getMousePoint(void)
{
	return m_qpMousePoint;
}
QVector<QPoint> CImage::getMousePoints(void)
{
	QVector<QPoint> result;
	
	for(int i=0; i< m_qpMouseTrackingPoints.size(); i++) {
		result.push_back(m_qpMouseTrackingPoints[i]);
	}

	return result;
}

// redraw //
void CImage::redraw(bool isMouseMove)
{
	if(m_pucImage == NULL) {return;}

	// 기본 이미지, Label 이미지, Selected Region
	if(isMouseMove == false) {
		// 기본 XA 영상 (0~255로 정규화)
		for(int row=0; row< m_nImageHeight; row++) {
			for(int col=0; col<m_nImageWidth; col++) {
				int index = row*m_nImageWidth + col;
				int value = m_pucImage[index];

				m_qImage.setPixel(col, row, qRgb(value, value, value));
			}
		}

		// scaling
		m_qImageScreen = m_qImage.scaled(QSize(m_nImageScreenWidth, m_nImageScreenHeight), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}
}

// Event //
void CImage::mousePressEvent(QMouseEvent* event)
{
	Q_UNUSED(event);

	int nMouseX = ( (event->x() / (float)m_nImageScreenWidth * (float)m_nImageWidth) + 0.5);
	int nMouseY = ( (event->y() / (float)m_nImageScreenHeight * (float)m_nImageHeight) + 0.5);
	
	if(event->button() == Qt::LeftButton) {
		m_bLMouseDown = true;
		m_qpLMouseDownPoint = QPoint(nMouseX, nMouseY);

		if(checkImageRange(nMouseX, nMouseY, 2)) {
			m_qpMousePoint = QPoint(nMouseX, nMouseY);
			m_qpMouseTrackingPoints.clear();
			m_qpMouseTrackingPoints.push_back(m_qpMousePoint);
		}
	}
	else if(event->button() == Qt::RightButton) {

	}
	
	redraw(false);
}
void CImage::mouseMoveEvent(QMouseEvent* event)
{
	Q_UNUSED(event);

	int nMouseX = ( (event->x() / (float)m_nImageScreenWidth * (float)m_nImageWidth) + 0.5);
	int nMouseY = ( (event->y() / (float)m_nImageScreenHeight * (float)m_nImageHeight) + 0.5);

	if(checkImageRange(nMouseX, nMouseY, 2)) {
		m_qpMousePoint = QPoint(nMouseX, nMouseY);

		if((m_bLMouseDown||m_bRMouseDown)) {
			m_qpMouseTrackingPoints.push_back(m_qpMousePoint);
			redraw(true);
		}
	}
	else {
		
	}
}
void CImage::mouseReleaseEvent(QMouseEvent* event)
{
	Q_UNUSED(event);

	int nMouseX = ( (event->x() / (float)m_nImageScreenWidth * (float)m_nImageWidth) + 0.5);
	int nMouseY = ( (event->y() / (float)m_nImageScreenHeight * (float)m_nImageHeight) + 0.5);
	//qDebug("mouse up event(%d, %d)", nMouseX, nMouseY);

	if(event->button() == Qt::LeftButton) {
		m_bLMouseDown = false;

		//
		if(checkImageRange(nMouseX, nMouseY, 2)) {
			m_qpMousePoint = QPoint(nMouseX, nMouseY);
			//m_qpMouseTrackingPoints.clear();
		}
	}
	else if(event->button() == Qt::RightButton) {
		m_bRMouseDown = false;
	}

	redraw(false);
}
void CImage::mouseDoubleClickEvent(QMouseEvent* event)
{

}
void CImage::wheelEvent(QWheelEvent* event)
{
	
}
void CImage::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);

	if(m_nImageWidth > 0) {
		QPixmap p = QPixmap::fromImage(m_qImageScreen);
		m_qImageScreenDrawingPosition->setPixmap(p);
	}
}