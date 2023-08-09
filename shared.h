#pragma once

typedef UINT32(__stdcall* DbgPrintEx_t)(UINT32 ComponentId, UINT32 Level, const unsigned char* Format, ...);
typedef void(__stdcall* RtlInitUnicodeString_t)(PUNICODE_STRING DestinationString, const wchar_t* SourceString);
typedef UINT32(__stdcall* ObReferenceObjectByName_t)(PUNICODE_STRING ObjectName, UINT32 Attributes, VOID* AccessState, UINT32 DesiredAccess, VOID* ObjectType, unsigned char AccessMode, VOID* ParseContext, VOID** Object);
typedef INT64(__stdcall* ObfDereferenceObject_t)(VOID* Object);

#define POOL_TAG 'oamL'

/*typedef struct _UNICODE_STRING
{
	UINT16 Length;
	UINT16 MaximumLength;
	wchar_t* Buffer;
} UNICODE_STRING, * PUNICODE_STRING;*/

typedef struct _KLDR_DATA_TABLE_ENTRY
{
	struct _LIST_ENTRY InLoadOrderLinks;
	VOID* ExceptionTable;
	UINT32 ExceptionTableSize;
	VOID* GpValue;
	struct _NON_PAGED_DEBUG_INFO* NonPagedDebugInfo;
	VOID* DllBase;
	VOID* EntryPoint;
	UINT32 SizeOfImage;
	struct _UNICODE_STRING FullDllName;
	struct _UNICODE_STRING BaseDllName;
	UINT32 Flags;
	UINT16 LoadCount;
	union
	{
		UINT16 SignatureLevel : 4;
		UINT16 SignatureType : 3;
		UINT16 Unused : 9;
		UINT16 EntireField;
	} u1;
	VOID* SectionPointer;
	UINT32 CheckSum;
	UINT32 CoverageSectionSize;
	VOID* CoverageSection;
	VOID* LoadedImports;
	VOID* Spare;
	UINT32 SizeOfImageNotRounded;
	UINT32 TimeDateStamp;
} KLDR_DATA_TABLE_ENTRY, * PKLDR_DATA_TABLE_ENTRY;

typedef struct _LOADER_PARAMETER_BLOCK
{
	UINT32 OsMajorVersion;
	UINT32 OsMinorVersion;
	UINT32 Size;
	UINT32 OsLoaderSecurityVersion;
	struct _LIST_ENTRY LoadOrderListHead;
	struct _LIST_ENTRY MemoryDescriptorListHead;
	struct _LIST_ENTRY BootDriverListHead;
	struct _LIST_ENTRY EarlyLaunchListHead;
	struct _LIST_ENTRY CoreDriverListHead;
	struct _LIST_ENTRY CoreExtensionsDriverListHead;
	struct _LIST_ENTRY TpmCoreDriverListHead;
} LOADER_PARAMETER_BLOCK, * PLOADER_PARAMETER_BLOCK;

/*#define CONTAINING_RECORD(address, type, field) ((type *)( \
                                                  (char*)(address) - \
                                                  (UINT64)(&((type *)0)->field)))*/

/*typedef struct _IMAGE_FILE_HEADER
{
	UINT16 Machine;
	UINT16 NumberOfSections;
	UINT32 TimeDateStamp;
	UINT32 PointerToSymbolTable;
	UINT32 NumberOfSymbols;
	UINT16 SizeOfOptionalHeader;
	UINT16 Characteristics;
} IMAGE_FILE_HEADER, * PIMAGE_FILE_HEADER;*/

/*typedef struct _IMAGE_DATA_DIRECTORY
{
	UINT32 VirtualAddress;
	UINT32 Size;
} IMAGE_DATA_DIRECTORY, * PIMAGE_DATA_DIRECTORY;*/

#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES 16

