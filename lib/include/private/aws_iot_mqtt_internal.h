/*
 * Copyright (C) 2018 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * @file aws_iot_mqtt_internal.h
 * @brief Internal header of MQTT library. This header should not be included in
 * typical application code.
 */

#ifndef _AWS_IOT_MQTT_INTERNAL_H_
#define _AWS_IOT_MQTT_INTERNAL_H_

/* Build using a config header, if provided. */
#ifdef AWS_IOT_CONFIG_FILE
    #include AWS_IOT_CONFIG_FILE
#endif

/* MQTT include. */
#include "aws_iot_mqtt.h"

/* Queue include. */
#include "private/aws_iot_queue.h"

/* Platform clock include. */
#include "platform/aws_iot_clock.h"

/**
 * @def AwsIotMqtt_Assert( expression )
 * @brief Assertion macro for the MQTT library.
 *
 * Set @ref AWS_IOT_MQTT_ENABLE_ASSERTS to `1` to enable assertions in the MQTT
 * library.
 *
 * @param[in] expression Expression to be evaluated.
 */
#if AWS_IOT_MQTT_ENABLE_ASSERTS == 1
    #ifndef AwsIotMqtt_Assert
        #include <assert.h>
        #define AwsIotMqtt_Assert( expression )    assert( expression )
    #endif
#else
    #define AwsIotMqtt_Assert( expression )
#endif

/* Configure logs for MQTT functions. */
#ifdef AWS_IOT_LOG_LEVEL_MQTT
    #define _LIBRARY_LOG_LEVEL        AWS_IOT_LOG_LEVEL_MQTT
#else
    #ifdef AWS_IOT_LOG_LEVEL_GLOBAL
        #define _LIBRARY_LOG_LEVEL    AWS_IOT_LOG_LEVEL_GLOBAL
    #else
        #define _LIBRARY_LOG_LEVEL    AWS_IOT_LOG_NONE
    #endif
#endif

#define _LIBRARY_LOG_NAME    ( "MQTT" )
#include "private/aws_iot_logging_setup.h"

/*
 * Provide default values for undefined memory allocation functions based on
 * the usage of dynamic memory allocation.
 */
#if AWS_IOT_STATIC_MEMORY_ONLY == 1
    #include "platform/aws_iot_static_memory.h"

    /**
     * @brief Allocate an #_mqttConnection_t. This function should have the same
     * signature as [malloc]
     * (http://pubs.opengroup.org/onlinepubs/9699919799/functions/malloc.html).
     */
    #ifndef AwsIotMqtt_MallocConnection
        #define AwsIotMqtt_MallocConnection    AwsIot_MallocMqttConnection
    #endif

    /**
     * @brief Free an #_mqttConnection_t. This function should have the same
     * signature as [free]
     * (http://pubs.opengroup.org/onlinepubs/9699919799/functions/free.html).
     */
    #ifndef AwsIotMqtt_FreeConnection
        #define AwsIotMqtt_FreeConnection    AwsIot_FreeMqttConnection
    #endif

    /**
     * @brief Allocate memory for an MQTT packet. This function should have the
     * same signature as [malloc]
     * (http://pubs.opengroup.org/onlinepubs/9699919799/functions/malloc.html).
     */
    #ifndef AwsIotMqtt_MallocMessage
        #define AwsIotMqtt_MallocMessage    AwsIot_MallocMessageBuffer
    #endif

    /**
     * @brief Free an MQTT packet. This function should have the same signature
     * as [free]
     * (http://pubs.opengroup.org/onlinepubs/9699919799/functions/free.html).
     */
    #ifndef AwsIotMqtt_FreeMessage
        #define AwsIotMqtt_FreeMessage    AwsIot_FreeMessageBuffer
    #endif

    /**
     * @brief Allocate an #_mqttOperation_t. This function should have the same
     * signature as [malloc]
     * (http://pubs.opengroup.org/onlinepubs/9699919799/functions/malloc.html).
     */
    #ifndef AwsIotMqtt_MallocOperation
        #define AwsIotMqtt_MallocOperation    AwsIot_MallocMqttOperation
    #endif

    /**
     * @brief Free an #_mqttOperation_t. This function should have the same
     * signature as [free]
     * (http://pubs.opengroup.org/onlinepubs/9699919799/functions/free.html).
     */
    #ifndef AwsIotMqtt_FreeOperation
        #define AwsIotMqtt_FreeOperation    AwsIot_FreeMqttOperation
    #endif

    /**
     * @brief Allocate an #_mqttSubscription_t. This function should have the
     * same signature as [malloc]
     * (http://pubs.opengroup.org/onlinepubs/9699919799/functions/malloc.html).
     */
    #ifndef AwsIotMqtt_MallocSubscription
        #define AwsIotMqtt_MallocSubscription    AwsIot_MallocMqttSubscription
    #endif

    /**
     * @brief Free an #_mqttSubscription_t. This function should have the same
     * signature as [free]
     * (http://pubs.opengroup.org/onlinepubs/9699919799/functions/free.html).
     */
    #ifndef AwsIotMqtt_FreeSubscription
        #define AwsIotMqtt_FreeSubscription    AwsIot_FreeMqttSubscription
    #endif

    /**
     * @brief Allocate an #_mqttTimerEvent_t. This function should have the same
     * signature as [malloc]
     * (http://pubs.opengroup.org/onlinepubs/9699919799/functions/malloc.html).
     */
    #ifndef AwsIotMqtt_MallocTimerEvent
        #define AwsIotMqtt_MallocTimerEvent    AwsIot_MallocMqttTimerEvent
    #endif

    /**
     * @brief Free an #_mqttTimerEvent_t. This function should have the same
     * signature as [free]
     * (http://pubs.opengroup.org/onlinepubs/9699919799/functions/free.html).
     */
    #ifndef AwsIotMqtt_FreeTimerEvent
        #define AwsIotMqtt_FreeTimerEvent    AwsIot_FreeMqttTimerEvent
    #endif
