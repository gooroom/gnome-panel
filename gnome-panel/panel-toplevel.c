/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*-
 *
 * panel-toplevel.c: The panel's toplevel window object.
 *
 * Copyright (C) 2003 Sun Microsystems, Inc.
 * Copyright (C) 2004 Rob Adams
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

#include <config.h>

#include "panel-toplevel.h"

#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <gdk/gdkx.h>
#include <glib/gi18n.h>

#include <libpanel-util/panel-glib.h>

#include "gp-arrow-button.h"
#include "gp-theme.h"
#include "panel-xutils.h"
#include "panel-multiscreen.h"
#include "panel-a11y.h"
#include "panel-typebuiltins.h"
#include "panel-widget.h"
#include "panel-bindings.h"
#include "panel-struts.h"
#include "panel-lockdown.h"
#include "panel-schemas.h"

#define DEFAULT_SIZE              48
#define DEFAULT_AUTO_HIDE_SIZE    1
#define DEFAULT_HIDE_DELAY        300
#define DEFAULT_UNHIDE_DELAY      100
#define DEFAULT_DND_THRESHOLD     8
#define MINIMUM_WIDTH             100
#define MAXIMUM_SIZE_SCREEN_RATIO 5
#define SNAP_TOLERANCE_FACTOR     6
#define HANDLE_SIZE               10

typedef enum {
	PANEL_STATE_NORMAL       = 0,
	PANEL_STATE_AUTO_HIDDEN  = 1,
	PANEL_STATE_HIDDEN_UP    = 2,
	PANEL_STATE_HIDDEN_DOWN  = 3,
	PANEL_STATE_HIDDEN_LEFT  = 4,
	PANEL_STATE_HIDDEN_RIGHT = 5
} PanelState;

typedef enum {
	PANEL_GRAB_OP_NONE,
	PANEL_GRAB_OP_MOVE,
	PANEL_GRAB_OP_RESIZE,
	PANEL_GRAB_OP_RESIZE_UP,
	PANEL_GRAB_OP_RESIZE_DOWN,
	PANEL_GRAB_OP_RESIZE_LEFT,
	PANEL_GRAB_OP_RESIZE_RIGHT
} PanelGrabOpType;

struct _PanelToplevelPrivate {
	char                   *toplevel_id;

	char                   *settings_path;
	GSettings              *settings;
	GSettings              *delayed_settings;
	guint                   apply_delayed_id;

	GpTheme                *theme;

	gboolean                expand;
	PanelOrientation        orientation;
	int                     size;

	/* relative to the monitor origin */
	int                     x;
	int                     y;
	/* relative to the bottom right corner, -1 to ignore and use x, y*/
	int                     x_right;
	int                     y_bottom;

	int                     monitor;
	/* this is used when the configured monitor is missing. We keep it so
	 * we can move the toplevel to the right monitor when it becomes
	 * available */
	int                     configured_monitor;

	int                     hide_delay;
	int                     unhide_delay;
	int                     auto_hide_size;
	PanelAnimationSpeed     animation_speed;

	int                     snap_tolerance;
	GtkSettings            *gtk_settings;

	PanelState              state;

	char                   *name;
	char                   *description;

	guint                   hide_timeout;
	guint                   unhide_timeout;

	GdkRectangle            geometry;

	int                     original_width;
	int                     original_height;

	PanelGrabOpType         grab_op;

	/* The offset within the panel from which the panel
	 * drag was initiated relative to the screen origin.
	 */
	int                     drag_offset_x;
	int                     drag_offset_y;

	/* Saved state before for cancelled grab op */
	int                     orig_monitor;
	int                     orig_x;
	int                     orig_y;
	int                     orig_x_right;
	int                     orig_y_bottom;
	int                     orig_size;
	int                     orig_orientation;

	/* relative to the monitor origin */
	int                     animation_end_x;
	int                     animation_end_y;
	gint64                  animation_start_time;
	gint64                  animation_end_time;
	guint                   animation_timeout;

	PanelWidget            *panel_widget;
	GtkWidget              *grid;
	GtkWidget              *hide_button_top;
	GtkWidget              *hide_button_bottom;
	GtkWidget              *hide_button_left;
	GtkWidget              *hide_button_right;

	gint			n_autohide_disablers;

	guint                   auto_hide : 1;
	guint                   animate : 1;
	guint                   buttons_enabled : 1;
	guint                   arrows_enabled : 1;

	/* The co-ordinates are relative to center screen */
	guint                   x_centered : 1;
	guint                   y_centered : 1;

	/* The panel is not lined up with th screen edge */
	guint                   floating : 1;

	/* We are currently animating a hide/show */
	guint                   animating : 1;

	/* This is a keyboard initiated grab operation */
	guint                   grab_is_keyboard : 1;

	/* The x-y co-ordinates temporarily specify the panel center.
	 * This is used when the panel is rotating, because the width/height
	 * of the toplevel might change, so we need to compute new values for
	 * those. */
	guint                   position_centered : 1;

	/* More saved grab op state */
	guint                   orig_x_centered : 1;
	guint                   orig_y_centered : 1;

	/* flag to see if we have already done geometry updating,
	   if not then we're still loading and can ignore many things */
	guint                   updated_geometry_initial : 1;
	/* flag to see if we have done the initial animation */
	guint                   initial_animation_done : 1;
};

enum {
	HIDE_SIGNAL,
	UNHIDE_SIGNAL,
	POPUP_PANEL_MENU_SIGNAL,
	TOGGLE_EXPAND_SIGNAL,
	EXPAND_SIGNAL,
	UNEXPAND_SIGNAL,
	TOGGLE_HIDDEN_SIGNAL,
	BEGIN_MOVE_SIGNAL,
	BEGIN_RESIZE_SIGNAL,
	LAST_SIGNAL
};

enum {
	PROP_0,
	PROP_TOPLEVEL_ID,
	PROP_SETTINGS_PATH,
	PROP_NAME,
	PROP_EXPAND,
	PROP_ORIENTATION,
	PROP_SIZE,
	PROP_X,
	PROP_X_RIGHT,
	PROP_X_CENTERED,
	PROP_Y,
	PROP_Y_BOTTOM,
	PROP_Y_CENTERED,
	PROP_MONITOR,
	PROP_AUTOHIDE,
	PROP_HIDE_DELAY,
	PROP_UNHIDE_DELAY,
	PROP_AUTOHIDE_SIZE,
	PROP_ANIMATE,
	PROP_ANIMATION_SPEED,
	PROP_BUTTONS_ENABLED
};

static guint         toplevel_signals [LAST_SIGNAL] = { 0 };
static GSList       *toplevel_list = NULL;

G_DEFINE_TYPE_WITH_PRIVATE (PanelToplevel, panel_toplevel, GTK_TYPE_WINDOW)

static void panel_toplevel_calculate_animation_end_geometry (PanelToplevel *toplevel);

static gboolean panel_toplevel_position_is_writable (PanelToplevel *toplevel);

static void panel_toplevel_bind_gsettings       (PanelToplevel *toplevel);
static void panel_toplevel_set_toplevel_id      (PanelToplevel *toplevel,
						 const char    *toplevel_id);
static void panel_toplevel_set_settings_path    (PanelToplevel *toplevel,
						 const char    *settings_path);
static void panel_toplevel_set_animate          (PanelToplevel *toplevel,
						 gboolean       animate);

static void panel_toplevel_update_monitor       (PanelToplevel *toplevel);
static void panel_toplevel_set_monitor_internal (PanelToplevel *toplevel,
						 int            monitor,
						 gboolean       force_resize);

static void
update_style_classes (PanelToplevel *toplevel)
{
	GtkStyleContext *context;

	context = gtk_widget_get_style_context (GTK_WIDGET (toplevel));

	gtk_style_context_remove_class (context, GTK_STYLE_CLASS_HORIZONTAL);
	gtk_style_context_remove_class (context, GTK_STYLE_CLASS_VERTICAL);
	gtk_style_context_remove_class (context, GTK_STYLE_CLASS_RIGHT);
	gtk_style_context_remove_class (context, GTK_STYLE_CLASS_LEFT);
	gtk_style_context_remove_class (context, GTK_STYLE_CLASS_TOP);
	gtk_style_context_remove_class (context, GTK_STYLE_CLASS_BOTTOM);

	switch (toplevel->priv->orientation) {
	case PANEL_ORIENTATION_TOP:
		gtk_style_context_add_class (context, GTK_STYLE_CLASS_HORIZONTAL);
		gtk_style_context_add_class (context, GTK_STYLE_CLASS_TOP);
		break;

	case PANEL_ORIENTATION_LEFT:
		gtk_style_context_add_class (context, GTK_STYLE_CLASS_VERTICAL);
		gtk_style_context_add_class (context, GTK_STYLE_CLASS_LEFT);
		break;

	case PANEL_ORIENTATION_BOTTOM:
		gtk_style_context_add_class (context, GTK_STYLE_CLASS_HORIZONTAL);
		gtk_style_context_add_class (context, GTK_STYLE_CLASS_BOTTOM);
		break;

	case PANEL_ORIENTATION_RIGHT:
		gtk_style_context_add_class (context, GTK_STYLE_CLASS_VERTICAL);
		gtk_style_context_add_class (context, GTK_STYLE_CLASS_RIGHT);
		break;

	default:
		g_assert_not_reached ();
		break;
	}
}

GSList *
panel_toplevel_list_toplevels (void)
{
	return toplevel_list;
}

PanelToplevel *
panel_toplevel_get_by_id (const char *toplevel_id)
{
	GSList *l;

	if (PANEL_GLIB_STR_EMPTY (toplevel_id))
		return NULL;

	for (l = toplevel_list; l; l = l->next) {
		PanelToplevel *toplevel = l->data;

		if (!g_strcmp0 (toplevel->priv->toplevel_id, toplevel_id))
			return toplevel;
	}

	return NULL;
}

gboolean
panel_toplevel_is_last (PanelToplevel *toplevel)
{
	GSList *l;

	for (l = toplevel_list; l; l = l->next) {
		if (l->data != toplevel)
			return FALSE;
	}

	return TRUE;
}

gboolean
panel_toplevel_find_empty_spot (GdkScreen        *screen,
				PanelOrientation *orientation,
				int              *monitor)
{
	int      *filled_spots;
	GSList   *li;
	int       i;
	gboolean  found_a_spot = FALSE;
	GdkDisplay *display;
	gint n_monitors;

	*monitor = 0;
	*orientation = PANEL_ORIENTATION_TOP;

	display = gdk_display_get_default ();
	n_monitors = gdk_display_get_n_monitors (display);

	filled_spots = g_new0 (int, n_monitors);

	for (li = panel_toplevel_list_toplevels (); li != NULL; li = li->next) {
		PanelToplevel *toplevel = li->data;
		GdkScreen *toplevel_screen = gtk_window_get_screen (GTK_WINDOW (toplevel));
		int toplevel_monitor = panel_toplevel_get_monitor (toplevel);

		if (toplevel_screen != screen ||
		    toplevel_monitor < 0)
			continue;

		filled_spots[toplevel_monitor] |= panel_toplevel_get_orientation (toplevel);
	}

	for (i = 0; i < n_monitors; i++) {
		/* These are ordered based on "priority" of the
		   orientation when picking it */
		if ( ! (filled_spots[i] & PANEL_ORIENTATION_TOP)) {
			*orientation = PANEL_ORIENTATION_TOP;
			*monitor = i;
			found_a_spot = TRUE;
			break;
		} else if ( ! (filled_spots[i] & PANEL_ORIENTATION_BOTTOM)) {
			*orientation = PANEL_ORIENTATION_BOTTOM;
			*monitor = i;
			found_a_spot = TRUE;
			break;
		} else if ( ! (filled_spots[i] & PANEL_ORIENTATION_RIGHT)) {
			*orientation = PANEL_ORIENTATION_RIGHT;
			*monitor = i;
			found_a_spot = TRUE;
			break;
		} else if ( ! (filled_spots[i] & PANEL_ORIENTATION_LEFT)) {
			*orientation = PANEL_ORIENTATION_LEFT;
			*monitor = i;
			found_a_spot = TRUE;
			break;
		}
	}

	g_free (filled_spots);

	return found_a_spot;
}

static GdkScreen *
panel_toplevel_get_screen_geometry (PanelToplevel *toplevel,
				    int           *width,
				    int           *height)
{
	GdkScreen *screen;
	Screen *xscreen;

	g_return_val_if_fail (PANEL_IS_TOPLEVEL (toplevel), NULL);
	g_return_val_if_fail (width != NULL && height != NULL, NULL);

	screen = gtk_window_get_screen (GTK_WINDOW (toplevel));
	xscreen = gdk_x11_screen_get_xscreen (screen);

	*width = WidthOfScreen (xscreen);
	*height = HeightOfScreen (xscreen);

	return screen;
}

static GdkScreen *
panel_toplevel_get_monitor_geometry (PanelToplevel *toplevel,
				     int           *x,
				     int           *y,
				     int           *width,
				     int           *height)
{
	GdkScreen *screen;

	g_return_val_if_fail (PANEL_IS_TOPLEVEL (toplevel), NULL);
	g_return_val_if_fail (width != NULL && height != NULL, NULL);

	screen = gtk_window_get_screen (GTK_WINDOW (toplevel));

	if (x) *x = panel_multiscreen_x (screen, toplevel->priv->monitor);
	if (y) *y = panel_multiscreen_y (screen, toplevel->priv->monitor);

	if (width)  *width  = panel_multiscreen_width  (screen, toplevel->priv->monitor);
	if (height) *height = panel_multiscreen_height (screen, toplevel->priv->monitor);

	return screen;
}

static GdkCursorType
panel_toplevel_grab_op_cursor (PanelToplevel   *toplevel,
			       PanelGrabOpType  grab_op)
{
	GdkCursorType retval = -1;

	switch (grab_op) {
	case PANEL_GRAB_OP_MOVE:
	case PANEL_GRAB_OP_RESIZE:
		if (toplevel->priv->grab_is_keyboard)
			retval = GDK_CROSS;
		else
			retval = GDK_FLEUR;
		break;
		break;
	case PANEL_GRAB_OP_RESIZE_UP:
		retval = GDK_TOP_SIDE;
		break;
	case PANEL_GRAB_OP_RESIZE_DOWN:
		retval = GDK_BOTTOM_SIDE;
		break;
	case PANEL_GRAB_OP_RESIZE_LEFT:
		retval = GDK_LEFT_SIDE;
		break;
	case PANEL_GRAB_OP_RESIZE_RIGHT:
		retval = GDK_RIGHT_SIDE;
		break;
	case PANEL_GRAB_OP_NONE:
	default:
		g_assert_not_reached ();
		break;
	}

	return retval;
}

static void
panel_toplevel_init_resize_drag_offsets (PanelToplevel   *toplevel,
					 PanelGrabOpType  grab_op)
{
	toplevel->priv->drag_offset_x = 0;
	toplevel->priv->drag_offset_y = 0;

	switch (grab_op) {
	case PANEL_GRAB_OP_RESIZE_DOWN:
		toplevel->priv->drag_offset_y = toplevel->priv->geometry.y;
		break;
	case PANEL_GRAB_OP_RESIZE_UP:
		toplevel->priv->drag_offset_y =
			toplevel->priv->geometry.y + toplevel->priv->geometry.height;
		break;
	case PANEL_GRAB_OP_RESIZE_RIGHT:
		toplevel->priv->drag_offset_x = toplevel->priv->geometry.x;
		break;
	case PANEL_GRAB_OP_RESIZE_LEFT:
		toplevel->priv->drag_offset_x =
			toplevel->priv->geometry.x + toplevel->priv->geometry.width;
		break;
	case PANEL_GRAB_OP_MOVE:
	case PANEL_GRAB_OP_RESIZE:
	case PANEL_GRAB_OP_NONE:
	default:
		g_assert_not_reached ();
		break;
	}
}

static void
panel_toplevel_warp_pointer (PanelToplevel *toplevel)
{
	GtkWidget    *widget;
	GdkRectangle  geometry;
	int           x, y;

	widget = GTK_WIDGET (toplevel);

	geometry = toplevel->priv->geometry;

	x = y = 0;

	switch (toplevel->priv->grab_op) {
	case PANEL_GRAB_OP_MOVE:
	case PANEL_GRAB_OP_RESIZE:
		x = (geometry.width  / 2);
		y = (geometry.height / 2);
		break;
	case PANEL_GRAB_OP_RESIZE_UP:
		x = (geometry.width  / 2);
		break;
	case PANEL_GRAB_OP_RESIZE_DOWN:
		x = (geometry.width  / 2);
		y = geometry.height;
		break;
	case PANEL_GRAB_OP_RESIZE_LEFT:
		y = (geometry.height / 2);
		break;
	case PANEL_GRAB_OP_RESIZE_RIGHT:
		x = geometry.width;
		y = (geometry.height / 2);
		break;
	case PANEL_GRAB_OP_NONE:
	default:
		g_assert_not_reached ();
		break;
	}

	if (toplevel->priv->grab_op == PANEL_GRAB_OP_MOVE ||
	    toplevel->priv->grab_op == PANEL_GRAB_OP_RESIZE) {
		toplevel->priv->drag_offset_x = x;
		toplevel->priv->drag_offset_y = y;
	} else
		panel_toplevel_init_resize_drag_offsets (toplevel, toplevel->priv->grab_op);

	panel_warp_pointer (gtk_widget_get_window (widget), x, y);
}

static void
panel_toplevel_begin_grab_op (PanelToplevel   *toplevel,
			      PanelGrabOpType  op_type,
			      gboolean         grab_keyboard,
			      guint32          time_)
{
	GtkWidget     *widget;
	GdkWindow     *window;
	GdkCursorType  cursor_type;
	GdkCursor     *cursor;
	GdkDisplay    *display;
	GdkSeat *seat;
	GdkSeatCapabilities capabilities;

	if (toplevel->priv->state != PANEL_STATE_NORMAL ||
	    toplevel->priv->grab_op != PANEL_GRAB_OP_NONE)
		return;

	if (panel_lockdown_get_panels_locked_down_s ())
		return;

	/* If any of the position/orientation are not writable,
	   then we can't really move freely */
	if (op_type == PANEL_GRAB_OP_MOVE &&
	    !panel_toplevel_position_is_writable (toplevel))
		return;

	/* If size is not writable, then we can't resize */
	if ((op_type == PANEL_GRAB_OP_RESIZE || 
	     op_type == PANEL_GRAB_OP_RESIZE_UP || 
	     op_type == PANEL_GRAB_OP_RESIZE_DOWN || 
	     op_type == PANEL_GRAB_OP_RESIZE_LEFT || 
	     op_type == PANEL_GRAB_OP_RESIZE_RIGHT) &&
	    ! g_settings_is_writable (toplevel->priv->settings,
				      PANEL_TOPLEVEL_SIZE_KEY))
		return;

	widget = GTK_WIDGET (toplevel);
	window = gtk_widget_get_window (widget);

	toplevel->priv->grab_op          = op_type;
	toplevel->priv->grab_is_keyboard = grab_keyboard;

	toplevel->priv->orig_monitor     = toplevel->priv->monitor;
	toplevel->priv->orig_x           = toplevel->priv->x;
	toplevel->priv->orig_x_right     = toplevel->priv->x_right;
	toplevel->priv->orig_x_centered  = toplevel->priv->x_centered;
	toplevel->priv->orig_y           = toplevel->priv->y;
	toplevel->priv->orig_y_bottom    = toplevel->priv->y_bottom;
	toplevel->priv->orig_y_centered  = toplevel->priv->y_centered;
	toplevel->priv->orig_size        = toplevel->priv->size;
	toplevel->priv->orig_orientation = toplevel->priv->orientation;

	gtk_grab_add (widget);

	if (toplevel->priv->grab_is_keyboard)
		panel_toplevel_warp_pointer (toplevel);

	cursor_type = panel_toplevel_grab_op_cursor (
				toplevel, toplevel->priv->grab_op);

	cursor = gdk_cursor_new_for_display (gdk_display_get_default (),
	                                     cursor_type);

	display = gdk_window_get_display (window);
	seat = gdk_display_get_default_seat (display);

	capabilities = GDK_SEAT_CAPABILITY_POINTER;
	if (grab_keyboard)
		capabilities |= GDK_SEAT_CAPABILITY_KEYBOARD;

	gdk_seat_grab (seat, window, capabilities, FALSE, cursor,
	               NULL, NULL, NULL);

	g_object_unref (cursor);
}

