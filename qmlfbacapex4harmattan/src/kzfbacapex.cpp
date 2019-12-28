#include "kzfbacapex.h"
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QSettings>
#include <QApplication>

//#define MYDEBUG

extern QString adjust_path(const QString &path);

static const QString FBAPath="/usr/bin/fba";
static const QString RomPath="/home/user/MyDocs/roms/";

	KZFbaCapex::KZFbaCapex(QObject *object)
:QObject(object),
	last(0),
	cpp_clock(600),
	cpp_sense(100),
	cpp_sound(true),
	cpp_samplerate(11025),
	cpp_scaling(3),//do not need read
	cpp_rotate(0),//do not need read
	cpp_z80core(0),
	cpp_filter(0),
	cpp_tweak(0),
	cpp_running(false),
	cpp_showfps(false),
	execcmd(0),
	setting(new QSettings(this)),
	cpp_harmJoystickMode(0),
	cpp_harmButtonLayout(0),
	cpp_harmShow2PController(false),
	cpp_harmEnableBIOS(true),
	//cpp_fbaPath(adjust_path("fbalpha/fba")),
	cpp_fbaPath(FBAPath),
	cpp_orientation(0),
	cpp_runMode(0)
{
	setObjectName("KZFbaCapex");
	/*
	initSettingMap.insert("fba2x_clock", QVariant(600));
initSettingMap.insert("fba2x_sound", QVariant(true));
initSettingMap.insert("fba2x_samplerate", QVariant(11025));
initSettingMap.insert("fba2x_z80_core", QVariant(0));
initSettingMap.insert("fba2x_tweak", QVariant(0));
initSettingMap.insert("fba2x_sensitivity", QVariant(100));
initSettingMap.insert("fba2x_filter", QVariant(0));
*/
	readSetting();
	getRomlist();
}

KZFbaCapex::~KZFbaCapex()
{
	clearRomlist();
	if(execcmd && execcmd->state() != QProcess::NotRunning)
		killFbaEmu();
}

bool KZFbaCapex::running() const
{
	return cpp_running;
}

int KZFbaCapex::clock() const
{
	return cpp_clock;
}

int KZFbaCapex::tweak() const
{
	return cpp_tweak;
}

bool KZFbaCapex::showfps() const
{
	return cpp_showfps;
}

int KZFbaCapex::sense() const
{
	return cpp_sense;
}

bool KZFbaCapex::sound() const
{
	return cpp_samplerate;
}

int KZFbaCapex::scaling() const
{
	return cpp_scaling;
}

int KZFbaCapex::rotate() const
{
	return cpp_rotate;
}

int KZFbaCapex::z80core() const
{
	return cpp_z80core;
}

int KZFbaCapex::filter() const
{
	return cpp_filter;
}

const QList<QObject *> & KZFbaCapex::romlist() const
{
	return cpp_romlist;
}

void KZFbaCapex::setClock(int c)
{
	if(cpp_clock != c && c >= 200 && c <= 1200)
	{
		cpp_clock = c;
	setSetting("fba2x_clock", cpp_clock);
		emit clockChanged(cpp_clock);
	}
}

void KZFbaCapex::setSense(int s)
{
	if(cpp_sense != s && s >= 10 && s <= 100)
	{
		cpp_sense = s;
	setSetting("fba2x_sensitivity", cpp_sense);
		emit senseChanged(cpp_sense);
	}
}

void KZFbaCapex::setSound(bool b)
{
	if(cpp_sound != b)
	{
		cpp_sound = b;
	setSetting("fba2x_sound", cpp_sound);
		emit soundChanged(cpp_sound);
	}
}

void KZFbaCapex::setShowfps(bool b)
{
	if(cpp_showfps != b)
	{
		cpp_showfps = b;
		setSetting("showfps", cpp_showfps);
		emit showfpsChanged(cpp_showfps);
	}
}

void KZFbaCapex::setTweak(int t)
{
	if(cpp_tweak != t && t >= 0 && t <= 2)
	{
		cpp_tweak = t;
	setSetting("fba2x_tweak", cpp_tweak);
		emit tweakChanged(cpp_tweak);
	}
}

