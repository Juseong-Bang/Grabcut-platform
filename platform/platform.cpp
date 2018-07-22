#include "platform.h"

CPlatform::CPlatform(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);			// ui setting 
	setMouseTracking(true);
	setAcceptDrops(true);

	init();
	createProgressBar();

	setSignalSlot();
}
CPlatform::~CPlatform()
{
	clear();
}

// Borderless Main Window
BorderlessMainWindow::BorderlessMainWindow(QWidget *parent) : QMainWindow(parent, Qt::CustomizeWindowHint ) {
	setObjectName("borderlessMainWindow");
	setWindowFlags(Qt::FramelessWindowHint| Qt::WindowSystemMenuHint);
	this->resize(1024, 680);

	mMainWindow = new CPlatform(this);
	setWindowTitle(mMainWindow->windowTitle());

	mMainWindow->statusBar()->setSizeGripEnabled(true);
	mMainWindow->setWindowFlags(Qt::FramelessWindowHint);

	verticalLayout = new QVBoxLayout();
	verticalLayout->setSpacing(0);
	verticalLayout->setMargin(0);

	horizontalLayout = new QHBoxLayout();
	horizontalLayout->setSpacing(0);
	horizontalLayout->setMargin(0);

	mTitlebarWidget = new QWidget(this);
	mTitlebarWidget->setObjectName("titlebarWidget");
	mTitlebarWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	mTitlebarWidget->setLayout(horizontalLayout);

	mMinimizeButton = new QPushButton(mTitlebarWidget);
	mMinimizeButton->setObjectName("minimizeButton");
	connect(mMinimizeButton, SIGNAL(clicked()), this, SLOT(slot_minimized()));

	mRestoreButton = new QPushButton(mTitlebarWidget);
	mRestoreButton->setObjectName("restoreButton");
	mRestoreButton->setVisible(false);
	connect(mRestoreButton, SIGNAL(clicked()), this, SLOT(slot_restored()));

	mMaximizeButton = new QPushButton(mTitlebarWidget);
	mMaximizeButton->setObjectName("maximizeButton");
	connect(mMaximizeButton, SIGNAL(clicked()), this, SLOT(slot_maximized()));

	mCloseButton = new QPushButton(mTitlebarWidget);
	mCloseButton->setObjectName("closeButton");
	connect(mCloseButton, SIGNAL(clicked()), this, SLOT(slot_closed()));

	mWindowIcon = new QLabel(mTitlebarWidget);
	mWindowIcon->setObjectName("windowIcon");
	mWindowIcon->setPixmap(QPixmap("Resources/Deep Imaging Laboratory2.bmp"));
	mWindowIcon->setStyleSheet("padding: 5px;");

	mWindowTitle = new QLabel(mTitlebarWidget);
	mWindowTitle->setObjectName("windowTitle");
	//mWindowTitle->setText(windowTitle());
	QString str = QString::fromLocal8Bit("연구실 플랫폼 개발용");
	mWindowTitle->setText(str);
	mWindowTitle->setStyleSheet("padding-left: 3px; font-size: 14px; font-weight: bold; color: #ffffff");

	mSeparateLine = new QFrame();
	mSeparateLine->setFrameShape(QFrame::HLine);
	mSeparateLine->setStyleSheet("border-top: 1px solid #666666;");

	horizontalLayout->addWidget(mWindowIcon);
	horizontalLayout->addWidget(mWindowTitle);
	horizontalLayout->addStretch(1);
	horizontalLayout->addWidget(mMinimizeButton);
	horizontalLayout->addWidget(mRestoreButton);
	horizontalLayout->addWidget(mMaximizeButton);
	horizontalLayout->addWidget(mCloseButton);

	verticalLayout->addWidget(mTitlebarWidget);
	verticalLayout->addWidget(mSeparateLine);
	verticalLayout->addWidget(mMainWindow);

	centralWidget = new QWidget(this);
	centralWidget->setObjectName("centralWidget");
	centralWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	centralWidget->setLayout(verticalLayout);

	setCentralWidget(centralWidget);
}
void BorderlessMainWindow::mousePressEvent(QMouseEvent* event) {
	if (!mTitlebarWidget->underMouse() && !mWindowTitle->underMouse())
		return;

	if(event->button() == Qt::LeftButton) {
		mMoving = true;
		mLastMousePosition = event->pos();
	}
}
void BorderlessMainWindow::mouseMoveEvent(QMouseEvent* event) {
	if (!mTitlebarWidget->underMouse() && !mWindowTitle->underMouse())
		return;

	if( event->buttons().testFlag(Qt::LeftButton) && mMoving) {
		this->move(this->pos() + (event->pos() - mLastMousePosition));
	}
}
void BorderlessMainWindow::mouseReleaseEvent(QMouseEvent* event) {
	if (!mTitlebarWidget->underMouse() && !mWindowTitle->underMouse())
	return;

	if(event->button() == Qt::LeftButton) {
		mMoving = false;
	}
}
void BorderlessMainWindow::mouseDoubleClickEvent(QMouseEvent *event) {
	Q_UNUSED(event);
	if (!mTitlebarWidget->underMouse() && !mWindowTitle->underMouse())
		return;

	mMaximized = !mMaximized;
	if (mMaximized) {
		slot_maximized();
	} else {
		slot_restored();
	}
}
void BorderlessMainWindow::slot_minimized() {
	setWindowState(Qt::WindowMinimized);
}
void BorderlessMainWindow::slot_restored() {
	mRestoreButton->setVisible(false);
	mMaximizeButton->setVisible(true);
	setWindowState(Qt::WindowNoState);
	setStyleSheet("#borderlessMainWindow{border:1px solid palette(highlight);}");
}
void BorderlessMainWindow::slot_maximized() {
	mRestoreButton->setVisible(true);
	mMaximizeButton->setVisible(false);
	showFullScreen();
	setStyleSheet("#borderlessMainWindow{border:1px solid palette(base);}");
}
void BorderlessMainWindow::slot_closed() {
	close();
}
void BorderlessMainWindow::resizeEvent(QResizeEvent* event)
{
	int nImageLayoutWidth = mMainWindow->ui.horizontalSpacer_ScreenWidth->geometry().width();
	int nImageLayoutHeight = mMainWindow->ui.verticalSpacer_ScreenHeight->geometry().height();

	// 너비는 변하질 않음...
	if(mMainWindow->m_ciImage != NULL) {
		int nWidth = 0;
		int nHeight = 0;
		if(nImageLayoutWidth > nImageLayoutHeight) {
			nWidth = nImageLayoutHeight;
			nHeight = nImageLayoutHeight * ((float)mMainWindow->m_ciImage->m_nImageHeight / (float)mMainWindow->m_ciImage->m_nImageWidth);
		}
		else {
			nWidth = nImageLayoutWidth;
			nHeight = nImageLayoutWidth * ((float)mMainWindow->m_ciImage->m_nImageHeight / (float)mMainWindow->m_ciImage->m_nImageWidth);
		}

		mMainWindow->m_ciImage->setImageScreenSize(nWidth,nHeight);
	}
}

