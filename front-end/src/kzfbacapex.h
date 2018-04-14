#ifndef KZ_QML_FBA_CAPEX_H
#define KZ_QML_FBA_CAPEX_H

#include <QObject>
#include <QList>
#include <QProcess>

#include "kzfbarominfo.h"

class KZFbaCapex : public QObject
{
	Q_OBJECT
		Q_PROPERTY(int clock READ clock WRITE setClock NOTIFY clockChanged)
		Q_PROPERTY(int sense READ sense WRITE setSense NOTIFY senseChanged)
		Q_PROPERTY(bool sound READ sound WRITE setSound NOTIFY soundChanged)
		Q_PROPERTY(int samplerate READ samplerate WRITE setSamplerate NOTIFY samplerateChanged)
		Q_PROPERTY(int scaling READ scaling CONSTANT) //WRITE NOTIFY)
		Q_PROPERTY(int rotate READ rotate CONSTANT) //WRITE NOTIFY)
		Q_PROPERTY(int tweak READ tweak WRITE setTweak NOTIFY tweakChanged)
		Q_PROPERTY(int z80core READ z80core WRITE setZ80core NOTIFY z80coreChanged)
		Q_PROPERTY(int filter READ filter WRITE setFilter NOTIFY filterChanged)
		//Q_PROPERTY(bool showfps READ showfps WRITE setShowfps NOTIFY showfpsChanged)
		Q_PROPERTY(QList<QObject *> romlist READ romlist NOTIFY romlistChanged)
		//Q_PROPERTY(QmlFbaCapexState state READ state NOTIFY stateChanged)
		Q_PROPERTY(QString stdoutandstderr READ stdoutandstderr NOTIFY stdoutandstderrChanged)
		//Q_ENUMS(QmlFbaCapexState)
		Q_PROPERTY(bool running READ running NOTIFY runningChanged)

	public:
		KZFbaCapex(QObject *object = 0);
		~KZFbaCapex();
		//enum QmlFbaCapexState{LoadSettingSuccess,LoadSettingFail};
		//void setState(QmlFbaCapexState state);

	private:
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
		//bool cpp_showfps;
		QList<QObject *> cpp_romlist;
		QList<QObject *> romlist_all;
		QList<QObject *> romlist_missing;
		QList<QObject *> romlist_available;
		QProcess execcmd;
		//QmlFbaCapexState cpp_state;
		QString cpp_stdoutandstderr;

		void readSetting();
		void clearRomlist();

		public slots:
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
		//bool showfps() const;
		const QList<QObject *> & romlist() const;
		//QmlFbaCapexState state() const;
		const QString & stdoutandstderr() const;

		void setClock(int c);
		void setSense(int s);
		void setSound(bool b);
		//void setShowfps(bool b);
		void setSamplerate(int s);
		void setZ80core(int z);
		void setFilter(int f);
		void setTweak(int t);

		int getRomlist();
		void execFbaRunCommand(const QString &file);
		void writeSetting();
		void resetSetting();
		void killFbaEmu();
		void filterRomlist(int flag = 0);
		void refrushRomlist(int flag = 0);

		private slots:
			void readStdout();
		void readStderr();
		void setRunning2True();
		void setRunning2False(int exitCode, QProcess::ExitStatus exitStatus);

signals:
		void clockChanged();
		void senseChanged();
		void soundChanged();
		void tweakChanged();
		void samplerateChanged();
		void z80coreChanged();
		void filterChanged();
		//void showfpsChanged();
		//void stateChanged();
		void romlistChanged();
		void stdoutandstderrChanged();
		void runningChanged();
		void settingChanged();

};

#endif
