// backend for qml
#pragma once
#include <QObject>

class Backend : public QObject {
    Q_OBJECT

public:
    explicit Backend(QObject* parent = nullptr) : QObject(parent) {}
    Q_PROPERTY(int m_counter READ getCounter WRITE setCounter NOTIFY counterChanged FINAL)

    Q_INVOKABLE int getCounter() { return m_counter; }

    Q_INVOKABLE void increment() {
        m_counter ++;
        emit counterChanged(m_counter);
    }

    void setCounter(int value) { m_counter = value; }

signals:
    void counterChanged(int value);
private:
    int m_counter = 0;
};


/*
    Text {
        id: textCounter
        // text: backend.getCounter() // -> QML绑定函数不能自动更新 需要绑定值
        text: backend.m_counter
        width: 100
        height: 50
        font.pixelSize: 20
    }

    绑定属性来自动更新 backend.m_counter -x-> 而不是绑定函数
    使用 Q_PROPERTY 定义的QML能够访问的属性值, 读取或修改时会自动调用函数进行修改 -> 这就是 Q_PROPERTY 的功能
*/