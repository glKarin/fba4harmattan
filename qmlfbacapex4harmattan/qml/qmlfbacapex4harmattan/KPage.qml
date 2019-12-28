import com.nokia.meego 1.1

Page{
	id:root;

	objectName: "KZPage";
	orientationLock: fbacapex.orientation === 1 ? PageOrientation.LockPortrait : (fbacapex.orientation === 2 ? PageOrientation.LockLandscape : PageOrientation.Automatic);

}
