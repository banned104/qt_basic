#include "qt_thread_test.h"
#include <QDebug>
/*
 * (quint64)QThread::currentThreadId() 是一个静态函数, 因为是64位平台, 指针大小为8字节, 需要转换为 quint64 否则报错 cast from pointer to smaller type 'int' loses information
 *
 */

ThreadFromQThread::ThreadFromQThread(QObject* parent) : QThread(parent)
    ,m_runCount(20) {

}

ThreadFromQThread::~ThreadFromQThread() {
    qDebug() << "ThreadFromQThread::~ThreadFromQThread()";
}

void ThreadFromQThread::setSomething() {
    msleep(500);
    QString str = QString("%1->%2,thread id: %3").arg(__FUNCTION__).arg(__FILE__).arg((quint64)QThread::currentThreadId());
    emit message(str);
}

void ThreadFromQThread::getSomething() {
    msleep(500);
    emit message(QString("%1->%2,thread id: %3").arg(__FUNCTION__).arg(__FILE__).arg((quint64)QThread::currentThreadId()));
}

void ThreadFromQThread::setRunCount(int count) {
    m_runCount = count;
    emit message(QString("%1->%2,thread id: %3").arg(__FUNCTION__).arg(__FILE__).arg((quint64)QThread::currentThreadId()));
}

void ThreadFromQThread::run() {
    QString str = QString("%1->%2,thread id: %3").arg(__FUNCTION__).arg(__FILE__).arg((quint64)QThread::currentThreadId());
    int count = 0;
    emit message(str);

    while(1) {
        sleep(1);
        ++count;
        emit progress( ((float)count / m_runCount) * 100);
        emit message(QString("ThreadFromQThread::run times%1").arg(count));

        doSomething();

        if(m_runCount == count) {
            break;
        }
    }
}

void ThreadFromQThread::doSomething() {
    msleep(500);
    emit message(QString("%1->%2,thread id: %3").arg(__FUNCTION__).arg(__FILE__).arg((quint64)QThread::currentThreadId()));
}