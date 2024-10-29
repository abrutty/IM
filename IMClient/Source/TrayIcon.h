#pragma once

// 右下角系统托盘类，系统托盘不会处理消息，是将消息通知对应的窗口
class CTrayIcon		// 托盘图标封装类
{
public:
	CTrayIcon(void);
	~CTrayIcon(void);

public:
	BOOL AddIcon(HWND hCallBackWnd, UINT uCallBackMsg,
		UINT uID, HICON hIcon, LPCTSTR lpszTip = NULL);
	BOOL ModifyIcon(HICON hIcon, LPCTSTR lpszTip = NULL, UINT uID = 1);
	BOOL RemoveIcon();
	LRESULT OnTrayIconNotify(WPARAM wParam, LPARAM lParam);
	void OnTimer(UINT_PTR nIDEvent);
	BOOL GetTrayIconRect(RECT* lpRect);			// 获取托盘图标区域位置

private:
	BOOL EnumNotifyWindow(HWND hWnd, RECT& rect);	// 枚举获取托盘图标区域位置
	HWND FindTrayNotifyWnd();						// 获取普通托盘区窗口句柄
	HWND FindNotifyIconOverflowWindow();			// 获取溢出托盘区窗口句柄
	// GetTrayIconRect、EnumNotifyWindow、FindTrayNotifyWnd、FindNotifyIconOverflowWindow 主要是为了兼容老系统
private:
	NOTIFYICONDATA m_stNotifyIconData; // Windows 系统托盘的标准数据结构
	BOOL m_bHover;	// 状态
	DWORD m_dwTimerId;	// OnTimer 需要处理的任务
};
