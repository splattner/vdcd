/* Generated by the protocol buffer compiler.  DO NOT EDIT! */

#ifndef PROTOBUF_C_vdcapi_2eproto__INCLUDED
#define PROTOBUF_C_vdcapi_2eproto__INCLUDED

#include <google/protobuf-c/protobuf-c.h>

PROTOBUF_C_BEGIN_DECLS


typedef struct _Vdcapi__VdsmRequestHello Vdcapi__VdsmRequestHello;
typedef struct _Vdcapi__VdcResponseHello Vdcapi__VdcResponseHello;
typedef struct _Vdcapi__VdcSendAnnounce Vdcapi__VdcSendAnnounce;
typedef struct _Vdcapi__VdcSendVanish Vdcapi__VdcSendVanish;
typedef struct _Vdcapi__VdsmSendBye Vdcapi__VdsmSendBye;
typedef struct _Vdcapi__VdsmSendRemove Vdcapi__VdsmSendRemove;
typedef struct _Vdcapi__VdsmRequestGetProperty Vdcapi__VdsmRequestGetProperty;
typedef struct _Vdcapi__VdcResponseGetProperty Vdcapi__VdcResponseGetProperty;
typedef struct _Vdcapi__VdsmSendSetProperty Vdcapi__VdsmSendSetProperty;
typedef struct _Vdcapi__VdsmSendPing Vdcapi__VdsmSendPing;
typedef struct _Vdcapi__VdcSendPong Vdcapi__VdcSendPong;
typedef struct _Vdcapi__VdsmNotificationCallScene Vdcapi__VdsmNotificationCallScene;
typedef struct _Vdcapi__VdsmNotificationSaveScene Vdcapi__VdsmNotificationSaveScene;
typedef struct _Vdcapi__VdsmNotificationUndoScene Vdcapi__VdsmNotificationUndoScene;
typedef struct _Vdcapi__VdsmNotificationSetLocalPrio Vdcapi__VdsmNotificationSetLocalPrio;
typedef struct _Vdcapi__VdsmNotificationCallMinScene Vdcapi__VdsmNotificationCallMinScene;
typedef struct _Vdcapi__VdsmNotificationIdentify Vdcapi__VdsmNotificationIdentify;
typedef struct _Vdcapi__VdsmNotificationPingDevice Vdcapi__VdsmNotificationPingDevice;
typedef struct _Vdcapi__VdcSendPushProperty Vdcapi__VdcSendPushProperty;


/* --- enums --- */


/* --- messages --- */

struct  _Vdcapi__VdsmRequestHello
{
  ProtobufCMessage base;
  char *dsuid;
  protobuf_c_boolean has_api_version;
  uint32_t api_version;
  char *push_uri;
};
#define VDCAPI__VDSM__REQUEST_HELLO__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&vdcapi__vdsm__request_hello__descriptor) \
    , NULL, 0,0, NULL }


struct  _Vdcapi__VdcResponseHello
{
  ProtobufCMessage base;
  char *dsuid;
  protobuf_c_boolean has_allow_disconnect;
  protobuf_c_boolean allow_disconnect;
};
#define VDCAPI__VDC__RESPONSE_HELLO__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&vdcapi__vdc__response_hello__descriptor) \
    , NULL, 0,0 }


struct  _Vdcapi__VdcSendAnnounce
{
  ProtobufCMessage base;
  char *dsuid;
};
#define VDCAPI__VDC__SEND_ANNOUNCE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&vdcapi__vdc__send_announce__descriptor) \
    , NULL }


struct  _Vdcapi__VdcSendVanish
{
  ProtobufCMessage base;
  char *dsuid;
};
#define VDCAPI__VDC__SEND_VANISH__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&vdcapi__vdc__send_vanish__descriptor) \
    , NULL }


struct  _Vdcapi__VdsmSendBye
{
  ProtobufCMessage base;
  char *dsuid;
};
#define VDCAPI__VDSM__SEND_BYE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&vdcapi__vdsm__send_bye__descriptor) \
    , NULL }


