#ifndef KZ_QML_FBA_CAPEX_H
#define KZ_QML_FBA_CAPEX_H

#include <QObject>
#include <QList>
#include <QProcess>

#include "kzfbarominfo.h"

class QSettings;

class KZFbaCapex : public QObject
{
	Q_OBJECT
		Q_PROPERTY(int clock READ clock WRITE setClock NOTIFY clockChanged)
		Q_PROPERTY(int sense READ sense WRITE setSense NOTIFY senseChanged)
		Q_PROPERTY(bool sound READ sound WRITE setSound NOTIFY soundChanged)
		Q_PROPERTY(int samplerate READ samplerate WRITE setSamplerate NOTIFY samplerateChanged)
		Q_PROPERTY(int scaling READ scaling CONSTANT FINAL) //WRITE NOTIFY)
		Q_PROPERTY(int rotate READ rotate CONSTANT FINAL) //WRITE NOTIFY)
		Q_PROPERTY(int tweak READ tweak WRITE setTweak NOTIFY tweakChanged FINAL)
		Q_PROPERTY(int z80core READ z80core WRITE setZ80core NOTIFY z80coreChanged)
		Q_PROPERTY(int filter READ filter WRITE setFilter NOTIFY filterChanged)
		Q_PROPERTY(bool showfps READ showfps WRITE setShowfps NOTIFY showfpsChanged)
		Q_PROPERTY(QList<QObject *> romlist READ romlist NOTIFY romlistChanged FINAL)
		//Q_PROPERTY(QmlFbaCapexState state READ state NOTIFY stateChanged)
		Q_PROPERTY(QString stdoutandstderr READ stdoutandstderr NOTIFY stdoutandstderrChanged FINAL)
		//Q_ENUMS(QmlFbaCapexState)
		Q_PROPERTY(bool running READ running NOTIFY runningChanged FINAL)

		Q_PROPERTY(int harmJoystickMode READ harmJoystickMode WRITE setHarmJoystickMode NOTIFY harmJoystickModeChanged)
		Q_PROPERTY(int harmButtonLayout READ harmButtonLayout WRITE setHarmButtonLayout NOTIFY harmButtonLayoutChanged)
		Q_PROPERTY(QString harmBIOS READ harmBIOS WRITE setHarmBIOS NOTIFY harmBIOSChanged)
		Q_PROPERTY(bool harmShow2PController READ harmShow2PController WRITE setHarmShow2PController NOTIFY harmShow2PControllerChanged)
		Q_PROPERTY(bool harmEnableBIOS READ harmEnableBIOS WRITE setHarmEnableBIOS NOTIFY harmEnableBIOSChanged)

		Q_PROPERTY(QString appLastPath READ appLastPath WRITE setAppLastPath NOTIFY appLastPathChanged)
		Q_PROPERTY(int orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)
		Q_PROPERTY(QString fbaPath READ fbaPath WRITE setFbaPath NOTIFY fbaPathChanged)
		Q_PROPERTY(int runMode READ runMode WRITE setRunMode NOTIFY runModeChanged)

	public:
		KZFbaCapex(QObject *object = 0);
		~KZFbaCapex();
		int clock() const;
		int sense() const;
		bool sound() const;
		int samplerate() const;
		int scaling() const;
		int rotate() const;
		int z80core() const;
		int tweak() const;
		int filter() const;
		bool running() const;
		bool showfps() const;
		const QList<QObject *> & romlist() const;
		//QmlFbaCapexState state() const;
		const QString & stdoutandstderr() const;
		void setClock(int c);
		void setSense(int s);
		void setSound(bool b);
		void setShowfps(bool b);
		void setSamplerate(int s);
		void setZ80core(int z);
		void setFilter(int f);
		void setTweak(int t);

		int harmJoystickMode() const;
		void setHarmJoystickMode(int i);
		int harmButtonLayout() const;
		void setHarmButtonLayout(int i);
		QString harmBIOS() const;
		void setHarmBIOS(const QString &bios);
		bool harmShow2PController() const;
		void setHarmShow2PController(bool b);
		bool harmEnableBIOS() const;
		void setHarmEnableBIOS(bool b);

		QString appLastPath() const;
		void setAppLastPath(const QString &path);
		QString fbaPath() const;
		void setFbaPath(const QString &path);
		int orientation() const;
		void setOrientation(int i);
		int runMode() const;
		void setRunMode(int i);

		Q_INVOKABLE int getRomlist();
		Q_INVOKABLE void execFbaRunCommand(const QString &file);
		Q_INVOKABLE void resetSetting();
		Q_INVOKABLE void killFbaEmu();
		Q_INVOKABLE void filterRomlist(int flag = -1);
		Q_INVOKABLE void refrushRomlist(int flag = 0);
		Q_INVOKABLE int findRomList(const QString &text = QString());
		Q_INVOKABLE int filterFbaRomName(const QString &text = QString());


	private:
		int last;
		int cpp_clock;
		int cpp_sense;
		bool cpp_sound;
		int cpp_samplerate;
		int cpp_scaling;
		int cpp_rotate;
		int cpp_z80core;
		int cpp_filter;
		int cpp_tweak;
		bool cpp_running;
		bool cpp_showfps;
		QList<QObject *> cpp_romlist;
		QList<QObject *> romlist_all;
		QList<QObject *> romlist_missing;
		QList<QObject *> romlist_available;
		QProcess *execcmd;
		//QmlFbaCapexState cpp_state;
		QString cpp_stdoutandstderr;
		QSettings *setting;

		int cpp_harmJoystickMode;
		int cpp_harmButtonLayout;
		QString cpp_harmBIOS;
		bool cpp_harmShow2PController;
		bool cpp_harmEnableBIOS;

		QString cpp_appLastPath;
		QString cpp_fbaPath;
		int cpp_orientation;
		int cpp_runMode;

	private:
		void readSetting();
		void clearRomlist();

		void debug(const QString &msg);
		void setSetting(const QString &name, const QVariant &value);
		void setStdoutandstderr(const QString &text = QString(), int channel = 0);
		void setRunning(bool b);
		bool initProcess();

		private slots:
			void readStdout();
		void readStderr();
		void setRunning2True();
		void setRunning2False(int exitCode, QProcess::ExitStatus exitStatus);

signals:
		void clockChanged(int clock);
		void senseChanged(int sense);
		void soundChanged(bool sound);
		void tweakChanged(int tweak);
		void samplerateChanged(int samplerate);
		void z80coreChanged(int z80core);
		void filterChanged(int filter);
		void showfpsChanged(bool showfps);
		//void stateChanged();
		void romlistChanged();
		void stdoutandstderrChanged(const QString &stdoutandstderr);
		void runningChanged(bool running);

		void harmJoystickModeChanged(int harmJoystickMode);
		void harmButtonLayoutChanged(int harmButtonLayout);
		void harmBIOSChanged(const QString &harmBIOS);
		void harmShow2PControllerChanged(bool harmShow2PController);
		void harmEnableBIOSChanged(bool harmEnableBIOS);

		void appLastPathChanged(const QString &appLastPath);
		void fbaPathChanged(const QString &fbaPath);
		void orientationChanged(int orientation);
		void runModeChanged(int runMode);

		void message(const QString &msg);
		void consoleLog(const QString &text, int channel);

};

#endif
