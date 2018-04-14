import QtQuick 1.1
import com.nokia.meego 1.1

PageStack{
	id:root;
	property alias text:textbox.text;
	signal killFbaEmu();
	//property alias 

	HeadView{
		id:head;
		anchors.top:parent.top;
		width:parent.width;
        title:qsTr("FBA Emulator Running Info");
		z:1;
	}

	TextEdit{
		id:textbox;
		anchors.fill:parent;
		anchors.topMargin:head.height;
		anchors.bottomMargin:toolbar.height;
		readOnly:true;
		verticalAlignment:TextEdit.AlignBottom;
		wrapMode:TextEdit.Wrap;
		font.pointSize:15;
		//cursorPosition:text.length;
	}

	function appendText(text)
	{
		if(text.length === 0)
			textbox.text = text;
		else
			textbox.text += text;
	}

	Rectangle{
		id:toolbar;
		anchors.bottom:parent.bottom;
		height:80;
		width:parent.width;
		Button{
			id:killbutton;
			width:200;
            text:qsTr("Terminate");
			anchors.centerIn:parent;
			onClicked:{
				killFbaEmu();
			}
		}
	}

}
