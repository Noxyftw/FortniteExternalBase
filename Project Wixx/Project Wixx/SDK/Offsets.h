#pragma once
#include <cstdint>
#include "../Driver/Driver.hxx"

namespace offsets {

	namespace World {

		uintptr_t Gworld = 0x1B2C5BA0; // UWorld
		uintptr_t GEngine = 0x1B2C7518; // UWorld

		uintptr_t GNames = 0x1B179F00;  // ANoxyftwRandom

		uintptr_t GObjects = 0x1B1D3640;
		uintptr_t GObjectsCount = 0x1B1D365C; // ANoxyftwRandom 
		uintptr_t StaticFindObject = 0x8141B8; // ANoxyftwRandom // NOT UPDATED!!!
		uintptr_t StaticLoadObject = 0x3C16E; // ANoxyftwRandom


		uintptr_t ProcessEvent = 0xEEFC4; // ANoxyftwRandom // NOT UPDATED!!!
		uintptr_t BoneMatrix = 0x4F6CF0; // ANoxyftwRandom
		uintptr_t FNameToString = 0x4485C; // ANoxyftwRandom

	}

	namespace Core {

		uintptr_t LocalPlayers = 0x38; // UGameInstance
		uintptr_t PlayerController = 0x30; // APlayerController
		uintptr_t PlayerCameraManager = 0x328; // APlayerController
		uintptr_t AcknowledgedPawn = 0x318; // APlayerController
		uintptr_t PlayerState = 0x290; // APawn
		uintptr_t TeamIndex = 0xf69; // AFortPlayerStateAthena
		uintptr_t bIsDying = 0x728; // FortPawn
		uintptr_t bIsDBNO = 0x881; // FortPawn
		uintptr_t bIsABot = 0x27A; // APlayerState


		uintptr_t ActorArray = 0x40; // ANoxyftwRandom

		uintptr_t Platform = 0x400; // AFortPlayerState
		uintptr_t TargetedFortPawn = 0x16C0; // FortPlayerController
		uintptr_t RankedProgress = 0xD8; // FortPlayerStateComponent_Habanero
		uintptr_t PlayerAimOffset = 0x2330; // FortPlayerController


		uintptr_t OwningGameInstance = 0x238; // UWorld
		uintptr_t GameState = 0x1C0; // UWorld 
		uintptr_t PlayerArray = 0x288; // AGameStateBase
		uintptr_t NetConnection = 0x4A8;
		uintptr_t RotationInput = NetConnection + 0x8;

		uintptr_t PersistentLevel = 0x38; // UWorld
		uintptr_t Levels = 0x1D8; // UWorld

		uintptr_t RootComponent = 0x1B0; // Actor
		uintptr_t PawnPrivate = 0x2E8; // APlayerState
		uintptr_t MoveIgnoreActors = 0x328; // UPrimitiveComponent

		uintptr_t Killscore = 0xf80; // FortPlayerStateAthena
		uintptr_t Mesh = 0x2F0; // USkeletalMeshComponent
		uintptr_t BoneArray = 0x660; // ANoxyftwRandom
		uintptr_t BoneCache = BoneArray + 0x10; // ANoxyftwRandom
		uintptr_t MeshDeformerInstances = 0x630; // USkinnedMeshComponent

		uintptr_t ComponentToWorld = 0x1E0; // USceneComponent
		uintptr_t RelativeLocation = 0x140; // USceneComponent
		uintptr_t RelativeRotation = 0x158; // USceneComponent
		uintptr_t RelativeScale3D = 0x170; // USceneComponent
		uintptr_t ComponentVelocity = 0x188; // USceneComponent 
		uintptr_t AdditionalAimOffset = 0x2300; // FortPlayerController
		uintptr_t LastRenderTime = 0x330; // USceneComponent
		uintptr_t LocationUnderReticle = 0x21A0; // FortPlayerController
		uintptr_t Spectators = 0xAC0;
		uintptr_t SpectatorArray = 0x1838;

	}

	namespace Lobby {
		uintptr_t Actors = 0x1A0;
	}

	namespace Camera {


		uintptr_t CameraLocation = 0x168; // ANoxyftwCamera
		uintptr_t CameraRotation = CameraLocation + 0x10; // ANoxyftwCamera
		uintptr_t CameraFOV = 0x374; // ANoxyftwCamera
		uintptr_t CachedViewInfoRenderedLastFrame = 0x158 + 0x20; // ANoxyftwCamera

	}

	namespace Vehicle {


		uintptr_t CurrentVehicle = 0x2C30; // FortPlayerPawn

	}

	namespace Fov {
		uintptr_t FOVMinimum = 0x2258; // FortPlayerController
		uintptr_t FOVMaximum = 0x225C; // FortPlayerController
	}

	namespace Weapon {





		uintptr_t CurrentWeapon = 0x9d0; // FortPawn
		uintptr_t WeaponData = 0x638; // FortWeapon
		uintptr_t ItemName = 0x38; // UItemDefinitionBase

		uintptr_t CurrentReloadDuration = 0x1098; // FortWeapon

		uintptr_t WeaponOffsetCorrection = 0x2360; // FortPlayerController
		uintptr_t AmmoCount = 0x1100; // FortWeapon
		uintptr_t bIsReloadingWeapon = 0x371; // FortWeapon
		uintptr_t ReloadAnimation = 0x1610; // FortWeapon
		uintptr_t LWProjectile_ActivateRemovedTimestamp = 0x2748; // FortWeaponRanged



		uintptr_t ProjectileSpeed = 0x210C;  // 
		uintptr_t ProjectileGravity = ProjectileSpeed + 0x4; // 



		uintptr_t ServerWorldTimeSecondsDelta = 0x2A8; // AGameStateBase

		uintptr_t LastFireTimeVerified = 0x10F4; // FortWeapon
		uintptr_t WeaponCoreAnimation = 0x1660; // FortWeapon

		uintptr_t PrimaryPickupItemEntry = 0x368; // FortPickup

		uintptr_t ItemType = 0xA0; // EFortItemType
		uintptr_t PrimaryAssetOverride = 0xA9; //
		uintptr_t ItemRarity = 0xAA; //
		uintptr_t bAlreadySearched = 0xCE2; // ABuildingContainer

	}

	namespace Habanero {

		uintptr_t HabaneroComponent = 0x918; // FortPlayerState

	}


}





























// Made By Noxyftw