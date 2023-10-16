#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "Engine/DataTable.h"
#include "GameFramework/GameModeBase.h"
#include "OpenAIDefinitions.h"
#include "CGameMode.generated.h"

USTRUCT()
struct FResultData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FString InMessage;

	UPROPERTY(EditAnywhere)
		FString OutMessage;

	UPROPERTY(EditAnywhere)
		FString InImageMessage;
	
	UPROPERTY(EditAnywhere)
		FString ImageURL;
};

USTRUCT()
struct FResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		TArray<FResultData> Result;
};

USTRUCT()
struct FTextData : public  FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Ans0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Ans1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Ans2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Ans3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Ans4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Ans5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Ans6;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameMode_Update_UI, TArray<FText>, Texts);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameMode_Update_Sprite, int32, TurnIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameMode_Create_Result, const TArray<FString>&, InputMessage);

UCLASS()
class MY_VN5_API ACGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<APawn> MainPawnClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UCUserWidget_MainInterface> MainInterfaceClass;

	UPROPERTY(EditDefaultsOnly)
		class UDataTable* TextDataTable;

	UPROPERTY(BlueprintReadWrite)
		class UCGPTRequestManager* GPTManager;

	UPROPERTY(BlueprintReadOnly)
		FString InputMessage;

	UPROPERTY(EditAnywhere)
		FResult Result;
	
	UFUNCTION()
		void UpdateText();

protected:
	class UCUserWidget_MainInterface* MainInterface;
	
public:
	ACGameMode();
	
	virtual void BeginPlay() override;

	void CreateInputMessage();

public:
	UPROPERTY(BlueprintAssignable)
		FGameMode_Update_UI OnGameMode_Update_UI;
	
	UPROPERTY(BlueprintAssignable)
		FGameMode_Update_Sprite OnGameMode_Update_Sprite;

	UPROPERTY(BlueprintAssignable)
		FGameMode_Create_Result OnGameMode_Create_Result;

	UFUNCTION()
		void SetChatResult(const FChatCompletion message, const FString& errorMessage, bool Success);

	UFUNCTION()
		void SetDALLEResult(const TArray<FString>& generatedImageUrls, const FString& errorMessage, bool Success);

	UFUNCTION()
		void SetDownloadedTexture(UTexture2DDynamic* InTexture);

	UFUNCTION(BlueprintCallable)
		FORCEINLINE class UCUserWidget_MainInterface* GetMainInterface() const { return MainInterface; }
	
	FORCEINLINE int32 GetTurnIndex() const { return TurnIndex; }
	FORCEINLINE int32 GetDataLength() const { return DataLength; }
	FORCEINLINE bool IsStop() const { return bStop; }

	FORCEINLINE TStaticArray<float, 7> GetTextsSpeed() const { return TextsSpeed; }
	FORCEINLINE void SetTextSpeed(int32 InIndex, float InSpeed);
	
	FORCEINLINE void CollectText(const FText& InText) { CollectedTexts.Push(InText); }

	void StartAI(const FChatSettings& InChatSettings);
	void StartChatAI(const FChatSettings& InChatSettings);
	void StartDALLEAI();
	void ApplyToViewport(UTexture2DDynamic* InTexture);
	void SaveResultData();
	
private:
	class ACPlayer* Player;

	FTimerHandle TimerHandle;

	int32 TurnIndex;
	int32 DataLength;
	
	float DelaySeconds;
	
	bool bStop;
	bool bEnd;

	TArray<FText> Texts;
	TStaticArray<float, 7> TextsSpeed;

	TArray<FText> CollectedTexts;

	FString OutputMessage;
	FString InImageMessage;
	FString OutputImageURL;
	UTexture2DDynamic* OutputTexture;
};