#else /* if AWS_IOT_STATIC_MEMORY_ONLY */
    #include <stdlib.h>

    #ifndef AwsIotMqtt_MallocConnection
        #define AwsIotMqtt_MallocConnection    malloc
    #endif

    #ifndef AwsIotMqtt_FreeConnection
        #define AwsIotMqtt_FreeConnection    free
    #endif

    #ifndef AwsIotMqtt_MallocMessage
        #define AwsIotMqtt_MallocMessage    malloc
    #endif

    #ifndef AwsIotMqtt_FreeMessage
        #define AwsIotMqtt_FreeMessage    free
    #endif

    #ifndef AwsIotMqtt_MallocOperation
        #define AwsIotMqtt_MallocOperation    malloc
    #endif

    #ifndef AwsIotMqtt_FreeOperation
        #define AwsIotMqtt_FreeOperation    free
    #endif

    #ifndef AwsIotMqtt_MallocSubscription
        #define AwsIotMqtt_MallocSubscription    malloc
    #endif

    #ifndef AwsIotMqtt_FreeSubscription
        #define AwsIotMqtt_FreeSubscription    free
    #endif

    #ifndef AwsIotMqtt_MallocTimerEvent
        #define AwsIotMqtt_MallocTimerEvent    malloc
    #endif

    #ifndef AwsIotMqtt_FreeTimerEvent
        #define AwsIotMqtt_FreeTimerEvent    free
    #endif
#endif /* if AWS_IOT_STATIC_MEMORY_ONLY */

/**
 * @cond DOXYGEN_IGNORE
 * Doxygen should ignore this section.
 *
 * Provide default values for undefined configuration constants.
 */
#ifndef AWS_IOT_MQTT_ENABLE_METRICS
    #define AWS_IOT_MQTT_ENABLE_METRICS                 ( 1 )
#endif
#ifndef AWS_IOT_MQTT_ENABLE_SERIALIZER_OVERRIDES
    #define AWS_IOT_MQTT_ENABLE_SERIALIZER_OVERRIDES    ( 0 )
#endif
#ifndef AWS_IOT_MQTT_MAX_CALLBACK_THREADS
    #define AWS_IOT_MQTT_MAX_CALLBACK_THREADS           ( 2 )
#endif
#ifndef AWS_IOT_MQTT_MAX_SEND_THREADS
    #define AWS_IOT_MQTT_MAX_SEND_THREADS               ( 1 )
#endif
#ifndef AWS_IOT_MQTT_TEST
    #define AWS_IOT_MQTT_TEST                           ( 0 )
#endif
#ifndef AWS_IOT_MQTT_RESPONSE_WAIT_MS
    #define AWS_IOT_MQTT_RESPONSE_WAIT_MS               ( 1000 )
#endif
#ifndef AWS_IOT_MQTT_RETRY_MS_CEILING
    #define AWS_IOT_MQTT_RETRY_MS_CEILING               ( 60000 )
#endif
#ifndef AWS_IOT_MQTT_TIMER_EVENT_THRESHOLD_MS
    #define AWS_IOT_MQTT_TIMER_EVENT_THRESHOLD_MS       ( 100 )
#endif
/** @endcond */

/*
 * Constants related to limits defined in AWS Service Limits.
 *
 * For details, see
 * https://docs.aws.amazon.com/general/latest/gr/aws_service_limits.html
 *
 * Used to validate parameters if when connecting to an AWS IoT MQTT server.
 */
#define _AWS_IOT_MQTT_SERVER_MIN_KEEPALIVE                      ( 30 )   /**< @brief Minumum keep-alive interval accepted by AWS IoT. */
#define _AWS_IOT_MQTT_SERVER_MAX_KEEPALIVE                      ( 1200 ) /**< @brief Maximum keep-alive interval accepted by AWS IoT. */
#define _AWS_IOT_MQTT_SERVER_MAX_CLIENTID                       ( 128 )  /**< @brief Maximum length of client identifier accepted by AWS IoT. */
#define _AWS_IOT_MQTT_SERVER_MAX_TOPIC_LENGTH                   ( 256 )  /**< @brief Maximum length of topic names or filters accepted by AWS IoT. */
#define _AWS_IOT_MQTT_SERVER_MAX_TOPIC_FILTERS_PER_SUBSCRIBE    ( 8 )    /**< @brief Maximum number of topic filters in a single SUBSCRIBE packet. */

/*
 * MQTT control packet type and flags. Always the first byte of an MQTT
 * packet.
 *
 * For details, see
 * http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/csprd02/mqtt-v3.1.1-csprd02.html#_Toc385349757
 */
