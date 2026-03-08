#pragma once
#include <QObject>
#include <QDebug>

class Counter : public QObject {
    // 必须加这个宏定义才能使用Qt特性
    Q_OBJECT

public:
    explicit Counter(QObject* parent = nullptr) : QObject(parent) {}
    int getValue() { return m_values; }
signals:
/*
    信号只是声明 没有定义 没有函数体
    信号函数的参数列表要和槽函数的参数一致,否则会导致连接失败, 不能少只能多
    一个信号可以连接多个槽, 一个槽也可以接多个信号

    信号发送的值都是值拷贝的形式
    在Sender或者Receiver析构的时候会disconnect
 */

    void signalChangeValue(int value);     // 需要修改值, 值为value
    void signalChangeValue2Param(int value, int value2);     // 需要修改值, 值为value
public slots:
    // slots 就是普通成员函数 只是多了个 "可以被connect的标记" 所以直接调用也没问题
    void setValue(int value) {
        m_values = value;
        qDebug() << this->objectName() << "-> Value has been changed to: " << m_values;
        emit signalChangeValue(value);
    }

    void slotSetValue2Param(int value, int value2) {
        m_values = value + value2;
        qDebug() << this->objectName() << "-> (2Param) Value has been changed to: " << m_values;
    }

private:
    int m_values;
};
