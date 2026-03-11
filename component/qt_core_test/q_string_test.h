#include <QString>
#include <QByteArray>
#include <QVariant>
#include <QDebug>
#include <QDateTime>
#include <QVector3D>


/*
     * QVariant 需要传递多种不同类型时使用; 替代结构体
*/
void parse(const QVariant& data) {
    // 判断实际的类型
    if ( data.typeId() == QMetaType::Int ) {
        // 安全转换为int
        int value = data.toInt();
        qDebug() << "[1] QMetaType::Int: " << value;

        // data.setValue(22222);        // 常用两种方法改
        // data = 33333;

        // 判断是否能够强转为QString
        if ( data.canConvert<QString>() ) {
            qDebug() << "[2] data.canConvert<QString>(): " << data.toString();
        }
    } else if ( data.typeId() == QMetaType::QString ) {
        qDebug() << "QMetaType::QString: " << data.toString();
    }
    return;
}

// *********************************************

struct MaterialInfo {
    float roughness;
    float metalness;
    QVector3D baseColor;
};
// 需要这个宏定义声明 向Qt元对象系统注册类型
Q_DECLARE_METATYPE(MaterialInfo)

void parseCustomStruct() {
    MaterialInfo m1 = {0.3, 0.1, QVector3D{1,1,1}};
    QVariant var;
    var.setValue(m1);       // 这里使用setValue

    // QVariant 取出数据
    if ( var.canConvert<MaterialInfo>() ) {
        MaterialInfo m2 = var.value<MaterialInfo>();
        qDebug() << "[QVariant 取出数据] MaterialInfo m2 = var.value<MaterialInfo>();" << m2.baseColor;
    }
}
// *********************************************

void QStringTest() {
    QString str1 = "String11111";
    str1 += "Adddd";
    str1.append("aDDDDD");      // 与+/+=相同
    qDebug() << str1;

    QString str2;
    // 不需要指定 %d/%s之类; 只需要数字;
    str2 = QString("%1 is the best %2").arg("Hans Zimmer").arg(7777777);
    qDebug()<< str2;

    // 相同返回0
    int a1 = QString::compare("abcd", "ABCD", Qt::CaseInsensitive);
    int a2 = QString::compare("abcd", "ABCD", Qt::CaseSensitive);
    int b1 = QString::compare(str1, str2, Qt::CaseInsensitive);
    qDebug()<< QString("a1: %1, a2: %2, b1: %3").arg(a1).arg(a2).arg(b1);

    // QByteArray/QVector/QVariant 三种最常用
    /*
     * 用于存储原始字节 和 传统以'\0'结尾的8位字符串
     * 使用QByteArray比使用 const char * 方便的多; 并且也是在末尾默认加一个 '\0'
     * QByteArray 常用于保存原始二进制数据;
     * QString 中保存的是UTF-16; 两个字节保存一个字符;
     */
    QByteArray byte1("Qt Creator");
    QByteArray byte2 = byte1.toLower();
    qDebug() << byte2;

    /*  M:Month; m:minute;
     *  Format              Result
        dd.MM.yyyy          21.05.2001
        ddd MMMM d yy       Tue May 21 01
        hh:mm:ss.zzz        14:13:09.120
        hh:mm:ss.z          14:13:09.12
        h:m:s ap            2:13:9 pm
     */
    QDateTime dt;
    QString strDT = dt.currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    qDebug() << strDT;

    qDebug() << "--------------------------";
    /*
     * error: cannot bind non-const lvalue reference of type 'QVariant&' to
     * an rvalue of type 'QVariant'
     */
    // parse(QString("hahah"));
    // parse(11111);

    QString _s = "hahah";
    parse(_s);      // QString 隐式转换为 QVariant 返回 rvalue; rvalue必须用 const Type& 传入右值
    parse(11111);

    parseCustomStruct();

    return;
}