struct  _Vdcapi__VdsmSendRemove
{
  ProtobufCMessage base;
};
#define VDCAPI__VDSM__SEND_REMOVE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&vdcapi__vdsm__send_remove__descriptor) \
     }


struct  _Vdcapi__VdsmRequestGetProperty
{
  ProtobufCMessage base;
  char *dsuid;
  char *name;
  protobuf_c_boolean has_value;
  int32_t value;
};
#define VDCAPI__VDSM__REQUEST_GET_PROPERTY__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&vdcapi__vdsm__request_get_property__descriptor) \
    , NULL, NULL, 0,0 }


struct  _Vdcapi__VdcResponseGetProperty
{
  ProtobufCMessage base;
};
#define VDCAPI__VDC__RESPONSE_GET_PROPERTY__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&vdcapi__vdc__response_get_property__descriptor) \
     }


struct  _Vdcapi__VdsmSendSetProperty
{
  ProtobufCMessage base;
  char *dsuid;
  char *name;
  protobuf_c_boolean has_index;
  uint32_t index;
  protobuf_c_boolean has_offset;
  uint32_t offset;
  protobuf_c_boolean has_count;
  uint32_t count;
};
#define VDCAPI__VDSM__SEND_SET_PROPERTY__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&vdcapi__vdsm__send_set_property__descriptor) \
    , NULL, NULL, 0,0, 0,0, 0,1 }


struct  _Vdcapi__VdsmSendPing
{
  ProtobufCMessage base;
  char *dsuid;
};
#define VDCAPI__VDSM__SEND_PING__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&vdcapi__vdsm__send_ping__descriptor) \
    , NULL }


struct  _Vdcapi__VdcSendPong
{
  ProtobufCMessage base;
  char *dsuid;
};
#define VDCAPI__VDC__SEND_PONG__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&vdcapi__vdc__send_pong__descriptor) \
    , NULL }


struct  _Vdcapi__VdsmNotificationCallScene
{
  ProtobufCMessage base;
  char *dsuid;
  protobuf_c_boolean has_scene;
  int32_t scene;
  protobuf_c_boolean has_force;
  protobuf_c_boolean force;
  protobuf_c_boolean has_group;
  int32_t group;
  protobuf_c_boolean has_zoneid;
  int32_t zoneid;
};
#define VDCAPI__VDSM__NOTIFICATION_CALL_SCENE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&vdcapi__vdsm__notification_call_scene__descriptor) \
    , NULL, 0,0, 0,0, 0,0, 0,0 }


struct  _Vdcapi__VdsmNotificationSaveScene
{
  ProtobufCMessage base;
  char *dsuid;
  protobuf_c_boolean has_scene;
  int32_t scene;
  protobuf_c_boolean has_group;
  int32_t group;
  protobuf_c_boolean has_zoneid;
  int32_t zoneid;
};
#define VDCAPI__VDSM__NOTIFICATION_SAVE_SCENE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&vdcapi__vdsm__notification_save_scene__descriptor) \
    , NULL, 0,0, 0,0, 0,0 }


struct  _Vdcapi__VdsmNotificationUndoScene
{
  ProtobufCMessage base;
  char *dsuid;
  protobuf_c_boolean has_scene;
  int32_t scene;
  protobuf_c_boolean has_group;
  int32_t group;
  protobuf_c_boolean has_zoneid;
  int32_t zoneid;
};
#define VDCAPI__VDSM__NOTIFICATION_UNDO_SCENE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&vdcapi__vdsm__notification_undo_scene__descriptor) \
    , NULL, 0,0, 0,0, 0,0 }


struct  _Vdcapi__VdsmNotificationSetLocalPrio
{
  ProtobufCMessage base;
  char *dsuid;
  protobuf_c_boolean has_scene;
  int32_t scene;
  protobuf_c_boolean has_group;
  int32_t group;
  protobuf_c_boolean has_zoneid;
  int32_t zoneid;
};
#define VDCAPI__VDSM__NOTIFICATION_SET_LOCAL_PRIO__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&vdcapi__vdsm__notification_set_local_prio__descriptor) \
    , NULL, 0,0, 0,0, 0,0 }


