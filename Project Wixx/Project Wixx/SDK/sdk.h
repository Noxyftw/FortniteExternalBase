#pragma once
#include <d3d9.h>
#include <vector>
#include <numbers>
#include <immintrin.h>
#include "Offsets.h"
#include <algorithm>
#include <mutex>
#include <shared_mutex>
#include <d3d9.h>
#include <vector>
#include <numbers>
#include <immintrin.h>
#include <algorithm>
#include <mutex>
#include <shared_mutex>
#include <string>
#define Noxyftw_PI 3.14159265358979323846264338327950288419716939937510


inline int width = GetSystemMetrics(SM_CXSCREEN);
inline int height = GetSystemMetrics(SM_CYSCREEN);
inline int screen_center_x1 = width / 2;
inline int screen_center_y1 = height / 2;

class FRotator {
public:
	float m_pitch;
	float m_yaw;
	float m_roll;

	FRotator() : m_pitch(0), m_yaw(0), m_roll(0) {}
	FRotator(float pitch, float yaw, float roll)
		: m_pitch(pitch), m_yaw(yaw), m_roll(roll) {
	}
};

class Vector2
{
public:
	Vector2() : x(0.f), y(0.f) {}
	Vector2(double _x, double _y) : x(_x), y(_y) {}
	~Vector2() {}
	double x, y;
};
template< typename t >

class TArray
{
public:
	TArray() : tData(), iCount(), iMaxCount() {}
	TArray(t* data, int count, int max_count) :
		tData(tData), iCount(iCount), iMaxCount(iMaxCount) {
	}

public:
	auto Get(int idx) -> t
	{
		return memory.read< t >(reinterpret_cast<__int64>(this->tData) + (idx * sizeof(t)));
	}

	auto Size() -> std::uint32_t
	{
		return this->iCount;
	}

	bool IsValid()
	{
		return this->iCount != 0;
	}

	t* tData;
	int iCount;
	int iMaxCount;
	std::uintptr_t Array;
	std::uint32_t Count;
	std::uint32_t MaxCount;

	t Get(std::uint32_t Index) const {
		if (Index >= Count) {
			return t();
		}
		return memory.read<t>(Array + (Index * sizeof(t)));
	}

	t operator[](std::uint32_t Index) const {
		return Get(Index);
	}

	std::uint32_t size() const {
		return Count;
	}

	bool isValid() const {
		return Array && Count <= MaxCount && MaxCount <= 1000000;
	}

	std::uintptr_t getAddress() const {
		return Array;
	}
};
template<class T>
class FArray
{
public:
	int getLength() const
	{
		return count;
	}

	int getIdentifier()
	{
		return data * count * max;
	}

	bool isValid() const
	{
		if (count > max)
			return false;
		if (!data)
			return false;
		return true;
	}

	uint64_t getAddress() const
	{
		return data;
	}

	T operator [](size_t idx) const
	{
		return memory.read<T>(data + sizeof(T) * idx);
	}

protected:
	uint64_t data;
	uint64_t count;
	uint64_t max;
};

class Vector3
{
public:
	Vector3() : x(0.f), y(0.f), z(0.f) {}
	Vector3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
	~Vector3() {}

	double x, y, z;

	inline double dot(Vector3 v) { return x * v.x + y * v.y + z * v.z; }

	inline double distance(Vector3 v) {
		return sqrt((v.x - x) * (v.x - x) + (v.y - y) * (v.y - y) + (v.z - z) * (v.z - z));
	}

