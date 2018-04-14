#ifndef K_FILEINFO_H
#define K_FILEINFO_H

#include <QObject>
#include <QString>

class KFileInfo:public QObject
{
	Q_OBJECT
		Q_PROPERTY(QString path READ path CONSTANT)
		Q_PROPERTY(QString owner READ owner CONSTANT)
		Q_PROPERTY(QString group READ group CONSTANT)
		Q_PROPERTY(QString size READ size CONSTANT)
		Q_PROPERTY(QString permission READ permission CONSTANT)
		Q_PROPERTY(bool type READ type CONSTANT)
		Q_PROPERTY(QString name READ name CONSTANT)

		enum User{Owner=0x1000,Group=0x10,Other=0x1};

	private:
		QString cpp_path;
		QString cpp_name;
		QString cpp_owner;
		QString cpp_group;
		QString cpp_permission;
		QString cpp_size;
		bool cpp_type;

	public:
		KFileInfo(QObject *object=0)
			:QObject(object)
		{
			cpp_type=false;
		}
		virtual ~KFileInfo()
		{
		}
		void setName(const QString &name)
		{
			cpp_name=name;
		}
		void setPath(const QString &path)
		{
			cpp_path=path;
		}
		void setOwner(const QString &owner)
		{
			cpp_owner=owner;
		}
		void setGroup(const QString &group)
		{
			cpp_group=group;
		}
		void setPermission(const QString &per)
		{
			cpp_permission=per;
		}
		void setType(bool b){
			cpp_type=b;
		}
		void setSize(const QString &size)
		{
			cpp_size=size;
		}

		public slots:
		const QString & path() const
		{
			return cpp_path;
		}
		const QString & name() const
		{
			return cpp_name;
		}
		const QString & owner() const
		{
			return cpp_owner;
		}
		const QString & group() const
		{
			return cpp_group;
		}
		const QString & permission() const
		{
			return cpp_permission;
		}
		const QString & size() const
		{
			return cpp_size;
		}
		bool type() const
		{
			return cpp_type;
		}

};
#endif
