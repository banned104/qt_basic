#include "qt_thread_test.h"
#include <QDebug>
/*
 * (quint64)QThread::currentThreadId() 是一个静态函数, 因为是64位平台, 指针大小为8字节, 需要转换为 quint64 否则报错 cast from pointer to smaller type 'int' loses information
 *
 */

ThreadFromQThread::ThreadFromQThread(QObject* parent) : QThread(parent)
    ,m_runCount(20) {
    QString str = QString("%1->%2,thread id: %3").arg(__FUNCTION__).arg(__FILE__).arg((quint64)QThread::currentThreadId());
    qDebug() << "ThreadFromQThread Construction: " << str;
}

ThreadFromQThread::~ThreadFromQThread() {
    qDebug() << "ThreadFromQThread::~ThreadFromQThread()";
}


/*
    ! 只有run()在另一个线程中执行, 这里的 xxxSomething 都是在主线程中运行, 所以msleep()会阻塞主线程, 导致UI界面卡顿.
    就是 在UI界面点击按钮调用 xxxSomething() 就会阻塞主线程, 因为 xxxSomething() 是在主线程中运行的.
    
    如果是在 run 中调用 xxxSomething() 就不会阻塞主线程, 因为 run() 是在子线程中执行的.
    -> 在run中调用的函数都是在子线程中运行!!!!
*/
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

void ThreadFromQThread::run() override{
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


        {
            // QMutexLocker 类似 std::unique_lock
            QMutexLocker _locker(&locker);
            // _locker.unlock();        // 可以提前unlock
            if ( m_isStop ) {
                // 在完成run函数之后, QThread会发出 signal finished() 信号
                return;
            }
        }

        // QThread::exec();
    }
}

void ThreadFromQThread::doSomething() {
    msleep(500);
    emit message(QString("%1->%2,thread id: %3").arg(__FUNCTION__).arg(__FILE__).arg((quint64)QThread::currentThreadId()));
}

/*
    如何判断是否需要 QThread::exec() ?
    在子线程中有两种模式:
    1. 循环执行任务(while(1)) -> 在线程中做一段持续的计算/轮询/阻塞式工作流. 此时run()是主循环,如果调用exec()相当于把死循环交给了事件循环.
    2. 事件驱动线程(exec()) -> 子线程核心是事件循环, 子线程中有QTimer/socket/serial port/network对象, 有 queued connection 的槽函数, 或者需要这个子线程响应事件时写 exec() 
    
*/


/*
    如何正确终止线程?
    -> 这时我点击Ui界面中的 quit按钮, thread并没进行任何处理，QThread在不调用exec()情况下是exit函数和quit函数是没有作用的。
    -> 并且使用terminate() 强制结束线程会导致资源泄漏, 因为线程没有机会清理资源, 可能会导致内存泄漏, 文件句柄泄漏等问题. 所以不建议使用 terminate() 来结束线程.

    如何正常结束一个线程:
    添加一个 bool 变量 m_stop, 在run 中每次循环都检测这个变量 如果为真则 直接return;
*/

/*
    如何正确启动一个线程?
    -> 创建线程对象的两种情况: 
    --> 1.[全局线程] 将生成线程的窗体当做线程的父对象,窗体析构时会析构线程对象; 但是窗体结束时线程还没结束怎么处理?
    
    --> 2.[局部线程] 一般是突然要处理一个大的计算,需要一个线程; 但是在线程计算没有完成时,用户突然终止或者变更时如何处理?

    ------------------ [全局线程] 在一个窗体类中创建线程 ------------------
    Widget::Widget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::Widget)
    ,m_objThread(NULL)
    {
    
    ui->setupUi(this);
        //全局线程的创建
        //全局线程创建时可以把窗体指针作为父对象
        m_thread = new ThreadFromQThread(this);
        //关联线程的信号和槽
        connect(m_thread,&ThreadFromQThread::message
                ,this,&Widget::receiveMessage);//
        connect(m_thread,&ThreadFromQThread::progress
                ,this,&Widget::progress);
        connect(m_thread,&QThread::finished
                ,this,&Widget::onQThreadFinished);
        //UI心跳开始
        m_heart.start();
    }

    析构时需要调用wait, 让主线程等待子线程结束才继续往下运行;
    如果主线程直接destroy掉, 也会顺手把 m_thread 销毁, 可能导致未定义行为;
    Widget::~Widget()
    {
        qDebug() << "start destroy widget";
        m_thread->stopImmediately();
        m_thread->wait();
        delete ui;
        qDebug() << "end destroy widget";
    }

    ------------------ [局部线程] 在一个函数中创建线程 ------------------
    不指定父对象,通过绑定void QObject::deleteLater () [slot] 这个槽函数来让其自动释放, 这样就不需要 m_thread.wait();
    退出run函数后, QThread会发出 finished() 信号, 连接这个信号到 deleteLater() 来销毁线程对象.

    void Widget::onButtonQThreadRunLoaclClicked()
    {
        //局部线程的创建的创建
        ThreadFromQThread* thread = new ThreadFromQThread(NULL);//这里父对象指定为NULL
        connect(thread,&ThreadFromQThread::message
                ,this,&Widget::receiveMessage);
        connect(thread,&ThreadFromQThread::progress
                ,this,&Widget::progress);
        connect(thread,&QThread::finished
                ,this,&Widget::onQThreadFinished);
        connect(thread,&QThread::finished
                ,thread,&QObject::deleteLater);//线程结束后调用deleteLater来销毁分配的内存
        thread->start();
    }

    [额外] 
    如果线程已经启动,重复调用start不会有任何效果.
    理论上所有 QObject 都不应该手动delete, 因为多线程情况下, 要删除的这个 QObject 可能还在Qt事件循环中排队, 但是外面却把这个QObject删除了, 会导致程序崩溃.

*/