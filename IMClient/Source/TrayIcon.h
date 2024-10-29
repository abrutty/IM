#pragma once

// ���½�ϵͳ�����࣬ϵͳ���̲��ᴦ����Ϣ���ǽ���Ϣ֪ͨ��Ӧ�Ĵ���
class CTrayIcon		// ����ͼ���װ��
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
	BOOL GetTrayIconRect(RECT* lpRect);			// ��ȡ����ͼ������λ��

private:
	BOOL EnumNotifyWindow(HWND hWnd, RECT& rect);	// ö�ٻ�ȡ����ͼ������λ��
	HWND FindTrayNotifyWnd();						// ��ȡ��ͨ���������ھ��
	HWND FindNotifyIconOverflowWindow();			// ��ȡ������������ھ��
	// GetTrayIconRect��EnumNotifyWindow��FindTrayNotifyWnd��FindNotifyIconOverflowWindow ��Ҫ��Ϊ�˼�����ϵͳ
private:
	NOTIFYICONDATA m_stNotifyIconData; // Windows ϵͳ���̵ı�׼���ݽṹ
	BOOL m_bHover;	// ״̬
	DWORD m_dwTimerId;	// OnTimer ��Ҫ���������
};
