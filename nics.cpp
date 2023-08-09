#include <ntifs.h>
#include <ntstrsafe.h>
#include "utils.h"
#include "shared.h"
#include "log.h"
#include "nics.h"

NTSTATUS NICs::ChangeNetworkDispatch()
{
	auto* base = Utils::GetModuleBase("ndis.sys");
	if (!base)
		return STATUS_UNSUCCESSFUL;

	//ndisDummyIrpHandler
	UINT64 scan = (UINT64)(Utils::FindPatternImage(base, ("\x48\x8D\x05\xCC\xCC\xCC\xCC\xB9\xCC\xCC\xCC\xCC\x49\x8D\x7E\x70"), "xxx????x????xxxx")); // 48 8D 05 ? ? ? ? B9 ? ? ? ? 49 8D 7E 70 unchanged
	if (!scan)
		return STATUS_UNSUCCESSFUL;
	scan = reinterpret_cast<UINT64>(RELATIVE_ADDRESS((UINT8*)scan, 7));

	UINT64 listScan = (UINT64)(Utils::FindPatternImage(base, ("\x48\x8B\x35\xCC\xCC\xCC\xCC\x44\x0F\xB6\xF0"), "xxx????xxxx")); // 48 8B 35 ? ? ? ? 44 0F B6 E0 -> 48 8B 35 ? ? ? ? 44 0F B6 F0
	// mov     rsi, cs:?ndisMiniDriverList@@3PEAU_NDIS_M_DRIVER_BLOCK@@EA ; _NDIS_M_DRIVER_BLOCK * ndisMiniDriverList
	if (!listScan)
		return STATUS_UNSUCCESSFUL;

	bool found = false;
	PNDIS_M_DRIVER_BLOCK block = *reinterpret_cast<PNDIS_M_DRIVER_BLOCK*>(RELATIVE_ADDRESS(listScan, 7));
	for (PNDIS_M_DRIVER_BLOCK currentDriver = block; currentDriver; currentDriver = reinterpret_cast<PNDIS_M_DRIVER_BLOCK>(currentDriver->NextDriver))
	{
		if (!currentDriver->DriverObject)
			continue;

		if (!currentDriver->DriverObject->MajorFunction)
			continue;

		*reinterpret_cast<UINT64*>(&currentDriver->DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL]) = scan;
		found = true;
	}

	return found ? STATUS_SUCCESS : STATUS_UNSUCCESSFUL;
}