struct  _Vdcapi__VdsmNotificationCallMinScene
{
  ProtobufCMessage base;
  char *dsuid;
  protobuf_c_boolean has_group;
  int32_t group;
  protobuf_c_boolean has_zoneid;
  int32_t zoneid;
};
#define VDCAPI__VDSM__NOTIFICATION_CALL_MIN_SCENE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&vdcapi__vdsm__notification_call_min_scene__descriptor) \
    , NULL, 0,0, 0,0 }


struct  _Vdcapi__VdsmNotificationIdentify
{
  ProtobufCMessage base;
  char *dsuid;
  protobuf_c_boolean has_group;
  int32_t group;
  protobuf_c_boolean has_zoneid;
  int32_t zoneid;
};
#define VDCAPI__VDSM__NOTIFICATION_IDENTIFY__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&vdcapi__vdsm__notification_identify__descriptor) \
    , NULL, 0,0, 0,0 }


struct  _Vdcapi__VdsmNotificationPingDevice
{
  ProtobufCMessage base;
  char *dsuid;
};
#define VDCAPI__VDSM__NOTIFICATION_PING_DEVICE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&vdcapi__vdsm__notification_ping_device__descriptor) \
    , NULL }


struct  _Vdcapi__VdcSendPushProperty
{
  ProtobufCMessage base;
  char *dsuid;
  char *name;
  protobuf_c_boolean has_index;
  uint32_t index;
};
#define VDCAPI__VDC__SEND_PUSH_PROPERTY__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&vdcapi__vdc__send_push_property__descriptor) \
    , NULL, NULL, 0,0 }


/* Vdcapi__VdsmRequestHello methods */
void   vdcapi__vdsm__request_hello__init
                     (Vdcapi__VdsmRequestHello         *message);
size_t vdcapi__vdsm__request_hello__get_packed_size
                     (const Vdcapi__VdsmRequestHello   *message);
size_t vdcapi__vdsm__request_hello__pack
                     (const Vdcapi__VdsmRequestHello   *message,
                      uint8_t             *out);
size_t vdcapi__vdsm__request_hello__pack_to_buffer
                     (const Vdcapi__VdsmRequestHello   *message,
                      ProtobufCBuffer     *buffer);
Vdcapi__VdsmRequestHello *
       vdcapi__vdsm__request_hello__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   vdcapi__vdsm__request_hello__free_unpacked
                     (Vdcapi__VdsmRequestHello *message,
                      ProtobufCAllocator *allocator);
/* Vdcapi__VdcResponseHello methods */
void   vdcapi__vdc__response_hello__init
                     (Vdcapi__VdcResponseHello         *message);
size_t vdcapi__vdc__response_hello__get_packed_size
                     (const Vdcapi__VdcResponseHello   *message);
size_t vdcapi__vdc__response_hello__pack
                     (const Vdcapi__VdcResponseHello   *message,
                      uint8_t             *out);
size_t vdcapi__vdc__response_hello__pack_to_buffer
                     (const Vdcapi__VdcResponseHello   *message,
                      ProtobufCBuffer     *buffer);
Vdcapi__VdcResponseHello *
       vdcapi__vdc__response_hello__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   vdcapi__vdc__response_hello__free_unpacked
                     (Vdcapi__VdcResponseHello *message,
                      ProtobufCAllocator *allocator);
/* Vdcapi__VdcSendAnnounce methods */
void   vdcapi__vdc__send_announce__init
                     (Vdcapi__VdcSendAnnounce         *message);
size_t vdcapi__vdc__send_announce__get_packed_size
                     (const Vdcapi__VdcSendAnnounce   *message);
size_t vdcapi__vdc__send_announce__pack
                     (const Vdcapi__VdcSendAnnounce   *message,
                      uint8_t             *out);
size_t vdcapi__vdc__send_announce__pack_to_buffer
                     (const Vdcapi__VdcSendAnnounce   *message,
                      ProtobufCBuffer     *buffer);
