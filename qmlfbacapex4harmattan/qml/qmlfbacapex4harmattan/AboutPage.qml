import QtQuick 1.1
import com.nokia.meego 1.1

KTabItem{
	id:root;
	title:qsTr("About");
	objectName: "KZAboutPage";

	Flickable{
		id: flick;
		anchors.fill: parent;
		contentWidth: layout.width;
		contentHeight: Math.max(height, layout.height);
		clip: true;

		Column{
			id: layout;
			anchors.horizontalCenter: parent.horizontalCenter;
			width: flick.width;
			spacing: constants.spacingMedium;
			Text{
				anchors.horizontalCenter: parent.horizontalCenter;
				width: parent.width;
				horizontalAlignment: Text.AlignHCenter;
				font.family: "Nokia Pure Text";
				color:"green";
				font.pixelSize: constants.fontBig; // 50
				font.bold: true;
				//text:"QFBACapex";
				text: _CPP_name;
			}
			Image{
				width:80;
				height:80;
				anchors.horizontalCenter: parent.horizontalCenter;
				smooth: true;
				source: Qt.resolvedUrl("../../rsc/fbaicon.png");
			}
			Text{
				width:parent.width;
				horizontalAlignment: Text.AlignHCenter;
				wrapMode:Text.WordWrap;
				color:"#9E1B29"; // pink
				font.pixelSize: constants.fontMedium; // 20
				font.family: "Nokia Pure Text";
				text:qsTr("A FBA Emulator Front End By QT For MeeGo Harmattan. \n - by Karin Zhao") + " (dospy - 香磷ヽ)";
			}
			AnimatedImage{
				anchors.horizontalCenter: parent.horizontalCenter;
				smooth: true;
				source: Qt.resolvedUrl("../../rsc/whip.gif");
				paused: !root.isCurrent;
			}
			Column{
				anchors.horizontalCenter: parent.horizontalCenter;
				width: parent.width;
				spacing: constants.spacingMicro;
				Repeater{
					model: [
						{
							name: qsTr("Version"),
							value: _CPP_version,
							link: "",
						},
						{
							name: qsTr("Dev"),
							value: _CPP_dev,
							link: _CPP_tmo,
						},
						{
							name: qsTr("Source"),
							value: "Github",
							link: _CPP_github,
						},
						{
							name: qsTr("Frontend download"),
							value: "Openrepos",
							link: _CPP_openrepos,
						},
						{
							name: qsTr("FBAlpha download"),
							value: "Openrepos",
							link: _CPP_openrepos_fba,
						},
					];
					delegate: Component{
						Text{
							width: parent.width;
							font.pixelSize: constants.fontMedium;
							text: modelData.name + ": " + (modelData.link ? "<a href='" + modelData.link + "'>" + modelData.value + "</a>" : modelData.value);
							wrapMode: Text.WordWrap;
							color: "black";
							onLinkActivated: Qt.openUrlExternally(link);
						}
					}
				}
			}
		}
	}

	ScrollDecorator{
		flickableItem: flick;
	}
}