/*typedef struct _IMAGE_OPTIONAL_HEADER64
{
	UINT16 Magic;
	UINT8 MajorLinkerVersion;
	UINT8 MinorLinkerVersion;
	UINT32 SizeOfCode;
	UINT32 SizeOfInitializedData;
	UINT32 SizeOfUninitializedData;
	UINT32 AddressOfEntryPoint;
	UINT32 BaseOfCode;
	UINT64 ImageBase;
	UINT32 SectionAlignment;
	UINT32 FileAlignment;
	UINT16 MajorOperatingSystemVersion;
	UINT16 MinorOperatingSystemVersion;
	UINT16 MajorImageVersion;
	UINT16 MinorImageVersion;
	UINT16 MajorSubsystemVersion;
	UINT16 MinorSubsystemVersion;
	UINT32 Win32VersionValue;
	UINT32 SizeOfImage;
	UINT32 SizeOfHeaders;
	UINT32 CheckSum;
	UINT16 Subsystem;
	UINT16 DllCharacteristics;
	UINT64 SizeOfStackReserve;
	UINT64 SizeOfStackCommit;
	UINT64 SizeOfHeapReserve;
	UINT64 SizeOfHeapCommit;
	UINT32 LoaderFlags;
	UINT32 NumberOfRvaAndSizes;
	IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER64, * PIMAGE_OPTIONAL_HEADER64;*/

/*typedef struct _IMAGE_NT_HEADERS64
{
	UINT32 Signature;
	IMAGE_FILE_HEADER FileHeader;
	IMAGE_OPTIONAL_HEADER64 OptionalHeader;
} IMAGE_NT_HEADERS64, * PIMAGE_NT_HEADERS64, IMAGE_NT_HEADERS, * PIMAGE_NT_HEADERS;*/

/*typedef struct _IMAGE_DOS_HEADER
{
	UINT16 e_magic;                     // Magic number
	UINT16 e_cblp;                      // Bytes on last page of file
	UINT16 e_cp;                        // Pages in file
	UINT16 e_crlc;                      // Relocations
	UINT16 e_cparhdr;                   // Size of header in paragraphs
	UINT16 e_minalloc;                  // Minimum extra paragraphs needed
	UINT16 e_maxalloc;                  // Maximum extra paragraphs needed
	UINT16 e_ss;                        // Initial (relative) SS value
	UINT16 e_sp;                        // Initial SP value
	UINT16 e_csum;                      // Checksum
	UINT16 e_ip;                        // Initial IP value
	UINT16 e_cs;                        // Initial (relative) CS value
	UINT16 e_lfarlc;                    // File address of relocation table
	UINT16 e_ovno;                      // Overlay number
	UINT16 e_res[4];                    // Reserved words
	UINT16 e_oemid;                     // OEM identifier (for e_oeminfo)
	UINT16 e_oeminfo;                   // OEM information; e_oemid specific
	UINT16 e_res2[10];                  // Reserved words
	INT32 e_lfanew;                     // File address of new exe header
} IMAGE_DOS_HEADER, * PIMAGE_DOS_HEADER;*/

#define IMAGE_SIZEOF_SHORT_NAME 8

/*typedef struct _IMAGE_SECTION_HEADER
{
	UINT8 Name[IMAGE_SIZEOF_SHORT_NAME];
	union
	{
		UINT32 PhysicalAddress;
		UINT32 VirtualSize;
	} Misc;
	UINT32 VirtualAddress;
	UINT32 SizeOfRawData;
	UINT32 PointerToRawData;
	UINT32 PointerToRelocations;
	UINT32 PointerToLinenumbers;
	UINT16 NumberOfRelocations;
	UINT16 NumberOfLinenumbers;
	UINT32 Characteristics;
} IMAGE_SECTION_HEADER, * PIMAGE_SECTION_HEADER;*/

//#define FIELD_OFFSET(type, field)    ((INT32)(INT64)&(((type *)0)->field))
//#define UFIELD_OFFSET(type, field)    ((UINT32)(INT64)&(((type *)0)->field))

/*#define IMAGE_FIRST_SECTION( ntheader ) ((PIMAGE_SECTION_HEADER)        \
    ((UINT64)(ntheader) +                                            \
     FIELD_OFFSET( IMAGE_NT_HEADERS, OptionalHeader ) +                 \
     ((ntheader))->FileHeader.SizeOfOptionalHeader   \
    ))*/

