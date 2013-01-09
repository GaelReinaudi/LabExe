/***************************************************************************
   Copyright (C) 2007 by Matthias Lechner <matthias@lmme.de>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the
   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
 ***************************************************************************/

#ifndef GTREEITEM_H
#define GTREEITEM_H
#include <QObject>
#include <QList>
#include <QVariant>
#include <QVector>
#include <QtGui>

class GTreeItem;

class GTreeItem : public QObject 
{
	Q_OBJECT

	enum ChildProperty {VisibleChildren, AllChildren};

public:
	GTreeItem( GTreeItem* pParent = 0 );
    ~GTreeItem();

public:
	int childCount( ChildProperty childProperty = VisibleChildren ) const;
	int ThisChildNumber() const;
	GTreeItem* ParentTreeItem() const;
	QList<GTreeItem*> ChildrenItems() const { return m_childItems; };
	//! Returns a QList of all the children item, in all the generations.
	QList<GTreeItem*> AllGenerationsChildrenItems() const;
	//! Returns a QList of all parents, starting from the higher.
	QList<GTreeItem*> AllParentsItems() const;

protected:
	//! Sets the parent of this node
	//! If you override it, don't forget to call GTreeItem::SetParent().
	void SetParent( GTreeItem* newparent );
	void AddChild( GTreeItem* newchild );

protected slots:
	//! This function is called just after a child has been added. 
	//! It calls the parents function, and so on up to the higher parent.
	//! The second parameter, AtGeneration, tells the parent at which depth the child has actually been added.
	virtual void ChildWasJustAdded(GTreeItem* pItemAdded, int AtGeneration);

private:
	GTreeItem* ChildTreeItem( int row );
    void setIcon( QIcon icon );
    void setFont( QFont font );
    void setVisible( bool visible );
    QVariant data() const;
    QIcon icon() const;
    QFont font() const;
    bool visible() const;

signals:
   void dataChanged( GTreeItem* item );

private:
    GTreeItem* m_parentItem;
	QList<GTreeItem*> m_childItems;
	QString m_itemData;
    QIcon m_icon;
    QFont m_font;
    bool m_visible;

};

#endif // GTREEITEM_H