Vdcapi__VdcSendAnnounce *
       vdcapi__vdc__send_announce__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   vdcapi__vdc__send_announce__free_unpacked
                     (Vdcapi__VdcSendAnnounce *message,
                      ProtobufCAllocator *allocator);
/* Vdcapi__VdcSendVanish methods */
void   vdcapi__vdc__send_vanish__init
                     (Vdcapi__VdcSendVanish         *message);
size_t vdcapi__vdc__send_vanish__get_packed_size
                     (const Vdcapi__VdcSendVanish   *message);
size_t vdcapi__vdc__send_vanish__pack
                     (const Vdcapi__VdcSendVanish   *message,
                      uint8_t             *out);
size_t vdcapi__vdc__send_vanish__pack_to_buffer
                     (const Vdcapi__VdcSendVanish   *message,
                      ProtobufCBuffer     *buffer);
Vdcapi__VdcSendVanish *
       vdcapi__vdc__send_vanish__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   vdcapi__vdc__send_vanish__free_unpacked
                     (Vdcapi__VdcSendVanish *message,
                      ProtobufCAllocator *allocator);
/* Vdcapi__VdsmSendBye methods */
void   vdcapi__vdsm__send_bye__init
                     (Vdcapi__VdsmSendBye         *message);
size_t vdcapi__vdsm__send_bye__get_packed_size
                     (const Vdcapi__VdsmSendBye   *message);
size_t vdcapi__vdsm__send_bye__pack
                     (const Vdcapi__VdsmSendBye   *message,
                      uint8_t             *out);
size_t vdcapi__vdsm__send_bye__pack_to_buffer
                     (const Vdcapi__VdsmSendBye   *message,
                      ProtobufCBuffer     *buffer);
Vdcapi__VdsmSendBye *
       vdcapi__vdsm__send_bye__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   vdcapi__vdsm__send_bye__free_unpacked
                     (Vdcapi__VdsmSendBye *message,
                      ProtobufCAllocator *allocator);
/* Vdcapi__VdsmSendRemove methods */
void   vdcapi__vdsm__send_remove__init
                     (Vdcapi__VdsmSendRemove         *message);
size_t vdcapi__vdsm__send_remove__get_packed_size
                     (const Vdcapi__VdsmSendRemove   *message);
size_t vdcapi__vdsm__send_remove__pack
                     (const Vdcapi__VdsmSendRemove   *message,
                      uint8_t             *out);
size_t vdcapi__vdsm__send_remove__pack_to_buffer
                     (const Vdcapi__VdsmSendRemove   *message,
                      ProtobufCBuffer     *buffer);
Vdcapi__VdsmSendRemove *
       vdcapi__vdsm__send_remove__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   vdcapi__vdsm__send_remove__free_unpacked
                     (Vdcapi__VdsmSendRemove *message,
                      ProtobufCAllocator *allocator);
/* Vdcapi__VdsmRequestGetProperty methods */
void   vdcapi__vdsm__request_get_property__init
                     (Vdcapi__VdsmRequestGetProperty         *message);
size_t vdcapi__vdsm__request_get_property__get_packed_size
                     (const Vdcapi__VdsmRequestGetProperty   *message);
size_t vdcapi__vdsm__request_get_property__pack
                     (const Vdcapi__VdsmRequestGetProperty   *message,
                      uint8_t             *out);
size_t vdcapi__vdsm__request_get_property__pack_to_buffer
                     (const Vdcapi__VdsmRequestGetProperty   *message,
                      ProtobufCBuffer     *buffer);
Vdcapi__VdsmRequestGetProperty *
       vdcapi__vdsm__request_get_property__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   vdcapi__vdsm__request_get_property__free_unpacked
                     (Vdcapi__VdsmRequestGetProperty *message,
                      ProtobufCAllocator *allocator);
/* Vdcapi__VdcResponseGetProperty methods */
void   vdcapi__vdc__response_get_property__init
                     (Vdcapi__VdcResponseGetProperty         *message);
size_t vdcapi__vdc__response_get_property__get_packed_size
                     (const Vdcapi__VdcResponseGetProperty   *message);
