import QtQuick 1.1
import com.nokia.meego 1.1
import karin.fba.capex 1.2

KPage{
	id:root;

	KZFbaCapex{
		id:fbacapex;
		onStdoutandstderrChanged:{
			runningPage.appendText(stdoutandstderr);
		}
		onSettingChanged:{
			settingPage.setValue(clock,sense,sound,samplerate,scaling,rotate,tweak,z80core,filter);/*,showfps);*/
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
				settingPage.setValue(fbacapex.clock,fbacapex.sense,fbacapex.sound,fbacapex.samplerate,fbacapex.scaling,fbacapex.rotate,fbacapex.tweak,fbacapex.z80core,fbacapex.filter);/*,showfps);*/
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
		currentTab:romlistPage;
		FbaRomListPage{
			id:romlistPage;
			model:fbacapex.romlist;
			onOpenFbaFile:{
				tabgroup.currentTab = runningPage;
				fbacapex.execFbaRunCommand(file);
			}
			onFilterRomlist:{
				fbacapex.filterRomlist(flag);
			}
			onRefrushRomlist:{
				fbacapex.refrushRomlist(flag);
			}
		}
		SettingPage{
			id:settingPage;
			onBackToDefault:{
				fbacapex.resetSetting();
			}
			onClockChanged:{
				fbacapex.clock = clock;
			}
			onSenseChanged:{
				fbacapex.sense = sense;
			}
			onSamplerateChanged:{
				fbacapex.samplerate = samplerate;
			}
			onTweakChanged:{
				fbacapex.tweak = tweak;
			}
			onZ80coreChanged:{
				fbacapex.z80core = z80core;
			}
			onFilterChanged:{
				fbacapex.filter = filter;
			}
			onSoundChanged:{
				fbacapex.sound = sound;
			}
		}
		FilePage{
			id:filePage;
			onOpenFbaFile:{
				tabgroup.currentTab = runningPage;
				fbacapex.execFbaRunCommand(file);
			}
		}
		RunLockPage{
			id:runningPage;
			onKillFbaEmu:{
				if(fbacapex.running)
					fbacapex.killFbaEmu();
			}

		}
		AboutPage{
			id:aboutPage;
		}
	}
}
