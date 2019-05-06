/**
 * Copyright (C) 2018 Alibaba.inc, All rights reserved.
 *
 * @file AGGatt.h
 * @brief gatt client interface
 * @author zhaojian.lzj@alibaba-inc.com
 * @data 2018/04/07
 * @version 1.0
 */

#ifndef AG_GATT_H
#define AG_GATT_H
#include "AGDefine.h"
#ifdef __cplusplus
extern "C" {
#endif
#define GATT_MAX_UUID_LEN 37
typedef enum
{
    EVENT_GATT_CONNECT,
    EVENT_GATT_DISCONNECT,
    EVENT_GATT_MAX
} GattEvent;
typedef enum
{
    GATT_DB_PRIMARY_SERVICE,
    GATT_DB_SECONDARY_SERVICE,
    GATT_DB_INCLUDED_SERVICE,
    GATT_DB_CHARACTERISTIC,
    GATT_DB_DESCRIPTOR,
} GattType;
typedef enum
{
    GATT_WRITE_NO_RESPONSE = 1,
    GATT_WRITE_WITH_RESPONSE = 2,
    GATT_WRITE_PREPARE = 3,
}GattWriteType;

typedef struct
{
    UINT32 id;
    CHAR uuid[GATT_MAX_UUID_LEN];   //服务名称或者属性名称
    GattType type;                     //类型属性
    UINT32 handle;                    //标识
    /*
     * If |type| is |GATT_DB_PRIMARY_SERVICE|, or
     * |GATT_DB_SECONDARY_SERVICE|, this contains the start and end attribute
     * handles.
     */
    UINT32            start_handle;
    UINT32            end_handle;
    /*
     * If |type| is |GATT_DB_CHARACTERISTIC|, this contains the properties of
     * the characteristic.
     */
    UINT8             properties;
} GattServiceElem;

typedef struct
{
    INT32 connId;
    INT32 count;
    GattServiceElem *pElem;
} GattServices;
class IAGGattCallback
{
public:
    //蓝牙初始化完成通知
    /**
     * @brief Inform the gatt init result
     * @param[in] clientIf Client interface
     * @return void
     */
    virtual void onGattInitCallback(INT32 clientIf) = 0;
    //扫描到设备，并告知上层的设备信号
    /**
     * @brief Inform the scan result
     * @param[in] bdAddr The address of remote device
     * @param[in] rssi The rssi of remote devce
     * @param[in] pData adv_data
     * @param[in] size Size of adv_data
     * @return void
     */
    virtual void onLeScan(const CHAR* bdAddr, INT32 rssi, UINT8* pData, INT32 size) = 0;

    //连接回调结果, 连接状态\断开状态
    /**
     * @brief Inform the ble connection result
     * @param[in] connId Connection ID
     * @param[in] bdAddr Peer bluetooth device addr
     * @param[in] event See BT_GATTC_EVENT_T enums
     * @return void
     */
    virtual void onGattEvent(INT32 connId, const CHAR* bdAddr, GattEvent event) = 0;

    //获取服务列表回调
    /**
     * @brief Get the services list
     * @param[in] gattServices Services list
     * @return void
     */
    virtual void onServicesDiscovered(GattServices* gattServices) = 0;

    //返回读取蓝牙属性
    /**
     * @brief Inform characteristic read result
     * @param[in] connId Connection id
     * @param[in] handle Attribute handle
     * @param[in] status Status
     * @param[in] data Att data read
     * @param[in] size Length of data in bytes
     * @return void
     */
    virtual void onCharacteristicRead(INT32 connId, UINT32 handle, INT32 status,
                                        const UINT8* data, INT32 size) = 0;
    //返回写蓝牙属性
    /**
     * @brief Inform characteristic write result
     * @param[in] connId Connection id
     * @param[in] handle Attribute handle
     * @param[in] status Status
     * @return void
     */
    virtual void onCharacteristicWrite(INT32 connId, UINT32 handle, INT32 status) = 0;

    /**
     * @brief Inform descriptor read result
     * @param[in] connId Connection id
     * @param[in] handle Attribute handle
     * @param[in] status Status
     * @param[in] data Payload
     * @param[in] size Length of data in bytes
     * @return void
     */
    virtual void onDescriptorRead(INT32 connId, UINT32 handle, INT32 status,
                                        const UINT8* data, INT32 size) = 0;
    /**
     * @brief Inform descriptor write result
     * @param[in] connId Connection id
     * @param[in] handle Attribute handle
     * @param[in] status Status
     * @return void
     */
    virtual void onDescriptorWrite(INT32 connId, UINT32 handle, INT32 status) = 0;

    /**
     * @brief Notify characteristic changed
     * @param[in] connId Connection id
     * @param[in] handle Attribute handle
     * @param[in] status Status
     * @param[in] data Payload
     * @param[in] size Length of data in bytes
     * @param[in] isNotify Is notify or not
     * @return void
     */
    virtual void onCharacteristicChanged(INT32 connId, UINT32 handle, const CHAR* bdaddr,
                                            const UINT8* data, INT32 size, UINT8 isNotify) = 0;
    //打开蓝牙属性通知
    /**
     * @brief Enable characteristic notify callbac
     * @param[in] connId Connection id
     * @param[in] handle Attribute handle
     * @param[in] registered 1 is success
     * @return void
     */
    virtual void onSetCharacteristicNotification(INT32 connId, UINT32 handle, INT32 registered) = 0;

    /**
     * @brief Enable advertisement callback
     * @param[in] enable Characteristic notify callbac
     * @return void
     */
    virtual void onAdvEnable(INT32 status) = 0;
};
class AGGatt {
public:
    /**
     * @brief Init gatt client
     * @return 0 if success; otherwise return the error code
     */
    virtual INT32 init() = 0;
    /**
     * @brief Get ble on/off status
     * @return True if on; else if off
     */
    virtual BOOLEAN getBtOnOffStatus() = 0;
    /**
     * @brief Start Scan
     * @return return 0 if success; otherwise return the error code
     */
    virtual INT32 startScan() = 0;
    /**
     * @brief Stop scan
     * @return return 0 if success; otherwise return the error code
     */
    virtual INT32 stopScan() = 0;
    /**
     * @brief Deinit gatt client
     * @return return 0 if success; otherwise return the error code
     */
    virtual INT32 gattDeinit() = 0;

    /**
     * @brief Set Gatt Callback, see IAGGattCallback
     * @param[in] callback Gatt Callback
     * @return void
     */
    virtual void setCallback(IAGGattCallback* callback) = 0;

    /**
     * @brief Connect Gatt
     * @param[in] bdAddr Peer bluetooth device address
     * @return 0 if success; otherwise return the error code
     */
    virtual INT32 connect(const CHAR* bdAddr) = 0;

    /**
     * @brief Discover Services
     * @param[in] Connection id
     * @return 0 if success; otherwise return the error code
     */
    virtual INT32 discoverServices(INT32 connId) = 0;

    /**
     * @brief Read characteristic
     * @param[in] connId Connection id
     * @param[in] handle Attribute handle
     * @return 0 if success; otherwise return the error code
     */
    virtual INT32 readCharacteristic(INT32 connId, UINT32 handle) = 0;

    /**
     * @brief Write characteristic
     * @param[in] connId Connection id
     * @param[in] handle Attribute handle
     * @param[in] writeType 1-write no response;2-write with response;3-write prepare
     * @param[in] data Payload
     * @param[in] size Payload length in bytes
     * @return 0 if success; otherwise return the error code
     */
    virtual INT32 writeCharacteristic(INT32 connId, UINT32 handle, GattWriteType writeType, const UINT8* data, INT32 size) = 0;

    //enable属性变化通知
    /**
     * @brief Enable characteristic notify
     * @param[in] bdaddr Peer bluetooth device address
     * @param[in] handle Attribute handle
     * @param[in] enable Enable or not
     * @return 0 if success; otherwise return the error code
     */
    virtual INT32 setCharacteristicNotification(CHAR* bdaddr,UINT32 handle,BOOLEAN enable) = 0;

    /**
     * @brief Disconnect gatt
     * @param[in] connId Connection id
     * @param[in] bdaddr Peer bluetooth device address
     * @return 0 if success; otherwise return the error code
     */
    virtual INT32 disconnect(INT32 connId, const CHAR* bdAddr) = 0;

    /**
     * @brief Enable ble advertisement
     * @param[in] enable True if enable;else disabe
     * @return 0 if success; otherwise return the error code
     */
    virtual INT32 enableAdv( BOOLEAN enable) = 0;

    /**
     * @brief Set the advertisement data
     * @return 0 if success; otherwise return the error code
     */
    virtual  INT32 setAdvData(CHAR* data) = 0;

    /**
     * @brief Get wifi mac
     * @param[in] wifi_mac Wifi address
     * @param[in] size Length in bytes
     * @return 0 if success; otherwise return the error code
     */
    virtual INT32 getWifiMac(CHAR* wifi_mac, INT32 size) = 0;

    /**
     * @brief Destructor of AGGatt
     */
    virtual ~AGGatt(){}
};

#ifdef __ANDROID__
#endif

#ifdef __cplusplus
}
#endif
#endif //AG_GATT_H
