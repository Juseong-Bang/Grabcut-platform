#ifndef platform_H
#define platform_H
#include <QtWidgets/QMainWindow>
#include <qtreewidget.h>
#include <QMouseEvent>
#include <qfiledialog.h>
#include <qtoolbar.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qtextcodec.h>
#include <QMimeData>
#include <qprogressbar.h>
#include <direct.h>
#include <vector>
#include <string>
#include <QDirIterator>

#include "ui_platform.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/config/osconfig.h" 
#include "dcmtk/dcmdata/dctk.h"

#include<cv.h>
#include<highgui.h>
#include<cxcore.h>

#include "image.h"
#include "data.h"

using namespace std;


#define MAX_FILE_LENGTH 64
#define SAFE_DELETE_ARRAY(p)	{ if(p) delete[](p); p = NULL;}
#define SAFE_DELETE_VOLUME(p, depth)	{ if(p) {for (int i=0;i<depth;i++)	if(p[i]) delete[](p[i]); } delete[] p; p=NULL;};

class CPlatform : public QMainWindow
{
	Q_OBJECT

// ������, �Ҹ���, �ʱ�ȭ
public:
	CPlatform(QWidget *parent = 0);
	~CPlatform();
	void init();
	void clear();

public:
	CImage* m_ciImage;					// image
	CData m_ciData;						// data
	int m_nActivatedFrameIdx;			// ���� ȭ�鿡 ����ȭ�� Frame ��ȣ
	bool stat; //iter ���� 
	cv::Rect roi;
	cv::Mat mask;// �ݺ� ����ϴ� ����ũ
	cv::Mat bg;
	cv::Mat fg;

	vector<vector<cv::Point>> contours;
	vector<cv::Vec4i> hierarchy;

	// QT layout, action ����
public:
	// layout
	Ui::platformClass ui;
	QToolBar* fileToolBar;
	QMenu* fileMenu;
	QAction* newFileAction;
	QProgressBar* progressBar;
	
	void createProgressBar();
	void setSignalSlot();
	void setProgressBarValue(int nCurrentIdx, int nMaximumIdx);
protected:
	void keyPressEvent(QKeyEvent* event);
	void keyReleaseEvent(QKeyEvent* event);
	void dropEvent(QDropEvent* event);
	void dragEnterEvent(QDragEnterEvent* event);
	void resizeEvent(QResizeEvent* event);
	
public slots:
	// load + fileDirectory
	void readDICOM(QStringList fileList);
	void showImage(int nFrameIdx);
	void showImage(QTreeWidgetItem* item, int column);
	
	//
	void run();
	void CPlatform::getBinMask(const cv::Mat & comMask, cv::Mat & binMask);
};

class BorderlessMainWindow: public QMainWindow
{
	Q_OBJECT
public:
	explicit BorderlessMainWindow(QWidget *parent = 0);
	~BorderlessMainWindow() {}
protected:
	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void resizeEvent(QResizeEvent* event);
private slots:
	void slot_minimized();
	void slot_restored();
	void slot_maximized();
	void slot_closed();
private:
	CPlatform *mMainWindow;
	QWidget *mTitlebarWidget;
	QFrame *mSeparateLine;
	QLabel *mWindowIcon;
	QLabel *mWindowTitle;
	QPushButton *mMinimizeButton;
	QPushButton *mRestoreButton;
	QPushButton *mMaximizeButton;
	QPushButton *mCloseButton;
	QPoint mLastMousePosition;

	QVBoxLayout *verticalLayout;
	QHBoxLayout *horizontalLayout;
	QWidget *centralWidget;

	bool mMoving;
	bool mMaximized;
};

#endif // platform_H
