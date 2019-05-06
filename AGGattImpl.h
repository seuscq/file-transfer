#ifndef AG_GATT_IMPL_H
#define AG_GATT_IMPL_H

#include "AGGatt.h"

using namespace std;
class AGGattImpl: public AGGatt
{
public:
    AGGattImpl();
    ~AGGattImpl();
    INT32 init();
    BOOLEAN getBtOnOffStatus();
    INT32 startScan();
    INT32 stopScan();
    INT32 gattDeinit();

    void setCallback(IAGGattCallback* callback);

    INT32 connect(const CHAR* bdAddr);

    INT32 discoverServices(INT32 connId);

    INT32 readCharacteristic(INT32 connId, UINT32 handle);

    INT32 writeCharacteristic(INT32 connId, UINT32 handle, GattWriteType writeType, const UINT8* data, INT32 size);

    //enable属性变化通知
    INT32 setCharacteristicNotification(CHAR* bdaddr,UINT32 handle,BOOLEAN enable);

    INT32 disconnect(INT32 connId, const CHAR* bdAddr);

    INT32 enableAdv( BOOLEAN enable);

    INT32 setAdvData(CHAR* data);

    INT32 getWifiMac(CHAR* wifi_mac,INT32 size);

    IAGGattCallback* getCallback();
private:
    IAGGattCallback* mCallback;
};
#endif//AG_GATT_IMPL_H
