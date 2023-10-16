#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_PhraseBtn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUserWidget_PhraseBtn_Clicked, FText, Text);

UCLASS()
class MY_VN5_API UCUserWidget_PhraseBtn : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class USoundCue* PopSound; 
		
protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* Button;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class URichTextBlock* Text;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* ExplosionEffectImg;
	UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* ExplosionAnim;
	
public:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

public:
	FORCEINLINE class UButton* GetButton() const { return Button; }
	FORCEINLINE class URichTextBlock* GetText() { return Text; }
	FORCEINLINE class UImage* GetExplosionEffectImg() { return ExplosionEffectImg; }
	FORCEINLINE class UWidgetAnimation* GetExplosionAnim() { return ExplosionAnim; }
	

protected:
	UFUNCTION(BlueprintCallable)
		void OnClick();

public:
	FORCEINLINE bool IsUpdated() const { return bUpdated; }
	FORCEINLINE void BtnUpdated() { bUpdated = true; }
	FORCEINLINE void BtnReset() { bUpdated = false; }
	void UpdateSelf(FText InText);
	void ResetSelf();
	void OnDead();
public:
	FUserWidget_PhraseBtn_Clicked OnUserWidget_PhraseBtn_Clicked;

private:
	bool bUpdated;
};
