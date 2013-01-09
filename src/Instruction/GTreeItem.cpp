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

#include "GTreeItem.h"
#include <QString>

GTreeItem::GTreeItem( GTreeItem *pParent )
	: QObject(pParent)
{
    m_parentItem = 0;
	SetParent(pParent);
    m_itemData = "data";
    m_visible = true;
}

GTreeItem::~GTreeItem()
{
    qDeleteAll( m_childItems );
    m_childItems.clear();
}

void GTreeItem::setIcon( QIcon icon ) {
    m_icon = icon;
//    emit dataChanged( this );
}

void GTreeItem::setFont( QFont font ) {
    m_font = font;
//    emit dataChanged( this );
}

void GTreeItem::setVisible( bool visible ) {
    m_visible = visible;
}

bool GTreeItem::visible() const {
    return m_visible;
}

GTreeItem *GTreeItem::ChildTreeItem( int row )
{
    return m_childItems.value( row );
}

int GTreeItem::childCount( ChildProperty childProperty ) const
{
    int count = 0;

    if( childProperty == AllChildren )
        count = m_childItems.count();
    else if( childProperty == VisibleChildren ) {
        for( int i=0; i < m_childItems.count(); i++ ) {
            if( m_childItems.at( i )->visible() )
                count++;
        }
    }
    else
        count = m_childItems.count();

    return count;
}

QVariant GTreeItem::data() const
{
    return m_itemData;
}

QIcon GTreeItem::icon() const
{
    return m_icon;
}

QFont GTreeItem::font() const
{
    return m_font;
}

GTreeItem *GTreeItem::ParentTreeItem() const
{
    return m_parentItem;
}

int GTreeItem::ThisChildNumber() const
{
    if( m_parentItem )
        return m_parentItem->m_childItems.indexOf( const_cast<GTreeItem*>(this) );

    return 0;
}

void GTreeItem::SetParent( GTreeItem* newparent )
{
	// First of all, we check that the newparent is not one of my children
	if(AllGenerationsChildrenItems().contains(newparent))
		return;
	// We remove this from an other parent's child list
	if(m_parentItem)
		m_parentItem->m_childItems.removeAll(this);
	m_parentItem = newparent;
	// We add it to the new parent if it exists.
	if(m_parentItem) {
		// Normally, it should not be already in the list.
		Q_ASSERT_X(!m_parentItem->m_childItems.contains(this), "GTreeItem::SetParent", "This GTreeItem seems to be already in the list of children. That should not be the case.");
		m_parentItem->m_childItems.append(this);
		m_parentItem->ChildWasJustAdded(this, 1);
	}
}

void GTreeItem::AddChild( GTreeItem* newchild )
{
	if(newchild)
		newchild->SetParent(this);
}

void GTreeItem::ChildWasJustAdded( GTreeItem* pItemAdded, int AtGeneration )
{
	if(m_parentItem)
		m_parentItem->ChildWasJustAdded(pItemAdded, AtGeneration + 1);
}

QList<GTreeItem*> GTreeItem::AllGenerationsChildrenItems() const
{
	QList<GTreeItem*> ListAllChildren;
	ListAllChildren.append(m_childItems);
	foreach(GTreeItem* pChild, m_childItems) {
		ListAllChildren.append(pChild->AllGenerationsChildrenItems());
	}
	return ListAllChildren;
}

QList<GTreeItem*> GTreeItem::AllParentsItems() const
{
	QList<GTreeItem*> ListAllParents;
	if(ParentTreeItem()) {
		ListAllParents.append(ParentTreeItem()->AllParentsItems());
		ListAllParents.append(ParentTreeItem());
	}
	return ListAllParents;
}