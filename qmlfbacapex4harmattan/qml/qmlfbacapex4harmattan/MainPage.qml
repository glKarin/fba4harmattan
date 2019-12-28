import QtQuick 1.1
import com.nokia.meego 1.1

KPage{
	id:root;
	objectName: "KZMainPage";

	HeadView{
		id:head;
		anchors.top:parent.top;
		anchors.left:parent.left;
		anchors.right:parent.right;
		z:999;
		title:tabgroup.currentTab.title;
		ToolIcon{
			anchors.verticalCenter: parent.verticalCenter;
			anchors.right: parent.right;
			anchors.rightMargin: constants.spacingMedium;
			iconId: "toolbar-close";
			onClicked: {
				if(quittimer.readyQuit)
				Qt.quit();
				else
				{
					quittimer.readyQuit = true;
					showMsg(qsTr("Click close button again to quit."));
					quittimer.restart();
				}
			}
		}
		Timer{
			id: quittimer;
			property bool readyQuit: false;
			interval: 3000;
			repeat: false;
			onTriggered: {
				readyQuit = false;
			}
		}
	}
	tools:ToolBarLayout{
		id:toolbar;
		ButtonRow{
			TabButton{
				id:romlist;
				iconSource:"../../rsc/icon-m-toolbar-list.png";
				enabled:!fbacapex.running;
                tab:romlistPage;
			}
			TabButton{
				id:setting;
				iconSource:"../../rsc/icon-m-toolbar-settings.png";
				enabled:!fbacapex.running;
				tab:settingPage;
				onClicked:{
				settingPage.setValue();
				}
			}
			TabButton{
                id:filelist;
				enabled:!fbacapex.running;
				iconSource:"../../rsc/icon-m-toolbar-directory.png";
                tab:filePage;
			}
			TabButton{
				id:runningButton;
				iconSource:"../../rsc/icon-m-toolbar-application.png";
				//enabled:!fbacapex.running;
				tab:runningPage;
			}
			TabButton{
				id:about;
				iconSource:"../../rsc/icon-m-toolbar-home.png";
				enabled:!fbacapex.running;
				tab:aboutPage;
			}
		}
	}

	TabGroup{
		id:tabgroup;
		anchors.fill:parent;
		anchors.topMargin:head.height;
		currentTab:romlistPage;
		FbaRomListPage{
			id:romlistPage;
			anchors.fill:parent;
			onOpenFbaFile:{
				root.emulate(file);
			}
		}
		SettingPage{
			id:settingPage;
			anchors.fill:parent;
		}
		FilePage{
			id:filePage;
			anchors.fill:parent;
			onOpenFbaFile:{
				root.emulate(file);
			}
		}
		RunLockPage{
			id:runningPage;
			anchors.fill:parent;
		}
		AboutPage{
			id:aboutPage;
			anchors.fill:parent;
		}
	}

	function emulate(file)
	{
		tabgroup.currentTab = runningPage;
		fbacapex.execFbaRunCommand(file);
	}
}