enum WinloadContext
{
	ApplicationContext,
	FirmwareContext
};

#define IMAGE_DOS_SIGNATURE 0x5A4D
#define IMAGE_NT_SIGNATURE 0x00004550

#define IMAGE_DIRECTORY_ENTRY_EXPORT 0
#define IMAGE_DIRECTORY_ENTRY_IMPORT 1

/*typedef struct _IMAGE_EXPORT_DIRECTORY
{
	UINT32 Characteristics;
	UINT32 TimeDateStamp;
	UINT16 MajorVersion;
	UINT16 MinorVersion;
	UINT32 Name;
	UINT32 Base;
	UINT32 NumberOfFunctions;
	UINT32 NumberOfNames;
	UINT32 AddressOfFunctions;
	UINT32 AddressOfNames;
	UINT32 AddressOfNameOrdinals;
} IMAGE_EXPORT_DIRECTORY, * PIMAGE_EXPORT_DIRECTORY;*/

/*typedef enum _MODE
{
	KernelMode,
	UserMode,
	MaximumMode
} MODE;*/

#define OBJ_INHERIT                         0x00000002L
#define OBJ_PERMANENT                       0x00000010L
#define OBJ_EXCLUSIVE                       0x00000020L
#define OBJ_CASE_INSENSITIVE                0x00000040L
#define OBJ_OPENIF                          0x00000080L
#define OBJ_OPENLINK                        0x00000100L
#define OBJ_KERNEL_HANDLE                   0x00000200L
#define OBJ_FORCE_ACCESS_CHECK              0x00000400L
#define OBJ_IGNORE_IMPERSONATED_DEVICEMAP   0x00000800L
#define OBJ_DONT_REPARSE                    0x00001000L
#define OBJ_VALID_ATTRIBUTES                0x00001FF2L

#define IRP_MJ_CREATE                   0x00
#define IRP_MJ_CREATE_NAMED_PIPE        0x01
#define IRP_MJ_CLOSE                    0x02
#define IRP_MJ_READ                     0x03
#define IRP_MJ_WRITE                    0x04
#define IRP_MJ_QUERY_INFORMATION        0x05
#define IRP_MJ_SET_INFORMATION          0x06
#define IRP_MJ_QUERY_EA                 0x07
#define IRP_MJ_SET_EA                   0x08
#define IRP_MJ_FLUSH_BUFFERS            0x09
#define IRP_MJ_QUERY_VOLUME_INFORMATION 0x0a
#define IRP_MJ_SET_VOLUME_INFORMATION   0x0b
#define IRP_MJ_DIRECTORY_CONTROL        0x0c
#define IRP_MJ_FILE_SYSTEM_CONTROL      0x0d
#define IRP_MJ_DEVICE_CONTROL           0x0e
#define IRP_MJ_INTERNAL_DEVICE_CONTROL  0x0f
#define IRP_MJ_SHUTDOWN                 0x10
#define IRP_MJ_LOCK_CONTROL             0x11
#define IRP_MJ_CLEANUP                  0x12
#define IRP_MJ_CREATE_MAILSLOT          0x13
#define IRP_MJ_QUERY_SECURITY           0x14
#define IRP_MJ_SET_SECURITY             0x15
#define IRP_MJ_POWER                    0x16
#define IRP_MJ_SYSTEM_CONTROL           0x17
#define IRP_MJ_DEVICE_CHANGE            0x18
#define IRP_MJ_QUERY_QUOTA              0x19
#define IRP_MJ_SET_QUOTA                0x1a
#define IRP_MJ_PNP                      0x1b
#define IRP_MJ_PNP_POWER                IRP_MJ_PNP
#define IRP_MJ_MAXIMUM_FUNCTION         0x1b

typedef struct _OBJECT_TYPE* POBJECT_TYPE;