// 초기화 //
void CPlatform::init()
{
	m_nActivatedFrameIdx = -1;
	m_ciImage = NULL;
}
void CPlatform::clear()
{
	if(m_ciImage != NULL) {
		delete m_ciImage;
		m_ciImage = NULL;
	}
}

// Widget 생성 //
void CPlatform::createProgressBar()
{
	progressBar = new QProgressBar(ui.statusBar);
	progressBar->setMaximumSize(250, 15);
	progressBar->setAlignment(Qt::AlignRight);
	progressBar->setValue(0);
	progressBar->setVisible(false);

	ui.statusBar->addPermanentWidget(progressBar);
}
void CPlatform::setSignalSlot()
{
	connect(ui.pushButton_run, SIGNAL(clicked()), this, SLOT(run()));
}
void CPlatform::setProgressBarValue(int nCurrentIdx, int nMaximumIdx)
{
	float fValue = (float)(nCurrentIdx+1) / (float)nMaximumIdx * 100;

	if(nCurrentIdx == 0) {
		progressBar->setVisible(true);
	}

	progressBar->setValue(fValue);

	if((nCurrentIdx+1) == nMaximumIdx) {
		progressBar->setValue(0);
		progressBar->setVisible(false);
	}
}

// 화면 resize event //
void CPlatform::resizeEvent(QResizeEvent* event)
{
	int nImageLayoutWidth = ui.horizontalSpacer_ScreenWidth->geometry().width();
	int nImageLayoutHeight = ui.verticalSpacer_ScreenHeight->geometry().height();

	// 너비는 변하질 않음...
	if(m_ciImage != NULL) {
		int nWidth = 0;
		int nHeight = 0;
		if(nImageLayoutWidth > nImageLayoutHeight) {
			nWidth = nImageLayoutHeight;
			nHeight = nImageLayoutHeight * ((float)m_ciImage->m_nImageHeight / (float)m_ciImage->m_nImageWidth);
		}
		else {
			nWidth = nImageLayoutWidth;
			nHeight = nImageLayoutWidth * ((float)m_ciImage->m_nImageHeight / (float)m_ciImage->m_nImageWidth);
		}

		m_ciImage->setImageScreenSize(nWidth,nHeight);
	}
}