static void
panel_toplevel_end_grab_op (PanelToplevel *toplevel,
			    guint32        time_)
{
	GtkWidget *widget;
	GdkDisplay *display;
	GdkSeat *seat;

	g_return_if_fail (toplevel->priv->grab_op != PANEL_GRAB_OP_NONE);

	widget = GTK_WIDGET (toplevel);

	toplevel->priv->grab_op          = PANEL_GRAB_OP_NONE;
	toplevel->priv->grab_is_keyboard = FALSE;

	gtk_grab_remove (widget);

	display = gtk_widget_get_display (widget);
	seat = gdk_display_get_default_seat (display);

	gdk_seat_ungrab (seat);
}

static void
panel_toplevel_cancel_grab_op (PanelToplevel *toplevel,
			       guint32        time_)
{
	panel_toplevel_set_orientation (toplevel, toplevel->priv->orig_orientation);
	panel_toplevel_set_monitor (toplevel, toplevel->priv->orig_monitor);
	panel_toplevel_set_size (toplevel, toplevel->priv->orig_size);
	panel_toplevel_set_x (toplevel,
			      toplevel->priv->orig_x,
			      toplevel->priv->orig_x_right,
			      toplevel->priv->orig_x_centered);
	panel_toplevel_set_y (toplevel,
			      toplevel->priv->orig_y,
			      toplevel->priv->orig_y_bottom,
			      toplevel->priv->orig_y_centered);
}

static void
panel_toplevel_resize_to_pointer (PanelToplevel *toplevel,
				  int            x,
				  int            y)
{
	int new_size;
	int new_x, new_y;
	int new_x_right, new_y_bottom;
	int new_x_centered, new_y_centered;
	int monitor_width, monitor_height;

	new_size       = toplevel->priv->size;
	new_x          = toplevel->priv->x;
	new_y          = toplevel->priv->y;
	new_x_right    = toplevel->priv->x_right;
	new_y_bottom   = toplevel->priv->y_bottom;
	new_x_centered = toplevel->priv->x_centered;
	new_y_centered = toplevel->priv->y_centered;

	panel_toplevel_get_monitor_geometry (toplevel, NULL, NULL, &monitor_width, &monitor_height);

	switch (toplevel->priv->grab_op) {
	case PANEL_GRAB_OP_RESIZE_UP:
		new_size = toplevel->priv->drag_offset_y - y;
		new_size = CLAMP (new_size, 0, monitor_height / 4);
		new_y -= (new_size - toplevel->priv->size);
		if (!toplevel->priv->y_centered && (new_y + new_size / 2) > monitor_height / 2)
			new_y_bottom = monitor_height - (new_y + new_size);
		else
			new_y_bottom = -1;
		break;
	case PANEL_GRAB_OP_RESIZE_DOWN:
		new_size = y - toplevel->priv->drag_offset_y;
		new_size = CLAMP (new_size, 0, monitor_height / 4);
		if (!toplevel->priv->y_centered && (new_y + new_size / 2) > monitor_height / 2)
			new_y_bottom = monitor_height - (new_y + new_size);
		else
			new_y_bottom = -1;
		break;
	case PANEL_GRAB_OP_RESIZE_LEFT:
		new_size = toplevel->priv->drag_offset_x - x;
		new_size = CLAMP (new_size, 0, monitor_width / 4);
		new_x -= (new_size - toplevel->priv->size);
		if (!toplevel->priv->x_centered && (new_x + new_size / 2) > monitor_width / 2)
			new_x_right = monitor_width - (new_x + new_size);
		else
			new_x_right = -1;
		break;
	case PANEL_GRAB_OP_RESIZE_RIGHT:
		new_size = x - toplevel->priv->drag_offset_x;
		new_size = CLAMP (new_size, 0, monitor_width / 4);
		if (!toplevel->priv->x_centered && (new_x + new_size / 2) > monitor_width / 2)
			new_x_right = monitor_width - (new_x + new_size);
		else
			new_x_right = -1;
		break;
	case PANEL_GRAB_OP_MOVE:
	case PANEL_GRAB_OP_RESIZE:
	case PANEL_GRAB_OP_NONE:
	default:
		g_assert_not_reached ();
		break;
	}

	if (new_size == 0)
		return;

	panel_toplevel_set_x (toplevel, new_x, new_x_right, new_x_centered);
	panel_toplevel_set_y (toplevel, new_y, new_y_bottom, new_y_centered);
	panel_toplevel_set_size (toplevel, new_size);
}

/* this is called for expanded panels that are dragged around */
static void
panel_toplevel_calc_new_orientation (PanelToplevel *toplevel,
				     int            pointer_x,
				     int            pointer_y)
{
	PanelOrientation  new_orientation;
	GdkScreen        *screen;
	int               hborder, vborder;
	int               monitor;
	int               monitor_width, monitor_height;
	int               new_x, new_y;

	screen = gtk_window_get_screen (GTK_WINDOW (toplevel));

	monitor = panel_multiscreen_get_monitor_at_point (pointer_x, pointer_y);

	if (toplevel->priv->geometry.height < toplevel->priv->geometry.width)
		vborder = hborder = (3 * toplevel->priv->geometry.height) >> 1;
	else
		vborder = hborder = (3 * toplevel->priv->geometry.width)  >> 1;

	new_x = pointer_x - panel_multiscreen_x (screen, monitor);
	new_y = pointer_y - panel_multiscreen_y (screen, monitor);
	monitor_width = panel_multiscreen_width (screen, monitor);
	monitor_height = panel_multiscreen_height (screen, monitor);

	new_orientation = toplevel->priv->orientation;

	switch (toplevel->priv->orientation) {
	case PANEL_ORIENTATION_TOP:
		if (new_y > (monitor_height - hborder))
			new_orientation = PANEL_ORIENTATION_BOTTOM;

		else if (new_y > hborder) {
			if (new_x > (monitor_width - vborder))
				new_orientation = PANEL_ORIENTATION_RIGHT;
			else if (new_x < vborder)
				new_orientation = PANEL_ORIENTATION_LEFT;
		} 
		break;
	case PANEL_ORIENTATION_BOTTOM:
		if (new_y < hborder)
			new_orientation = PANEL_ORIENTATION_TOP;

		else if (new_y < (monitor_height - hborder)) {
			if (new_x > (monitor_width - vborder))
				new_orientation = PANEL_ORIENTATION_RIGHT;
			else if (new_x < vborder)
				new_orientation = PANEL_ORIENTATION_LEFT;
		} 
		break;
	case PANEL_ORIENTATION_LEFT:
		if (new_x > (monitor_width - vborder))
			new_orientation = PANEL_ORIENTATION_RIGHT;

		else if (new_x > vborder) {
			if (new_y > (monitor_height - hborder))
				new_orientation = PANEL_ORIENTATION_BOTTOM;
			else if (new_y < hborder)
				new_orientation = PANEL_ORIENTATION_TOP;
		} 
		break;
	case PANEL_ORIENTATION_RIGHT:
		if (new_x < vborder)
			new_orientation = PANEL_ORIENTATION_LEFT;

		else if (new_x < (monitor_width - vborder)) {
			if (new_y > (monitor_height - hborder))
				new_orientation = PANEL_ORIENTATION_BOTTOM;
			else if (new_y < hborder)
				new_orientation = PANEL_ORIENTATION_TOP;
		}
		break;
	default:
		g_assert_not_reached ();
		break;
	}

	panel_toplevel_set_monitor (toplevel, monitor);
	panel_toplevel_set_orientation (toplevel, new_orientation);
}

static void
panel_toplevel_move_to (PanelToplevel *toplevel,
			int            new_x,
			int            new_y)
{
	GdkScreen        *screen;
	PanelOrientation  new_orientation;
	gboolean          x_centered, y_centered;
	int               screen_width, screen_height;
	int               monitor_width, monitor_height;
	int               width, height;
	int               new_monitor;
	int               x, y, x_right, y_bottom;
	int               snap_tolerance;

	screen = panel_toplevel_get_screen_geometry (
			toplevel, &screen_width, &screen_height);

	width  = toplevel->priv->geometry.width;
	height = toplevel->priv->geometry.height;

	snap_tolerance = toplevel->priv->snap_tolerance;

	new_x = CLAMP (new_x, 0, screen_width  - width);
	new_y = CLAMP (new_y, 0, screen_height - height);

	new_orientation = toplevel->priv->orientation;

	if (new_x <= snap_tolerance &&
	    toplevel->priv->orientation & PANEL_VERTICAL_MASK)
		new_orientation = PANEL_ORIENTATION_LEFT;

	else if ((new_x + width) >= (screen_width - snap_tolerance) &&
		 toplevel->priv->orientation & PANEL_VERTICAL_MASK)
		new_orientation = PANEL_ORIENTATION_RIGHT;

	if (new_y <= snap_tolerance &&
	    toplevel->priv->orientation & PANEL_HORIZONTAL_MASK)
		new_orientation = PANEL_ORIENTATION_TOP;

	else if ((new_y + height) >= (screen_height - snap_tolerance) &&
		 toplevel->priv->orientation & PANEL_HORIZONTAL_MASK)
		new_orientation = PANEL_ORIENTATION_BOTTOM;

	new_monitor = panel_multiscreen_get_monitor_at_point (new_x, new_y);

	panel_toplevel_get_monitor_geometry (
			toplevel, NULL, NULL, &monitor_width, &monitor_height);

	x_centered = toplevel->priv->x_centered;
	y_centered = toplevel->priv->y_centered;

	x = new_x - panel_multiscreen_x (screen, new_monitor);
	y = new_y - panel_multiscreen_y (screen, new_monitor);

	if (toplevel->priv->orientation & PANEL_HORIZONTAL_MASK) {
		y_centered = FALSE;
		if (new_y <= snap_tolerance ||
		    new_y + height >= screen_height - snap_tolerance)
			x_centered = abs (x - ((monitor_width - width) / 2))
								<= snap_tolerance;
		else
			x_centered = FALSE;
	} else {
		x_centered = FALSE;
		if (new_x <= snap_tolerance ||
		    new_x + width >= screen_width - snap_tolerance)
			y_centered = abs (y - ((monitor_height - height) / 2))
								<= snap_tolerance;
		else
			y_centered = FALSE;
	}

	if (x_centered)
		x = (monitor_width  - width) / 2;
	if (y_centered)
		y = (monitor_height - height) / 2;

	if (!x_centered && (x + width / 2) > monitor_width / 2)
		x_right = monitor_width - (x + width);
	else
		x_right = -1;

	if (!y_centered && (y + height / 2) > monitor_height / 2)
		y_bottom = monitor_height - (y + height);
	else
		y_bottom = -1;

	panel_toplevel_set_monitor (toplevel, new_monitor);
	panel_toplevel_set_orientation (toplevel, new_orientation);
	panel_toplevel_set_x (toplevel, x, x_right, x_centered);
	panel_toplevel_set_y (toplevel, y, y_bottom, y_centered);
}

static void
panel_toplevel_move_to_pointer (PanelToplevel *toplevel,
				int            pointer_x,
				int            pointer_y)
{
	int new_x, new_y;

	new_x = pointer_x - toplevel->priv->drag_offset_x;
	new_y = pointer_y - toplevel->priv->drag_offset_y;

	panel_toplevel_move_to (toplevel, new_x, new_y);
}

static void
panel_toplevel_rotate_to_pointer (PanelToplevel *toplevel,
				  int            pointer_x,
				  int            pointer_y)
{
	int        x_diff, y_diff;
	int        x, y;
	int        snap_tolerance;

	x = toplevel->priv->geometry.x;
	y = toplevel->priv->geometry.y;
	snap_tolerance = toplevel->priv->snap_tolerance;

	x_diff = pointer_x - (x + toplevel->priv->geometry.width / 2);
	y_diff = pointer_y - (y + toplevel->priv->geometry.height / 2);

	if (((-y_diff > x_diff + snap_tolerance) && x_diff > 0 && y_diff < 0) ||
	    (( y_diff < x_diff + snap_tolerance) && x_diff < 0 && y_diff < 0))
		panel_toplevel_set_orientation (toplevel, PANEL_ORIENTATION_RIGHT);

	else if (((-x_diff < y_diff - snap_tolerance) && x_diff > 0 && y_diff < 0) ||
	         (( x_diff > y_diff - snap_tolerance) && x_diff > 0 && y_diff > 0))
		panel_toplevel_set_orientation (toplevel, PANEL_ORIENTATION_BOTTOM);

	else if ((( y_diff > x_diff + snap_tolerance) && x_diff > 0 && y_diff > 0) ||
	         ((-y_diff < x_diff + snap_tolerance) && x_diff < 0 && y_diff > 0))
		panel_toplevel_set_orientation (toplevel, PANEL_ORIENTATION_LEFT);

	else if (((-x_diff > y_diff - snap_tolerance) && x_diff < 0 && y_diff > 0) ||
	         (( x_diff < y_diff - snap_tolerance) && x_diff < 0 && y_diff < 0))
		panel_toplevel_set_orientation (toplevel, PANEL_ORIENTATION_TOP);
}

static gboolean
panel_toplevel_warp_pointer_increment (PanelToplevel *toplevel,
				       int            keyval,
				       int            increment)
{
	GdkScreen 	*screen;
	GdkWindow 	*root_window;
	GdkDisplay      *display;
	GdkSeat         *seat;
	GdkDevice 	*device;
	int        	 new_x, new_y;

	screen = gtk_window_get_screen (GTK_WINDOW (toplevel));
	root_window = gdk_screen_get_root_window (screen);

	display = gtk_widget_get_display (GTK_WIDGET (toplevel));
	seat = gdk_display_get_default_seat (display);
	device = gdk_seat_get_pointer (seat);
	gdk_window_get_device_position(root_window, device, &new_x, &new_y, NULL);

	switch (keyval) {
	case GDK_KEY_Up:
	case GDK_KEY_KP_Up:
		new_y -= increment;
		break;
	case GDK_KEY_Left:
	case GDK_KEY_KP_Left:
		new_x -= increment;
		break;
	case GDK_KEY_Down:
	case GDK_KEY_KP_Down:
		new_y += increment;
		break;
	case GDK_KEY_Right:
	case GDK_KEY_KP_Right:
		new_x += increment;
		break;
	default:
		g_assert_not_reached ();
		return FALSE;
	}

	panel_warp_pointer (root_window, new_x, new_y);

	return TRUE;
}

static gboolean
panel_toplevel_move_keyboard_floating (PanelToplevel *toplevel,
				       GdkEventKey   *event)
{
#define SMALL_INCREMENT  1
#define NORMAL_INCREMENT 10

	int increment = NORMAL_INCREMENT;

	if ((event->state & gtk_accelerator_get_default_mod_mask ()) == GDK_SHIFT_MASK)
		increment = SMALL_INCREMENT;

	return panel_toplevel_warp_pointer_increment (
				toplevel, event->keyval, increment);

#undef SMALL_INCREMENT
#undef NORMAL_INCREMENT
}

static gboolean
panel_toplevel_move_keyboard_expanded (PanelToplevel *toplevel,
				       GdkEventKey   *event)
{
	PanelOrientation new_orientation;

	switch (event->keyval) {
	case GDK_KEY_Up:
	case GDK_KEY_KP_Up:
		new_orientation = PANEL_ORIENTATION_TOP;
		break;
	case GDK_KEY_Left:
	case GDK_KEY_KP_Left:
		new_orientation = PANEL_ORIENTATION_LEFT;
		break;
	case GDK_KEY_Down:
	case GDK_KEY_KP_Down:
		new_orientation = PANEL_ORIENTATION_BOTTOM;
		break;
	case GDK_KEY_Right:
	case GDK_KEY_KP_Right:
		new_orientation = PANEL_ORIENTATION_RIGHT;
		break;
	default:
		g_assert_not_reached ();
		return FALSE;
	}

	panel_toplevel_set_orientation (toplevel, new_orientation);

	return TRUE;
}

static gboolean
panel_toplevel_initial_resize_keypress (PanelToplevel *toplevel,
					GdkEventKey   *event)
{
	PanelGrabOpType grab_op;

	switch (event->keyval) {
	case GDK_KEY_Up:
	case GDK_KEY_KP_Up:
		if (!(toplevel->priv->orientation & PANEL_HORIZONTAL_MASK))
			return FALSE;
		grab_op = PANEL_GRAB_OP_RESIZE_UP;
		break;
	case GDK_KEY_Left:
	case GDK_KEY_KP_Left:
		if (!(toplevel->priv->orientation & PANEL_VERTICAL_MASK))
			return FALSE;
		grab_op = PANEL_GRAB_OP_RESIZE_LEFT;
		break;
	case GDK_KEY_Down:
	case GDK_KEY_KP_Down:
		if (!(toplevel->priv->orientation & PANEL_HORIZONTAL_MASK))
			return FALSE;
		grab_op = PANEL_GRAB_OP_RESIZE_DOWN;
		break;
	case GDK_KEY_Right:
	case GDK_KEY_KP_Right:
		if (!(toplevel->priv->orientation & PANEL_VERTICAL_MASK))
			return FALSE;
		grab_op = PANEL_GRAB_OP_RESIZE_RIGHT;
		break;
	default:
		g_assert_not_reached ();
		return FALSE;
	}

	panel_toplevel_end_grab_op (toplevel, event->time);
	panel_toplevel_begin_grab_op (toplevel, grab_op, TRUE, event->time);

	return TRUE;
}

static gboolean
panel_toplevel_handle_grab_op_key_event (PanelToplevel *toplevel,
					 GdkEventKey   *event)
{
	gboolean retval = FALSE;

	switch (event->keyval) {
	case GDK_KEY_Up:
	case GDK_KEY_KP_Up:
	case GDK_KEY_Left:
	case GDK_KEY_KP_Left:
	case GDK_KEY_Down:
	case GDK_KEY_KP_Down:
	case GDK_KEY_Right:
	case GDK_KEY_KP_Right:
		switch (toplevel->priv->grab_op) {
		case PANEL_GRAB_OP_MOVE:
			if (toplevel->priv->expand)
				retval = panel_toplevel_move_keyboard_expanded (
									toplevel, event);
			else
				retval = panel_toplevel_move_keyboard_floating (
									toplevel, event);
			break;
		case PANEL_GRAB_OP_RESIZE:
			retval = panel_toplevel_initial_resize_keypress (toplevel, event);
			break;
		case PANEL_GRAB_OP_RESIZE_UP:
		case PANEL_GRAB_OP_RESIZE_DOWN:
		case PANEL_GRAB_OP_RESIZE_LEFT:
		case PANEL_GRAB_OP_RESIZE_RIGHT:
			retval = panel_toplevel_warp_pointer_increment (
						toplevel, event->keyval, 1);
			break;
		case PANEL_GRAB_OP_NONE:
		default:
			g_assert_not_reached ();
			break;
		}
		break;
	case GDK_KEY_Escape:
		panel_toplevel_cancel_grab_op (toplevel, event->time);
		panel_toplevel_end_grab_op (toplevel, event->time);
		retval = TRUE;
		break;
	case GDK_KEY_Return:
	case GDK_KEY_KP_Enter:
	case GDK_KEY_space:
	case GDK_KEY_KP_Space:
	default:
		panel_toplevel_end_grab_op (toplevel, event->time);
		retval = TRUE;
		break;
	}

	return retval;
}

static gboolean
panel_toplevel_handle_grab_op_motion_event (PanelToplevel  *toplevel,
					    GdkEventMotion *event)
{
	switch (toplevel->priv->grab_op) {
	case PANEL_GRAB_OP_MOVE:
		if (toplevel->priv->expand)
			panel_toplevel_calc_new_orientation (
					toplevel, event->x_root, event->y_root);

		else if ((event->state & gtk_accelerator_get_default_mod_mask ()) == GDK_CONTROL_MASK)
			panel_toplevel_rotate_to_pointer (
					toplevel, event->x_root, event->y_root);

		else
			panel_toplevel_move_to_pointer (
					toplevel, event->x_root, event->y_root);
		return TRUE;
	case PANEL_GRAB_OP_RESIZE_UP:
	case PANEL_GRAB_OP_RESIZE_DOWN:
	case PANEL_GRAB_OP_RESIZE_LEFT:
	case PANEL_GRAB_OP_RESIZE_RIGHT:
		panel_toplevel_resize_to_pointer (toplevel, event->x_root, event->y_root);
		return TRUE;
	case PANEL_GRAB_OP_NONE:
	case PANEL_GRAB_OP_RESIZE:
	default:
		break;
	}

	return FALSE;
}

