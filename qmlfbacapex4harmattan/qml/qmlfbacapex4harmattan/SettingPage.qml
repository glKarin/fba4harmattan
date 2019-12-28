import QtQuick 1.1
import com.nokia.meego 1.1

KTabItem{
	id:root;
	objectName: "KZSettingPage";

	title: qsTr("Fba2x Settings");

	function setValue()
	{
		clockslider.value = fbacapex.clock;
		senseslider.value = fbacapex.sense;
		soundswitch.checked = fbacapex.sound;
		samplerateslider.value = fbacapex.samplerate;
		scalingbuttonrow.checkedButton = (fbacapex.scaling === 0 ? scalingnonebutton : (fbacapex.scaling === 1 ? scaling2xbutton : (fbacapex.scaling === 2 ? scalingbestfitbutton : scalingfullbutton)));
		rotatebuttonrow.checkedButton = (fbacapex.rotate == 1 ? rotatehorizontalbutton : (fbacapex.rotate === 2 ? rotateverticalbutton : rotateautobutton));
		tweakbuttonrow.checkedButton = (fbacapex.tweak === 1 ? tweakforcem68kbutton : (fbacapex.tweak === 2 ? tweakforcecyclonebutton : tweakautobutton));
		z80emucorebuttonrow.checkedButton = fbacapex.z80core === 1 ? z80emucorecz80button : z80emucoredrz80button;
		filterbuttonrow.checkedButton = fbacapex.filter === 1 ? filternonebutton : filterdefaultbutton;
		showfpsswitch.checked = fbacapex.showfps;

		joystickmodebuttonrow.checkedButton = (fbacapex.harmJoystickMode === 1 ? joystickmodecircle : (fbacapex.harmJoystickMode === 2 ? joystickmodebutton : joystickmodejoystick));
		buttonlayoutbuttonrow.checkedButton = (fbacapex.harmButtonLayout === 1 ? buttonlayoutlandscapebutton : (fbacapex.harmButtonLayout === 2 ? buttonlayout4button : buttonlayoutportraitbutton));
		show2pswitch.checked = fbacapex.harmShow2PController;
		biosinput.text = fbacapex.harmBIOS;
		enablebiosswitch.checked = fbacapex.harmEnableBIOS;
		
		orientationbuttonrow.checkedButton = (fbacapex.orientation === 1 ? orientationportraitbutton : (fbacapex.orientation === 2 ? orientationlandscapebutton : orientationautobutton));
		//fbapathinput.text = fbacapex.fbaPath;
		runmodebuttonrow.checkedButton = (fbacapex.runMode === 1 ? runmodesequentialbutton : (fbacapex.runMode === 2 ? runmodeoncebutton : runmodeparallelbutton));
	}

	/*
	HeadView{
		id:head;
		title:qsTr("Fba2x Settings");
	}
	*/
	
 ButtonRow{
	 id: tabrow;
	 anchors.top: parent.top;
	 anchors.left: parent.left;
	 width: app.inPortrait ? parent.width : constants.landscapeWidth;
	 TabButton{
		 text: qsTr("General");
		 tab: fba2xtab;
	 }
	 TabButton{
		 text: qsTr("Harmattan");
		 tab: harmtab;
	 }
	 TabButton{
		 text: qsTr("Syetem");
		 tab: systab;
	 }
 }

 TabGroup{
	 id: tabgroup;
	 anchors.top: app.inPortrait ? tabrow.bottom : parent.top;
	 anchors.left: app.inPortrait ? parent.left : tabrow.right;
	 anchors.right: parent.right;
	 anchors.bottom: app.inPortrait ? tool.top : parent.bottom;
	 currentTab: fba2xtab;

	 Item{
		 id: fba2xtab;
		 anchors.fill:parent;
		 Flickable{
			 id: fba2xflick;
			 anchors.fill:parent;
			 contentWidth:fba2xlayout.width;
			 contentHeight: Math.max(fba2xlayout.height, height);
			 clip: true;

			 Column{
				 id: fba2xlayout;
				 anchors.horizontalCenter: parent.horizontalCenter;
				 width: fba2xflick.width;
				 SettingItem{
					 id:clockset;
					 Text{
						 anchors.horizontalCenter: parent.horizontalCenter;
						 text:qsTr("FBA2X Clock : ") + clockslider.value + "MHZ";
						 font.pixelSize: constants.fontMedium;
						 verticalAlignment: Text.AlignVCenter;
						 width:parent.width;
					 }
					 Slider{
						 id:clockslider;
						 anchors.horizontalCenter: parent.horizontalCenter;
						 width:parent.width;
						 minimumValue:200;
						 maximumValue:1200;
						 stepSize:1;
						 onValueChanged: {
							 if(fbacapex)
							 fbacapex.clock = value;
						 }
					 }
				 }
				 SettingItem{
					 id:senseset;
					 Text{
						 anchors.horizontalCenter: parent.horizontalCenter;
						 text:qsTr("Analogue Sensitivity : ") + senseslider.value + "%";
						 font.pixelSize: constants.fontMedium;
						 verticalAlignment: Text.AlignVCenter;
						 width:parent.width;
					 }
					 Slider{
						 id:senseslider;
						 anchors.horizontalCenter: parent.horizontalCenter;
						 width:parent.width;
						 minimumValue:10;
						 maximumValue:100;
						 stepSize:1;
						 onValueChanged: {
							 if(fbacapex)
							 fbacapex.sense = value;
						 }
					 }
				 }
				 SettingItem{
					 id:soundset;
					 Row{
						 anchors.horizontalCenter: parent.horizontalCenter;
						 width: parent.width;
						 height: constants.sizeSmall;
						 Text{
							 text:qsTr("Sound : ") + ((soundswitch.checked) ? qsTr("Enable") : qsTr("Disable"));
							 font.pixelSize: constants.fontMedium;
							 verticalAlignment: Text.AlignVCenter;
							 anchors.verticalCenter:parent.verticalCenter;
							 width: parent.width - soundswitch.width;
							 height: parent.height;
						 }
						 Switch{
							 id:soundswitch;
							 anchors.verticalCenter:parent.verticalCenter;
							 onCheckedChanged: {
								 fbacapex.sound = checked;
							 }
						 }
					 }
				 }
				 SettingItem{
					 id:samplerateset;
					 Text{
						 anchors.horizontalCenter: parent.horizontalCenter;
						 text:qsTr("Audio Sample Rate : ") + samplerateslider.value + "HZ";
						 font.pixelSize: constants.fontMedium;
						 verticalAlignment: Text.AlignVCenter;
						 width:parent.width;
					 }
					 Slider{
						 id:samplerateslider;
						 anchors.horizontalCenter: parent.horizontalCenter;
						 width:parent.width;
						 minimumValue:11025;
						 maximumValue:44100;
						 stepSize:11025;
						 onValueChanged: {
							 if(fbacapex)
							 fbacapex.samplerate = value;
						 }
					 }
				 }
				 SettingItem{
					 id:scalingset;
					 Text{
						 anchors.horizontalCenter: parent.horizontalCenter;
						 text:qsTr("Scaling : ") + scalingbuttonrow.checkedButton.text;
						 font.pixelSize: constants.fontMedium;
						 verticalAlignment: Text.AlignVCenter;
						 width:parent.width;
					 }
					 ButtonRow{
						 id:scalingbuttonrow;
						 anchors.horizontalCenter: parent.horizontalCenter;
						 width: parent.width;
						 enabled:false;
						 Button{
							 id:scalingnonebutton;
							 text:qsTr("None");
						 }
						 Button{
							 id:scaling2xbutton;
							 text:qsTr("2X");
						 }
						 Button{
							 id:scalingbestfitbutton;
							 text:qsTr("Best Fit");
						 }
						 Button{
							 id:scalingfullbutton;
							 text:qsTr("Full");
						 }
					 }
				 }
				 SettingItem{
					 id:rotateset;
					 Text{
						 anchors.horizontalCenter: parent.horizontalCenter;
						 text:qsTr("Rotate : ") + rotatebuttonrow.checkedButton.text;
						 font.pixelSize: constants.fontMedium;
						 verticalAlignment: Text.AlignVCenter;
						 width:parent.width;
					 }
					 ButtonRow{
						 id:rotatebuttonrow;
						 anchors.horizontalCenter: parent.horizontalCenter;
						 width:parent.width;
						 enabled:false;
						 Button{
							 id:rotateautobutton;
							 text:qsTr("Auto");
						 }
						 Button{
							 id:rotatehorizontalbutton;
							 text:qsTr("Horizontal");
						 }
						 Button{
							 id:rotateverticalbutton;
							 text:qsTr("Vertical");
						 }
					 }
				 }
				 SettingItem{
					 id:tweakset;
					 Text{
						 anchors.horizontalCenter: parent.horizontalCenter;
						 text:qsTr("68K Emu Core : ") + tweakbuttonrow.checkedButton.text;
						 font.pixelSize: constants.fontMedium;
						 verticalAlignment: Text.AlignVCenter;
						 width:parent.width;
					 }
					 ButtonRow{
						 id:tweakbuttonrow;
						 anchors.horizontalCenter: parent.horizontalCenter;
						 width:parent.width;
						 Button{
							 id:tweakautobutton;
							 text:qsTr("Auto");
							 onClicked:{
								 fbacapex.tweak = 0;
							 }
						 }
						 Button{
							 id:tweakforcem68kbutton;
							 text:qsTr("Force M68K");
							 onClicked:{
								 fbacapex.tweak = 1;
							 }
						 }
						 Button{
							 id:tweakforcecyclonebutton;
							 text:qsTr("Force Cyclone");
							 onClicked:{
								 fbacapex.tweak = 2;
							 }
						 }
					 }
				 }
				 SettingItem{
					 id:z80emucoreset;
					 Text{
						 anchors.horizontalCenter: parent.horizontalCenter;
						 text:qsTr("Z80 Emu Core : ") + z80emucorebuttonrow.checkedButton.text;
						 font.pixelSize: constants.fontMedium;
						 verticalAlignment: Text.AlignVCenter;
						 width:parent.width;
					 }
					 ButtonRow{
						 id:z80emucorebuttonrow;
						 anchors.horizontalCenter: parent.horizontalCenter;
						 width:parent.width;
						 Button{
							 id:z80emucoredrz80button;
							 text:qsTr("DR Z80");
							 onClicked:{
								 fbacapex.z80core = 0;
							 }
						 }
						 Button{
							 id:z80emucorecz80button;
							 text:qsTr("C Z80");
							 onClicked:{
								 fbacapex.z80core = 1;
							 }
						 }
					 }
				 }
				 SettingItem{
					 id:filterset;
					 Text{
						 text:qsTr("Filter : ") + filterbuttonrow.checkedButton.text;
						 font.pixelSize: constants.fontMedium;
						 verticalAlignment: Text.AlignVCenter;
						 width:parent.width;
					 }
					 ButtonRow{
						 id:filterbuttonrow;
						 anchors.horizontalCenter: parent.horizontalCenter;
						 width:parent.width;
						 Button{
							 id:filterdefaultbutton;
							 text:qsTr("Default");
							 onClicked:{
								 fbacapex.filter = 0;
							 }
						 }
						 Button{
							 id:filternonebutton;
							 text:qsTr("None");
							 onClicked:{
								 fbacapex.filter = 1;
							 }
						 }
					 }
				 }
				 SettingItem{
					 id:showfpsset;
					 Row{
						 anchors.horizontalCenter: parent.horizontalCenter;
						 width: parent.width;
						 height: constants.sizeSmall;
						 Text{
							 text:qsTr("Show FPS : ") + ((showfpsswitch.checked) ? qsTr("Enable") : qsTr("Disable"));
							 font.pixelSize: constants.fontMedium;
							 verticalAlignment: Text.AlignVCenter;
							 height: parent.height;
							 anchors.verticalCenter:parent.verticalCenter;
							 width: parent.width - showfpsswitch.width;
						 }
						 Switch{
							 id:showfpsswitch;
							 anchors.verticalCenter:parent.verticalCenter;
							 onCheckedChanged: {
								 fbacapex.showfps = checked;
							 }
						 }
					 }
				 }
			 }
		 }
		 ScrollDecorator{
			 flickableItem: fba2xflick;
		 }
	 }

	 Item{
		 id: harmtab;
		 anchors.fill:parent;
		 Flickable{
			 id: harmflick;
			 anchors.fill:parent;
			 contentWidth:harmlayout.width;
			 contentHeight: Math.max(harmlayout.height, height);
			 clip: true;

			 Column{
				 id: harmlayout;
				 anchors.horizontalCenter: parent.horizontalCenter;
				 width: harmflick.width;
				 SettingItem{
					 id: joystickmodeset;
					 Text{
						 anchors.horizontalCenter: parent.horizontalCenter;
						 text:qsTr("Joystick mode : ") + joystickmodebuttonrow.checkedButton.text;
						 font.pixelSize: constants.fontMedium;
						 verticalAlignment: Text.AlignVCenter;
						 width:parent.width;
					 }
					 ButtonRow{
						 id:joystickmodebuttonrow;
						 width:parent.width;
						 anchors.horizontalCenter: parent.horizontalCenter;
						 Button{
							 id: joystickmodejoystick;
							 text:qsTr("Joystick");
							 onClicked:{
								 fbacapex.harmJoystickMode = 0;
							 }
						 }
						 Button{
							 id: joystickmodecircle;
							 text:qsTr("Circle");
							 onClicked:{
								 fbacapex.harmJoystickMode = 1;
							 }
						 }
						 Button{
							 id: joystickmodebutton;
							 text:qsTr("Button");
							 onClicked:{
								 fbacapex.harmJoystickMode = 2;
							 }
						 }
					 }
				 }
				 SettingItem{
					 id: buttonlayoutset;
					 Text{
						 anchors.horizontalCenter: parent.horizontalCenter;
						 text:qsTr("Button layout : ") + buttonlayoutbuttonrow.checkedButton.text;
						 font.pixelSize: constants.fontMedium;
						 verticalAlignment: Text.AlignVCenter;
						 width:parent.width;
					 }
					 ButtonRow{
						 id:buttonlayoutbuttonrow;
						 width:parent.width;
						 anchors.horizontalCenter: parent.horizontalCenter;
						 Button{
							 id: buttonlayoutportraitbutton;
							 text:qsTr("Portrait");
							 onClicked:{
								 fbacapex.harmButtonLayout = 0;
							 }
						 }
						 Button{
							 id: buttonlayoutlandscapebutton;
							 text:qsTr("Landscape");
							 onClicked:{
								 fbacapex.harmButtonLayout = 1;
							 }
						 }
						 Button{
							 id: buttonlayout4button;
							 text:qsTr("ABCD only");
							 onClicked:{
								 fbacapex.harmButtonLayout = 2;
							 }
						 }
					 }
				 }
				 SettingItem{
					 id: show2pset;
					 Row{
						 anchors.horizontalCenter: parent.horizontalCenter;
						 width: parent.width;
						 height: constants.sizeSmall;
						 Text{
							 text:qsTr("Show 2P controller : ") + ((show2pswitch.checked) ? qsTr("Enable") : qsTr("Disable"));
							 font.pixelSize: constants.fontMedium;
							 verticalAlignment: Text.AlignVCenter;
							 height: parent.height;
							 anchors.verticalCenter:parent.verticalCenter;
							 width: parent.width - show2pswitch.width;
						 }
						 Switch{
							 id: show2pswitch;
							 anchors.verticalCenter:parent.verticalCenter;
							 onCheckedChanged: {
								 fbacapex.harmShow2PController = checked;
							 }
						 }
					 }
				 }
				 SettingItem{
					 id: enablebiosset;
					 Row{
						 anchors.horizontalCenter: parent.horizontalCenter;
						 width: parent.width;
						 height: constants.sizeSmall;
						 Text{
							 text:qsTr("BIOS setting : ") + ((enablebiosswitch.checked) ? qsTr("Enable") : qsTr("Disable"));
							 font.pixelSize: constants.fontMedium;
							 verticalAlignment: Text.AlignVCenter;
							 height: parent.height;
							 anchors.verticalCenter:parent.verticalCenter;
							 width: parent.width - enablebiosswitch.width;
						 }
						 Switch{
							 id: enablebiosswitch;
							 anchors.verticalCenter:parent.verticalCenter;
							 onCheckedChanged: {
								 fbacapex.harmEnableBIOS = checked;
							 }
						 }
					 }
				 }
				 SettingItem{
					 id: biosset;
					 enabled: fbacapex.harmEnableBIOS;
					 Text{
						 anchors.horizontalCenter: parent.horizontalCenter;
						 width: parent.width;
						 text:qsTr("BIOS(like NEOGEO, set BIOS to 'AES ...' for Home mode, all supported BIOS will show in runtime console log) : ") + biosinput.text;
						 font.pixelSize: constants.fontMedium;
						 verticalAlignment: Text.AlignVCenter;
						 wrapMode: Text.WrapAnywhere;
						 //maximumLineCount: 3;
						 //elide: Text.ElideRight;
					 }
					 TextField{
						 id: biosinput;
						 anchors.horizontalCenter: parent.horizontalCenter;
						 width: parent.width;
						 readOnly: !biosset.enabled;
						 placeholderText : qsTr("Input BIOS name(all supported BIOS will show in runtime console log)");
						 inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase;
						 platformStyle: TextFieldStyle {
							 paddingRight: clear.width;
						 }
						 /*
							onTextChanged: {
								fbacapex.harmBIOS = text;
							}
							*/
						 platformSipAttributes:SipAttributes {
							 actionKeyLabel: qsTr("Save");
							 actionKeyHighlighted: actionKeyEnabled;
							 actionKeyEnabled: biosinput.text.length !== "";
						 }
						 Keys.onReturnPressed:{
							 fbacapex.harmBIOS = biosinput.text;
						 }
						 ToolIcon{
							 id:clear;
							 anchors.right:parent.right;
							 anchors.verticalCenter: parent.verticalCenter;
							 iconId:"toolbar-close";
							 enabled: biosset.enabled;
							 visible: enabled;
							 onClicked: {
								 biosinput.text="";
								 fbacapex.harmBIOS = biosinput.text;
							 }
						 }
					 }
				 }
			 }
		 }
		 ScrollDecorator{
			 flickableItem: harmflick;
		 }
	 }

	 Item{
		 id: systab;
		 anchors.fill:parent;
		 Flickable{
			 id: sysflick;
			 anchors.fill:parent;
			 contentWidth:syslayout.width;
			 contentHeight: Math.max(syslayout.height, height);
			 clip: true;

			 Column{
				 id: syslayout;
				 anchors.horizontalCenter: parent.horizontalCenter;
				 width: sysflick.width;
				 SettingItem{
					 id: orientationset;
					 Text{
						 anchors.horizontalCenter: parent.horizontalCenter;
						 text:qsTr("Orientation : ") + orientationbuttonrow.checkedButton.text;
						 font.pixelSize: constants.fontMedium;
						 verticalAlignment: Text.AlignVCenter;
						 width:parent.width;
					 }
					 ButtonRow{
						 id:orientationbuttonrow;
						 width:parent.width;
						 anchors.horizontalCenter: parent.horizontalCenter;
						 Button{
							 id: orientationautobutton;
							 text:qsTr("Automatic");
							 onClicked:{
								 fbacapex.orientation = 0;
							 }
						 }
						 Button{
							 id: orientationportraitbutton;
							 text:qsTr("Portrait");
							 onClicked:{
								 fbacapex.orientation = 1;
							 }
						 }
						 Button{
							 id: orientationlandscapebutton;
							 text:qsTr("Landscape");
							 onClicked:{
								 fbacapex.orientation = 2;
							 }
						 }
					 }
				 }
				 SettingItem{
					 id: runmodeset;
					 Text{
						 anchors.horizontalCenter: parent.horizontalCenter;
						 text:qsTr("Run mode : ") + runmodebuttonrow.checkedButton.text;
						 font.pixelSize: constants.fontMedium;
						 verticalAlignment: Text.AlignVCenter;
						 width:parent.width;
					 }
					 ButtonRow{
						 id:runmodebuttonrow;
						 width:parent.width;
						 anchors.horizontalCenter: parent.horizontalCenter;
						 Button{
							 id: runmodeparallelbutton;
							 text:qsTr("Parallel");
							 onClicked:{
								 fbacapex.runMode = 0;
							 }
						 }
						 Button{
							 id: runmodesequentialbutton;
							 text:qsTr("Sequential");
							 onClicked:{
								 fbacapex.runMode = 1;
							 }
						 }
						 Button{
							 id: runmodeoncebutton;
							 text:qsTr("Once");
							 onClicked:{
								 fbacapex.runMode = 2;
							 }
						 }
					 }
				 }
				 /*
				 SettingItem{
					 id: fbapathset;
					 Text{
						 anchors.horizontalCenter: parent.horizontalCenter;
						 width: parent.width;
						 text:qsTr("FBAlpha binary path(Using 'Internal' will run '/opt/qmlfbacapex4harmattan/fbalpha/fba', using '$PATH' will run '$PATH/fba') : ") + fbapathinput.text;
						 font.pixelSize: constants.fontMedium;
						 verticalAlignment: Text.AlignVCenter;
						 wrapMode: Text.WrapAnywhere;
						 //maximumLineCount: 3;
						 //elide: Text.ElideRight;
					 }
					 TextField{
						 id: fbapathinput;
						 anchors.horizontalCenter: parent.horizontalCenter;
						 width: parent.width;
						 placeholderText : qsTr("Input special FBAlpha binary path.");
						 inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase;
						 platformStyle: TextFieldStyle {
							 paddingRight: clear2.width;
						 }
						 platformSipAttributes:SipAttributes {
							 actionKeyLabel: qsTr("Save");
							 actionKeyHighlighted: actionKeyEnabled;
							 actionKeyEnabled: fbapathinput.text.length !== "";
						 }
						 Keys.onReturnPressed:{
							 fbacapex.fbaPath = fbapathinput.text;
						 }
						 ToolIcon{
							 id:clear2;
							 anchors.right:parent.right;
							 anchors.verticalCenter: parent.verticalCenter;
							 iconId:"toolbar-close";
							 onClicked: {
								 fbapathinput.text="";
							 }
						 }
					 }
					 Row{
						 width: parent.width;
						 anchors.horizontalCenter: parent.horizontalCenter;
						 height: childrenRect.height;
						 Button{
							 width: parent.width / 2;
							 text: qsTr("Internal");
							 onClicked: {
								 fbacapex.fbaPath = "";
								 fbapathinput.text = fbacapex.fbaPath;
							 }
						 }
						 Button{
							 width: parent.width / 2;
							 text: qsTr("$PATH");
							 onClicked: {
								 fbacapex.fbaPath = "fba";
								 fbapathinput.text = fbacapex.fbaPath;
							 }
						 }
					 }
				 }
				 */
			 }
		 }
		 ScrollDecorator{
			 flickableItem: sysflick;
		 }
	 }
 }

 Item{
	 id: tool;
	 anchors.left: parent.left;
	 anchors.right: tabrow.right;
	 anchors.bottom: parent.bottom;
	 height: constants.sizeMedium;
	 clip: true;
	 Button{
		 text:qsTr("Back To Default");
		 anchors.centerIn:parent;
		 onClicked:{
			 fbacapex.resetSetting();
			 root.setValue();
		 }
	 }
 }
}