// drag and drop event //
void CPlatform::dragEnterEvent(QDragEnterEvent* event)
{
	if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}
void CPlatform::dropEvent(QDropEvent* event)
{
	QStringList fileImageList;
	QStringList fileMaskList;

	QString fileName;
	QStringList fileToken;
	QString fileExtenstion;

	// Drag 한 파일을 확장자별로 분류
	foreach (const QUrl &url, event->mimeData()->urls()) {
		fileName = url.toLocalFile();
		fileToken = fileName.split('.');
		fileExtenstion = fileToken.at(fileToken.size()-1);

		QFileInfo f(fileName);
		if(f.isDir()) {
			QDirIterator list(fileName, QDir::Dirs|QDir::Files, QDirIterator::Subdirectories);
			do {
				list.next();
				if(list.fileInfo().isDir() == false) {
					fileName = list.filePath();
					fileToken = fileName.split('.');
					fileExtenstion = fileToken.at(fileToken.size() - 1);

					if(fileToken.size() > 1) {
						if(fileExtenstion.compare("bmp") == 0) {
							fileMaskList.push_back(fileName);
						}
						else {
							fileImageList.push_back(fileName);
						}
					}
				}
			} while (list.hasNext());
		}
		else {
			if(fileToken.size() > 1) {
				if(fileExtenstion.compare("bmp") == 0) {
					fileMaskList.push_back(fileName);
				}
				else {
					fileImageList.push_back(fileName);
				}
			}
		}
	}

	readDICOM(fileImageList);
}

// keyboard event //
void CPlatform::keyPressEvent(QKeyEvent* event)
{
	switch(event->key()) {
		case Qt::Key_Control:
			
		break;
	}
}
void CPlatform::keyReleaseEvent(QKeyEvent* event)
{
	switch(event->key()) {
		case Qt::Key_Control:
			
		break;
	}
}

