/*
 * Generated by gdbus-codegen 2.57.2 from org.freedesktop.login1.Manager.xml. DO NOT EDIT.
 *
 * The license of this code is the same as for the D-Bus interface description
 * it was derived from.
 */

#ifndef __GP_LOGIN1_MANAGER_GEN_H__
#define __GP_LOGIN1_MANAGER_GEN_H__

#include <gio/gio.h>

G_BEGIN_DECLS


/* ------------------------------------------------------------------------ */
/* Declarations for org.freedesktop.login1.Manager */

#define GP_TYPE_LOGIN1_MANAGER_GEN (gp_login1_manager_gen_get_type ())
#define GP_LOGIN1_MANAGER_GEN(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), GP_TYPE_LOGIN1_MANAGER_GEN, GpLogin1ManagerGen))
#define GP_IS_LOGIN1_MANAGER_GEN(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), GP_TYPE_LOGIN1_MANAGER_GEN))
#define GP_LOGIN1_MANAGER_GEN_GET_IFACE(o) (G_TYPE_INSTANCE_GET_INTERFACE ((o), GP_TYPE_LOGIN1_MANAGER_GEN, GpLogin1ManagerGenIface))

struct _GpLogin1ManagerGen;
typedef struct _GpLogin1ManagerGen GpLogin1ManagerGen;
typedef struct _GpLogin1ManagerGenIface GpLogin1ManagerGenIface;

struct _GpLogin1ManagerGenIface
{
  GTypeInterface parent_iface;

  gboolean (*handle_can_hibernate) (
    GpLogin1ManagerGen *object,
    GDBusMethodInvocation *invocation);

  gboolean (*handle_can_hybrid_sleep) (
    GpLogin1ManagerGen *object,
    GDBusMethodInvocation *invocation);

  gboolean (*handle_can_suspend) (
    GpLogin1ManagerGen *object,
    GDBusMethodInvocation *invocation);

  gboolean (*handle_hibernate) (
    GpLogin1ManagerGen *object,
    GDBusMethodInvocation *invocation,
    gboolean arg_interactive);

  gboolean (*handle_hybrid_sleep) (
    GpLogin1ManagerGen *object,
    GDBusMethodInvocation *invocation,
    gboolean arg_interactive);

  gboolean (*handle_suspend) (
    GpLogin1ManagerGen *object,
    GDBusMethodInvocation *invocation,
    gboolean arg_interactive);

};

GType gp_login1_manager_gen_get_type (void) G_GNUC_CONST;

GDBusInterfaceInfo *gp_login1_manager_gen_interface_info (void);
guint gp_login1_manager_gen_override_properties (GObjectClass *klass, guint property_id_begin);


/* D-Bus method call completion functions: */
void gp_login1_manager_gen_complete_suspend (
    GpLogin1ManagerGen *object,
    GDBusMethodInvocation *invocation);

void gp_login1_manager_gen_complete_hibernate (
    GpLogin1ManagerGen *object,
    GDBusMethodInvocation *invocation);

void gp_login1_manager_gen_complete_hybrid_sleep (
    GpLogin1ManagerGen *object,
    GDBusMethodInvocation *invocation);

void gp_login1_manager_gen_complete_can_suspend (
    GpLogin1ManagerGen *object,
    GDBusMethodInvocation *invocation,
    const gchar *result);

void gp_login1_manager_gen_complete_can_hibernate (
    GpLogin1ManagerGen *object,
    GDBusMethodInvocation *invocation,
    const gchar *result);

void gp_login1_manager_gen_complete_can_hybrid_sleep (
    GpLogin1ManagerGen *object,
    GDBusMethodInvocation *invocation,
    const gchar *result);



