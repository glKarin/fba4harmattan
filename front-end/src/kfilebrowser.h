#ifndef K_FILE_BROWSER_H
#define K_FILE_BROWSER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QFile>

class KFileBrowser : public QObject
{
	Q_OBJECT
		Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
		Q_PROPERTY(QList<QObject *> filelist READ filelist NOTIFY filelistChanged)
		//Q_PROPERTY(READ WRITE NOTIFY)
		
	private:
		QString cpp_path;
		QList<QObject *> cpp_filelist;

	private:
		void reset();
		void init();
		QString permissionCode2String(QFile::Permissions per);
		QString size2FormatString(qint64 size,bool isdir);

	public:
		KFileBrowser(QObject *object=0);
		~KFileBrowser();

		public slots:
			const QString & path() const;
		const QList<QObject *> & filelist() const;
		void setPath(const QString &path);

signals:
		void pathChanged();
		void filelistChanged();
};

#endif
