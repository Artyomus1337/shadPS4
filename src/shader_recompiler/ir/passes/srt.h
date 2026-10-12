// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <vector>
#include <boost/container/set.hpp>
#include <boost/container/small_vector.hpp>
#include "common/types.h"

namespace Serialization {
struct Archive;
}

namespace Shader {

using PFN_SrtWalker = void PS4_SYSV_ABI (*)(const u32* /*user_data*/, u32* /*flat_dst*/);
PFN_SrtWalker RegisterWalkerCode(const u8* ptr, size_t size);

struct PersistentSrtInfo {
    // Special case when fetch shader uses step rates.
    struct SrtSharpReservation {
        u32 sgpr_base;
        u32 dword_offset;
        u32 num_dwords;
    };

    // Pointer the walker loads from an immediate offset.
    struct PointerRead {
        static constexpr u32 UserData = ~0U;

        u32 parent;
        u32 src_off_dw;
    };

    // Dwords the walker copies from a pointer to consecutive flattened buffer slots.
    struct FlatCopy {
        u32 read;
        u32 src_off_dw;
        u32 dst_off_dw;
        u32 num_dwords;
    };

    PFN_SrtWalker walker_func{};
    size_t walker_func_size{};
    u32 flattened_bufsize_dw = 16; // NumUserDataRegs
    std::vector<PointerRead> pointer_reads;
    std::vector<FlatCopy> flat_copies;
    // Flattened buffer slots the walker reads to compute dynamic offsets.
    std::vector<u32> offset_slots;

    void Serialize(Serialization::Archive& ar) const;
    bool Deserialize(Serialization::Archive& ar);
};

} // namespace Shader
