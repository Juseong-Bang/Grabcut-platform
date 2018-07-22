#pragma once

#include <qevent.h>
#include <qpainter.h>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QMessageBox>
#include <qsignalmapper.h>

using namespace std;

class CPlatform;
class CImage: public QWidget
{
	Q_OBJECT

// 생성자, 소멸자, 초기화
public:
	CImage(QWidget *parent=0);
	~CImage();
	void init(void);
	void init(CPlatform* parent);
	void clear(void);

	void setParent(CPlatform* parent);
	void redraw(bool isMouseMove);
	bool setImage(unsigned char* pusImage, int nWidth, int nHeight);
	void setImageScreenSize(int nScreenWidth, int nScreenHeight);

	unsigned char* m_pucImage;						// image (scale 전)
	int m_nImageWidth;								// image 가로 (scale 전)
	int m_nImageHeight;								// image 세로 (scale 전)

	QPoint getMousePoint(void);						// mouse point (마지막으로 클릭한 좌표)
	QVector<QPoint> getMousePoints(void);			// mouse points (마우스 누른채로 움직였던 좌표들)

private:
	CPlatform* m_parent;
	QLabel* m_qImageScreenDrawingPosition;			// 화면 image 위치 (Layout 디자인에 맞도록 자동조절)
	QImage m_qImageScreen;							// 화면 image (scale 후)
	int m_nImageScreenWidth;						// 화면 image 가로 (scale 후)
	int m_nImageScreenHeight;						// 화면 image 세로 (scale 후)

	// image data //
	int m_nActivatedFrameIdx;
	QImage m_qImage;								// qimage temp (scale 전)

	// mouse mpoint
	QPoint m_qpMousePoint;							// 현재점
	QPoint m_qpLMouseDownPoint;						// 왼쪽 클릭했던 점
	QVector<QPoint> m_qpMouseTrackingPoints;		// 클릭 이후, 마우스 궤적
	bool m_bLMouseDown;								// mouse click
	bool m_bRMouseDown;								// mouse click(right)

	// point
	QPoint pointToWindowPoint(QPoint point);
	QPoint windowPointToPoint(QPoint windowPoint);

	void normalizeImage(short* psInImage, int nWidth, int nHeight, unsigned char* &pucOutImage);
	bool checkImageRange(int nX, int nY, int nMargin);
	void setSignalSlot();

protected:
	void paintEvent(QPaintEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void mouseDoubleClickEvent(QMouseEvent* event);
	void wheelEvent(QWheelEvent* event);
};