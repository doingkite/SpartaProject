#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpartaCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class SPARTAPROJECT_API ASpartaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	ASpartaCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Health")
	float Health;

	virtual float TakeDamage
	(
		float DamaageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser
	) override;


	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category =" Movement")
	float NormalSpeed;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category="Movement")
	float SprintSpeedMultiplier;

	UPROPERTY(VIsibleAnyWhere, BlueprintReadOnly, Category="Movement")
	float SprintSpeed;

	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);

	void OnDeath();
};
