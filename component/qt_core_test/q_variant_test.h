#include <QDebug>
#include <QVariant>
#include <QVector3D>

// QVariant 存储自定义类型
struct CustomType {
    CustomType(){}
    CustomType(int input_i, QString input_str)
        : i(input_i)
        , s(input_str) {}

    int i;
    QString s;
};
// 必须注册为元类型
Q_DECLARE_METATYPE(CustomType);

void QVariantTest() {
    // 存入数据
    QVariant varInt = 44;           // 存储int
    QVariant varQStr("SttrVariant");    // 存储QString
    varInt.setValue(1000);
    // 取出数据
    int i = varInt.toInt();         // 取出1000
    QString s_fromVariant = varQStr.toString();         // 取出QString
    QString s_fromVariant2 = varQStr.value<QString>();

    // 检查和类型转换
    // Qt6 推荐做法 使用 typeId
    if(varInt.typeId() == QMetaType::Int) {
        qDebug() << "varInt.typeId(): " << varInt.typeId();
    }
    // 检查是否能够转换 编译期
    if (varQStr.canConvert<bool>()) {}
    else {qDebug() << "varQStr.canConvert<bool>(): " << varQStr.canConvert<bool>();}

    // 存储自定义类型
    CustomType ct;
    QVariant varCustom;
    varCustom.setValue(ct);
}


/*
 * 函数的返回值为 QVariant 时, 表明在不同情况下函数会返回不同的类型
 * 实际开发中 配置文件里的键值对可能是不同的数据类型
 * 比如背景是 字符串, 音量是 float, 某些开关是布尔值; 如下;
 */
class SettingsManager {
private:
    QMap<QString, QVariant> m_configData;

public:
    SettingsManager() {
        // 初始化一些模拟的配置数据
        m_configData.insert("WindowTitle", "Hello");
        m_configData.insert("Backgound", QVector3D(0.1, 0.2, 0.3));
        m_configData.insert("fullscreen", false);
        // m_configData.insert("custom", CustomType(1, "111"));
        // insert找不到有自定义类型的构造函数重载 需要手动设置为QVariant
        m_configData.insert("custom", QVariant::fromValue(CustomType(1, "111")));
    }

    // 核心函数: 统一的反省数据获取接口
    QVariant getValue(QString key) {
        if ( m_configData.contains(key) ) {
            return m_configData.value(key);
        }
        // 没有对应的Key 返回空
        return QVariant();
    }
};

QDebug operator<<(QDebug debug, const CustomType& ct) {
    debug << ct.i << " " << ct.s;
    return debug;
}

void QVariantReturnTest() {
    SettingsManager settings;

    QVariant var = settings.getValue("fullscreen");
    if ( var.typeId() == QMetaType::Bool ) {
        qDebug() << "[Bool] var.typeId() == QMetaType::Bool: " << var.toBool();
    }

    var = settings.getValue("Backgound");
    if ( var.typeId() == QMetaType::QVector3D ) {
        // 如果 .toXXX() 没有对应的, 直接用 value<Type>()
        qDebug() << "[QVector3D] var.typeId() == QMetaType::QVector3D: " << var.value<QVector3D>();
    }

    var = settings.getValue("custom");
    if ( var.canConvert<CustomType>() ) {
        qDebug() << "[?] Custom in Variant: " << var.value<CustomType>();
    }
}
