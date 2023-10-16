#include "CUserWidget_PhraseBtn.h"
#include "Global.h"
#include "Components/Image.h"
#include "Components/RichTextBlock.h"

#include "Sound/SoundCue.h"

void UCUserWidget_PhraseBtn::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UCUserWidget_PhraseBtn::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCUserWidget_PhraseBtn::OnClick()
{
	UGameplayStatics::PlaySound2D(GetWorld(), PopSound);
	SetVisibility(ESlateVisibility::Hidden);

	OnUserWidget_PhraseBtn_Clicked.Broadcast(Text->GetText());
}

void UCUserWidget_PhraseBtn::OnDead()
{
	ExplosionEffectImg->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(ExplosionAnim);
	Text->SetVisibility(ESlateVisibility::Hidden);
	SetIsEnabled(false);
	
	FTimerHandle timerHandle;
	float time = 0.1f; 
	GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateLambda([&]()
	{
		ExplosionEffectImg->SetVisibility(ESlateVisibility::Hidden);
	}), time, false);
}

void UCUserWidget_PhraseBtn::UpdateSelf(FText InText)
{
	SetIsEnabled(true);
	SetVisibility(ESlateVisibility::Visible);
	ExplosionEffectImg->SetVisibility(ESlateVisibility::Hidden);
	Text->SetText(InText);
	Text->SetVisibility(ESlateVisibility::Visible);
}

void UCUserWidget_PhraseBtn::ResetSelf()
{
	FTimerHandle timerHandle;
	const float time = 1.f;
		
	GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateLambda([&]()
	{
		BtnReset();
	}), time, false);
}
