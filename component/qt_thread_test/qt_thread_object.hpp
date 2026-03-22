/*
 * https://blog.csdn.net/czyt1988/article/details/71194457

    * 使用QObject创建多线程的方法如下：

    * 写一个继承QObject的类，对需要进行复杂耗时逻辑的入口函数声明为槽函数
    * 此类在旧线程new出来，不能给它设置任何父对象
    * 同时声明一个QThread对象
    * 把obj通过moveToThread方法转移到新线程中，此时object已经是在线程中了
    * 把线程的finished信号和object的deleteLater槽连接，这个信号槽必须连接，否则会内存泄漏
    * 正常连接其他信号和槽（在连接信号槽之前调用moveToThread，不需要处理connect的第五个参数，否则就显示声明用Qt::QueuedConnection来连接）
    * 初始化完后调用'QThread::start()'来启动线程
    * 在逻辑结束后，调用QThread::quit退出线程的事件循环
    * 使用QObject来实现多线程比用继承QThread的方法更加灵活，整个类都是在新的线程中，通过信号槽和主线程传递数据
*/

#include <QThread>

class Worker : public QObject {
    Q_OBJECT

public slots:
    void doWorks(const QString &param) {
        QString result;
        result += param;

        emit resultReady(result);
    }
signals:
    void resultReady(const QString &msg);
};


class Controller : public QObject {
    Q_OBJECT
/*
    Controller 对象创建

    编译器先初始化所有成员变量
    QThread workerThread 调用 QThread::QThread() 默认构造
    此时 workerThread 对象已经存在，但线程还没启动
*/
    QThread workerThread;  // 这里是声明+定义
                           // 当 Controller 对象被创建时，
                           // workerThread 就已经存在了

public:
    Controller() {
        // 在旧线程中new 且不设置任何父对象
        Worker *worker = new Worker();
        // 把worker对象转移到新线程中
        worker->moveToThread(&workerThread);

        // 把线程的finished信号和object的deleteLater槽连接，这个信号槽必须连接，否则会内存泄漏
        connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
        // 正常连接其他信号和槽（在连接信号槽之前调用moveToThread，不需要处理connect的第五个参数，否则就显示声明用Qt::QueuedConnection来连接）
        connect(this, &Controller::operate, worker, &Worker::doWorks);
        // 最后调用start()启动
        workerThread.start();
    }

    ~Controller() {
        /* Tells the thread's event loop to exit with return code 0 (success). Equivalent to calling QThread::exit(0). */
        // 这里的quit完全等于 exit(0); 
        // 这里的 quit 和 exit() 都是让线程的事件循环退出, 但是如果线程没有事件循环(没有调用 exec()) 那么 quit 和 exit() 是没有任何作用的.
        workerThread.quit();
        workerThread.wait();
    }

public slots:
    void handleResult(const QString& msg);
signals:
    void operate(const QString&);
};
