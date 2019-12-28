import QtQuick 1.1

Rectangle{
	id:root;
	objectName: "KZSettingItem";
	property alias spacing: layout.spacing;
	default property alias items: layout.children;

	anchors.horizontalCenter: parent.horizontalCenter;
	width:parent.width;
	height: childrenRect.height + 2 * constants.spacingLarge; // 100
	clip: true;

	Column{
		id: layout;
		anchors.top: parent.top;
		anchors.topMargin: constants.spacingLarge;
		anchors.horizontalCenter: parent.horizontalCenter;
		width: parent.width - 2 * constants.spacingMedium;
		spacing: constants.spacingTiny;
	}
}
