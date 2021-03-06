/*
 * GNOME panel launcher module.
 * (C) 1997 The Free Software Foundation
 *
 * Authors: Miguel de Icaza
 *          Federico Mena
 * CORBAized by George Lebl
 * de-CORBAized by George Lebl
 */

#ifndef LAUNCHER_H
#define LAUNCHER_H

#include "applet.h"
#include "panel-widget.h"

G_BEGIN_DECLS

typedef struct {
	AppletInfo        *info;
	GtkWidget         *button;

	char              *location;
	GKeyFile          *key_file;

	GtkWidget         *prop_dialog;
	GSList 		  *error_dialogs;

	gulong             destroy_handler;
} Launcher;

void panel_launcher_create           (PanelToplevel       *toplevel,
				      PanelObjectPackType  pack_type,
				      int                  pack_index,
				      const char          *location);
gboolean panel_launcher_create_copy  (PanelToplevel       *toplevel,
				      PanelObjectPackType  pack_type,
				      int                  pack_index,
				      const char          *location);
void panel_launcher_create_from_info (PanelToplevel       *toplevel,
				      PanelObjectPackType  pack_type,
				      int                  pack_index,
				      gboolean             exec_info,
				      const char          *exec_or_uri,
				      const char          *name,
				      const char          *comment,
				      const char          *icon);

void		launcher_launch    		(Launcher  *launcher,
						 GtkWidget *widget);

void		launcher_properties		(Launcher  *launcher);

void            launcher_load                   (PanelWidget *panel_widget,
						 const char  *id,
						 GSettings   *settings);

void            panel_launcher_delete           (Launcher *launcher);

void		ask_about_launcher		(const char *file,
						 PanelWidget *panel,
						 PanelObjectPackType pack_type);

Launcher *	find_launcher			(const char *path);

void            launcher_properties_destroy     (Launcher *launcher);

void            panel_launcher_set_dnd_enabled  (Launcher *launcher,
						 gboolean  dnd_enabled);


G_END_DECLS

#endif