/*typedef struct _DRIVER_OBJECT
{
	INT16 Type;
	INT16 Size;
	struct _DEVICE_OBJECT* DeviceObject;
	UINT32 Flags;
	VOID* DriverStart;
	UINT32 DriverSize;
	VOID* DriverSection;
	struct _DRIVER_EXTENSION* DriverExtension;
	struct _UNICODE_STRING DriverName;
	struct _UNICODE_STRING* HardwareDatabase;
	struct _FAST_IO_DISPATCH* FastIoDispatch;
	UINT32(*DriverInit)(struct _DRIVER_OBJECT* arg1, struct _UNICODE_STRING* arg2);
	VOID(*DriverStartIo)(struct _DEVICE_OBJECT* arg1, struct _IRP* arg2);
	VOID(*DriverUnload)(struct _DRIVER_OBJECT* arg1);
	UINT32(*MajorFunction[28])(struct _DEVICE_OBJECT* arg1, struct _IRP* arg2);
} DRIVER_OBJECT, * PDRIVER_OBJECT;*/

#define RELATIVE_ADDRESS(address, size) ((VOID*)((UINT8*)(address) + *(INT32*)((UINT8*)(address) + ((size) - (INT32)sizeof(INT32))) + (size)))

typedef struct _NDIS_M_DRIVER_BLOCK
{
	union
	{
		struct
		{
			VOID* Header;
			VOID* NextDriver;
		};
		struct
		{
			char Space[0x028]; // 0x028 DriverObject
			PDRIVER_OBJECT DriverObject;
		};
	};
} NDIS_M_DRIVER_BLOCK, * PNDIS_M_DRIVER_BLOCK;

/*typedef union _LARGE_INTEGER
{
	struct
	{
		UINT32 LowPart;
		INT32 HighPart;
	} DUMMYSTRUCTNAME;
	struct
	{
		UINT32 LowPart;
		INT32 HighPart;
	} u;
	INT64 QuadPart;
} LARGE_INTEGER;*/

// ------------------------------------------------
// ntoskrnl.exe
// ------------------------------------------------

typedef enum _SYSTEM_INFORMATION_CLASS
{
	SystemInformationClassMin = 0,
	SystemBasicInformation = 0,
	SystemProcessorInformation = 1,
	SystemPerformanceInformation = 2,
	SystemTimeOfDayInformation = 3,
	SystemPathInformation = 4,
	SystemNotImplemented1 = 4,
	SystemProcessInformation = 5,
	SystemProcessesAndThreadsInformation = 5,
	SystemCallCountInfoInformation = 6,
	SystemCallCounts = 6,
	SystemDeviceInformation = 7,
	SystemConfigurationInformation = 7,
	SystemProcessorPerformanceInformation = 8,
	SystemProcessorTimes = 8,
	SystemFlagsInformation = 9,
	SystemGlobalFlag = 9,
	SystemCallTimeInformation = 10,
	SystemNotImplemented2 = 10,
	SystemModuleInformation = 11,
	SystemLocksInformation = 12,
	SystemLockInformation = 12,
	SystemStackTraceInformation = 13,
	SystemNotImplemented3 = 13,
	SystemPagedPoolInformation = 14,
	SystemNotImplemented4 = 14,
	SystemNonPagedPoolInformation = 15,
	SystemNotImplemented5 = 15,
	SystemHandleInformation = 16,
	SystemObjectInformation = 17,
	SystemPageFileInformation = 18,
	SystemPagefileInformation = 18,
	SystemVdmInstemulInformation = 19,
	SystemInstructionEmulationCounts = 19,
	SystemVdmBopInformation = 20,
	SystemInvalidInfoClass1 = 20,
	SystemFileCacheInformation = 21,
	SystemCacheInformation = 21,
	SystemPoolTagInformation = 22,
	SystemInterruptInformation = 23,
	SystemProcessorStatistics = 23,
	SystemDpcBehaviourInformation = 24,
	SystemDpcInformation = 24,
	SystemFullMemoryInformation = 25,
	SystemNotImplemented6 = 25,
	SystemLoadImage = 26,
	SystemUnloadImage = 27,
	SystemTimeAdjustmentInformation = 28,
	SystemTimeAdjustment = 28,
	SystemSummaryMemoryInformation = 29,
	SystemNotImplemented7 = 29,
	SystemNextEventIdInformation = 30,
	SystemNotImplemented8 = 30,
	SystemEventIdsInformation = 31,
	SystemNotImplemented9 = 31,
	SystemCrashDumpInformation = 32,
	SystemExceptionInformation = 33,
	SystemCrashDumpStateInformation = 34,
	SystemKernelDebuggerInformation = 35,
	SystemContextSwitchInformation = 36,
	SystemRegistryQuotaInformation = 37,
	SystemLoadAndCallImage = 38,
	SystemPrioritySeparation = 39,
	SystemPlugPlayBusInformation = 40,
	SystemNotImplemented10 = 40,
	SystemDockInformation = 41,
	SystemNotImplemented11 = 41,
	SystemInvalidInfoClass2 = 42,
	SystemProcessorSpeedInformation = 43,
	SystemInvalidInfoClass3 = 43,
	SystemCurrentTimeZoneInformation = 44,
	SystemTimeZoneInformation = 44,
	SystemLookasideInformation = 45,
	SystemSetTimeSlipEvent = 46,
	SystemCreateSession = 47,
	SystemDeleteSession = 48,
	SystemInvalidInfoClass4 = 49,
	SystemRangeStartInformation = 50,
	SystemVerifierInformation = 51,
	SystemAddVerifier = 52,
	SystemSessionProcessesInformation = 53,
	SystemInformationClassMax
} SYSTEM_INFORMATION_CLASS;

