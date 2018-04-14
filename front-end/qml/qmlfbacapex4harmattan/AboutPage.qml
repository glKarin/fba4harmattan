import QtQuick 1.1
import com.nokia.meego 1.1

KPage{
    id:root;
    Column{
        anchors.fill:parent;
        HeadView{
            id:head;
            width:parent.width;
            title:qsTr("About");
        }
        Text{
            id:label;
            //width:parent.width;
            anchors.horizontalCenter: parent.horizontalCenter;
            color:"green";
            font.pointSize: 50;
            text:"QFBACapex";
        }
        Image{
            id:image;
            width:80;
            height:80;
            anchors.horizontalCenter: parent.horizontalCenter;
            source: "../../rsc/fbaicon.png";
        }
        TextArea{
            id:info;
            width:parent.width;
            anchors.horizontalCenter: parent.horizontalCenter;
            readOnly: true;
            text:qsTr("A FBA Emulator Front End By QT For MeeGo Harmattan. \n - by Karin Zhao (dospy - 香磷ヽ) \n");
        }
    }
}