static void
panel_toplevel_calc_floating (PanelToplevel *toplevel)
{
	int        screen_width, screen_height;
	int        monitor_x, monitor_y;
	int        monitor_width, monitor_height;
	int        x, y;
	int        snap_tolerance;

	if (toplevel->priv->expand) {
		toplevel->priv->floating = FALSE;
		return;
	}

	panel_toplevel_get_screen_geometry (toplevel,
					    &screen_width, &screen_height);
	panel_toplevel_get_monitor_geometry (toplevel, &monitor_x, &monitor_y,
					     &monitor_width, &monitor_height);

	if (toplevel->priv->x_right == -1)
		x = monitor_x + toplevel->priv->x;
	else
		x = monitor_x + (monitor_width - (toplevel->priv->x_right + toplevel->priv->geometry.width));
	if (toplevel->priv->y_bottom == -1)
		y = monitor_y + toplevel->priv->y;
	else
		y = monitor_y + (monitor_height - (toplevel->priv->y_bottom + toplevel->priv->geometry.height));

	snap_tolerance = toplevel->priv->snap_tolerance;

	//FIXME? everywhere else, snap_tolerance is relative to the monitor,
	//not the screen
	if (toplevel->priv->orientation & PANEL_HORIZONTAL_MASK)
		toplevel->priv->floating =
			(y > snap_tolerance) && (y < (screen_height - toplevel->priv->geometry.height - snap_tolerance));
	else
		toplevel->priv->floating =
			(x > snap_tolerance) && (x < (screen_width - toplevel->priv->geometry.width - snap_tolerance));
}

void 
panel_toplevel_push_autohide_disabler (PanelToplevel *toplevel)
{
	g_return_if_fail (toplevel != NULL);

	if (!toplevel->priv->n_autohide_disablers++)
		panel_toplevel_queue_auto_hide (toplevel);
}

void
panel_toplevel_pop_autohide_disabler (PanelToplevel *toplevel)
{
	g_return_if_fail (toplevel != NULL);
	g_return_if_fail (toplevel->priv->n_autohide_disablers > 0);

	if (!--toplevel->priv->n_autohide_disablers)
		panel_toplevel_queue_auto_hide (toplevel);
}

static gboolean
panel_toplevel_get_autohide_disabled (PanelToplevel *toplevel)
{
	return toplevel->priv->n_autohide_disablers > 0 ? TRUE : FALSE;
}

static gboolean
panel_toplevel_hide_button_event (PanelToplevel  *toplevel,
				  GdkEventButton *event,
				  GtkButton      *button)
{
	if (event->button == 1)
		return FALSE;

	return gtk_widget_event (GTK_WIDGET (toplevel), (GdkEvent *) event);
}

static void
panel_toplevel_hide_button_clicked (PanelToplevel *toplevel,
				    GtkButton     *button)
{
	GtkArrowType arrow_type;
	gboolean ltr;

	if (toplevel->priv->animating ||
	    toplevel->priv->state == PANEL_STATE_AUTO_HIDDEN)
		return;

	ltr = gtk_widget_get_direction (GTK_WIDGET (toplevel)) == GTK_TEXT_DIR_LTR;
	arrow_type = gp_arrow_button_get_arrow_type (GP_ARROW_BUTTON (button));

	if (toplevel->priv->state == PANEL_STATE_NORMAL) {
		GtkDirectionType direction;

		switch (arrow_type) {
		case GTK_ARROW_UP:
			direction = GTK_DIR_UP;
			break;
		case GTK_ARROW_DOWN:
			direction = GTK_DIR_DOWN;
			break;
		case GTK_ARROW_LEFT:
			direction = ltr ? GTK_DIR_LEFT : GTK_DIR_RIGHT;
			break;
		case GTK_ARROW_RIGHT:
			direction = ltr ? GTK_DIR_RIGHT : GTK_DIR_LEFT;
			break;
		case GTK_ARROW_NONE:
		default:
			g_assert_not_reached ();
			break;
		}

		panel_toplevel_hide (toplevel, FALSE, direction);
	} else
		panel_toplevel_unhide (toplevel);
}

static GtkWidget *
panel_toplevel_add_hide_button (PanelToplevel *toplevel,
				GtkArrowType   arrow_type,
				int            left,
				int            top)
{
	GtkWidget *button;

	button = gp_arrow_button_new (arrow_type);

	g_signal_connect_swapped (button, "clicked",
				  G_CALLBACK (panel_toplevel_hide_button_clicked), toplevel);
	g_signal_connect_swapped (button, "button_press_event",
				  G_CALLBACK (panel_toplevel_hide_button_event), toplevel);
	g_signal_connect_swapped (button, "button_release_event",
				  G_CALLBACK (panel_toplevel_hide_button_event), toplevel);

	gtk_grid_attach (GTK_GRID (toplevel->priv->grid), button, left, top, 1, 1);

	return button;
}

static void
panel_toplevel_update_buttons_showing (PanelToplevel *toplevel)
{
	if (toplevel->priv->orientation & PANEL_HORIZONTAL_MASK) {
		gtk_widget_hide (toplevel->priv->hide_button_top);
		gtk_widget_hide (toplevel->priv->hide_button_bottom);
		gtk_widget_show (toplevel->priv->hide_button_left);
		gtk_widget_show (toplevel->priv->hide_button_right);
	} else {
		gtk_widget_show (toplevel->priv->hide_button_top);
		gtk_widget_show (toplevel->priv->hide_button_bottom);
		gtk_widget_hide (toplevel->priv->hide_button_left);
		gtk_widget_hide (toplevel->priv->hide_button_right);
	}
}

static void
panel_toplevel_update_hide_buttons (PanelToplevel *toplevel)
{
	if (toplevel->priv->buttons_enabled)
		panel_toplevel_update_buttons_showing (toplevel);
	else {
		g_object_set (
			G_OBJECT (toplevel->priv->hide_button_top),
			"visible", toplevel->priv->state == PANEL_STATE_HIDDEN_DOWN,
			NULL);
		g_object_set (
			G_OBJECT (toplevel->priv->hide_button_bottom),
			"visible", toplevel->priv->state == PANEL_STATE_HIDDEN_UP,
			NULL);
		g_object_set (
			G_OBJECT (toplevel->priv->hide_button_left),
			"visible", toplevel->priv->state == PANEL_STATE_HIDDEN_RIGHT,
			NULL);
		g_object_set (
			G_OBJECT (toplevel->priv->hide_button_right),
			"visible", toplevel->priv->state == PANEL_STATE_HIDDEN_LEFT,
			NULL);
	}

	if (toplevel->priv->arrows_enabled) {
		gtk_widget_show (gtk_bin_get_child (GTK_BIN (toplevel->priv->hide_button_top)));
		gtk_widget_show (gtk_bin_get_child (GTK_BIN (toplevel->priv->hide_button_bottom)));
		gtk_widget_show (gtk_bin_get_child (GTK_BIN (toplevel->priv->hide_button_left)));
		gtk_widget_show (gtk_bin_get_child (GTK_BIN (toplevel->priv->hide_button_right)));
	} else {
		gtk_widget_hide (gtk_bin_get_child (GTK_BIN (toplevel->priv->hide_button_top)));
		gtk_widget_hide (gtk_bin_get_child (GTK_BIN (toplevel->priv->hide_button_bottom)));
		gtk_widget_hide (gtk_bin_get_child (GTK_BIN (toplevel->priv->hide_button_left)));
		gtk_widget_hide (gtk_bin_get_child (GTK_BIN (toplevel->priv->hide_button_right)));
	}
}

static gboolean
panel_toplevel_contains_pointer (PanelToplevel *toplevel)
{
	GdkDisplay *display;
	GdkScreen  *screen;
	GtkWidget  *widget;
	GdkSeat    *seat;
	GdkDevice  *pointer;
	int         x, y;

	widget = GTK_WIDGET (toplevel);

	if (!gtk_widget_get_realized (widget))
		return FALSE;

	display = gdk_display_get_default ();
	seat = gdk_display_get_default_seat (display);
	pointer = gdk_seat_get_pointer (seat);

	screen = NULL;
	x = y = -1;
	gdk_device_get_position (pointer, &screen, &x, &y);

	if (screen != gtk_window_get_screen (GTK_WINDOW (toplevel)))
		return FALSE;

	if (x == -1 || y == -1)
		return FALSE;

	if (x < toplevel->priv->geometry.x || x >= (toplevel->priv->geometry.x + toplevel->priv->geometry.width) ||
	    y < toplevel->priv->geometry.y || y >= (toplevel->priv->geometry.y + toplevel->priv->geometry.height))
		return FALSE;

	return TRUE;
}

static inline int
panel_toplevel_get_effective_auto_hide_size (PanelToplevel *toplevel)
{
	int size;

	if (toplevel->priv->orientation & PANEL_HORIZONTAL_MASK)
		size = CLAMP (toplevel->priv->auto_hide_size,
			      1, toplevel->priv->original_height / 2);
	else
		size = CLAMP (toplevel->priv->auto_hide_size,
			      1, toplevel->priv->original_width / 2);

	/* paranoia */
	return (size <= 0) ? DEFAULT_AUTO_HIDE_SIZE : size;
}

static gboolean
panel_toplevel_update_struts (PanelToplevel *toplevel, gboolean end_of_animation)
{
	PanelOrientation  orientation;
	GdkScreen        *screen;
	gboolean          geometry_changed = FALSE;
	int               strut, strut_start, strut_end;
	int               x, y, width, height;
	int               monitor_x, monitor_y;
	int               monitor_width, monitor_height;

	if (!toplevel->priv->updated_geometry_initial)
		return FALSE;

	/* In the case of the initial animation, we really want the struts to
	 * represent what is at the end of the animation, to avoid desktop
	 * icons jumping around. */
	if (!toplevel->priv->initial_animation_done) {
		end_of_animation = TRUE;

		/* We've not started the animation yet, so we have to compute
		 * where we want to end. Note that we don't want to compute
		 * this everytime, since the struts conflict resolution will be
		 * overridden if we do so */
		if (!toplevel->priv->animating)
			panel_toplevel_calculate_animation_end_geometry (toplevel);
	}

	screen = panel_toplevel_get_monitor_geometry (toplevel,
						      &monitor_x,
						      &monitor_y,
						      &monitor_width,
						      &monitor_height);

	if (end_of_animation) {
		x = toplevel->priv->animation_end_x;
		y = toplevel->priv->animation_end_y;
		x += panel_multiscreen_x (screen, toplevel->priv->monitor);
		y += panel_multiscreen_y (screen, toplevel->priv->monitor);
		width = toplevel->priv->geometry.width;
		height = toplevel->priv->geometry.height;
	} else {
		x      = toplevel->priv->geometry.x;
		y      = toplevel->priv->geometry.y;
		width  = toplevel->priv->geometry.width;
		height = toplevel->priv->geometry.height;
	}

	orientation = toplevel->priv->orientation;

	strut = strut_start = strut_end = 0;

	if (orientation & PANEL_HORIZONTAL_MASK) {
		if (y <= monitor_y) {
			orientation = PANEL_ORIENTATION_TOP;
			strut = y + height - monitor_y;
		} else if (y >= monitor_y + monitor_height - height) {
			orientation = PANEL_ORIENTATION_BOTTOM;
			strut = monitor_y + monitor_height - y;
		}

		if (strut > 0) {
			strut_start = MAX (x, monitor_x);
			strut_end = MIN (x + width, monitor_x + monitor_width) - 1;
		}
	} else {
		if (x <= monitor_x) {
			orientation = PANEL_ORIENTATION_LEFT;
			strut = x + width - monitor_x;
		} else if (x >= monitor_x + monitor_width - width) {
			orientation = PANEL_ORIENTATION_RIGHT;
			strut = monitor_x + monitor_width - x;
		}

		if (strut > 0) {
			strut_start = MAX (y, monitor_y);
			strut_end = MIN (y + height, monitor_y + monitor_height) - 1;
		}
	}

	if (orientation != toplevel->priv->orientation) {
		toplevel->priv->orientation = orientation;
		g_object_notify (G_OBJECT (toplevel), "orientation");
	}

	if (toplevel->priv->auto_hide && strut > 0)
		strut = panel_toplevel_get_effective_auto_hide_size (toplevel);

	if (strut > 0)
		geometry_changed = panel_struts_register_strut (toplevel,
								toplevel->priv->monitor,
								orientation,
								strut,
								strut_start,
								strut_end);
	else
		panel_struts_unregister_strut (toplevel);

	if (toplevel->priv->state == PANEL_STATE_NORMAL ||
	    toplevel->priv->state == PANEL_STATE_AUTO_HIDDEN ||
	    toplevel->priv->animating)
		panel_struts_set_window_hint (toplevel);
	else
		panel_struts_unset_window_hint (toplevel);

	return geometry_changed;
}

static const char *
panel_toplevel_construct_description (PanelToplevel *toplevel)
{
	int orientation, type;

	static const char *description[4][4] = {
		{
	/* translators: these string will be shown in MetaCity's switch window
	 * popup when you pass the focus to a panel */
			N_("Top Expanded Edge Panel"),
			N_("Top Centered Panel"),
	     		N_("Top Floating Panel"),
	     		N_("Top Edge Panel"),
		},
		
		{
			N_("Bottom Expanded Edge Panel"),
	     		N_("Bottom Centered Panel"),
	     		N_("Bottom Floating Panel"),
	     		N_("Bottom Edge Panel"),
		},

		{
			N_("Left Expanded Edge Panel"),
			N_("Left Centered Panel"),
			N_("Left Floating Panel"),
			N_("Left Edge Panel"),
		},

		{
			N_("Right Expanded Edge Panel"),
			N_("Right Centered Panel"),
			N_("Right Floating Panel"),
			N_("Right Edge Panel"),
		},
	};

	switch (toplevel->priv->orientation) {
	case PANEL_ORIENTATION_TOP:
		orientation = 0;
		break;
	case PANEL_ORIENTATION_BOTTOM:
		orientation = 1;
		break;
	case PANEL_ORIENTATION_LEFT:
		orientation = 2;
		break;
	case PANEL_ORIENTATION_RIGHT:
		orientation = 3;
		break;
	default:
		orientation = 0;
		g_assert_not_reached ();
		break;
	}

	if (toplevel->priv->expand)
		type = 0;
	else if (toplevel->priv->x_centered ||
		 toplevel->priv->y_centered)
		type = 1;
	else if (toplevel->priv->floating)
		type = 2;
	else
		type = 3;
	
	return description[orientation][type];
}

static void
panel_toplevel_update_description (PanelToplevel *toplevel)
{
	const char *description;

	description = panel_toplevel_construct_description (toplevel);

	if (toplevel->priv->description &&
	    !strcmp (toplevel->priv->description, description))
		return;

	if (toplevel->priv->description)
		g_free (toplevel->priv->description);
	toplevel->priv->description = g_strdup (_(description));

	if (!toplevel->priv->name)
		gtk_window_set_title (GTK_WINDOW (toplevel),
				      toplevel->priv->description);

	panel_a11y_set_atk_name_desc (
		GTK_WIDGET (toplevel->priv->panel_widget),
		toplevel->priv->name ? toplevel->priv->name :
				       _(toplevel->priv->description),
		_(toplevel->priv->description));
}

static void
panel_toplevel_update_normal_position (PanelToplevel *toplevel,
				       int           *x,
				       int           *y)
{
	int        monitor_width, monitor_height;
	int        width, height;
	int        snap_tolerance;

	g_assert (x != NULL && y != NULL);

	panel_toplevel_get_monitor_geometry (
			toplevel, NULL, NULL, &monitor_width, &monitor_height);

	width  = toplevel->priv->original_width;
	height = toplevel->priv->original_height;
	snap_tolerance = toplevel->priv->snap_tolerance;

	*x = CLAMP (*x, 0, monitor_width  - width);
	*y = CLAMP (*y, 0, monitor_height - height);

	if (toplevel->priv->x <= snap_tolerance &&
	    toplevel->priv->x_right == -1 &&
	    !toplevel->priv->x_centered)
		*x = 0;
	else if (toplevel->priv->x_right != -1 &&
		 toplevel->priv->x_right <= snap_tolerance &&
		 !toplevel->priv->x_centered)
		*x = monitor_width - width;

	if (toplevel->priv->y <= snap_tolerance &&
	    toplevel->priv->y_bottom == -1 &&
	    !toplevel->priv->y_centered)
		*y = 0;
	else if (toplevel->priv->y_bottom != -1 &&
		 toplevel->priv->y_bottom <= snap_tolerance &&
		 !toplevel->priv->y_centered)
		*y = monitor_height - height;
}

static void
panel_toplevel_update_auto_hide_position (PanelToplevel *toplevel,
					  int           *x,
					  int           *y,
					  gboolean       for_end_position)
{
	int width, height;
	int monitor_width, monitor_height;
	int auto_hide_size;
	int snap_tolerance;

	g_assert (x != NULL && y != NULL);

	if (toplevel->priv->floating) {
		panel_toplevel_update_normal_position (toplevel, x, y);
		return;
	}

	panel_toplevel_get_monitor_geometry (
			toplevel, NULL, NULL, &monitor_width, &monitor_height);

	width  = toplevel->priv->original_width;
	height = toplevel->priv->original_height;
	snap_tolerance = toplevel->priv->snap_tolerance;

	/* For the initial animation, we animate from outside the screen, and
	 * so we don't want the toplevel to be visible at all. But when the
	 * request is for the end position, then we give the real result (it's
	 * useful for struts) */
	if (for_end_position || toplevel->priv->initial_animation_done) {
		auto_hide_size = panel_toplevel_get_effective_auto_hide_size (toplevel);
	} else {
		auto_hide_size = 0;
	}

	switch (toplevel->priv->orientation) {
	case PANEL_ORIENTATION_TOP:
		*y = - (height - auto_hide_size);
		break;
	case PANEL_ORIENTATION_BOTTOM:
		*y = monitor_height - auto_hide_size;
		break;
	case PANEL_ORIENTATION_LEFT:
		*x = - (width - auto_hide_size);
		break;
	case PANEL_ORIENTATION_RIGHT:
		*x = monitor_width - auto_hide_size;
		break;
	default:
		g_assert_not_reached ();
		break;
	}

	if (toplevel->priv->orientation & PANEL_HORIZONTAL_MASK) {
		if (toplevel->priv->x <= snap_tolerance &&
		    toplevel->priv->x_right == -1 &&
		    !toplevel->priv->x_centered)
			*x = 0;
		else if (toplevel->priv->x_right != -1 &&
			 toplevel->priv->x_right <= snap_tolerance &&
			 !toplevel->priv->x_centered)
			*x = monitor_width - width;
	} else /* if (toplevel->priv->orientation & PANEL_VERTICAL_MASK) */ {
		if (toplevel->priv->y <= snap_tolerance &&
		    toplevel->priv->y_bottom == -1 &&
		    !toplevel->priv->y_centered)
			*y = 0;
		else if (toplevel->priv->y_bottom != -1 &&
			 toplevel->priv->y_bottom <= snap_tolerance &&
			 !toplevel->priv->y_centered)
			*y = monitor_height - height;
	}
}

/* FIXME: this is wrong for Xinerama. In the Xinerama case
 *        I think if hiding it requires it to go onto the
 *        next monitor then it should just move it on to
 *        the next monitor and set its state back to normal
 */
static void
panel_toplevel_update_hidden_position (PanelToplevel *toplevel,
				       int           *x,
				       int           *y)
{
	int width, height;
	int min_hide_size;
	int monitor_height, monitor_width;
	GtkAllocation hide_allocation;

	g_assert (x != NULL && y != NULL);

	g_assert (toplevel->priv->state == PANEL_STATE_HIDDEN_UP   ||
		  toplevel->priv->state == PANEL_STATE_HIDDEN_DOWN ||
		  toplevel->priv->state == PANEL_STATE_HIDDEN_LEFT ||
		  toplevel->priv->state == PANEL_STATE_HIDDEN_RIGHT);

	panel_toplevel_get_monitor_geometry (
			toplevel, NULL, NULL, &monitor_width, &monitor_height);

	width  = toplevel->priv->original_width;
	height = toplevel->priv->original_height;

	//FIXME should find a better default
	min_hide_size = DEFAULT_AUTO_HIDE_SIZE;

	switch (toplevel->priv->state) {
	case PANEL_STATE_HIDDEN_UP:
		gtk_widget_get_allocation (toplevel->priv->hide_button_bottom,
					   &hide_allocation);
		*y = - (height - MAX (hide_allocation.height, min_hide_size));
		break;
	case PANEL_STATE_HIDDEN_DOWN:
		gtk_widget_get_allocation (toplevel->priv->hide_button_top,
					   &hide_allocation);
		*y = monitor_height - MAX (hide_allocation.height, min_hide_size);
		break;
	case PANEL_STATE_HIDDEN_LEFT:
		gtk_widget_get_allocation (toplevel->priv->hide_button_right,
					   &hide_allocation);
		*x = - (width - MAX (hide_allocation.width, min_hide_size));
		break;
	case PANEL_STATE_HIDDEN_RIGHT:
		gtk_widget_get_allocation (toplevel->priv->hide_button_left,
					   &hide_allocation);
		*x = monitor_width - MAX (hide_allocation.width, min_hide_size);
		break;
	case PANEL_STATE_NORMAL:
	case PANEL_STATE_AUTO_HIDDEN:
	default:
		g_assert_not_reached ();
		break;
	}
}

