#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "component/counter.h"
#include "component/counter_for_qml.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    /****************************************/
    /* C++ 与 QML 相互通信 */
    /* 需要将C++组件的定义移动到loadFromModule之前 否则QML无法获取到backend实例 */
    /****************************************/
    Backend backend;
    QQmlContext* context = engine.rootContext();
    context->setContextProperty("backend", &backend);

    engine.loadFromModule("QAI", "Main");


    /****************************************/
    /* C++ 信号与槽 */
    /****************************************/
    Counter a,b;
    a.setObjectName("Counter AAAAAAAA");
    b.setObjectName("Counter BBBBBBBB");

    // connect是静态函数 直接调用
    QObject::connect(&a, &Counter::signalChangeValue,
                     &b, &Counter::setValue );

    // 连接lambada函数, 注意这里lambada函数的Sender是 QGuiApplication app;
    QObject::connect(&a, &Counter::signalChangeValue,
                     &app, [&](){
        qDebug() << "App got signal from a";
    });

    // 槽函数slot就是一个普通函数, a调用setValue之后会发送signalChangeValue
    // 然后b就会收到这个信号 也调用自己的setValue 同时b也触发signalChangeValue
    // 但是这个b的信号没有接到任何槽函数 不会导致循环触发信号
    a.setValue(100);

    return app.exec();
}
