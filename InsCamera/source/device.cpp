#include "..\include\device.h"

DEVICE* DEVICE::device = nullptr;

DEVICE* DEVICE::get_instance() 
{
	if (device == nullptr)
	{
		device = new DEVICE;
	}
	return device;
}

LOCALDEVICE* LOCALDEVICE::localdevice = nullptr;

void LOCALDEVICE::init(const char* mip, const char* mnet_mask , const char* mgateway)
{
	strcpy(ip,mip); 
	strcpy(net_mask, mnet_mask);
	strcpy(gateway, mnet_mask);
}

LOCALDEVICE* LOCALDEVICE::get_instance()
{
	if (localdevice == nullptr)
	{
		localdevice = new LOCALDEVICE;
	}
	return localdevice;
}