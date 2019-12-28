import QtQuick 1.1
import com.nokia.meego 1.1

KTabItem{
	id:root;
	objectName: "KZRunLockPage";

	title:qsTr("FBA Emulator Running Info");

	Flickable{
		id: flick;
		anchors.top: parent.top;
		anchors.left: parent.left;
		anchors.right: app.inPortrait ? parent.right : toolbar.left;
		anchors.bottom: app.inPortrait ? toolbar.top : parent.bottom;
		contentWidth:textbox.width;
		contentHeight:Math.max(textbox.height, height);
		clip: true;

		Text{
			id:textbox;
			anchors.horizontalCenter: parent.horizontalCenter;
			width: flick.width;
			wrapMode:Text.WrapAnywhere;
			color:"black";
			font.family: "Nokia Pure Text";
			font.pixelSize:constants.fontSmall; // 15
		}

	}
	Connections{
		target:fbacapex;
		onConsoleLog:{
			runningPage.appendText(text, channel);
		}
	}

	function appendText(text, channel)
	{
		if(text.length === 0 || channel === 0)
		textbox.text = "";
		else
		textbox.text += ("<font color='#%1'>" + text.replace(/\n/g, "<br/>") + "</font>").arg(channel == 1 ? "000000" : "FF0000");

		flick.contentY = flick.contentHeight - flick.height;
	}

	Rectangle{
		id:toolbar;
		anchors.bottom:parent.bottom;
		anchors.right:parent.right;
		height:app.inPortrait ? constants.sizeLarge : parent.height; // 80
		width:app.inPortrait ? parent.width : constants.landscapeWidth / 3 * 2;
		Button{
			width:200;
			enabled: fbacapex.running;
			text:qsTr("Terminate");
			anchors.centerIn:parent;
			onClicked:{
				if(fbacapex.running){
					fbacapex.killFbaEmu();
				}
			}
		}
	}

	ScrollDecorator{
		flickableItem: flick;
	}
}