/* D-Bus method calls: */
void gp_login1_manager_gen_call_suspend (
    GpLogin1ManagerGen *proxy,
    gboolean arg_interactive,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean gp_login1_manager_gen_call_suspend_finish (
    GpLogin1ManagerGen *proxy,
    GAsyncResult *res,
    GError **error);

gboolean gp_login1_manager_gen_call_suspend_sync (
    GpLogin1ManagerGen *proxy,
    gboolean arg_interactive,
    GCancellable *cancellable,
    GError **error);

void gp_login1_manager_gen_call_hibernate (
    GpLogin1ManagerGen *proxy,
    gboolean arg_interactive,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean gp_login1_manager_gen_call_hibernate_finish (
    GpLogin1ManagerGen *proxy,
    GAsyncResult *res,
    GError **error);

gboolean gp_login1_manager_gen_call_hibernate_sync (
    GpLogin1ManagerGen *proxy,
    gboolean arg_interactive,
    GCancellable *cancellable,
    GError **error);

void gp_login1_manager_gen_call_hybrid_sleep (
    GpLogin1ManagerGen *proxy,
    gboolean arg_interactive,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean gp_login1_manager_gen_call_hybrid_sleep_finish (
    GpLogin1ManagerGen *proxy,
    GAsyncResult *res,
    GError **error);

gboolean gp_login1_manager_gen_call_hybrid_sleep_sync (
    GpLogin1ManagerGen *proxy,
    gboolean arg_interactive,
    GCancellable *cancellable,
    GError **error);

void gp_login1_manager_gen_call_can_suspend (
    GpLogin1ManagerGen *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean gp_login1_manager_gen_call_can_suspend_finish (
    GpLogin1ManagerGen *proxy,
    gchar **out_result,
    GAsyncResult *res,
    GError **error);

gboolean gp_login1_manager_gen_call_can_suspend_sync (
    GpLogin1ManagerGen *proxy,
    gchar **out_result,
    GCancellable *cancellable,
    GError **error);

void gp_login1_manager_gen_call_can_hibernate (
    GpLogin1ManagerGen *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean gp_login1_manager_gen_call_can_hibernate_finish (
    GpLogin1ManagerGen *proxy,
    gchar **out_result,
    GAsyncResult *res,
    GError **error);

gboolean gp_login1_manager_gen_call_can_hibernate_sync (
    GpLogin1ManagerGen *proxy,
    gchar **out_result,
    GCancellable *cancellable,
    GError **error);

void gp_login1_manager_gen_call_can_hybrid_sleep (
    GpLogin1ManagerGen *proxy,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data);

gboolean gp_login1_manager_gen_call_can_hybrid_sleep_finish (
    GpLogin1ManagerGen *proxy,
    gchar **out_result,
    GAsyncResult *res,
    GError **error);

gboolean gp_login1_manager_gen_call_can_hybrid_sleep_sync (
    GpLogin1ManagerGen *proxy,
    gchar **out_result,
    GCancellable *cancellable,
    GError **error);



/* ---- */

#define GP_TYPE_LOGIN1_MANAGER_GEN_PROXY (gp_login1_manager_gen_proxy_get_type ())
#define GP_LOGIN1_MANAGER_GEN_PROXY(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), GP_TYPE_LOGIN1_MANAGER_GEN_PROXY, GpLogin1ManagerGenProxy))
#define GP_LOGIN1_MANAGER_GEN_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), GP_TYPE_LOGIN1_MANAGER_GEN_PROXY, GpLogin1ManagerGenProxyClass))
#define GP_LOGIN1_MANAGER_GEN_PROXY_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), GP_TYPE_LOGIN1_MANAGER_GEN_PROXY, GpLogin1ManagerGenProxyClass))
#define GP_IS_LOGIN1_MANAGER_GEN_PROXY(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), GP_TYPE_LOGIN1_MANAGER_GEN_PROXY))
#define GP_IS_LOGIN1_MANAGER_GEN_PROXY_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), GP_TYPE_LOGIN1_MANAGER_GEN_PROXY))

typedef struct _GpLogin1ManagerGenProxy GpLogin1ManagerGenProxy;
typedef struct _GpLogin1ManagerGenProxyClass GpLogin1ManagerGenProxyClass;
typedef struct _GpLogin1ManagerGenProxyPrivate GpLogin1ManagerGenProxyPrivate;

struct _GpLogin1ManagerGenProxy
{
  /*< private >*/
  GDBusProxy parent_instance;
  GpLogin1ManagerGenProxyPrivate *priv;
};

struct _GpLogin1ManagerGenProxyClass
{
  GDBusProxyClass parent_class;
};

GType gp_login1_manager_gen_proxy_get_type (void) G_GNUC_CONST;

#if GLIB_CHECK_VERSION(2, 44, 0)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GpLogin1ManagerGenProxy, g_object_unref)
#endif

void gp_login1_manager_gen_proxy_new (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
GpLogin1ManagerGen *gp_login1_manager_gen_proxy_new_finish (
    GAsyncResult        *res,
    GError             **error);
GpLogin1ManagerGen *gp_login1_manager_gen_proxy_new_sync (
    GDBusConnection     *connection,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);

void gp_login1_manager_gen_proxy_new_for_bus (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GAsyncReadyCallback  callback,
    gpointer             user_data);
GpLogin1ManagerGen *gp_login1_manager_gen_proxy_new_for_bus_finish (
    GAsyncResult        *res,
    GError             **error);
GpLogin1ManagerGen *gp_login1_manager_gen_proxy_new_for_bus_sync (
    GBusType             bus_type,
    GDBusProxyFlags      flags,
    const gchar         *name,
    const gchar         *object_path,
    GCancellable        *cancellable,
    GError             **error);


/* ---- */

#define GP_TYPE_LOGIN1_MANAGER_GEN_SKELETON (gp_login1_manager_gen_skeleton_get_type ())
#define GP_LOGIN1_MANAGER_GEN_SKELETON(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), GP_TYPE_LOGIN1_MANAGER_GEN_SKELETON, GpLogin1ManagerGenSkeleton))
#define GP_LOGIN1_MANAGER_GEN_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), GP_TYPE_LOGIN1_MANAGER_GEN_SKELETON, GpLogin1ManagerGenSkeletonClass))
#define GP_LOGIN1_MANAGER_GEN_SKELETON_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), GP_TYPE_LOGIN1_MANAGER_GEN_SKELETON, GpLogin1ManagerGenSkeletonClass))
#define GP_IS_LOGIN1_MANAGER_GEN_SKELETON(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), GP_TYPE_LOGIN1_MANAGER_GEN_SKELETON))
#define GP_IS_LOGIN1_MANAGER_GEN_SKELETON_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), GP_TYPE_LOGIN1_MANAGER_GEN_SKELETON))

typedef struct _GpLogin1ManagerGenSkeleton GpLogin1ManagerGenSkeleton;
typedef struct _GpLogin1ManagerGenSkeletonClass GpLogin1ManagerGenSkeletonClass;
typedef struct _GpLogin1ManagerGenSkeletonPrivate GpLogin1ManagerGenSkeletonPrivate;

struct _GpLogin1ManagerGenSkeleton
{
  /*< private >*/
  GDBusInterfaceSkeleton parent_instance;
  GpLogin1ManagerGenSkeletonPrivate *priv;
};

struct _GpLogin1ManagerGenSkeletonClass
{
  GDBusInterfaceSkeletonClass parent_class;
};

GType gp_login1_manager_gen_skeleton_get_type (void) G_GNUC_CONST;

#if GLIB_CHECK_VERSION(2, 44, 0)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GpLogin1ManagerGenSkeleton, g_object_unref)
#endif

GpLogin1ManagerGen *gp_login1_manager_gen_skeleton_new (void);


G_END_DECLS

#endif /* __GP_LOGIN1_MANAGER_GEN_H__ */