#define _MQTT_PACKET_TYPE_CONNECT                               ( ( uint8_t ) 0x10U ) /**< @brief CONNECT (client-to-server). */
#define _MQTT_PACKET_TYPE_CONNACK                               ( ( uint8_t ) 0x20U ) /**< @brief CONNACK (server-to-client). */
#define _MQTT_PACKET_TYPE_PUBLISH                               ( ( uint8_t ) 0x30U ) /**< @brief PUBLISH (bi-directional). */
#define _MQTT_PACKET_TYPE_PUBACK                                ( ( uint8_t ) 0x40U ) /**< @brief PUBACK (server-to-client). */
#define _MQTT_PACKET_TYPE_SUBSCRIBE                             ( ( uint8_t ) 0x82U ) /**< @brief SUBSCRIBE (client-to-server). */
#define _MQTT_PACKET_TYPE_SUBACK                                ( ( uint8_t ) 0x90U ) /**< @brief SUBACK (server-to-client). */
#define _MQTT_PACKET_TYPE_UNSUBSCRIBE                           ( ( uint8_t ) 0xa2U ) /**< @brief UNSUBSCRIBE (client-to-server). */
#define _MQTT_PACKET_TYPE_UNSUBACK                              ( ( uint8_t ) 0xb0U ) /**< @brief UNSUBACK (server-to-client). */
#define _MQTT_PACKET_TYPE_PINGREQ                               ( ( uint8_t ) 0xc0U ) /**< @brief PINGREQ (client-to-server). */
#define _MQTT_PACKET_TYPE_PINGRESP                              ( ( uint8_t ) 0xd0U ) /**< @brief PINGRESP (server-to-client). */
#define _MQTT_PACKET_TYPE_DISCONNECT                            ( ( uint8_t ) 0xe0U ) /**< @brief DISCONNECT (client-to-server). */

/*---------------------- MQTT internal data structures ----------------------*/

/**
 * @cond DOXYGEN_IGNORE
 * Doxygen should ignore this section.
 *
 * Forward declarations.
 */
typedef struct _mqttSubscription   _mqttSubscription_t;
typedef struct _mqttOperation      _mqttOperation_t;
typedef struct _mqttConnection     _mqttConnection_t;
typedef struct _mqttTimerEvent     _mqttTimerEvent_t;
/** @endcond */

/**
 * @brief Represents a subscription stored in an MQTT connection.
 */
typedef struct _mqttSubscription
{
    _mqttSubscription_t * pNext;     /**< @brief Next element in the list. */
    _mqttSubscription_t * pPrevious; /**< @brief Previous element in the list. */

    int references;                  /**< @brief How many subscription callbacks are using this subscription. */

    /**
     * @brief Tracks whether @ref mqtt_function_unsubscribe has been called for
     * this subscription.
     *
     * If there are active subscription callbacks, @ref mqtt_function_unsubscribe
     * cannot remove this subscription. Instead, it will set this flag, which
     * schedules the removal of this subscription once all subscription callbacks
     * terminate.
     */
    bool unsubscribed;

    struct
    {
        uint16_t identifier;           /**< @brief Packet identifier. */
        size_t order;                  /**< @brief Order in the packet's list of subscriptions. */
    } packetInfo;                      /**< @brief Information about the SUBSCRIBE packet that registered this subscription. */

    AwsIotMqttCallbackInfo_t callback; /**< @brief Callback information for this subscription. */

    uint16_t topicFilterLength;        /**< @brief Length of #_mqttSubscription_t.pTopicFilter. */
    char pTopicFilter[];               /**< @brief The subscription topic filter. */
} _mqttSubscription_t;

/**
 * @brief Internal structure to hold data on an MQTT connection.
 */
typedef struct _mqttConnection
{
    bool errorOccurred;                   /**< @brief Tracks if a protocol violation or other error occurred. */
    bool awsIotMqttMode;                  /**< @brief Specifies if this connection is to an AWS IoT MQTT server. */
    AwsIotMqttNetIf_t network;            /**< @brief Network interface provided to @ref mqtt_function_connect. */
    AwsIotList_t subscriptionList;        /**< @brief Holds subscriptions associated with this connection. */

    AwsIotMutex_t mutex;                  /**< @brief Prevents concurrent threads from modifying connection data. */
    AwsIotTimer_t timer;                  /**< @brief Expires when a timer event should be processed. */
    AwsIotList_t timerEventList;          /**< @brief List of active timer events. */
    uint16_t keepAliveSeconds;            /**< @brief Keep-alive interval. */
    _mqttOperation_t * pPingreqOperation; /**< @brief PINGREQ operation. Only used if keep-alive is active. */
    _mqttTimerEvent_t * pKeepAliveEvent;  /**< @brief When to process a keep-alive. Only used if keep-alive is active. */
} _mqttConnection_t;

/**
 * @brief Internal structure representing a single MQTT operation, such as
 * CONNECT, SUBSCRIBE, PUBLISH, etc.
 *
 * Queues of these structures keeps track of all in-progress MQTT operations.
 */
