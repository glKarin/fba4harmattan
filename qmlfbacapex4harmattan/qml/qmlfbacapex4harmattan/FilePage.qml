import QtQuick 1.1
import com.nokia.meego 1.1
import karin.file.browser 1.3

KTabItem{
	id:root;
	objectName: "KZFilePage";

	title:qsTr("Open FBA Rom File");
	signal openFbaFile(string file);

	KFileBrowser{
		id:filemodel;
		onPathChanged:{
			lst.currentIndex=0;
			lst.positionViewAtBeginning();
			fbacapex.appLastPath = path;
		}
	}

	Item{
		id: fileheader;
		anchors.top: parent.top;
		anchors.left: parent.left;
		width: app.inPortrait ? parent.width : constants.landscapeWidth;
		height: constants.sizeMedium;
		z: 1;
		clip: true;
		Text{
			anchors.fill: parent;
			anchors.margins: constants.spacingSmall;
			color:"black";
			font.pixelSize: constants.fontSmall;
			font.family: "Nokia Pure Text";
			text: filemodel.path;
			elide: Text.ElideLeft;
			wrapMode: Text.WrapAnywhere;
			verticalAlignment: Text.AlignVCenter;
			maximumLineCount: 2;
		}
	}

	ListView{
		id:lst;
		anchors.top: app.inPortrait ? fileheader.bottom : parent.top;
		anchors.left: app.inPortrait ? parent.left : fileheader.right;
		anchors.right: parent.right;
		anchors.bottom: app.inPortrait ? filetools.top : parent.bottom;
		model:filemodel.filelist;
		delegate:sign;
		focus: true;
		clip:true;
		spacing:constants.spacingTiny; // 3
	}
	ScrollDecorator{
		flickableItem:lst;
	}

	Row{
		id: filetools;
		anchors.bottom: parent.bottom;
		anchors.left: parent.left;
		anchors.right: app.inPortrait ? parent.right : fileheader.right;
		height: app.inPortrait ? constants.sizeMedium : constants.sizeLarge;
		z: 1;
		clip: true;
		CheckBox{
			anchors.verticalCenter: parent.verticalCenter;
			width: parent.width / 2;
			text: qsTr("Show hidden");
			onClicked: {
				filemodel.showHidden = checked;
			}
		}
		CheckBox{
			anchors.verticalCenter: parent.verticalCenter;
			width: parent.width / 2;
			text: qsTr("ZIP only");
			onClicked: {
				filemodel.filter = checked ? "*.zip" : "";
			}
		}
	}

	Component{
		id:sign;
		Rectangle{
			height:layout.height;
			width:ListView.view.width;
			color:ListView.isCurrentItem?"lightsteelblue":"white";
			Column{
				id:layout;
				anchors.horizontalCenter: parent.horizontalCenter;
				width:parent.width - 2 * constants.spacingTiny;
				Text{
					width:parent.width;
					color:"black";
					font.pixelSize: constants.fontMedium; // 22
					font.family: "Nokia Pure Text";
					wrapMode:Text.WrapAnywhere;
					text:modelData.type ? modelData.name+"/" : modelData.name;
				}
				Text{
					width:parent.width;
					color:"grey";
					font.pixelSize: constants.fontSmall; // 18
					font.family: "Nokia Pure Text";
					text:modelData.owner + " : " + modelData.group + "  " + modelData.size +"  " +modelData.permission;
				}
		}
			MouseArea{
				anchors.fill:parent;
				onClicked:{
					lst.currentIndex=index;
				}
				onDoubleClicked:{
					lst.currentIndex=index;
					if(modelData.type)
					{
						filemodel.path=modelData.path;
					}
					else
					{
						root.openFbaFile(modelData.path);
					}
				}
			}
		}
	}

	Component.onCompleted: {
		filemodel.path = fbacapex.appLastPath;
	}
}
