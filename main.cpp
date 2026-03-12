#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "component/counter.h"
#include "component/counter_for_qml.hpp"
#include "component/qt_core_test/q_string_test.h"
// #include "component/qt_core_test/q_hash_test.h"
#include "component/qt_core_test/q_variant_test.h"

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

    // 信号函数参数数量 > 槽函数参数数量 正确, 可以;  槽 ≤ 信号，多余参数槽忽略
    QObject::connect(&a, &Counter::signalChangeValue2Param,
                     &b, &Counter::setValue);

    // // 信号函数参数数量 < 槽函数参数数量 错误
    // QObject::connect(&a, &Counter::signalChangeValue,
    //                  &b, &Counter::slotSetValue2Param);
    

    // 连接lambada函数, 注意这里lambada函数的Sender是 QGuiApplication app;
    // 这里 Sender不能是nullptr
    QMetaObject::Connection connLambda = QObject::connect(&a, &Counter::signalChangeValue,
                     &app, [&](){
        qDebug() << "App lambda got signal from a";
    });


    // 槽函数slot就是一个普通函数, a调用setValue之后会发送signalChangeValue
    // 然后b就会收到这个信号 也调用自己的setValue 同时b也触发signalChangeValue
    // 但是这个b的信号没有接到任何槽函数 不会导致循环触发信号
    a.setValue(100);
    // 直接调用信号函数 这就是一个普通函数
    a.signalChangeValue2Param(100, 1000);

    // 断开所有 &Counter::signalChangeValue2Param 连接的槽函数/信号
    QObject::disconnect(&a, &Counter::signalChangeValue2Param, nullptr, nullptr);
    // 全部断开
    QObject::disconnect(&a, nullptr, nullptr, nullptr);
    // 断开单个connect;  connLambda只是一个句柄;
    QObject::disconnect(connLambda);
    qDebug() << "After Disconnect";
    a.setValue(100);        // 已经断开 没有槽函数输出

    /****************************************/
    /* QString */
    /****************************************/
    QStringTest();

    // q_hash_test.h
    // QListTest();
    // QMapTest();
    // QHashTest();

    // q_variant_test.h
    QVariantReturnTest();

    return app.exec();
}
