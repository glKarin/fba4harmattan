import QtQuick 1.1

Rectangle{
	id:root;
	objectName: "KZHeadView";
	property alias title:autoText.text;
	width:parent.width;
	height:80;
	color:"red";
	clip: true;

		Text{
			id:autoText;
			anchors.fill: parent;
			horizontalAlignment: Text.AlignHCenter;
			verticalAlignment: Text.AlignVCenter;
			font.bold: true;
			font.pixelSize: constants.fontMedium; // 24
			font.family: "Nokia Pure Text";
			color:"black";
			elide: Text.ElideRight;
		}

}
