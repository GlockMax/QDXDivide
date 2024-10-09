
#ifdef _WIN32
#include <Windows.h>
#include <sstream>
#else
#include <unistd.h>
#include <pthread.h>
#endif // _WIN32

#include "grddongle.h"

#ifndef _WIN32
pthread_mutex_t CGrdMutex::m_hMutex = PTHREAD_MUTEX_INITIALIZER;
#endif // !_WIN32

CGrdMutex::CGrdMutex()
{
#ifdef _WIN32
    std::stringstream ss;
    ss << "CGrdDongle" << ::GetCurrentProcessId();
    m_hMutex = ::CreateMutexA(NULL, FALSE, ss.str().c_str());
    if (m_hMutex)
        ::WaitForSingleObject(m_hMutex, INFINITE);
#else
    pthread_mutex_lock(&m_hMutex);
#endif // _WIN32
}

CGrdMutex::~CGrdMutex()
{
#ifdef _WIN32
    ::ReleaseMutex(m_hMutex);
    ::CloseHandle(m_hMutex);
#else
    pthread_mutex_unlock(&m_hMutex);
#endif // _WIN32
}

#ifdef _WIN32
long CGrdDongle::m_lGrdDongleClassCounter = 0;
#else
uint32_t CGrdDongle::m_lGrdDongleClassCounter = 0;
#endif // _WIN32

bool CGrdDongle::m_bGrdDongleStartup = false;
