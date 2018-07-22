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

// ������, �Ҹ���, �ʱ�ȭ
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

	unsigned char* m_pucImage;						// image (scale ��)
	int m_nImageWidth;								// image ���� (scale ��)
	int m_nImageHeight;								// image ���� (scale ��)

	QPoint getMousePoint(void);						// mouse point (���������� Ŭ���� ��ǥ)
	QVector<QPoint> getMousePoints(void);			// mouse points (���콺 ����ä�� �������� ��ǥ��)

private:
	CPlatform* m_parent;
	QLabel* m_qImageScreenDrawingPosition;			// ȭ�� image ��ġ (Layout �����ο� �µ��� �ڵ�����)
	QImage m_qImageScreen;							// ȭ�� image (scale ��)
	int m_nImageScreenWidth;						// ȭ�� image ���� (scale ��)
	int m_nImageScreenHeight;						// ȭ�� image ���� (scale ��)

	// image data //
	int m_nActivatedFrameIdx;
	QImage m_qImage;								// qimage temp (scale ��)

	// mouse mpoint
	QPoint m_qpMousePoint;							// ������
	QPoint m_qpLMouseDownPoint;						// ���� Ŭ���ߴ� ��
	QVector<QPoint> m_qpMouseTrackingPoints;		// Ŭ�� ����, ���콺 ����
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