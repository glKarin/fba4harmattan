#ifndef KZ_FBAROM_INFO_H
#define KZ_FBAROM_INFO_H

#include <QString>
#include <QObject>

class KZFbaRomInfo : public QObject
{
	Q_OBJECT
		Q_PROPERTY(QString name READ name NOTIFY nameChanged)
		Q_PROPERTY(QString path READ path NOTIFY pathChanged)
		Q_PROPERTY(QString com READ com NOTIFY comChanged)
		Q_PROPERTY(bool exists READ exists NOTIFY existsChanged)
		Q_PROPERTY(int year READ year NOTIFY yearChanged)

	public slots:
		const QString & name() const
		{
			return cpp_name;
		}

		const QString & path() const
		{
			return cpp_path;
		}

		int year() const
		{
			return cpp_year;
		}

		bool exists() const
		{
			return cpp_exists;
		}

		const QString & com() const
		{
			return cpp_com;
		}

	public:
		KZFbaRomInfo(QObject *object = 0)
			:QObject(object)
		{
		}

		~KZFbaRomInfo()
		{
		}

		void setName(const QString &name)
		{
			cpp_name = name;
		}

		void setPath(const QString &path)
		{
			cpp_path = path;
		}

		void setYear(int year)
		{
			cpp_year = year;
		}

		void setExists(bool b)
		{
			cpp_exists = b;
		}

		void setCom(const QString &com)
		{
			cpp_com = com;
		}

		signals:
		void nameChanged();
		void pathChanged();
		void yearChanged();
		void comChanged();
		void existsChanged();

	private:
		QString cpp_name;
		QString cpp_path;
		QString cpp_com;
		bool cpp_exists;
		int cpp_year;
};

#endif
