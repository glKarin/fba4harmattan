#include "kzfbacapex.h"
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QSettings>

const QString FBAPath="/usr/bin/fba";
const QString RomPath="/home/user/MyDocs/roms/";

	KZFbaCapex::KZFbaCapex(QObject *object)
:QObject(object)
{
	cpp_running = false;
	readSetting();
	getRomlist();
	connect(&execcmd,SIGNAL(readyReadStandardOutput()),this,SLOT(readStdout()));
	connect(&execcmd,SIGNAL(readyReadStandardError()),this,SLOT(readStderr()));
	connect(&execcmd,SIGNAL(started()),this,SLOT(setRunning2True()));
	connect(&execcmd,SIGNAL(finished(int, QProcess::ExitStatus)),this,SLOT(setRunning2False(int, QProcess::ExitStatus)));
}

KZFbaCapex::~KZFbaCapex()
{
	clearRomlist();
	writeSetting();
	if(execcmd.state() != QProcess::NotRunning)
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

/*
	 bool KZFbaCapex::showfps() const
	 {
	 return cpp_showfps;
	 }
	 */

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
#ifdef MYDEBUG
	qDebug()<<"set clock "<<cpp_clock;
#endif
		emit clockChanged();
	}
}

void KZFbaCapex::setSense(int s)
{
	if(cpp_sense != s && s >= 10 && s <= 100)
	{
		cpp_sense = s;
#ifdef MYDEBUG
	qDebug()<<"set sense "<<cpp_sense;
#endif
		emit senseChanged();
	}
}

void KZFbaCapex::setSound(bool b)
{
	if(cpp_sound != b)
	{
		cpp_sound = b;
#ifdef MYDEBUG
	qDebug()<<"set sound "<<cpp_sound;
#endif
		emit soundChanged();
	}
}

/*
	 void KZFbaCapex::setShowfps(bool b)
	 {
	 if(cpp_showfps != b)
	 {
	 cpp_showfps = b;
	 emit showfpsChanged();
	 }
	 }
	 */

void KZFbaCapex::setTweak(int t)
{
	if(cpp_tweak != t && t >= 0 && t <= 2)
	{
		cpp_tweak = t;
#ifdef MYDEBUG
	qDebug()<<"set tweak "<<cpp_tweak;
#endif
		emit tweakChanged();
	}
}

void KZFbaCapex::setSamplerate(int s)
{
	if(cpp_samplerate != s && (s == 11025 || s == 22050 || s == 44100))
	{
		cpp_samplerate = s;
#ifdef MYDEBUG
	qDebug()<<"set samplerate "<<cpp_samplerate;
#endif
		emit samplerateChanged();
	}
}

void KZFbaCapex::setZ80core(int z)
{
	if(cpp_z80core != z && z >= 0 && z <= 1)
	{
		cpp_z80core = z;
#ifdef MYDEBUG
	qDebug()<<"set z80core "<<cpp_z80core;
#endif
		emit z80coreChanged();
	}
}

void KZFbaCapex::setFilter(int f)
{
	if(cpp_filter != f && f >= 0 && f <= 1)
	{
		cpp_filter = f;
#ifdef MYDEBUG
	qDebug()<<"set filter "<<cpp_filter;
#endif
		emit filterChanged();
	}
}

void KZFbaCapex::readSetting()
{
	QSettings setting(QSettings::NativeFormat,QSettings::UserScope,"Karin_Zhao","qmlfbacapex",this);
	if(!QFile(setting.fileName()).exists())
	{
#ifdef MYDEBUG
	qDebug()<<"first run, the setting not found";
#endif
		resetSetting();
		return;
	}
	cpp_clock = setting.value("fba2x_clock").toInt();
	cpp_sound = setting.value("fba2x_sound").toBool();
	cpp_samplerate = setting.value("fba2x_samplerate").toInt();
	cpp_z80core = setting.value("fba2x_z80_core").toInt();
	cpp_tweak = setting.value("fba2x_tweak").toInt();
	cpp_rotate = 0;//do not need read
	cpp_scaling = 3;//do not need read
	cpp_sense = setting.value("fba2x_sensitivity").toInt();
	cpp_filter = setting.value("fba2x_filter").toInt();
	//cpp_showfps = setting.value("showfps").toBool();
#ifdef MYDEBUG
	qDebug()<<"read setting succesful";
#endif
	emit settingChanged();
}

void KZFbaCapex::writeSetting()
{
	QSettings setting(QSettings::NativeFormat,QSettings::UserScope,"Karin_Zhao","qmlfbacapex",this);
	setting.setValue("fba2x_clock",cpp_clock);
	setting.setValue("fba2x_sound",cpp_sound);
	setting.setValue("fba2x_samplerate",cpp_samplerate);
	setting.setValue("fba2x_tweak",cpp_tweak);
	setting.setValue("fba2x_z80_core",cpp_z80core);
	setting.setValue("fba2x_filter",cpp_filter);
	setting.setValue("fba2x_sensitivity",cpp_sense);
	//setting.setValue("showfps",cpp_showfps);
#ifdef MYDEBUG
	qDebug()<<"write setting succesful";
#endif
}