void KZFbaCapex::setSamplerate(int s)
{
	if(cpp_samplerate != s && (s == 11025 || s == 22050 || s == 44100))
	{
		cpp_samplerate = s;
	setSetting("fba2x_samplerate", cpp_samplerate);
		emit samplerateChanged(cpp_samplerate);
	}
}

void KZFbaCapex::setZ80core(int z)
{
	if(cpp_z80core != z && z >= 0 && z <= 1)
	{
		cpp_z80core = z;
	setSetting("fba2x_z80_core", cpp_z80core);
		emit z80coreChanged(cpp_z80core);
	}
}

void KZFbaCapex::setFilter(int f)
{
	if(cpp_filter != f && f >= 0 && f <= 1)
	{
		cpp_filter = f;
	setSetting("fba2x_filter", cpp_filter);
		emit filterChanged(cpp_filter);
	}
}

void KZFbaCapex::readSetting()
{
	if(!QFileInfo(setting -> fileName()).isFile())
	{
		debug("first run, the setting not found");
		resetSetting();
		return;
	}
	setClock(setting -> value("fba2x_clock").toInt());
	setSound(setting -> value("fba2x_sound").toBool());
	setSamplerate(setting -> value("fba2x_samplerate").toInt());
	setZ80core(setting -> value("fba2x_z80_core").toInt());
	setTweak(setting -> value("fba2x_tweak").toInt());
	setSense(setting -> value("fba2x_sensitivity").toInt());
	setFilter(setting -> value("fba2x_filter").toInt());
	setShowfps(setting->value("showfps").toBool());

	setHarmJoystickMode(setting->value("harm_joystick_mode").value<int>());
	setHarmButtonLayout(setting->value("harm_button_layout").value<int>());
	setHarmBIOS(setting->value("harm_bios").value<QString>());
	setHarmShow2PController(setting->value("harm_show_2p").value<bool>());
	setHarmEnableBIOS(setting->value("harm_enable_bios").value<bool>());

	setAppLastPath(setting->value("sys/app_last_path").value<QString>());
	setFbaPath(setting->value("sys/fba_path").value<QString>());
	setOrientation(setting->value("sys/orientation").value<int>());
	setRunMode(setting->value("sys/run_mode").value<int>());

	debug("read setting succesful");
}

void KZFbaCapex::execFbaRunCommand(const QString &file)
{
	if((cpp_runMode != 1 && cpp_runMode != 2) && (execcmd && execcmd->state() != QProcess::NotRunning))
		return;
	QStringList list;
	list<<"--clock=" + QString::number(cpp_clock)
		<<"--sense=" + QString::number(cpp_sense)
		<<"--scaling=" + QString::number(cpp_scaling)
		<<"--rotate=" + QString::number(cpp_rotate)
		<<"--z80core=" + QString::number(cpp_z80core)
		<<"--filter=" + QString::number(cpp_filter);

	if(cpp_sound)
	{
		list << "--sound-sdl"
			<< "--samplerate=" + QString::number(cpp_samplerate);
	}
	else
		list << "--no-sound";

#if 1
		 if(cpp_showfps)
		 list<<"--showfps";
		 else
			 list<<"--no-showfps";
#endif
	if(cpp_tweak == 1)
		list<<"--force-m68k";
	else if(cpp_tweak == 2)
		list<<"--force-c68k";
	// else automitic

	// for harmattan FBAlpha
	if(cpp_harmButtonLayout != 0)
		list << "--harm-layout=" + QString::number(cpp_harmButtonLayout);
	if(cpp_harmJoystickMode != 0)
		list << "--harm-joystick-mode=" + QString::number(cpp_harmJoystickMode);
	if(cpp_harmShow2PController)
		list << "--harm-show-2p";
	if(cpp_harmEnableBIOS)
	{
		if(!cpp_harmBIOS.isEmpty())
			list << "--harm-BIOS=" + cpp_harmBIOS;
	}

	if(cpp_runMode == 1)
		list << "--frontend=" + QApplication::applicationFilePath();

	list<<file;
	qDebug() << (cpp_fbaPath + " " + list.join(" "));

	if(cpp_runMode == 1 || cpp_runMode == 2)
	{
		QProcess::startDetached(cpp_fbaPath, list);
		qApp->quit();
	}
	else
	{
		if(initProcess())
		{
			//execcmd->setWorkingDirectory(file.left(file.lastIndexOf('/')+1));
			setStdoutandstderr();
			setStdoutandstderr("command -> " + cpp_fbaPath + "  " + list.join(" ") + "\n\n", 1);
			execcmd->start(cpp_fbaPath,list);
		}
		else
		{
			QProcess::startDetached(cpp_fbaPath, list);
			qApp->quit();
		}
	}
}

