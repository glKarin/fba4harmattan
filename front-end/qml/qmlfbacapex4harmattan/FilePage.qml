import QtQuick 1.1
import com.nokia.meego 1.1
import karin.file.browser 1.3

PageStack{
	id:root;
	signal openFbaFile(string file);

	KFileBrowser{
		id:filemodel;
		onPathChanged:{
			lst.currentIndex=0;
		}
	}

	HeadView{
		id:head;
		width:parent.width;
        title:qsTr("Open FBA Rom File");
	}

	ListView{
		id:lst;
		anchors.fill:parent;
		anchors.topMargin:head.height;
		model:filemodel.filelist;
		delegate:sign;
		focus: true;
		clip:true;
		spacing:3;
	}
	ScrollDecorator{
		flickableItem:lst;
	}

	Component{
		id:sign;
		Rectangle{
			height:60;
			width:root.width;
			color:ListView.isCurrentItem?"lightsteelblue":"white";
			Column{
				anchors.fill:parent;
				Text{
					id:autoText;
					height:35;
					width:root.width;
					color:"black";
					font.pointSize:22;
					elide:Text.ElideMiddle;
					text:modelData.type ? modelData.name+"/" : modelData.name;
				}
				Text{
					id:autoText2;
					height:25;
					width:root.width;
					color:"grey";
					font.pointSize:18;
					text:modelData.owner + " : " + modelData.group + "  " + modelData.size +"  " +modelData.permission;
				}
		}
			MouseArea{
				anchors.fill:parent;
				onClicked:{
					lst.currentIndex=index;
				}
				onDoubleClicked:{
					if(modelData.type)
					{
						filemodel.path=modelData.path;
						lst.positionViewAtBeginning();
					}
					else
					{
						openFbaFile(modelData.path);
					}
				}
			}
		}
	}

}
