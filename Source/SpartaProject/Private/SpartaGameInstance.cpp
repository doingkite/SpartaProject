#include "SpartaGameInstance.h"

USpartaGameInstance::USpartaGameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;

}

//���� ���� �����͸� �����ϰ� �����ϴ� �ݰ� ����
void USpartaGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Total Score Updated: %d"), TotalScore);
}