	Vector3 operator-(Vector3 v) {
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	float Length() const {
		return sqrtf(static_cast<float>(x * x + y * y + z * z));
	}
};

struct Camera {
	Vector3 location;
	Vector3 rotation;
	float fov;
};

namespace cache {
	inline uintptr_t uworld;
	inline uintptr_t pawn_private;
	inline uintptr_t game_instance;
	inline uintptr_t health_set;
	inline Vector2 target;
	inline uintptr_t target_entity;
	inline uintptr_t local_players;
	inline uintptr_t player_controller;
	inline uintptr_t local_pawn;
	inline uintptr_t root_component;
	inline uintptr_t player_state;
	inline Vector3 relative_location;
	inline Vector3 plocaldistance;
	inline Vector3 localRelativeLocation;
	inline uintptr_t closest_pawn;
	inline int my_team_id;
	inline uintptr_t game_state;
	inline uintptr_t player_array;
	inline int player_count;
	inline float closest_distance;
	inline uintptr_t closest_mesh;
	inline Vector2 closest_mesh_line;
	inline float health;
	inline uintptr_t closest_aactor;
	inline uintptr_t current_weapon;
	inline uintptr_t persistent_level;
	inline uintptr_t actor_root;
	inline float speed;
	inline float pred_distance;
	inline Vector3 velocity;
	inline float gravity;
	inline Camera local_camera;
	inline Vector3 hitbox;
	inline float distance;
	inline uintptr_t Localpawn;


}
class structs
{
public:



	uintptr_t WeaponData;
	uintptr_t UWorld;
	uintptr_t GameInstance;
	uintptr_t GameState;
	uintptr_t LocalPlayer;
	uintptr_t AcknownledgedPawn;
	uintptr_t PlayerState;
	Vector3 relative_location;
	uintptr_t PlayerController;
	uintptr_t RootComponent;
	uintptr_t Mesh;
	uintptr_t PlayerArray;
	uintptr_t LocalWeapon;
	uintptr_t LocalVehicle;
	uintptr_t target_entity;
	uintptr_t pawnprivate;
	uintptr_t Localpawn;
	uintptr_t CurrentWeapon;
	float Server;

	int32_t
		AmmoCount;

	int
		TeamIndex,
		PlayerArraySize;


}; structs CachePointers;


namespace udata {

	struct world
	{
		uintptr_t gworld;
		uintptr_t owning_game_instance;
		uintptr_t local_player;
		uintptr_t player_controller;
		uintptr_t player_state;
		uintptr_t local_pawn;
		uintptr_t root_component;
		uintptr_t Spectator;


		int team_id;

		uintptr_t game_state;
		uintptr_t mesh;
		uintptr_t closeest_actor;
		std::vector<uintptr_t> player_array;
	};

	struct actor
	{
		uintptr_t player_state;
		uintptr_t pawn_private;
		uintptr_t mesh;

		int team_id;
		int kill_score;

		uintptr_t character_movement;
		uintptr_t current_weapon;

		bool knocked;
		bool visibility;

		actor(uintptr_t ps, uintptr_t pawn, uintptr_t m, int tid)
			: player_state(ps), pawn_private(pawn), mesh(m), team_id(tid),
			kill_score(0), character_movement(0), current_weapon(0),
			knocked(false), visibility(false) {
		}
	};

	struct environment
	{
		uintptr_t actor;
		std::string name;
		bool is_pickup;
		float distance;
	};

	inline std::vector<environment> environment_list;
	inline std::vector<environment> temp_environment_list;

	inline world world_t;
	inline std::vector<actor> actor_t;
}
using item = udata::environment;




struct FQuat { double x, y, z, w; };
struct FTransform
{
	FQuat rotation;
	Vector3 translation;
	uint8_t pad1c[0x8];
	Vector3 scale3d;
	uint8_t pad2c[0x8];

