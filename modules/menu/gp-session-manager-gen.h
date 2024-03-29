/*
 * This file is generated by gdbus-codegen, do not modify it.
 *
 * The license of this code is the same as for the D-Bus interface description
 * it was derived from. Note that it links to GLib, so must comply with the
 * LGPL linking clauses.
 */

#ifndef __GP_SESSION_MANAGER_GEN_H__
#define __GP_SESSION_MANAGER_GEN_H__

#include <gio/gio.h>

G_BEGIN_DECLS


/* ------------------------------------------------------------------------ */
/* Declarations for org.gnome.SessionManager */

#define GP_TYPE_SESSION_MANAGER_GEN (gp_session_manager_gen_get_type ())
#define GP_SESSION_MANAGER_GEN(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), GP_TYPE_SESSION_MANAGER_GEN, GpSessionManagerGen))
#define GP_IS_SESSION_MANAGER_GEN(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), GP_TYPE_SESSION_MANAGER_GEN))
#define GP_SESSION_MANAGER_GEN_GET_IFACE(o) (G_TYPE_INSTANCE_GET_INTERFACE ((o), GP_TYPE_SESSION_MANAGER_GEN, GpSessionManagerGenIface))

struct _GpSessionManagerGen;
typedef struct _GpSessionManagerGen GpSessionManagerGen;
typedef struct _GpSessionManagerGenIface GpSessionManagerGenIface;

struct _GpSessionManagerGenIface
{
  GTypeInterface parent_iface;

  gboolean (*handle_can_reboot_to_firmware_setup) (
    GpSessionManagerGen *object,
    GDBusMethodInvocation *invocation);

  gboolean (*handle_can_shutdown) (
    GpSessionManagerGen *object,
    GDBusMethodInvocation *invocation);

  gboolean (*handle_logout) (
    GpSessionManagerGen *object,
    GDBusMethodInvocation *invocation,
    guint arg_mode);

  gboolean (*handle_reboot) (
    GpSessionManagerGen *object,
    GDBusMethodInvocation *invocation);

  gboolean (*handle_set_reboot_to_firmware_setup) (
    GpSessionManagerGen *object,
    GDBusMethodInvocation *invocation,
    gboolean arg_enable);

  gboolean (*handle_shutdown) (
    GpSessionManagerGen *object,
    GDBusMethodInvocation *invocation);

};

GType gp_session_manager_gen_get_type (void) G_GNUC_CONST;

GDBusInterfaceInfo *gp_session_manager_gen_interface_info (void);
guint gp_session_manager_gen_override_properties (GObjectClass *klass, guint property_id_begin);


/* D-Bus method call completion functions: */
void gp_session_manager_gen_complete_shutdown (
    GpSessionManagerGen *object,
    GDBusMethodInvocation *invocation);

void gp_session_manager_gen_complete_reboot (
    GpSessionManagerGen *object,
    GDBusMethodInvocation *invocation);

void gp_session_manager_gen_complete_can_shutdown (
    GpSessionManagerGen *object,
    GDBusMethodInvocation *invocation,
    gboolean is_available);

void gp_session_manager_gen_complete_set_reboot_to_firmware_setup (
    GpSessionManagerGen *object,
    GDBusMethodInvocation *invocation);

void gp_session_manager_gen_complete_can_reboot_to_firmware_setup (
    GpSessionManagerGen *object,
    GDBusMethodInvocation *invocation,
    gboolean is_available);

void gp_session_manager_gen_complete_logout (
    GpSessionManagerGen *object,
    GDBusMethodInvocation *invocation);