typedef struct _mqttOperation
{
    /* Pointers to neighboring queue elements. */
    _mqttOperation_t * pNext;            /**< @brief Next (older) item in the queue. */
    _mqttOperation_t * pPrevious;        /**< @brief Previous (newer) item in the queue. */

    bool incomingPublish;                /**< @brief Set to true if this operation an incoming PUBLISH. */
    _mqttConnection_t * pMqttConnection; /**< @brief MQTT connection associated with this operation. */

    union
    {
        /* If incomingPublish is false, this struct is valid. */
        struct
        {
            /* Basic operation information. */
            AwsIotMqttOperationType_t operation; /**< @brief What operation this structure represents. */
            uint32_t flags;                      /**< @brief Flags passed to the function that created this operation. */
            uint16_t packetIdentifier;           /**< @brief The packet identifier used with this operation. */

            /* Serialized packet and size. */
            uint8_t * pMqttPacket; /**< @brief The MQTT packet to send over the network. */
            size_t packetSize;     /**< @brief Size of `pMqttPacket`. */

            /* How to notify of an operation's completion. */
            union
            {
                AwsIotSemaphore_t waitSemaphore;   /**< @brief Semaphore to be used with @ref mqtt_function_wait. */
                AwsIotMqttCallbackInfo_t callback; /**< @brief User-provided callback function and parameter. */
            } notify;                              /**< @brief How to notify of this operation's completion. */
            AwsIotMqttError_t status;              /**< @brief Result of this operation. This is reported once a response is received. */

            _mqttTimerEvent_t * pPublishRetry;     /**< @brief How an operation will be retried. Only used for QoS 1 or 2 publishes. */
        };

        /* If incomingPublish is true, this struct is valid. */
        struct
        {
            AwsIotMqttPublishInfo_t publishInfo;   /**< @brief Deserialized PUBLISH. */
            _mqttOperation_t * pNextPublish;       /**< @brief Pointer to the next PUBLISH in the data stream. */
            const void * pReceivedData;            /**< @brief Any buffer associated with this PUBLISH that should be freed. */
            void ( * freeReceivedData )( void * ); /**< @brief Function called to free `pReceivedData`. */
        };
    };
} _mqttOperation_t;

/**
 * @brief Represents an operation that is subject to a timer.
 *
 * These events are queued per MQTT connection. They are sorted by their
 * expiration time.
 */
typedef struct _mqttTimerEvent
{
    _mqttTimerEvent_t * pNext;     /**< @brief Next (later) item in the queue. */
    _mqttTimerEvent_t * pPrevious; /**< @brief Previous (sooner) item in the queue. */

    uint64_t expirationTime;       /**< @brief When this event should be processed. */
    _mqttOperation_t * pOperation; /**< @brief The MQTT operation associated with this event. */

    /* Valid members depend on the operation associated with this event
     * (PINGREQ or PUBLISH). The checkPingresp member is valid for PINGREQ,
     * and the retry member is valid for PUBLISH. */
    union
    {
        bool checkPingresp; /**< @brief This keep-alive operation is waiting for PINGRESP. */

        struct
        {
            int count;           /**< @brief The number of times this message has been retried. */
            int limit;           /**< @brief The maximum number of times to retry this message. */
            uint64_t nextPeriod; /**< @brief How long to wait before the next retry. */
        } retry;
    };
} _mqttTimerEvent_t;

/* Declarations of the MQTT queues for internal MQTT files. */
extern AwsIotQueue_t _AwsIotMqttSendQueue;
extern AwsIotQueue_t _AwsIotMqttReceiveQueue;
extern AwsIotQueue_t _AwsIotMqttCallbackQueue;

/*-------------------- MQTT struct validation functions ---------------------*/

/**
 * @brief Check that an #AwsIotMqttNetIf_t is valid.
 *
 * @param[in] pNetworkInterface The #AwsIotMqttNetIf_t to validate.
 *
 * @return `true` if `pNetworkInterface` is valid; `false` otherwise.
 */
bool AwsIotMqttInternal_ValidateNetIf( const AwsIotMqttNetIf_t * const pNetworkInterface );

/**
 * @brief Check that an #AwsIotMqttConnectInfo_t is valid.
 *
 * @param[in] pConnectInfo The #AwsIotMqttConnectInfo_t to validate.
 *
 * @return `true` if `pConnectInfo` is valid; `false` otherwise.
 */
bool AwsIotMqttInternal_ValidateConnect( const AwsIotMqttConnectInfo_t * const pConnectInfo );

/**
 * @brief Check that an #AwsIotMqttPublishInfo_t is valid.
 *
 * @param[in] awsIotMqttMode Specifies if this PUBLISH packet is being sent to
 * an AWS IoT MQTT server.
 * @param[in] pPublishInfo The #AwsIotMqttPublishInfo_t to validate.
 *
 * @return `true` if `pPublishInfo` is valid; `false` otherwise.
 */
bool AwsIotMqttInternal_ValidatePublish( bool awsIotMqttMode,
                                         const AwsIotMqttPublishInfo_t * const pPublishInfo );

/**
 * @brief Check that an #AwsIotMqttReference_t is valid and waitable.
 *
 * @param[in] reference The #AwsIotMqttReference_t to validate.
 *
 * @return `true` if `reference` is valid; `false` otherwise.
 */
bool AwsIotMqttInternal_ValidateReference( AwsIotMqttReference_t reference );

/**
 * @brief Check that a list of #AwsIotMqttSubscription_t is valid.
 *
 * @param[in] operation Either #AWS_IOT_MQTT_SUBSCRIBE or #AWS_IOT_MQTT_UNSUBSCRIBE.
 * Some parameters are not validated for #AWS_IOT_MQTT_UNSUBSCRIBE.
 * @param[in] awsIotMqttMode Specifies if this SUBSCRIBE packet is being sent to
 * an AWS IoT MQTT server.
 * @param[in] pListStart First element of the list to validate.
 * @param[in] listSize Number of elements in the subscription list.
 *
 * @return `true` if every element in the list is valid; `false` otherwise.
 */
bool AwsIotMqttInternal_ValidateSubscriptionList( AwsIotMqttOperationType_t operation,
                                                  bool awsIotMqttMode,
                                                  const AwsIotMqttSubscription_t * const pListStart,
                                                  size_t listSize );

/*------------------------ Queue and list functions -------------------------*/