typedef struct _SYSTEM_MODULE
{
	ULONG_PTR Reserved[2];
	PVOID Base;
	ULONG Size;
	ULONG Flags;
	USHORT Index;
	USHORT Unknown;
	USHORT LoadCount;
	USHORT ModuleNameOffset;
	CHAR ImageName[256];
} SYSTEM_MODULE, * PSYSTEM_MODULE;

typedef struct _SYSTEM_MODULE_INFORMATION
{
	ULONG_PTR ulModuleCount;
	SYSTEM_MODULE Modules[1];
} SYSTEM_MODULE_INFORMATION, * PSYSTEM_MODULE_INFORMATION;

extern "C"
{
	NTSTATUS ZwQuerySystemInformation(SYSTEM_INFORMATION_CLASS systemInformationClass, PVOID systemInformation, ULONG systemInformationLength, PULONG returnLength);
	NTSTATUS ObReferenceObjectByName(PUNICODE_STRING objectName, ULONG attributes, PACCESS_STATE accessState, ACCESS_MASK desiredAccess, POBJECT_TYPE objectType, KPROCESSOR_MODE accessMode, PVOID parseContext, PVOID* object);
	extern POBJECT_TYPE* IoDriverObjectType;
}


// ------------------------------------------------
// storport.sys
// ------------------------------------------------

typedef struct _STOR_SCSI_IDENTITY
{
	char Space[0x8]; // +0x008 SerialNumber     : _STRING
	STRING SerialNumber;
} STOR_SCSI_IDENTITY, *PSTOR_SCSI_IDENTITY;

typedef struct _TELEMETRY_UNIT_EXTENSION
{
	/*
		+0x000 Flags            : <anonymous-tag>
		+0x000 DeviceHealthEventsLogged : Pos 0, 1 Bit
		+0x000 FailedFirstSMARTCommand : Pos 1, 1 Bit
		+0x000 FailedFirstDeviceStatisticsLogCommand : Pos 2, 1 Bit
		+0x000 FailedFirstNvmeCloudSSDCommand : Pos 3, 1 Bit
		+0x000 SmartPredictFailure : Pos 4, 1 Bit
		+0x000 Reserved         : Pos 5, 27 Bits
	 */
	int SmartMask;
} TELEMETRY_UNIT_EXTENSION, * PTELEMETRY_UNIT_EXTENSION;

