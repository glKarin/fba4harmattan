import QtQuick 1.1
import com.nokia.meego 1.1
import com.nokia.extras 1.1
import karin.fba.capex 1.2

PageStackWindow{
	id: app;
	objectName: "KZMainWindow";

	initialPage:MainPage{}

	InfoBanner{
		id: infobanner;
		topMargin: 36 + constants.spacingMedium;
		leftMargin: constants.spacingSmall;
	}

	KZFbaCapex{
		id:fbacapex;
		onMessage: {
			showMsg(msg);
		}
	}

	QtObject{
		id: constants;

		property int spacingMicro: 1;
		property int spacingTiny: 2;
		property int spacingSmall: 4;
		property int spacingMedium: 8;
		property int spacingLarge: 12;
		property int spacingBig: 18;
		property int spacingSuper: 24;

		property int fontMicro: 12;
		property int fontTiny: 18;
		property int fontSmall: 24;
		property int fontMedium: 28;
		property int fontLarge: 36;
		property int fontBig: 48;
		property int fontSuper: 64;

		property int sizeMicro: 24;
		property int sizeTiny: 36;
		property int sizeSmall: 48;
		property int sizeMedium: 64;
		property int sizeLarge: 80;
		property int sizeBig: 96;
		property int sizeSuper: 120;

		property int landscapeWidth: 360;
	}

	function showMsg(msg)
	{
		infobanner.text = msg;
		infobanner.show();
	}
}
