import QtQuick 1.1
import com.nokia.meego 1.1

PageStack{
	id:root;
	signal openFbaFile(string file);
	property alias model:lst.model;
	signal filterRomlist(int flag);
	signal refrushRomlist(int flag);

	HeadView{
		id:head;
		width:parent.width;
        title:qsTr("Choice FBA Rom File");
	}

	ButtonRow{
		id:buttonrow;
		anchors.top:head.bottom;
		width:parent.width;
		z:1;
		height:60;
		Button{
			id:allbutton
			text:qsTr("All");
			onClicked:{
				filterRomlist(0);
				lst.currentIndex = 0;
				lst.positionViewAtBeginning();
			}
		}
		Button{
			id:availablebutton;
			text:qsTr("Available Only");
			onClicked:{
				filterRomlist(1);
				lst.currentIndex = 0;
				lst.positionViewAtBeginning();
			}
		}
		Button{
			id:missingbutton;
			text:qsTr("Missing Only");
			onClicked:{
				filterRomlist(2);
				lst.currentIndex = 0;
				lst.positionViewAtBeginning();
			}
		}
	}

	ListView{
		id:lst;
		anchors.fill:parent;
		anchors.topMargin:head.height + buttonrow.height;
		delegate:sign;
		focus: true;
		clip:true;
		spacing:2;
		header:headerbutton;
		/*
		section.criteria:ViewSection.FirstCharacter;
		section.property:"name";
		section.delegate:SectionDelegate{text:section}
		*/
	}

	ScrollDecorator{
		flickableItem:lst;
	}

	/*
	FastScroll {
		listView:lst;
	}
	*/

	Component{
		id:headerbutton;
		Button{
			width:root.width;
            text:qsTr("Refrush");
			onClicked:{
				var cur;
				if(buttonrow.checkedButton === allbutton)
				cur = 0;
				else if(buttonrow.checkedButton === availablebutton)
				cur = 1;
				else if(buttonrow.checkedButton === missingbutton)
				cur = 2;
				console.log(cur);
				refrushRomlist(cur);
                lst.currentIndex = 0;
                lst.positionViewAtBeginning();
			}
		}
	}

	Component{
		id:sign;
		Rectangle{
			height:80;
			width:root.width;
			color:ListView.isCurrentItem?"lightsteelblue":"white";
			Column{
				anchors.fill:parent;
				Text{
					id:autoText;
					width:parent.width;
					height:50;
					color:modelData.exists ? "red" : "black";
					font.pointSize:24;
					elide:Text.ElideMiddle;
					text:modelData.name;
				}
				Text{
					id:autoText2;
					width:parent.width;
					height:30;
					font.pointSize:20;
					elide:Text.ElideMiddle;
					color:modelData.exists ? "red" : "black";
					text:modelData.year + "  " +  modelData.com;
				}
			}
			MouseArea{
				anchors.fill:parent;
				onClicked:{
					lst.currentIndex=index;
				}
				onDoubleClicked:{
					if(modelData.exists){
						openFbaFile(modelData.path);
					}
				}
			}
		}
	}
}