	D3DMATRIX to_matrix_with_scale()
	{
		D3DMATRIX m{};

		const Vector3 Scale
		(
			(scale3d.x == 0.0) ? 1.0 : scale3d.x,
			(scale3d.y == 0.0) ? 1.0 : scale3d.y,
			(scale3d.z == 0.0) ? 1.0 : scale3d.z
		);

		const double x2 = rotation.x + rotation.x;
		const double y2 = rotation.y + rotation.y;
		const double z2 = rotation.z + rotation.z;
		const double xx2 = rotation.x * x2;
		const double yy2 = rotation.y * y2;
		const double zz2 = rotation.z * z2;
		const double yz2 = rotation.y * z2;
		const double wx2 = rotation.w * x2;
		const double xy2 = rotation.x * y2;
		const double wz2 = rotation.w * z2;
		const double xz2 = rotation.x * z2;
		const double wy2 = rotation.w * y2;

		m._41 = translation.x;
		m._42 = translation.y;
		m._43 = translation.z;
		m._11 = (1.0f - (yy2 + zz2)) * Scale.x;
		m._22 = (1.0f - (xx2 + zz2)) * Scale.y;
		m._33 = (1.0f - (xx2 + yy2)) * Scale.z;
		m._32 = (yz2 - wx2) * Scale.z;
		m._23 = (yz2 + wx2) * Scale.y;
		m._21 = (xy2 - wz2) * Scale.y;
		m._12 = (xy2 + wz2) * Scale.x;
		m._31 = (xz2 + wy2) * Scale.z;
		m._13 = (xz2 - wy2) * Scale.x;
		m._14 = 0.0f;
		m._24 = 0.0f;
		m._34 = 0.0f;
		m._44 = 1.0f;

		return m;
	}
};

D3DMATRIX matrix_multiplication(D3DMATRIX pm1, D3DMATRIX pm2)
{
	D3DMATRIX pout{};
	pout._11 = pm1._11 * pm2._11 + pm1._12 * pm2._21 + pm1._13 * pm2._31 + pm1._14 * pm2._41;
	pout._12 = pm1._11 * pm2._12 + pm1._12 * pm2._22 + pm1._13 * pm2._32 + pm1._14 * pm2._42;
	pout._13 = pm1._11 * pm2._13 + pm1._12 * pm2._23 + pm1._13 * pm2._33 + pm1._14 * pm2._43;
	pout._14 = pm1._11 * pm2._14 + pm1._12 * pm2._24 + pm1._13 * pm2._34 + pm1._14 * pm2._44;
	pout._21 = pm1._21 * pm2._11 + pm1._22 * pm2._21 + pm1._23 * pm2._31 + pm1._24 * pm2._41;
	pout._22 = pm1._21 * pm2._12 + pm1._22 * pm2._22 + pm1._23 * pm2._32 + pm1._24 * pm2._42;
	pout._23 = pm1._21 * pm2._13 + pm1._22 * pm2._23 + pm1._23 * pm2._33 + pm1._24 * pm2._43;
	pout._24 = pm1._21 * pm2._14 + pm1._22 * pm2._24 + pm1._23 * pm2._34 + pm1._24 * pm2._44;
	pout._31 = pm1._31 * pm2._11 + pm1._32 * pm2._21 + pm1._33 * pm2._31 + pm1._34 * pm2._41;
	pout._32 = pm1._31 * pm2._12 + pm1._32 * pm2._22 + pm1._33 * pm2._32 + pm1._34 * pm2._42;
	pout._33 = pm1._31 * pm2._13 + pm1._32 * pm2._23 + pm1._33 * pm2._33 + pm1._34 * pm2._43;
	pout._34 = pm1._31 * pm2._14 + pm1._32 * pm2._24 + pm1._33 * pm2._34 + pm1._34 * pm2._44;
	pout._41 = pm1._41 * pm2._11 + pm1._42 * pm2._21 + pm1._43 * pm2._31 + pm1._44 * pm2._41;
	pout._42 = pm1._41 * pm2._12 + pm1._42 * pm2._22 + pm1._43 * pm2._32 + pm1._44 * pm2._42;
	pout._43 = pm1._41 * pm2._13 + pm1._42 * pm2._23 + pm1._43 * pm2._33 + pm1._44 * pm2._43;
	pout._44 = pm1._41 * pm2._14 + pm1._42 * pm2._24 + pm1._43 * pm2._34 + pm1._44 * pm2._44;
	return pout;
}

D3DMATRIX to_matrix(Vector3 rot, Vector3 origin = Vector3(0, 0, 0))
{
	float radpitch = (rot.x * Noxyftw_PI / 180);
	float radyaw = (rot.y * Noxyftw_PI / 180);
	float radroll = (rot.z * Noxyftw_PI / 180);
	float sp = sinf(radpitch);
	float cp = cosf(radpitch);
	float sy = sinf(radyaw);
	float cy = cosf(radyaw);
	float sr = sinf(radroll);
	float cr = cosf(radroll);
	D3DMATRIX matrix{};
	matrix.m[0][0] = cp * cy;
	matrix.m[0][1] = cp * sy;
	matrix.m[0][2] = sp;
	matrix.m[0][3] = 0.f;
	matrix.m[1][0] = sr * sp * cy - cr * sy;
	matrix.m[1][1] = sr * sp * sy + cr * cy;
	matrix.m[1][2] = -sr * cp;
	matrix.m[1][3] = 0.f;
	matrix.m[2][0] = -(cr * sp * cy + sr * sy);
	matrix.m[2][1] = cy * sr - cr * sp * sy;
	matrix.m[2][2] = cr * cp;
	matrix.m[2][3] = 0.f;
	matrix.m[3][0] = origin.x;
	matrix.m[3][1] = origin.y;
	matrix.m[3][2] = origin.z;
	matrix.m[3][3] = 1.f;
	return matrix;
}



struct FNRot
{
	double a;
	char pad_0008[24];
	double b;
	char pad_0028[424];
	double c;
};


inline double RadiansToDegrees(double dRadians)
{
	return dRadians * (180.0 / Noxyftw_PI);
}
struct FPlane : public Vector3
{
	double W;
};


struct smat4_f64 {
	__m256d col0, col1, col2, col3;