/*
 * This is "almost" like the double sine movement
 * from the original panel except that it uses
 * a cubic (twice again).  I suppose it looks less
 * mathematical now :) -- _v_
 */
static int
get_delta (int    src,
           int    dest,
           gint64 start_time,
           gint64 end_time,
           gint64 cur_time)
{
	double x, n, d, percentage;

	n = cur_time - start_time;
	d = end_time - start_time;

	if (abs (dest - src) <= 1 || n >= d)
		return dest - src;

	/* The cubic is: p(x) = (-2) x^2 (x-1.5) */
	/* running p(p(x)) to make it more "pronounced",
	 * effectively making it a ninth-degree polynomial */

	x = (double)n/d;
	x = -2 * (x*x) * (x-1.5);
	/* run it again */
	percentage = -2 * (x*x) * (x-1.5);

	percentage = CLAMP (percentage, 0.0, 1.0);

	return ((dest - src) * percentage);
}

static void
panel_toplevel_update_animating_position (PanelToplevel *toplevel)
{
	GdkScreen *screen;
	gint64     time_val;
	int        deltax, deltay;
	int        monitor_offset_x, monitor_offset_y;

	time_val = g_get_real_time ();

	screen = gtk_window_get_screen (GTK_WINDOW (toplevel));

	monitor_offset_x = panel_multiscreen_x (screen, toplevel->priv->monitor);
	monitor_offset_y = panel_multiscreen_y (screen, toplevel->priv->monitor);

	deltax = get_delta (toplevel->priv->geometry.x - monitor_offset_x,
			    toplevel->priv->animation_end_x,
			    toplevel->priv->animation_start_time,
			    toplevel->priv->animation_end_time,
			    time_val);

	deltay = get_delta (toplevel->priv->geometry.y - monitor_offset_y,
			    toplevel->priv->animation_end_y,
			    toplevel->priv->animation_start_time,
			    toplevel->priv->animation_end_time,
			    time_val);

	toplevel->priv->geometry.x += deltax;
	toplevel->priv->geometry.y += deltay;

	if (toplevel->priv->geometry.x - monitor_offset_x == toplevel->priv->animation_end_x &&
	    toplevel->priv->geometry.y - monitor_offset_y == toplevel->priv->animation_end_y) {
		toplevel->priv->animating = FALSE;
		/* Note: it's important to set initial_animation_done to TRUE
		 * as soon as possible (hence, here) since we don't want to
		 * have a wrong value in a size request event */
		toplevel->priv->initial_animation_done = TRUE;

		gtk_widget_queue_resize (GTK_WIDGET (toplevel));

		if (toplevel->priv->state == PANEL_STATE_NORMAL)
			g_signal_emit (toplevel, toplevel_signals [UNHIDE_SIGNAL], 0);
	}
}

static void
panel_toplevel_update_expanded_position (PanelToplevel *toplevel)
{
	GdkScreen *screen;
	int        monitor_width, monitor_height;
	int        monitor_x, monitor_y;
	int        x, y;
	int        x_right, y_bottom;
	int        monitor;

	if (!toplevel->priv->expand)
		return;

	screen = gtk_window_get_screen (GTK_WINDOW (toplevel));

	panel_toplevel_get_monitor_geometry (toplevel, &monitor_x, &monitor_y,
					     &monitor_width, &monitor_height);

	x = -1;
	y = -1;
	x_right = -1;
	y_bottom = -1;

	switch (toplevel->priv->orientation) {
	case PANEL_ORIENTATION_TOP:
		x = monitor_x;
		y = monitor_y;
		break;
	case PANEL_ORIENTATION_LEFT:
		x = monitor_x;
		y = monitor_y;
		break;
	case PANEL_ORIENTATION_BOTTOM:
		x = monitor_x;
		y = monitor_y + monitor_height - toplevel->priv->geometry.height;
		y_bottom = 0;
		break;
	case PANEL_ORIENTATION_RIGHT:
		x = monitor_x + monitor_width - toplevel->priv->geometry.width;
		y = monitor_y;
		x_right = 0;
		break;
	default:
		g_assert_not_reached ();
		break;
	}

	monitor = panel_multiscreen_get_monitor_at_point (x, y);

	panel_toplevel_set_monitor_internal (toplevel, monitor, TRUE);

	x -= panel_multiscreen_x (screen, monitor);
	y -= panel_multiscreen_y (screen, monitor);

	g_object_freeze_notify (G_OBJECT (toplevel));

	if (toplevel->priv->x != x) {
		toplevel->priv->x = x;
		g_object_notify (G_OBJECT (toplevel), "x");
	}

	if (toplevel->priv->y != y) {
		toplevel->priv->y = y;
		g_object_notify (G_OBJECT (toplevel), "y");
	}

	if (toplevel->priv->x_right != x_right) {
		toplevel->priv->x_right = x_right;
		g_object_notify (G_OBJECT (toplevel), "x_right");
	}

	if (toplevel->priv->y_bottom != y_bottom) {
		toplevel->priv->y_bottom = y_bottom;
		g_object_notify (G_OBJECT (toplevel), "y_bottom");
	}

	g_object_thaw_notify (G_OBJECT (toplevel));
}

static void
panel_toplevel_update_position (PanelToplevel *toplevel)
{
	int        x, y;
	int        monitor_width, monitor_height;
	GdkScreen *screen;

	panel_toplevel_get_monitor_geometry (
			toplevel, NULL, NULL, &monitor_width, &monitor_height);

	if (toplevel->priv->animating) {
		panel_toplevel_update_animating_position (toplevel);
		return;
	}

	if (toplevel->priv->position_centered) {
		toplevel->priv->position_centered = FALSE;

		g_object_freeze_notify (G_OBJECT (toplevel));

		if (!toplevel->priv->x_centered) {
			int x_right;

			toplevel->priv->x -= toplevel->priv->geometry.width  / 2;
			g_object_notify (G_OBJECT (toplevel), "x");

			if ((toplevel->priv->x + toplevel->priv->geometry.width / 2) > monitor_width / 2)
				x_right = monitor_width - (toplevel->priv->x + toplevel->priv->geometry.width);
			else
				x_right = -1;
			if (toplevel->priv->x_right != x_right) {
				toplevel->priv->x_right = x_right;
				g_object_notify (G_OBJECT (toplevel),
						 "x-right");
			}
		}

		if (!toplevel->priv->y_centered) {
			int y_bottom;

			toplevel->priv->y -= toplevel->priv->geometry.height / 2;
			g_object_notify (G_OBJECT (toplevel), "y");

			if ((toplevel->priv->y + toplevel->priv->geometry.height / 2) > monitor_height / 2)
				y_bottom = monitor_height - (toplevel->priv->y + toplevel->priv->geometry.height);
			else
				y_bottom = -1;
			if (toplevel->priv->y_bottom != y_bottom) {
				toplevel->priv->y_bottom = y_bottom;
				g_object_notify (G_OBJECT (toplevel),
						 "y-bottom");
			}
		}

		g_object_thaw_notify (G_OBJECT (toplevel));
	}

	panel_toplevel_update_expanded_position (toplevel);
	panel_toplevel_calc_floating (toplevel); //FIXME should probably be done after panel_toplevel_update_normal_position() too

	if (toplevel->priv->x_right == -1)
		x = toplevel->priv->x;
	else
		x = monitor_width - (toplevel->priv->x_right + toplevel->priv->geometry.width);
	if (toplevel->priv->y_bottom == -1)
		y = toplevel->priv->y;
	else
		y = monitor_height - (toplevel->priv->y_bottom + toplevel->priv->geometry.height);

	if (!toplevel->priv->expand) {
		if (toplevel->priv->x_centered)
			x = (monitor_width - toplevel->priv->geometry.width) / 2;
		if (toplevel->priv->y_centered)
			y = (monitor_height - toplevel->priv->geometry.height) / 2;
	}

	if (toplevel->priv->state == PANEL_STATE_NORMAL)
		panel_toplevel_update_normal_position (toplevel, &x, &y);

	else if (toplevel->priv->state == PANEL_STATE_AUTO_HIDDEN)
		panel_toplevel_update_auto_hide_position (toplevel, &x, &y, FALSE);

	else 
		panel_toplevel_update_hidden_position (toplevel, &x, &y);

	screen = gtk_window_get_screen (GTK_WINDOW (toplevel));
	x += panel_multiscreen_x (screen, toplevel->priv->monitor);
	y += panel_multiscreen_y (screen, toplevel->priv->monitor);

	toplevel->priv->geometry.x = x;
	toplevel->priv->geometry.y = y;
}

static int
calculate_minimum_height (GtkWidget        *widget,
			  PanelOrientation  orientation)
{
	GtkStateFlags     state;
	GtkStyleContext  *style_context;
	PangoFontDescription *font_desc;
	PangoContext     *pango_context;
	PangoFontMetrics *metrics;
	GtkBorder         padding;
	int               ascent;
	int               descent;
	int               thickness;
  
	state = gtk_widget_get_state_flags (widget);
	style_context = gtk_widget_get_style_context (widget);
        gtk_style_context_get (style_context, state, "font", &font_desc, NULL);

	pango_context = gtk_widget_get_pango_context (widget);
	metrics = pango_context_get_metrics (pango_context,
					     font_desc,
					     pango_context_get_language (pango_context));
	ascent  = pango_font_metrics_get_ascent  (metrics);
	descent = pango_font_metrics_get_descent (metrics);

	pango_font_description_free (font_desc);
	pango_font_metrics_unref (metrics);

	gtk_style_context_get_padding (style_context, state, &padding);

	thickness = orientation & PANEL_HORIZONTAL_MASK ?
		padding.top + padding.bottom :
		padding.left + padding.right;

	return PANGO_PIXELS (ascent + descent) + thickness;
}

static int
panel_toplevel_update_size_from_hints (PanelToplevel  *toplevel,
				       int             requisition_size,
				       int             monitor_size,
				       int             non_panel_widget_size)
{
	int                   nb_size_hints;
	AppletSizeHints      *applets_hints;
	AppletSizeHintsAlloc *using_hint;

	int i;
	int total_size;
	int full_hints;

	total_size = non_panel_widget_size + requisition_size;

	nb_size_hints = toplevel->priv->panel_widget->nb_applets_size_hints;
	if (nb_size_hints <= 0)
		return total_size;

	applets_hints = toplevel->priv->panel_widget->applets_hints;
	using_hint = toplevel->priv->panel_widget->applets_using_hint;

	for (i = 0; i < nb_size_hints; i++) {
		using_hint[i].index = applets_hints[i].len - 2;
		using_hint[i].size = applets_hints[i].hints[applets_hints[i].len - 1];
		total_size += using_hint[i].size;
	}

	if (total_size > monitor_size)
		return monitor_size;

	full_hints = 0;
	while (full_hints != nb_size_hints && total_size < monitor_size) {
		int bonus;
		int extra_bonus;

		bonus = (monitor_size - total_size)
			/ (nb_size_hints - full_hints);
		extra_bonus = (monitor_size - total_size)
			      % (nb_size_hints - full_hints);
		full_hints = 0;

		for (i = 0; i < nb_size_hints; i++) {
			int new_size;
			int current_bonus;

			current_bonus = bonus;

			/* first find the (max, min) range in hints that we
			 * will use; since we try to allocate as much size as
			 * possible, this means we want the (max, min) range
			 * where min is the highest possible (ie, the range
			 * with the smaller index possible), while still
			 * keeping min smaller than the potential new size */
			while (using_hint[i].index > 0 &&
			       applets_hints[i].hints[using_hint[i].index - 1] < using_hint[i].size + current_bonus) {
				new_size = applets_hints[i].hints[using_hint[i].index - 1];
				current_bonus = using_hint[i].size
						+ current_bonus - new_size;
				total_size = total_size - using_hint[i].size
					     + new_size;

				using_hint[i].index -= 2;
				using_hint[i].size = new_size;
			}

			/* now, give the bonus, while still keeping a size that
			 * is lower than max from the (max, min) range we've
			 * settled for */
			new_size = MIN (applets_hints[i].hints[using_hint[i].index],
					using_hint[i].size + current_bonus);
			if (new_size > using_hint[i].size) {
				total_size += (new_size - using_hint[i].size);
				using_hint[i].size = new_size;
			}


			/* if there's some extra bonus to take, try to allocate
			 * it too */
			if (extra_bonus > 0) {
				new_size = MIN (applets_hints[i].hints[using_hint[i].index],
						using_hint[i].size + extra_bonus);
				if (new_size > using_hint[i].size) {
					total_size += (new_size
						       - using_hint[i].size);
					extra_bonus -= (new_size
							- using_hint[i].size);
					using_hint[i].size = new_size;
				}
			}

			if (using_hint[i].size == applets_hints[i].hints[using_hint[i].index])
				full_hints++;
		}
	}

	return total_size;
}

static void
panel_toplevel_update_size (PanelToplevel  *toplevel,
			    GtkRequisition *requisition)
{
	int monitor_width, monitor_height;
	int width, height;
	int size;
	int minimum_height;
	int non_panel_widget_size;

	if (toplevel->priv->animating)
		return;

	panel_toplevel_get_monitor_geometry (
			toplevel, NULL, NULL, &monitor_width, &monitor_height);

	width  = requisition->width;
	height = requisition->height;

	if (!toplevel->priv->expand &&
	    !toplevel->priv->buttons_enabled)
		non_panel_widget_size = 2 * HANDLE_SIZE;
	else
		non_panel_widget_size = 0;

	minimum_height = calculate_minimum_height (GTK_WIDGET (toplevel),
						   toplevel->priv->orientation);

	if (toplevel->priv->orientation & PANEL_HORIZONTAL_MASK) {

		height = MAX (MIN (MAX (height, toplevel->priv->size),
				   panel_toplevel_get_maximum_size (toplevel)),
			      minimum_height);

		if (toplevel->priv->expand)
			width  = monitor_width;
		else
			width = panel_toplevel_update_size_from_hints (
							toplevel,
							requisition->width,
							monitor_width,
							non_panel_widget_size);

		width  = MAX (MINIMUM_WIDTH, width);
		size = height;
	} else {
		width = MAX (MIN (MAX (width, toplevel->priv->size),
				  panel_toplevel_get_maximum_size (toplevel)),
			     minimum_height);

		if (toplevel->priv->expand)
			height = monitor_height;
		else
			height = panel_toplevel_update_size_from_hints (
							toplevel,
							requisition->height,
							monitor_height,
							non_panel_widget_size);

		height = MAX (MINIMUM_WIDTH, height);
		size = width;
	}

	toplevel->priv->geometry.width  = CLAMP (width,  0, monitor_width);
	toplevel->priv->geometry.height = CLAMP (height, 0, monitor_height);
	toplevel->priv->original_width  = toplevel->priv->geometry.width;
	toplevel->priv->original_height = toplevel->priv->geometry.height;

	if (toplevel->priv->size < size)
		panel_widget_set_size (toplevel->priv->panel_widget, size);
}

static void
panel_toplevel_update_geometry (PanelToplevel  *toplevel,
				GtkRequisition *requisition)
{
	toplevel->priv->updated_geometry_initial = TRUE;
	panel_toplevel_update_size (toplevel, requisition);
	panel_toplevel_update_position (toplevel);

	panel_toplevel_update_struts (toplevel, FALSE);

	if (toplevel->priv->state == PANEL_STATE_NORMAL ||
	    toplevel->priv->state == PANEL_STATE_AUTO_HIDDEN) {
		panel_struts_update_toplevel_geometry (toplevel,
						       &toplevel->priv->geometry.x,
						       &toplevel->priv->geometry.y,
						       &toplevel->priv->geometry.width,
						       &toplevel->priv->geometry.height);
	} else {
		panel_struts_update_toplevel_geometry (toplevel,
						       &toplevel->priv->geometry.x,
						       &toplevel->priv->geometry.y,
						       NULL, NULL);
	}

	panel_toplevel_update_description (toplevel);
}

static gboolean
panel_toplevel_popup_panel_menu (PanelToplevel *toplevel)
{
	gboolean retval = FALSE;

	g_signal_emit_by_name (toplevel, "popup_menu", &retval);

	return retval;
}

static gboolean
panel_toplevel_toggle_expand (PanelToplevel *toplevel)
{
	panel_toplevel_set_expand (toplevel, !toplevel->priv->expand);

	return TRUE;
}

static gboolean
panel_toplevel_expand (PanelToplevel *toplevel)
{
	panel_toplevel_set_expand (toplevel, TRUE);

	return TRUE;
}

static gboolean
panel_toplevel_unexpand (PanelToplevel *toplevel)
{
	panel_toplevel_set_expand (toplevel, FALSE);

	return TRUE;
}

static gboolean
panel_toplevel_toggle_hidden (PanelToplevel *toplevel)
{
	GtkDirectionType dir;

	dir = toplevel->priv->orientation & PANEL_VERTICAL_MASK ?
	      GTK_DIR_UP : GTK_DIR_LEFT;

	if (toplevel->priv->state == PANEL_STATE_NORMAL)
		panel_toplevel_hide (toplevel, toplevel->priv->auto_hide, dir);
	else
		panel_toplevel_unhide (toplevel);

	return FALSE;
}

static gboolean
panel_toplevel_begin_move (PanelToplevel *toplevel)
{
	if (toplevel->priv->grab_op != PANEL_GRAB_OP_NONE)
		return FALSE;

	panel_toplevel_begin_grab_op (
			toplevel, PANEL_GRAB_OP_MOVE, TRUE, GDK_CURRENT_TIME);

	return TRUE;
}

static gboolean
panel_toplevel_begin_resize (PanelToplevel *toplevel)
{
	if (toplevel->priv->grab_op != PANEL_GRAB_OP_NONE)
		return FALSE;

	panel_toplevel_begin_grab_op (
			toplevel, PANEL_GRAB_OP_RESIZE, TRUE, GDK_CURRENT_TIME);

	return TRUE;
}

static void
panel_toplevel_move_resize_window (PanelToplevel *toplevel,
				   gboolean       move,
				   gboolean       resize)
{
	GtkWidget *widget;

	widget = GTK_WIDGET (toplevel);

	g_assert (gtk_widget_get_realized (widget));

	if (move && resize)
		gdk_window_move_resize (gtk_widget_get_window (widget),
					toplevel->priv->geometry.x,
					toplevel->priv->geometry.y,
					toplevel->priv->geometry.width,
					toplevel->priv->geometry.height);
	else if (move)
		gdk_window_move (gtk_widget_get_window (widget),
				 toplevel->priv->geometry.x,
				 toplevel->priv->geometry.y);
	else if (resize)
		gdk_window_resize (gtk_widget_get_window (widget),
				   toplevel->priv->geometry.width,
				   toplevel->priv->geometry.height);
}

static void
panel_toplevel_initially_hide (PanelToplevel *toplevel)
{
	toplevel->priv->initial_animation_done = FALSE;

	/* We start the panel off hidden until all the applets are
	 * loaded, and then finally slide it down when it's ready to be
	 * used */
	toplevel->priv->state = PANEL_STATE_AUTO_HIDDEN;
	gtk_widget_queue_resize (GTK_WIDGET (toplevel));
}

static void
panel_toplevel_realize (GtkWidget *widget)
{
	PanelToplevel *toplevel;
	GdkScreen *screen;
	GdkVisual *visual;
	GdkWindow *window;
	GdkGeometry geometry;

	toplevel = PANEL_TOPLEVEL (widget);

	screen = gtk_widget_get_screen (widget);
	visual = gdk_screen_get_rgba_visual (screen);

	if (visual == NULL)
		visual = gdk_screen_get_system_visual (screen);

	gtk_widget_set_visual (widget, visual);
	gtk_window_stick (GTK_WINDOW (widget));

	GTK_WIDGET_CLASS (panel_toplevel_parent_class)->realize (widget);

	window = gtk_widget_get_window (widget);

	panel_struts_set_window_hint (toplevel);

	gdk_window_set_group (window, window);
	gdk_window_set_geometry_hints (window, &geometry, GDK_HINT_POS);

	panel_toplevel_initially_hide (toplevel);

	panel_toplevel_move_resize_window (toplevel, TRUE, TRUE);
}

static void
panel_toplevel_disconnect_timeouts (PanelToplevel *toplevel)
{
	if (toplevel->priv->hide_timeout)
		g_source_remove (toplevel->priv->hide_timeout);
	toplevel->priv->hide_timeout = 0;

	if (toplevel->priv->unhide_timeout)
		g_source_remove (toplevel->priv->unhide_timeout);
	toplevel->priv->unhide_timeout = 0;

	if (toplevel->priv->animation_timeout)
		g_source_remove (toplevel->priv->animation_timeout);
	toplevel->priv->animation_timeout = 0;
}

