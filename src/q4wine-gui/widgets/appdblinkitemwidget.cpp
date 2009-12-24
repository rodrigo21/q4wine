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
 ***************************************************************************/

#include "appdblinkitemwidget.h"

AppDBLinkItemWidget::AppDBLinkItemWidget(QString text, short int action, QWidget * parent) : QLabel(parent)
{
	this->setText(text);
	this->action = action;
	this->setEnabled(true);
	return;
}

AppDBLinkItemWidget::~AppDBLinkItemWidget(){
	//nothing but...
}

void AppDBLinkItemWidget::setEnabled(bool enable){
	// Sets item color to QT::Link color while item is enabled.
	QPalette p(palette());

	if (enable){
		p.setColor(QPalette::WindowText, QPalette().color(QPalette::Link));
		this->installEventFilter(this);
		this->setCursor(Qt::PointingHandCursor);
	} else {
		p.setColor(QPalette::WindowText, QPalette().color(QPalette::WindowText));
		this->removeEventFilter(this);
		this->setCursor(Qt::ArrowCursor);
	}
	this->setPalette(p);
	return;
}

void AppDBLinkItemWidget::setBold(bool enable){
	// Sets item text bold state.
	QFont font;

	font.setBold(enable);
	this->setFont(font);
	return;
}

void AppDBLinkItemWidget::setAction(short int action){
	/*** Action codes ***
	1: Search
	2: Toggle search page
	3: Show app
	4: Show app version
	5: Show category
	6: Open url
	7: Show parent comment
	*****************/
	this->action=action;
	return;
}

void AppDBLinkItemWidget::setSearchUrl(QString url){
	/* Search Url
		This might be used for saving user search text
	*/
	this->search=url;
	return;
}

void AppDBLinkItemWidget::setAppId(int id){
	/* App Id
		Saveing App Id
	*/
	this->appid=id;
	return;
}

void AppDBLinkItemWidget::setVerId(int id){
	/* Version Id
		Saveing Version Id
	*/
	this->verid=id;
	return;
}

void AppDBLinkItemWidget::setCatId(int id){
	/* Category Id
		Saveing Category Id
	*/
	this->catid=id;
	return;
}

void AppDBLinkItemWidget::setPage(short int id){
	/* Category Id
		Saveing Category Id
	*/
	this->page=id;
	return;
}

void AppDBLinkItemWidget::setParentId(int id){
	/* Parent comment Id
		Saveing id of parent comment
	*/
	this->parentid=id;
	return;
}

bool AppDBLinkItemWidget::eventFilter(QObject *obj, QEvent *event){
	if (event->type()==QEvent::MouseButtonRelease){
		switch (action){
  case 2:
			emit(itemTrigged(2, search, page, 0, 0));
			break;
  case 3:
			emit(itemTrigged(3, "", appid, 0, 0));
			break;
  case 5:
			emit(itemTrigged(5, "", catid, 0, 0));
			break;
  case 6:
			emit(itemTrigged(6, search, 0, 0, 0));
			break;
  case 7:
			emit(itemTrigged(7, "", appid, 0, 0));
			break;
  case 8:
			emit(requestParentComment(parentid));
			break;
		}
	} else if (event->type()==QEvent::Enter){
		QPalette p(palette());
		// Set colour
		p.setColor(QPalette::WindowText, QPalette().color(QPalette::Highlight));
		this->setPalette(p);
	} else if (event->type()==QEvent::Leave){
		QPalette p(palette());
		// Restore default color
		p.setColor(QPalette::WindowText, QPalette().color(QPalette::Link));
		this->setPalette(p);
	}
	return false;
}
