#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include "Drawing.h"
#include "../SDK/sdk.h"
#include "../Settings/Lazy.hxx"
#include <thread>
#include <Windows.h>
#include <iostream>
#include <mutex>
#include <vector>
#include <algorithm>



bool memory_event(Vector3 newpos)
{
    memory.write<Vector3>(CachePointers.PlayerController + offsets::Core::RotationInput, newpos);
    return true;
}

void memory_move(Vector3 head3d)
{
    Vector2 head2d = project_world_to_screen(head3d);


    float verity = 6.0f + (200.0f - 6.0f) * ((Aim::Smoothness - 1) / 9.0f); 

    Vector2 target{};
    if (head2d.x != 0)
    {
        if (head2d.x > globals.ScreenXHALF)
        {
            target.x = -(globals.ScreenXHALF - head2d.x);
            target.x /= verity;
            if (target.x + globals.ScreenXHALF > globals.ScreenXHALF * 2) target.x = 0;
        }
        if (head2d.x < globals.ScreenXHALF)
        {
            target.x = head2d.x - globals.ScreenXHALF;
            target.x /= verity;
            if (target.x + globals.ScreenXHALF < 0) target.x = 0;
        }
    }

    if (head2d.y != 0)
    {
        if (head2d.y > globals.ScreenYHALF)
        {
            target.y = -(globals.ScreenYHALF - head2d.y);
            target.y /= verity;
            if (target.y + globals.ScreenYHALF > globals.ScreenYHALF * 2) target.y = 0;
        }
        if (head2d.y < globals.ScreenYHALF)
        {
            target.y = head2d.y - globals.ScreenYHALF;
            target.y /= verity;
            if (target.y + globals.ScreenYHALF < 0) target.y = 0;
        }
    }


    memory_event(Vector3(-target.y / 5, target.x / 5, 0));
}

Vector3 GetAimBonePosition(uintptr_t mesh)
{
    switch (Aim::TargetBone)
    {
    case 0: return getplayerbone(mesh, 110);
    case 1: return getplayerbone(mesh, 67);
    case 2: return getplayerbone(mesh, 7);
    case 3: return getplayerbone(mesh, 2);
    default: return getplayerbone(mesh, 110);
    }
}


#define FUNC CallSpoofer::SpoofFunction spoof( _AddressOfReturnAddress( ) );

inline void DrawBox(ImDrawList* draw_list, const ImVec2 a, const ImVec2 b, ImColor color, float thickness = 1.0f, bool outline = false) {
    if (outline) {
        draw_list->AddRect(a, b, ImColor(0, 0, 0, 255), 0.0f, ImDrawFlags_RoundCornersAll, thickness + 2.0f);
    }
    draw_list->AddRect(a, b, color, 0.0f, ImDrawFlags_RoundCornersAll, thickness);
}

inline void DrawSkeleton(ImDrawList* draw_list, uintptr_t skeletalmesh, ImColor color, float thickness = 1.0f, bool outline = false) {
    const int boneIndices[] = { 110, 3, 66, 9, 38, 10, 39, 11, 40, 78, 71, 79, 72, 75, 82, 67 };
    Vector2 bonePositions[16];
    
    for (int i = 0; i < 16; ++i)
        bonePositions[i] = project_world_to_screen(getplayerbone(skeletalmesh, boneIndices[i]));

    const std::pair<int, int> bonePairs[] = {
        {1, 2}, {5, 3}, {6, 4}, {5, 7}, {6, 8},
        {10, 1}, {9, 1}, {12, 10}, {11, 9}, {13, 12}, {14, 11}, {2, 15}, {2, 4}, {2, 3}
    };

    for (const auto& [i1, i2] : bonePairs) {
        Vector2 p1 = bonePositions[i1], p2 = bonePositions[i2];
        if (p1.x && p1.y && p2.x && p2.y) {
            if (outline) {
                draw_list->AddLine(ImVec2(p1.x, p1.y), ImVec2(p2.x, p2.y), ImColor(0, 0, 0, 255), thickness + 2.0f);
            }
            draw_list->AddLine(ImVec2(p1.x, p1.y), ImVec2(p2.x, p2.y), color, thickness);
        }
    }
}

inline uintptr_t decrypt_world()
{
    if (!memory.BaseAddress)
        memory.BaseAddress = driver.GetBase();

    if (!memory.BaseAddress)
        return 0;

    std::uint64_t encoded =
        memory.read<std::uint64_t>(memory.BaseAddress + 0x1B2C5BA0);

    std::uintptr_t world = static_cast<std::uintptr_t>(0x6501B96661E130DDULL * encoded + 0x79D95BD19230E74DULL);

    return world ? world : 0;
}