void KZFbaCapex::resetSetting()
{
	setClock(600);
	setSense(100);
	setSound(true);
	setSamplerate(11025);
	setTweak(0);
	setZ80core(0);
	setFilter(0);
	setShowfps(false);
	
	setHarmJoystickMode(0);
	setHarmButtonLayout(0);
	setHarmBIOS("");
	setHarmShow2PController(false);
	setHarmEnableBIOS(true);

	setAppLastPath(QDir::homePath());
	setFbaPath(QString());
	setOrientation(0);
	setRunMode(0);
	debug("reset setting");
}

int KZFbaCapex::getRomlist()
{
	int count = 0;
	QDir dir(RomPath);
	if(!dir.exists())
		return -1;
    QFile zipname(adjust_path("doc/zipname.fba"));
	if(!zipname.exists())
		return -1;
	if(!zipname.open(QIODevice::ReadOnly | QIODevice::Text))
		return -1;
	QTextStream stream(&zipname);
	while(!stream.atEnd())
	{
		QString s = stream.readLine();
		QStringList list = s.split(',');
		KZFbaRomInfo *info = new KZFbaRomInfo;
		info -> setName(list.at(1));
		int i = list.at(2).indexOf(' ');
		info -> setYear(list.at(2).left(i+1).toInt());
		info -> setCom(list.at(2).right(list.at(2).length() - i - 1));
		info -> setPath(RomPath + list.at(0) + ".zip");
		if(dir.exists(list.at(0) + ".zip"))
		{
			info -> setExists(true);
			romlist_available.push_back(info);
		}
		else
		{
			info -> setExists(false);
			romlist_missing.push_back(info);
		}
		count++;
		romlist_all.push_back(info);
	}
	zipname.close();
	cpp_romlist = romlist_all;
	debug("read all fba rom : " + QString::number(count));
	emit romlistChanged();
	return count;
}

void KZFbaCapex::refrushRomlist(int flag)
{
	romlist_available.clear();
	romlist_missing.clear();
	for(QList<QObject *>::iterator itor = romlist_all.begin();
			itor != romlist_all.end();
			++itor)
	{
		if(QFile::exists(static_cast<KZFbaRomInfo *>(*itor) -> path()))
		{
			static_cast<KZFbaRomInfo *>(*itor) -> setExists(true);
			romlist_available.push_back(*itor);
		}
		else
		{
			static_cast<KZFbaRomInfo *>(*itor) -> setExists(false);
			romlist_missing.push_back(*itor);
		}
	}
	cpp_romlist.clear();
	if(flag == 0)
	{
		cpp_romlist = romlist_all;
	debug("update all fba rom");
	}
	else if(flag == 1)
	{
		cpp_romlist = romlist_available;
	debug("update available fba rom");
	}
	else if(flag == 2)
	{
		cpp_romlist = romlist_missing;
	debug("update missing fba rom");
	}
	emit romlistChanged();
}

void KZFbaCapex::filterRomlist(int flag)
{
	if(last == flag && flag != -1)
	{
	debug("don't need to filter");
		return;
	}
	cpp_romlist.clear();
	if(flag != -1)
		last = flag;
	if(last == 0)
	{
		cpp_romlist = romlist_all;
	debug("filter all fba rom");
	}
	else if(last == 1)
	{
		cpp_romlist = romlist_available;
	debug("filter available fba rom");
	}
	else if(last == 2)
	{
		cpp_romlist = romlist_missing;
	debug("filter missing fba rom");
	}
	emit romlistChanged();
}
/*
	 KZFbaCapex::QmlFbaCapexState KZFbaCapex::state() const
//{
return cpp_state;
}

void KZFbaCapex::setState(KZFbaCapex::QmlFbaCapexState state)
{
cpp_state = state;
emit stateChanged();
}
*/

