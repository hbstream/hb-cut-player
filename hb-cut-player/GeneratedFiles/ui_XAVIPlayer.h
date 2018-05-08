/********************************************************************************
** Form generated from reading UI file 'XAVIPlayer.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XAVIPLAYER_H
#define UI_XAVIPLAYER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_XAVIPlayerClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_3;
    QFrame *frame;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayoutV;
    QSlider *sliderVideo;
    QSlider *sliderAudio;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnOpen;
    QPushButton *btnStart;
    QPushButton *btnStop;
    QPushButton *btnIncrease;
    QPushButton *btnDecrease;
    QPushButton *btnInc10;
    QPushButton *btnDec10;
    QPushButton *btnRateReset;
    QSpacerItem *horizontalSpacer;
    QLabel *labelTime;
    QTabWidget *tabWidget;
    QWidget *tabCut;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_2;
    QGridLayout *gridLayout;
    QPushButton *btnSetCutStop;
    QPushButton *btnSetCutStart;
    QLabel *labelOutName;
    QLineEdit *editCutOutName;
    QLineEdit *editCutTimeBeg;
    QLineEdit *editCutTimeEnd;
    QVBoxLayout *verticalLayout_5;
    QPushButton *btnAddCutTask;
    QPushButton *btnCut;
    QTableWidget *tableCutTask;
    QWidget *tabMerge;

    void setupUi(QMainWindow *XAVIPlayerClass)
    {
        if (XAVIPlayerClass->objectName().isEmpty())
            XAVIPlayerClass->setObjectName(QStringLiteral("XAVIPlayerClass"));
        XAVIPlayerClass->resize(892, 807);
        centralWidget = new QWidget(XAVIPlayerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        centralWidget->setFont(font);
        verticalLayout_3 = new QVBoxLayout(centralWidget);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);

        verticalLayout_3->addWidget(frame);

        frame_2 = new QFrame(centralWidget);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame_2);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayoutV = new QHBoxLayout();
        horizontalLayoutV->setSpacing(6);
        horizontalLayoutV->setObjectName(QStringLiteral("horizontalLayoutV"));
        sliderVideo = new QSlider(frame_2);
        sliderVideo->setObjectName(QStringLiteral("sliderVideo"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(sliderVideo->sizePolicy().hasHeightForWidth());
        sliderVideo->setSizePolicy(sizePolicy1);
        sliderVideo->setOrientation(Qt::Horizontal);

        horizontalLayoutV->addWidget(sliderVideo);

        sliderAudio = new QSlider(frame_2);
        sliderAudio->setObjectName(QStringLiteral("sliderAudio"));
        sliderAudio->setOrientation(Qt::Horizontal);

        horizontalLayoutV->addWidget(sliderAudio);


        verticalLayout->addLayout(horizontalLayoutV);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        btnOpen = new QPushButton(frame_2);
        btnOpen->setObjectName(QStringLiteral("btnOpen"));

        horizontalLayout->addWidget(btnOpen);

        btnStart = new QPushButton(frame_2);
        btnStart->setObjectName(QStringLiteral("btnStart"));

        horizontalLayout->addWidget(btnStart);

        btnStop = new QPushButton(frame_2);
        btnStop->setObjectName(QStringLiteral("btnStop"));

        horizontalLayout->addWidget(btnStop);

        btnIncrease = new QPushButton(frame_2);
        btnIncrease->setObjectName(QStringLiteral("btnIncrease"));
        btnIncrease->setMaximumSize(QSize(50, 16777214));

        horizontalLayout->addWidget(btnIncrease);

        btnDecrease = new QPushButton(frame_2);
        btnDecrease->setObjectName(QStringLiteral("btnDecrease"));
        btnDecrease->setMaximumSize(QSize(50, 16777215));

        horizontalLayout->addWidget(btnDecrease);

        btnInc10 = new QPushButton(frame_2);
        btnInc10->setObjectName(QStringLiteral("btnInc10"));
        btnInc10->setMaximumSize(QSize(50, 16777215));

        horizontalLayout->addWidget(btnInc10);

        btnDec10 = new QPushButton(frame_2);
        btnDec10->setObjectName(QStringLiteral("btnDec10"));
        btnDec10->setMaximumSize(QSize(50, 16777215));

        horizontalLayout->addWidget(btnDec10);

        btnRateReset = new QPushButton(frame_2);
        btnRateReset->setObjectName(QStringLiteral("btnRateReset"));
        btnRateReset->setMaximumSize(QSize(50, 16777215));

        horizontalLayout->addWidget(btnRateReset);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        labelTime = new QLabel(frame_2);
        labelTime->setObjectName(QStringLiteral("labelTime"));

        horizontalLayout->addWidget(labelTime);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(verticalLayout);


        verticalLayout_3->addWidget(frame_2);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy2);
        tabWidget->setMinimumSize(QSize(0, 0));
        tabCut = new QWidget();
        tabCut->setObjectName(QStringLiteral("tabCut"));
        verticalLayout_6 = new QVBoxLayout(tabCut);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(3, 3, 3, 3);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        btnSetCutStop = new QPushButton(tabCut);
        btnSetCutStop->setObjectName(QStringLiteral("btnSetCutStop"));

        gridLayout->addWidget(btnSetCutStop, 1, 0, 1, 1);

        btnSetCutStart = new QPushButton(tabCut);
        btnSetCutStart->setObjectName(QStringLiteral("btnSetCutStart"));

        gridLayout->addWidget(btnSetCutStart, 0, 0, 1, 1);

        labelOutName = new QLabel(tabCut);
        labelOutName->setObjectName(QStringLiteral("labelOutName"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(labelOutName->sizePolicy().hasHeightForWidth());
        labelOutName->setSizePolicy(sizePolicy3);

        gridLayout->addWidget(labelOutName, 2, 0, 1, 1);

        editCutOutName = new QLineEdit(tabCut);
        editCutOutName->setObjectName(QStringLiteral("editCutOutName"));

        gridLayout->addWidget(editCutOutName, 2, 1, 1, 1);

        editCutTimeBeg = new QLineEdit(tabCut);
        editCutTimeBeg->setObjectName(QStringLiteral("editCutTimeBeg"));

        gridLayout->addWidget(editCutTimeBeg, 0, 1, 1, 1);

        editCutTimeEnd = new QLineEdit(tabCut);
        editCutTimeEnd->setObjectName(QStringLiteral("editCutTimeEnd"));

        gridLayout->addWidget(editCutTimeEnd, 1, 1, 1, 1);


        horizontalLayout_2->addLayout(gridLayout);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(-1, 3, -1, -1);
        btnAddCutTask = new QPushButton(tabCut);
        btnAddCutTask->setObjectName(QStringLiteral("btnAddCutTask"));
        btnAddCutTask->setMinimumSize(QSize(0, 0));

        verticalLayout_5->addWidget(btnAddCutTask);

        btnCut = new QPushButton(tabCut);
        btnCut->setObjectName(QStringLiteral("btnCut"));
        QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(btnCut->sizePolicy().hasHeightForWidth());
        btnCut->setSizePolicy(sizePolicy4);
        btnCut->setMinimumSize(QSize(0, 0));
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setWeight(75);
        btnCut->setFont(font1);

        verticalLayout_5->addWidget(btnCut);


        horizontalLayout_2->addLayout(verticalLayout_5);

        tableCutTask = new QTableWidget(tabCut);
        if (tableCutTask->columnCount() < 5)
            tableCutTask->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableCutTask->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableCutTask->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableCutTask->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableCutTask->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableCutTask->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        tableCutTask->setObjectName(QStringLiteral("tableCutTask"));
        QSizePolicy sizePolicy5(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy5.setHorizontalStretch(1);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(tableCutTask->sizePolicy().hasHeightForWidth());
        tableCutTask->setSizePolicy(sizePolicy5);
        tableCutTask->setMaximumSize(QSize(16777215, 100));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font2.setPointSize(10);
        tableCutTask->setFont(font2);
        tableCutTask->setColumnCount(5);

        horizontalLayout_2->addWidget(tableCutTask);


        verticalLayout_6->addLayout(horizontalLayout_2);

        tabWidget->addTab(tabCut, QString());
        tabMerge = new QWidget();
        tabMerge->setObjectName(QStringLiteral("tabMerge"));
        tabWidget->addTab(tabMerge, QString());

        verticalLayout_3->addWidget(tabWidget);

        XAVIPlayerClass->setCentralWidget(centralWidget);
        frame_2->raise();
        tabWidget->raise();
        frame->raise();

        retranslateUi(XAVIPlayerClass);
        QObject::connect(btnOpen, SIGNAL(clicked()), XAVIPlayerClass, SLOT(onBtnOpenClick()));
        QObject::connect(btnStart, SIGNAL(clicked()), XAVIPlayerClass, SLOT(onBtnStartClick()));
        QObject::connect(btnStop, SIGNAL(clicked()), XAVIPlayerClass, SLOT(onBtnStopClick()));
        QObject::connect(sliderAudio, SIGNAL(sliderMoved(int)), XAVIPlayerClass, SLOT(onAudioSliderMove(int)));
        QObject::connect(sliderVideo, SIGNAL(sliderMoved(int)), XAVIPlayerClass, SLOT(onVideoSliderMove(int)));
        QObject::connect(sliderVideo, SIGNAL(sliderPressed()), XAVIPlayerClass, SLOT(onVideoSliderPress()));
        QObject::connect(sliderVideo, SIGNAL(sliderReleased()), XAVIPlayerClass, SLOT(onVideoSliderRelease()));
        QObject::connect(btnIncrease, SIGNAL(clicked()), XAVIPlayerClass, SLOT(onBtnIncreaseClick()));
        QObject::connect(btnDecrease, SIGNAL(clicked()), XAVIPlayerClass, SLOT(onBtnDecreaseClick()));
        QObject::connect(btnInc10, SIGNAL(clicked()), XAVIPlayerClass, SLOT(onBtnInc10Click()));
        QObject::connect(btnDec10, SIGNAL(clicked()), XAVIPlayerClass, SLOT(onBtnDec10Click()));
        QObject::connect(btnRateReset, SIGNAL(clicked()), XAVIPlayerClass, SLOT(onBtnRateReset()));

        QMetaObject::connectSlotsByName(XAVIPlayerClass);
    } // setupUi

    void retranslateUi(QMainWindow *XAVIPlayerClass)
    {
        XAVIPlayerClass->setWindowTitle(QApplication::translate("XAVIPlayerClass", "XFLVPlayer", Q_NULLPTR));
        btnOpen->setText(QApplication::translate("XAVIPlayerClass", "\346\211\223\345\274\200", Q_NULLPTR));
        btnStart->setText(QApplication::translate("XAVIPlayerClass", "\345\274\200\345\247\213", Q_NULLPTR));
        btnStop->setText(QApplication::translate("XAVIPlayerClass", "\347\273\223\346\235\237", Q_NULLPTR));
        btnIncrease->setText(QApplication::translate("XAVIPlayerClass", "\345\212\240\351\200\2375", Q_NULLPTR));
        btnDecrease->setText(QApplication::translate("XAVIPlayerClass", "\345\207\217\351\200\2375", Q_NULLPTR));
        btnInc10->setText(QApplication::translate("XAVIPlayerClass", "\345\212\240\351\200\23710", Q_NULLPTR));
        btnDec10->setText(QApplication::translate("XAVIPlayerClass", "\345\207\217\351\200\23710", Q_NULLPTR));
        btnRateReset->setText(QApplication::translate("XAVIPlayerClass", "\351\207\215\347\275\256", Q_NULLPTR));
        labelTime->setText(QApplication::translate("XAVIPlayerClass", "00:00:00/00:00:00", Q_NULLPTR));
        btnSetCutStop->setText(QApplication::translate("XAVIPlayerClass", "\350\256\276\345\256\232\347\273\223\346\235\237", Q_NULLPTR));
        btnSetCutStart->setText(QApplication::translate("XAVIPlayerClass", "\350\256\276\345\256\232\345\274\200\345\247\213", Q_NULLPTR));
        labelOutName->setText(QApplication::translate("XAVIPlayerClass", "  \350\276\223\345\207\272\345\220\215\347\247\260", Q_NULLPTR));
        btnAddCutTask->setText(QApplication::translate("XAVIPlayerClass", "\346\267\273\345\212\240\344\273\273\345\212\241", Q_NULLPTR));
        btnCut->setText(QApplication::translate("XAVIPlayerClass", "\345\274\200\345\247\213\346\211\247\350\241\214", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem = tableCutTask->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("XAVIPlayerClass", "\344\273\273\345\212\241\345\217\267", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem1 = tableCutTask->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("XAVIPlayerClass", "\346\272\220\346\226\207\344\273\266", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem2 = tableCutTask->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("XAVIPlayerClass", "\350\265\267\345\247\213\346\227\266\351\227\264", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem3 = tableCutTask->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("XAVIPlayerClass", "\347\273\223\346\235\237\346\227\266\351\227\264", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem4 = tableCutTask->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("XAVIPlayerClass", "\347\233\256\346\240\207\346\226\207\344\273\266", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabCut), QApplication::translate("XAVIPlayerClass", "\345\211\252\345\210\207", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabMerge), QApplication::translate("XAVIPlayerClass", "\345\220\210\345\271\266", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class XAVIPlayerClass: public Ui_XAVIPlayerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XAVIPLAYER_H
