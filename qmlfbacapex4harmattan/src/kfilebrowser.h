#ifndef K_FILE_BROWSER_H
#define K_FILE_BROWSER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QFile>

class QDir;

class KFileBrowser : public QObject
{
	Q_OBJECT
		Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
		Q_PROPERTY(QList<QObject *> filelist READ filelist NOTIFY filelistChanged)
		//Q_PROPERTY(READ WRITE NOTIFY)
		
		Q_PROPERTY(bool showHidden READ showHidden WRITE setShowHidden NOTIFY showHiddenChanged)
		Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)
		
	private:
		QString cpp_path;
		QList<QObject *> cpp_filelist;
		bool cpp_showHidden;
		QString cpp_filter;
		QDir *m_dir;

	private:
		void reset();
		void init();
		QString permissionCode2String(QFile::Permissions per);
		QString size2FormatString(qint64 size,bool isdir);

	public:
		KFileBrowser(QObject *object=0);
		~KFileBrowser();

		QString path() const;
		const QList<QObject *> & filelist() const;
		void setPath(const QString &path);

		bool showHidden() const;
		void setShowHidden(bool b);
		QString filter() const;
		void setFilter(const QString &filter);

signals:
		void pathChanged(const QString &path);
		void filelistChanged();

		void showHiddenChanged(bool b);
		void filterChanged(const QString &filter);
};

#endif
