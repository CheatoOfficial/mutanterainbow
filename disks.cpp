#include <ntifs.h>
#include <ntstrsafe.h>
#include "utils.h"
#include "shared.h"
#include "log.h"
#include "disks.h"

NTSTATUS Disks::ChangeDiskDispatch()
{
	auto* base = Utils::GetModuleBase("CLASSPNP.SYS");
	if (!base)
		return STATUS_UNSUCCESSFUL;

	/*
	 * ClassMpdevInternalDeviceControl
	 * It will instantly return with unsuccessful status
	 */
	UINT64 scan = (UINT64)(Utils::FindPatternImage(base, ("\x40\x53\x48\x83\xEC\x20\x48\x8B\x41\x40\x48\x8B\xDA\x4C\x8B\xC1\x80\xB8\xCC\xCC\xCC\xCC\xCC\x74\x57"), "xxxxxxxxxxxxxxxxxx?????xx")); // 40 53 48 83 EC 20 48 8B 41 40 48 8B DA 4C 8B C1 80 B8 ? ? ? ? ? 74 57 unchanged 10 20H2 -> 11 22H2 22621.1344 -> 22621.1848, alt sig 40 53 48 83 EC 20 48 8B 41 40 48 8B DA 
	if (!scan)
		return STATUS_UNSUCCESSFUL;

	UNICODE_STRING targetName;
	RtlInitUnicodeString(&targetName, (L"\\Driver\\Disk"));

	PDRIVER_OBJECT driverObject;
	auto status = ObReferenceObjectByName(&targetName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, nullptr, 0, *IoDriverObjectType, KernelMode, nullptr, reinterpret_cast<VOID**>(&driverObject));
	if (status != 0)
		return STATUS_UNSUCCESSFUL;

	*reinterpret_cast<UINT64*>(&driverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL]) = (UINT64)scan;

	ObfDereferenceObject(driverObject);
	return STATUS_SUCCESS;
}