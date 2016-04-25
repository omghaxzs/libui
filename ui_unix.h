// 7 april 2015

/*
This file assumes that you have included <gtk/gtk.h> and "ui.h" beforehand. It provides API-specific functions for interfacing with foreign controls on Unix systems that use GTK+ to provide their UI (currently all except Mac OS X).
*/

#ifndef __LIBUI_UI_UNIX_H__
#define __LIBUI_UI_UNIX_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct uiUnixControl uiUnixControl;
struct uiUnixControl {
	uiControl c;
	uiControl *parent;
	gboolean addedBefore;
	void (*SetContainer)(uiUnixControl *, GtkContainer *, gboolean);
};
#define uiUnixControl(this) ((uiUnixControl *) (this))
// TODO document
_UI_EXTERN void uiUnixControlSetContainer(uiUnixControl *, GtkContainer *, gboolean);

#define uiUnixControlDefaultDestroy(type, handlefield) \
	static void type ## Destroy(uiControl *c) \
	{ \
		uiControlVerifyDestroy(c); \
		/* TODO is this safe on floating refs? */ \
		g_object_unref(type(c)->handlefield); \
		uiFreeControl(c); \
	}
#define uiUnixControlDefaultHandle(type, handlefield) \
	static uintptr_t type ## Handle(uiControl *c) \
	{ \
		return (uintptr_t) (type(c)->handlefield); \
	}
#define uiUnixControlDefaultParent(type, handlefield) \
	static uiControl *type ## Parent(uiControl *c) \
	{ \
		return uiUnixControl(c)->parent; \
	}
#define uiUnixControlDefaultSetParent(type, handlefield) \
	static void type ## SetParent(uiControl *c, uiControl *parent) \
	{ \
		uiControlVerifySetParent(c, parent); \
		uiUnixControl(c)->parent = parent; \
	}
#define uiUnixControlDefaultToplevel(type, handlefield) \
	static int type ## Toplevel(uiControl *c) \
	{ \
		return 0; \
	}
#define uiUnixControlDefaultVisible(type, handlefield) \
	static int type ## Visible(uiControl *c) \
	{ \
		return gtk_widget_get_visible(type(c)->handlefield); \
	}
#define uiUnixControlDefaultShow(type, handlefield) \
	static void type ## Show(uiControl *c) \
	{ \
		gtk_widget_show(type(c)->handlefield); \
	}
#define uiUnixControlDefaultHide(type, handlefield) \
	static void type ## Hide(uiControl *c) \
	{ \
		gtk_widget_hide(type(c)->handlefield); \
	}
#define uiUnixControlDefaultEnabled(type, handlefield) \
	static int type ## Enabled(uiControl *c) \
	{ \
		return gtk_widget_get_sensitive(type(c)->handlefield); \
	}
#define uiUnixControlDefaultEnable(type, handlefield) \
	static void type ## Enable(uiControl *c) \
	{ \
		gtk_widget_set_sensitive(type(c)->handlefield, TRUE); \
	}
#define uiUnixControlDefaultDisable(type, handlefield) \
	static void type ## Disable(uiControl *c) \
	{ \
		gtk_widget_set_sensitive(type(c)->handlefield, FALSE); \
	}
#define uiUnixControlDefaultSetContainer(type, handlefield) \
	static void type ## SetContainer(uiUnixControl *c, GtkContainer *container, gboolean remove) \
	{ \
		if (!uiUnixControl(c)->addedBefore) { \
			g_object_ref_sink(type(c)->handlefield); /* our own reference, which we release in Destroy() */ \
			gtk_widget_show(type(c)->handlefield); \
			uiUnixControl(c)->addedBefore = TRUE; \
		} \
		if (remove) \
			gtk_container_remove(container, type(c)->handlefield); \
		else \
			gtk_container_add(container, type(c)->handlefield); \
	}

#define uiUnixControlAllDefaultsExceptDestroy(type, handlefield) \
	uiUnixControlDefaultHandle(type, handlefield) \
	uiUnixControlDefaultParent(type, handlefield) \
	uiUnixControlDefaultSetParent(type, handlefield) \
	uiUnixControlDefaultToplevel(type, handlefield) \
	uiUnixControlDefaultVisible(type, handlefield) \
	uiUnixControlDefaultShow(type, handlefield) \
	uiUnixControlDefaultHide(type, handlefield) \
	uiUnixControlDefaultEnabled(type, handlefield) \
	uiUnixControlDefaultEnable(type, handlefield) \
	uiUnixControlDefaultDisable(type, handlefield) \
	uiUnixControlDefaultSetContainer(type, handlefield)

#define uiUnixControlAllDefaults(type, handlefield) \
	uiUnixControlDefaultDestroy(type, handlefield) \
	uiUnixControlAllDefaultsExceptDestroy(type, handlefield)

// TODO document
#define uiUnixNewControl(type, var) \
	var = type(uiUnixAllocControl(sizeof (type), type ## Signature, #type)); \
	uiControl(var)->Destroy = type ## Destroy; \
	uiControl(var)->Handle = type ## Handle; \
	uiControl(var)->Parent = type ## Parent; \
	uiControl(var)->SetParent = type ## SetParent; \
	uiControl(var)->Toplevel = type ## Toplevel; \
	uiControl(var)->Visible = type ## Visible; \
	uiControl(var)->Show = type ## Show; \
	uiControl(var)->Hide = type ## Hide; \
	uiControl(var)->Enabled = type ## Enabled; \
	uiControl(var)->Enable = type ## Enable; \
	uiControl(var)->Disable = type ## Disable; \
	uiUnixControl(var)->SetContainer = type ## SetContainer;
// TODO document
_UI_EXTERN uiUnixControl *uiUnixAllocControl(size_t n, uint32_t typesig, const char *typenamestr);

// uiUnixStrdupText() takes the given string and produces a copy of it suitable for being freed by uiFreeText().
_UI_EXTERN char *uiUnixStrdupText(const char *);

#ifdef __cplusplus
}
#endif

#endif
