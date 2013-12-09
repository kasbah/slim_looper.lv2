/* Generated by the protocol buffer compiler.  DO NOT EDIT! */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C_NO_DEPRECATED
#define PROTOBUF_C_NO_DEPRECATED
#endif

#include "slim.pb-c.h"
void   looper_command_message__init
                     (LooperCommandMessage         *message)
{
  static LooperCommandMessage init_value = LOOPER_COMMAND_MESSAGE__INIT;
  *message = init_value;
}
size_t looper_command_message__get_packed_size
                     (const LooperCommandMessage *message)
{
  PROTOBUF_C_ASSERT (message->base.descriptor == &looper_command_message__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t looper_command_message__pack
                     (const LooperCommandMessage *message,
                      uint8_t       *out)
{
  PROTOBUF_C_ASSERT (message->base.descriptor == &looper_command_message__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t looper_command_message__pack_to_buffer
                     (const LooperCommandMessage *message,
                      ProtobufCBuffer *buffer)
{
  PROTOBUF_C_ASSERT (message->base.descriptor == &looper_command_message__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
LooperCommandMessage *
       looper_command_message__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (LooperCommandMessage *)
     protobuf_c_message_unpack (&looper_command_message__descriptor,
                                allocator, len, data);
}
void   looper_command_message__free_unpacked
                     (LooperCommandMessage *message,
                      ProtobufCAllocator *allocator)
{
  PROTOBUF_C_ASSERT (message->base.descriptor == &looper_command_message__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor looper_command_message__field_descriptors[1] =
{
  {
    "command",
    1,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_ENUM,
    0,   /* quantifier_offset */
    PROTOBUF_C_OFFSETOF(LooperCommandMessage, command),
    &looper_command__descriptor,
    NULL,
    0,            /* packed */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned looper_command_message__field_indices_by_name[] = {
  0,   /* field[0] = command */
};
static const ProtobufCIntRange looper_command_message__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 1 }
};
const ProtobufCMessageDescriptor looper_command_message__descriptor =
{
  PROTOBUF_C_MESSAGE_DESCRIPTOR_MAGIC,
  "LooperCommandMessage",
  "LooperCommandMessage",
  "LooperCommandMessage",
  "",
  sizeof(LooperCommandMessage),
  1,
  looper_command_message__field_descriptors,
  looper_command_message__field_indices_by_name,
  1,  looper_command_message__number_ranges,
  (ProtobufCMessageInit) looper_command_message__init,
  NULL,NULL,NULL    /* reserved[123] */
};
const ProtobufCEnumValue looper_command__enum_values_by_number[5] =
{
  { "PLAY_OR_PAUSE", "LOOPER_COMMAND__PLAY_OR_PAUSE", 0 },
  { "RECORD", "LOOPER_COMMAND__RECORD", 1 },
  { "OVERDUB", "LOOPER_COMMAND__OVERDUB", 2 },
  { "INSERT", "LOOPER_COMMAND__INSERT", 3 },
  { "REPLACE", "LOOPER_COMMAND__REPLACE", 4 },
};
static const ProtobufCIntRange looper_command__value_ranges[] = {
{0, 0},{0, 5}
};
const ProtobufCEnumValueIndex looper_command__enum_values_by_name[5] =
{
  { "INSERT", 3 },
  { "OVERDUB", 2 },
  { "PLAY_OR_PAUSE", 0 },
  { "RECORD", 1 },
  { "REPLACE", 4 },
};
const ProtobufCEnumDescriptor looper_command__descriptor =
{
  PROTOBUF_C_ENUM_DESCRIPTOR_MAGIC,
  "LooperCommand",
  "LooperCommand",
  "LooperCommand",
  "",
  5,
  looper_command__enum_values_by_number,
  5,
  looper_command__enum_values_by_name,
  1,
  looper_command__value_ranges,
  NULL,NULL,NULL,NULL   /* reserved[1234] */
};
