#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "../../ImGui/imgui.h"
#include "../../Settings/Settings.h"
#include "../../SDK/sdk.h"
#include "../../Actor/Actorloop.h"


namespace menu
{
    // Green if pointer is valid, red if null
    inline void PtrText(const char* label, unsigned long long ptr)
    {
        ImVec4 col = ptr
            ? ImVec4(0.40f, 1.00f, 0.40f, 1.00f)
            : ImVec4(1.00f, 0.35f, 0.35f, 1.00f);
        ImGui::TextColored(col, "%-17s [0x%llX]", label, ptr);
    }

    inline void RefreshCache()
    {
        CachePointers.UWorld = decrypt_world();
        CachePointers.GameInstance = memory.read<__int64>(CachePointers.UWorld + offsets::Core::OwningGameInstance);
        CachePointers.LocalPlayer = memory.read<__int64>(memory.read<__int64>(CachePointers.GameInstance + offsets::Core::LocalPlayers));
        CachePointers.PlayerController = memory.read<__int64>(CachePointers.LocalPlayer + offsets::Core::PlayerController);
        CachePointers.AcknownledgedPawn = memory.read<__int64>(CachePointers.PlayerController + offsets::Core::AcknowledgedPawn);
        CachePointers.PlayerState = CachePointers.AcknownledgedPawn ? memory.read<__int64>(CachePointers.AcknownledgedPawn + offsets::Core::PlayerState) : 0;
        CachePointers.RootComponent = CachePointers.AcknownledgedPawn ? memory.read<__int64>(CachePointers.AcknownledgedPawn + offsets::Core::RootComponent) : 0;
        CachePointers.Mesh = CachePointers.AcknownledgedPawn ? memory.read<__int64>(CachePointers.AcknownledgedPawn + offsets::Core::Mesh) : 0;
        CachePointers.relative_location = CachePointers.RootComponent ? memory.read<Vector3>(CachePointers.RootComponent + offsets::Core::RelativeLocation) : Vector3(0, 0, 0);
        CachePointers.GameState = memory.read<__int64>(CachePointers.UWorld + offsets::Core::GameState);
        CachePointers.PlayerArray = memory.read<__int64>(CachePointers.GameState + offsets::Core::PlayerArray);
        CachePointers.PlayerArraySize = memory.read<int>(CachePointers.GameState + (offsets::Core::PlayerArray + sizeof(uintptr_t)));
    }

    inline void render()
    {
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags &= ~ImGuiConfigFlags_NoKeyboard;
        io.WantCaptureKeyboard = true;

        ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
        ImGui::Begin("Noxyftw External Base", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

        if (ImGui::BeginTabBar("MainTabs"))
        {
            if (ImGui::BeginTabItem("Aimbot"))
            {
                ImGui::Checkbox("Aimbot (RMB)", &Aim::SoftAim);
                ImGui::Checkbox("FOV", &Aim::Fov);
                ImGui::SliderFloat("Smoothness", &Aim::Smoothness, 1.0f, 10.0f);
                ImGui::SliderFloat("FOV Size", &Aim::FovSize, 50.0f, 300.0f);

                ImGui::Checkbox("Triggerbot (", &Aim::Triggerbot);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Visual"))
            {
                ImGui::Checkbox("Box ESP", &Visuals::Box);
                ImGui::Checkbox("Skeleton ESP", &Visuals::Skeleton);
                ImGui::Checkbox("Kills", &Visuals::Kills);
                ImGui::Checkbox("Distance", &Visuals::Distance);
                ImGui::Checkbox("Corner ESP", &Visuals::Corner);
                ImGui::Checkbox("Filled Box", &Visuals::filledbox);
                ImGui::Checkbox("Platform", &Visuals::Platform);
                ImGui::Checkbox("Name", &Visuals::Name);

                if (Visuals::Box || Visuals::Skeleton)
                    ImGui::Checkbox("Outlines", &Visuals::Outlines);

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Misc"))
            {
                ImGui::SliderFloat("Box Thickness", &Visuals::Box_Thickness, 1.f, 10.f, "%.0f");
                ImGui::SliderFloat("Bone Thickness", &Visuals::Bone_Thickness, 1.f, 10.f, "%.0f");
                ImGui::SliderFloat("Corner Size", &Visuals::Corner_Size, 5.f, 30.f, "%.0f");
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Colors"))
            {
                ImGui::Checkbox("Rainbow Box", &Visuals::RainbowBox);
                ImGui::Checkbox("Rainbow Skeleton", &Visuals::RainbowSkeleton);
                ImGui::Checkbox("Rainbow Corner", &Visuals::RainbowCorner);
                ImGui::Checkbox("Rainbow Distance", &Visuals::RainbowDistance);
                ImGui::Checkbox("Rainbow Name", &Visuals::RainbowName);
                ImGui::Checkbox("Rainbow Kills", &Visuals::RainbowKills);
                ImGui::Checkbox("Rainbow Platform", &Visuals::RainbowPlatform);

                ImGui::Separator();

                ImGui::ColorEdit4("Box Color", (float*)&colors.box);
                ImGui::ColorEdit4("Corner Color", (float*)&colors.corner);
                ImGui::ColorEdit4("Skeleton Color", (float*)&colors.skeleton);
                ImGui::ColorEdit4("Distance Color", (float*)&colors.Distance);
                ImGui::ColorEdit4("Name Color", (float*)&colors.Name);
                ImGui::ColorEdit4("Kills Color", (float*)&colors.Kills);
                ImGui::ColorEdit4("Platform Color", (float*)&colors.Platform);
                ImGui::ColorEdit4("Fill Color", (float*)&colors.fill);

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Debugs"))
            {
                ImGui::Text("UWorld:          [0x%llX]", CachePointers.UWorld);
                ImGui::Text("GameInstance:    [0x%llX]", CachePointers.GameInstance);
                ImGui::Text("LocalPlayer:     [0x%llX]", CachePointers.LocalPlayer);
                ImGui::Text("PlayerController:[0x%llX]", CachePointers.PlayerController);
                ImGui::Text("AcknowledgedPawn:[0x%llX]", CachePointers.AcknownledgedPawn);
                ImGui::Text("PlayerState:     [0x%llX]", CachePointers.PlayerState);
                ImGui::Text("RootComponent:   [0x%llX]", CachePointers.RootComponent);
                ImGui::Text("Mesh:            [0x%llX]", CachePointers.Mesh);
                ImGui::Text("GameState:       [0x%llX]", CachePointers.GameState);
                ImGui::Text("PlayerArray:     [0x%llX]", CachePointers.PlayerArray);
                ImGui::Text("PlayerArraySize: %d", CachePointers.PlayerArraySize);

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End();
    }
}