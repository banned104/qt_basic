#ifndef QTHREADTEST_H
#define QTHREADTEST_H

/*
 * https://blog.csdn.net/czyt1988/article/details/64441443
 * https://www.cnblogs.com/CaiNiaoIceLee/p/16111546.html 不错
 ! 一定要注意 这个 QThreadFromQThread 自身是在主线程中, 并不是在子线程.
 * 重写run 但是是使用 start() 启动新线程
 * 继承QThread的注意: 构造函数是在new的线程(旧线程)调用的, 只有run在新线程跑
 * 并且这里的 getSomething/setSomething都是在旧线程调用的
 *
 * 除非在run函数中调用 QThread::exec()否则不会有任何事件循环在线程中运行;
 * 每个线程都可以有自己的事件循环。初始线程使用QCoreApplication::exec()启动它的事件循环，或者对于单对话框GUI应用程序，有时使用QDialog::exec()。其他线程可以使用QThread::exec()启动事件循环
 * 可以安全地从QThread::run()实现发出信号，因为信号发出是线程安全的。
 */

#include <QThread>
#include <QMutex>

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
    Q_INVOKABLE void doSomething();
protected:
    void run(); // protected
private:
    int m_runCount;
    bool m_isStop = false;

    QMutex locker;
};

// void QThreadTest() {

// }

#endif