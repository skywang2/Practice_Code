#include "qplayer.h"
#include <utility>
#include <Windows.h>
#include <QGridLayout> 
#include <QString> 
#include <QLabel>
#include <cstdio>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox> 
#include <QDebug> 
#include "HCNetSDK.h"
using namespace std;
typedef int& INTR;

qPlayer::qPlayer(QWidget *parent)
	: QMainWindow(parent),
	userID(-1),
	lRealPlayHandle(-1),
	windowRect(),
	clientRect()
{
	ui.setupUi(this);

	gridMainWin = new QGridLayout(this);
	ledtip = new QLineEdit(this);
	ledtip->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	ledtPassword = new QLineEdit(this);
	ledtPassword->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	labMainWin = new QLabel(QString::fromLocal8Bit("画面"), this);
	labMainWin->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);//sizeHint值由控件内部默认内容确定
	btnLogin = new QPushButton(QString::fromLocal8Bit("登录"), this);
	btnLogout = new QPushButton(QString::fromLocal8Bit("注销"), this);
	btnStartPlay = new QPushButton(QString::fromLocal8Bit("开始"), this);
	btnStartPlay->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	btnStopPlay = new QPushButton(QString::fromLocal8Bit("停止"), this);
	btnStopPlay->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

	gridMainWin->addWidget(ledtip, 0, 0, Qt::AlignHCenter);
	gridMainWin->addWidget(ledtPassword, 0, 1, Qt::AlignHCenter);
	gridMainWin->addWidget(labMainWin, 1, 0, 1, 2);
	gridMainWin->addWidget(btnLogin, 2, 0);
	gridMainWin->addWidget(btnLogout, 2, 1);
	gridMainWin->addWidget(btnStartPlay, 3, 0);
	gridMainWin->addWidget(btnStopPlay, 3, 1);
	this->centralWidget()->setLayout(gridMainWin);

	connect(this->ledtip, &QLineEdit::textEdited, [&](const QString &x){
		this->ip = x; 
	});
	connect(this->btnLogin, &QPushButton::released, this, &qPlayer::OnBtnClickedLogin);
	connect(this->btnLogout, &QPushButton::released, this, &qPlayer::OnBtnClickedLogout);
	connect(this->btnStartPlay, &QPushButton::released, this, &qPlayer::OnBtnClickedStartPlay);
	connect(this->btnStopPlay, &QPushButton::released, this, &qPlayer::OnBtnClickedStopPlay);

	// 初始化
	NET_DVR_Init();
	//设置连接时间与重连时间
	NET_DVR_SetConnectTime(2000, 1);
	NET_DVR_SetReconnect(10000, true);

	qDebug() << "SDK init success.";
}

qPlayer::~qPlayer()
{
	NET_DVR_Cleanup();
}

/*
function:	qPlayer::OnBtnClickedLogin()
brief:		登录设备
return:		void
*/
void qPlayer::OnBtnClickedLogin()
{
	if (userID >= 0)
	{
		qDebug() << "userID:" << userID << " has login.";
		return;
	}

	//登录参数，包括设备地址、登录用户、密码等
	NET_DVR_USER_LOGIN_INFO struLoginInfo = { 0 };
	struLoginInfo.bUseAsynLogin = 0; //同步登录方式
	strcpy(struLoginInfo.sDeviceAddress, ip.toStdString().c_str()); //设备IP地址
	struLoginInfo.wPort = 8000; //设备服务端口
	strcpy(struLoginInfo.sUserName, "admin"); //设备登录用户名
	strcpy(struLoginInfo.sPassword, "abcd1234"); //设备登录密码
	//设备信息, 输出参数
	NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = { 0 };

	userID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
	if (userID < 0)
	{
		qDebug("Login failed, error code: %d\n", NET_DVR_GetLastError());
		//NET_DVR_Cleanup();
		QMessageBox::warning(this, QString::fromLocal8Bit("警告"), QString(NET_DVR_GetErrorMsg()));
		return;
	}
	qDebug() << "userID:" << userID;
	OnBtnClickedStartPlay();
}

/*
function:	qPlayer::OnBtnClickedLogout()
brief:		注销设备
return:		void
*/
void qPlayer::OnBtnClickedLogout()
{
	OnBtnClickedStopPlay();
	//注销用户
	if (NET_DVR_Logout(userID))
	{
		qDebug() << "Logout success.";
		userID = -1;
	}
	else
	{
		qDebug() << "Logout failed.";
	}

}

/*
function:	qPlayer::OnBtnClickedStartPlay()
brief:		开始播放
return:		void
*/
void qPlayer::OnBtnClickedStartPlay()
{
	if (lRealPlayHandle >= 0)
	{
		qDebug() << "lRealPlayHandle:" << lRealPlayHandle << " has play.";
		return;
	}
	//启动预览并设置回调数据流
	HWND hWnd = (HWND)labMainWin->winId();     //获取窗口句柄
	//HWND hWnd = (HWND)this->centralWidget()->winId();
	NET_DVR_PREVIEWINFO struPlayInfo = { 0 };
	struPlayInfo.hPlayWnd = hWnd;         //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
	struPlayInfo.lChannel = 1;       //预览通道号
	struPlayInfo.dwStreamType = 0;       //0-主码流，1-子码流，2-码流3，3-码流4，以此类推
	struPlayInfo.dwLinkMode = 0;       //0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP
	struPlayInfo.bBlocked = 0;       //0- 非阻塞取流，1- 阻塞取流

	lRealPlayHandle = NET_DVR_RealPlay_V40(userID, &struPlayInfo, NULL, NULL);
	if (lRealPlayHandle < 0)
	{
		qDebug("NET_DVR_RealPlay_V40 error\n");
		return;
	}
	qDebug() << "lRealPlayHandle:" << lRealPlayHandle;

}

/*
function:	qPlayer::OnBtnClickedStopPlay()
brief:		停止播放
return:		void
*/
void qPlayer::OnBtnClickedStopPlay()
{
	//关闭预览
	if (NET_DVR_StopRealPlay(lRealPlayHandle))
	{
		qDebug() << "StopRealPlay success.";
		lRealPlayHandle = -1;
		labMainWin->clear();
	}
	else
	{
		qDebug() << "StopRealPlay failed.";
	}

	HWND id = (HWND)this->winId();
	GetWindowRect(id, &windowRect);
	GetClientRect(id, &clientRect);

	HDC dc = GetDC(id);
	HPEN pen = CreatePen(PS_SOLID, 3, COLORREF(0x0000FF00));
	SelectObject(dc, GetStockObject(NULL_BRUSH));
	SelectObject(dc, pen);
	Rectangle(dc, clientRect.left+10, clientRect.top+10, clientRect.right-10, clientRect.bottom-10);
}