static void
panel_toplevel_unrealize (GtkWidget *widget)
{
	PanelToplevel *toplevel;

	toplevel = PANEL_TOPLEVEL (widget);

	panel_toplevel_disconnect_timeouts (toplevel);

	GTK_WIDGET_CLASS (panel_toplevel_parent_class)->unrealize (widget);
}

static void
panel_toplevel_dispose (GObject *widget)
{
	PanelToplevel *toplevel = (PanelToplevel *) widget;
	
	panel_toplevel_disconnect_timeouts (toplevel);

	g_clear_object (&toplevel->priv->theme);

        G_OBJECT_CLASS (panel_toplevel_parent_class)->dispose (widget);
}

static void
panel_toplevel_check_resize (GtkContainer *container)
{
	GtkAllocation   allocation;
	GtkRequisition  requisition;
	GtkWidget      *widget;

	widget = GTK_WIDGET (container);

	if (!gtk_widget_get_visible (widget))
		return;

	gtk_widget_get_preferred_size (widget, &requisition, NULL);
	gtk_widget_get_allocation (widget, &allocation);

	allocation.width = requisition.width;
	allocation.height = requisition.height;

	gtk_widget_size_allocate (widget, &allocation);
}

static void
panel_toplevel_size_request (GtkWidget      *widget,
			     GtkRequisition *requisition)
{
	PanelToplevel *toplevel;
	GtkBin        *bin;
	GtkWidget     *child;
	GdkRectangle   old_geometry;
	int            position_changed = FALSE;
	int            size_changed = FALSE;
	int            dummy; /* to pass a valid pointer */

	toplevel = PANEL_TOPLEVEL (widget);
	bin = GTK_BIN (widget);

	/* we get a size request when there are new monitors, so first try to
	 * see if we need to move to a new monitor */
	panel_toplevel_update_monitor (toplevel);

	child = gtk_bin_get_child (bin);
	if (child && gtk_widget_get_visible (child)) {
		gtk_widget_get_preferred_width (child, &dummy, &requisition->width);
		gtk_widget_get_preferred_height (child, &dummy, &requisition->height);
	}

	old_geometry = toplevel->priv->geometry;

	panel_toplevel_update_geometry (toplevel, requisition);

	requisition->width  = toplevel->priv->geometry.width;
	requisition->height = toplevel->priv->geometry.height;

	if (!gtk_widget_get_realized (widget))
		return;

	if (old_geometry.width  != toplevel->priv->geometry.width ||
	    old_geometry.height != toplevel->priv->geometry.height)
		size_changed = TRUE;

	if (old_geometry.x != toplevel->priv->geometry.x ||
	    old_geometry.y != toplevel->priv->geometry.y)
		position_changed = TRUE;

	panel_toplevel_move_resize_window (toplevel, position_changed, size_changed);
}

static void
panel_toplevel_get_preferred_width(GtkWidget *widget, gint *minimal_width, gint *natural_width)
{
	GtkRequisition requisition;

	panel_toplevel_size_request (widget, &requisition);

	*minimal_width = *natural_width = requisition.width;
}

static void
panel_toplevel_get_preferred_height(GtkWidget *widget, gint *minimal_height, gint *natural_height)
{
	GtkRequisition requisition;

	panel_toplevel_size_request (widget, &requisition);

	*minimal_height = *natural_height = requisition.height;
}

static void
panel_toplevel_size_allocate (GtkWidget     *widget,
			      GtkAllocation *allocation)
{
	PanelToplevel   *toplevel = (PanelToplevel *) widget;
	GtkBin          *bin = (GtkBin *) widget;
	GtkWidget       *child;
	GtkAllocation    challoc;
	GtkAllocation    child_allocation;

	gtk_widget_set_allocation (widget, allocation);

	if (toplevel->priv->expand ||
	    toplevel->priv->buttons_enabled)
		challoc = *allocation;
	else {
		if (toplevel->priv->orientation & PANEL_HORIZONTAL_MASK) {
			challoc.x      = HANDLE_SIZE;
			challoc.y      = 0;
			challoc.width  = allocation->width - 2 * HANDLE_SIZE;
			challoc.height = allocation->height;
		} else {
			challoc.x      = 0;
			challoc.y      = HANDLE_SIZE;
			challoc.width  = allocation->width;
			challoc.height = allocation->height - 2 * HANDLE_SIZE;
		}
	}

	challoc.width  = MAX (1, challoc.width);
	challoc.height = MAX (1, challoc.height);

	child = gtk_bin_get_child (bin);
	gtk_widget_get_allocation (child, &child_allocation);

	if (gtk_widget_get_mapped (widget) &&
	    (challoc.x != child_allocation.x ||
	     challoc.y != child_allocation.y ||
	     challoc.width  != child_allocation.width ||
	     challoc.height != child_allocation.height)) {
		GtkAllocation tmp;

		gtk_widget_get_allocation (widget, &tmp);
		gdk_window_invalidate_rect (gtk_widget_get_window (widget), &tmp, FALSE);
	}

	if (child && gtk_widget_get_visible (child))
		gtk_widget_size_allocate (child, &challoc);
}

static gboolean
panel_toplevel_draw (GtkWidget *widget,
                     cairo_t   *cr)
{
	PanelToplevel   *toplevel = (PanelToplevel *) widget;
	gboolean         retval = FALSE;
	GtkStyleContext *context;
	GtkStateFlags    state;
        int awidth, aheight;

	if (GTK_WIDGET_CLASS (panel_toplevel_parent_class)->draw)
		retval = GTK_WIDGET_CLASS (panel_toplevel_parent_class)->draw (widget, cr);

	if (toplevel->priv->expand ||
	    toplevel->priv->buttons_enabled)
		return retval;

	state = gtk_widget_get_state_flags (widget);
        awidth = gtk_widget_get_allocated_width (widget);
        aheight = gtk_widget_get_allocated_height (widget);

	context = gtk_widget_get_style_context (widget);

	gtk_style_context_save (context);
	gtk_style_context_set_state (context, state);

	if (toplevel->priv->orientation & PANEL_HORIZONTAL_MASK) {
		int x, y, width, height;

		x      = 0;
		y      = 0;
		width  = HANDLE_SIZE;
		height = aheight;

		cairo_save (cr);
		gtk_render_handle (context, cr, x, y, width, height);
		cairo_restore (cr);

		x = awidth - HANDLE_SIZE;

		cairo_save (cr);
		gtk_render_handle (context, cr, x, y, width, height);
		cairo_restore (cr);
	} else {
		int x, y, width, height;

		x      = 0;
		y      = 0;
		width  = awidth;
		height = HANDLE_SIZE;

		cairo_save (cr);
		gtk_render_handle (context, cr, x, y, width, height);
		cairo_restore (cr);

		y = aheight - HANDLE_SIZE;

		cairo_save (cr);
		gtk_render_handle (context, cr, x, y, width, height);
		cairo_restore (cr);
	}

	gtk_style_context_restore (context);

	return retval;
}

static gboolean
panel_toplevel_button_press_event (GtkWidget      *widget,
				   GdkEventButton *event)
{
	PanelToplevel *toplevel;
	guint          modifiers;
	GtkWidget     *event_widget;

	g_return_val_if_fail (PANEL_IS_TOPLEVEL (widget), FALSE);

	toplevel = PANEL_TOPLEVEL (widget);

	if (event->button != 1 && event->button != 2)
		return FALSE;

	if (toplevel->priv->animating)
		return FALSE;

	modifiers = event->state & gtk_accelerator_get_default_mod_mask ();

	/* Get the mouse-button modifier from metacity so that only intentional
	 * moves are considered. We don't this for non-expanded panels since we
	 * only have the handles that the user can grab. */
	if (toplevel->priv->expand &&
	    modifiers != panel_bindings_get_mouse_button_modifier_keymask ())
		return FALSE;

	gdk_window_get_user_data (event->window, (gpointer)&event_widget);
	g_assert (GTK_IS_WIDGET (event_widget));
	gtk_widget_translate_coordinates (event_widget,
					  widget,
					  event->x,
					  event->y,
					  &toplevel->priv->drag_offset_x,
					  &toplevel->priv->drag_offset_y);

	panel_toplevel_begin_grab_op (toplevel, PANEL_GRAB_OP_MOVE, FALSE, event->time);

	return TRUE;
}

static gboolean
panel_toplevel_button_release_event (GtkWidget      *widget,
				     GdkEventButton *event)
{
	PanelToplevel *toplevel;

	if (event->button != 1 && event->button != 2)
		return FALSE;

	toplevel = PANEL_TOPLEVEL (widget);

	if (toplevel->priv->grab_op == PANEL_GRAB_OP_NONE)
		return FALSE;

	if (toplevel->priv->grab_is_keyboard)
		return FALSE;

	panel_toplevel_end_grab_op (toplevel, event->time);

	return TRUE;
}

static gboolean
panel_toplevel_key_press_event (GtkWidget   *widget,
				GdkEventKey *event)
{
	PanelToplevel *toplevel = (PanelToplevel *) widget;

	if (toplevel->priv->grab_op != PANEL_GRAB_OP_NONE &&
	    panel_toplevel_handle_grab_op_key_event (toplevel, event))
		return TRUE;

	if (GTK_WIDGET_CLASS (panel_toplevel_parent_class)->key_press_event)
		return GTK_WIDGET_CLASS (panel_toplevel_parent_class)->key_press_event (widget, event);

	return FALSE;
}

static gboolean
panel_toplevel_motion_notify_event (GtkWidget      *widget,
				    GdkEventMotion *event)
{
	if (gdk_event_get_screen ((GdkEvent *)event) == 
	    gtk_window_get_screen (GTK_WINDOW (widget)))
		return panel_toplevel_handle_grab_op_motion_event (
				PANEL_TOPLEVEL (widget), event);
	else
		return FALSE;
}

static gboolean
panel_toplevel_animation_timeout (PanelToplevel *toplevel)
{
	gtk_widget_queue_resize (GTK_WIDGET (toplevel));

	if (!toplevel->priv->animating) {
		toplevel->priv->animation_end_x = 0xdead;
		toplevel->priv->animation_end_y = 0xdead;
		toplevel->priv->animation_start_time = 0xdead;
		toplevel->priv->animation_end_time = 0xdead;
		toplevel->priv->animation_timeout = 0;
		toplevel->priv->initial_animation_done = TRUE;
	}

	return toplevel->priv->animating;
}

static gint64
panel_toplevel_get_animation_time (PanelToplevel *toplevel)
{
 /* The number of seconds to complete the animation.
  */
#define ANIMATION_TIME_FAST   0.4
#define ANIMATION_TIME_MEDIUM 1.2
#define ANIMATION_TIME_SLOW   2.0

	gint64 t;

	switch (toplevel->priv->animation_speed) {
	case PANEL_ANIMATION_SLOW:
		t = ANIMATION_TIME_SLOW * G_USEC_PER_SEC;
		break;
	case PANEL_ANIMATION_MEDIUM:
		t = ANIMATION_TIME_MEDIUM * G_USEC_PER_SEC;
		break;
	case PANEL_ANIMATION_FAST:
		t = ANIMATION_TIME_FAST * G_USEC_PER_SEC;
		break;
	default:
		g_assert_not_reached ();
		break;
	}

	return t;

#undef ANIMATION_TIME_FAST
#undef ANIMATION_TIME_MEDIUM
#undef ANIMATION_TIME_SLOW
}

static void
panel_toplevel_calculate_animation_end_geometry (PanelToplevel *toplevel)
{
	int        monitor_width, monitor_height;

	toplevel->priv->animation_end_x      = toplevel->priv->x;
	toplevel->priv->animation_end_y      = toplevel->priv->y;

	panel_toplevel_get_monitor_geometry (
				toplevel, NULL, NULL, &monitor_width, &monitor_height);

	if (!toplevel->priv->expand) {

		if (toplevel->priv->x_centered)
			toplevel->priv->animation_end_x =
				(monitor_width - toplevel->priv->geometry.width) / 2;
		if (toplevel->priv->y_centered)
			toplevel->priv->animation_end_y =
				(monitor_height - toplevel->priv->geometry.height) / 2;
	}

	/* we consider the toplevels which are in the initial animation stage
	 * as in a normal state */
	if (toplevel->priv->state == PANEL_STATE_NORMAL ||
	    (!toplevel->priv->initial_animation_done &&
	     !toplevel->priv->auto_hide))
		panel_toplevel_update_normal_position (toplevel,
						       &toplevel->priv->animation_end_x,
						       &toplevel->priv->animation_end_y);

	else if (toplevel->priv->state == PANEL_STATE_AUTO_HIDDEN)
		panel_toplevel_update_auto_hide_position (toplevel,
							  &toplevel->priv->animation_end_x,
							  &toplevel->priv->animation_end_y,
							  TRUE);
	else
		panel_toplevel_update_hidden_position (toplevel,
						       &toplevel->priv->animation_end_x,
						       &toplevel->priv->animation_end_y);
}

static void
panel_toplevel_start_animation (PanelToplevel *toplevel)
{
	GdkScreen      *screen;
	GtkRequisition  requisition;
	int             deltax, deltay;
	int             cur_x = -1, cur_y = -1;
	long            t;

	panel_toplevel_calculate_animation_end_geometry (toplevel);

	toplevel->priv->animating = TRUE;

	panel_toplevel_update_struts (toplevel, TRUE);
	panel_struts_update_toplevel_geometry (toplevel,
					       &toplevel->priv->animation_end_x,
					       &toplevel->priv->animation_end_y,
					       NULL,
					       NULL);
	panel_toplevel_update_struts (toplevel, FALSE);

	gdk_window_get_origin (gtk_widget_get_window (GTK_WIDGET (toplevel)), &cur_x, &cur_y);

	screen = gtk_widget_get_screen (GTK_WIDGET (toplevel));

	cur_x -= panel_multiscreen_x (screen, toplevel->priv->monitor);
	cur_y -= panel_multiscreen_y (screen, toplevel->priv->monitor);

	deltax = toplevel->priv->animation_end_x - cur_x;
	deltay = toplevel->priv->animation_end_y - cur_y;

	gtk_widget_get_preferred_size (GTK_WIDGET (toplevel), &requisition, NULL);

	if (deltax == 0 && deltay == 0) {
		toplevel->priv->animation_end_x      = -1;
		toplevel->priv->animation_end_y      = -1;
		toplevel->priv->animating            = FALSE;
		return;
	}

	toplevel->priv->animation_start_time = g_get_real_time ();

	t = panel_toplevel_get_animation_time (toplevel);
	toplevel->priv->animation_end_time = g_get_real_time ();
	toplevel->priv->animation_end_time += t;

	if (!toplevel->priv->animation_timeout)
		toplevel->priv->animation_timeout =
			g_timeout_add (20, (GSourceFunc) panel_toplevel_animation_timeout, toplevel);
}

void
panel_toplevel_hide (PanelToplevel    *toplevel,
		     gboolean          auto_hide,
		     GtkDirectionType  direction)
{
	g_return_if_fail (PANEL_IS_TOPLEVEL (toplevel));

	if (toplevel->priv->state != PANEL_STATE_NORMAL)
		return;

	g_signal_emit (toplevel, toplevel_signals [HIDE_SIGNAL], 0);

	if (auto_hide)
		toplevel->priv->state = PANEL_STATE_AUTO_HIDDEN;
	else {
		switch (direction) {
		case GTK_DIR_UP:
			g_return_if_fail (toplevel->priv->orientation & PANEL_VERTICAL_MASK);
			toplevel->priv->state = PANEL_STATE_HIDDEN_UP;
			break;
		case GTK_DIR_DOWN:
			g_return_if_fail (toplevel->priv->orientation & PANEL_VERTICAL_MASK);
			toplevel->priv->state = PANEL_STATE_HIDDEN_DOWN;
			break;
		case GTK_DIR_LEFT:
			g_return_if_fail (toplevel->priv->orientation & PANEL_HORIZONTAL_MASK);
			toplevel->priv->state = PANEL_STATE_HIDDEN_LEFT;
			break;
		case GTK_DIR_RIGHT:
			g_return_if_fail (toplevel->priv->orientation & PANEL_HORIZONTAL_MASK);
			toplevel->priv->state = PANEL_STATE_HIDDEN_RIGHT;
			break;
		case GTK_DIR_TAB_FORWARD:
		case GTK_DIR_TAB_BACKWARD:
		default:
			g_assert_not_reached ();
			break;
		}

		panel_toplevel_update_hide_buttons (toplevel);
	}

	if (toplevel->priv->animate && gtk_widget_get_realized (GTK_WIDGET (toplevel)))
		panel_toplevel_start_animation (toplevel);

	gtk_widget_queue_resize (GTK_WIDGET (toplevel));
}

static gboolean
panel_toplevel_auto_hide_timeout_handler (PanelToplevel *toplevel)
{
	GtkDirectionType dir;

	g_return_val_if_fail (PANEL_IS_TOPLEVEL (toplevel), FALSE);

	if (panel_toplevel_get_autohide_disabled (toplevel)) {
		toplevel->priv->hide_timeout = 0;
		return FALSE;
	}

	/* keep coming back until the animation has finished.
	 * FIXME: we should really remove the timeout/idle
	 *        completely and re-instate it when the
	 *        animation has finished.
	 */
	if (toplevel->priv->animating)
		return TRUE;

	dir = toplevel->priv->orientation & PANEL_VERTICAL_MASK ?
	      GTK_DIR_UP : GTK_DIR_LEFT;

	panel_toplevel_hide (toplevel, TRUE, dir);

	toplevel->priv->hide_timeout = 0;

	return FALSE;
}

void
panel_toplevel_unhide (PanelToplevel *toplevel)
{
	g_return_if_fail (PANEL_IS_TOPLEVEL (toplevel));

	if (toplevel->priv->state == PANEL_STATE_NORMAL)
		return;

	toplevel->priv->state = PANEL_STATE_NORMAL;

	panel_toplevel_update_hide_buttons (toplevel);

	if (toplevel->priv->animate && gtk_widget_get_realized (GTK_WIDGET (toplevel)))
		panel_toplevel_start_animation (toplevel);

	gtk_widget_queue_resize (GTK_WIDGET (toplevel));

	if (!toplevel->priv->animate)
		g_signal_emit (toplevel, toplevel_signals [UNHIDE_SIGNAL], 0);
}

static gboolean
panel_toplevel_auto_unhide_timeout_handler (PanelToplevel *toplevel)
{
	g_return_val_if_fail (PANEL_IS_TOPLEVEL (toplevel), FALSE);

	/* keep coming back until the animation has finished.
	 * FIXME: we should really remove the timeout/idle
	 *        completely and re-instate it when the
	 *        animation has finished.
	 */
	if (toplevel->priv->animating)
		return TRUE;

	if (!toplevel->priv->animate)
		toplevel->priv->initial_animation_done = TRUE;

	/* initial animation for auto-hidden panels: we need to unhide and hide
	 * again to get at the right size */
	if (!toplevel->priv->initial_animation_done &&
	    toplevel->priv->auto_hide) {
		GtkDirectionType dir;

		dir = toplevel->priv->orientation & PANEL_VERTICAL_MASK ?
		      GTK_DIR_UP : GTK_DIR_LEFT;

		toplevel->priv->unhide_timeout = 0;
		panel_toplevel_unhide (toplevel);
		panel_toplevel_hide (toplevel, TRUE, dir);

		return FALSE;
	}

	if (!panel_toplevel_contains_pointer (toplevel) &&
	    toplevel->priv->auto_hide) {
		toplevel->priv->unhide_timeout = 0;
		return FALSE;
	}

	panel_toplevel_unhide (toplevel);

	toplevel->priv->unhide_timeout = 0;

	return FALSE;
}

void
panel_toplevel_queue_auto_hide (PanelToplevel *toplevel)
{
	g_return_if_fail (PANEL_IS_TOPLEVEL (toplevel));

	if (!toplevel->priv->auto_hide ||
	    panel_toplevel_contains_pointer (toplevel) ||
	    panel_toplevel_get_autohide_disabled (toplevel))
	  return;

	if (toplevel->priv->unhide_timeout)
		g_source_remove (toplevel->priv->unhide_timeout);
	toplevel->priv->unhide_timeout = 0;

	if (toplevel->priv->hide_timeout ||
	    toplevel->priv->state != PANEL_STATE_NORMAL)
		return;

	if (toplevel->priv->hide_delay > 0)
		toplevel->priv->hide_timeout = 
			g_timeout_add (toplevel->priv->hide_delay,
				       (GSourceFunc) panel_toplevel_auto_hide_timeout_handler,
				       toplevel);
	else
		toplevel->priv->hide_timeout = 
			g_idle_add ((GSourceFunc) panel_toplevel_auto_hide_timeout_handler,
				    toplevel);
}