/**
 * @brief Create all the MQTT operation queues. Called by @ref mqtt_function_init
 * when initializing the MQTT library.
 *
 * @return #AWS_IOT_MQTT_SUCCESS or #AWS_IOT_MQTT_INIT_FAILED.
 */
AwsIotMqttError_t AwsIotMqttInternal_CreateQueues( void );

/**
 * @brief Free resources taken by #AwsIotMqttInternal_CreateQueues.
 *
 * No parameters, no return values.
 */
void AwsIotMqttInternal_DestroyQueues( void );

/**
 * @brief Create #_AwsIotMqttSendQueue, #_AwsIotMqttReceiveQueue, or
 * #_AwsIotMqttCallbackQueue.
 *
 * @param[in] pQueue Pointer to the queue to create.
 * @param[in] notifyRoutine The queue's notification routine.
 * @param[in] maxNotifyThreads The maximum number of notification threads.
 *
 * @return #AWS_IOT_MQTT_SUCCESS or #AWS_IOT_MQTT_INIT_FAILED.
 */
AwsIotMqttError_t AwsIotMqttInternal_CreateOperationQueue( AwsIotQueue_t * const pQueue,
                                                           AwsIotThreadRoutine_t notifyRoutine,
                                                           uint32_t maxNotifyThreads );

/**
 * @brief Create #_mqttConnection_t.subscriptionList.
 *
 * @param[in] pList Pointer to the list to create.
 *
 * @return #AWS_IOT_MQTT_SUCCESS or #AWS_IOT_MQTT_INIT_FAILED.
 */
AwsIotMqttError_t AwsIotMqttInternal_CreateSubscriptionList( AwsIotList_t * const pList );

/**
 * @brief Create #_mqttConnection_t.timerEventList.
 *
 * @param[in] pList Pointer to the list to create
 *
 * @return #AWS_IOT_MQTT_SUCCESS or #AWS_IOT_MQTT_INIT_FAILED.
 */
AwsIotMqttError_t AwsIotMqttInternal_CreateTimerEventList( AwsIotList_t * const pList );

/**
 * @brief Inserts a timer event into the (sorted) timer list.
 *
 * @param[in] pTimerList The list to modify.
 * @param[in] pTimerEvent The timer event to insert.
 *
 * @note This function must be called with [pTimerList->mutex](@ref AwsIotList_t.mutex)
 * locked.
 */
void AwsIotMqttInternal_TimerListInsert( AwsIotList_t * const pTimerList,
                                         _mqttTimerEvent_t * const pTimerEvent );

/*-------------------- MQTT packet serializer functions ---------------------*/

/**
 * @brief Initialize the MQTT packet serializer. Called by @ref mqtt_function_init
 * when initializing the MQTT library.
 *
 * @return #AWS_IOT_MQTT_SUCCESS or #AWS_IOT_MQTT_INIT_FAILED.
 */
AwsIotMqttError_t AwsIotMqttInternal_InitSerialize( void );

/**
 * @brief Free resources taken by #AwsIotMqttInternal_InitSerialize.
 *
 * No parameters, no return values.
 */
void AwsIotMqttInternal_CleanupSerialize( void );

/**
 * @brief Get the MQTT packet type from a stream of bytes.
 *
 * @param[in] pPacket Pointer to the beginning of the byte stream.
 * @param[in] packetSize Size of the byte stream.
 *
 * @return One of the server-to-client MQTT packet types.
 *
 * @note This function is only used for incoming packets, and may not work
 * correctly for outgoing packets.
 */
uint8_t AwsIotMqttInternal_GetPacketType( const uint8_t * const pPacket,
                                          size_t packetSize );

/**
 * @brief Generate a CONNECT packet from the given parameters.
 *
 * @param[in] pConnectInfo User-provided CONNECT information.
 * @param[in] pWillInfo User-provided Last Will and Testament information.
 * @param[out] pConnectPacket Where the CONNECT packet is written.
 * @param[out] pPacketSize Size of the packet written to `pConnectPacket`.
 *
 * @return #AWS_IOT_MQTT_SUCCESS or #AWS_IOT_MQTT_NO_MEMORY.
 */
AwsIotMqttError_t AwsIotMqttInternal_SerializeConnect( const AwsIotMqttConnectInfo_t * const pConnectInfo,
                                                       const AwsIotMqttPublishInfo_t * const pWillInfo,
                                                       uint8_t ** const pConnectPacket,
                                                       size_t * const pPacketSize );

/**
 * @brief Deserialize a CONNACK packet.
 *
 * Converts the packet from a stream of bytes to an #AwsIotMqttError_t. Also
 * prints out debug log messages about the packet.
 * @param[in] pConnackStart Pointer to the start of a CONNACK packet.
 * @param[in] dataLength Length of the data stream after `pConnackStart`.
 * @param[out] pBytesProcessed The number of bytes in the data stream processed
 * by this function. For CONNACK, this is always 4.
 *
 * @return #AWS_IOT_MQTT_SUCCESS if CONNACK specifies that CONNECT was accepted;
 * #AWS_IOT_MQTT_SERVER_REFUSED if CONNACK specifies that CONNECT was rejected;
 * #AWS_IOT_MQTT_BAD_RESPONSE if the CONNACK packet doesn't follow MQTT spec.
 */
AwsIotMqttError_t AwsIotMqttInternal_DeserializeConnack( const uint8_t * const pConnackStart,
                                                         size_t dataLength,
                                                         size_t * const pBytesProcessed );

