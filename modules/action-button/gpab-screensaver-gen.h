/*
 * This file is generated by gdbus-codegen, do not modify it.
 *
 * The license of this code is the same as for the D-Bus interface description
 * it was derived from. Note that it links to GLib, so must comply with the
 * LGPL linking clauses.
 */

#ifndef __GPAB_SCREENSAVER_GEN_H__
#define __GPAB_SCREENSAVER_GEN_H__

#include <gio/gio.h>

G_BEGIN_DECLS


/* ------------------------------------------------------------------------ */
/* Declarations for org.gnome.ScreenSaver */

#define GPAB_TYPE_SCREENSAVER_GEN (gpab_screensaver_gen_get_type ())
#define GPAB_SCREENSAVER_GEN(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), GPAB_TYPE_SCREENSAVER_GEN, GpabScreensaverGen))
#define GPAB_IS_SCREENSAVER_GEN(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), GPAB_TYPE_SCREENSAVER_GEN))
#define GPAB_SCREENSAVER_GEN_GET_IFACE(o) (G_TYPE_INSTANCE_GET_INTERFACE ((o), GPAB_TYPE_SCREENSAVER_GEN, GpabScreensaverGenIface))

struct _GpabScreensaverGen;
typedef struct _GpabScreensaverGen GpabScreensaverGen;
typedef struct _GpabScreensaverGenIface GpabScreensaverGenIface;

struct _GpabScreensaverGenIface
{
  GTypeInterface parent_iface;

  gboolean (*handle_lock) (
    GpabScreensaverGen *object,
    GDBusMethodInvocation *invocation);

  gboolean (*handle_set_active) (
    GpabScreensaverGen *object,
    GDBusMethodInvocation *invocation,
    gboolean arg_value);

};

GType gpab_screensaver_gen_get_type (void) G_GNUC_CONST;

GDBusInterfaceInfo *gpab_screensaver_gen_interface_info (void);
guint gpab_screensaver_gen_override_properties (GObjectClass *klass, guint property_id_begin);


/* D-Bus method call completion functions: */
void gpab_screensaver_gen_complete_lock (
    GpabScreensaverGen *object,
    GDBusMethodInvocation *invocation);

void gpab_screensaver_gen_complete_set_active (
    GpabScreensaverGen *object,
    GDBusMethodInvocation *invocation);



/* D-Bus method calls: */
void gpab_screensaver_gen_call_lock (
    GpabScreensaverGen *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean gpab_screensaver_gen_call_lock_finish (
    GpabScreensaverGen *proxy,
    GAsyncResult *res,
    GError **error);

gboolean gpab_screensaver_gen_call_lock_sync (
    GpabScreensaverGen *proxy,
    GCancellable *cancellable,
    GError **error);

void gpab_screensaver_gen_call_set_active (
    GpabScreensaverGen *proxy,
    gboolean arg_value,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean gpab_screensaver_gen_call_set_active_finish (
    GpabScreensaverGen *proxy,
    GAsyncResult *res,
    GError **error);

gboolean gpab_screensaver_gen_call_set_active_sync (
    GpabScreensaverGen *proxy,
    gboolean arg_value,
    GCancellable *cancellable,
    GError **error);



/* ---- */

#define GPAB_TYPE_SCREENSAVER_GEN_PROXY (gpab_screensaver_gen_proxy_get_type ())
#define GPAB_SCREENSAVER_GEN_PROXY(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), GPAB_TYPE_SCREENSAVER_GEN_PROXY, GpabScreensaverGenProxy))
#define GPAB_SCREENSAVER_GEN_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), GPAB_TYPE_SCREENSAVER_GEN_PROXY, GpabScreensaverGenProxyClass))
#define GPAB_SCREENSAVER_GEN_PROXY_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), GPAB_TYPE_SCREENSAVER_GEN_PROXY, GpabScreensaverGenProxyClass))
#define GPAB_IS_SCREENSAVER_GEN_PROXY(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), GPAB_TYPE_SCREENSAVER_GEN_PROXY))
#define GPAB_IS_SCREENSAVER_GEN_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), GPAB_TYPE_SCREENSAVER_GEN_PROXY))

typedef struct _GpabScreensaverGenProxy GpabScreensaverGenProxy;
typedef struct _GpabScreensaverGenProxyClass GpabScreensaverGenProxyClass;
typedef struct _GpabScreensaverGenProxyPrivate GpabScreensaverGenProxyPrivate;

struct _GpabScreensaverGenProxy
{
  /*< private >*/
  GDBusProxy parent_instance;
  GpabScreensaverGenProxyPrivate *priv;
};

struct _GpabScreensaverGenProxyClass
{
  GDBusProxyClass parent_class;
};

GType gpab_screensaver_gen_proxy_get_type (void) G_GNUC_CONST;

#if GLIB_CHECK_VERSION(2, 44, 0)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GpabScreensaverGenProxy, g_object_unref)
#endif

void gpab_screensaver_gen_proxy_new (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
GpabScreensaverGen *gpab_screensaver_gen_proxy_new_finish (
    GAsyncResult        *res,
    GError             **error);
GpabScreensaverGen *gpab_screensaver_gen_proxy_new_sync (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);

void gpab_screensaver_gen_proxy_new_for_bus (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
GpabScreensaverGen *gpab_screensaver_gen_proxy_new_for_bus_finish (
    GAsyncResult        *res,
    GError             **error);
GpabScreensaverGen *gpab_screensaver_gen_proxy_new_for_bus_sync (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);


/* ---- */

#define GPAB_TYPE_SCREENSAVER_GEN_SKELETON (gpab_screensaver_gen_skeleton_get_type ())
#define GPAB_SCREENSAVER_GEN_SKELETON(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), GPAB_TYPE_SCREENSAVER_GEN_SKELETON, GpabScreensaverGenSkeleton))
#define GPAB_SCREENSAVER_GEN_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), GPAB_TYPE_SCREENSAVER_GEN_SKELETON, GpabScreensaverGenSkeletonClass))
#define GPAB_SCREENSAVER_GEN_SKELETON_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), GPAB_TYPE_SCREENSAVER_GEN_SKELETON, GpabScreensaverGenSkeletonClass))
#define GPAB_IS_SCREENSAVER_GEN_SKELETON(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), GPAB_TYPE_SCREENSAVER_GEN_SKELETON))
#define GPAB_IS_SCREENSAVER_GEN_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), GPAB_TYPE_SCREENSAVER_GEN_SKELETON))

typedef struct _GpabScreensaverGenSkeleton GpabScreensaverGenSkeleton;
typedef struct _GpabScreensaverGenSkeletonClass GpabScreensaverGenSkeletonClass;
typedef struct _GpabScreensaverGenSkeletonPrivate GpabScreensaverGenSkeletonPrivate;

struct _GpabScreensaverGenSkeleton
{
  /*< private >*/
  GDBusInterfaceSkeleton parent_instance;
  GpabScreensaverGenSkeletonPrivate *priv;
};

struct _GpabScreensaverGenSkeletonClass
{
  GDBusInterfaceSkeletonClass parent_class;
};

GType gpab_screensaver_gen_skeleton_get_type (void) G_GNUC_CONST;

#if GLIB_CHECK_VERSION(2, 44, 0)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GpabScreensaverGenSkeleton, g_object_unref)
#endif

GpabScreensaverGen *gpab_screensaver_gen_skeleton_new (void);


G_END_DECLS

#endif /* __GPAB_SCREENSAVER_GEN_H__ */