/********************************************************************************
** Form generated from reading UI file 'dv_client.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DV_CLIENT_H
#define UI_DV_CLIENT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QScrollArea>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DV_ClientClass
{
public:
    QAction *actionAbout;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionClose;
    QAction *actionSeogwi;
    QAction *actionJeju;
    QAction *actionConnect;
    QAction *actionDisconnect;
    QAction *actionTranslate;
    QAction *actionRotate;
    QAction *actionZoomIn;
    QAction *actionZoomOut;
    QAction *actionWireframe;
    QAction *actionLighting;
    QAction *actionRequest;
    QAction *actionRendering;
    QWidget *centralWidget;
    QScrollArea *scrollArea_Render;
    QWidget *scrollAreaWidgetContents;
    QScrollArea *scrollArea_Mini;
    QWidget *scrollAreaWidgetContents_3;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuView;
    QMenu *menuRegion;
    QMenu *menuEffect;
    QMenu *menuNetwork;
    QMenu *menuSetting;
    QMenu *menuHelp;
    QMenu *menuNavigator;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *DV_ClientClass)
    {
        if (DV_ClientClass->objectName().isEmpty())
            DV_ClientClass->setObjectName(QString::fromUtf8("DV_ClientClass"));
        DV_ClientClass->resize(1199, 800);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DV_ClientClass->sizePolicy().hasHeightForWidth());
        DV_ClientClass->setSizePolicy(sizePolicy);
        actionAbout = new QAction(DV_ClientClass);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionOpen = new QAction(DV_ClientClass);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionSave = new QAction(DV_ClientClass);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionClose = new QAction(DV_ClientClass);
        actionClose->setObjectName(QString::fromUtf8("actionClose"));
        actionSeogwi = new QAction(DV_ClientClass);
        actionSeogwi->setObjectName(QString::fromUtf8("actionSeogwi"));
        actionJeju = new QAction(DV_ClientClass);
        actionJeju->setObjectName(QString::fromUtf8("actionJeju"));
        actionConnect = new QAction(DV_ClientClass);
        actionConnect->setObjectName(QString::fromUtf8("actionConnect"));
        actionDisconnect = new QAction(DV_ClientClass);
        actionDisconnect->setObjectName(QString::fromUtf8("actionDisconnect"));
        actionTranslate = new QAction(DV_ClientClass);
        actionTranslate->setObjectName(QString::fromUtf8("actionTranslate"));
        actionTranslate->setCheckable(true);
        QIcon icon;
        icon.addFile(QString::fromUtf8("Resources/icon_pan.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionTranslate->setIcon(icon);
        actionRotate = new QAction(DV_ClientClass);
        actionRotate->setObjectName(QString::fromUtf8("actionRotate"));
        actionRotate->setCheckable(true);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("Resources/icon_rotate.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRotate->setIcon(icon1);
        actionZoomIn = new QAction(DV_ClientClass);
        actionZoomIn->setObjectName(QString::fromUtf8("actionZoomIn"));
        actionZoomIn->setCheckable(true);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8("Resources/icon_zoomin.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoomIn->setIcon(icon2);
        actionZoomOut = new QAction(DV_ClientClass);
        actionZoomOut->setObjectName(QString::fromUtf8("actionZoomOut"));
        actionZoomOut->setCheckable(true);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8("Resources/icon_zoomout.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoomOut->setIcon(icon3);
        actionWireframe = new QAction(DV_ClientClass);
        actionWireframe->setObjectName(QString::fromUtf8("actionWireframe"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8("Resources/icon_wireframe.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionWireframe->setIcon(icon4);
        actionLighting = new QAction(DV_ClientClass);
        actionLighting->setObjectName(QString::fromUtf8("actionLighting"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8("Resources/icon_onoff.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLighting->setIcon(icon5);
        actionRequest = new QAction(DV_ClientClass);
        actionRequest->setObjectName(QString::fromUtf8("actionRequest"));
        actionRendering = new QAction(DV_ClientClass);
        actionRendering->setObjectName(QString::fromUtf8("actionRendering"));
        centralWidget = new QWidget(DV_ClientClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy1);
        scrollArea_Render = new QScrollArea(centralWidget);
        scrollArea_Render->setObjectName(QString::fromUtf8("scrollArea_Render"));
        scrollArea_Render->setGeometry(QRect(10, 10, 1900, 1100));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(scrollArea_Render->sizePolicy().hasHeightForWidth());
        scrollArea_Render->setSizePolicy(sizePolicy2);
        scrollArea_Render->setMinimumSize(QSize(1900, 1100));
        scrollArea_Render->setMaximumSize(QSize(4096, 2060));
        scrollArea_Render->setFrameShadow(QFrame::Sunken);
        scrollArea_Render->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1898, 1098));
        scrollArea_Render->setWidget(scrollAreaWidgetContents);
        scrollArea_Mini = new QScrollArea(centralWidget);
        scrollArea_Mini->setObjectName(QString::fromUtf8("scrollArea_Mini"));
        scrollArea_Mini->setGeometry(QRect(870, 10, 321, 271));
        sizePolicy.setHeightForWidth(scrollArea_Mini->sizePolicy().hasHeightForWidth());
        scrollArea_Mini->setSizePolicy(sizePolicy);
        scrollArea_Mini->setWidgetResizable(true);
        scrollAreaWidgetContents_3 = new QWidget();
        scrollAreaWidgetContents_3->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_3"));
        scrollAreaWidgetContents_3->setGeometry(QRect(0, 0, 319, 269));
        scrollArea_Mini->setWidget(scrollAreaWidgetContents_3);
        DV_ClientClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(DV_ClientClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1199, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuRegion = new QMenu(menuView);
        menuRegion->setObjectName(QString::fromUtf8("menuRegion"));
        menuEffect = new QMenu(menuBar);
        menuEffect->setObjectName(QString::fromUtf8("menuEffect"));
        menuNetwork = new QMenu(menuBar);
        menuNetwork->setObjectName(QString::fromUtf8("menuNetwork"));
        menuSetting = new QMenu(menuBar);
        menuSetting->setObjectName(QString::fromUtf8("menuSetting"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuNavigator = new QMenu(menuBar);
        menuNavigator->setObjectName(QString::fromUtf8("menuNavigator"));
        DV_ClientClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(DV_ClientClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        DV_ClientClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(DV_ClientClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        DV_ClientClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuNavigator->menuAction());
        menuBar->addAction(menuEffect->menuAction());
        menuBar->addAction(menuNetwork->menuAction());
        menuBar->addAction(menuSetting->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addSeparator();
        menuFile->addAction(actionClose);
        menuView->addAction(menuRegion->menuAction());
        menuView->addSeparator();
        menuView->addAction(actionRequest);
        menuView->addAction(actionRendering);
        menuRegion->addAction(actionSeogwi);
        menuRegion->addAction(actionJeju);
        menuNetwork->addAction(actionConnect);
        menuNetwork->addAction(actionDisconnect);
        menuHelp->addAction(actionAbout);
        mainToolBar->addAction(actionTranslate);
        mainToolBar->addAction(actionRotate);
        mainToolBar->addAction(actionZoomIn);
        mainToolBar->addAction(actionZoomOut);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionWireframe);
        mainToolBar->addAction(actionLighting);
        mainToolBar->addSeparator();

        retranslateUi(DV_ClientClass);

        QMetaObject::connectSlotsByName(DV_ClientClass);
    } // setupUi

    void retranslateUi(QMainWindow *DV_ClientClass)
    {
        DV_ClientClass->setWindowTitle(QApplication::translate("DV_ClientClass", "DV_Client", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("DV_ClientClass", "About", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("DV_ClientClass", "Open", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("DV_ClientClass", "Save", 0, QApplication::UnicodeUTF8));
        actionClose->setText(QApplication::translate("DV_ClientClass", "Close", 0, QApplication::UnicodeUTF8));
        actionSeogwi->setText(QApplication::translate("DV_ClientClass", "Seogwi", 0, QApplication::UnicodeUTF8));
        actionJeju->setText(QApplication::translate("DV_ClientClass", "Jeju", 0, QApplication::UnicodeUTF8));
        actionConnect->setText(QApplication::translate("DV_ClientClass", "Connect", 0, QApplication::UnicodeUTF8));
        actionDisconnect->setText(QApplication::translate("DV_ClientClass", "Disconnect", 0, QApplication::UnicodeUTF8));
        actionTranslate->setText(QApplication::translate("DV_ClientClass", "Translate", 0, QApplication::UnicodeUTF8));
        actionRotate->setText(QApplication::translate("DV_ClientClass", "Rotate", 0, QApplication::UnicodeUTF8));
        actionZoomIn->setText(QApplication::translate("DV_ClientClass", "ZoomIn", 0, QApplication::UnicodeUTF8));
        actionZoomOut->setText(QApplication::translate("DV_ClientClass", "ZoomOut", 0, QApplication::UnicodeUTF8));
        actionWireframe->setText(QApplication::translate("DV_ClientClass", "Wireframe", 0, QApplication::UnicodeUTF8));
        actionLighting->setText(QApplication::translate("DV_ClientClass", "Lighting", 0, QApplication::UnicodeUTF8));
        actionRequest->setText(QApplication::translate("DV_ClientClass", "Request", 0, QApplication::UnicodeUTF8));
        actionRendering->setText(QApplication::translate("DV_ClientClass", "Rendering", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("DV_ClientClass", "File", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("DV_ClientClass", "View", 0, QApplication::UnicodeUTF8));
        menuRegion->setTitle(QApplication::translate("DV_ClientClass", "Region", 0, QApplication::UnicodeUTF8));
        menuEffect->setTitle(QApplication::translate("DV_ClientClass", "Effect", 0, QApplication::UnicodeUTF8));
        menuNetwork->setTitle(QApplication::translate("DV_ClientClass", "Network", 0, QApplication::UnicodeUTF8));
        menuSetting->setTitle(QApplication::translate("DV_ClientClass", "Setting", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("DV_ClientClass", "Help", 0, QApplication::UnicodeUTF8));
        menuNavigator->setTitle(QApplication::translate("DV_ClientClass", "Navigator", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DV_ClientClass: public Ui_DV_ClientClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DV_CLIENT_H
