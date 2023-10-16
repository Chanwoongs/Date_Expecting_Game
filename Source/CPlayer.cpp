#include "..\Public\CPlayer.h"

#include "CGameMode.h"
#include "Global.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Paper2DClasses.h"


ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetCapsuleComponent());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);
	
	CHelpers::CreateComponent<UPaperSpriteComponent>(this, &Background, "Background", GetCapsuleComponent());
	
	CHelpers::GetAsset<UPaperFlipbook>(&Sprites[0], "/Script/Paper2D.PaperFlipbook'/Game/Art/Hero_Face/Pixel_40/FB_Face_Pixel_40.FB_Face_Pixel_40'");
	CHelpers::GetAsset<UPaperFlipbook>(&Sprites[1], "/Script/Paper2D.PaperFlipbook'/Game/Art/Hero_Face/Pixel_30/FB_Face_Pixel_30.FB_Face_Pixel_30'");
	CHelpers::GetAsset<UPaperFlipbook>(&Sprites[2], "/Script/Paper2D.PaperFlipbook'/Game/Art/Hero_Face/Pixel_20/FB_Face_Pixel_20.FB_Face_Pixel_20'");
	CHelpers::GetAsset<UPaperFlipbook>(&Sprites[3], "/Script/Paper2D.PaperFlipbook'/Game/Art/Hero_Face/Pixel_10/FB_Face_Pixel_10.FB_Face_Pixel_10'");
	CHelpers::GetAsset<UPaperFlipbook>(&Sprites[4], "/Script/Paper2D.PaperFlipbook'/Game/Art/Hero_Face/Pixel_5/FB_Face_Pixel_5.FB_Face_Pixel_5'");
	CHelpers::GetAsset<UPaperFlipbook>(&Sprites[5], "/Script/Paper2D.PaperFlipbook'/Game/Art/Hero_Face/Pixel_0/FB_Face_Pixel_0.FB_Face_Pixel_0'");
	CHelpers::GetAsset<UPaperFlipbook>(&Sprites[6], "/Script/Paper2D.PaperFlipbook'/Game/Art/Hero_Face/Pixel_0/FB_Face_Pixel_0.FB_Face_Pixel_0'");

	CHelpers::GetAsset<UPaperSprite>(&BackgroundSprites[0], "/Script/Paper2D.PaperSprite'/Game/Art/Background/background_pixel_40_Sprite.background_pixel_40_Sprite'");
	CHelpers::GetAsset<UPaperSprite>(&BackgroundSprites[1], "/Script/Paper2D.PaperSprite'/Game/Art/Background/background_pixel_30_Sprite.background_pixel_30_Sprite'");
	CHelpers::GetAsset<UPaperSprite>(&BackgroundSprites[2], "/Script/Paper2D.PaperSprite'/Game/Art/Background/background_pixel_20_Sprite.background_pixel_20_Sprite'");
	CHelpers::GetAsset<UPaperSprite>(&BackgroundSprites[3], "/Script/Paper2D.PaperSprite'/Game/Art/Background/background_pixel_10_Sprite.background_pixel_10_Sprite'");
	CHelpers::GetAsset<UPaperSprite>(&BackgroundSprites[4], "/Script/Paper2D.PaperSprite'/Game/Art/Background/background_pixel_5_Sprite.background_pixel_5_Sprite'");
	CHelpers::GetAsset<UPaperSprite>(&BackgroundSprites[5], "/Script/Paper2D.PaperSprite'/Game/Art/Background/background_pixel_0_Sprite.background_pixel_0_Sprite'");
	CHelpers::GetAsset<UPaperSprite>(&BackgroundSprites[6], "/Script/Paper2D.PaperSprite'/Game/Art/Background/background_pixel_Sprite.background_pixel_Sprite'");
	
	SpringArm->SetRelativeRotation(FRotator(0, -90, 0));
	
	GetSprite()->SetRelativeLocation(FVector(-154.0, 0, 0));
	GetSprite()->SetRelativeScale3D(FVector(0.6, 0.6, 0.6));

	Background->SetRelativeLocation((FVector(0, -470, 0)));
	Background->SetSpriteColor(FLinearColor(0.2, 0.2, 0.2, 1));

	GetSprite()->SetFlipbook(Sprites[0]);
	Background->SetSprite(BackgroundSprites[0]);
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	GameMode = CHelpers::GetCustomGameMode<ACGameMode>(GetWorld());
	GameMode->OnGameMode_Update_Sprite.AddDynamic(this, &ACPlayer::UpdateCharacterSprite);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

	GameMode->OnGameMode_Create_Result.AddDynamic(this, &ACPlayer::RequestResult);
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Quit_Game", IE_Pressed, this, &ACPlayer::QuitGame);
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector bgTransform = Background->GetRelativeLocation();
	bgTransform.Y -= 0.025;
	Background->SetRelativeLocation(bgTransform);
}

void ACPlayer::UpdateCharacterSprite(int32 TurnIndex)
{
	GetSprite()->SetFlipbook(Sprites[TurnIndex]);
	Background->SetSprite(BackgroundSprites[TurnIndex]);
}

void ACPlayer::QuitGame() 
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}
