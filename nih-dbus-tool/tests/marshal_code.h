/* nih-dbus-tool
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

#ifndef NIH_DBUS_TOOL_TESTS_MARSHAL_CODE_H
#define NIH_DBUS_TOOL_TESTS_MARSHAL_CODE_H

#include <nih/macros.h>

#include <dbus/dbus.h>

#include <stdint.h>

struct dbus_struct_suasan {
	char *    item0;
	uint32_t  item1;
	char **   item2;
	int16_t * item3;
	size_t    item3_len;
};

struct dbus_struct_su {
	char *    item0;
	uint32_t  item1;
};


NIH_BEGIN_EXTERN

int my_byte_marshal               (DBusMessage *message, uint8_t value);
int my_boolean_marshal            (DBusMessage *message, int value);
int my_int16_marshal              (DBusMessage *message, int16_t value);
int my_uint16_marshal             (DBusMessage *message, uint16_t value);
int my_int32_marshal              (DBusMessage *message, int32_t value);
int my_uint32_marshal             (DBusMessage *message, uint32_t value);
int my_int64_marshal              (DBusMessage *message, int64_t value);
int my_uint64_marshal             (DBusMessage *message, uint64_t value);
int my_double_marshal             (DBusMessage *message, double value);
int my_string_marshal             (DBusMessage *message, const char *value);
int my_object_path_marshal        (DBusMessage *message, const char * value);
int my_signature_marshal          (DBusMessage *message, const char * value);
int my_int16_array_marshal        (DBusMessage *message,
				   const int16_t * value, size_t value_len);
int my_int16_array_array_marshal  (DBusMessage *message,
				   int16_t * const * value,
				   const size_t * value_len);
int my_string_array_marshal       (DBusMessage *message,
				   char * const * value);
int my_string_array_array_marshal (DBusMessage *message,
				   char ** const * value);
int my_struct_marshal             (DBusMessage *message,
				   const struct dbus_struct_suasan * value);
int my_struct_array_marshal       (DBusMessage *message,
				   struct dbus_struct_su * const * value);

NIH_END_EXTERN

#endif /* NIH_DBUS_TOOL_TESTS_MARSHAL_CODE_H */