// lkd> dt storport!_RAID_UNIT_EXTENSION -b
typedef struct _RAID_UNIT_EXTENSION
{
	union
	{
		struct
		{
			char Space[0x68]; // +0x068 Identity         : _STOR_SCSI_IDENTITY
			STOR_SCSI_IDENTITY Identity;
		} _Identity;

		struct
		{
			char Space[0x7c8]; // +0x7c8 TelemetryExtension : _TELEMETRY_UNIT_EXTENSION
			TELEMETRY_UNIT_EXTENSION Telemetry;
		} _Smart;
	};
} RAID_UNIT_EXTENSION, *PRAID_UNIT_EXTENSION;

typedef __int64(__fastcall* RaidUnitRegisterInterfaces)(PRAID_UNIT_EXTENSION a1);

// ------------------------------------------------
// disk.sys
// ------------------------------------------------

typedef NTSTATUS(__fastcall* DiskEnableDisableFailurePrediction)(void* a1, bool a2);

// ------------------------------------------------
// SMBIOS / gnu-efi
// ------------------------------------------------
// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/IndustryStandard/SmBios.h
typedef struct
{
	UINT8   Type;
	UINT8   Length;
	UINT8   Handle[2];
} SMBIOS_HEADER;

typedef UINT8   SMBIOS_STRING;

typedef struct
{
	SMBIOS_HEADER   Hdr;
	SMBIOS_STRING   Vendor;
	SMBIOS_STRING   BiosVersion;
	UINT8           BiosSegment[2];
	SMBIOS_STRING   BiosReleaseDate;
	UINT8           BiosSize;
	UINT8           BiosCharacteristics[8];
	UINT8           BIOSCharacteristicsExtensionBytes[2];
	UINT8           SystemBiosMajorRelease;
	UINT8           SystemBiosMinorRelease;
	UINT8           EmbeddedControllerFirmwareMajorRelease;
	UINT8           EmbeddedControllerFirmwareMinorRelease;
} SMBIOS_TYPE0;

typedef struct
{
	SMBIOS_HEADER   Hdr;
	SMBIOS_STRING   Manufacturer;
	SMBIOS_STRING   ProductName;
	SMBIOS_STRING   Version;
	SMBIOS_STRING   SerialNumber;

	//
	// always byte copy this data to prevent alignment faults!
	//
	GUID			Uuid; // EFI_GUID == GUID?

	UINT8           WakeUpType;
	SMBIOS_STRING   SKUNumber;
	SMBIOS_STRING   Family;
} SMBIOS_TYPE1;

typedef struct
{
	SMBIOS_HEADER   Hdr;
	SMBIOS_STRING         Manufacturer;
	SMBIOS_STRING         ProductName;
	SMBIOS_STRING         Version;
	SMBIOS_STRING         SerialNumber;
	SMBIOS_STRING         AssetTag;
	SMBIOS_STRING         LocationInChassis;
} SMBIOS_TYPE2;

typedef struct
{
	SMBIOS_HEADER   Hdr;
	SMBIOS_STRING   Manufacturer;
	UINT8           Type;
	SMBIOS_STRING   Version;
	SMBIOS_STRING   SerialNumber;
	SMBIOS_STRING   AssetTag;
	UINT8           BootupState;
	UINT8           PowerSupplyState;
	UINT8           ThermalState;
	UINT8           SecurityStatus;
	UINT8           OemDefined[4];
} SMBIOS_TYPE3;

typedef struct
{
	SMBIOS_HEADER   Hdr;
	SMBIOS_STRING    Socket; //UINT8 before?
	UINT8           ProcessorType;
	UINT8           ProcessorFamily;
	SMBIOS_STRING   ProcessorManufacture;
	UINT8           ProcessorId[8];
	SMBIOS_STRING   ProcessorVersion;
	UINT8           Voltage;
	UINT8           ExternalClock[2];
	UINT8           MaxSpeed[2];
	UINT8           CurrentSpeed[2];
	UINT8           Status;
	UINT8           ProcessorUpgrade;
	UINT8           L1CacheHandle[2];
	UINT8           L2CacheHandle[2];
	UINT8           L3CacheHandle[2];
	SMBIOS_STRING    SerialNumber;
	SMBIOS_STRING    AssetTag;
	SMBIOS_STRING    PartNumber;
} SMBIOS_TYPE4;

