#pragma once
#include "..\..\Utils\Memory\Memory.h"
#include "..\..\Utils\Config\Config.h"
#include "..\..\Utils\SimpleMath\SimpleMath.h"
using namespace DirectX::SimpleMath;

struct AxisAlignedBox
{
    Vector4 Min;
    Vector4 Max;
};

// offsets
namespace offset
{
	// Main
	constexpr auto ObfuscationMgr = 0x14351D058;
	constexpr auto ClientGameContext = 0x1437F7758;
	constexpr auto PlayerManager = 0x68;
	constexpr auto GameRender = 0x1439e6d08;

	// ClientPlayer 
	constexpr auto PlayerName = 0x40;
	constexpr auto TeamID = 0x1C34;
	constexpr auto ClientVehicleEntity = 0x1D38;
	constexpr auto ClientSoldierEntity = 0x1D48;

	// ClientVehicleEntity 
	constexpr auto EntityData = 0x30; // Data->VehicleCurrntHealth

	// ClientSoldierEntity 
	constexpr auto healthcomponent = 0x01D0;
	constexpr auto bonecollisioncomponent = 0x0490;
	constexpr auto authorativeYaw = 0x0604;
	constexpr auto poseType = 0x638;
	constexpr auto occluded = 0x6EB;
	constexpr auto location = 0x990;
}

class CPlayer
{
public:
    uint64_t ClientPlayer;
    uint64_t ClientSoldier;
    uint64_t ClientVehicle;
    uintptr_t pQuat;

    int Team;
    float Health;
    float MaxHealth;
    Vector3 Position;
    bool Occlude;
    std::string Name;

    void GetPlayer(int id);
    bool IsValid();
    bool InVehicle();
    bool IsSpectator();
    bool IsVisible();
    bool IsDead();
    bool Update();
    Vector3 GetBonePositionByID(int ID);
    std::string GetName();
    AxisAlignedBox GetAABB();
};

extern void UpdateData();
extern uint64_t EncryptedPlayerMgr_GetPlayer(uint64_t ptr, int id);
extern uint64_t GetPlayerById(int id);
extern bool WorldToScreen(const Vector3& vIn, Vector2& flOut); // Vector2
extern float GetDistance(Vector3 value1, Vector3 value2);

enum BONES
{
    HEAD = 0x35,
    NECK = 0x33,
    SPINE2 = 0x7,
    SPINE1 = 0x6,
    SPINE = 0x5,
    LEFTSHOULDER = 0x8,
    RIGHTSHOULDER = 0xA3,
    LEFTELBOWROLL = 0xE, // ひじ
    RIGHTELBOWROLL = 0xA9, // ひじ
    LEFTHAND = 0x10,
    RIGHTHAND = 0xAB,
    LEFTKNEEROLL = 0x11F, // ひざ
    RIGHTKNEEROLL = 0x12D, // ひざ
    LEFTFOOT = 0x115,
    RIGHTFOOT = 0x123
};

/*
class ClientGameContext
{
public:
    char pad_0x0000[0x68]; //0x0000
    ClientPlayerManager* m_clientPlayerManager; //0x0068
    static ClientGameContext* GetInstance() {
        return *(ClientGameContext**)(OFFSET_CLIENTGAMECONTEXT);
    }
};

class HealthComponent
{
public:
    char pad_0000[32]; //0x0000
    float m_Health; //0x0020
    float m_MaxHealth; //0x0024
    char pad_0028[24]; //0x0028
    float m_VehicleHealth; //0x0040
    char pad_0044[4092]; //0x0044
}; //Size: 0x1040

class ClientSoldierEntity 
{
public:
    char pad_0000[464]; //0x0000
    HealthComponent* healthcomponent; //0x01D0
    char pad_01D8[696]; //0x01D8
    BoneCollisionComponent* bonecollisioncomponent; //0x0490
    char pad_0498[363]; //0x0498
    uint8_t N00000670; //0x0603
    float authorativeYaw; //0x0604
    char pad_0608[41]; //0x0608
    uint8_t N00000521; //0x0631
    char pad_0632[6]; //0x0632
    uint8_t poseType; //0x0638
    char pad_0639[176]; //0x0639
    uint8_t N00000538; //0x06E9
    uint8_t N0000022B; //0x06EA
    uint8_t occluded; //0x06EB
    char pad_06EC[669]; //0x06EC
    uint8_t N0000058C; //0x0989
    char pad_098A[6]; //0x098A
    Vector location; //0x0990
    char pad_099C[1712]; //0x099C
}; //Size: 0x104C

class ClientPlayer
{
public:
    virtual~ClientPlayer();
    virtual DWORD_PTR GetCharacterEntity(); //=> ClientSoldierEntity + 0x268
    virtual DWORD_PTR GetCharacterUserData(); //=> PlayerCharacterUserData
    virtual class EntryComponent* GetEntryComponent();
    virtual bool InVehicle();
    virtual unsigned int getId();
    char _0x0008[16];
    char* name; //0x0018
    char pad_0020[32]; //0x0020
    char szName[8]; //0x0040
    char pad_0048[7144]; //0x0048
    uint8_t N00000393; //0x1C30
    uint8_t N0000042C; //0x1C31
    char pad_1C32[2]; //0x1C32
    uint8_t teamId; //0x1C34
    char pad_1C35[259]; //0x1C35
    ClientVehicleEntity* clientVehicleEntity; //0x1D38
    char pad_1D40[8]; //0x1D40
    ClientSoldierEntity* clientSoldierEntity; //0x1D48
    char pad_1D50[736]; //0x1D50
};

class RenderView 
{
public:
    char pad_0x0000[0x320]; //0x0000
    Matrix4x4 m_viewMatrixInverse; //0x0320
    char pad_0x0360[0x100]; //0x0360
    Matrix4x4 viewProj; //0x0460
    char pad_0x04A0[0x28]; //0x04A0
}; //Size: 0x05C0

class GameRenderer
{
public:
    char pad_0000[96]; //0x0000
    class RenderView* renderView; //0x0060
    char pad_0068[4112]; //0x0068

    static GameRenderer* GetInstance() {
        return *(GameRenderer**)OFFSET_GAMERENDERER;
    }
}; //Size: 0x0088

class DxRenderer 
{
public:
    char pad_0x0000[0x810]; //0x0000
    __int32 m_frameCounter; //0x0810
    __int32 m_framesInProgress; //0x0814
    __int32 m_framesInProgress2; //0x0818
    unsigned char m_isActive; //0x081C
    char pad_0x081D[0x3]; //0x081D
    Screen* pScreen; //0x0820
    char pad_0x0828[0x78]; //0x0828
    ID3D11Device* m_pDevice; //0x08A0
    ID3D11DeviceContext* m_pContext; //0x08A8
    char pad_0x08B0[0x68]; //0x08B0
    char* m_AdapterName; //0x0918
    char pad_0x0920[0x28]; //0x0920

    static DxRenderer* GetInstance() {
        return *(DxRenderer**)(OFFSET_DXRENDERER);
    }
};

class Screen 
{
public:
    char pad_0x0000[0x5F]; //0x0000
    unsigned char m_isTopWindow; //0x005F
    unsigned char m_isMinimized; //0x0060
    unsigned char m_isMaximized; //0x0061
    unsigned char m_isResizing; //0x0062
    char pad_0x0063[0x5]; //0x0063
    __int32 m_width; //0x0068
    __int32 m_height; //0x006C
    char pad_0x0070[0x210]; //0x0070
    IDXGISwapChain* m_swapChain; //0x0280
    char pad_0x0288[0xB8]; //0x0288
}; //Size=0x0340
*/