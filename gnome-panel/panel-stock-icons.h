/*
 * panel-stock-icons.h: panel stock icons registration
 *
 * Copyright (C) 2002 Sun Microsystems, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *	Mark McLoughlin <mark@skynet.ie>
 */

#ifndef __PANEL_STOCK_ICONS_H__
#define __PANEL_STOCK_ICONS_H__

#include <glib.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

/* themeable size - "panel-menu" -- This is used for the icons in the menus */
#define PANEL_DEFAULT_MENU_ICON_SIZE 		24

GtkIconSize panel_menu_icon_get_size         (void);

G_END_DECLS

#endif /* __PANEL_STOCK_ICONS_H__ */
