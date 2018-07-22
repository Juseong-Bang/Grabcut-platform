/********************************************************************************
** Form generated from reading UI file 'platform.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLATFORM_H
#define UI_PLATFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_platformClass
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_ScreenHeight;
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *horizontalSpacer_ScreenWidth;
    QGridLayout *gridLayout_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QTabWidget *tabWidget_2;
    QWidget *tab_3;
    QWidget *layoutWidget_4;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_13;
    QDoubleSpinBox *doubleSpinBox_3;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_3;
    QDoubleSpinBox *doubleSpinBox;
    QPushButton *pushButton_run;
    QWidget *layoutWidget_5;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_14;
    QDoubleSpinBox *doubleSpinBox_2;
    QSpacerItem *verticalSpacer;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *platformClass)
    {
        if (platformClass->objectName().isEmpty())
            platformClass->setObjectName(QStringLiteral("platformClass"));
        platformClass->resize(910, 711);
        platformClass->setMaximumSize(QSize(16777215, 16777215));
        QIcon icon;
        icon.addFile(QStringLiteral("Resources/Deep Imaging Laboratory.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        platformClass->setWindowIcon(icon);
        platformClass->setAutoFillBackground(false);
        platformClass->setStyleSheet(QStringLiteral(""));
        centralWidget = new QWidget(platformClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        gridLayout_3 = new QGridLayout(centralWidget);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setSizeConstraint(QLayout::SetMaximumSize);
        horizontalLayout_3->setContentsMargins(5, -1, 5, -1);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetMinimumSize);

        horizontalLayout_3->addLayout(verticalLayout);

        verticalSpacer_ScreenHeight = new QSpacerItem(0, 600, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        horizontalLayout_3->addItem(verticalSpacer_ScreenHeight);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalSpacer_ScreenWidth = new QSpacerItem(600, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        verticalLayout_3->addItem(horizontalSpacer_ScreenWidth);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 5, 0);
        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setStyleSheet(QStringLiteral("background-color: rgb(227, 227, 227);"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);

        gridLayout_2->addWidget(frame, 0, 0, 1, 1);


        verticalLayout_3->addLayout(gridLayout_2);

        verticalLayout_3->setStretch(1, 1);

        horizontalLayout_3->addLayout(verticalLayout_3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setSizeConstraint(QLayout::SetMinimumSize);
        tabWidget_2 = new QTabWidget(centralWidget);
        tabWidget_2->setObjectName(QStringLiteral("tabWidget_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tabWidget_2->sizePolicy().hasHeightForWidth());
        tabWidget_2->setSizePolicy(sizePolicy1);
        tabWidget_2->setMinimumSize(QSize(258, 155));
        tabWidget_2->setMaximumSize(QSize(258, 640));
        tabWidget_2->setStyleSheet(QStringLiteral(""));
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        layoutWidget_4 = new QWidget(tab_3);
        layoutWidget_4->setObjectName(QStringLiteral("layoutWidget_4"));
        layoutWidget_4->setGeometry(QRect(0, 60, 257, 28));
        horizontalLayout_8 = new QHBoxLayout(layoutWidget_4);
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(0, 4, 0, 0);
        label_13 = new QLabel(layoutWidget_4);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setMinimumSize(QSize(94, 20));
        label_13->setMaximumSize(QSize(71, 20));
        label_13->setLayoutDirection(Qt::LeftToRight);
        label_13->setStyleSheet(QString::fromUtf8("color: rgb(23, 168, 26);\n"
"font: 75 10.5pt \"\353\247\221\354\235\200 \352\263\240\353\224\225\";"));
        label_13->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_8->addWidget(label_13);

        doubleSpinBox_3 = new QDoubleSpinBox(layoutWidget_4);
        doubleSpinBox_3->setObjectName(QStringLiteral("doubleSpinBox_3"));
        doubleSpinBox_3->setMaximumSize(QSize(100, 16777215));
        doubleSpinBox_3->setStyleSheet(QString::fromUtf8("font: 75 10.5pt \"\353\247\221\354\235\200 \352\263\240\353\224\225\";"));
        doubleSpinBox_3->setMinimum(-1000);
        doubleSpinBox_3->setMaximum(1000);
        doubleSpinBox_3->setSingleStep(0.1);
        doubleSpinBox_3->setValue(3);

        horizontalLayout_8->addWidget(doubleSpinBox_3);

        layoutWidget_2 = new QWidget(tab_3);
        layoutWidget_2->setObjectName(QStringLiteral("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(0, 0, 257, 28));
        horizontalLayout_6 = new QHBoxLayout(layoutWidget_2);
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 4, 0, 0);
        label_3 = new QLabel(layoutWidget_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMinimumSize(QSize(94, 20));
        label_3->setMaximumSize(QSize(71, 20));
        label_3->setLayoutDirection(Qt::LeftToRight);
        label_3->setStyleSheet(QString::fromUtf8("color: rgb(23, 168, 26);\n"
"font: 75 10.5pt \"\353\247\221\354\235\200 \352\263\240\353\224\225\";"));
        label_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_6->addWidget(label_3);

        doubleSpinBox = new QDoubleSpinBox(layoutWidget_2);
        doubleSpinBox->setObjectName(QStringLiteral("doubleSpinBox"));
        doubleSpinBox->setMaximumSize(QSize(100, 16777215));
        doubleSpinBox->setStyleSheet(QString::fromUtf8("font: 75 10.5pt \"\353\247\221\354\235\200 \352\263\240\353\224\225\";"));
        doubleSpinBox->setMinimum(-1000);
        doubleSpinBox->setMaximum(1000);
        doubleSpinBox->setSingleStep(0.1);
        doubleSpinBox->setValue(5);

        horizontalLayout_6->addWidget(doubleSpinBox);

        pushButton_run = new QPushButton(tab_3);
        pushButton_run->setObjectName(QStringLiteral("pushButton_run"));
        pushButton_run->setEnabled(true);
        pushButton_run->setGeometry(QRect(20, 99, 221, 25));
        pushButton_run->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 75 10.5pt \"\353\247\221\354\235\200 \352\263\240\353\224\225\";"));
        layoutWidget_5 = new QWidget(tab_3);
        layoutWidget_5->setObjectName(QStringLiteral("layoutWidget_5"));
        layoutWidget_5->setGeometry(QRect(0, 30, 257, 28));
        horizontalLayout_9 = new QHBoxLayout(layoutWidget_5);
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(0, 4, 0, 0);
        label_14 = new QLabel(layoutWidget_5);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setMinimumSize(QSize(94, 20));
        label_14->setMaximumSize(QSize(71, 20));
        label_14->setLayoutDirection(Qt::LeftToRight);
        label_14->setStyleSheet(QString::fromUtf8("color: rgb(23, 168, 26);\n"
"font: 75 10.5pt \"\353\247\221\354\235\200 \352\263\240\353\224\225\";"));
        label_14->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_9->addWidget(label_14);

        doubleSpinBox_2 = new QDoubleSpinBox(layoutWidget_5);
        doubleSpinBox_2->setObjectName(QStringLiteral("doubleSpinBox_2"));
        doubleSpinBox_2->setMaximumSize(QSize(100, 16777215));
        doubleSpinBox_2->setStyleSheet(QString::fromUtf8("font: 75 10.5pt \"\353\247\221\354\235\200 \352\263\240\353\224\225\";"));
        doubleSpinBox_2->setMinimum(-1000);
        doubleSpinBox_2->setMaximum(1000);
        doubleSpinBox_2->setSingleStep(0.1);
        doubleSpinBox_2->setValue(3);

        horizontalLayout_9->addWidget(doubleSpinBox_2);

        tabWidget_2->addTab(tab_3, QString());

        verticalLayout_2->addWidget(tabWidget_2);

        verticalSpacer = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        verticalLayout_2->addItem(verticalSpacer);


        horizontalLayout_3->addLayout(verticalLayout_2);


        gridLayout_3->addLayout(horizontalLayout_3, 0, 0, 1, 1);

        platformClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(platformClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 910, 21));
        platformClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(platformClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        platformClass->setStatusBar(statusBar);

        retranslateUi(platformClass);

        tabWidget_2->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(platformClass);
    } // setupUi

    void retranslateUi(QMainWindow *platformClass)
    {
        platformClass->setWindowTitle(QApplication::translate("platformClass", "\354\227\260\352\265\254\354\213\244 \355\224\214\353\236\253\355\217\274 \354\227\260\354\212\265\354\232\251", 0));
        label_13->setText(QApplication::translate("platformClass", "Param3", 0));
        label_3->setText(QApplication::translate("platformClass", "Param1", 0));
        pushButton_run->setText(QApplication::translate("platformClass", "\354\225\214\352\263\240\353\246\254\354\246\230 \354\210\230\355\226\211", 0));
        label_14->setText(QApplication::translate("platformClass", "Param2", 0));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_3), QApplication::translate("platformClass", "Tool", 0));
    } // retranslateUi

};

namespace Ui {
    class platformClass: public Ui_platformClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLATFORM_H