// open, load, Image //
void CPlatform::readDICOM(QStringList fileList)
{
	if(fileList.size() > 0) {
		char cInputPath[1024] = {0};
		int nFileCount = fileList.size();
		for(int i=0; i< nFileCount; i++) {
			QString path = fileList.at(i).toUtf8().constData();
			QTextCodec* c = QTextCodec::codecForLocale();
			QByteArray b = c->fromUnicode(path);
			std::memcpy(cInputPath, b.data(), b.size() + 1);

			int idx = m_ciData.inputDICOM(cInputPath);
			if(idx == -1) {continue;}

			m_nActivatedFrameIdx = idx;
			char* pcPatientNum = m_ciData.getPatientName(idx);
			char* pcSeriesDate = m_ciData.getSeriesDate(idx);
			int pcInstanceNumber = m_ciData.getInstanceNumber(idx);

			setProgressBarValue(i, nFileCount);
		}
		showImage(m_nActivatedFrameIdx);
	}
}
void CPlatform::showImage(int nFrameIdx)
{
	m_nActivatedFrameIdx = nFrameIdx;
	short* psRawImage = m_ciData.getRawImage(nFrameIdx);
	unsigned char* psLabelImage = m_ciData.getLabelImage(nFrameIdx);
	unsigned char* psSelectedImage = m_ciData.getSelectedImage(nFrameIdx);
	int nWidth = m_ciData.getWidth(nFrameIdx);
	int nHeight = m_ciData.getHeight(nFrameIdx);

	// 처음 Data를 로드하는 경우
	if(m_ciImage == NULL) {
		m_ciImage = new CImage;

		m_ciImage->init(this);
		m_ciImage->setMinimumWidth(200);
		m_ciImage->setMinimumHeight(200);
		m_ciImage->setImageScreenSize(628, 628);

		ui.gridLayout_2->removeWidget(ui.frame);
		ui.gridLayout_2->addWidget(m_ciImage, 0, 0);
	}
	else {
		m_ciImage->init(this);
		m_ciImage->setMinimumWidth(200);
		m_ciImage->setMinimumHeight(200);
		m_ciImage->setImageScreenSize(628, 628);
	}

	// 넣고 다시
	int nImageLayoutWidth = ui.horizontalSpacer_ScreenWidth->geometry().width();
	int nImageLayoutHeight = ui.verticalSpacer_ScreenHeight->geometry().height();

	if(nImageLayoutWidth > nImageLayoutHeight) {
		nWidth = nImageLayoutHeight;
		nHeight = nImageLayoutHeight * ((float)m_ciImage->m_nImageHeight / (float)m_ciImage->m_nImageWidth);
	}
	else {
		nWidth = nImageLayoutWidth;
		nHeight = nImageLayoutWidth * ((float)m_ciImage->m_nImageHeight / (float)m_ciImage->m_nImageWidth);
	}

	m_ciImage->setImageScreenSize(nWidth,nHeight);
}
void CPlatform::showImage(QTreeWidgetItem* item, int column)
{
	if(item->text(1) != 0) {
		int nFrameIdx = item->text(1).toInt();
		showImage(nFrameIdx);
	}
}

// 알고리즘 //
void CPlatform::run()
{
	// paramter //
	//double fParam1 = ui.doubleSpinBox->value();
	//double fParam2 = ui.doubleSpinBox_2->value();
	//double fParam3 = ui.doubleSpinBox_3->value();

	// mousePoint // (mouse 좌표는 이미지 크기에 맞도록 자동 변환되어서 반환) (화면크기 x)
	// QPoint qpoint = m_ciImage->getMousePoint();	// Mouse 좌표 값 한개만 가져올때
	// QVector<QPoint> qpoints = m_ciImage->getMousePoints();	// 드래그

	// 배열 복사 //
	int nWidth = 0;
	int nHeight = 0;
	short* pusImage = NULL;
	unsigned char* pucImage = NULL;
	
	m_ciData.copyRawImage(0, nWidth, nHeight, pusImage);
	pucImage = new unsigned char[nWidth * nHeight];
	memset(pucImage, 0, sizeof(unsigned char) * nWidth * nHeight);

	// 알고리즘 수행 //
	// sample code
	for(int row=0; row< nHeight; row++) {
		for(int col=0; col< nWidth; col++) {
			int index = row*nWidth + col;
			pucImage[index] = 255 - pusImage[index];
		}
	}

	// 결과값 메모리에 복사 //
	m_ciImage->setImage(pucImage, nWidth, nHeight);

	// 메모리 소멸 //
	SAFE_DELETE_ARRAY(pusImage);
	SAFE_DELETE_ARRAY(pucImage);
}