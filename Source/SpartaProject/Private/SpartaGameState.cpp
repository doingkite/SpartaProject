#include "SpartaGameState.h"
#include "SpartaGameInstance.h"
#include "SpartaProject/SpartaPlayerController.h"
#include "SpartaProject/SpawnVolume.h"
#include "SpartaProject/CoinItem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

//���� ���� - BeginPlay() - StartLevel() - BasicLevel()

//�������� 40�� ������ ��ȯ
//���� �����۵� �� �Ծ��� ��� ���� ����
//30���� �ð��� �־����� Ÿ�ӿ����� �ŵ� �ٷ� ���� ����
//ü�� 0�� �Ǵ� ��� ���� ����

ASpartaGameState::ASpartaGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	//Startlevel()���� ������ �ҷ��ö����� �ϴ� �ʱ�ȭ���� �ٸ�
	LevelDuration = 30.f;
	CurrentLevelIndex = 0;
	MaxLevels = 3;
}

void ASpartaGameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();
	//ƽ�Լ�(�������Ʈ) ��ſ� ���
	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ASpartaGameState::UpdateHUD,//�ð��� ������ �� �Լ��� �ҷ���
		0.1f,
		true
	);
}

int32 ASpartaGameState::GetScore() const
{
	return Score;
}

// ���� �߰� ��û�� �����ϰ� �����ϴ� ��û â��
void ASpartaGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			SpartaGameInstance->AddToScore(Amount);
		}
	}
}

void ASpartaGameState::StartLevel()
{
	//������ ���µ� ������ ���� HUD UI ���ش�
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			SpartaPlayerController->ShowGameHUD();
		}
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			CurrentLevelIndex = SpartaGameInstance->CurrentLevelIndex;
		}
	} /* = > �ش� �ڵ� �డ ���� :
	  ������ �ٲ� ������ �ʱ�ȭ�Ǵ� ����(GameState ��)��, 
	  ���� ��ü���� �����ϰ� �����Ǵ� GameInstance�� ������ ����ȭ�ϱ� ����
	  */

	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	//���� ���忡�� �� ���Ϳ� �ش�Ǵ� ��� �� ��������, ã�� ������ FoundVolumes�� �־��ֱ�
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	const int32 ItemToSpawn = 40; //�������� 40���� ������ ��ȯ

	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			//FoundVolumes�� ù��° ��Ҹ� SpawnVolume���� Ÿ�Ժ�ȯ
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]); 
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}

	//StartLevel() ȣ�� �� �ð� �����
	GetWorldTimerManager().SetTimer
	(
		LevelTimerHandle, //FTimerHandle
		this,
		&ASpartaGameState::OnLevelTimeUp, //Ÿ�̸� �����ڸ��� ȣ������ �Լ�
		LevelDuration, //�� ������ �� �� ���� ������ �� ������
		false
	);
}

void ASpartaGameState::OnLevelTimeUp()
{
	EndLevel();
}

void ASpartaGameState::OnCoinCollected()
{
	CollectedCoinCount++;

	// CollectedCoinCount / SpawnedCoinCount ������ ���� �� ���� ����
	UE_LOG(LogTemp, Warning, TEXT("Coin Collected: %d / %d"),
		CollectedCoinCount,
		SpawnedCoinCount);

	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndLevel();
	}

}

void ASpartaGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	// StartLevel()�� ���������� GameIstance�� ����ȭ �ڵ� �߰�
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			//���� �߰� ����
			AddScore(Score);
			CurrentLevelIndex++;
			//StartLevel()���� ������ �ٸ� ����: StartLevel()�� **'�ε�(Load)'**�̰� EndLevel()�� **'����(Save)'**�̱� ������ �������� �帧 ������ ���ݴ��̴�.
			SpartaGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}

	//�ִ� ������ �����ߴ°�
	if (CurrentLevelIndex >=  MaxLevels)
	{
		OnGameOver();
		return;
	}
	// ���� ���� �� �̸��� ��ȿ���� Ȯ��
	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver();
	}
}

void ASpartaGameState::OnGameOver()
{
	//������ ���������� ���θ޴� ���ֱ�
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			//���� ��ü�� ���߱�
			SpartaPlayerController->SetPause(true);
			SpartaPlayerController->ShowMainMenu(true);
		}
	}
}

void ASpartaGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget())
			{
				//�ð�
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));

				}
				//����
				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
						if (SpartaGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), SpartaGameInstance->TotalScore)));
						}
					}
				}
				//����
				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("level: %d"), CurrentLevelIndex+1)));
				}
			}
		}
	}
}