/**
 * @brief Generate a PUBLISH packet from the given parameters.
 *
 * @param[in] pPublishInfo User-provided PUBLISH information.
 * @param[out] pPublishPacket Where the PUBLISH packet is written.
 * @param[out] pPacketSize Size of the packet written to `pPublishPacket`.
 * @param[out] pPacketIdentifier The packet identifier generated for this PUBLISH.
 *
 * @return #AWS_IOT_MQTT_SUCCESS or #AWS_IOT_MQTT_NO_MEMORY.
 */
AwsIotMqttError_t AwsIotMqttInternal_SerializePublish( const AwsIotMqttPublishInfo_t * const pPublishInfo,
                                                       uint8_t ** const pPublishPacket,
                                                       size_t * const pPacketSize,
                                                       uint16_t * const pPacketIdentifier );

/**
 * @brief Set the DUP bit in a QoS 1 or 2 PUBLISH packet.
 *
 * @param[in] awsIotMqttMode Specifies if this PUBLISH packet is being sent to
 * an AWS IoT MQTT server.
 * @param[in] pPublishPacket Pointer to the PUBLISH packet to modify.
 * @param[out] pNewPacketIdentifier Since AWS IoT does not support the DUP flag,
 * a new packet identifier is generated and should be written here. This parameter
 * is only used when connected to an AWS IoT MQTT server.
 *
 * @note See #AwsIotMqttPublishInfo_t for caveats with retransmission to the
 * AWS IoT MQTT server.
 */
void AwsIotMqttInternal_PublishSetDup( bool awsIotMqttMode,
                                       uint8_t * const pPublishPacket,
                                       uint16_t * const pNewPacketIdentifier );

/**
 * @brief Deserialize a PUBLISH packet received from the server.
 *
 * Converts the packet from a stream of bytes to an #AwsIotMqttPublishInfo_t and
 * extracts the packet identifier. Also prints out debug log messages about the
 * packet.
 *
 * @param[in] pPublishStart Pointer to the start of a PUBLISH packet.
 * @param[in] dataLength Length of the data stream after `pPublishStart`.
 * @param[out] pOutput Where the deserialized PUBLISH will be written.
 * @param[out] pPacketIdentifier The packet identifier in the PUBLISH.
 * @param[out] pBytesProcessed The number of bytes in the data stream processed
 * by this function.
 *
 * @return #AWS_IOT_MQTT_SUCCESS if PUBLISH is valid; #AWS_IOT_MQTT_BAD_RESPONSE
 * if the PUBLISH packet doesn't follow MQTT spec.
 */
AwsIotMqttError_t AwsIotMqttInternal_DeserializePublish( const uint8_t * const pPublishStart,
                                                         size_t dataLength,
                                                         AwsIotMqttPublishInfo_t * const pOutput,
                                                         uint16_t * const pPacketIdentifier,
                                                         size_t * const pBytesProcessed );

/**
 * @brief Generate a PUBACK packet for the given packet identifier.
 *
 * @param[in] packetIdentifier The packet identifier to place in PUBACK.
 * @param[out] pPubackPacket Where the PUBACK packet is written.
 * @param[out] pPacketSize Size of the packet written to `pPubackPacket`.
 *
 * @return #AWS_IOT_MQTT_SUCCESS or #AWS_IOT_MQTT_NO_MEMORY.
 */
AwsIotMqttError_t AwsIotMqttInternal_SerializePuback( uint16_t packetIdentifier,
                                                      uint8_t ** const pPubackPacket,
                                                      size_t * const pPacketSize );

/**
 * @brief Deserialize a PUBACK packet.
 *
 * Converts the packet from a stream of bytes to an #AwsIotMqttError_t and extracts
 * the packet identifier. Also prints out debug log messages about the packet.
 *
 * @param[in] pPubackStart Pointer to the start of a PUBACK packet.
 * @param[in] dataLength Length of the data stream after `pPubackStart`.
 * @param[out] pPacketIdentifier The packet identifier in the PUBACK.
 * @param[out] pBytesProcessed The number of bytes in the data stream processed
 * by this function. For PUBACK, this is always 4.
 *
 * @return #AWS_IOT_MQTT_SUCCESS if PUBACK is valid; #AWS_IOT_MQTT_BAD_RESPONSE
 * if the PUBACK packet doesn't follow MQTT spec.
 */
AwsIotMqttError_t AwsIotMqttInternal_DeserializePuback( const uint8_t * const pPubackStart,
                                                        size_t dataLength,
                                                        uint16_t * const pPacketIdentifier,
                                                        size_t * const pBytesProcessed );

/**
 * @brief Generate a SUBSCRIBE packet from the given parameters.
 *
 * @param[in] pSubscriptionList User-provided array of subscriptions.
 * @param[in] subscriptionCount Size of `pSubscriptionList`.
 * @param[out] pSubscribePacket Where the SUBSCRIBE packet is written.
 * @param[out] pPacketSize Size of the packet written to `pSubscribePacket`.
 * @param[out] pPacketIdentifier The packet identifier generated for this SUBSCRIBE.
 *
 * @return #AWS_IOT_MQTT_SUCCESS or #AWS_IOT_MQTT_NO_MEMORY.
 */
AwsIotMqttError_t AwsIotMqttInternal_SerializeSubscribe( const AwsIotMqttSubscription_t * const pSubscriptionList,
                                                         size_t subscriptionCount,
                                                         uint8_t ** const pSubscribePacket,
                                                         size_t * const pPacketSize,
                                                         uint16_t * const pPacketIdentifier );

