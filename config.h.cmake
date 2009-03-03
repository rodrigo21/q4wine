/***************************************************************************
 *   Copyright (C) 2008 by Malakhov Alexey                                 *
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

/*
 * NOTE: DO NOT EDIT THIS FILE! 
 *
 * It will be rplaced by config.sh and qmake
 *
 * Edit config.h.in instead
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

struct ExecObject{
	QString execcmd;
	QString prefixid;
	QString runcmd;
	QString useconsole;
	QString cmdargs;
	QString override;
	QString winedebug;
	QString display;
	QString wrkdir;
	QString desktop;
	QString nice;
};


#define APP_NAME "Q4Wine"
#define APP_SHORT_NAME "q4wine"

#cmakedefine _OS_LINUX_
#cmakedefine _OS_FREEBSD_
#cmakedefine _OS_DARWIN_

#cmakedefine WITHOUT_ICOTOOLS

/*
 * Note APP_VERS and APP_PREF automatycaly generates by qmake and config.sh
 *
 * APP_VERS -- Application version
 * APP_PREF -- Application install prefix (Used for trak translations, help and thems files)
 */