size_t vdcapi__vdc__response_get_property__pack
                     (const Vdcapi__VdcResponseGetProperty   *message,
                      uint8_t             *out);
size_t vdcapi__vdc__response_get_property__pack_to_buffer
                     (const Vdcapi__VdcResponseGetProperty   *message,
                      ProtobufCBuffer     *buffer);
Vdcapi__VdcResponseGetProperty *
       vdcapi__vdc__response_get_property__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   vdcapi__vdc__response_get_property__free_unpacked
                     (Vdcapi__VdcResponseGetProperty *message,
                      ProtobufCAllocator *allocator);
/* Vdcapi__VdsmSendSetProperty methods */
void   vdcapi__vdsm__send_set_property__init
                     (Vdcapi__VdsmSendSetProperty         *message);
size_t vdcapi__vdsm__send_set_property__get_packed_size
                     (const Vdcapi__VdsmSendSetProperty   *message);
size_t vdcapi__vdsm__send_set_property__pack
                     (const Vdcapi__VdsmSendSetProperty   *message,
                      uint8_t             *out);
size_t vdcapi__vdsm__send_set_property__pack_to_buffer
                     (const Vdcapi__VdsmSendSetProperty   *message,
                      ProtobufCBuffer     *buffer);
Vdcapi__VdsmSendSetProperty *
       vdcapi__vdsm__send_set_property__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   vdcapi__vdsm__send_set_property__free_unpacked
                     (Vdcapi__VdsmSendSetProperty *message,
                      ProtobufCAllocator *allocator);
/* Vdcapi__VdsmSendPing methods */
void   vdcapi__vdsm__send_ping__init
                     (Vdcapi__VdsmSendPing         *message);
size_t vdcapi__vdsm__send_ping__get_packed_size
                     (const Vdcapi__VdsmSendPing   *message);
size_t vdcapi__vdsm__send_ping__pack
                     (const Vdcapi__VdsmSendPing   *message,
                      uint8_t             *out);
size_t vdcapi__vdsm__send_ping__pack_to_buffer
                     (const Vdcapi__VdsmSendPing   *message,
                      ProtobufCBuffer     *buffer);
Vdcapi__VdsmSendPing *
       vdcapi__vdsm__send_ping__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   vdcapi__vdsm__send_ping__free_unpacked
                     (Vdcapi__VdsmSendPing *message,
                      ProtobufCAllocator *allocator);
/* Vdcapi__VdcSendPong methods */
void   vdcapi__vdc__send_pong__init
                     (Vdcapi__VdcSendPong         *message);
size_t vdcapi__vdc__send_pong__get_packed_size
                     (const Vdcapi__VdcSendPong   *message);
size_t vdcapi__vdc__send_pong__pack
                     (const Vdcapi__VdcSendPong   *message,
                      uint8_t             *out);
size_t vdcapi__vdc__send_pong__pack_to_buffer
                     (const Vdcapi__VdcSendPong   *message,
                      ProtobufCBuffer     *buffer);
Vdcapi__VdcSendPong *
       vdcapi__vdc__send_pong__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   vdcapi__vdc__send_pong__free_unpacked
                     (Vdcapi__VdcSendPong *message,
                      ProtobufCAllocator *allocator);
/* Vdcapi__VdsmNotificationCallScene methods */
void   vdcapi__vdsm__notification_call_scene__init
                     (Vdcapi__VdsmNotificationCallScene         *message);
size_t vdcapi__vdsm__notification_call_scene__get_packed_size
                     (const Vdcapi__VdsmNotificationCallScene   *message);
size_t vdcapi__vdsm__notification_call_scene__pack
                     (const Vdcapi__VdsmNotificationCallScene   *message,
                      uint8_t             *out);
size_t vdcapi__vdsm__notification_call_scene__pack_to_buffer
                     (const Vdcapi__VdsmNotificationCallScene   *message,
                      ProtobufCBuffer     *buffer);