void
panel_toplevel_queue_auto_unhide (PanelToplevel *toplevel)
{
	g_return_if_fail (PANEL_IS_TOPLEVEL (toplevel));

	if (toplevel->priv->unhide_timeout)
		return;

	if (toplevel->priv->hide_timeout)
		g_source_remove (toplevel->priv->hide_timeout);
	toplevel->priv->hide_timeout = 0;

	if (toplevel->priv->state != PANEL_STATE_AUTO_HIDDEN)
		return;

	if (toplevel->priv->unhide_delay > 0)
		toplevel->priv->unhide_timeout = 
			g_timeout_add (toplevel->priv->unhide_delay,
				       (GSourceFunc) panel_toplevel_auto_unhide_timeout_handler,
				       toplevel);
	else
		toplevel->priv->unhide_timeout = 
			g_idle_add ((GSourceFunc) panel_toplevel_auto_unhide_timeout_handler,
				    toplevel);
}

void
panel_toplevel_queue_initial_unhide (PanelToplevel *toplevel)
{
	g_return_if_fail (PANEL_IS_TOPLEVEL (toplevel));

	if (toplevel->priv->initial_animation_done)
		return;

	if (toplevel->priv->unhide_timeout)
		return;

	toplevel->priv->unhide_timeout =
		g_idle_add ((GSourceFunc) panel_toplevel_auto_unhide_timeout_handler,
			    toplevel);
}

static gboolean
panel_toplevel_enter_notify_event (GtkWidget        *widget,
				   GdkEventCrossing *event)
{
	PanelToplevel *toplevel;

	g_return_val_if_fail (PANEL_IS_TOPLEVEL (widget), FALSE);

	toplevel = PANEL_TOPLEVEL (widget);

	if (toplevel->priv->auto_hide && event->detail != GDK_NOTIFY_INFERIOR)
		panel_toplevel_queue_auto_unhide (toplevel);

	if (GTK_WIDGET_CLASS (panel_toplevel_parent_class)->enter_notify_event)
		return GTK_WIDGET_CLASS (panel_toplevel_parent_class)->enter_notify_event (widget, event);

	return FALSE;
}

static gboolean
panel_toplevel_leave_notify_event (GtkWidget        *widget,
				   GdkEventCrossing *event)
{
	PanelToplevel *toplevel;

	g_return_val_if_fail (PANEL_IS_TOPLEVEL (widget), FALSE);

	toplevel = PANEL_TOPLEVEL (widget);

	if (toplevel->priv->auto_hide && event->detail != GDK_NOTIFY_INFERIOR)
		panel_toplevel_queue_auto_hide (toplevel);

	if (GTK_WIDGET_CLASS (panel_toplevel_parent_class)->leave_notify_event)
		return GTK_WIDGET_CLASS (panel_toplevel_parent_class)->leave_notify_event (widget, event);

	return FALSE;
}

static gboolean
panel_toplevel_focus_in_event (GtkWidget     *widget,
			       GdkEventFocus *event)
{
	PanelToplevel *toplevel = PANEL_TOPLEVEL (widget);
	
	if (toplevel->priv->state == PANEL_STATE_AUTO_HIDDEN)
		panel_toplevel_unhide (toplevel);

	if (GTK_WIDGET_CLASS (panel_toplevel_parent_class)->focus_in_event)
		return GTK_WIDGET_CLASS (panel_toplevel_parent_class)->focus_in_event (widget, event);

	return FALSE;
}

static gboolean
panel_toplevel_focus_out_event (GtkWidget     *widget,
				GdkEventFocus *event)
{
	PanelToplevel *toplevel = PANEL_TOPLEVEL (widget);

	/* It appears that sometimes we don't get a leave notify event,
	   but just a focus in/out, so queue the autohide in that case.
	   If the pointer is inside the panel then obviously we won't hide */
	if (toplevel->priv->auto_hide)
		panel_toplevel_queue_auto_hide (toplevel);

	if (GTK_WIDGET_CLASS (panel_toplevel_parent_class)->focus_out_event)
		return GTK_WIDGET_CLASS (panel_toplevel_parent_class)->focus_out_event (widget, event);

	return FALSE;
}

static void
panel_toplevel_style_updated (GtkWidget *widget)
{
	PanelToplevel *toplevel = PANEL_TOPLEVEL (widget);

	panel_toplevel_update_hide_buttons (toplevel);

	if (GTK_WIDGET_CLASS (panel_toplevel_parent_class)->style_updated)
		GTK_WIDGET_CLASS (panel_toplevel_parent_class)->style_updated (widget);

	panel_widget_set_size (toplevel->priv->panel_widget, toplevel->priv->size);
}

static void
panel_toplevel_composited_changed (GtkWidget *widget)
{
	PanelToplevel *toplevel;
	GdkScreen *screen;
	gboolean composited;

	toplevel = PANEL_TOPLEVEL (widget);
	screen = gdk_screen_get_default ();
	composited = gdk_screen_is_composited (screen);

	gp_theme_set_composited (toplevel->priv->theme, composited);
}

static void
panel_toplevel_drag_threshold_changed (PanelToplevel *toplevel)
{
	int threshold;

	threshold = 0;
	g_object_get (G_OBJECT (toplevel->priv->gtk_settings),
		      "gtk-dnd-drag-threshold", &threshold,
		      NULL);

	if (threshold)
		toplevel->priv->snap_tolerance = threshold * SNAP_TOLERANCE_FACTOR;
}

static void
panel_toplevel_enable_animations_changed (PanelToplevel *toplevel)
{
	gboolean enable_animations;

	enable_animations = TRUE;
	g_object_get (G_OBJECT (toplevel->priv->gtk_settings),
		      "gtk-enable-animations", &enable_animations,
		      NULL);

	panel_toplevel_set_animate (toplevel, enable_animations);
}

static void
panel_toplevel_disconnect_gtk_settings (PanelToplevel *toplevel)
{
	if (!toplevel->priv->gtk_settings)
		return;

	g_signal_handlers_disconnect_by_func (toplevel->priv->gtk_settings,
					      G_CALLBACK (panel_toplevel_drag_threshold_changed),
					      toplevel);
	g_signal_handlers_disconnect_by_func (toplevel->priv->gtk_settings,
					      G_CALLBACK (panel_toplevel_enable_animations_changed),
					      toplevel);
}

static void
panel_toplevel_update_gtk_settings (PanelToplevel *toplevel)
{
	panel_toplevel_disconnect_gtk_settings (toplevel);

	toplevel->priv->gtk_settings = gtk_widget_get_settings (GTK_WIDGET (toplevel->priv->panel_widget));

	g_signal_connect_swapped (G_OBJECT (toplevel->priv->gtk_settings),
				  "notify::gtk-dnd-drag-threshold",
				  G_CALLBACK (panel_toplevel_drag_threshold_changed),
				  toplevel);

	panel_toplevel_drag_threshold_changed (toplevel);

	g_signal_connect_swapped (G_OBJECT (toplevel->priv->gtk_settings),
				  "notify::gtk-enable-animations",
				  G_CALLBACK (panel_toplevel_enable_animations_changed),
				  toplevel);

	panel_toplevel_enable_animations_changed (toplevel);
}

static void
panel_toplevel_screen_changed (GtkWidget *widget,
			       GdkScreen *previous_screen)
{
	panel_toplevel_update_gtk_settings (PANEL_TOPLEVEL (widget));

	if (GTK_WIDGET_CLASS (panel_toplevel_parent_class)->screen_changed)
		GTK_WIDGET_CLASS (panel_toplevel_parent_class)->screen_changed (widget, previous_screen);

	gtk_widget_queue_resize (widget);
}

static void
panel_toplevel_constructed (GObject *object)
{
	PanelToplevel *toplevel;
	GdkScreen *screen;
	gboolean composited;
	GtkOrientation orientation;

	toplevel = PANEL_TOPLEVEL (object);
	screen = gdk_screen_get_default ();

	G_OBJECT_CLASS (panel_toplevel_parent_class)->constructed (object);

	composited = gdk_screen_is_composited (screen);
	orientation = GTK_ORIENTATION_HORIZONTAL;

	if (toplevel->priv->orientation & PANEL_VERTICAL_MASK)
		orientation = GTK_ORIENTATION_VERTICAL;

	toplevel->priv->theme = gp_theme_new (toplevel->priv->toplevel_id,
	                                      composited, orientation);
}

static GObject *
panel_toplevel_constructor (GType                  type,
			    guint                  n_construct_properties,
			    GObjectConstructParam *construct_properties)
{
	GObject       *obj;
	PanelToplevel *toplevel;

	obj = G_OBJECT_CLASS (panel_toplevel_parent_class)->constructor (type,
									 n_construct_properties,
									 construct_properties);

	toplevel = PANEL_TOPLEVEL (obj);

	/* We can't do that in init() as init() will set the properties to
	 * their default, and this would write to gsettings */
	panel_toplevel_bind_gsettings (toplevel);

	return obj;
}

