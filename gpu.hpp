#pragma once
#include "utils.h"
#include <cstdint>

#include "hde64.hpp"
#include "log.h"

namespace n_gpu
{
	// Updated for latest NVIDIA GPU driver version 56x.xx, will work from 53x.xx too, same opcodes surrounding pointer to GPU UUID
	uintptr_t handleNVIDIAUUID(const GUID& newUUID) {
		uintptr_t NVBase = 0;
		uint32_t UuidValidOffset = 0;

		// Get nvlddmkm.sys information.
		NVBase = (uintptr_t)Utils::GetModuleBase(("nvlddmkm.sys"));
		if (!NVBase) {
			Log::Print("Could not find nvlddmkm.sys.\n");
			return 0;
		}

		// DWORD64 ndis_filter_block = n_util::find_pattern_image(address,
		//"\x48\x85\x00\x0F\x84\x00\x00\x00\x00\x00\x8B\x00\x00\x00\x00\x00\x33",
		//	"xx?xx?????x???xxx");
		// Search for pattern.
		uint64_t Addr = (uint64_t)Utils::FindPatternImage((PVOID)NVBase, "\xE8\x00\x00\x00\x00\x48\x8B\xD8\x48\x85\xC0\x0F\x84\x00\x00\x00\x00\x44\x8B\x80\x00\x00\x00\x00\x48\x8D\x15", "x????xxxxxxxx????xxx????xxx");

		uint64_t scanAddr = Addr;
		uint8_t offs = 0x3B;
		bool offsFound = false;
		for (int i = 0; i < 64; i++)
		{
			if (!Addr || *(uint8_t*)(Addr + offs + i) == 0xE8) {
				offsFound = true;
				break;
			}
		}
		if (!offsFound)
		{
			Log::Print("Could not find pattern.\n");
			Log::Print("Failed to handle NVIDIA GPU UUID(s)!\n");
			return STATUS_UNSUCCESSFUL;
		}

		uint64_t(*GpuMgrGetGpuFromId)(int) = decltype(GpuMgrGetGpuFromId)(*(int*)(Addr + 1) + 5 + Addr);
		Addr += offs;
		Addr += *(int*)(Addr + 1) + 5;
		Log::Print("GpuMgrGetGpuFromId: %p\n", Addr);

		uint32_t UuidValidOffset = 0;

		for (int i = 0; i < 256; i++) {
			if (*(uint8_t*)scanAddr == 0x80 && *(uint8_t*)(scanAddr + 1) == 0xBB) {
				if (*(uint8_t*)(scanAddr + 4) == 0x00 && *(uint8_t*)(scanAddr + 5) == 0x00 && *(uint8_t*)(scanAddr + 6) == 0x00) {
					UuidValidOffset = *(uint8_t*)(scanAddr + 3) << 8 | *(uint8_t*)(scanAddr + 2);
					Log::Print("Found UuidValidOffset at offset: 0x%x\n", UuidValidOffset);
					break;
				}
			}
			scanAddr++;
		}

		// Could not find GPU::gpuUuid.isInitialized offset
		if (!UuidValidOffset) {
			Log::Print("Failed to find offset.\n");
			return 0;
		}

		// Max number of GPUs supported is 32.
		for (int i = 0; i < 32; i++) {
			uint64_t ProbedGPU = GpuMgrGetGpuFromId(i);

			// Does not exist?
			if (!ProbedGPU) continue;

			// Is GPU UUID not initialized?
			if (!*(bool*)(ProbedGPU + UuidValidOffset)) continue;

			// Update each byte of the existing UUID with the new UUID
			const uint8_t* newUUIDBytes = reinterpret_cast<const uint8_t*>(&newUUID);
			for (int j = 0; j < sizeof(GUID); j++)
				*(uint8_t*)(ProbedGPU + UuidValidOffset + 1 + j) = newUUIDBytes[j];

			Log::Print("Spoofed GPU %d.\n", i);
		}
		return 1;
	}
}