inline void Game() {
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

    static uintptr_t sticky_target = NULL;

    __int64 localPS = CachePointers.PlayerController ? memory.read<__int64>(CachePointers.PlayerController + offsets::Core::PlayerState) : 0; // playercntrller buggy 

    if (!localPS) localPS = CachePointers.PlayerState;

    CachePointers.TeamIndex = localPS ? memory.read<char>(localPS + offsets::Core::TeamIndex) : -1;

    cache::local_camera = get_camera();
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

    Visuals::VisiblePlayers = 0;

    struct EntityData { uintptr_t player_state; uintptr_t pawn_private; bool is_lobby; };
    std::vector<EntityData> entities_to_draw;
    bool in_lobby = (CachePointers.AcknownledgedPawn == 0);

   
    auto iterate_actors = [&]() {
        uintptr_t levels_array = memory.read<uintptr_t>(CachePointers.UWorld + offsets::Core::Levels);
        int levels_count = memory.read<int>(CachePointers.UWorld + offsets::Core::Levels + 0x8);
        if (levels_count <= 0 || levels_count >= 50) return;

        for (int l = 0; l < levels_count; l++) {
            uintptr_t level = memory.read<uintptr_t>(levels_array + l * sizeof(uintptr_t));
            if (!level) continue;

            uintptr_t actor_array = memory.read<uintptr_t>(level + offsets::Lobby::Actors);
            int actor_count = memory.read<int>(level + offsets::Lobby::Actors + sizeof(uintptr_t));
            if (actor_count <= 0 || actor_count >= 20000) continue;

            int max_loop = (actor_count > 500) ? 500 : actor_count;
            for (int a = 0; a < max_loop; a++) {
                uintptr_t current_actor = memory.read<uintptr_t>(actor_array + a * sizeof(uintptr_t));
                if (!current_actor) continue;

                uintptr_t skeletalmesh = memory.read<uintptr_t>(current_actor + offsets::Core::Mesh);
                if (!skeletalmesh) continue;

                uintptr_t bone_array = memory.read<uintptr_t>(skeletalmesh + offsets::Core::BoneArray);
                if (!bone_array) bone_array = memory.read<uintptr_t>(skeletalmesh + offsets::Core::BoneArray + 0x10);
                if (!bone_array) continue;

               
                if (in_lobby) {
                    uintptr_t filter_2e0 = memory.read<uintptr_t>(current_actor + 0x2E0);
                    if (filter_2e0 != 0) continue;
                }

                uintptr_t ps = memory.read<uintptr_t>(current_actor + offsets::Core::PlayerState);
                entities_to_draw.push_back({ ps, current_actor, in_lobby });
            }
        }
        };

    if (in_lobby) {
        if (Visuals::LobbyESP) iterate_actors();
    }
    else {
      
        uintptr_t player_array = CachePointers.PlayerArray;
        int player_count = CachePointers.PlayerArraySize;

     
        if (player_array && player_array != CachePointers.GameState
            && player_count > 0 && player_count < 200)
        {
            for (int i = 0; i < player_count; i++) {
                uintptr_t player_state = memory.read<uintptr_t>(player_array + i * sizeof(uintptr_t));
                if (!player_state) continue;
                uintptr_t pawn_private = memory.read<uintptr_t>(player_state + offsets::Core::PawnPrivate);
                if (!pawn_private) continue;
                entities_to_draw.push_back({ player_state, pawn_private, false });
            }
        }

       
        if (entities_to_draw.empty())
            iterate_actors();
    }

    colors.update(0.001f);

    for (const auto& ent : entities_to_draw) {
        uintptr_t PlayerState = ent.player_state;
        uintptr_t pawn_private = ent.pawn_private;
        bool is_lobby_ent = ent.is_lobby;

        if (pawn_private == CachePointers.AcknownledgedPawn) continue;

        uintptr_t skeletalmesh = memory.read<uintptr_t>(pawn_private + offsets::Core::Mesh);
        if (!skeletalmesh) continue;

        Vector3 head3d = getplayerbone(skeletalmesh, 110);
        Vector2 head2d = project_world_to_screen(Vector3(head3d.x, head3d.y, head3d.z + 20));
        Vector3 bottom3d = getplayerbone(skeletalmesh, 0);
        Vector2 bottom2d = project_world_to_screen(bottom3d);
        float box_height = abs(head2d.y - bottom2d.y);
        float box_width = box_height * Visuals::Box_Width;
        Vector3 root_bone = getplayerbone(skeletalmesh, 0);
        Vector2 root_box = project_world_to_screen(Vector3(root_bone.x, root_bone.y, root_bone.z - 10));
        Vector3 head_bone = getplayerbone(skeletalmesh, 110);
        Vector2 head_box = project_world_to_screen(Vector3(head_bone.x, head_bone.y, head_bone.z + 15));

        uintptr_t targeted = memory.read<uintptr_t>(CachePointers.PlayerController + offsets::Core::TargetedFortPawn);

        if (bottom2d.x == 0 && bottom2d.y == 0) continue;
        if (head2d.x == 0 && head2d.y == 0) continue;

        if (is_lobby_ent) {
            bool off_screen = (head2d.x < 0 || head2d.x > globals.ScreenX ||
                head2d.y < 0 || head2d.y > globals.ScreenY);
            if (off_screen) continue;
        }

        const float halfWidth = box_width / 2.0f;
        ImVec2 topLeft(head_box.x - halfWidth, head_box.y);
        ImVec2 bottomRight(root_box.x + halfWidth, root_box.y);

        if (is_lobby_ent && box_height < 20.0f) continue;

        float distance = CachePointers.relative_location.distance(bottom3d) / 100.f;

      
        if (Visuals::SkipTeam && !is_lobby_ent && CachePointers.TeamIndex >= 0) {
            char team_id = memory.read<char>(PlayerState + offsets::Core::TeamIndex);
            if (team_id == CachePointers.TeamIndex)
                continue;
        }

        if (head2d.x > 0 && head2d.x < globals.ScreenX &&
            head2d.y > 0 && head2d.y < globals.ScreenY)
        {
            if (is_lobby_ent || is_visible(skeletalmesh))
                Visuals::VisiblePlayers++;
        }

        ImColor rainbowColor = ImColor(colors.wave.r, colors.wave.g, colors.wave.b);

        if (Visuals::Box) {
            ImColor boxColor = Visuals::RainbowBox ? rainbowColor : colors.box;
            DrawBox(draw_list, topLeft, bottomRight, boxColor, Visuals::Box_Thickness, Visuals::Outlines);
        }

        if (Visuals::Skeleton) {
            ImColor skeletonColor = Visuals::RainbowSkeleton ? rainbowColor : colors.skeleton;
            DrawSkeleton(draw_list, skeletalmesh, skeletonColor, Visuals::Bone_Thickness, Visuals::Outlines);
        }

        if (Aim::Triggerbot) {
            if (GetAsyncKeyState(VK_XBUTTON2) && targeted) {
                keybd_event(0x01, 0, 0, 0);
                Sleep(1);
                keybd_event(0x01, 0, 0x0002, 0);
            }
        }

        if (Visuals::Corner) {
            ImColor cornerColor = Visuals::RainbowCorner ? rainbowColor : colors.corner;
            float corner_size = Visuals::Corner_Size;
            ImVec2 tl(topLeft.x, topLeft.y);
            ImVec2 tr(bottomRight.x, topLeft.y);
            ImVec2 bl(topLeft.x, bottomRight.y);
            ImVec2 br(bottomRight.x, bottomRight.y);

            draw_list->AddLine(tl, ImVec2(tl.x + corner_size, tl.y), cornerColor, Visuals::Box_Thickness);
            draw_list->AddLine(tl, ImVec2(tl.x, tl.y + corner_size), cornerColor, Visuals::Box_Thickness);
            draw_list->AddLine(tr, ImVec2(tr.x - corner_size, tr.y), cornerColor, Visuals::Box_Thickness);
            draw_list->AddLine(tr, ImVec2(tr.x, tr.y + corner_size), cornerColor, Visuals::Box_Thickness);
            draw_list->AddLine(bl, ImVec2(bl.x + corner_size, bl.y), cornerColor, Visuals::Box_Thickness);
            draw_list->AddLine(bl, ImVec2(bl.x, bl.y - corner_size), cornerColor, Visuals::Box_Thickness);
            draw_list->AddLine(br, ImVec2(br.x - corner_size, br.y), cornerColor, Visuals::Box_Thickness);
            draw_list->AddLine(br, ImVec2(br.x, br.y - corner_size), cornerColor, Visuals::Box_Thickness);
        }

        if (Visuals::Distance) {
            ImColor distanceColor = Visuals::RainbowDistance ? rainbowColor : colors.Distance;
            std::string dist_text = std::to_string((int)distance) + "m";
            draw_list->AddText(ImVec2(bottomRight.x + 5, bottomRight.y), distanceColor, dist_text.c_str());
        }

        if (Visuals::Kills) {
            ImColor killsColor = Visuals::RainbowKills ? rainbowColor : colors.Kills;
            int kills = memory.read<int>(PlayerState + offsets::Core::Killscore);
            std::string kills_text = "Kills: " + std::to_string(kills);
            draw_list->AddText(ImVec2(topLeft.x, topLeft.y - 15), killsColor, kills_text.c_str());
        }

        if (Visuals::filledbox) {
            draw_list->AddRectFilled(topLeft, bottomRight, ImColor(0.06f, 0.06f, 0.06f, 0.6f));
        }

        float gap = 2.0f;
        float current_y = head_box.y;

        if (Visuals::Platform) {
            std::string platform_str = "";
            if (PlayerState) {
                uintptr_t platform_ptr = memory.read<uintptr_t>(PlayerState + offsets::Core::Platform);
                wchar_t platform_char[64] = { 0 };
                driver.readphys(reinterpret_cast<PVOID>(platform_ptr), reinterpret_cast<uint8_t*>(platform_char), sizeof(platform_char));
                std::wstring platform_wide_str(platform_char);
                platform_str = std::string(platform_wide_str.begin(), platform_wide_str.end());
            }

            std::string display_platform = "";
            if (platform_str == xorstr_("XBL") || platform_str == xorstr_("XSX"))  display_platform = "XBOX";
            else if (platform_str == xorstr_("PSN") || platform_str == xorstr_("PS5")) display_platform = "PSN";
            else if (platform_str == xorstr_("SWT"))                                    display_platform = "SWITCH";
            else if (platform_str == xorstr_("WIN"))                                    display_platform = "WIN";
            else if (platform_str == xorstr_("AND"))                                    display_platform = "ANDROID";
            else if (platform_str == xorstr_("IOS"))                                    display_platform = "IOS";
            else                                                                        display_platform = "WIN";

            if (!display_platform.empty()) {
                ImColor platformColor = Visuals::RainbowPlatform ? rainbowColor : colors.Platform;
                ImVec2 platformSize = ImGui::CalcTextSize(display_platform.c_str());
                float plat_y = current_y - platformSize.y - gap;
                DrawString(13.f, head_box.x - (platformSize.x / 2), plat_y,
                    platformColor, false, true, display_platform.c_str());
                current_y = plat_y - gap;
            }
        }

        if (Visuals::Name) {
            std::string playerUsername = GetPlayerName(PlayerState);
            ImColor nameColor = Visuals::RainbowName ? rainbowColor : colors.Name;
            ImVec2 nameSize = ImGui::CalcTextSize(playerUsername.c_str());
            float name_y = current_y - nameSize.y - gap;
            DrawString(14.f, head_box.x - (nameSize.x / 2), name_y,
            nameColor, false, true, playerUsername.c_str());
            current_y = name_y - gap;
        }

        if (Aim::SoftAim)
        {
            float closest_dist = Aim::FovSize;
            uintptr_t best_target = NULL;


            double dx = head2d.x - globals.ScreenXHALF;
            double dy = head2d.y - globals.ScreenYHALF;
            float current_dist = sqrtf(dx * dx + dy * dy);


            if (current_dist < closest_dist)
            {
                best_target = pawn_private;
                closest_dist = current_dist;
            }


            if (sticky_target)
            {
                uintptr_t sticky_mesh = memory.read<uint64_t>(sticky_target + offsets::Core::Mesh);
                if (sticky_mesh)
                {
                    Vector3 sticky_head = GetAimBonePosition(sticky_mesh);
                    Vector2 sticky_2d = project_world_to_screen(sticky_head);

                    float sticky_dx = sticky_2d.x - globals.ScreenXHALF;
                    float sticky_dy = sticky_2d.y - globals.ScreenYHALF;
                    float sticky_dist = sqrtf(sticky_dx * sticky_dx + sticky_dy * sticky_dy);


                    if (sticky_dist < Aim::FovSize && sticky_dist < closest_dist)
                    {
                        best_target = sticky_target;
                        closest_dist = sticky_dist;
                    }
                }
            }


            CachePointers.target_entity = best_target;
            sticky_target = best_target;
        }


        if (CachePointers.target_entity && Aim::SoftAim)
        {
            auto closest_mesh = memory.read<uint64_t>(CachePointers.target_entity + offsets::Core::Mesh);
            if (!closest_mesh) return;

            Vector3 hitbox = GetAimBonePosition(closest_mesh);
            Vector2 hitbox2d = project_world_to_screen(hitbox);

            if (hitbox2d.x > 0 && hitbox2d.y > 0)
            {
                float dist = get_cross_distance(hitbox2d.x, hitbox2d.y, globals.ScreenXHALF, globals.ScreenYHALF);


                if (dist <= Aim::FovSize)
                {
                    if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
                    {
                        memory_move(hitbox);
                    }
                }
            }
        }

        if (Aim::Fov) {
            ImVec2 screen_center(globals.ScreenXHALF, globals.ScreenYHALF);
            ImU32 color = ImColor(255, 255, 255, 255);
            float radius = Aim::FovSize;
            ImGui::GetBackgroundDrawList()->AddCircle(screen_center, radius, color, 64, 1.5f);
        }

    }
}

    





