#include <ntifs.h>
#include "log.h"
#include "utils.h"
#include "shared.h"
#include "smbios.h"

/**
 * \brief Get's the string from SMBIOS table
 * \param header Table header
 * \param string String itself
 * \return Pointer to the null terminated string
 */
char* Smbios::GetString(SMBIOS_HEADER* header, SMBIOS_STRING string)
{
	const auto* start = reinterpret_cast<const char*>(header) + header->Length;

	if (!string || *start == 0)
		return nullptr;

	while (--string)
	{
		start += strlen(start) + 1;
	}

	return const_cast<char*>(start);
}

/**
 * \brief Replace string at a given location by randomized string with same length
 * \param string Pointer to string (has to be null terminated)
 */
void Smbios::RandomizeString(char* string)
{
	const auto length = static_cast<int>(strlen(string));

	auto* buffer = static_cast<char*>(ExAllocatePoolWithTag(NonPagedPool, length, POOL_TAG));
	Utils::RandomText(buffer, length);
	buffer[length] = '\0';

	memcpy(string, buffer, length);

	ExFreePool(buffer);
}

/**
 * \brief Modify information in the table of given header
 * \param header Table header (only 0-3 implemented)
 * \return 
 */
NTSTATUS Smbios::ProcessTable(SMBIOS_HEADER* header)
{
	if (!header->Length)
		return STATUS_UNSUCCESSFUL;

	/*if (header->Type == 0)
	{
		auto* type0 = reinterpret_cast<SMBIOS_TYPE0*>(header);

		auto* vendor = GetString(header, type0->Vendor);
		RandomizeString(vendor);
	}

	if (header->Type == 1)
	{
		auto* type1 = reinterpret_cast<SMBIOS_TYPE1*>(header);

		auto* manufacturer = GetString(header, type1->Manufacturer);
		RandomizeString(manufacturer);

		auto* productName = GetString(header, type1->ProductName);
		RandomizeString(productName);

		auto* serialNumber = GetString(header, type1->SerialNumber);
		RandomizeString(serialNumber);
	}

	if (header->Type == 2)
	{
		auto* type2 = reinterpret_cast<SMBIOS_TYPE2*>(header);

		auto* manufacturer = GetString(header, type2->Manufacturer);
		RandomizeString(manufacturer);

		auto* productName = GetString(header, type2->ProductName);
		RandomizeString(productName);

		auto* serialNumber = GetString(header, type2->SerialNumber);
		RandomizeString(serialNumber);
	}

	if (header->Type == 3)
	{
		auto* type3 = reinterpret_cast<SMBIOS_TYPE3*>(header);

		auto* manufacturer = GetString(header, type3->Manufacturer);
		RandomizeString(manufacturer);

		auto* serialNumber = GetString(header, type3->SerialNumber);
		RandomizeString(serialNumber);
	}*/

	if (header->Type == 0) // bios information
	{
		//Log::Print("Processing SMBIOS header table 0.\n");
		auto* type0 = reinterpret_cast<SMBIOS_TYPE0*>(header);

		auto* vendor = GetString(header, type0->Vendor);
		strcpy(vendor, ("Unknown"));

		auto* ver = GetString(header, type0->BiosVersion);
		strcpy(ver, ("1.0"));

		auto* brd = GetString(header, type0->BiosReleaseDate);
		strcpy(brd, ("1/1/1970"));
		//Log::Print("Processed SMBIOS header table 0.\n");
	}

	if (header->Type == 1) // system information
	{
		//Log::Print("Processing SMBIOS header table 1.\n");
		auto* type1 = reinterpret_cast<SMBIOS_TYPE1*>(header);

		auto* manufacturer = GetString(header, type1->Manufacturer);
		strcpy(manufacturer, ("Unknown"));

		auto* productName = GetString(header, type1->ProductName);
		strcpy(productName, ("Unknown"));

		auto* ver = GetString(header, type1->Version);
		strcpy(ver, ("1.0"));

		auto* serialNumber = GetString(header, type1->SerialNumber);
		strcpy(serialNumber, ("To Be Filled By O.E.M."));

		auto* SKUNumber = GetString(header, type1->SKUNumber);
		strcpy(SKUNumber, ("To Be Filled By O.E.M."));

		auto* Family = GetString(header, type1->Family);
		strcpy(Family, ("Default string"));
		//Log::Print("Processed SMBIOS header table 1.\n");
	}

	if (header->Type == 2) // baseboard information
	{
		//Log::Print("Processing SMBIOS header table 2.\n");
		auto* type2 = reinterpret_cast<SMBIOS_TYPE2*>(header);

		auto* manufacturer = GetString(header, type2->Manufacturer);
		strcpy(manufacturer, ("Unknown"));

		auto* productName = GetString(header, type2->ProductName);
		strcpy(productName, ("Unknown"));

		auto* ver = GetString(header, type2->Version);
		strcpy(ver, ("1.0"));

		auto* serialNumber = GetString(header, type2->SerialNumber);
		strcpy(serialNumber, ("To Be Filled By O.E.M."));

		auto* AssetTag = GetString(header, type2->AssetTag);
		strcpy(AssetTag, ("To Be Filled By O.E.M."));

		auto* LocationInChassis = GetString(header, type2->LocationInChassis);
		strcpy(LocationInChassis, ("Default string"));
		//Log::Print("Processed SMBIOS header table 2.\n");
	}

	if (header->Type == 3) // system enclosure or chassis information
	{
		//Log::Print("Processing SMBIOS header table 3.\n");
		auto* type3 = reinterpret_cast<SMBIOS_TYPE3*>(header);

		auto* manufacturer = GetString(header, type3->Manufacturer);
		strcpy(manufacturer, ("Unknown"));

		auto* ver = GetString(header, type3->Version);
		strcpy(ver, ("1.0"));

		auto* serialNumber = GetString(header, type3->SerialNumber);
		strcpy(serialNumber, ("To Be Filled By O.E.M."));

		auto* at = GetString(header, type3->AssetTag);
		strcpy(at, ("To Be Filled By O.E.M."));
		//Log::Print("Processed SMBIOS header table 3.\n");
	}

	if (header->Type == 4) // processor information
	{
		//Log::Print("Processing SMBIOS header table 4.\n");
		auto* type4 = reinterpret_cast<SMBIOS_TYPE4*>(header);

		//auto* Socket = GetString(header, type4->Socket);
		//strcpy(Socket, "Default string");

		//auto* manufacturer = GetString(header, type4->ProcessorManufacture);
		//strcpy(manufacturer, ("Unknown"));

		auto* ver = GetString(header, type4->ProcessorVersion);
		strcpy(ver, ("1.0"));

		auto* SerialNumber = GetString(header, type4->SerialNumber);
		strcpy(SerialNumber, ("To Be Filled By O.E.M."));

		auto* AssetTag = GetString(header, type4->AssetTag);
		strcpy(AssetTag, ("To Be Filled By O.E.M."));

		auto* PartNumber = GetString(header, type4->PartNumber);
		strcpy(PartNumber, ("Default string"));
		//Log::Print("Processed SMBIOS header table 4.\n");
	}

	/*

	typedef struct
	{
		SMBIOS_STRING                        DeviceLocator;
		SMBIOS_STRING                        BankLocator;
		SMBIOS_STRING                        Manufacturer;
		SMBIOS_STRING                        SerialNumber;
		SMBIOS_STRING                        AssetTag;
		SMBIOS_STRING                        PartNumber;
		SMBIOS_STRING                        FirmwareVersion;
	} SMBIOS_TYPE17;
		*/
	if (header->Type == 17) // memory device information
	{
		//Log::Print("Processing SMBIOS header table 17.\n");
		auto* type17 = reinterpret_cast<SMBIOS_TYPE17*>(header);

		//auto* DeviceLocator = GetString(header, type17->DeviceLocator);
		//strcpy(DeviceLocator, "Default string");

		//auto* BankLocator = GetString(header, type17->BankLocator);
		//strcpy(BankLocator, "Default string");

		auto* Manufacturer = GetString(header, type17->Manufacturer);
		strcpy(Manufacturer, ("Unknown"));

		auto* SerialNumber = GetString(header, type17->SerialNumber);
		strcpy(SerialNumber, ("00000000"));

		auto* AssetTag = GetString(header, type17->AssetTag);
		strcpy(AssetTag, ("To Be Filled By O.E.M."));

		// Will crash PC
		//auto* PartNumber = GetString(header, type17->PartNumber);
		//strcpy(PartNumber, "Default string");

		// Will crash PC
		//auto* FirmwareVersion = GetString(header, type17->FirmwareVersion);
		//strcpy(FirmwareVersion, "1.0");


		//Log::Print("Processed SMBIOS header table 17.\n");
	}

	
	return STATUS_SUCCESS;
}