Vdcapi__VdsmNotificationCallScene *
       vdcapi__vdsm__notification_call_scene__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   vdcapi__vdsm__notification_call_scene__free_unpacked
                     (Vdcapi__VdsmNotificationCallScene *message,
                      ProtobufCAllocator *allocator);
/* Vdcapi__VdsmNotificationSaveScene methods */
void   vdcapi__vdsm__notification_save_scene__init
                     (Vdcapi__VdsmNotificationSaveScene         *message);
size_t vdcapi__vdsm__notification_save_scene__get_packed_size
                     (const Vdcapi__VdsmNotificationSaveScene   *message);
size_t vdcapi__vdsm__notification_save_scene__pack
                     (const Vdcapi__VdsmNotificationSaveScene   *message,
                      uint8_t             *out);
size_t vdcapi__vdsm__notification_save_scene__pack_to_buffer
                     (const Vdcapi__VdsmNotificationSaveScene   *message,
                      ProtobufCBuffer     *buffer);
Vdcapi__VdsmNotificationSaveScene *
       vdcapi__vdsm__notification_save_scene__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   vdcapi__vdsm__notification_save_scene__free_unpacked
                     (Vdcapi__VdsmNotificationSaveScene *message,
                      ProtobufCAllocator *allocator);
/* Vdcapi__VdsmNotificationUndoScene methods */
void   vdcapi__vdsm__notification_undo_scene__init
                     (Vdcapi__VdsmNotificationUndoScene         *message);
size_t vdcapi__vdsm__notification_undo_scene__get_packed_size
                     (const Vdcapi__VdsmNotificationUndoScene   *message);
size_t vdcapi__vdsm__notification_undo_scene__pack
                     (const Vdcapi__VdsmNotificationUndoScene   *message,
                      uint8_t             *out);
size_t vdcapi__vdsm__notification_undo_scene__pack_to_buffer
                     (const Vdcapi__VdsmNotificationUndoScene   *message,
                      ProtobufCBuffer     *buffer);
Vdcapi__VdsmNotificationUndoScene *
       vdcapi__vdsm__notification_undo_scene__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   vdcapi__vdsm__notification_undo_scene__free_unpacked
                     (Vdcapi__VdsmNotificationUndoScene *message,
                      ProtobufCAllocator *allocator);
/* Vdcapi__VdsmNotificationSetLocalPrio methods */
void   vdcapi__vdsm__notification_set_local_prio__init
                     (Vdcapi__VdsmNotificationSetLocalPrio         *message);
size_t vdcapi__vdsm__notification_set_local_prio__get_packed_size
                     (const Vdcapi__VdsmNotificationSetLocalPrio   *message);
size_t vdcapi__vdsm__notification_set_local_prio__pack
                     (const Vdcapi__VdsmNotificationSetLocalPrio   *message,
                      uint8_t             *out);
size_t vdcapi__vdsm__notification_set_local_prio__pack_to_buffer
                     (const Vdcapi__VdsmNotificationSetLocalPrio   *message,
                      ProtobufCBuffer     *buffer);
Vdcapi__VdsmNotificationSetLocalPrio *
       vdcapi__vdsm__notification_set_local_prio__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   vdcapi__vdsm__notification_set_local_prio__free_unpacked
                     (Vdcapi__VdsmNotificationSetLocalPrio *message,
                      ProtobufCAllocator *allocator);
/* Vdcapi__VdsmNotificationCallMinScene methods */
void   vdcapi__vdsm__notification_call_min_scene__init
                     (Vdcapi__VdsmNotificationCallMinScene         *message);
size_t vdcapi__vdsm__notification_call_min_scene__get_packed_size
                     (const Vdcapi__VdsmNotificationCallMinScene   *message);
size_t vdcapi__vdsm__notification_call_min_scene__pack
                     (const Vdcapi__VdsmNotificationCallMinScene   *message,
                      uint8_t             *out);
size_t vdcapi__vdsm__notification_call_min_scene__pack_to_buffer
                     (const Vdcapi__VdsmNotificationCallMinScene   *message,
                      ProtobufCBuffer     *buffer);
