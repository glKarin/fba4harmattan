import QtQuick 1.1

Rectangle{
	id:root;
	property alias title:autoText.text;
	height:80;
	z:1;
	color:"red";

		Text{
			id:autoText;
			anchors.centerIn:parent;
			font.pointSize:24;
			color:"black";
		}

}
