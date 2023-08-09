#include <ntifs.h>
#include <windef.h>
#include <ntimage.h>
#include "shared.h"
#include "utils.h"
#include <cstdint>

/**
 * \brief Get base address of kernel module
 * \param moduleName Name of the module (ex. storport.sys)
 * \return Address of the module or null pointer if failed
 */
PVOID Utils::GetModuleBase(const char* moduleName)
{
	PVOID address = nullptr;
	ULONG size = 0;
	
	auto status = ZwQuerySystemInformation(SystemModuleInformation, &size, 0, &size);
	if (status != STATUS_INFO_LENGTH_MISMATCH)
		return nullptr;

	auto* moduleList = static_cast<PSYSTEM_MODULE_INFORMATION>(ExAllocatePoolWithTag(NonPagedPool, size, POOL_TAG));
	if (!moduleList)
		return nullptr;

	status = ZwQuerySystemInformation(SystemModuleInformation, moduleList, size, nullptr);
	if (!NT_SUCCESS(status))
		goto end;

	for (auto i = 0; i < moduleList->ulModuleCount; i++)
	{
		auto module = moduleList->Modules[i];
		if (strstr(module.ImageName, moduleName))
		{
			address = module.Base;
			break;
		}
	}
	
end:
	ExFreePool(moduleList);
	return address;
}

int CustomStrCmp(const char* str1, const char* str2)
{
	while (*str1 && (*str1 == *str2))
	{
		str1++;
		str2++;
	}

	return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

UINT64 Utils::GetExport(void* base, const char* functionName)
{
	PIMAGE_DOS_HEADER dosHeader = static_cast<PIMAGE_DOS_HEADER>(base);
	if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE)
		return 0;

	PIMAGE_NT_HEADERS64 ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS64>(reinterpret_cast<UINT64>(base) + dosHeader->e_lfanew);

	UINT32 exportsRva = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
	if (!exportsRva)
		return 0;

	PIMAGE_EXPORT_DIRECTORY exports = reinterpret_cast<PIMAGE_EXPORT_DIRECTORY>(reinterpret_cast<UINT64>(base) + exportsRva);
	UINT32* nameRva = reinterpret_cast<UINT32*>(reinterpret_cast<UINT64>(base) + exports->AddressOfNames);

	for (UINT32 i = 0; i < exports->NumberOfNames; ++i)
	{
		char* func = reinterpret_cast<char*>(reinterpret_cast<UINT64>(base) + nameRva[i]);
		if (CustomStrCmp(func, functionName) == 0)
		{
			UINT32* funcRva = reinterpret_cast<UINT32*>(reinterpret_cast<UINT64>(base) + exports->AddressOfFunctions);
			UINT16* ordinalRva = reinterpret_cast<UINT16*>(reinterpret_cast<UINT64>(base) + exports->AddressOfNameOrdinals);

			return reinterpret_cast<UINT64>(base) + funcRva[ordinalRva[i]];
		}
	}

	return 0;
}

/**
 * \brief Checks if buffer at the location of base parameter
 * matches pattern and mask
 * \param base Address to check
 * \param pattern Byte pattern to match
 * \param mask Mask containing unknown bytes
 * \return 
 */
bool Utils::CheckMask(const char* base, const char* pattern, const char* mask)
{
	for (; *mask; ++base, ++pattern, ++mask) 
	{
		if ('x' == *mask && *base != *pattern) 
		{
			return false;
		}
	}

	return true;
}

/**
 * \brief Find byte pattern in given buffer
 * \param base Address to start searching in
 * \param length Maximum length
 * \param pattern Byte pattern to match
 * \param mask Mask containing unknown bytes
 * \return Pointer to matching memory
 */
PVOID Utils::FindPattern(PVOID base, int length, const char* pattern, const char* mask)
{
	length -= static_cast<int>(strlen(mask));
	for (auto i = 0; i <= length; ++i) 
	{
		const auto* data = static_cast<char*>(base);
		const auto* address = &data[i];
		if (CheckMask(address, pattern, mask))
			return PVOID(address);
	}

	return nullptr;
}

/**
 * \brief Find byte pattern in given module/image ".text" and "PAGE" sections
 * \param base Base address of the kernel module
 * \param pattern Byte pattern to match
 * \param mask Mask containing unknown bytes
 * \return Pointer to matching memory
 */
PVOID Utils::FindPatternImage(PVOID base, const char* pattern, const char* mask)
{
	PVOID match = nullptr;

	auto* headers = reinterpret_cast<PIMAGE_NT_HEADERS>(static_cast<char*>(base) + static_cast<PIMAGE_DOS_HEADER>(base)->e_lfanew);
	auto* sections = IMAGE_FIRST_SECTION(headers);
	
	for (auto i = 0; i < headers->FileHeader.NumberOfSections; ++i) 
	{
		auto* section = &sections[i];
		if ('EGAP' == *reinterpret_cast<PINT>(section->Name) || memcmp(section->Name, ".text", 5) == 0) 
		{
			match = FindPattern(static_cast<char*>(base) + section->VirtualAddress, section->Misc.VirtualSize, pattern, mask);
			if (match) 
				break;
		}
	}

	return match;
}

/**
 * \brief Generate pseudo-random text into given buffer
 * \param text Pointer to text
 * \param length Desired length
 */
void Utils::RandomText(char* text, const int length)
{
	if (!text)
		return;

	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	auto seed = KeQueryTimeIncrement();

	for (auto n = 0; n <= length; n++)
	{
		auto key = RtlRandomEx(&seed) % static_cast<int>(sizeof(alphanum) - 1);
		text[n] = alphanum[key];
	}
}