#include "SpartaPlayerController.h"
#include "EnhancedInputSubsystems.h"

ASpartaPlayerController::ASpartaPlayerController()
	:
	InputMappingContext(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr),
	JumpAction(nullptr),
	SprintAction(nullptr)
{
}

void ASpartaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if(InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}








