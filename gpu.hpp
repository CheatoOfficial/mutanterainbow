#pragma once
#include "utils.h"
#include <cstdint>

#include "hde64.hpp"
#include "log.h"

namespace n_gpu
{
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

		// Pattern not found or incorrect match.
		if (!Addr || *(uint8_t*)(Addr + 0x3B) != 0xE8) {
			Log::Print("Could not find pattern.\n");
			return 0;
		}

		// Resolve reference.
		uint64_t(*GpuMgrGetGpuFromId)(int) = decltype(GpuMgrGetGpuFromId)(*(int*)(Addr + 1) + 5 + Addr);

		Addr += 0x3B;

		// gpuGetGidInfo
		Addr += *(int*)(Addr + 1) + 5;

		// Walk instructions to find GPU::gpuUuid.isInitialized offset.
		for (int InstructionCount = 0; InstructionCount < 50; InstructionCount++) {
			hde64s HDE;
			hde64_disasm((void*)Addr, &HDE);

			// Did HDE fail to disassemble the instruction?
			if (HDE.flags & F_ERROR) {
				Log::Print("Failed to disassemble %p.\n", Addr);
				return 0;
			}

			// cmp [rcx + GPU::gpuUuid.isInitialized], dil
			uint32_t Opcode = *(uint32_t*)Addr & 0xFFFFFF;
			if (Opcode == 0xB93840) {
				UuidValidOffset = *(uint32_t*)(Addr + 3);
				break;
			}

			// Increment instruction pointer.
			Addr += HDE.len;
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