	struct vec4d { double x, y, z, w; };

	vec4d transform(const Vector3& v) const noexcept {
		const __m256d vx = _mm256_set1_pd(v.x);
		const __m256d vy = _mm256_set1_pd(v.y);
		const __m256d vz = _mm256_set1_pd(v.z);

		__m256d result = _mm256_mul_pd(vx, col0);
		result = _mm256_fmadd_pd(vy, col1, result);
		result = _mm256_fmadd_pd(vz, col2, result);
		result = _mm256_add_pd(result, col3);

		vec4d out;
		_mm256_storeu_pd(reinterpret_cast<double*>(&out), result);
		return out;
	}
};

struct mat4_d64 {
	double m[4][4]{};

	smat4_f64 to_simd() const noexcept {
		return smat4_f64{
			_mm256_loadu_pd(&m[0][0]),
			_mm256_loadu_pd(&m[1][0]),
			_mm256_loadu_pd(&m[2][0]),
			_mm256_loadu_pd(&m[3][0])
		};
	}
};


struct world_cached_view_info {
	mat4_d64 view_matrix;
	mat4_d64 projection_matrix;
	mat4_d64 view_projection_matrix;
	mat4_d64 view_to_world;
};

struct ue_tarray_header {
	uintptr_t Data;
	int32_t   Count;
	int32_t   Max;
};

inline smat4_f64 g_world_to_clip{};


Vector2 project_world_to_screen(Vector3 world_location)
{
	if (world_location.x == 0 && world_location.y == 0 && world_location.z == 0)
		return {};

	const auto clip = g_world_to_clip.transform(world_location);
	if (clip.w <= 0.0)
		return {};

	const auto rhw = 1.0 / clip.w;

	return Vector2(
		(clip.x * rhw + 1.0) * 0.5 * width,
		(1.0 - clip.y * rhw) * 0.5 * height
	);
}

Camera get_camera()
{
	static Camera last_valid{};
	Camera vp{};

	if (!CachePointers.UWorld) return last_valid;

	auto tarray = memory.read<ue_tarray_header>(CachePointers.UWorld + offsets::Camera::CachedViewInfoRenderedLastFrame);
	if (tarray.Count > 0 && tarray.Data) {
		auto info = memory.read<world_cached_view_info>(tarray.Data);


		if (info.view_to_world.m[3][0] == 0.0 && info.view_to_world.m[3][1] == 0.0 && info.view_to_world.m[3][2] == 0.0)
			return last_valid;


		vp.location.x = info.view_to_world.m[3][0];
		vp.location.y = info.view_to_world.m[3][1];
		vp.location.z = info.view_to_world.m[3][2];


		double pitch = std::asin(std::clamp(info.view_to_world.m[0][2], -1.0, 1.0));
		double yaw = std::atan2(info.view_to_world.m[0][1], info.view_to_world.m[0][0]);
		vp.rotation.x = RadiansToDegrees(pitch);
		vp.rotation.y = RadiansToDegrees(yaw);
		vp.rotation.z = 0.0;


		vp.fov = static_cast<float>(2.0 * std::atan(1.0 / info.projection_matrix.m[0][0]) * (180.0 / Noxyftw_PI));


		g_world_to_clip = info.view_projection_matrix.to_simd();
		last_valid = vp;
	}
	else {
		return last_valid;
	}

	return vp;
}




Vector3 getplayerbone(uintptr_t mesh, int bone_id)
{
	uintptr_t bone_array = memory.read<uintptr_t>(mesh + offsets::Core::BoneArray);
	if (bone_array == 0) bone_array = memory.read<uintptr_t>(mesh + offsets::Core::BoneArray + 0x10);
	FTransform bone = memory.read<FTransform>(bone_array + (bone_id * 0x60));
	FTransform component_to_world = memory.read<FTransform>(mesh + offsets::Core::ComponentToWorld);
	D3DMATRIX matrix = matrix_multiplication(bone.to_matrix_with_scale(), component_to_world.to_matrix_with_scale());
	return Vector3(matrix._41, matrix._42, matrix._43);
}





static float powf_(float _X, float _Y)
{
	return (_mm_cvtss_f32(_mm_pow_ps(_mm_set_ss(_X), _mm_set_ss(_Y))));
}

static double get_cross_distance(double x1, double y1, double x2, double y2) {
	return sqrtf(powf((x2 - x1), 2) + powf_((y2 - y1), 2));
}

std::string WStringToUTF8(const wchar_t* lpwcszWString)
{
	char* pElementText;
	int iTextLen = ::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, NULL, 0, NULL, NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, (iTextLen + 1) * sizeof(char));
	::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, pElementText, iTextLen, NULL, NULL);
	std::string strReturn(pElementText);
	delete[] pElementText;
	return strReturn;
}
std::wstring MBytesToWString(const char* lpcszString)
{
	int len = strlen(lpcszString);
	int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, NULL, 0);
	wchar_t* pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, (LPWSTR)pUnicode, unicodeLen);
	std::wstring wString = (wchar_t*)pUnicode;
	delete[] pUnicode;
	return wString;
}

