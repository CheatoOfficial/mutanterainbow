/*
 * Mutante (base for this driver)
 * Made by Samuel Tulach
 * https://github.com/SamuelTulach/mutante
 * 
 * Rainbow (spoofing pasted from, was actually derived from mutante yeah what a surprise rofl)
 * Also made by Samuel Tulach
 * https://github.com/SamuelTulach/rainbow
 * 
 * NVIDIA UUID spoofing
 * Made by Aryan Kumar also known as Xyrem/Rafael4096/furiosdestruct/ZeraX
 * https://gist.github.com/Xyrem/4b6ea950e26565581ccd851be09ace00
 * 
 * Combined into an absolute meme called mutanterainbow by Ausbloke from chea.to
 * Was a fun project to paste together, don't expect to not be kicked from the Battle Bus on the first match of Fortnite after spoofing
 * Or funnier, the Spawn Island :) yeah that happened
 */

#include <ntifs.h>
#include "log.h"
#include "shared.h"

#include "disks.h"
#include "nics.h"
#include "smbios.h"
#include <stdio.h>
#include "gpu.hpp"

void printGUID(const GUID& guid) {
	Log::Print(
		"GUID: %08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X\n",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]
	);
}

// If it works, it works. Thank me later - Ausbloke
NTSTATUS convertStringToGUID(const char* strGUID, GUID& newUUID) {
	NTSTATUS status = STATUS_SUCCESS;

	// Fields for the GUID
	unsigned long p0;
	unsigned int p1, p2;
	unsigned int p3, p4, p5, p6, p7, p8, p9, p10;

	// Parse the string into the GUID fields
	int fieldsAssigned = sscanf_s(strGUID, "%8lx-%4hx-%4hx-%2hx%2hx-%2hx%2hx%2hx%2hx%2hx%2hx",
		&p0, &p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10);

	if (fieldsAssigned == 11) {
		newUUID.Data1 = _byteswap_ulong(p0);
		newUUID.Data2 = _byteswap_ushort((USHORT)p1);
		newUUID.Data3 = _byteswap_ushort((USHORT)p2);
		newUUID.Data4[0] = (UCHAR)p3;
		newUUID.Data4[1] = (UCHAR)p4;
		newUUID.Data4[2] = (UCHAR)p5;
		newUUID.Data4[3] = (UCHAR)p6;
		newUUID.Data4[4] = (UCHAR)p7;
		newUUID.Data4[5] = (UCHAR)p8;
		newUUID.Data4[6] = (UCHAR)p9;
		newUUID.Data4[7] = (UCHAR)p10;
	}
	else {
		status = STATUS_INVALID_PARAMETER;
	}

	return status;
}

/**
 * \brief Driver's main entry point
 * \param object Pointer to driver object (invalid when manual mapping)
 * \param registry Registry path (invalid when manual mapping)
 * \return Status of the driver execution
 */
NTSTATUS DriverEntry(PDRIVER_OBJECT object, PUNICODE_STRING registry)
{
	UNREFERENCED_PARAMETER(object);
	UNREFERENCED_PARAMETER(registry);

	Log::Print("Driver loaded. Build on %s.", __DATE__);

	Disks::ChangeDiskDispatch();
	NICs::ChangeNetworkDispatch();
	Smbios::ChangeSmbiosSerials(); // aint working? Nigger

	// gpu works
	//const char* strGUID = "ffffffff-eeee-dddd-cccc-1234567890ab";
	const char* strGUID = "00000000-0000-0000-0000-000000000000";
	GUID newUUID;
	convertStringToGUID(strGUID, newUUID);

	uintptr_t result = n_gpu::handleNVIDIAUUID(newUUID);
	if (result != 0) {
		// Success
		Log::Print("UUID spoofing completed successfully.");
	}
	else {
		// Error
		Log::Print("UUID spoofing failed.");
	}

	return STATUS_SUCCESS;
}