void KZFbaCapex::clearRomlist()
{
	if(!romlist_all.isEmpty())
	{
		for(QList<QObject *>::iterator itor = romlist_all.begin();
				itor != romlist_all.end();
				++itor)
			delete *itor;
	}
	debug("delete all fba rom list");
}

void KZFbaCapex::killFbaEmu()
{
	if(!execcmd)
		return;

	if(execcmd->state() != QProcess::NotRunning)
	{
		execcmd->kill();
		while(!execcmd->waitForFinished());
	debug("process has killed");
	}
}

void KZFbaCapex::readStdout()
{
	if(!execcmd)
		return;

	//execcmd->setReadChannel(QProcess::StandardOutput);
	setStdoutandstderr(execcmd->readAllStandardOutput(), 1);
}

void KZFbaCapex::readStderr()
{
	if(!execcmd)
		return;

	//execcmd->setReadChannel(QProcess::StandardError);
	setStdoutandstderr(execcmd->readAllStandardError(), 2);
}

const QString & KZFbaCapex::stdoutandstderr() const
{
	return cpp_stdoutandstderr;
}

int KZFbaCapex::samplerate() const
{
	return cpp_samplerate;
}

void KZFbaCapex::setRunning2True()
{
	debug("process started");
	setRunning(true);
}
void KZFbaCapex::setRunning2False(int exitCode, QProcess::ExitStatus exitStatus)
{
	Q_UNUSED(exitStatus);
	debug("process finished");
	setRunning(false);
	setStdoutandstderr("\nexit code -> " + QString::number(exitCode) + "\n", exitCode == 0 ? 1 : 2);
}

int KZFbaCapex::harmJoystickMode() const
{
	return cpp_harmJoystickMode;
}

void KZFbaCapex::setHarmJoystickMode(int i)
{
	if(cpp_harmJoystickMode != i)
	{
		cpp_harmJoystickMode = i;
		setSetting("harm_joystick_mode", cpp_harmJoystickMode);
		emit harmJoystickModeChanged(cpp_harmJoystickMode);
	}
}

bool KZFbaCapex::harmShow2PController() const
{
	return cpp_harmShow2PController;
}

void KZFbaCapex::setHarmShow2PController(bool b)
{
	if(cpp_harmShow2PController != b)
	{
		cpp_harmShow2PController = b;
		setSetting("harm_show_2p", cpp_harmShow2PController);
		emit harmShow2PControllerChanged(cpp_harmShow2PController);
	}
}

int KZFbaCapex::harmButtonLayout() const
{
	return cpp_harmButtonLayout;
}

void KZFbaCapex::setHarmButtonLayout(int i)
{
	if(cpp_harmButtonLayout != i)
	{
		cpp_harmButtonLayout = i;
		setSetting("harm_button_layout", cpp_harmButtonLayout);
		emit harmButtonLayoutChanged(cpp_harmButtonLayout);
	}
}

QString KZFbaCapex::harmBIOS() const
{
	return cpp_harmBIOS;
}

void KZFbaCapex::setHarmBIOS(const QString &bios)
{
	if(cpp_harmBIOS != bios)
	{
		cpp_harmBIOS = bios;
		setSetting("harm_bios", cpp_harmBIOS);
		emit harmBIOSChanged(cpp_harmBIOS);
	}
}

QString KZFbaCapex::appLastPath() const
{
	return cpp_appLastPath;
}

void KZFbaCapex::setAppLastPath(const QString &path)
{
	if(cpp_appLastPath != path)
	{
		cpp_appLastPath = path;
		setSetting("sys/app_last_path", cpp_appLastPath);
		emit appLastPathChanged(cpp_appLastPath);
	}
}

void KZFbaCapex::debug(const QString &msg)
{
	if(msg.isEmpty())
		return;

	qDebug() << "[KZQDebug]: " << msg;
#ifdef MYDEBUG
	emit message(msg);
#endif
}

void KZFbaCapex::setSetting(const QString &name, const QVariant &value)
{
	setting->setValue(name, value);
	debug("Set " + name + " -> " + value.toString());
}

