
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
    // 使用bind 可以避免使用静态函数，相当于把成员函数当作全局静态函数使用
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