Vdcapi__VdsmNotificationCallMinScene *
       vdcapi__vdsm__notification_call_min_scene__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   vdcapi__vdsm__notification_call_min_scene__free_unpacked
                     (Vdcapi__VdsmNotificationCallMinScene *message,
                      ProtobufCAllocator *allocator);
/* Vdcapi__VdsmNotificationIdentify methods */
void   vdcapi__vdsm__notification_identify__init
                     (Vdcapi__VdsmNotificationIdentify         *message);
size_t vdcapi__vdsm__notification_identify__get_packed_size
                     (const Vdcapi__VdsmNotificationIdentify   *message);
size_t vdcapi__vdsm__notification_identify__pack
                     (const Vdcapi__VdsmNotificationIdentify   *message,
                      uint8_t             *out);
size_t vdcapi__vdsm__notification_identify__pack_to_buffer
                     (const Vdcapi__VdsmNotificationIdentify   *message,
                      ProtobufCBuffer     *buffer);
Vdcapi__VdsmNotificationIdentify *
       vdcapi__vdsm__notification_identify__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   vdcapi__vdsm__notification_identify__free_unpacked
                     (Vdcapi__VdsmNotificationIdentify *message,
                      ProtobufCAllocator *allocator);
/* Vdcapi__VdsmNotificationPingDevice methods */
void   vdcapi__vdsm__notification_ping_device__init
                     (Vdcapi__VdsmNotificationPingDevice         *message);
size_t vdcapi__vdsm__notification_ping_device__get_packed_size
                     (const Vdcapi__VdsmNotificationPingDevice   *message);
size_t vdcapi__vdsm__notification_ping_device__pack
                     (const Vdcapi__VdsmNotificationPingDevice   *message,
                      uint8_t             *out);
size_t vdcapi__vdsm__notification_ping_device__pack_to_buffer
                     (const Vdcapi__VdsmNotificationPingDevice   *message,
                      ProtobufCBuffer     *buffer);
Vdcapi__VdsmNotificationPingDevice *
       vdcapi__vdsm__notification_ping_device__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   vdcapi__vdsm__notification_ping_device__free_unpacked
                     (Vdcapi__VdsmNotificationPingDevice *message,
                      ProtobufCAllocator *allocator);
/* Vdcapi__VdcSendPushProperty methods */
void   vdcapi__vdc__send_push_property__init
                     (Vdcapi__VdcSendPushProperty         *message);
size_t vdcapi__vdc__send_push_property__get_packed_size
                     (const Vdcapi__VdcSendPushProperty   *message);
size_t vdcapi__vdc__send_push_property__pack
                     (const Vdcapi__VdcSendPushProperty   *message,
                      uint8_t             *out);
size_t vdcapi__vdc__send_push_property__pack_to_buffer
                     (const Vdcapi__VdcSendPushProperty   *message,
                      ProtobufCBuffer     *buffer);
Vdcapi__VdcSendPushProperty *
       vdcapi__vdc__send_push_property__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   vdcapi__vdc__send_push_property__free_unpacked
                     (Vdcapi__VdcSendPushProperty *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Vdcapi__VdsmRequestHello_Closure)
                 (const Vdcapi__VdsmRequestHello *message,
                  void *closure_data);
typedef void (*Vdcapi__VdcResponseHello_Closure)
                 (const Vdcapi__VdcResponseHello *message,
                  void *closure_data);
typedef void (*Vdcapi__VdcSendAnnounce_Closure)
                 (const Vdcapi__VdcSendAnnounce *message,
                  void *closure_data);
typedef void (*Vdcapi__VdcSendVanish_Closure)
                 (const Vdcapi__VdcSendVanish *message,
                  void *closure_data);
typedef void (*Vdcapi__VdsmSendBye_Closure)
                 (const Vdcapi__VdsmSendBye *message,
                  void *closure_data);
typedef void (*Vdcapi__VdsmSendRemove_Closure)
                 (const Vdcapi__VdsmSendRemove *message,
                  void *closure_data);
typedef void (*Vdcapi__VdsmRequestGetProperty_Closure)
                 (const Vdcapi__VdsmRequestGetProperty *message,
                  void *closure_data);
