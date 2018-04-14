#include <QVariant>
#include <QDir>
#include <QDebug>
#include <QFileInfo>

#include "kfilebrowser.h"
#include "kfileinfo.h"

KFileBrowser::KFileBrowser(QObject *object)
	:QObject(object)
{
	cpp_path=QDir::homePath();
	init();
}

KFileBrowser::~KFileBrowser()
{
	reset();
}

void KFileBrowser::reset()
{
	if(!cpp_filelist.isEmpty())
	{
		for(QList<QObject *>::iterator itor=cpp_filelist.begin();
				itor!=cpp_filelist.end();
				itor++)
			delete *itor;
		cpp_filelist.clear();
	}
}

void KFileBrowser::init()
{
	reset();
	QFileInfoList list=QDir(cpp_path).entryInfoList(QDir::AllEntries | QDir::NoDot,QDir::DirsFirst | QDir::Name);
	for(QFileInfoList::const_iterator itor=list.begin();
			itor!=list.end();
			itor++)
	{
		if(cpp_path=="/" && itor->fileName()=="..")
			continue;
		KFileInfo *info=new KFileInfo;
		info->setName(itor->fileName());
		info->setPath(itor->absoluteFilePath());
		if(itor->isDir())
			info->setType(true);
		else
			info->setType(false);
		info->setOwner(itor->owner());
		info->setGroup(itor->group());
		info->setSize(size2FormatString(itor->size(),itor->isDir()));
		info->setPermission(permissionCode2String(itor->permissions()));
		cpp_filelist.push_back(info);
	}
	emit filelistChanged();
}

QString KFileBrowser::size2FormatString(qint64 size,bool isdir)
{
	QString result;
	QString p("%1%2");
	const double KB=1024;
	if(isdir)
	{
		if(size<=4*KB)
			result=p.arg(size).arg('b');
		else if(size>4*KB && size<=64*KB)
			result=p.arg(size/KB).arg('k');
	}else
	{
		QVariant var(size);
		bool b=var.canConvert<double>();
		double d;
		const double MB=KB*KB;
		const double GB=MB*KB;
		if(size>2*KB && size<2*MB && b){
			d=var.toDouble()/KB;
			result=p.arg(d,1,'f',1).arg('k');
		}else if(size>=2*MB && size<2*GB && b){
			d=var.toDouble()/MB;
			result=p.arg(d,1,'f',1).arg('M');
		}else if(size>=2*GB && b){
			d=var.toDouble()/GB;
			result=p.arg(d,1,'f',1).arg('G');
		}else{
			result=p.arg(size).arg('b');
		}
	}
	return result;
}

const QString & KFileBrowser::path() const
{
	return cpp_path;
}

void KFileBrowser::setPath(const QString &path)
{
	cpp_path=path;
	init();
	emit pathChanged();
}

/*
void KFileBrowser::currentPath(const QString &path)
{
	if(path=="../")
	{
		int len=cpppath.length();
		int i=cpppath.lastIndexOf('/',len-2);
		if(i!=-1)
			cpppath=cpppath.left(i+1);
		else
			cpppath="/";
		emit pathChange();
	}else if(path.endsWith('/')){
		cpppath=cpppath+path;
		emit pathChange();
	}else{
		currentFile(path);
	}
}
*/

const QList<QObject *> & KFileBrowser::filelist() const
{
	return cpp_filelist;
}

QString KFileBrowser::permissionCode2String(QFile::Permissions per)
{
	QString result("---------");
	if(per & 0x4000)
		result.replace(0,1,"r");
	if(per & 0x2000)
		result.replace(1,1,"w");
	if(per & 0x1000)
		result.replace(2,1,"x");
	if(per & 0x0040)
		result.replace(3,1,"r");
	if(per & 0x0020)
		result.replace(4,1,"w");
	if(per & 0x0010)
		result.replace(5,1,"x");
	if(per & 0x0004)
		result.replace(6,1,"r");
	if(per & 0x0002)
		result.replace(7,1,"w");
	if(per & 0x0001)
		result.replace(8,1,"x");
	return result;
}
