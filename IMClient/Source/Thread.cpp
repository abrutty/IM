
#include "stdafx.h"
#include "Thread.h"
#include <functional>//bind

CThread::CThread()
{

}

CThread::~CThread()
{

}

void CThread::Start()
{
    // ʹ��bind ���Ա���ʹ�þ�̬�������൱�ڰѳ�Ա��������ȫ�־�̬����ʹ��
    m_spThread.reset(new std::thread(std::bind(&CThread::ThreadProc, this)));
}

void CThread::ThreadProc()
{
    Run();
}

void CThread::Join()
{
    m_spThread->join();
}