typedef void (*Vdcapi__VdcResponseGetProperty_Closure)
                 (const Vdcapi__VdcResponseGetProperty *message,
                  void *closure_data);
typedef void (*Vdcapi__VdsmSendSetProperty_Closure)
                 (const Vdcapi__VdsmSendSetProperty *message,
                  void *closure_data);
typedef void (*Vdcapi__VdsmSendPing_Closure)
                 (const Vdcapi__VdsmSendPing *message,
                  void *closure_data);
typedef void (*Vdcapi__VdcSendPong_Closure)
                 (const Vdcapi__VdcSendPong *message,
                  void *closure_data);
typedef void (*Vdcapi__VdsmNotificationCallScene_Closure)
                 (const Vdcapi__VdsmNotificationCallScene *message,
                  void *closure_data);
typedef void (*Vdcapi__VdsmNotificationSaveScene_Closure)
                 (const Vdcapi__VdsmNotificationSaveScene *message,
                  void *closure_data);
typedef void (*Vdcapi__VdsmNotificationUndoScene_Closure)
                 (const Vdcapi__VdsmNotificationUndoScene *message,
                  void *closure_data);
typedef void (*Vdcapi__VdsmNotificationSetLocalPrio_Closure)
                 (const Vdcapi__VdsmNotificationSetLocalPrio *message,
                  void *closure_data);
typedef void (*Vdcapi__VdsmNotificationCallMinScene_Closure)
                 (const Vdcapi__VdsmNotificationCallMinScene *message,
                  void *closure_data);
typedef void (*Vdcapi__VdsmNotificationIdentify_Closure)
                 (const Vdcapi__VdsmNotificationIdentify *message,
                  void *closure_data);
typedef void (*Vdcapi__VdsmNotificationPingDevice_Closure)
                 (const Vdcapi__VdsmNotificationPingDevice *message,
                  void *closure_data);
typedef void (*Vdcapi__VdcSendPushProperty_Closure)
                 (const Vdcapi__VdcSendPushProperty *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor vdcapi__vdsm__request_hello__descriptor;
extern const ProtobufCMessageDescriptor vdcapi__vdc__response_hello__descriptor;
extern const ProtobufCMessageDescriptor vdcapi__vdc__send_announce__descriptor;
extern const ProtobufCMessageDescriptor vdcapi__vdc__send_vanish__descriptor;
extern const ProtobufCMessageDescriptor vdcapi__vdsm__send_bye__descriptor;
extern const ProtobufCMessageDescriptor vdcapi__vdsm__send_remove__descriptor;
extern const ProtobufCMessageDescriptor vdcapi__vdsm__request_get_property__descriptor;
extern const ProtobufCMessageDescriptor vdcapi__vdc__response_get_property__descriptor;
extern const ProtobufCMessageDescriptor vdcapi__vdsm__send_set_property__descriptor;
extern const ProtobufCMessageDescriptor vdcapi__vdsm__send_ping__descriptor;
extern const ProtobufCMessageDescriptor vdcapi__vdc__send_pong__descriptor;
extern const ProtobufCMessageDescriptor vdcapi__vdsm__notification_call_scene__descriptor;
extern const ProtobufCMessageDescriptor vdcapi__vdsm__notification_save_scene__descriptor;
extern const ProtobufCMessageDescriptor vdcapi__vdsm__notification_undo_scene__descriptor;
extern const ProtobufCMessageDescriptor vdcapi__vdsm__notification_set_local_prio__descriptor;
extern const ProtobufCMessageDescriptor vdcapi__vdsm__notification_call_min_scene__descriptor;
extern const ProtobufCMessageDescriptor vdcapi__vdsm__notification_identify__descriptor;
extern const ProtobufCMessageDescriptor vdcapi__vdsm__notification_ping_device__descriptor;
extern const ProtobufCMessageDescriptor vdcapi__vdc__send_push_property__descriptor;

PROTOBUF_C_END_DECLS


#endif  /* PROTOBUF_vdcapi_2eproto__INCLUDED */