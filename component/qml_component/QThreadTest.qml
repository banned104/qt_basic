import QtQuick
import QtQuick.Controls
import com.mythread.test 1.0

Item {

    // Rectangle {
    //     color: "transparent"
    // }

    Connections {
        target: QThreadTestCPP              // C++已经注册 singleton
        function onMessage(info) {          // C++ 中的信号名是 message, 这里需要首字母大写
            console.log("[C++]:"+ info)
        }
    }

    Row {
        Column {
            Button {
                text: "QThread Run"
                onClicked: QThreadTestCPP.start()
            }

            Button {
                text: "QThread Quit"
                onClicked: QThreadTestCPP.quit()
            }

            Button {
                text: "QThread Terminate"
                onClicked: QThreadTestCPP.terminate()
            }
        }

        Column {
            Button {
                text: "getSomething"
                onClicked: QThreadTestCPP.getSomething()
            }
            Button {
                text: "setSomething"
                onClicked: QThreadTestCPP.setSomething()
            }
            Button {
                text: "doSomething"
                onClicked: QThreadTestCPP.doSomething()
            }
        }

        Text {
            text: qsTr("text")

        }
    }

    // ProgressBar {

    // }

}