/* D-Bus method calls: */
void gp_session_manager_gen_call_shutdown (
    GpSessionManagerGen *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean gp_session_manager_gen_call_shutdown_finish (
    GpSessionManagerGen *proxy,
    GAsyncResult *res,
    GError **error);

gboolean gp_session_manager_gen_call_shutdown_sync (
    GpSessionManagerGen *proxy,
    GCancellable *cancellable,
    GError **error);

void gp_session_manager_gen_call_reboot (
    GpSessionManagerGen *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean gp_session_manager_gen_call_reboot_finish (
    GpSessionManagerGen *proxy,
    GAsyncResult *res,
    GError **error);

gboolean gp_session_manager_gen_call_reboot_sync (
    GpSessionManagerGen *proxy,
    GCancellable *cancellable,
    GError **error);

void gp_session_manager_gen_call_can_shutdown (
    GpSessionManagerGen *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean gp_session_manager_gen_call_can_shutdown_finish (
    GpSessionManagerGen *proxy,
    gboolean *out_is_available,
    GAsyncResult *res,
    GError **error);

gboolean gp_session_manager_gen_call_can_shutdown_sync (
    GpSessionManagerGen *proxy,
    gboolean *out_is_available,
    GCancellable *cancellable,
    GError **error);

void gp_session_manager_gen_call_set_reboot_to_firmware_setup (
    GpSessionManagerGen *proxy,
    gboolean arg_enable,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean gp_session_manager_gen_call_set_reboot_to_firmware_setup_finish (
    GpSessionManagerGen *proxy,
    GAsyncResult *res,
    GError **error);

gboolean gp_session_manager_gen_call_set_reboot_to_firmware_setup_sync (
    GpSessionManagerGen *proxy,
    gboolean arg_enable,
    GCancellable *cancellable,
    GError **error);

void gp_session_manager_gen_call_can_reboot_to_firmware_setup (
    GpSessionManagerGen *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean gp_session_manager_gen_call_can_reboot_to_firmware_setup_finish (
    GpSessionManagerGen *proxy,
    gboolean *out_is_available,
    GAsyncResult *res,
    GError **error);

gboolean gp_session_manager_gen_call_can_reboot_to_firmware_setup_sync (
    GpSessionManagerGen *proxy,
    gboolean *out_is_available,
    GCancellable *cancellable,
    GError **error);

void gp_session_manager_gen_call_logout (
    GpSessionManagerGen *proxy,
    guint arg_mode,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean gp_session_manager_gen_call_logout_finish (
    GpSessionManagerGen *proxy,
    GAsyncResult *res,
    GError **error);

gboolean gp_session_manager_gen_call_logout_sync (
    GpSessionManagerGen *proxy,
    guint arg_mode,
    GCancellable *cancellable,
    GError **error);



/* ---- */

#define GP_TYPE_SESSION_MANAGER_GEN_PROXY (gp_session_manager_gen_proxy_get_type ())
#define GP_SESSION_MANAGER_GEN_PROXY(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), GP_TYPE_SESSION_MANAGER_GEN_PROXY, GpSessionManagerGenProxy))
#define GP_SESSION_MANAGER_GEN_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), GP_TYPE_SESSION_MANAGER_GEN_PROXY, GpSessionManagerGenProxyClass))
#define GP_SESSION_MANAGER_GEN_PROXY_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), GP_TYPE_SESSION_MANAGER_GEN_PROXY, GpSessionManagerGenProxyClass))
#define GP_IS_SESSION_MANAGER_GEN_PROXY(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), GP_TYPE_SESSION_MANAGER_GEN_PROXY))
#define GP_IS_SESSION_MANAGER_GEN_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), GP_TYPE_SESSION_MANAGER_GEN_PROXY))

typedef struct _GpSessionManagerGenProxy GpSessionManagerGenProxy;
typedef struct _GpSessionManagerGenProxyClass GpSessionManagerGenProxyClass;
typedef struct _GpSessionManagerGenProxyPrivate GpSessionManagerGenProxyPrivate;

struct _GpSessionManagerGenProxy
{
  /*< private >*/
  GDBusProxy parent_instance;
  GpSessionManagerGenProxyPrivate *priv;
};

struct _GpSessionManagerGenProxyClass
{
  GDBusProxyClass parent_class;
};

GType gp_session_manager_gen_proxy_get_type (void) G_GNUC_CONST;

#if GLIB_CHECK_VERSION(2, 44, 0)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GpSessionManagerGenProxy, g_object_unref)
#endif

void gp_session_manager_gen_proxy_new (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
GpSessionManagerGen *gp_session_manager_gen_proxy_new_finish (
    GAsyncResult        *res,
    GError             **error);
GpSessionManagerGen *gp_session_manager_gen_proxy_new_sync (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);

void gp_session_manager_gen_proxy_new_for_bus (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
GpSessionManagerGen *gp_session_manager_gen_proxy_new_for_bus_finish (
    GAsyncResult        *res,
    GError             **error);
GpSessionManagerGen *gp_session_manager_gen_proxy_new_for_bus_sync (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);


/* ---- */

#define GP_TYPE_SESSION_MANAGER_GEN_SKELETON (gp_session_manager_gen_skeleton_get_type ())
#define GP_SESSION_MANAGER_GEN_SKELETON(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), GP_TYPE_SESSION_MANAGER_GEN_SKELETON, GpSessionManagerGenSkeleton))
#define GP_SESSION_MANAGER_GEN_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), GP_TYPE_SESSION_MANAGER_GEN_SKELETON, GpSessionManagerGenSkeletonClass))
#define GP_SESSION_MANAGER_GEN_SKELETON_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), GP_TYPE_SESSION_MANAGER_GEN_SKELETON, GpSessionManagerGenSkeletonClass))
#define GP_IS_SESSION_MANAGER_GEN_SKELETON(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), GP_TYPE_SESSION_MANAGER_GEN_SKELETON))
#define GP_IS_SESSION_MANAGER_GEN_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), GP_TYPE_SESSION_MANAGER_GEN_SKELETON))

typedef struct _GpSessionManagerGenSkeleton GpSessionManagerGenSkeleton;
typedef struct _GpSessionManagerGenSkeletonClass GpSessionManagerGenSkeletonClass;
typedef struct _GpSessionManagerGenSkeletonPrivate GpSessionManagerGenSkeletonPrivate;

struct _GpSessionManagerGenSkeleton
{
  /*< private >*/
  GDBusInterfaceSkeleton parent_instance;
  GpSessionManagerGenSkeletonPrivate *priv;
};

struct _GpSessionManagerGenSkeletonClass
{
  GDBusInterfaceSkeletonClass parent_class;
};

GType gp_session_manager_gen_skeleton_get_type (void) G_GNUC_CONST;

#if GLIB_CHECK_VERSION(2, 44, 0)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GpSessionManagerGenSkeleton, g_object_unref)
#endif

GpSessionManagerGen *gp_session_manager_gen_skeleton_new (void);


G_END_DECLS

#endif /* __GP_SESSION_MANAGER_GEN_H__ */
