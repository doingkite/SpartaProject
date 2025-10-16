#include "SpartaGameState.h"
#include "SpartaGameInstance.h"
#include "SpartaProject/SpartaPlayerController.h"
#include "SpartaProject/SpawnVolume.h"
#include "SpartaProject/CoinItem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

//게임 시작 - BeginPlay() - StartLevel() - BasicLevel()

//레벨에서 40개 아이템 소환
//코인 아이템들 다 먹었을 경우 다음 레벨
//30초의 시간이 주어지고 타임오버가 돼도 바로 다음 레벨
//체력 0이 되는 즉시 게임 오버

ASpartaGameState::ASpartaGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	//Startlevel()에서 레벨을 불러올때마다 하는 초기화랑은 다름
	LevelDuration = 30.f;
	CurrentLevelIndex = 0;
	MaxLevels = 3;
}

void ASpartaGameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();
	//틱함수(블루프린트) 대신에 사용
	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ASpartaGameState::UpdateHUD,//시간이 끝나면 이 함수를 불러라
		0.1f,
		true
	);
}

int32 ASpartaGameState::GetScore() const
{
	return Score;
}

// 점수 추가 요청을 안전하게 전달하는 요청 창구
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
	//레벨이 오픈될 때마다 게임 HUD UI 켜준다
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
	} /* = > 해당 코드 축가 이유 :
	  레벨이 바뀔 때마다 초기화되는 정보(GameState 등)를, 
	  게임 전체에서 유일하게 유지되는 GameInstance의 정보와 동기화하기 위해
	  */

	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	//현재 월드에서 이 액터에 해당되는 모든 것 가져오기, 찾은 볼륨들 FoundVolumes에 넣어주기
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	const int32 ItemToSpawn = 40; //레벨에서 40개의 아이템 소환

	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			//FoundVolumes의 첫번째 요소를 SpawnVolume으로 타입변환
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

	//StartLevel() 호출 후 시간 재야함
	GetWorldTimerManager().SetTimer
	(
		LevelTimerHandle, //FTimerHandle
		this,
		&ASpartaGameState::OnLevelTimeUp, //타이머 끝나자마자 호출해줄 함수
		LevelDuration, //이 레벨이 몇 초 동안 지속이 될 것인지
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

	// CollectedCoinCount / SpawnedCoinCount 스폰된 코인 중 먹은 코인
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

	// StartLevel()과 마찬가지로 GameIstance의 동기화 코드 추가
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			//점수 추가 로직
			AddScore(Score);
			CurrentLevelIndex++;
			//StartLevel()과의 순서가 다른 이유: StartLevel()은 **'로드(Load)'**이고 EndLevel()은 **'저장(Save)'**이기 때문에 데이터의 흐름 방향이 정반대이다.
			SpartaGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}

	//최대 레벨에 도달했는가
	if (CurrentLevelIndex >=  MaxLevels)
	{
		OnGameOver();
		return;
	}
	// 다음 레벨 맵 이름이 유효한지 확인
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
	//게임이 끝날때마다 메인메뉴 켜주기
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			//게임 자체를 멈추기
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
				//시간
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));

				}
				//점수
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
				//레벨
				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("level: %d"), CurrentLevelIndex+1)));
				}
			}
		}
	}
}