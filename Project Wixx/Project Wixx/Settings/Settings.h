#pragma once
#include "../ImGui/imgui.h"

#include <algorithm>
#include <Windows.h>

class globals_t {
public:
    int ScreenX = 0;
    int ScreenY = 0;
    int ScreenXHALF = 0;
    int ScreenYHALF = 0;
    int ScreenWidth = 0;
    int ScreenHeight = 0;
    HWND window_handle = nullptr;
    bool show_menu = true;
}; inline globals_t globals;

namespace Aim {
    bool SoftAim = false;
    float Smoothness = 6.f;
    bool Fov = false;
    float FovSize = 100.f;
    bool head = false;
    bool chest = false;
    bool neck = false;
    bool dih = false;
    bool FovArrow = false;
    bool FovLines = false;
    bool Triggerbot = false;
    bool FilledFov = false;
    bool RainbowFov = false;
    int TargetBone = 0;

    namespace Triggerbots {
        char aimkey_name[32] = "ALT";
        bool waiting = false;
        int waiting_on_key = 0;
    }

    namespace Aimbot {
        char aimkey_name1[32] = "ALT";
        bool waiting1 = false;
        int waiting_on_key1 = 0;
    }
}

namespace Visuals {
    // ESP Toggles
    bool Box = false;
    bool Corner = false;
    bool DBox = false;
    bool filledbox = false;
    bool Skeleton = false;
    bool Gender = false;
	bool hpbar = false;
    bool Distance = false;
    bool Name = false;
    bool Weapon = false;
	bool level = false;
    bool Snapline = false;
    bool heart = false;
    bool Outlines = false;

    // Rainbow Toggles
    bool RainbowBox = false;
    bool RainbowSkeleton = false;
    bool RainbowCorner = false;
    bool RainbowDistance = false;
    bool RainbowName = false;
    bool RainbowKills = false;
    bool RainbowPlatform = false;

    // ESP Settings
    int BoxCount = 0;
    int SkeliCount = 0;
    int SnapCount = 0;
    int filledboxCount = 0;
    float Box_y = 0.37f;
    float Render_Distance = 200.f;
    float Box_Thickness = 1.f;
    float Bone_Thickness = 1.f;
    float Box_Width = 0.5f;
    float heart_size = 16.0f;
    float heart_thickness = 2.4f;
    float Corner_Size = 15.0f;

    // Filtering
    bool SkipTeam = false;
    bool SkipAI = false;
    bool SkipKnocked = false;
    bool SelfESP = false;
    bool LobbyESP = true;

    // Misc
    bool Loot = false;
    bool Chest = false;
    bool reload = false;
    bool visible = false;
    bool Watermark = true;
    bool ammo = false;
    bool spectators = false;
    bool RiceHat = false;
    bool Radar = false;
    bool Platform = false;
    bool Rank = false;
    bool Kills = false;

    // Runtime
    int VisiblePlayers = 0;

    // Menu
    int menukey = VK_F2;
    bool waiting_for_key = false;
    char aimkey_name[32] = "F2";
}

namespace Exploits {
    bool Playersize = false;
    bool FOVChanger = false;
    bool BulletTP = false;
    bool NoRecoil = false;
    bool RapidFire = false;
    bool Darksky = false;
    bool AirStuck = false;
    bool Tiny = false;
    bool Giant = false;
    bool Customsize = false;
    float Customsizeslider = 1.0f;
    bool instareload = false;
    bool Spinbot = false;
    bool CurrentVehicle = false;
    float SpinbotScale = 360.f;
    float PlayerSize = 1.f;
    float FOVChangerScale = 130.f;

    namespace Airstuck {
        char aimkey_name[32] = "Alt";
        bool waiting = false;
        int waiting_on_key = 0;
    }
}

struct rgb_t {
    int r = 255, g = 0, b = 0;
};

struct Colors {
    ImColor skeletoncol = ImColor(255, 255, 255, 255);
    ImColor skeleton1 = ImColor(255, 0, 0, 255);
};

struct colors_t {
    rgb_t wave = { 255, 0, 0 };
    ImColor skeleton = ImColor(1.0f, 1.0f, 1.0f, 1.0f);
    ImColor box = ImColor(1.0f, 1.0f, 1.0f, 1.0f);
    ImColor corner = ImColor(1.0f, 1.0f, 1.0f, 1.0f);
    ImColor Distance = ImColor(255, 0, 0, 255);
    ImColor crosshair = ImColor(1.0f, 1.0f, 1.0f, 1.0f);
    ImColor Name = ImColor(255, 0, 0, 255);
    ImColor Kills = ImColor(255, 0, 0, 255);
    ImColor Platform = ImColor(255, 255, 255, 255);
    ImColor fill = ImColor(0.06f, 0.06f, 0.06f, 0.6f);
    ImColor curved = ImColor(1.0f, 1.0f, 1.0f, 1.0f);
    ImColor visible = ImColor(255, 0, 0, 255);


    static rgb_t hueToRGB(float hue) {
        float r = fabs(hue * 6.0f - 3.0f) - 1.0f;
        float g = 2.0f - fabs(hue * 6.0f - 2.0f);
        float b = 2.0f - fabs(hue * 6.0f - 4.0f);

        r = std::clamp(r, 0.0f, 1.0f);
        g = std::clamp(g, 0.0f, 1.0f);
        b = std::clamp(b, 0.0f, 1.0f);

        return rgb_t{ static_cast<int>(r * 255), static_cast<int>(g * 255), static_cast<int>(b * 255) };
    }

    void update(float speed = 0.001f) {
        static float hue = 0.f;
        hue += speed;
        if (hue > 1.f) hue = 0.f;
        wave = hueToRGB(hue);
    }
};

inline colors_t colors;
inline Colors old_colors;