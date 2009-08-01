/***************************************************************************
 *   Copyright (C) 2008, 2009 by Malakhov Alexey                           *
 *   brezerk@gmail.com                                                     *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 *   In addition, as a special exception, the copyright holders give       *
 *   permission to link the code of this program with any edition of       *
 *   the Qt library by Trolltech AS, Norway (or with modified versions     *
 *   of Qt that use the same license as Qt), and distribute linked         *
 *   combinations including the two.  You must obey the GNU General        *
 *   Public License in all respects for all of the code used other than    *
 *   Qt.  If you modify this file, you may extend this exception to        *
 *   your version of the file, but you are not obligated to do so.  If     *
 *   you do not wish to do so, delete this exception statement from        *
 *   your version.                                                         *
 ***************************************************************************/

#include "q4wine-cli/main.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	QTextStream Qcout(stdout);
	QString _PREFIX, _DIR, _ICON, _IMAGE;
	QString path;
	int _ACTION=0;

	//! This is need for libq4wine-core.so import;
	typedef void *CoreLibPrototype (bool);
	CoreLibPrototype *CoreLibClassPointer;
	corelib *CoreLib;

	//Classes
	DataBase db;
	Prefix *db_prefix;
	Dir *db_dir;
	Icon *db_icon;
	Image *db_image;

	if (!initDb())
	   return -1;

	QStringList tables;
	tables << "prefix" << "dir" << "icon" << "images";
	if (!db.checkDb(tables))
	   return -1;

	// Loading libq4wine-core.so
	QLibrary libq4wine;
	libq4wine.setFileName("libq4wine-core");

	if (!libq4wine.load()){
		return -1;
	}

	// Getting corelib calss pointer
	CoreLibClassPointer = (CoreLibPrototype *) libq4wine.resolve("createCoreLib");
	CoreLib = (corelib *)CoreLibClassPointer(false);

	// Creating database classes
	db_dir = new Dir();
	db_icon = new Icon();
	db_image = new Image();
	db_prefix = new Prefix();


	QTranslator*  qtt = new QTranslator ( 0 );

	QString i18nPath;
	   i18nPath.clear();
	   i18nPath.append(APP_PREF);
	   i18nPath.append("/share/");
	   i18nPath.append(APP_SHORT_NAME);
	   i18nPath.append("/i18n");

	// Getting env LANG variable
	QString lang = CoreLib->getSetting("app", "lang", FALSE).toString() ;
	// This is hack for next QLocale bug:
	//  http://bugs.gentoo.org/150745

	if (lang.isEmpty()){
		lang = setlocale(LC_ALL, "");
		  if (lang.isEmpty()){
			lang = setlocale(LC_MESSAGES, "");
			    if (lang.isEmpty()){
				 lang = getenv("LANG");
			    }
		  }
		lang = lang.split(".").at(0).toLower();
		lang.append(".qm");
	}




	if (!lang.isNull()){
	   if (qtt->load(lang, i18nPath)){
		app.installTranslator( qtt );
	   } else {
		qDebug()<<"[EE] Can't open user selected translation";
		if (qtt->load("en_us.qm", i18nPath)){
		   app.installTranslator( qtt );
		} else {
		   qDebug()<<"[EE] Can't open default translation, fall back to native translation ;[";
		}
	   }
	} else {
	   qDebug()<<"[EE] Can't get LANG variable, fall back to native translation ;[";
	}

	_ACTION=-1;

	for (int i=1; i<argc; i++){
		if ((app.arguments().at(i)=="--help") or (app.arguments().at(i)=="-h")){
			_ACTION=-1;
			break;
		}
		if ((app.arguments().at(i)=="--version") or (app.arguments().at(i)=="-v")){
			_ACTION=11;
			break;
		}

		if ((app.arguments().at(i)=="--procs") or (app.arguments().at(i)=="-ps")){
			_ACTION=1;
		}

		if ((app.arguments().at(i)=="--prefix") or (app.arguments().at(i)=="-p")){
			i++;
			if (i<argc)
			_PREFIX=app.arguments().at(i);
			if (!db_prefix->isExistsByName(_PREFIX)){
				Qcout<<QObject::tr("Prefix named \"%1\" not exists. Run \"q4wine-cli -pl\" for prefix list.").arg(_PREFIX)<<endl;
				return -1;
			}
		}

		if ((app.arguments().at(i)=="--dir") or (app.arguments().at(i)=="-d")){
			i++;
			if (i<argc)
			_DIR=app.arguments().at(i);
		}

		if ((app.arguments().at(i)=="--icon") or (app.arguments().at(i)=="-i")){
			i++;
			if (i<argc)
			_ICON=app.arguments().at(i);
		}

		if ((app.arguments().at(i)=="--cdimage") or (app.arguments().at(i)=="-cd")){
			i++;
			if (i<argc)
			_IMAGE=app.arguments().at(i);
		}

		if ((app.arguments().at(i)=="--prefixlist") or (app.arguments().at(i)=="-pl")){
			_ACTION=2;
		}

		if ((app.arguments().at(i)=="--dirlist") or (app.arguments().at(i)=="-dl")){
			_ACTION=3;
		}

		if ((app.arguments().at(i)=="--iconlist") or (app.arguments().at(i)=="-il")){
			_ACTION=4;
		}

		if ((app.arguments().at(i)=="--cdlist") or (app.arguments().at(i)=="-cl")){
			_ACTION=5;
		}

		if ((app.arguments().at(i)=="--kill") or (app.arguments().at(i)=="-k")){
			_ACTION=6;
		}

		if ((app.arguments().at(i)=="--mount") or (app.arguments().at(i)=="-m")){
			_ACTION=7;
		}

		if ((app.arguments().at(i)=="--umount") or (app.arguments().at(i)=="-u")){
			_ACTION=8;
		}

		if ((app.arguments().at(i)=="--mountlist") or (app.arguments().at(i)=="-ml")){
			_ACTION=10;
		}

		if ((app.arguments().at(i)=="--binary") or (app.arguments().at(i)=="-b")){
			i++;
			if (i<argc)
			_IMAGE=app.arguments().at(i);
			path.clear();
			for (int j=++i; j<argc; j++){
				 path.append(app.arguments().at(j));
			}
			_ACTION=12;
		}
	}

	QList<QStringList> result;
	QStringList sresult;

	ExecObject execObj;

	switch (_ACTION){
		case 0:
			if (_PREFIX.isEmpty()){
				Qcout<<QObject::tr("No current prefix set. Set prefix via \"-p <prefix_name>\" key.")<<endl;
				return -1;
			}

			if (_ICON.isEmpty()){
				Qcout<<QObject::tr("No current icon set. Set icon via \"-i <icon_name>\" key.")<<endl;
				return -1;
			}

			if (!db_icon->isExistsByName(_PREFIX, _DIR, _ICON)){
				Qcout<<QObject::tr("Icon named \"%1\" not exists.  Run \"q4wine-cli -il\" for icon list.").arg(_ICON)<<endl;
				return -1;
			}

			if (CoreLib->runIcon(_PREFIX, _DIR, _ICON)){
				Qcout<<"Done"<<endl;
			} else {
				Qcout<<"Error"<<endl;
				return -1;
			}
		break;
		case 1:
			result = CoreLib->getWineProcessList();
			if (_PREFIX.isEmpty()){
				Qcout<<QObject::tr("Wine process list")<<endl;
			} else {
				Qcout<<QObject::tr("Wine process list for \"%1\" prefix").arg(_PREFIX)<<endl;
				path = db_prefix->getPath(_PREFIX);
			}
			// Preccess QList items one by one
			Qcout<<" "<<qSetFieldWidth(8)<<left<<QObject::tr("PID")<<qSetFieldWidth(6)<<left<<QObject::tr("Nice")<<qSetFieldWidth(20)<<left<<QObject::tr("Name")<<QObject::tr("Prefix path")<<qSetFieldWidth(0)<<endl;
			for (int i = 0; i < result.size(); ++i) {
				if (_PREFIX.isEmpty()){
					Qcout<<" "<<qSetFieldWidth(8)<<left<<result.at(i).at(0)<<qSetFieldWidth(6)<<left<<result.at(i).at(2)<<qSetFieldWidth(20)<<left<<result.at(i).at(1)<<result.at(i).at(3)<<qSetFieldWidth(0)<<endl;
				} else {
					if (path==result.at(i).at(3))
					   Qcout<<" "<<qSetFieldWidth(8)<<left<<result.at(i).at(0)<<qSetFieldWidth(6)<<left<<result.at(i).at(2)<<qSetFieldWidth(20)<<left<<result.at(i).at(1)<<result.at(i).at(3)<<qSetFieldWidth(0)<<endl;
				}
			}
		break;
		case 2:
			result = db_prefix->getFields();
			Qcout<<QObject::tr("Prefix list")<<endl;
			Qcout<<" "<<qSetFieldWidth(15)<<left<<QObject::tr("Name")<<QObject::tr("Path")<<qSetFieldWidth(0)<<endl;
			for (int i = 0; i < result.size(); ++i) {
				if (!result.at(i).at(2).isEmpty()){
					Qcout<<" "<<qSetFieldWidth(15)<<left<<result.at(i).at(1)<<result.at(i).at(2)<<qSetFieldWidth(0)<<endl;
				} else {
					Qcout<<" "<<qSetFieldWidth(15)<<left<<result.at(i).at(1)<<QDir::homePath()<<"/.wine/"<<qSetFieldWidth(0)<<endl;
				}
			}
		break;
		case 3:
			if (_PREFIX.isEmpty()){
				Qcout<<QObject::tr("No current prefix set. Set prefix via \"-p <prefix_name>\" key.")<<endl;
				return -1;
			}
			result = db_dir->getFieldsByPrefixName(_PREFIX);
			Qcout<<QObject::tr("Prefix \"%1\" has following dir list").arg(_PREFIX)<<endl;
			Qcout<<" "<<QObject::tr("Name")<<endl;
			for (int i = 0; i < result.size(); ++i) {
				Qcout<<" "<<result.at(i).at(1)<<endl;
			}
		break;
		case 4:
			if (_PREFIX.isEmpty()){
				Qcout<<QObject::tr("No current prefix set. Set prefix via \"-p <prefix_name>\" key.")<<endl;
				return -1;
			}

			if (! db_dir->isExistsByName(_PREFIX, _DIR)){
				Qcout<<QObject::tr("Dir named \"%1\" not exists. Run \"q4wine-cli -dl\" for dir list.").arg(_DIR)<<endl;
				return -1;
			}

			result = db_icon->getByPrefixAndDirName(_PREFIX, _DIR);
			if (_DIR.isEmpty()){
				Qcout<<QObject::tr("Prefix \"%1\" has following icon list").arg(_PREFIX)<<endl;
			} else {
				Qcout<<QObject::tr("Prefix \"%1\" has following icon list at \"%2\" directory").arg(_PREFIX).arg(_DIR)<<endl;
			}
			Qcout<<" "<<qSetFieldWidth(15)<<left<<QObject::tr("Name")<<QObject::tr("Description")<<qSetFieldWidth(0)<<endl;

			for (int i = 0; i < result.size(); ++i) {
				Qcout<<" "<<qSetFieldWidth(15)<<left<<result.at(i).at(1)<<result.at(i).at(2)<<qSetFieldWidth(0)<<endl;
			}
		break;
		case 5:
			result = db_image->getFields();
			Qcout<<QObject::tr("q4wine has following CD images in database")<<endl;
			Qcout<<" "<<qSetFieldWidth(25)<<left<<QObject::tr("Name")<<QObject::tr("Path")<<qSetFieldWidth(0)<<endl;
			for (int i = 0; i < result.size(); ++i) {
				Qcout<<" "<<qSetFieldWidth(25)<<left<<result.at(i).at(0)<<result.at(i).at(1)<<qSetFieldWidth(0)<<endl;
			}
		break;
		case 6:
			if (_PREFIX.isEmpty()){
				Qcout<<QObject::tr("No current prefix set. Set prefix via \"-p <prefix_name>\" key.")<<endl;
				return -1;
			}
			result = db_dir->getFieldsByPrefixName(_PREFIX);
			Qcout<<QObject::tr("Killing prefix \"%1\" wineserver.").arg(_PREFIX)<<endl;
			if (CoreLib->killWineServer(db_prefix->getPath(_PREFIX))){
				Qcout<<"Done"<<endl;
			} else {
				Qcout<<"Error"<<endl;
				return -1;
			}
		break;
		case 7:
			if (_PREFIX.isEmpty()){
				Qcout<<QObject::tr("No current prefix set. Set prefix via \"-p <prefix_name>\" key.")<<endl;
				return -1;
			}
			sresult = db_prefix->getFieldsByPrefixName(_PREFIX);

			if (sresult.at(6).isEmpty()){
				Qcout<<QObject::tr("No mount point set in prefix configuration.")<<endl;
				return -1;
			}

			if (_IMAGE.isEmpty()){
				if (sresult.at(7).isEmpty()){
					Qcout<<QObject::tr("No cdrom drive set in prefix configuration.")<<endl;
					return -1;
				}
				Qcout<<QObject::tr("Mounting drive \"%1\" into mount point \"%2\".").arg(sresult.at(7)).arg(sresult.at(6))<<endl;
				if (CoreLib->mountImage(sresult.at(7), _PREFIX)){
					Qcout<<"Done"<<endl;
				} else {
					Qcout<<"Error"<<endl;
					return -1;
				}
			} else {
				if (!QFile(_IMAGE).exists()){
					if (!db_image->isExistsByName(_IMAGE)){
						Qcout<<QObject::tr("No CD iamge \"%1\" exists. Run \"q4wine-cli -cl\" for CD image list.").arg(_IMAGE)<<endl;
						return -1;
					}
				}

				if (CoreLib->mountImage(_IMAGE, _PREFIX)){
					Qcout<<"Done"<<endl;
				} else {
					Qcout<<"Error"<<endl;
					return -1;
				}
			}
		break;
		case 8:
			if (_PREFIX.isEmpty()){
				Qcout<<QObject::tr("No current prefix set. Set prefix via \"-p <prefix_name>\" key.")<<endl;
				return -1;
			}
			sresult = db_prefix->getFieldsByPrefixName(_PREFIX);

			if (sresult.at(6).isEmpty()){
				Qcout<<QObject::tr("No mount point set in prefix configuration.")<<endl;
				return -1;
			}

			Qcout<<QObject::tr("Umounting mount point \"%1\".").arg(sresult.at(6))<<endl;
			if (CoreLib->umountImage(_PREFIX)){
				Qcout<<"Done"<<endl;
			} else {
				Qcout<<"Error"<<endl;
				return -1;
			}
		break;
		case 10:
			if (_PREFIX.isEmpty()){
				result = db_prefix->getFields();
				Qcout<<QObject::tr("Mounted media list for all prefixes")<<endl;
				Qcout<<" "<<qSetFieldWidth(15)<<left<<QObject::tr("Prefix")<<qSetFieldWidth(25)<<left<<QObject::tr("Mount point")<<QObject::tr("Media")<<qSetFieldWidth(0)<<endl;
				for (int i = 0; i < result.size(); ++i) {
					Qcout<<" "<<qSetFieldWidth(15)<<left<<result.at(i).at(1)<<qSetFieldWidth(25)<<left<<result.at(i).at(7)<<CoreLib->getMountedImages(result.at(i).at(7))<<qSetFieldWidth(0)<<endl;
				}
			} else {
				sresult = db_prefix->getFieldsByPrefixName(_PREFIX);

				if (sresult.at(6).isEmpty()){
				Qcout<<QObject::tr("No mount point set in prefix configuration.")<<endl;
				return -1;
				}

				Qcout<<QObject::tr("Mounted media list for prefix \"%1\"").arg(_PREFIX)<<endl;
				Qcout<<" "<<qSetFieldWidth(25)<<left<<QObject::tr("Mount point")<<QObject::tr("Media")<<qSetFieldWidth(0)<<endl;
				Qcout<<" "<<qSetFieldWidth(25)<<left<<sresult.at(6)<<CoreLib->getMountedImages(sresult.at(6))<<qSetFieldWidth(0)<<endl;
			}
		break;
		case 11:
			Qcout<<QObject::tr("q4wine-cli %1").arg(APP_VERS)<<endl;
			Qcout<<QObject::tr("(Copyright (C) 2008-2009, brezblock core team.")<<endl;
			Qcout<<QObject::tr("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.")<<endl;
			Qcout<<QObject::tr("This is free software: you are free to change and redistribute it.")<<endl;
			Qcout<<QObject::tr("There is NO WARRANTY, to the extent permitted by law.")<<endl;
			Qcout<<endl;
			Qcout<<QObject::tr("Author: Malakhov Alexey aka John Brezerk.")<<endl;
		break;
		case 12:
			if (_PREFIX.isEmpty()){
				Qcout<<QObject::tr("No current prefix set. Set prefix via \"-p <prefix_name>\" key.")<<endl;
				return -1;
			}

			qDebug()<<_IMAGE;

			if (!QFile(_IMAGE).exists()){
				Qcout<<QObject::tr("File \"%1\" not exists.").arg(_IMAGE)<<endl;
				return -1;
			}

			sresult = _IMAGE.split("/");
			execObj.wrkdir = _IMAGE.left(_IMAGE.length() - sresult.last().length());
			execObj.override = "";
			execObj.winedebug = "";
			execObj.useconsole = "";
			execObj.display = "";
			execObj.cmdargs = path;
			execObj.cmdargs = "";
			execObj.desktop = "";
			execObj.prefixid = db_prefix->getId(_PREFIX);
			execObj.execcmd=_IMAGE;
			if (CoreLib->runWineBinary(execObj)){
				Qcout<<"Done"<<endl;
			} else {
				Qcout<<"Error"<<endl;
				return -1;
			}
		break;
		default:
			Qcout<<QObject::tr("Usage:")<<endl;
			Qcout<<QObject::tr("  q4wine-cli [KEY]...")<<endl;
			Qcout<<QObject::tr("  q4wine-cli -p <prefix_name> [-d <dir_name>] -i <icon_name>")<<endl;
			Qcout<<QObject::tr("  q4wine-cli -p <prefix_name> -b <windows_binary_path> [args]")<<endl;
			Qcout<<QObject::tr("Console utility for wine applications and prefixes management.")<<endl<<endl;
			Qcout<<QObject::tr("KEYs list:")<<endl;
			Qcout<<qSetFieldWidth(25)<<left<<"  -h,  --help"<<QObject::tr("display this help and exit")<<qSetFieldWidth(0)<<endl;
			Qcout<<qSetFieldWidth(25)<<left<<"  -v,  --version"<<QObject::tr("output version information and exit")<<qSetFieldWidth(0)<<endl;
			Qcout<<qSetFieldWidth(25)<<left<<"  -ps, --procs"<<QObject::tr("output wine process list for current prefix or for all prefixes and exit ")<<qSetFieldWidth(0)<<endl;
			Qcout<<qSetFieldWidth(25)<<left<<"  -p,  --prefix"<<QObject::tr("sets the current prefix name")<<qSetFieldWidth(0)<<endl;
			Qcout<<qSetFieldWidth(25)<<left<<"  -d,  --dir"<<QObject::tr("sets the current direcory name")<<qSetFieldWidth(0)<<endl;
			Qcout<<qSetFieldWidth(25)<<left<<"  -i,  --icon"<<QObject::tr("sets the current icon name")<<qSetFieldWidth(0)<<endl;
			Qcout<<qSetFieldWidth(25)<<left<<"  -cd, --cdimage"<<QObject::tr("sets the cd iamge name")<<qSetFieldWidth(0)<<endl;
			Qcout<<qSetFieldWidth(25)<<left<<"  -b, --binary"<<QObject::tr("sets the path to windows binary for execute with current prefix settings")<<qSetFieldWidth(0)<<endl;
			Qcout<<qSetFieldWidth(25)<<left<<"  -k,  --kill"<<QObject::tr("sends -9 term signal to current prefix precess or for all prefixes processes")<<qSetFieldWidth(0)<<endl;
			Qcout<<qSetFieldWidth(25)<<left<<"  -pl, --prefixlist"<<QObject::tr("output all exesting prefixes names and exit")<<qSetFieldWidth(0)<<endl;
			Qcout<<qSetFieldWidth(25)<<left<<"  -dl, --dirlist"<<QObject::tr("output all exesting dir names for current prefix and exit")<<qSetFieldWidth(0)<<endl;
			Qcout<<qSetFieldWidth(25)<<left<<"  -il, --iconlist"<<QObject::tr("output all exesting icon names for current prefix/directory and exit")<<qSetFieldWidth(0)<<endl;
			Qcout<<qSetFieldWidth(25)<<left<<"  -cl, --cdlist"<<QObject::tr("output all cd images list and exit")<<qSetFieldWidth(0)<<endl;
			Qcout<<qSetFieldWidth(25)<<left<<"  -ml, --mountlist"<<QObject::tr("output all mounted media for current prefix or all prefixes and exit")<<qSetFieldWidth(0)<<endl;
			Qcout<<qSetFieldWidth(25)<<left<<"  -m,  --mount"<<QObject::tr("mount an cd iamage or drive for current prefix and exit")<<qSetFieldWidth(0)<<endl;
			Qcout<<qSetFieldWidth(25)<<left<<"  -u,  --umount"<<QObject::tr("umount an cd iamage or drive for current prefix and exit")<<qSetFieldWidth(0)<<endl;
			Qcout<<endl;
			Qcout<<QObject::tr("Exit status:")<<endl;
			Qcout<<QObject::tr("  0 if OK,")<<endl;
			Qcout<<QObject::tr(" -1 if serious troubles")<<endl;
			Qcout<<endl;
			Qcout<<QObject::tr("Report q4wine bugs to brezerk@gmail.com")<<endl;
			Qcout<<QObject::tr("q4wine homepage: <http://sourceforge.net/projects/q4wine/>")<<endl;
			Qcout<<QObject::tr("General help using GNU software: <http://www.gnu.org/gethelp/>")<<endl;
		break;
	}

	db.close();
	return 0;
}