import QtQuick 1.1
import com.nokia.meego 1.1

KTabItem{
	id:root;
	objectName: "KZFbaRomListPage";
	title:qsTr("Choice FBA Rom File");
	signal openFbaFile(string file);

	property int __curRomListType: 0;
	property int __curSearchType: 0;

	Column{
		id:tools;
		anchors.top:parent.top;
		anchors.left:parent.left;
		width:app.inPortrait ? parent.width : constants.landscapeWidth;
		height:160;
		z:1;
		ButtonRow{
			id:buttonrow;
			width:parent.width;
			Button{
				id:allbutton
				text:qsTr("All");
				onClicked:{
					filterRomList(0);
				}
			}
			Button{
				id:availablebutton;
				text:qsTr("Available Only");
				onClicked:{
					filterRomList(1);
				}
			}
			Button{
				id:missingbutton;
				text:qsTr("Missing Only");
				onClicked:{
					filterRomList(2);
				}
			}
		}
		Button{
			width:parent.width;
			text:qsTr("Refrush");
			onClicked:{
				/*
				var cur;
				if(buttonrow.checkedButton === allbutton){
				cur = 0;
			}else if(buttonrow.checkedButton === availablebutton){
				cur = 1;
			}else if(buttonrow.checkedButton === missingbutton){
				cur = 2; 
			}
			//console.log(cur);
			*/
				fbacapex.refrushRomlist(root.__curRomListType);
				lst.currentIndex = 0;
				lst.positionViewAtBeginning();
			}
		}

		Row{
			width:parent.width;
			height: childrenRect.height;
			clip: true;
			TextField{
				id:textfield;
				width: parent.width - searchtyperow.width;
				placeholderText : qsTr("Search Game Name");
				inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase;
				platformStyle: TextFieldStyle {
					paddingRight: clear.width;
				}
				onTextChanged: {
					root.search(text);
				}
				ToolIcon{
					id:clear;
					anchors.right:parent.right;
					anchors.verticalCenter: parent.verticalCenter;
					iconId:"toolbar-close";
					onClicked: {
						textfield.text="";
					}
				}
			}
			ButtonRow{
				id: searchtyperow;
				width: 200;
				height: textfield.height;
				Button{
					text: qsTr("Name");
					onClicked: {
						root.search(textfield.text, 0);
					}
				}
				Button{
					text: qsTr("Index");
					onClicked: {
						root.search(textfield.text, 1);
					}
				}
			}
		}
	}

	ListView{
		id:lst;
		anchors.top: app.inPortrait ? tools.bottom : parent.top;
		anchors.left: app.inPortrait ? parent.left : tools.right;
		anchors.bottom: parent.bottom;
		anchors.right: parent.right;
		delegate:sign;
		focus: true;
		clip:true;
		spacing:constants.spacingTiny;
		model:fbacapex.romlist;
	}

	ScrollDecorator{
		flickableItem:lst;
	}

    function jump(index)
    {
        var i=parseInt(index);
        lst.currentIndex=i;
        lst.positionViewAtIndex(i,ListView.Beginning);
    }

	Component{
		id:sign;
		Rectangle{
			height:layout.height;
			width:ListView.view.width;
			color:ListView.isCurrentItem?"lightsteelblue":"white";
			Column{
				id:layout;
				width:parent.width;
				Text{
					width:parent.width;
					color:modelData.exists ? "red" : "black";
					font.pixelSize: constants.fontMedium;
					font.family: "Nokia Pure Text";
					wrapMode:Text.WordWrap;
					//wrapMode:Text.WrapAnywhere;
					text:modelData.name;
				}
				Text{
					width:parent.width;
					wrapMode:Text.WordWrap;
					font.family: "Nokia Pure Text";
					font.pixelSize: constants.fontSmall;
					color:modelData.exists ? "blue" : "gray";
					text:modelData.com + "  " +  modelData.year;
					horizontalAlignment: Text.AlignRight;
				}
			}
			MouseArea{
				anchors.fill:parent;
				onClicked:{
					lst.currentIndex=index;
				}
				onDoubleClicked:{
					lst.currentIndex=index;
					if(modelData.exists){
						openFbaFile(modelData.path);
					}
				}
			}
		}
	}

	function filterRomList(index)
	{
		root.__curRomListType = index;
		fbacapex.filterRomlist(index);
		lst.currentIndex = 0;
		lst.positionViewAtBeginning();
	}

	function search(text, type)
	{
		if(type !== undefined)
		{
			if(root.__curSearchType === type)
			return;
			fbacapex.filterRomlist();
			root.__curSearchType = type;
		}

		if(root.__curSearchType === 1)
		{
			var i = fbacapex.filterFbaRomName(text);
			lst.currentIndex = i;
			lst.positionViewAtIndex(i, ListView.Beginning);
		}
		else
		{
			var i = fbacapex.findRomList(text);
			lst.currentIndex = i === 0 ? -1 : 0;
			lst.positionViewAtIndex(i, ListView.Beginning);
		}
	}

	Component.onCompleted: {
		buttonrow.checkedButton = availablebutton;
		filterRomList(1);
	}
}
