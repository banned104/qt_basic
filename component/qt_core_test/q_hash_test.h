#include <QDebug>


/*
 * QVector的别名
 * QList<T> 采取两种不同的存储策略
 * 1. 如果是也给指针类型,或者指针大小的基本类型, QList会直接存储在数组中
 * 2. 存储对象指针
 */

void QListTest() {
    QList<int> qlist;       // 初始化一个空的QList
    for(int i=0; i<10; i++) {
        qlist.append(i*i);
    }
    qDebug() << qlist;

/*
    QList<int>::Iterator it_qlist;
    for(it_qlist=qlist.begin(); it_qlist != qlist.end(); it_qlist++) {
        qDebug() << "->" << *it_qlist - 1;
    }

    for(auto _it=qlist.constBegin(); _it < qlist.constEnd(); _it++) {
        qDebug() << "const->" << *_it;
    }
*/

    qDebug() << "qlist.at(3): " << qlist.at(3);
    qDebug() << "qlist.contains(999): " << qlist.contains(999);

    qDebug() <<"qlist.first(): " << qlist.first();
    qlist.removeAt(0);
    qDebug() <<"After removeAt->qlist.first(): " << qlist.first();
}

/*
 * QMap<Key,T> 类型为Key的键 到 类型为T的值的 映射
 * 并且按照Key的次序存储数据
 */
void QMapTest() {
    // 插入删除操作
    QMap<QString,int> qmap;
    qmap["Chinese"] = 120;
    qmap["English"] = 140;
    qmap.insert("Math", 120);
    qDebug()<<qmap;

    qmap.remove(QString("Math"));
    qDebug()<<qmap;

    // STL风格遍历
    QMap<QString, int>::Iterator _it = qmap.begin();
    while(_it != qmap.end()) {
        qDebug() << "_it.key(): " << _it.key() << " _it.value(): " << _it.value();
        _it++;
    }

    // 直接通过Key查找
    qDebug() << "qmap.value(\"Math\")" << qmap.value("English");   // 传入Key
    qDebug() << "qmap.value(\"Chemistry\")" << qmap.value("Chemistry");
    // 查询是否包含某个键
    qDebug() << "Contains: " <<  qmap.contains(/*Key*/ "English");

    // 获取所有Key和 Value
    QVector<QString> keys = qmap.keys();
    QVector<int> values = qmap.values();

    /****** QMultiMap ******/
    /*
     * 还是基于红黑树; 相同的键值,按照Value排序;
     */
    QMultiMap<QString, QString> qmultimap;
    qmultimap.insert("student", "name");
    qmultimap.insert("student", "number");
    qmultimap.insert("student", "age");
    // 查找第一个值
    qDebug() <<"qmultimap.value(\"student\")" << qmultimap.value("student");
    // 返回值对应的列表
    qDebug() <<"qmultimap.values(\"student\")" << qmultimap.values("student");
    // qmultimap.values("student")类型:  5QListI7QStringE
    qDebug() <<"qmultimap.values(\"student\")类型: " << typeid(qmultimap.values("student")).name();
}
/*
    QMap(("Chinese", 120)("English", 140)("Math", 120))
    QMap(("Chinese", 120)("English", 140))
    _it.key():  "Chinese"  _it.value():  120
    _it.key():  "English"  _it.value():  140
    qmap.value("Math") 0
    qmap.value("Chemistry") 0
    Contains:  true
 */



/*
 * QHash与QMap几乎相同的API 但是是无序.
 * 对应STL的 std::unordered_map
 * QHash因为是HashTable 查找速度快于QMap
 */
void QHashTest() {
    QHash<QString, int> qhash;
    qhash["Key1"] = 33;
    qhash["Key2"] = 44;
    qhash["Key3"] = 55;
    qhash["Key4"] = 66;
    qhash.insert("Key1", 99);

    QHash<QString, int>::Iterator _it= qhash.begin();
    while(_it != qhash.end()) {
        qDebug() << "Key: " << _it.key() << "\t Value: " << _it.value();
        _it++;
    }
    /*
        Key:  "Key4" 	 Value:  66
        Key:  "Key2" 	 Value:  44
        Key:  "Key1" 	 Value:  99
        Key:  "Key3" 	 Value:  55
    */

}


/***********************/
/* 自定义数据类型 */
/***********************/
struct CustomType {
    int a;
    float b;
    /* 这里必须写 const {; 因为传递给比较函数的参数是const 常量引用, 
     * 当一个对象被声明为 const（或者通过 const 引用传递）时，编译器在语义上就把它当你在这个作用域里的只读数据。
     * 为了保证哪怕你调用了它的成员函数也不会修改它的内部数据，
     * C++ 规定：一个 const 对象只能调用被 const 修饰的成员函数。
     * 类似 QHash 内部的操作
        const CustomType obj1 = ...;
        const CustomType obj2 = ...;
        if (obj1 == obj2) { ... }  // 这一步等价于 obj1.operator==(obj2)

     *  永远要这样重载 bool operator==(const CustomType& other) const; 
     */
    bool operator==(const CustomType& other ) const {
        if(other.a == this->a) return true;
        else return false;
    }
};
size_t qHash(const CustomType& key, size_t seed = 0) {
    return qHash(key.a, seed);  // 复用 int 的哈希
    // 或：return qHashMulti(seed, key.id, key.name);
}
void CustomTypeInQHashTest() {
    // 这里使用CustomType 写Insert时就会报错缺少 operator==()
    // QHash QMap 使用时前者要提供operator==()和全局哈希函数, 后者要提供 operator<()

    QHash<CustomType, int> qhash2;
    CustomType custom{1, 1.0};
    qhash2.insert(custom, 1);
}