bool IsValidIngamePawn(uintptr_t pawn)
{
	if (!pawn) return false;
	float reviveTime = memory.read<float>(pawn + 0x4920);
	return reviveTime == 10.f;
}

inline void DrawString(float fontSize, int x, int y, ImColor color, bool bCenter, bool stroke, const char* pText, ...)
{
	va_list va_alist;
	char buf[128] = { 0 };
	va_start(va_alist, pText);
	_vsnprintf_s(buf, sizeof(buf), pText, va_alist);
	va_end(va_alist);
	std::string text = WStringToUTF8(MBytesToWString(buf).c_str());
	if (bCenter)
	{
		ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
		x = x - textSize.x / 4;
		y = y - textSize.y;
	}
	if (stroke)
	{
		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x - 1, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 1, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x - 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
	}
	ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x, y), ImColor(color), text.c_str());
}

auto is_visible(std::uintptr_t actor) -> bool
{
	auto proxy = memory.read<std::uintptr_t>(actor + 0xB0);

	if (!proxy)
		return false;

	auto last_render = memory.read<float>(proxy + 0x7DC);
	auto tolerance = memory.read<float>(actor + 0x330);
	auto last_submit = memory.read<double>(proxy + 0x7B8);

	return std::fmax(0.f, last_render + 0.0001f)
		>= last_submit - static_cast<double>(tolerance);
}

