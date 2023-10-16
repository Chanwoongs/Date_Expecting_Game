#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "CPlayer.generated.h"

UCLASS()
class MY_VN5_API ACPlayer : public APaperZDCharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;
	
	UPROPERTY(VisibleDefaultsOnly)
		class UPaperSpriteComponent* Background;

public:
	ACPlayer();

public:	
	virtual void BeginPlay() override;

	virtual auto SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) -> void override;
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void UpdateCharacterSprite(int32 TurnIndex);

	UFUNCTION(BlueprintImplementableEvent)
		void RequestResult(const TArray<FString>& InputMessages);

private:
	void QuitGame();
	
private:
	TStaticArray<class UPaperFlipbook*, 7> Sprites;
	TStaticArray<class UPaperSprite*, 7> BackgroundSprites;

private:
	class ACGameMode* GameMode;
	class ACPlayerController* PlayerController;
};