void KZFbaCapex::execFbaRunCommand(const QString &file)
{
	if(execcmd.state() != QProcess::NotRunning)
		return;
	QStringList list;
	QString sound_arg;
	if(cpp_sound)
		sound_arg = "--sound-sdl";
	else
		sound_arg = "--no-sound";
	list<<"--clock=" + QString::number(cpp_clock)
		<<"--sense=" + QString::number(cpp_sense)
		<<sound_arg
		<<"--samplerate=" + QString::number(cpp_samplerate)
		<<"--scaling=" + QString::number(cpp_scaling)
		<<"--rotate=" + QString::number(cpp_rotate)
		<<"--z80core=" + QString::number(cpp_z80core)
		<<"--filter=" + QString::number(cpp_filter);
	/*
		 if(cpp_showfps)
		 list<<"--showfps";
		 */
	if(cpp_tweak == 1)
		list<<"--force-m68k";
	else if(cpp_tweak == 2)
		list<<"--force-c68k";
	list<<file;
	//execcmd.setWorkingDirectory(file.left(file.lastIndexOf('/')+1));
	execcmd.start(FBAPath,list);
	cpp_stdoutandstderr.clear();
	emit stdoutandstderrChanged();
	cpp_stdoutandstderr = "command -> " + FBAPath + "  " + list.join(" ") + "\n\n";
	emit stdoutandstderrChanged();
#ifdef MYDEBUG
	qDebug()<<"conmand -> "<<cpp_stdoutandstderr;
#endif
}

void KZFbaCapex::resetSetting()
{
	cpp_clock = 600;
	cpp_sense = 100;
	cpp_sound = true;
	cpp_samplerate = 11025;
	cpp_scaling = 3;
	cpp_tweak = 0;
	cpp_rotate = 0;
	cpp_z80core = 0;
	cpp_filter = 0;
	//cpp_showfps = false;
#ifdef MYDEBUG
	qDebug()<<"reset setting";
#endif
	emit settingChanged();
}

int KZFbaCapex::getRomlist()
{
	int count = 0;
	QDir dir(RomPath);
	if(!dir.exists())
		return -1;
    QFile zipname("/opt/qmlfbacapex4harmattan/doc/zipname.fba");
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
	cpp_romlist = romlist_all;
#ifdef MYDEBUG
	qDebug()<<"read all fba rom : "<<count;
#endif
	emit romlistChanged();
	return count;
}

void KZFbaCapex::refrushRomlist(int flag)
{
	romlist_available.clear();
	romlist_missing.clear();
	for(QList<QObject *>::iterator itor = romlist_all.begin();
			itor != romlist_all.end();
			itor++)
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
#ifdef MYDEBUG
	qDebug()<<"update all fba rom";
#endif
	}
	else if(flag == 1)
	{
		cpp_romlist = romlist_available;
#ifdef MYDEBUG
	qDebug()<<"update available fba rom";
#endif
	}
	else if(flag == 2)
	{
		cpp_romlist = romlist_missing;
#ifdef MYDEBUG
	qDebug()<<"update missing fba rom";
#endif
	}
	emit romlistChanged();
}

void KZFbaCapex::filterRomlist(int flag)
{
	static int last;
	if(last == flag)
	{
#ifdef MYDEBUG
	qDebug()<<"don't need to filter";
#endif
		return;
	}
	cpp_romlist.clear();
	if(flag == 0)
	{
		cpp_romlist = romlist_all;
#ifdef MYDEBUG
	qDebug()<<"filter all fba rom";
#endif
	}
	else if(flag == 1)
	{
		cpp_romlist = romlist_available;
#ifdef MYDEBUG
	qDebug()<<"filter available fba rom";
#endif
	}
	else if(flag == 2)
	{
		cpp_romlist = romlist_missing;
#ifdef MYDEBUG
	qDebug()<<"filter missing fba rom";
#endif
	}
	emit romlistChanged();
	last = flag;
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
				itor++)
			delete *itor;
	}
#ifdef MYDEBUG
	qDebug()<<"delete all fba rom list";
#endif
}

void KZFbaCapex::killFbaEmu()
{
	if(execcmd.state() != QProcess::NotRunning)
	{
		execcmd.kill();
		while(!execcmd.waitForFinished());
#ifdef MYDEBUG
	qDebug()<<"process has killed";
#endif
	}
}

void KZFbaCapex::readStdout()
{
	execcmd.setReadChannel(QProcess::StandardOutput);
	cpp_stdoutandstderr = "stdout -> " + execcmd.readAllStandardOutput();
	emit stdoutandstderrChanged();
}

void KZFbaCapex::readStderr()
{
	execcmd.setReadChannel(QProcess::StandardError);
	cpp_stdoutandstderr = "stderr -> " + execcmd.readAllStandardError();
	emit stdoutandstderrChanged();
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
#ifdef MYDEBUG
	qDebug()<<"process started";
#endif
	cpp_running = true;
	emit runningChanged();
}
void KZFbaCapex::setRunning2False(int exitCode, QProcess::ExitStatus exitStatus)
{
	Q_UNUSED(exitStatus);
#ifdef MYDEBUG
	qDebug()<<"process finished";
#endif
	cpp_running = false;
	emit runningChanged();
	cpp_stdoutandstderr = "\nexit code -> " + QString::number(exitCode) + "\n";
	emit stdoutandstderrChanged();
}