/**
 * @brief Deserialize a SUBACK packet.
 *
 * Converts the packet from a stream of bytes to an #AwsIotMqttError_t and extracts
 * the packet identifier. Also prints out debug log messages about the packet.
 *
 * @param[in] mqttConnection The MQTT connection associated with the subscription.
 * Rejected topic filters are removed from this connection.
 * @param[in] pSubackStart Pointer to the start of a SUBACK packet.
 * @param[in] dataLength Length of the data stream after `pSubackStart`.
 * @param[out] pPacketIdentifier The packet identifier in the SUBACK.
 * @param[out] pBytesProcessed The number of bytes in the data stream processed by
 * this function.
 *
 * @return #AWS_IOT_MQTT_SUCCESS if SUBACK is valid; #AWS_IOT_MQTT_BAD_RESPONSE
 * if the SUBACK packet doesn't follow MQTT spec.
 */
AwsIotMqttError_t AwsIotMqttInternal_DeserializeSuback( AwsIotMqttConnection_t mqttConnection,
                                                        const uint8_t * const pSubackStart,
                                                        size_t dataLength,
                                                        uint16_t * const pPacketIdentifier,
                                                        size_t * const pBytesProcessed );

/**
 * @brief Generate an UNSUBSCRIBE packet from the given parameters.
 *
 * @param[in] pSubscriptionList User-provided array of subscriptions to remove.
 * @param[in] subscriptionCount Size of `pSubscriptionList`.
 * @param[out] pUnsubscribePacket Where the UNSUBSCRIBE packet is written.
 * @param[out] pPacketSize Size of the packet written to `pUnsubscribePacket`.
 * @param[out] pPacketIdentifier The packet identifier generated for this UNSUBSCRIBE.
 *
 * @return #AWS_IOT_MQTT_SUCCESS or #AWS_IOT_MQTT_NO_MEMORY.
 */
AwsIotMqttError_t AwsIotMqttInternal_SerializeUnsubscribe( const AwsIotMqttSubscription_t * const pSubscriptionList,
                                                           size_t subscriptionCount,
                                                           uint8_t ** const pUnsubscribePacket,
                                                           size_t * const pPacketSize,
                                                           uint16_t * const pPacketIdentifier );

/**
 * @brief Deserialize a UNSUBACK packet.
 *
 * Converts the packet from a stream of bytes to an #AwsIotMqttError_t and extracts
 * the packet identifier. Also prints out debug log messages about the packet.
 *
 * @param[in] pUnsubackStart Pointer to the start of an UNSUBACK packet.
 * @param[in] dataLength Length of the data stream after `pUnsubackStart`.
 * @param[out] pPacketIdentifier The packet identifier in the UNSUBACK.
 * @param[out] pBytesProcessed The number of bytes in the data stream processed by
 * this function. For UNSUBACK, this is always 4.
 *
 * @return #AWS_IOT_MQTT_SUCCESS if UNSUBACK is valid; #AWS_IOT_MQTT_BAD_RESPONSE
 * if the UNSUBACK packet doesn't follow MQTT spec.
 */
AwsIotMqttError_t AwsIotMqttInternal_DeserializeUnsuback( const uint8_t * const pUnsubackStart,
                                                          size_t dataLength,
                                                          uint16_t * const pPacketIdentifier,
                                                          size_t * const pBytesProcessed );

/**
 * @brief Generate a PINGREQ packet.
 *
 * @param[out] pPingreqPacket Where the PINGREQ packet is written.
 * @param[out] pPacketSize Size of the packet written to `pPingreqPacket`.
 *
 * @return Always returns #AWS_IOT_MQTT_SUCCESS.
 */
AwsIotMqttError_t AwsIotMqttInternal_SerializePingreq( uint8_t ** const pPingreqPacket,
                                                       size_t * const pPacketSize );

/**
 * @brief Deserialize a PINGRESP packet.
 *
 * Converts the packet from a stream of bytes to an #AwsIotMqttError_t. Also
 * prints out debug log messages about the packet.
 * @param[in] pPingrespStart Pointer to the start of a PINGRESP packet.
 * @param[in] dataLength Length of the data stream after `pPingrespStart`.
 * @param[out] pBytesProcessed The number of bytes in the data stream processed by
 * this function. For PINGRESP, this is always 2.
 *
 * @return #AWS_IOT_MQTT_SUCCESS if PINGRESP is valid; #AWS_IOT_MQTT_BAD_RESPONSE
 * if the PINGRESP packet doesn't follow MQTT spec.
 */
AwsIotMqttError_t AwsIotMqttInternal_DeserializePingresp( const uint8_t * const pPingrespStart,
                                                          size_t dataLength,
                                                          size_t * const pBytesProcessed );

/**
 * @brief Generate a DISCONNECT packet.
 *
 * @param[out] pDisconnectPacket Where the DISCONNECT packet is written.
 * @param[out] pPacketSize Size of the packet written to `pDisconnectPacket`.
 *
 * @return Always returns #AWS_IOT_MQTT_SUCCESS.
 */
AwsIotMqttError_t AwsIotMqttInternal_SerializeDisconnect( uint8_t ** const pDisconnectPacket,
                                                          size_t * const pPacketSize );

/**
 * @brief Free a packet generated by the serializer.
 *
 * @param[in] pPacket The packet to free.
 */
void AwsIotMqttInternal_FreePacket( uint8_t * pPacket );

/*-------------------- MQTT operation record functions ----------------------*/

