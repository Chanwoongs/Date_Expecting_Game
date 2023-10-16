#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_MainInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUserWidget_MainInterface_OnClick, URichTextBlock*, Text);

UCLASS()
class MY_VN5_API UCUserWidget_MainInterface : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCUserWidget_PhraseBtn* WB_PhraseBtn_0;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCUserWidget_PhraseBtn* WB_PhraseBtn_1;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCUserWidget_PhraseBtn* WB_PhraseBtn_2;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCUserWidget_PhraseBtn* WB_PhraseBtn_3;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCUserWidget_PhraseBtn* WB_PhraseBtn_4;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCUserWidget_PhraseBtn* WB_PhraseBtn_5;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCUserWidget_PhraseBtn* WB_PhraseBtn_6;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* Line;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* ResultBackground;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* ResultImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class URichTextBlock* CollectedText;
	
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* CollectedTextAnim;

	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* ResultAnim;

	UPROPERTY(BlueprintAssignable)
		FUserWidget_MainInterface_OnClick OnWidget_MainInterface_OnClick;
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UFUNCTION()
		void UpdateAllButtonText(TArray<FText> InTexts);
	UFUNCTION(BlueprintCallable)
		void UpdateCollectedText(FText InText);
	UFUNCTION(BlueprintCallable)
		void StartResultAnim();

private:
	void SetTextPosSpeed(class UCanvasPanelSlot* InCanvasPanelSlot, int32 InNum);
	void UpdateFaceUI(float InDeltaTime);

private:
	class ACGameMode* GameMode;

	TStaticArray<class UCUserWidget_PhraseBtn*, 7> PhraseBtns;
	TStaticArray<class UCanvasPanelSlot*, 7> CanvasPanelSlots;

	float TextMinSpeed;
	float TextMaxSpeed;
};
