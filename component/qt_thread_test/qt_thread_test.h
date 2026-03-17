#ifndef QTHREADTEST_H
#define QTHREADTEST_H

/*
 * https://blog.csdn.net/czyt1988/article/details/64441443
 *
 * start() 启动新线程
 * run
 */

#include <QThread>
class ThreadFromQThread : public QThread {
    Q_OBJECT
signals:
    Q_INVOKABLE void message(const QString& info);
    Q_INVOKABLE void progress(int precent);
public:
    ThreadFromQThread(QObject* parent);
    ~ThreadFromQThread();
    Q_INVOKABLE void setSomething();
    Q_INVOKABLE void getSomething();
    Q_INVOKABLE void setRunCount(int count);
    Q_INVOKABLE void run();
    Q_INVOKABLE void doSomething();
private:
    int m_runCount;
};

// void QThreadTest() {

// }

#endif