typedef struct
{
	SMBIOS_HEADER   Hdr;
	UINT16                                     MemoryArrayHandle;
	UINT16                                     MemoryErrorInformationHandle;
	UINT16                                     TotalWidth;
	UINT16                                     DataWidth;
	UINT16                                     Size;
	UINT8                                      FormFactor;        ///< The enumeration value from MEMORY_FORM_FACTOR.
	UINT8                                      DeviceSet;
	SMBIOS_STRING                        DeviceLocator;
	SMBIOS_STRING                        BankLocator;
	UINT8                                      MemoryType;        ///< The enumeration value from MEMORY_DEVICE_TYPE.
	//MEMORY_DEVICE_TYPE_DETAIL                  TypeDetail;
	UINT16                                     Speed;
	SMBIOS_STRING                        Manufacturer;
	SMBIOS_STRING                        SerialNumber;
	SMBIOS_STRING                        AssetTag;
	SMBIOS_STRING                        PartNumber;
	//
	// Add for smbios 2.6
	//
	UINT8                                      Attributes;
	//
	// Add for smbios 2.7
	//
	UINT32                                     ExtendedSize;
	//
	// Keep using name "ConfiguredMemoryClockSpeed" for compatibility
	// although this field is renamed from "Configured Memory Clock Speed"
	// to "Configured Memory Speed" in smbios 3.2.0.
	//
	UINT16                                     ConfiguredMemoryClockSpeed;
	//
	// Add for smbios 2.8.0
	//
	UINT16                                     MinimumVoltage;
	UINT16                                     MaximumVoltage;
	UINT16                                     ConfiguredVoltage;
	//
	// Add for smbios 3.2.0
	//
	UINT8                                      MemoryTechnology;  ///< The enumeration value from MEMORY_DEVICE_TECHNOLOGY
	//MEMORY_DEVICE_OPERATING_MODE_CAPABILITY    MemoryOperatingModeCapability;
	SMBIOS_STRING                        FirmwareVersion;
	UINT16                                     ModuleManufacturerID;
	UINT16                                     ModuleProductID;
	UINT16                                     MemorySubsystemControllerManufacturerID;
	UINT16                                     MemorySubsystemControllerProductID;
	UINT64                                     NonVolatileSize;
	UINT64                                     VolatileSize;
	UINT64                                     CacheSize;
	UINT64                                     LogicalSize;
	//
	// Add for smbios 3.3.0
	//
	UINT32                                     ExtendedSpeed;
	UINT32                                     ExtendedConfiguredMemorySpeed;
} SMBIOS_TYPE17;

typedef union
{
	SMBIOS_HEADER* Hdr;
	SMBIOS_TYPE0* Type0;
	SMBIOS_TYPE1* Type1;
	SMBIOS_TYPE2* Type2;
	SMBIOS_TYPE3* Type3;
	SMBIOS_TYPE4* Type4;
	UINT8* Raw;
} SMBIOS_STRUCTURE_POINTER;

typedef struct
{
	UINT8   AnchorString[4];
	UINT8   EntryPointStructureChecksum;
	UINT8   EntryPointLength;
	UINT8   MajorVersion;
	UINT8   MinorVersion;
	UINT16  MaxStructureSize;
	UINT8   EntryPointRevision;
	UINT8   FormattedArea[5];
	UINT8   IntermediateAnchorString[5];
	UINT8   IntermediateChecksum;
	UINT16  TableLength;
	UINT32  TableAddress;
	UINT16  NumberOfSmbiosStructures;
	UINT8   SmbiosBcdRevision;
} SMBIOS_STRUCTURE_TABLE;

typedef struct _RAW_SMBIOS
{
	UINT8	Unknown;
	UINT8	MajorVersion;
	UINT8	MinorVersion;
	UINT8	DmiRevision;
	UINT32	Size;
	UINT8* Entry;
} RAW_SMBIOS;