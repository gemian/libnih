/* libnih
 *
 * Copyright © 2009 Scott James Remnant <scott@netsplit.com>.
 * Copyright © 2009 Canonical Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef NIH_DBUS_MESSAGE_H
#define NIH_DBUS_MESSAGE_H

/**
 * The NihDBusMessage structure references both an incoming D-Bus message
 * and the connection it was received on, and may be created with the
 * nih_dbus_message_new() function.
 *
 * This allows both a single pointer to be passed around to deal with a
 * message, and also provides an nih_alloc() context for attaching allocated
 * data that can be discarded when the message has been processed (often
 * strings used to generate the reply, for example).
 *
 * A typical function that uses this structure is nih_dbus_message_error()
 * which generates and sends a reply to the incoming message that is an
 * error return with the given name and format.
 **/

#include <nih/macros.h>

#include <dbus/dbus.h>


/**
 * NihDBusMessage:
 * @conn: D-Bus connection message was received on,
 * @message: message object received.
 *
 * This structure is used as a context for the processing of a message; the
 * primary reason for its existance is to be used as an nih_alloc() context
 * for any reply data.
 *
 * Instances are allocated automatically and passed to marshaller functions,
 * and freed on their return.
 **/
typedef struct nih_dbus_message {
	DBusConnection *conn;
	DBusMessage *   message;
} NihDBusMessage;


NIH_BEGIN_EXTERN

NihDBusMessage *nih_dbus_message_new   (const void *parent,
					DBusConnection *conn,
					DBusMessage *message)
	__attribute__ ((warn_unused_result));

int             nih_dbus_message_error (NihDBusMessage *msg,
					const char *name,
					const char *format, ...)
	__attribute__ ((warn_unused_result));

NIH_END_EXTERN

#endif /* NIH_DBUS_MESSAGE_H */