inline std::string GetPlayerName(uintptr_t playerState) {
    if (!playerState) return "( AI )";

    auto Name = memory.read<uintptr_t>(playerState + 0x9E8); // PLAYERNAME
    if (!Name) return "( AI )";

    auto length = memory.read<int>(Name + 0x10);
    auto v6 = (__int64)length;

    if (length <= 0 || length > 255) return "( AI )";

    auto FText = (uintptr_t)memory.read<__int64>(Name + 0x8);
    if (!FText) return "( AI )";

    wchar_t* Buffer = new wchar_t[length + 1] { 0 };
    driver.readphys(reinterpret_cast<PVOID>(FText), Buffer, length * sizeof(wchar_t));

    char v21;
    int v22;
    int i;
    int v25;
    UINT16* v23;

    v21 = (char)(v6 - 1);
    if (!(UINT32)v6)
        v21 = 0;
    v22 = 0;
    v23 = (UINT16*)Buffer;
    for (i = (v21) & 3; ; *v23++ += i & 7)
    {
        v25 = (int)(v6 - 1);
        if (!(UINT32)v6)
            v25 = 0;
        if (v22 >= v25)
            break;
        i += 3;
        ++v22;
    }

    std::wstring PlayerName{ Buffer };
    delete[] Buffer;

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, PlayerName.c_str(), (int)PlayerName.size(), nullptr, 0, nullptr, nullptr);
    if (size_needed <= 0) {
        return std::string(PlayerName.begin(), PlayerName.end());
    }

    std::string result(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, PlayerName.c_str(), (int)PlayerName.size(), &result[0], size_needed, nullptr, nullptr);
    return result;
}

inline std::string GetPlayerNameLobby(uintptr_t playerState) {
    if (!playerState) return "( AI )";

    auto Name = memory.read<uintptr_t>(playerState + 0x308);
    if (!Name) return "( AI )";

    auto length = memory.read<int>(Name + 0x10);
    auto v6 = (__int64)length;

    if (length <= 0 || length > 255) return "( AI )";

    auto FText = (uintptr_t)memory.read<__int64>(Name + 0x8);
    if (!FText) return "( AI )";

    wchar_t* Buffer = new wchar_t[length + 1] { 0 };
    driver.readphys(reinterpret_cast<PVOID>(FText), Buffer, length * sizeof(wchar_t));

    char v21;
    int v22;
    int i;
    int v25;
    UINT16* v23;

    v21 = (char)(v6 - 1);
    if (!(UINT32)v6)
        v21 = 0;
    v22 = 0;
    v23 = (UINT16*)Buffer;
    for (i = (v21) & 3; ; *v23++ += i & 7)
    {
        v25 = (int)(v6 - 1);
        if (!(UINT32)v6)
            v25 = 0;
        if (v22 >= v25)
            break;
        i += 3;
        ++v22;
    }

    std::wstring PlayerName{ Buffer };
    delete[] Buffer;

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, PlayerName.c_str(), (int)PlayerName.size(), nullptr, 0, nullptr, nullptr);
    if (size_needed <= 0) {
        return std::string(PlayerName.begin(), PlayerName.end());
    }

    std::string result(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, PlayerName.c_str(), (int)PlayerName.size(), &result[0], size_needed, nullptr, nullptr);
    return result;
}