static void
panel_toplevel_set_property (GObject      *object,
			     guint         prop_id,
			     const GValue *value,
			     GParamSpec   *pspec)
{
	PanelToplevel *toplevel;

	g_return_if_fail (PANEL_IS_TOPLEVEL (object));

	toplevel = PANEL_TOPLEVEL (object);

	switch (prop_id) {
	case PROP_TOPLEVEL_ID:
		panel_toplevel_set_toplevel_id (toplevel, g_value_get_string (value));
		break;
	case PROP_SETTINGS_PATH:
		panel_toplevel_set_settings_path (toplevel, g_value_get_string (value));
		break;
	case PROP_NAME:
		panel_toplevel_set_name (toplevel, g_value_get_string (value));
		break;
	case PROP_EXPAND:
		panel_toplevel_set_expand (toplevel, g_value_get_boolean (value));
		break;
	case PROP_ORIENTATION:
		panel_toplevel_set_orientation (toplevel, g_value_get_enum (value));
		break;
	case PROP_SIZE:
		panel_toplevel_set_size (toplevel, g_value_get_int (value));
		break;
	case PROP_X:
		panel_toplevel_set_x (toplevel,
				      g_value_get_int (value),
				      toplevel->priv->x_right,
				      toplevel->priv->x_centered);
		break;
	case PROP_X_RIGHT:
		panel_toplevel_set_x (toplevel,
				      toplevel->priv->x,
				      g_value_get_int (value),
				      toplevel->priv->x_centered);
		break;
	case PROP_X_CENTERED:
		panel_toplevel_set_x (toplevel,
				      toplevel->priv->x,
				      toplevel->priv->x_right,
				      g_value_get_boolean (value));
		break;
	case PROP_Y:
		panel_toplevel_set_y (toplevel,
				      g_value_get_int (value),
				      toplevel->priv->y_bottom,
				      toplevel->priv->y_centered);
		break;
	case PROP_Y_BOTTOM:
		panel_toplevel_set_y (toplevel,
				      toplevel->priv->y,
				      g_value_get_int (value),
				      toplevel->priv->y_centered);
		break;
	case PROP_Y_CENTERED:
		panel_toplevel_set_y (toplevel,
				      toplevel->priv->y,
				      toplevel->priv->y_bottom,
				      g_value_get_boolean (value));
		break;
	case PROP_MONITOR:
		panel_toplevel_set_monitor (toplevel, g_value_get_int (value));
		break;
	case PROP_AUTOHIDE:
		panel_toplevel_set_auto_hide (toplevel, g_value_get_boolean (value));
		break;
	case PROP_HIDE_DELAY:
		panel_toplevel_set_hide_delay (toplevel, g_value_get_int (value));
		break;
	case PROP_UNHIDE_DELAY:
		panel_toplevel_set_unhide_delay (toplevel, g_value_get_int (value));
		break;
	case PROP_AUTOHIDE_SIZE:
		panel_toplevel_set_auto_hide_size (toplevel, g_value_get_int (value));
		break;
	case PROP_ANIMATE:
		panel_toplevel_set_animate (toplevel, g_value_get_boolean (value));
		break;
	case PROP_ANIMATION_SPEED:
		panel_toplevel_set_animation_speed (toplevel, g_value_get_enum (value));
		break;
	case PROP_BUTTONS_ENABLED:
		panel_toplevel_set_enable_buttons (toplevel, g_value_get_boolean (value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
panel_toplevel_get_property (GObject    *object,
			     guint       prop_id,
			     GValue     *value,
			     GParamSpec *pspec)
{
	PanelToplevel *toplevel;

	g_return_if_fail (PANEL_IS_TOPLEVEL (object));

	toplevel = PANEL_TOPLEVEL (object);

	switch (prop_id) {
	case PROP_TOPLEVEL_ID:
		g_value_set_string (value, toplevel->priv->toplevel_id);
		break;
	case PROP_SETTINGS_PATH:
		g_value_set_string (value, toplevel->priv->settings_path);
		break;
	case PROP_NAME:
		g_value_set_string (value, panel_toplevel_get_name (toplevel));
		break;
	case PROP_EXPAND:
		g_value_set_boolean (value, toplevel->priv->expand);
		break;
	case PROP_ORIENTATION:
		g_value_set_enum (value, toplevel->priv->orientation);
		break;
	case PROP_SIZE:
		g_value_set_int (value, toplevel->priv->size);
		break;
	case PROP_X:
		g_value_set_int (value, toplevel->priv->x);
		break;
	case PROP_X_RIGHT:
		g_value_set_int (value, toplevel->priv->x_right);
		break;
	case PROP_X_CENTERED:
		g_value_set_boolean (value, toplevel->priv->x_centered);
		break;
	case PROP_Y:
		g_value_set_int (value, toplevel->priv->y);
		break;
	case PROP_Y_BOTTOM:
		g_value_set_int (value, toplevel->priv->y_bottom);
		break;
	case PROP_Y_CENTERED:
		g_value_set_boolean (value, toplevel->priv->y_centered);
		break;
	case PROP_MONITOR:
		g_value_set_int (value, toplevel->priv->monitor);
		break;
	case PROP_AUTOHIDE:
		g_value_set_boolean (value, toplevel->priv->auto_hide);
		break;
	case PROP_HIDE_DELAY:
		g_value_set_int (value, toplevel->priv->hide_delay);
		break;
	case PROP_UNHIDE_DELAY:
		g_value_set_int (value, toplevel->priv->unhide_delay);
		break;
	case PROP_AUTOHIDE_SIZE:
		g_value_set_int (value, toplevel->priv->auto_hide_size);
		break;
	case PROP_ANIMATE:
		g_value_set_boolean (value, toplevel->priv->animate);
		break;
	case PROP_ANIMATION_SPEED:
		g_value_set_enum (value, toplevel->priv->animation_speed);
		break;
	case PROP_BUTTONS_ENABLED:
		g_value_set_boolean (value, toplevel->priv->buttons_enabled);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
panel_toplevel_finalize (GObject *object)
{
	PanelToplevel *toplevel = (PanelToplevel *) object;

	panel_struts_unregister_strut (toplevel);

	toplevel_list = g_slist_remove (toplevel_list, toplevel);

	panel_toplevel_disconnect_gtk_settings (toplevel);
	toplevel->priv->gtk_settings = NULL;

	if (toplevel->priv->description)
		g_free (toplevel->priv->description);
	toplevel->priv->description = NULL;

	if (toplevel->priv->name)
		g_free (toplevel->priv->name);
	toplevel->priv->name = NULL;

	if (toplevel->priv->apply_delayed_id)
		g_source_remove (toplevel->priv->apply_delayed_id);
	toplevel->priv->apply_delayed_id = 0;

	if (toplevel->priv->delayed_settings) {
		g_settings_apply (toplevel->priv->delayed_settings);
		g_object_unref (toplevel->priv->delayed_settings);
	}
	toplevel->priv->delayed_settings= NULL;

	if (toplevel->priv->settings)
		g_object_unref (toplevel->priv->settings);
	toplevel->priv->settings= NULL;

	if (toplevel->priv->settings_path)
		g_free (toplevel->priv->settings_path);
	toplevel->priv->settings_path = NULL;

	if (toplevel->priv->toplevel_id)
		g_free (toplevel->priv->toplevel_id);
	toplevel->priv->toplevel_id = NULL;

	G_OBJECT_CLASS (panel_toplevel_parent_class)->finalize (object);
}

static void
panel_toplevel_class_init (PanelToplevelClass *klass)
{
	GObjectClass      *gobject_class   = (GObjectClass      *) klass;
	GtkWidgetClass    *widget_class    = (GtkWidgetClass    *) klass;
	GtkContainerClass *container_class = (GtkContainerClass *) klass;
	GtkBindingSet     *binding_set;

        binding_set = gtk_binding_set_by_class (klass);

	gobject_class->constructed  = panel_toplevel_constructed;
	gobject_class->constructor  = panel_toplevel_constructor;
	gobject_class->set_property = panel_toplevel_set_property;
        gobject_class->get_property = panel_toplevel_get_property;
        gobject_class->dispose      = panel_toplevel_dispose;
	gobject_class->finalize     = panel_toplevel_finalize;

	widget_class->realize              = panel_toplevel_realize;
	widget_class->unrealize            = panel_toplevel_unrealize;
	widget_class->get_preferred_width  = panel_toplevel_get_preferred_width;
	widget_class->get_preferred_height = panel_toplevel_get_preferred_height;
	widget_class->size_allocate        = panel_toplevel_size_allocate;
	widget_class->draw                 = panel_toplevel_draw;
	widget_class->button_press_event   = panel_toplevel_button_press_event;
	widget_class->button_release_event = panel_toplevel_button_release_event;
	widget_class->key_press_event      = panel_toplevel_key_press_event;
	widget_class->motion_notify_event  = panel_toplevel_motion_notify_event;
	widget_class->enter_notify_event   = panel_toplevel_enter_notify_event;
	widget_class->leave_notify_event   = panel_toplevel_leave_notify_event;
	widget_class->screen_changed       = panel_toplevel_screen_changed;
	widget_class->focus_in_event       = panel_toplevel_focus_in_event;
	widget_class->focus_out_event      = panel_toplevel_focus_out_event;
	widget_class->style_updated        = panel_toplevel_style_updated;
	widget_class->composited_changed   = panel_toplevel_composited_changed;

	container_class->check_resize = panel_toplevel_check_resize;

	klass->hiding           = NULL;
	klass->unhiding         = NULL;
	klass->popup_panel_menu = panel_toplevel_popup_panel_menu;
	klass->toggle_expand    = panel_toplevel_toggle_expand;
	klass->expand           = panel_toplevel_expand;
	klass->unexpand         = panel_toplevel_unexpand;
	klass->toggle_hidden    = panel_toplevel_toggle_hidden;
	klass->begin_move       = panel_toplevel_begin_move;
	klass->begin_resize     = panel_toplevel_begin_resize;

	g_object_class_install_property (
		gobject_class,
		PROP_TOPLEVEL_ID,
		g_param_spec_string (
			"toplevel-id",
			"Panel identifier",
			"Unique identifier of this panel",
			NULL,
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property (
		gobject_class,
		PROP_SETTINGS_PATH,
		g_param_spec_string (
			"settings-path",
			"GSettings path",
			"The GSettings path used for this panel",
			NULL,
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property (
		gobject_class,
		PROP_NAME,
		g_param_spec_string (
			"panel-name",
			"Name",
			"The name of this panel",
			NULL,
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

	g_object_class_install_property (
		gobject_class,
		PROP_EXPAND,
		g_param_spec_boolean (
			"expand",
			"Expand",
			"Expand to take up the full monitor width/height",
			TRUE,
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

	g_object_class_install_property (
		gobject_class,
		PROP_ORIENTATION,
		g_param_spec_enum (
			"orientation",
			"Orientation",
			"The orientation of the panel",
			PANEL_TYPE_ORIENTATION,
			PANEL_ORIENTATION_TOP,
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

	g_object_class_install_property (
		gobject_class,
		PROP_SIZE,
		g_param_spec_int (
			"size",
			"Size",
			"The height (or width when vertical) of the panel",
			0,
			G_MAXINT,
			DEFAULT_SIZE,
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT));


	g_object_class_install_property (
		gobject_class,
		PROP_X,
		g_param_spec_int (
			"x",
			"X position",
			"The X position of the panel",
			0,
			G_MAXINT,
			0,
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

	g_object_class_install_property (
		gobject_class,
		PROP_X_RIGHT,
		g_param_spec_int (
			"x-right",
			"X position, from the right",
			"The X position of the panel, starting from the right of the screen",
			-1,
			G_MAXINT,
			-1,
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

	g_object_class_install_property (
		gobject_class,
		PROP_X_CENTERED,
		g_param_spec_boolean (
			"x-centered",
			"X centered",
			"The x co-ordinate is relative to center screen",
			FALSE,
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

	g_object_class_install_property (
		gobject_class,
		PROP_Y,
		g_param_spec_int (
			"y",
			"Y position",
			"The Y position of the panel",
			0,
			G_MAXINT,
			0,
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

	g_object_class_install_property (
		gobject_class,
		PROP_Y_BOTTOM,
		g_param_spec_int (
			"y-bottom",
			"Y position, from the bottom",
			"The Y position of the panel, starting from the bottom of the screen",
			-1,
			G_MAXINT,
			-1,
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

	g_object_class_install_property (
		gobject_class,
		PROP_Y_CENTERED,
		g_param_spec_boolean (
			"y-centered",
			"Y centered",
			"The y co-ordinate is relative to center screen",
			FALSE,
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

	g_object_class_install_property (
		gobject_class,
		PROP_MONITOR,
		g_param_spec_int (
			"monitor",
			"Xinerama monitor",
			"The monitor (in terms of Xinerama) which the panel is on",
			0,
			G_MAXINT,
			0,
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

	g_object_class_install_property (
		gobject_class,
		PROP_AUTOHIDE,
		g_param_spec_boolean (
			"auto-hide",
			"Auto hide",
			"Automatically hide the panel when the mouse leaves the panel",
			FALSE,
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

	g_object_class_install_property (
		gobject_class,
		PROP_HIDE_DELAY,
		g_param_spec_int (
			"hide-delay",
			"Hide delay",
			"The number of milliseconds to delay before automatically hiding",
			0,
			G_MAXINT,
			DEFAULT_HIDE_DELAY,
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

	g_object_class_install_property (
		gobject_class,
		PROP_UNHIDE_DELAY,
		g_param_spec_int (
			"unhide-delay",
			"Un-hide delay",
			"The number of milliseconds to delay before automatically un-hiding",
			0,
			G_MAXINT,
			DEFAULT_UNHIDE_DELAY,
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

	g_object_class_install_property (
		gobject_class,
		PROP_AUTOHIDE_SIZE,
		g_param_spec_int (
			"auto-hide-size",
			"Auto-hide size",
			"The number of pixels visible when the panel has been automatically hidden",
			1,
			G_MAXINT,
			DEFAULT_AUTO_HIDE_SIZE,
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

	g_object_class_install_property (
		gobject_class,
		PROP_ANIMATE,
		g_param_spec_boolean (
			"animate",
			"Animate",
			"Enable hiding/showing animations",
			TRUE,
			G_PARAM_READABLE));

	g_object_class_install_property (
		gobject_class,
		PROP_ANIMATION_SPEED,
		g_param_spec_enum (
			"animation-speed",
			"Animation Speed",
			"The speed at which to animate panel hiding/showing",
			PANEL_TYPE_ANIMATION_SPEED,
			PANEL_ANIMATION_MEDIUM,
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

	g_object_class_install_property (
		gobject_class,
		PROP_BUTTONS_ENABLED,
		g_param_spec_boolean (
			"buttons-enabled",
			"Buttons Enabled",
			"Enable hide/show buttons",
			TRUE,
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

	toplevel_signals [HIDE_SIGNAL] =
		g_signal_new ("hiding",
			      G_TYPE_FROM_CLASS (gobject_class),
			      G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
			      G_STRUCT_OFFSET (PanelToplevelClass, hiding),
			      NULL,
			      NULL,
			      NULL,
			      G_TYPE_NONE,
			      0);

	toplevel_signals [UNHIDE_SIGNAL] =
		g_signal_new ("unhiding",
			      G_TYPE_FROM_CLASS (gobject_class),
			      G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
			      G_STRUCT_OFFSET (PanelToplevelClass, unhiding),
			      NULL,
			      NULL,
			      NULL,
			      G_TYPE_NONE,
			      0);

	toplevel_signals [POPUP_PANEL_MENU_SIGNAL] =
		g_signal_new ("popup-panel-menu",
			      G_TYPE_FROM_CLASS (gobject_class),
			      G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
			      G_STRUCT_OFFSET (PanelToplevelClass, popup_panel_menu),
			      NULL,
			      NULL,
			      NULL,
			      G_TYPE_BOOLEAN,
			      0);

	toplevel_signals [TOGGLE_EXPAND_SIGNAL] =
		g_signal_new ("toggle-expand",
			      G_TYPE_FROM_CLASS (gobject_class),
			      G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
			      G_STRUCT_OFFSET (PanelToplevelClass, toggle_expand),
			      NULL,
			      NULL,
			      NULL,
			      G_TYPE_BOOLEAN,
			      0);

	toplevel_signals [EXPAND_SIGNAL] =
		g_signal_new ("expand",
			      G_TYPE_FROM_CLASS (gobject_class),
			      G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
			      G_STRUCT_OFFSET (PanelToplevelClass, expand),
			      NULL,
			      NULL,
			      NULL,
			      G_TYPE_BOOLEAN,
			      0);

	toplevel_signals [UNEXPAND_SIGNAL] =
		g_signal_new ("unexpand",
			      G_TYPE_FROM_CLASS (gobject_class),
			      G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
			      G_STRUCT_OFFSET (PanelToplevelClass, unexpand),
			      NULL,
			      NULL,
			      NULL,
			      G_TYPE_BOOLEAN,
			      0);

	toplevel_signals [TOGGLE_HIDDEN_SIGNAL] =
		g_signal_new ("toggle-hidden",
			      G_TYPE_FROM_CLASS (gobject_class),
			      G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
			      G_STRUCT_OFFSET (PanelToplevelClass, toggle_hidden),
			      NULL,
			      NULL,
			      NULL,
			      G_TYPE_BOOLEAN,
			      0);

	toplevel_signals [BEGIN_MOVE_SIGNAL] =
		g_signal_new ("begin-move",
			      G_TYPE_FROM_CLASS (gobject_class),
			      G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
			      G_STRUCT_OFFSET (PanelToplevelClass, begin_move),
			      NULL,
			      NULL,
			      NULL,
			      G_TYPE_BOOLEAN,
			      0);

	toplevel_signals [BEGIN_RESIZE_SIGNAL] =
		g_signal_new ("begin-resize",
			      G_TYPE_FROM_CLASS (gobject_class),
			      G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
			      G_STRUCT_OFFSET (PanelToplevelClass, begin_resize),
			      NULL,
			      NULL,
			      NULL,
			      G_TYPE_BOOLEAN,
			      0);

	gtk_binding_entry_add_signal (binding_set, GDK_KEY_F10, GDK_CONTROL_MASK,
                                     "popup_panel_menu", 0);

	gtk_widget_class_set_css_name (widget_class, "panel-toplevel");

	panel_bindings_set_entries (binding_set);
}

static void
panel_toplevel_setup_widgets (PanelToplevel *toplevel)
{
	GtkWidget *container;

	toplevel->priv->grid = gtk_grid_new ();

	toplevel->priv->hide_button_top    = panel_toplevel_add_hide_button (toplevel, GTK_ARROW_UP,    1, 0);
	toplevel->priv->hide_button_bottom = panel_toplevel_add_hide_button (toplevel, GTK_ARROW_DOWN,  1, 2);
	toplevel->priv->hide_button_left   = panel_toplevel_add_hide_button (toplevel, GTK_ARROW_LEFT,  0, 1);
	toplevel->priv->hide_button_right  = panel_toplevel_add_hide_button (toplevel, GTK_ARROW_RIGHT, 2, 1);

	if (toplevel->priv->orientation & PANEL_HORIZONTAL_MASK) {
		gtk_widget_show (toplevel->priv->hide_button_left);
		gtk_widget_show (toplevel->priv->hide_button_right);
	} else {
		gtk_widget_show (toplevel->priv->hide_button_top);
		gtk_widget_show (toplevel->priv->hide_button_bottom);
	}

	container = panel_widget_new (toplevel,
				      !toplevel->priv->expand,
				      toplevel->priv->orientation & PANEL_HORIZONTAL_MASK ?
						GTK_ORIENTATION_HORIZONTAL :
						GTK_ORIENTATION_VERTICAL,	
				      toplevel->priv->size);

	toplevel->priv->panel_widget = PANEL_WIDGET (container);

	gtk_widget_set_hexpand (container, TRUE);
	gtk_widget_set_vexpand (container, TRUE);

	gtk_grid_attach (GTK_GRID (toplevel->priv->grid), container, 1, 1, 1, 1);
	gtk_widget_show (container);

	gtk_container_add (GTK_CONTAINER (toplevel), toplevel->priv->grid);
	gtk_widget_show (toplevel->priv->grid);
}

static void
panel_toplevel_init (PanelToplevel *toplevel)
{
	GtkWidget *widget;

	toplevel->priv = panel_toplevel_get_instance_private (toplevel);

	toplevel->priv->toplevel_id      = NULL;

	toplevel->priv->settings_path    = NULL;
	toplevel->priv->settings         = NULL;
	toplevel->priv->delayed_settings = NULL;
	toplevel->priv->apply_delayed_id = 0;

	toplevel->priv->expand          = TRUE;
	toplevel->priv->orientation     = PANEL_ORIENTATION_BOTTOM;
	toplevel->priv->size            = DEFAULT_SIZE;
	toplevel->priv->x               = 0;
	toplevel->priv->y               = 0;
	toplevel->priv->x_right         = -1;
	toplevel->priv->y_bottom        = -1;
	toplevel->priv->monitor         = 0;
	toplevel->priv->configured_monitor = -1;
	toplevel->priv->hide_delay      = DEFAULT_HIDE_DELAY;
	toplevel->priv->unhide_delay    = DEFAULT_UNHIDE_DELAY;
	toplevel->priv->auto_hide_size  = DEFAULT_AUTO_HIDE_SIZE;
	toplevel->priv->animation_speed = PANEL_ANIMATION_FAST;

	toplevel->priv->snap_tolerance  = DEFAULT_DND_THRESHOLD * SNAP_TOLERANCE_FACTOR;
	toplevel->priv->gtk_settings    = NULL;

	toplevel->priv->state = PANEL_STATE_NORMAL;

	toplevel->priv->name        = NULL;
	toplevel->priv->description = NULL;

	toplevel->priv->hide_timeout   = 0;
	toplevel->priv->unhide_timeout = 0;

	toplevel->priv->geometry.x      = -1;
	toplevel->priv->geometry.y      = -1;
	toplevel->priv->geometry.width  = -1;
	toplevel->priv->geometry.height = -1;

	toplevel->priv->original_width  = -1;
	toplevel->priv->original_height = -1;

	toplevel->priv->grab_op = PANEL_GRAB_OP_NONE;

	toplevel->priv->drag_offset_x = 0;
	toplevel->priv->drag_offset_y = 0;

	toplevel->priv->animation_end_x = 0;
	toplevel->priv->animation_end_y = 0;
	toplevel->priv->animation_start_time = 0;
	toplevel->priv->animation_end_time = 0;
	toplevel->priv->animation_timeout = 0;

	toplevel->priv->panel_widget       = NULL;
	toplevel->priv->grid               = NULL;
	toplevel->priv->hide_button_top    = NULL;
	toplevel->priv->hide_button_bottom = NULL;
	toplevel->priv->hide_button_left   = NULL;
	toplevel->priv->hide_button_right  = NULL;

	toplevel->priv->n_autohide_disablers = 0;

	toplevel->priv->auto_hide         = FALSE;
	toplevel->priv->buttons_enabled   = TRUE;
	toplevel->priv->arrows_enabled    = TRUE;
	toplevel->priv->x_centered        = FALSE;
	toplevel->priv->y_centered        = FALSE;
	toplevel->priv->animating         = FALSE;
	toplevel->priv->grab_is_keyboard  = FALSE;
	toplevel->priv->position_centered = FALSE;
	toplevel->priv->updated_geometry_initial = FALSE;
	toplevel->priv->initial_animation_done   = FALSE;

	widget = GTK_WIDGET (toplevel);

	gtk_widget_add_events (widget,
			       GDK_BUTTON_PRESS_MASK |
			       GDK_BUTTON_RELEASE_MASK |
			       GDK_POINTER_MOTION_MASK |
			       GDK_ENTER_NOTIFY_MASK |
			       GDK_LEAVE_NOTIFY_MASK);

	panel_toplevel_setup_widgets (toplevel);
	panel_toplevel_update_description (toplevel);
	panel_toplevel_update_gtk_settings (toplevel);

	toplevel_list = g_slist_prepend (toplevel_list, toplevel);

	/* Prevent the window from being deleted via Alt+F4 by accident.  This
	 * happens with "alternative" window managers such as Sawfish or XFWM4.
	 */
	g_signal_connect (GTK_WIDGET (toplevel),
	                  "delete-event",
	                  G_CALLBACK (gtk_true),
	                  NULL);

	update_style_classes (toplevel);
}

PanelWidget *
panel_toplevel_get_panel_widget (PanelToplevel *toplevel)
{
	g_return_val_if_fail (PANEL_IS_TOPLEVEL (toplevel), NULL);

	return toplevel->priv->panel_widget;
}

static gboolean
panel_toplevel_position_is_writable (PanelToplevel *toplevel)
{
	if (panel_lockdown_get_panels_locked_down_s () ||
	    !(g_settings_is_writable (toplevel->priv->settings,
				      PANEL_TOPLEVEL_MONITOR_KEY) &&
	      g_settings_is_writable (toplevel->priv->settings,
				      PANEL_TOPLEVEL_ORIENTATION_KEY)))
		return FALSE;

	/* For expanded panels we don't really have to check x and y */
	if (panel_toplevel_get_expand (toplevel))
		return TRUE;

	return (g_settings_is_writable (toplevel->priv->settings,
					PANEL_TOPLEVEL_X_KEY) &&
		g_settings_is_writable (toplevel->priv->settings,
					PANEL_TOPLEVEL_Y_KEY) &&
		g_settings_is_writable (toplevel->priv->settings,
					PANEL_TOPLEVEL_X_RIGHT_KEY) &&
		g_settings_is_writable (toplevel->priv->settings,
					PANEL_TOPLEVEL_Y_BOTTOM_KEY) &&
		g_settings_is_writable (toplevel->priv->settings,
					PANEL_TOPLEVEL_X_CENTERED_KEY) &&
		g_settings_is_writable (toplevel->priv->settings,
					PANEL_TOPLEVEL_Y_CENTERED_KEY));
}

static gboolean
panel_toplevel_apply_delayed_settings (PanelToplevel *toplevel)
{
	g_settings_apply (toplevel->priv->delayed_settings);

	toplevel->priv->apply_delayed_id = 0;

	return FALSE;
}

static void
panel_toplevel_apply_delayed_settings_queue (PanelToplevel *toplevel)
{
	if (toplevel->priv->apply_delayed_id != 0)
		return;

	toplevel->priv->apply_delayed_id = g_timeout_add (500,
							  (GSourceFunc) panel_toplevel_apply_delayed_settings,
							  toplevel);
}

static void
panel_toplevel_bind_gsettings (PanelToplevel *toplevel)
{
	/* Delayed settings: the ones related to the position */

	g_settings_bind (toplevel->priv->delayed_settings,
			 PANEL_TOPLEVEL_MONITOR_KEY,
			 toplevel,
			 "monitor",
			 G_SETTINGS_BIND_DEFAULT|G_SETTINGS_BIND_NO_SENSITIVITY);

	g_settings_bind (toplevel->priv->delayed_settings,
			 PANEL_TOPLEVEL_SIZE_KEY,
			 toplevel,
			 "size",
			 G_SETTINGS_BIND_DEFAULT|G_SETTINGS_BIND_NO_SENSITIVITY);

	g_settings_bind (toplevel->priv->delayed_settings,
			 PANEL_TOPLEVEL_ORIENTATION_KEY,
			 toplevel,
			 "orientation",
			 G_SETTINGS_BIND_DEFAULT|G_SETTINGS_BIND_NO_SENSITIVITY);

	g_settings_bind (toplevel->priv->delayed_settings,
			 PANEL_TOPLEVEL_X_KEY,
			 toplevel,
			 "x",
			 G_SETTINGS_BIND_DEFAULT|G_SETTINGS_BIND_NO_SENSITIVITY);

	g_settings_bind (toplevel->priv->delayed_settings,
			 PANEL_TOPLEVEL_X_RIGHT_KEY,
			 toplevel,
			 "x-right",
			 G_SETTINGS_BIND_DEFAULT|G_SETTINGS_BIND_NO_SENSITIVITY);

	g_settings_bind (toplevel->priv->delayed_settings,
			 PANEL_TOPLEVEL_X_CENTERED_KEY,
			 toplevel,
			 "x-centered",
			 G_SETTINGS_BIND_DEFAULT|G_SETTINGS_BIND_NO_SENSITIVITY);

	g_settings_bind (toplevel->priv->delayed_settings,
			 PANEL_TOPLEVEL_Y_KEY,
			 toplevel,
			 "y",
			 G_SETTINGS_BIND_DEFAULT|G_SETTINGS_BIND_NO_SENSITIVITY);

	g_settings_bind (toplevel->priv->delayed_settings,
			 PANEL_TOPLEVEL_Y_BOTTOM_KEY,
			 toplevel,
			 "y-bottom",
			 G_SETTINGS_BIND_DEFAULT|G_SETTINGS_BIND_NO_SENSITIVITY);

	g_settings_bind (toplevel->priv->delayed_settings,
			 PANEL_TOPLEVEL_Y_CENTERED_KEY,
			 toplevel,
			 "y-centered",
			 G_SETTINGS_BIND_DEFAULT|G_SETTINGS_BIND_NO_SENSITIVITY);

	/* Normal settings */

	g_settings_bind (toplevel->priv->settings,
			 PANEL_TOPLEVEL_NAME_KEY,
			 toplevel,
			 "panel-name",
			 G_SETTINGS_BIND_DEFAULT|G_SETTINGS_BIND_NO_SENSITIVITY);

	g_settings_bind (toplevel->priv->settings,
			 PANEL_TOPLEVEL_EXPAND_KEY,
			 toplevel,
			 "expand",
			 G_SETTINGS_BIND_DEFAULT|G_SETTINGS_BIND_NO_SENSITIVITY);

	g_settings_bind (toplevel->priv->settings,
			 PANEL_TOPLEVEL_AUTO_HIDE_KEY,
			 toplevel,
			 "auto-hide",
			 G_SETTINGS_BIND_DEFAULT|G_SETTINGS_BIND_NO_SENSITIVITY);

	g_settings_bind (toplevel->priv->settings,
			 PANEL_TOPLEVEL_HIDE_DELAY_KEY,
			 toplevel,
			 "hide-delay",
			 G_SETTINGS_BIND_DEFAULT|G_SETTINGS_BIND_NO_SENSITIVITY);

	g_settings_bind (toplevel->priv->settings,
			 PANEL_TOPLEVEL_UNHIDE_DELAY_KEY,
			 toplevel,
			 "unhide-delay",
			 G_SETTINGS_BIND_DEFAULT|G_SETTINGS_BIND_NO_SENSITIVITY);

	g_settings_bind (toplevel->priv->settings,
			 PANEL_TOPLEVEL_AUTO_HIDE_SIZE_KEY,
			 toplevel,
			 "auto-hide-size",
			 G_SETTINGS_BIND_DEFAULT|G_SETTINGS_BIND_NO_SENSITIVITY);

	g_settings_bind (toplevel->priv->settings,
			 PANEL_TOPLEVEL_ANIMATION_SPEED_KEY,
			 toplevel,
			 "animation-speed",
			 G_SETTINGS_BIND_DEFAULT|G_SETTINGS_BIND_NO_SENSITIVITY);

	g_settings_bind (toplevel->priv->settings,
			 PANEL_TOPLEVEL_ENABLE_BUTTONS_KEY,
			 toplevel,
			 "buttons-enabled",
			 G_SETTINGS_BIND_DEFAULT|G_SETTINGS_BIND_NO_SENSITIVITY);

	/* FIXME: should be in panel_toplevel_add_hide_button */

	g_settings_bind (toplevel->priv->settings,
	                 PANEL_TOPLEVEL_ENABLE_ARROWS_KEY,
	                 toplevel->priv->hide_button_top, "arrow-visible",
	                 G_SETTINGS_BIND_GET | G_SETTINGS_BIND_NO_SENSITIVITY);

	g_settings_bind (toplevel->priv->settings,
	                 PANEL_TOPLEVEL_ENABLE_ARROWS_KEY,
	                 toplevel->priv->hide_button_bottom, "arrow-visible",
	                 G_SETTINGS_BIND_GET | G_SETTINGS_BIND_NO_SENSITIVITY);

	g_settings_bind (toplevel->priv->settings,
	                 PANEL_TOPLEVEL_ENABLE_ARROWS_KEY,
	                 toplevel->priv->hide_button_left, "arrow-visible",
	                 G_SETTINGS_BIND_GET | G_SETTINGS_BIND_NO_SENSITIVITY);

	g_settings_bind (toplevel->priv->settings,
	                 PANEL_TOPLEVEL_ENABLE_ARROWS_KEY,
	                 toplevel->priv->hide_button_right, "arrow-visible",
	                 G_SETTINGS_BIND_GET | G_SETTINGS_BIND_NO_SENSITIVITY);
}

static void
panel_toplevel_set_toplevel_id (PanelToplevel *toplevel,
				const char    *toplevel_id)
{
	g_assert (toplevel->priv->toplevel_id == NULL);

	toplevel->priv->toplevel_id = g_strdup (toplevel_id);
	gtk_widget_set_name (GTK_WIDGET (toplevel), toplevel_id);
}

const char *
panel_toplevel_get_id (PanelToplevel *toplevel)
{
	g_return_val_if_fail (PANEL_IS_TOPLEVEL (toplevel), NULL);

	return toplevel->priv->toplevel_id;
}

static void
panel_toplevel_set_settings_path (PanelToplevel *toplevel,
				  const char    *settings_path)
{
	g_assert (toplevel->priv->settings_path == NULL);
	g_assert (toplevel->priv->settings == NULL);
	g_assert (toplevel->priv->delayed_settings == NULL);

	toplevel->priv->settings_path = g_strdup (settings_path);
	toplevel->priv->settings = g_settings_new_with_path (PANEL_TOPLEVEL_SCHEMA,
							     settings_path);
	toplevel->priv->delayed_settings = g_settings_new_with_path (PANEL_TOPLEVEL_SCHEMA,
								     settings_path);
	g_settings_delay (toplevel->priv->delayed_settings);
}

static void
panel_toplevel_update_name (PanelToplevel *toplevel)
{
	char *title;

	g_assert (toplevel->priv->description != NULL);

	title = toplevel->priv->name ? toplevel->priv->name : toplevel->priv->description;

	gtk_window_set_title (GTK_WINDOW (toplevel), title);

	panel_a11y_set_atk_name_desc (
		GTK_WIDGET (toplevel->priv->panel_widget),
		title, toplevel->priv->description);
}

void
panel_toplevel_set_name (PanelToplevel *toplevel,
			 const char    *name)
{
	g_return_if_fail (PANEL_IS_TOPLEVEL (toplevel));

	if (!toplevel->priv->name && (!name || !name [0]))
		return;

	if (toplevel->priv->name && name && name [0] &&
	    !strcmp (toplevel->priv->name, name))
		return;

	if (toplevel->priv->name)
		g_free (toplevel->priv->name);
	toplevel->priv->name = NULL;

	if (name && name [0])
		toplevel->priv->name = g_strdup (name);

	panel_toplevel_update_name (toplevel);

	g_object_notify (G_OBJECT (toplevel), "panel-name");
}

const char *
panel_toplevel_get_name (PanelToplevel *toplevel)
{
	g_return_val_if_fail (PANEL_IS_TOPLEVEL (toplevel), NULL);

	return toplevel->priv->name;
}

const char *
panel_toplevel_get_description (PanelToplevel *toplevel)
{
	g_return_val_if_fail (PANEL_IS_TOPLEVEL (toplevel), NULL);

	return toplevel->priv->description;
}

void
panel_toplevel_set_expand (PanelToplevel *toplevel,
			   gboolean       expand)
{
	g_return_if_fail (PANEL_IS_TOPLEVEL (toplevel));

	expand = expand != FALSE;

	if (toplevel->priv->expand == expand)
		return;

	toplevel->priv->expand = expand;

	if (!toplevel->priv->expand &&
	    toplevel->priv->updated_geometry_initial) {
		switch (toplevel->priv->orientation) {
		case PANEL_ORIENTATION_TOP:
			panel_toplevel_set_x (toplevel, 0, -1, TRUE);
			break;
		case PANEL_ORIENTATION_BOTTOM:
			panel_toplevel_set_x (toplevel, 0, 0, TRUE);
			break;
		case PANEL_ORIENTATION_LEFT:
			panel_toplevel_set_y (toplevel, 0, -1, TRUE);
			break;
		case PANEL_ORIENTATION_RIGHT:
			panel_toplevel_set_y (toplevel, 0, 0, TRUE);
			break;
		default:
			g_assert_not_reached ();
			break;
		}
	}

	gtk_widget_queue_resize (GTK_WIDGET (toplevel));

	panel_widget_set_packed (toplevel->priv->panel_widget, !toplevel->priv->expand);

	g_object_notify (G_OBJECT (toplevel), "expand");
}

gboolean
panel_toplevel_get_expand (PanelToplevel *toplevel)
{
	g_return_val_if_fail (PANEL_IS_TOPLEVEL (toplevel), TRUE);

	return toplevel->priv->expand;
}

void
panel_toplevel_set_orientation (PanelToplevel    *toplevel,
				PanelOrientation  orientation)
{
	gboolean   rotate;
	int        monitor_width;
	int        monitor_height;

	g_return_if_fail (PANEL_IS_TOPLEVEL (toplevel));

	if (toplevel->priv->orientation == orientation)
		return;

	g_object_freeze_notify (G_OBJECT (toplevel));

	panel_toplevel_get_monitor_geometry (
		toplevel, NULL, NULL, &monitor_width, &monitor_height);

	/* Un-snap from center if no longer along screen edge */
	if (toplevel->priv->x_centered &&
	    (orientation & PANEL_VERTICAL_MASK)) {
		toplevel->priv->x_centered = FALSE;
		toplevel->priv->x = (monitor_width - toplevel->priv->geometry.width) / 2;
		g_object_notify (G_OBJECT (toplevel), "x");
		g_object_notify (G_OBJECT (toplevel), "x-centered");

		if (toplevel->priv->x_right != -1) {
			toplevel->priv->x_right = -1;
			g_object_notify (G_OBJECT (toplevel), "x-right");
		}
	}

	if (toplevel->priv->y_centered &&
	    (orientation & PANEL_HORIZONTAL_MASK)) {
		toplevel->priv->y_centered = FALSE;
		toplevel->priv->y = (monitor_height - toplevel->priv->geometry.height) / 2;
		g_object_notify (G_OBJECT (toplevel), "y");
		g_object_notify (G_OBJECT (toplevel), "y-centered");

		if (toplevel->priv->y_bottom != -1) {
			toplevel->priv->y_bottom = -1;
			g_object_notify (G_OBJECT (toplevel), "y-bottom");
		}
	}

	rotate = FALSE;
	if ((orientation & PANEL_HORIZONTAL_MASK) &&
	    (toplevel->priv->orientation & PANEL_VERTICAL_MASK))
		rotate = TRUE;
	else if ((orientation & PANEL_VERTICAL_MASK) &&
		 (toplevel->priv->orientation & PANEL_HORIZONTAL_MASK))
		rotate = TRUE;

	/* rotate around the center */
	if (rotate && !toplevel->priv->position_centered && !toplevel->priv->expand &&
	    toplevel->priv->updated_geometry_initial) {
		toplevel->priv->position_centered = TRUE;

		/* x, y temporary refer to the panel center, so we don't care
		 * about x_right, y_bottom. Those will get updated in
		 * panel_toplevel_update_position() accordingly. */
		if (!toplevel->priv->x_centered) {
			toplevel->priv->x += toplevel->priv->geometry.width  / 2;
			g_object_notify (G_OBJECT (toplevel), "x");
		}

		if (!toplevel->priv->y_centered) {
			toplevel->priv->y += toplevel->priv->geometry.height / 2;
			g_object_notify (G_OBJECT (toplevel), "y");
		}

	}

	toplevel->priv->orientation = orientation;
	update_style_classes (toplevel);

	gtk_widget_reset_style (GTK_WIDGET (toplevel));

	panel_toplevel_update_hide_buttons (toplevel);

	panel_widget_set_orientation (	
		toplevel->priv->panel_widget,
		toplevel->priv->orientation & PANEL_HORIZONTAL_MASK ?
					GTK_ORIENTATION_HORIZONTAL :
					GTK_ORIENTATION_VERTICAL);

	switch (toplevel->priv->state) {
	case PANEL_STATE_HIDDEN_UP:
		if (toplevel->priv->orientation & PANEL_HORIZONTAL_MASK)
			toplevel->priv->state = PANEL_STATE_HIDDEN_LEFT;
		break;
	case PANEL_STATE_HIDDEN_DOWN:
		if (toplevel->priv->orientation & PANEL_HORIZONTAL_MASK)
			toplevel->priv->state = PANEL_STATE_HIDDEN_RIGHT;
		break;
	case PANEL_STATE_HIDDEN_LEFT:
		if (toplevel->priv->orientation & PANEL_VERTICAL_MASK)
			toplevel->priv->state = PANEL_STATE_HIDDEN_UP;
		break;
	case PANEL_STATE_HIDDEN_RIGHT:
		if (toplevel->priv->orientation & PANEL_VERTICAL_MASK)
			toplevel->priv->state = PANEL_STATE_HIDDEN_DOWN;
		break;
	case PANEL_STATE_NORMAL:
	case PANEL_STATE_AUTO_HIDDEN:
	default:
		break;
	}

	gtk_widget_queue_resize (GTK_WIDGET (toplevel));

	panel_toplevel_apply_delayed_settings_queue (toplevel);
	g_object_notify (G_OBJECT (toplevel), "orientation");

	g_object_thaw_notify (G_OBJECT (toplevel));
}

PanelOrientation
panel_toplevel_get_orientation (PanelToplevel *toplevel)
{
	g_return_val_if_fail (PANEL_IS_TOPLEVEL (toplevel), PANEL_ORIENTATION_TOP);

	return toplevel->priv->orientation;
}

void
panel_toplevel_set_size (PanelToplevel *toplevel,
			 int            size)
{
	g_return_if_fail (PANEL_IS_TOPLEVEL (toplevel));
	g_return_if_fail (size >= 0);

	if (toplevel->priv->size == size)
		return;

	toplevel->priv->size = size;

	panel_widget_set_size (toplevel->priv->panel_widget, toplevel->priv->size);

	gtk_widget_queue_resize (GTK_WIDGET (toplevel));

	panel_toplevel_apply_delayed_settings_queue (toplevel);
	g_object_notify (G_OBJECT (toplevel), "size");
}

int
panel_toplevel_get_size (PanelToplevel *toplevel)
{
	g_return_val_if_fail (PANEL_IS_TOPLEVEL (toplevel), DEFAULT_SIZE);

	return toplevel->priv->size;
}

void
panel_toplevel_set_auto_hide_size (PanelToplevel *toplevel,
				   int            auto_hide_size)
{
	g_return_if_fail (PANEL_IS_TOPLEVEL (toplevel));

	if (toplevel->priv->auto_hide_size == auto_hide_size)
		return;

	toplevel->priv->auto_hide_size = auto_hide_size;

	if (toplevel->priv->state == PANEL_STATE_AUTO_HIDDEN) {
		if (panel_toplevel_update_struts (toplevel, FALSE)) {
			if (toplevel->priv->animate) {
				GtkDirectionType dir;

				dir = toplevel->priv->orientation & PANEL_VERTICAL_MASK ?
				      GTK_DIR_UP : GTK_DIR_LEFT;

				panel_toplevel_unhide (toplevel);
				panel_toplevel_hide (toplevel, TRUE, dir);
			} else
				gtk_widget_queue_resize (GTK_WIDGET (toplevel));
		}
	}

	g_object_notify (G_OBJECT (toplevel), "auto-hide-size");
}

void
panel_toplevel_set_x (PanelToplevel *toplevel,
		      int            x,
		      int            x_right,
		      gboolean       x_centered)
{
	gboolean changed = FALSE;

	g_return_if_fail (PANEL_IS_TOPLEVEL (toplevel));

	x_centered = x_centered != FALSE;

	g_object_freeze_notify (G_OBJECT (toplevel));

	if (toplevel->priv->x != x) {
		toplevel->priv->x = x;
		changed = TRUE;
		g_object_notify (G_OBJECT (toplevel), "x");
	}

	if (toplevel->priv->x_right != x_right) {
		toplevel->priv->x_right = x_right;
		changed = TRUE;
		g_object_notify (G_OBJECT (toplevel), "x-right");
	}

	if (toplevel->priv->x_centered != x_centered) {
		toplevel->priv->x_centered = x_centered;
		changed = TRUE;
		g_object_notify (G_OBJECT (toplevel), "x-centered");
	}

	if (changed) {
		panel_toplevel_apply_delayed_settings_queue (toplevel);
		gtk_widget_queue_resize (GTK_WIDGET (toplevel));
	}

	g_object_thaw_notify (G_OBJECT (toplevel));
}

void
panel_toplevel_set_y (PanelToplevel *toplevel,
		      int            y,
		      int            y_bottom,
		      gboolean       y_centered)
{
	gboolean changed = FALSE;

	g_return_if_fail (PANEL_IS_TOPLEVEL (toplevel));

	y_centered = y_centered != FALSE;

	g_object_freeze_notify (G_OBJECT (toplevel));

	if (toplevel->priv->y != y) {
		toplevel->priv->y = y;
		changed = TRUE;
		g_object_notify (G_OBJECT (toplevel), "y");
	}

	if (toplevel->priv->y_bottom != y_bottom) {
		toplevel->priv->y_bottom = y_bottom;
		changed = TRUE;
		g_object_notify (G_OBJECT (toplevel), "y-bottom");
	}

	if (toplevel->priv->y_centered != y_centered) {
		toplevel->priv->y_centered = y_centered;
		changed = TRUE;
		g_object_notify (G_OBJECT (toplevel), "y-centered");
	}

	if (changed) {
		panel_toplevel_apply_delayed_settings_queue (toplevel);
		gtk_widget_queue_resize (GTK_WIDGET (toplevel));
	}

	g_object_thaw_notify (G_OBJECT (toplevel));
}

/**
 * panel_toplevel_set_monitor_internal:
 *
 * Sets the monitor of the toplevel, but only the internal state. We need to
 * make the difference between the internal state and the configuration of the
 * user because internal constraints might affect the monitor of the toplevel.
 *
 * panel_toplevel_set_monitor_internal() won't update the configuration of the
 * user.
 **/
static void
panel_toplevel_set_monitor_internal (PanelToplevel *toplevel,
				     int            monitor,
				     gboolean       force_resize)
{
	if (toplevel->priv->monitor == monitor)
		return;

	toplevel->priv->monitor = monitor;

	if (force_resize)
		gtk_widget_queue_resize (GTK_WIDGET (toplevel));
}

/**
 * panel_toplevel_update_monitor:
 *
 * Moves the toplevel to its configured monitor or the first one, if needed.
 * This generally happens when the configured monitor was non-existing before,
 * and it appeared at runtime, or if it was existing and disappeared.
 *
 * This must only be called at the beginning of the size request of the
 * toplevel because it doesn't queue a size request.
 **/
static void
panel_toplevel_update_monitor (PanelToplevel *toplevel)
{
	GdkDisplay *display;

	display = gdk_display_get_default ();

	/* If we were not using the configured monitor, can we use it now? */
	if ((toplevel->priv->configured_monitor != -1) &&
	    (toplevel->priv->configured_monitor != toplevel->priv->monitor) &&
	    toplevel->priv->configured_monitor < gdk_display_get_n_monitors (display)) {
		panel_toplevel_set_monitor_internal (toplevel,
						     toplevel->priv->configured_monitor,
						     FALSE);

	/* else, can we still use the monitor we were using? */
	} else if (toplevel->priv->monitor >= gdk_display_get_n_monitors (display)) {
		panel_toplevel_set_monitor_internal (toplevel,
						     0,
						     FALSE);
	}
}

void
panel_toplevel_set_monitor (PanelToplevel *toplevel,
			    int            monitor)
{
	GdkDisplay *display;

	g_return_if_fail (PANEL_IS_TOPLEVEL (toplevel));

	if (toplevel->priv->configured_monitor == monitor)
		return;

	toplevel->priv->configured_monitor = monitor;

	/* Only use the configured monitor if it's existing. Else, we ignore
	 * the non-existing monitor, and keep the old one. The main use case is
	 * when logging in after having used a multiscreen environment.
	 * We will put the panel on the monitor 0 for this session, and it will
	 * move back to the right monitor next time. */
	display = gdk_display_get_default ();

	if (monitor < gdk_display_get_n_monitors (display))
		panel_toplevel_set_monitor_internal (toplevel, monitor, TRUE);

	panel_toplevel_apply_delayed_settings_queue (toplevel);
	g_object_notify (G_OBJECT (toplevel), "monitor");
}

int
panel_toplevel_get_monitor (PanelToplevel *toplevel)
{
	g_return_val_if_fail (PANEL_IS_TOPLEVEL (toplevel), -1);

	return toplevel->priv->monitor;
}

void
panel_toplevel_set_auto_hide (PanelToplevel *toplevel,
			      gboolean       auto_hide)
{
	g_return_if_fail (PANEL_IS_TOPLEVEL (toplevel));

	auto_hide = auto_hide != FALSE;

	if (toplevel->priv->auto_hide == auto_hide)
		return;

	toplevel->priv->auto_hide = auto_hide;

	if (toplevel->priv->auto_hide)
		panel_toplevel_queue_auto_hide (toplevel);
	else
		panel_toplevel_queue_auto_unhide (toplevel);

	if (panel_toplevel_update_struts (toplevel, FALSE))
		gtk_widget_queue_resize (GTK_WIDGET (toplevel));

	g_object_notify (G_OBJECT (toplevel), "auto-hide");
}

void
panel_toplevel_set_hide_delay (PanelToplevel *toplevel,
			       int            hide_delay)
{
	g_return_if_fail (PANEL_IS_TOPLEVEL (toplevel));

	if (toplevel->priv->hide_delay == hide_delay)
		return;

	toplevel->priv->hide_delay = hide_delay;

	g_object_notify (G_OBJECT (toplevel), "hide-delay");
}

void
panel_toplevel_set_unhide_delay (PanelToplevel *toplevel,
				 int            unhide_delay)
{
	g_return_if_fail (PANEL_IS_TOPLEVEL (toplevel));

	if (toplevel->priv->unhide_delay == unhide_delay)
		return;

	toplevel->priv->unhide_delay = unhide_delay;

	g_object_notify (G_OBJECT (toplevel), "unhide-delay");
}

static void
panel_toplevel_set_animate (PanelToplevel *toplevel,
			    gboolean       animate)
{
	g_return_if_fail (PANEL_IS_TOPLEVEL (toplevel));

	animate = animate != FALSE;

	if (toplevel->priv->animate == animate)
		return;

	toplevel->priv->animate = animate;

	g_object_notify (G_OBJECT (toplevel), "animate");
}

void
panel_toplevel_set_animation_speed (PanelToplevel       *toplevel,
				    PanelAnimationSpeed  animation_speed)
{
	g_return_if_fail (PANEL_IS_TOPLEVEL (toplevel));

	if (toplevel->priv->animation_speed == animation_speed)
		return;

	toplevel->priv->animation_speed = animation_speed;

	g_object_notify (G_OBJECT (toplevel), "animation-speed");
}

void
panel_toplevel_set_enable_buttons (PanelToplevel *toplevel,
				   gboolean       enable_buttons)
{
	enable_buttons = enable_buttons != FALSE;

	if (toplevel->priv->buttons_enabled == enable_buttons)
		return;

	toplevel->priv->buttons_enabled = enable_buttons;

	panel_toplevel_update_hide_buttons (toplevel);

	g_object_notify (G_OBJECT (toplevel), "buttons-enabled");
}

gboolean
panel_toplevel_get_is_hidden (PanelToplevel *toplevel)
{
	g_return_val_if_fail (PANEL_IS_TOPLEVEL (toplevel), FALSE);

	if (toplevel->priv->state == PANEL_STATE_HIDDEN_UP   ||
	    toplevel->priv->state == PANEL_STATE_HIDDEN_DOWN ||
	    toplevel->priv->state == PANEL_STATE_HIDDEN_LEFT ||
	    toplevel->priv->state == PANEL_STATE_HIDDEN_RIGHT)
		return TRUE;

	return FALSE;
}

int
panel_toplevel_get_maximum_size (PanelToplevel *toplevel)
{
	int monitor_width, monitor_height;

	panel_toplevel_get_monitor_geometry (toplevel, NULL, NULL,
					     &monitor_width, &monitor_height);

	if (toplevel->priv->orientation & PANEL_HORIZONTAL_MASK)
		return monitor_height / MAXIMUM_SIZE_SCREEN_RATIO;
	else
		return monitor_width / MAXIMUM_SIZE_SCREEN_RATIO;
}
