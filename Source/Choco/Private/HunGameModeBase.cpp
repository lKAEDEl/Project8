#include "HunGameModeBase.h"
#include "HunCharacter.h"
#include "HunPlayerController.h"
#include "HunGameStateBase.h"

AHunGameModeBase::AHunGameModeBase()
{
	DefaultPawnClass = AHunGameModeBase::StaticClass();
	PlayerControllerClass = AHunPlayerController::StaticClass();
	GameStateClass = AHunGameStateBase::StaticClass();
}