void KZFbaCapex::setStdoutandstderr(const QString &text, int channel)
{
	if(!text.isEmpty() && channel != 0)
	{
		cpp_stdoutandstderr += text;
		if(channel == 1)
			qDebug() << text;
		else
			qWarning() << text;
	}
	else
		cpp_stdoutandstderr.clear();
	emit stdoutandstderrChanged(cpp_stdoutandstderr);
	emit consoleLog(text, channel);
}

void KZFbaCapex::setRunning(bool b)
{
	cpp_running = b;
	emit runningChanged(cpp_running);
}

int KZFbaCapex::findRomList(const QString &text)
{
	if(text.isEmpty())
	{
		filterRomlist();
		return cpp_romlist.size();
	}

	QList<QObject *> newList;
	const QList<QObject *> &list = last == 1 ? romlist_available : (last == 2 ? romlist_missing : romlist_all);
	for(QList<QObject *>::const_iterator itor = list.constBegin();
			itor != list.constEnd();
			++itor)
	{
		if((static_cast<KZFbaRomInfo *>(*itor))->name().contains(text, Qt::CaseInsensitive))
			newList.push_back(*itor);
	}

	if(!newList.isEmpty()) // if not result, keep last result
	{
		cpp_romlist = newList;
		emit romlistChanged();
		return cpp_romlist.size();
	}
	else
		return 0;
}

int KZFbaCapex::filterFbaRomName(const QString &text)
{
	int r;

	r = 0;
	if(text.isEmpty())
	{
		filterRomlist();
		return r;
	}

	for(QList<QObject *>::const_iterator itor = cpp_romlist.constBegin();
			itor != cpp_romlist.constEnd();
			++itor)
	{
		if((static_cast<KZFbaRomInfo *>(*itor))->name().startsWith(text, Qt::CaseInsensitive))
			return r;
		r++;
	}

	return -1;
}

QString KZFbaCapex::fbaPath() const
{
	return cpp_fbaPath;
}

void KZFbaCapex::setFbaPath(const QString &value)
{
	return;
	QString path(value.isEmpty() ? adjust_path("fbalpha/fba") : value);
	if(cpp_fbaPath != path)
	{
		cpp_fbaPath = path;
		setSetting("sys/fba_path", cpp_fbaPath);
		emit fbaPathChanged(cpp_fbaPath);
	}
}

int KZFbaCapex::orientation() const
{
	return cpp_orientation;
}

void KZFbaCapex::setOrientation(int i)
{
	if(cpp_orientation != i)
	{
		cpp_orientation = i;
		setSetting("sys/orientation", cpp_orientation);
		emit orientationChanged(cpp_orientation);
	}
}

bool KZFbaCapex::harmEnableBIOS() const
{
	return cpp_harmEnableBIOS;
}

void KZFbaCapex::setHarmEnableBIOS(bool b)
{
	if(cpp_harmEnableBIOS != b)
	{
		cpp_harmEnableBIOS = b;
		setSetting("harm_enable_bios", cpp_harmEnableBIOS);
		emit harmEnableBIOSChanged(cpp_harmEnableBIOS);
	}
}

bool KZFbaCapex::initProcess()
{
	if(execcmd)
		return true;

	execcmd = new QProcess(this);
	//Q_ASSERT(execcmd != 0);
	if(!execcmd)
		return false;

	connect(execcmd,SIGNAL(readyReadStandardOutput()),this,SLOT(readStdout()));
	connect(execcmd,SIGNAL(readyReadStandardError()),this,SLOT(readStderr()));
	connect(execcmd,SIGNAL(started()),this,SLOT(setRunning2True()));
	connect(execcmd,SIGNAL(finished(int, QProcess::ExitStatus)),this,SLOT(setRunning2False(int, QProcess::ExitStatus)));

	return true;
}

int KZFbaCapex::runMode() const
{
	return cpp_runMode;
}

void KZFbaCapex::setRunMode(int i)
{
	if(cpp_runMode != i)
	{
		cpp_runMode = i;
		setSetting("sys/run_mode", cpp_runMode);
		emit runModeChanged(cpp_runMode);
	}
}

