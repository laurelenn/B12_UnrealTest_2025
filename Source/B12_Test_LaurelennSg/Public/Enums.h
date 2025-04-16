#pragma once


// Enum game states

UENUM(BlueprintType)
enum class ECaptureGameState : uint8
{
	Initialization  UMETA(DisplayName = "Initialization"),
	ReadyToStart UMETA(DisplayName = "Ready To Start"),
	Playing UMETA(DisplayName = "Playing"),
	Paused UMETA(DisplayName = "Paused"),
	GameOver UMETA(DisplayName = "Game Over"),
	ReadyToRestart UMETA(DisplayName = "Ready To Restart")
};

UENUM(BlueprintType)
enum class ECaptureGameOverReason : uint8
{
	AllPreysCaptured  UMETA(DisplayName = "All Preys Captured"),
	TimeOver UMETA(DisplayName = "Time Over"),
	PlayerDeath UMETA(DisplayName = "Player Death"),
	ArenaWavesOver UMETA(DisplayName = "Arena Waves Over")
};

UENUM(BlueprintType)
enum class EArenaWaveEndCondition : uint8
{
	TimerOnly UMETA(DisplayName = "Timer Only"),
	AllWavePreysCaptured UMETA(DisplayName = "All Preys Wave Captured"),
	// Can add conditions with the evolution of the gameplay (boss, enemies, cinematics, tutorials...)
};

UENUM(BlueprintType)
enum class EArenaEndCondition : uint8
{
	TimerOnly UMETA(DisplayName = "Timer Only"),
	AllPreysCaptured UMETA(DisplayName = "All Preys Arena Captured"),
	// Can add conditions with the evolution of the gameplay (boss, enemies, cinematics, tutorials...)
};

UENUM(BlueprintType)
enum class EAIPreyState : uint8
{
	Idle,
	Flee,
	Held,
	Captured,
	Dead
};

UENUM(BlueprintType)
enum class EAIEnemyState : uint8
{
	Idle,
	Hunting,
	Dead
};