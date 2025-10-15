#include "SpartaCharacter.h"
#include "SpartaPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ASpartaCharacter::ASpartaCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	NormalSpeed = 600.f;
	SprintSpeedMultiplier = 1.7f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	MaxHealth = 100.f;
	Health = MaxHealth;

}


void ASpartaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ASpartaPlayerController* PlayerController = Cast<ASpartaPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction //이벤트와 함수를 연결하는 핵심 코드
					(
					PlayerController->MoveAction, 
					ETriggerEvent::Triggered, 
					this, 
					&ASpartaCharacter::Move
					);
			}
			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction
					(
					PlayerController->JumpAction,
					ETriggerEvent::Started,
					this,
					&ASpartaCharacter::StartJump
					);

				EnhancedInput->BindAction
					(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&ASpartaCharacter::StopJump
					);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction
					(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::Look
					);
			}
			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction
					(
					PlayerController->SprintAction,
					ETriggerEvent::Started,
					this,
					&ASpartaCharacter::StartSprint
					);
				EnhancedInput->BindAction
					(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&ASpartaCharacter::StopSprint
					);
			}
		}
	}
}

void ASpartaCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}
void ASpartaCharacter::StartJump(const FInputActionValue& value)
{
	Jump();
}
void ASpartaCharacter::StopJump(const FInputActionValue& value)
{

	StopJumping();
	
}
void ASpartaCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void ASpartaCharacter::StartSprint(const FInputActionValue& value)
{
	if(GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}
void ASpartaCharacter::StopSprint(const FInputActionValue& value)
{
	if(GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

float ASpartaCharacter::GetHealth() const
{
	return Health;
}

void ASpartaCharacter::AddHealth(float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Health increased to: %f"), Health);
}

float ASpartaCharacter::TakeDamage
(
	float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser
)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health = FMath::Clamp(Health - ActualDamage, 0.f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Health decresead to: %f"), Health);

	if (Health <= 0.f)
	{
		OnDeath();
	}
	return ActualDamage;
}

void ASpartaCharacter::OnDeath()
{
	UE_LOG(LogTemp, Error, TEXT("Character is Dead!"));
}