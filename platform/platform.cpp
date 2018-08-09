#include "platform.h"
#include <queue>
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
BorderlessMainWindow::BorderlessMainWindow(QWidget *parent) : QMainWindow(parent, Qt::CustomizeWindowHint) {
	setObjectName("borderlessMainWindow");
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
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

	if (event->button() == Qt::LeftButton) {
		mMoving = true;
		mLastMousePosition = event->pos();
	}
}
void BorderlessMainWindow::mouseMoveEvent(QMouseEvent* event) {
	if (!mTitlebarWidget->underMouse() && !mWindowTitle->underMouse())
		return;

	if (event->buttons().testFlag(Qt::LeftButton) && mMoving) {
		this->move(this->pos() + (event->pos() - mLastMousePosition));
	}
}
void BorderlessMainWindow::mouseReleaseEvent(QMouseEvent* event) {
	if (!mTitlebarWidget->underMouse() && !mWindowTitle->underMouse())
		return;

	if (event->button() == Qt::LeftButton) {
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
	}
	else {
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
	if (mMainWindow->m_ciImage != NULL) {
		int nWidth = 0;
		int nHeight = 0;
		if (nImageLayoutWidth > nImageLayoutHeight) {
			nWidth = nImageLayoutHeight;
			nHeight = nImageLayoutHeight * ((float)mMainWindow->m_ciImage->m_nImageHeight / (float)mMainWindow->m_ciImage->m_nImageWidth);
		}
		else {
			nWidth = nImageLayoutWidth;
			nHeight = nImageLayoutWidth * ((float)mMainWindow->m_ciImage->m_nImageHeight / (float)mMainWindow->m_ciImage->m_nImageWidth);
		}

		mMainWindow->m_ciImage->setImageScreenSize(nWidth, nHeight);
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
	if (m_ciImage != NULL) {
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
	float fValue = (float)(nCurrentIdx + 1) / (float)nMaximumIdx * 100;

	if (nCurrentIdx == 0) {
		progressBar->setVisible(true);
	}

	progressBar->setValue(fValue);

	if ((nCurrentIdx + 1) == nMaximumIdx) {
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
	if (m_ciImage != NULL) {
		int nWidth = 0;
		int nHeight = 0;
		if (nImageLayoutWidth > nImageLayoutHeight) {
			nWidth = nImageLayoutHeight;
			nHeight = nImageLayoutHeight * ((float)m_ciImage->m_nImageHeight / (float)m_ciImage->m_nImageWidth);
		}
		else {
			nWidth = nImageLayoutWidth;
			nHeight = nImageLayoutWidth * ((float)m_ciImage->m_nImageHeight / (float)m_ciImage->m_nImageWidth);
		}

		m_ciImage->setImageScreenSize(nWidth, nHeight);


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
	foreach(const QUrl &url, event->mimeData()->urls()) {
		fileName = url.toLocalFile();
		fileToken = fileName.split('.');
		fileExtenstion = fileToken.at(fileToken.size() - 1);

		QFileInfo f(fileName);
		if (f.isDir()) {
			QDirIterator list(fileName, QDir::Dirs | QDir::Files, QDirIterator::Subdirectories);
			do {
				list.next();
				if (list.fileInfo().isDir() == false) {
					fileName = list.filePath();
					fileToken = fileName.split('.');
					fileExtenstion = fileToken.at(fileToken.size() - 1);

					if (fileToken.size() > 1) {
						if (fileExtenstion.compare("bmp") == 0) {
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
			if (fileToken.size() > 1) {
				if (fileExtenstion.compare("bmp") == 0) {
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
	switch (event->key()) {
	case Qt::Key_F8:
		run();
		break;
	case Qt::Key_Shift:
		m_ciImage->shift_key = true;
		break;
	case Qt::Key_Control:
		m_ciImage->ctrl_key = true;
		break;

	}

}
void CPlatform::keyReleaseEvent(QKeyEvent* event)
{
	switch (event->key()) {
	case Qt::Key_Shift:
		m_ciImage->shift_key = false;
		break;
	case Qt::Key_Control:
		m_ciImage->ctrl_key = false;
		break;
	}
}

// open, load, Image //
void CPlatform::readDICOM(QStringList fileList)
{
	if (fileList.size() > 0) {
		char cInputPath[1024] = { 0 };
		int nFileCount = fileList.size();
		for (int i = 0; i < nFileCount; i++) {
			QString path = fileList.at(i).toUtf8().constData();
			QTextCodec* c = QTextCodec::codecForLocale();
			QByteArray b = c->fromUnicode(path);
			std::memcpy(cInputPath, b.data(), b.size() + 1);

			int idx = m_ciData.inputDICOM(cInputPath);
			if (idx == -1) { continue; }

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
	stat = false;
	// 처음 Data를 로드하는 경우
	if (m_ciImage == NULL) {
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

	if (nImageLayoutWidth > nImageLayoutHeight) {
		nWidth = nImageLayoutHeight;
		nHeight = nImageLayoutHeight * ((float)m_ciImage->m_nImageHeight / (float)m_ciImage->m_nImageWidth);
	}
	else {
		nWidth = nImageLayoutWidth;
		nHeight = nImageLayoutWidth * ((float)m_ciImage->m_nImageHeight / (float)m_ciImage->m_nImageWidth);
	}

	m_ciImage->setImageScreenSize(nWidth, nHeight);
}
void CPlatform::showImage(QTreeWidgetItem* item, int column)
{
	if (item->text(1) != 0) {
		int nFrameIdx = item->text(1).toInt();
		showImage(nFrameIdx);
	}
}
void CPlatform::getBinMask(const cv::Mat & comMask, cv::Mat & binMask)
{
	if (comMask.empty() || comMask.type() != CV_8UC1)
		std::cout << "comMask is empty or has incorrect type (not CV_8UC1)\n";
	if (binMask.empty() || binMask.rows != comMask.rows || binMask.cols != comMask.cols)
		binMask.create(comMask.size(), CV_8UC1);
	binMask = comMask & 1;

	cv::Mat mask_tmp = binMask * 255;
	binMask.setTo(0);

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(mask_tmp, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, cv::Point(0, 0));

	//close segmentations (remove holes) + filter segmentations smaller than XX px
	std::vector<double> areas(contours.size(), 0);
	double max_area = 0;
	for (size_t i = 0, s = contours.size(); i < s; i++) {
		double a = cv::contourArea(contours[i], false);  //  Find the area of contour
		if (a > max_area)
			max_area = a;
		areas[i] = a;
	}

	double thr = 0.05*max_area;
	for (size_t i = 0, s = contours.size(); i < s; i++) {
		if (areas[i] > thr) {
			cv::drawContours(binMask, contours, i, cv::Scalar(1), CV_FILLED);
		}
	}
	cv::dilate(binMask, binMask, cv::Mat());
	cv::erode(binMask, binMask, cv::Mat());


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
	QVector<QPoint> qpoints = m_ciImage->getMousePoints();	// 드래그
		// 배열 복사 //
	int nWidth = 0;
	int nHeight = 0;
	short* pusImage = NULL;
	unsigned char* pucImage = NULL;
	int xst = qpoints.front().x();
	int xed = qpoints.last().x();
	int yst = qpoints.front().y();
	int yed = qpoints.last().y();

	cv::Mat prmask(nWidth, nHeight, CV_8UC1);//결과 마스크 
	cv::Mat pmask(nWidth, nHeight, CV_8UC1);//임시 마스크 

	if (stat == false) {
		roi = cv::Rect(cv::Point2i(xst, yst), cv::Point2i(xed, yed));
		mask = cv::Mat(nWidth, nHeight, CV_8UC1);
		bg = cv::Mat(nHeight, nWidth, CV_8UC1);
		fg = cv::Mat(nHeight, nWidth, CV_8UC1);
	}

	m_ciData.copyRawImage(0, nWidth, nHeight, pusImage);
	pucImage = new unsigned char[nWidth * nHeight * 3];
	memset(pucImage, 0, sizeof(unsigned char) * nWidth * nHeight);

	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++)
			for (int ch = 0; ch < 3; ch++)
			{
				int index = row*nWidth * 3 + col * 3;
				pucImage[index + ch] = pusImage[index + ch];
			}
	}

	cv::Mat image(nHeight, nWidth, CV_8UC3, pucImage);
	cv::Mat foreground(image.size(), CV_8UC3, cv::Scalar(255, 255, 255));
	cv::Mat background(image.size(), CV_8UC3, cv::Scalar(255, 255, 255));



	if (stat == false) {
		cv::grabCut(image, mask, roi, bg, fg, 1, cv::GC_INIT_WITH_RECT);
		stat = true;
	}
	else {
		//cv::imshow("before mask", mask * 60);
		/*
				cv::Mat	bbg = cv::Mat(m_ciImage->m_bg_mask.height(), m_ciImage->m_bg_mask.width(), CV_8UC4, const_cast<uchar*>(m_ciImage->m_bg_mask.bits()), m_ciImage->m_bg_mask.bytesPerLine());
				cv::Mat	ffg = cv::Mat(m_ciImage->m_fg_mask.height(), m_ciImage->m_fg_mask.width(), CV_8UC4, const_cast<uchar*>(m_ciImage->m_fg_mask.bits()), m_ciImage->m_fg_mask.bytesPerLine());

				cv::cvtColor(bbg, bbg, CV_RGB2GRAY);
				cv::cvtColor(ffg, ffg, CV_RGB2GRAY);

				cv::threshold(bbg, bbg, 125, 255, cv::THRESH_BINARY);
				cv::threshold(ffg, ffg, 125, 255, cv::THRESH_BINARY);
		*/
		cv::grabCut(image, mask, roi, bg, fg, 1, cv::GC_INIT_WITH_MASK);
	}
	//cv::imshow("after mask", mask * 60);
	cv::compare(mask, cv::GC_FGD, prmask, cv::CMP_EQ);
	cv::compare(mask, cv::GC_PR_FGD, pmask, cv::CMP_EQ);

	prmask += pmask;
	pmask = mask * 60;

	cv::dilate(prmask, prmask, cv::Mat());
	cv::erode(prmask, prmask, cv::Mat());

	/*image.copyTo(foreground, prmask);
	image.copyTo(background, ~prmask);*/
	queue<pair<int, int>> q; //row,col
	pmask.setTo(0);
	
	int ir[4] = { 0,0,-1,1 };
	int ic[4] = { -1,1,0,0 };
	int cnt = 1;
	int mx = 0, r, c;
	unsigned char mxg=0 ,gr = 0;
	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++)
			if (prmask.at<unsigned char>(row, col) > 0 && pmask.at<unsigned char>(row, col) == 0)
			{
				cnt = 1;
				gr++;
				q.push(make_pair(row, col));
				while (!q.empty())
				{
					r = q.front().first;
					c = q.front().second;
					q.pop();
					for (int a = 0; a < 4; a++)
					{
						int nr = r + ir[a];
						int nc = c + ic[a];
						if (nr < 0 || nc < 0 || nHeight <= nr || nWidth <= nc)
							continue;
						if (prmask.at<unsigned char>(nr, nc) > 0 && pmask.at<unsigned char>(nr, nc) == 0) {
							pmask.at<unsigned char>(nr, nc) = gr;
							q.push(make_pair(nr, nc));
							cnt++;
						}
					}
				}

				if (cnt > mx)
				{
					mx = cnt;
					mxg = gr;
				}
			}
	}

	if(mxg >0 && mx>0)
	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++)
		{
			if (pmask.at<unsigned char>(row, col) != mxg)
				prmask.at<unsigned char>(row, col) = 0;

		}
	}
	image.copyTo(foreground, prmask);
	image.copyTo(background, ~prmask);

	background *= 0.5;
	foreground.copyTo(background, prmask);

	/*cv::imshow("Foreground", foreground);
	cv::imshow("back", background);


*/

	pucImage = background.data;

	/*
		for (int row = 0; row < nHeight; row++) {
			for (int col = 0; col < nWidth; col++)
				for (int ch = 0; ch < 3; ch++) {
					int index = row*nWidth * 3 + col * 3;
					if ((yst == row || row == yed) && (col <= xed && xst <= col))
					{
						pucImage[index + ch] = 255;
					}
					else if ((yst <= row && row <= yed) && (col == xed || xst == col))
					{
						pucImage[index + ch] = 255;
					}
					else
						pucImage[index + ch] = pusImage[index + ch];
				}
		}

		*/
		// 결과값 메모리에 복사 //
	m_ciImage->setImage(pucImage, nWidth, nHeight);
	m_ciImage->m_mask = QImage(nWidth, nHeight, QImage::Format_RGB32);
	m_ciImage->redraw(false);

	// 메모리 소멸 //
	SAFE_DELETE_ARRAY(pusImage);
	//SAFE_DELETE_ARRAY(pucImage);
}