import QtQuick
import QtQuick.Controls
import "component/qml_component"

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    QThreadTest{

    }

    Rectangle {
        width: parent.width/2
        height: parent.height/2
        anchors.centerIn: parent
        radius: 5

        border.color: "red"
        border.width: 2
        Column {
            width: parent.width
            height: parent.height
            anchors.centerIn: parent
            spacing: 5
            Text {
                id: textCounter
                // text: backend.getCounter() // -> QML绑定函数不能自动更新 需要绑定值
                text: backend.m_counter
                width: 100
                height: 50
                font.pixelSize: 20
            }
            Button {
                id: btnAddCounter
                text: "Add one"
                width: 100
                height: 50
                onClicked: backend.increment()
            }

        }
    }
}
