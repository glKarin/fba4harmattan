import QtQuick 1.1
import com.nokia.meego 1.1

Item{
	id:root;
	property string	title;
	property bool isCurrent: parent && parent.hasOwnProperty("currentTab") ? parent.currentTab.objectName === root.objectName : false;

	objectName: "KZTabItem";
	//anchors.fill: parent;
	//clip: true;
}
