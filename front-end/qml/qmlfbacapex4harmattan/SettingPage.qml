import QtQuick 1.1
import com.nokia.meego 1.1

PageStack{
	id:root;
	signal backToDefault();
	property alias clock:clockslider.value;
	property alias sense:senseslider.value;
	property alias sound:soundswitch.checked;
	property alias samplerate:samplerateslider.value;
	property int scaling;
	property int rotate;
	property int tweak;
	property int z80core;
	property int filter;
	//property alias showfps:showfpsswitch.checked;

	function setValue(vclock,vsense,vsound,vsamplerate,vscaling,vrotate,vtweak,vz80core,vfilter)/*,vshowfps)*/
	{
		clockslider.value = vclock;
		senseslider.value = vsense;
		soundswitch.checked = vsound ? true :false;
		samplerateslider.value = vsamplerate;
		if(vscaling === 0)
			scalingbuttonrow.checkedButton = scalingnonebutton;
			else
		if(vscaling === 1)
		scalingbuttonrow.checkedButton = scaling2xbutton;
			else
		if(vscaling === 2)
		scalingbuttonrow.checkedButton = scalingbestfitbutton;
			else
		if(vscaling === 3)
			scalingbuttonrow.checkedButton = scalingfullbutton;
		if(vrotate === 0)
			rotatebuttonrow.checkedButton = rotateautobutton;
			else
		if(vrotate === 1)
			rotatebuttonrow.checkedButton = rotatehorizontalbutton;
			else
		if(vrotate === 2)
			rotatebuttonrow.checkedButton = rotateverticalbutton;
		if(vtweak === 0)
			tweakbuttonrow.checkedButton = tweakautobutton;
			else
		if(vtweak === 1)
			tweakbuttonrow.checkedButton = tweakforcem68kbutton;
			else
		if(vtweak === 2)
			tweakbuttonrow.checkedButton = tweakforcecyclonebutton;
		if(vz80core === 0)
			z80emucorebuttonrow.checkedButton = z80emucoredrz80button;
			else
		if(vz80core === 1)
			z80emucorebuttonrow.checkedButton = z80emucorecz80button;
		if(vfilter === 0)
			filterbuttonrow.checkedButton = filterdefaultbutton;
			else
		if(vfilter === 1)
			filterbuttonrow.checkedButton = filternonebutton;
		//showfpsswitch.checked = vshowfps ? true :false;
	}

	/*
	HeadView{
		id:head;
		title:qsTr("Fba2x Settings");
	}
	*/
	
	Flickable{
		id:flick;
		contentWidth:parent.width;
		contentHeight:9 * 100;
		anchors.fill:parent;
		//anchors.topMargin:head.height;
		SettingItem{
			id:clockset;
			anchors.top:parent.top;
			Column{
				anchors.fill:parent;
				Text{
                    text:qsTr("FBA2X Clock : ") + clockslider.value + "MHZ";
					font.pointSize:24;
					width:parent.width;
					height:40;
				}
				Slider{
					id:clockslider;
					width:parent.width;
					minimumValue:200;
					maximumValue:1200;
					stepSize:1;
				}
			}
		}
		SettingItem{
			id:senseset;
			anchors.top:clockset.bottom;
			Column{
				anchors.fill:parent;
				Text{
                    text:qsTr("Analogue Sensitivity : ") + senseslider.value + "%";
					font.pointSize:24;
					width:parent.width;
					height:40;
				}
				Slider{
					id:senseslider;
					width:parent.width;
					minimumValue:10;
					maximumValue:100;
					stepSize:1;
				}
			}
		}
		SettingItem{
			id:soundset;
			anchors.top:senseset.bottom;
			Text{
				text:qsTr("Sound : ") + ((soundswitch.checked) ? qsTr("Enable") : qsTr("Disable"));
				font.pointSize:24;
				height:40;
				anchors.verticalCenter:parent.verticalCenter;
				anchors.left:parent.left;
			}
			Switch{
				id:soundswitch;
				anchors.right:parent.right;
				anchors.verticalCenter:parent.verticalCenter;
			}
		}
		SettingItem{
			id:samplerateset;
			anchors.top:soundset.bottom;
			Column{
				anchors.fill:parent;
				Text{
					text:qsTr("Audio Sample Rate : ") + samplerateslider.value + "HZ";
					font.pointSize:24;
					width:parent.width;
					height:40;
				}
				Slider{
					id:samplerateslider;
					width:parent.width;
					minimumValue:11025;
					maximumValue:44100;
					stepSize:11025;
				}
			}
		}
		SettingItem{
			id:scalingset;
			anchors.top:samplerateset.bottom;
			Column{
				anchors.fill:parent;
				Text{
					text:qsTr("Scaling : ") + scalingbuttonrow.checkedButton.text;
					font.pointSize:24;
					width:parent.width;
					height:40;
				}
				ButtonRow{
					id:scalingbuttonrow;
					Button{
						id:scalingnonebutton;
						text:qsTr("None");
                        enabled:false;
						onClicked:{
							root.scaling = 0;
						}
					}
					Button{
						id:scaling2xbutton;
						text:qsTr("2X");
                        enabled:false;
						onClicked:{
							root.scaling = 1;
						}
					}
					Button{
						id:scalingbestfitbutton;
						text:qsTr("Best Fit");
                        enabled:false;
						onClicked:{
							root.scaling = 2;
						}
					}
					Button{
						id:scalingfullbutton;
						text:qsTr("Full");
                        enabled:false;
						onClicked:{
							root.scaling = 3;
						}
					}
				}
			}
		}
		SettingItem{
			id:rotateset;
			anchors.top:scalingset.bottom;
			Column{
				anchors.fill:parent;
				Text{
					text:qsTr("Rotate : ") + rotatebuttonrow.checkedButton.text;
					font.pointSize:24;
					width:parent.width;
					height:40;
				}
				ButtonRow{
					id:rotatebuttonrow;
					Button{
						id:rotateautobutton;
						text:qsTr("Auto");
                        enabled:false;
						onClicked:{
							root.rotate = 0;
						}
					}
					Button{
						id:rotatehorizontalbutton;
						text:qsTr("Horizontal");
                        enabled:false;
						onClicked:{
							root.rotate = 1;
						}
					}
					Button{
						id:rotateverticalbutton;
						text:qsTr("Vertical");
                        enabled:false;
						onClicked:{
							root.rotate = 2;
						}
					}
				}
			}
		}
		SettingItem{
			id:tweakset;
			anchors.top:rotateset.bottom;
			Column{
				anchors.fill:parent;
				Text{
					text:qsTr("68K Emu Core : ") + tweakbuttonrow.checkedButton.text;
					font.pointSize:24;
					width:parent.width;
					height:40;
				}
				ButtonRow{
					id:tweakbuttonrow;
					Button{
						id:tweakautobutton;
						text:qsTr("Auto");
						onClicked:{
							root.tweak = 0;
						}
					}
					Button{
						id:tweakforcem68kbutton;
						text:qsTr("Force M68K");
						onClicked:{
							root.tweak = 1;
						}
					}
					Button{
						id:tweakforcecyclonebutton;
						text:qsTr("Force Cyclone");
						onClicked:{
							root.tweak = 2;
						}
					}
				}
			}
		}
		SettingItem{
			id:z80emucoreset;
			anchors.top:rotateset.bottom;
			Column{
				anchors.fill:parent;
				Text{
					text:qsTr("Z80 Emu Core : ") + z80emucorebuttonrow.checkedButton.text;
					font.pointSize:24;
					width:parent.width;
					height:40;
				}
				ButtonRow{
					id:z80emucorebuttonrow;
					Button{
						id:z80emucoredrz80button;
						text:qsTr("DR Z80");
						onClicked:{
							root.z80core = 0;
						}
					}
					Button{
						id:z80emucorecz80button;
						text:qsTr("C Z80");
						onClicked:{
							root.z80core = 1;
						}
					}
				}
			}
		}
		SettingItem{
			id:filterset;
			anchors.top:z80emucoreset.bottom;
			Column{
				anchors.fill:parent;
				Text{
					text:qsTr("Filter : ") + filterbuttonrow.checkedButton.text;
					font.pointSize:24;
					width:parent.width;
					height:40;
				}
				ButtonRow{
					id:filterbuttonrow;
					Button{
						id:filterdefaultbutton;
						text:qsTr("Default");
						onClicked:{
							root.filter = 0;
						}
					}
					Button{
						id:filternonebutton;
						text:qsTr("None");
						onClicked:{
							root.filter = 1;
						}
					}
				}
			}
		}
		/*
		SettingItem{
			id:showfpsset;
			anchors.top:filterset.bottom;
			Text{
				text:qsTr("Show FPS : ") + ((showfpsswitch.checked) ? qsTr("Enable") : qsTr("Disable"));
				font.pointSize:24;
				height:40;
				anchors.verticalCenter:parent.verticalCenter;
				anchors.left:parent.left;
			}
			Switch{
				id:showfpsswitch;
				anchors.right:parent.right;
				anchors.verticalCenter:parent.verticalCenter;
			}
		}
		*/
		SettingItem{
			id:defaultbutton;
			//anchors.top:showfpsset.bottom;
			anchors.top:filterset.bottom;
			Button{
                text:qsTr("Back To Default");
				anchors.centerIn:parent;
				onClicked:{
					backToDefault();
				}
			}
		}
	}
}