/**
 * @brief Create a record for a new in-progress MQTT operation.
 *
 * @param[out] pNewOperation Set to point to the new operation on success.
 * @param[in] flags Flags variable passed to a user-facing MQTT function.
 * @param[in] pCallbackInfo User-provided callback function and parameter.
 *
 * @return #AWS_IOT_MQTT_SUCCESS, #AWS_IOT_MQTT_BAD_PARAMETER, or #AWS_IOT_MQTT_NO_MEMORY.
 */
AwsIotMqttError_t AwsIotMqttInternal_CreateOperation( _mqttOperation_t ** const pNewOperation,
                                                      uint32_t flags,
                                                      const AwsIotMqttCallbackInfo_t * const pCallbackInfo );

/**
 * @brief Free resources used to record an MQTT operation. This is called when
 * the operation completes.
 *
 * @param[in] pData The operation which completed. This parameter is of type
 * `void*` for compatibility with @ref queue_function_removeallmatches.
 */
void AwsIotMqttInternal_DestroyOperation( void * pData );

/**
 * @brief Notify of a completed MQTT operation.
 *
 * @param[in] pOperation The MQTT operation which completed.
 *
 * Depending on the parameters passed to a user-facing MQTT function, the
 * notification will cause @ref mqtt_function_wait to return or invoke a
 * user-provided callback.
 */
void AwsIotMqttInternal_Notify( _mqttOperation_t * const pOperation );

/**
 * @brief Search a queue of in-progress MQTT operations using an operation name
 * and packet identifier. Removes the operation from the queue if found.
 *
 * Allows operations to be removed from the middle of the queue.
 * @param[in] pQueue Which queue to search.
 * @param[in] operation The operation type to look for.
 * @param[in] pPacketIdentifier A packet identifier to match. Pass NULL to ignore.
 * @param[out] pOutput Set to point to the found operation.
 *
 * @return #AWS_IOT_MQTT_SUCCESS if a corresponding operation was found, otherwise
 * #AWS_IOT_MQTT_BAD_PARAMETER if not found.
 */
AwsIotMqttError_t AwsIotMqttInternal_OperationFind( AwsIotQueue_t * const pQueue,
                                                    AwsIotMqttOperationType_t operation,
                                                    const uint16_t * const pPacketIdentifier,
                                                    _mqttOperation_t ** const pOutput );

/**
 * @brief Search a queue of in-progress MQTT operations using a pointer.
 * Removes the operation from the queue if found.
 *
 * @param[in] pQueue Which queue to search.
 * @param[in] pTarget The operation to look for.
 *
 * @return #AWS_IOT_MQTT_SUCCESS if the `pTarget` points to an in-progress operation,
 * otherwise #AWS_IOT_MQTT_BAD_PARAMETER if not found.
 */
AwsIotMqttError_t AwsIotMqttInternal_OperationFindPointer( AwsIotQueue_t * const pQueue,
                                                           _mqttOperation_t * const pTarget );

/*------------------- Subscription management functions ---------------------*/

/**
 * @brief Add an array of subscriptions to the subscription manager.
 *
 * @param[in] pMqttConnection The MQTT connection associated with the subscriptions.
 * @param[in] subscribePacketIdentifier Packet identifier for the subscriptions'
 * SUBSCRIBE packet.
 * @param[in] pSubscriptionList The first element in the array.
 * @param[in] subscriptionCount Number of elements in `pSubscriptionList`.
 *
 * @return #AWS_IOT_MQTT_SUCCESS or #AWS_IOT_MQTT_NO_MEMORY.
 */
AwsIotMqttError_t AwsIotMqttInternal_AddSubscriptions( _mqttConnection_t * const pMqttConnection,
                                                       uint16_t subscribePacketIdentifier,
                                                       const AwsIotMqttSubscription_t * const pSubscriptionList,
                                                       size_t subscriptionCount );

/**
 * @brief Process a received PUBLISH from the server, invoking any subscription
 * callbacks that have a matching topic filter.
 *
 * @param[in] pMqttConnection The MQTT connection associated with the received
 * PUBLISH.
 * @param[in] pCallbackParam The parameter to pass to a PUBLISH callback.
 */
void AwsIotMqttInternal_ProcessPublish( _mqttConnection_t * const pMqttConnection,
                                        AwsIotMqttCallbackParam_t * const pCallbackParam );

/**
 * @brief Remove a single subscription from the subscription manager by
 * packetIdentifier and order.
 *
 * @param[in] pMqttConnection The MQTT connection associated with the subscriptions.
 * @param[in] packetIdentifier The packet identifier associated with the subscription's
 * SUBSCRIBE packet.
 * @param[in] order The order of the subscription in the SUBSCRIBE packet.
 * Pass `-1` to ignore order and remove all subscriptions for `packetIdentifier`.
 */
void AwsIotMqttInternal_RemoveSubscriptionByPacket( _mqttConnection_t * const pMqttConnection,
                                                    uint16_t packetIdentifier,
                                                    ssize_t order );

/**
 * @brief Remove an array of subscriptions from the subscription manager by
 * topic filter.
 *
 * @param[in] pMqttConnection The MQTT connection associated with the subscriptions.
 * @param[in] pSubscriptionList The first element in the array.
 * @param[in] subscriptionCount Number of elements in `pSubscriptionList`.
 */
void AwsIotMqttInternal_RemoveSubscriptionByTopicFilter( _mqttConnection_t * const pMqttConnection,
                                                         const AwsIotMqttSubscription_t * const pSubscriptionList,
                                                         size_t subscriptionCount );

#endif /* ifndef _AWS_IOT_MQTT_INTERNAL_H_ */