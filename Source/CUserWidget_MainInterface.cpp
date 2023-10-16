#include "CUserWidget_MainInterface.h"
#include "Global.h"

#include "CGameMode.h"
#include "CUserWidget_PhraseBtn.h"
#include "K2Node_DoOnceMultiInput.h"
#include "Algo/ForEach.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/RichTextBlock.h"

void UCUserWidget_MainInterface::NativeConstruct()
{
	Super::NativeConstruct();
	
	GameMode = CHelpers::GetCustomGameMode<ACGameMode>(GetWorld());

	GameMode->OnGameMode_Update_UI.AddDynamic(this, &UCUserWidget_MainInterface::UpdateAllButtonText);
	
	PhraseBtns[0] = WB_PhraseBtn_0;
	PhraseBtns[1] = WB_PhraseBtn_1;
	PhraseBtns[2] = WB_PhraseBtn_2;
	PhraseBtns[3] = WB_PhraseBtn_3;
	PhraseBtns[4] = WB_PhraseBtn_4;
	PhraseBtns[5] = WB_PhraseBtn_5;
	PhraseBtns[6] = WB_PhraseBtn_6;

	for (auto btn : PhraseBtns)
	{
		btn->OnUserWidget_PhraseBtn_Clicked.AddDynamic(this, &UCUserWidget_MainInterface::UpdateCollectedText);
	}

	TextMinSpeed = 200.f;
	TextMaxSpeed = 300.f;
}

void UCUserWidget_MainInterface::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CheckFalse(GameMode->IsStop());

	UGameViewportClient* ViewportClient = GEngine->GameViewport;
	FVector2D ViewportSize;
	ViewportClient->GetViewportSize(ViewportSize);
	
	for(int32 i = 0; i < GameMode->GetDataLength(); i++)
	{
		const float x = CanvasPanelSlots[i]->GetPosition().X - GameMode->GetTextsSpeed()[i] * InDeltaTime;
		
		const FVector2D pos(x, CanvasPanelSlots[i]->GetPosition().Y);
		CanvasPanelSlots[i]->SetPosition(pos);

		FGeometry btnGeometry = PhraseBtns[i]->GetCachedGeometry();
		FVector2D pixelPos, viewportPos;
		USlateBlueprintLibrary::LocalToViewport(GEngine->GameViewport, btnGeometry, FVector2D::ZeroVector, pixelPos, viewportPos);
		
		//CLog::Print(PhraseBtns[i]->IsUpdated(), i);
		if (pixelPos.X <= 100.f)
		{
			if (!PhraseBtns[i]->IsUpdated())
			{
				PhraseBtns[i]->BtnUpdated();
				PhraseBtns[i]->OnDead();
			}
		}
	}
	UpdateFaceUI(InDeltaTime);
}

void UCUserWidget_MainInterface::UpdateAllButtonText(TArray<FText> InTexts)
{
	for (int32 i = 0; i < GameMode->GetDataLength(); i++)
	{
		PhraseBtns[i]->UpdateSelf(InTexts[i]);
		CanvasPanelSlots[i] = Cast<UCanvasPanelSlot>(PhraseBtns[i]->Slot);
		SetTextPosSpeed(CanvasPanelSlots[i], i);
		PhraseBtns[i]->ResetSelf();
	}
}

void UCUserWidget_MainInterface::UpdateCollectedText(FText InText)
{
	GameMode->CollectText(InText);
	CollectedText->SetText(InText);
	PlayAnimation(CollectedTextAnim);
}

void UCUserWidget_MainInterface::SetTextPosSpeed(class UCanvasPanelSlot* InCanvasPanelSlot, const int32 InNum)
{
	float x = UKismetMathLibrary::RandomFloatInRange(-100.f, 100.f);
	float y = UKismetMathLibrary::RandomFloatInRange(-200.f, 200.f);
	const  FVector2D newPos(x, y);
	
	InCanvasPanelSlot->SetPosition(newPos);

	const float speed = UKismetMathLibrary::RandomFloatInRange(TextMinSpeed, TextMaxSpeed);
	GameMode->SetTextSpeed(InNum, speed);
}

void UCUserWidget_MainInterface::UpdateFaceUI(float InDeltaTime)
{
	const float lineX = Line->GetDesiredSize().X;
	if (lineX <= 0.f) return;
	
	float x = lineX - InDeltaTime * 10.f;
	const FVector2D pos(x, Line->GetDesiredSize().Y);
	Line->SetDesiredSizeOverride(pos);
}

void UCUserWidget_MainInterface::StartResultAnim()
{
	PlayAnimation(ResultAnim);
}