/**
 * \brief Loop through SMBIOS tables with provided first table header
 * \param mapped Header of the first table
 * \param size Size of all tables including strings
 * \return 
 */
NTSTATUS Smbios::LoopTables(void* mapped, ULONG size)
{
	auto* endAddress = static_cast<char*>(mapped) + size;
	while (true)
	{
		auto* header = static_cast<SMBIOS_HEADER*>(mapped);
		if (header->Type == 127 && header->Length == 4)
			break;
		
		ProcessTable(header);
		auto* end = static_cast<char*>(mapped) + header->Length;
		while (0 != (*end | *(end + 1))) end++;
		end += 2;
		if (end >= endAddress)
			break;	

		mapped = end;
	}
	
	return STATUS_SUCCESS;
}

/**
 * \brief Find SMBIOS physical address, map it and then loop through
 * table 0-3 and modify possible identifiable information
 * \return Status of the change (will return STATUS_SUCCESS if mapping was successful)
 */
NTSTATUS Smbios::ChangeSmbiosSerials()
{
	auto* base = Utils::GetModuleBase("ntoskrnl.exe");
	if (!base)
	{
		Log::Print("Failed to find ntoskrnl.sys base!\n");
		return STATUS_UNSUCCESSFUL;
	}

	auto* physicalAddress = static_cast<PPHYSICAL_ADDRESS>(Utils::FindPatternImage(base, "\x48\x8B\x0D\x00\x00\x00\x00\x48\x85\xC9\x74\x00\x8B\x15", "xxx????xxxx?xx")); // WmipFindSMBiosStructure -> WmipSMBiosTablePhysicalAddress
	if (!physicalAddress)
	{
		Log::Print("Failed to find SMBIOS physical address!\n");
		return STATUS_UNSUCCESSFUL;
	}

	physicalAddress = reinterpret_cast<PPHYSICAL_ADDRESS>(reinterpret_cast<char*>(physicalAddress) + 7 + *reinterpret_cast<int*>(reinterpret_cast<char*>(physicalAddress) + 3));
	if (!physicalAddress)
	{
		Log::Print("Physical address is null!\n");
		return STATUS_UNSUCCESSFUL;
	}

	auto* sizeScan = Utils::FindPatternImage(base, "\x8B\x1D\x00\x00\x00\x00\x48\x8B\xD0\x44\x8B\xC3\x48\x8B\xCD\xE8\x00\x00\x00\x00\x8B\xD3\x48\x8B", "xx????xxxxxxxxxx????xxxx");  // WmipFindSMBiosStructure -> WmipSMBiosTableLength
	if (!sizeScan)
	{
		Log::Print("Failed to find SMBIOS size!\n");
		return STATUS_UNSUCCESSFUL;
	}

	const auto size = *reinterpret_cast<ULONG*>(static_cast<char*>(sizeScan) + 6 + *reinterpret_cast<int*>(static_cast<char*>(sizeScan) + 2));
	if (!size)
	{
		Log::Print("SMBIOS size is null!\n");
		return STATUS_UNSUCCESSFUL;
	}

	auto* mapped = MmMapIoSpace((PHYSICAL_ADDRESS)*physicalAddress, size, MmNonCached);
	if (!mapped)
	{
		Log::Print("Failed to map SMBIOS structures!\n");
		return STATUS_UNSUCCESSFUL;
	}
	
	LoopTables(reinterpret_cast<void*>(mapped), size);
	
	MmUnmapIoSpace(reinterpret_cast<void*>(mapped), size);
	
	return STATUS_SUCCESS;
}