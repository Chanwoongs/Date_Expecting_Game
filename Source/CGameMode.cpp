#include "CGameMode.h"

#include <string>

#include "CHelpers.h"
#include "Global.h"

#include "GameFramework/GameUserSettings.h"
#include "Engine/DataTable.h"
#include "Engine/World.h"
#include "OpenAIAPI/Public/OpenAIUtils.h"
#include "OpenAIAPI/Public/OpenAICallChat.h"
#include "OpenAIAPI/Public/OpenAICallDALLE.h"
#include "JsonObjectConverter.h"
#include "Misc/FileHelper.h"

#include "CPlayer.h"
#include "CUserWidget_MainInterface.h"
#include "CGPTRequestManager.h"
#include "SAdvancedTransformInputBox.h"
#include "Blueprint/AsyncTaskDownloadImage.h"
#include "Components/Image.h"
#include "Components/RichTextBlock.h"

ACGameMode::ACGameMode() : TurnIndex(0), DelaySeconds(10.f), bStop(false), bEnd(false)
{
	CHelpers::GetClass(&MainPawnClass, "/Script/Engine.Blueprint'/Game/Blueprint/BP_CPlayer.BP_CPlayer_C'");
	CHelpers::GetClass(&MainInterfaceClass, "/Script/UMGEditor.WidgetBlueprint'/Game/Widget/WB_MainInterface.WB_MainInterface_C'");
	CHelpers::GetAsset(&TextDataTable, "/Script/Engine.DataTable'/Game/Data/DT_TextData.DT_TextData'");

	DefaultPawnClass = MainPawnClass;

	GPTManager = NewObject<UCGPTRequestManager>();
}

void ACGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	Player = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));

	UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings();
	GameUserSettings->SetScreenResolution(FIntPoint(1920, 1080));
	GameUserSettings->ApplyResolutionSettings(false);
	GameUserSettings->SetFullscreenMode(EWindowMode::Windowed);

	MainInterface = CreateWidget<UCUserWidget_MainInterface>(GetWorld(), MainInterfaceClass);
	MainInterface->AddToViewport();

	DataLength = 7;
	
	// // Data Print Test
	// for (const auto& Row : TextDataTable->GetRowMap())
	// {
	// 	FString str = Row.Key.ToString()''
	// 	CLog::Print(str);
	// }
	// //

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("UpdateText"));
	
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, DelaySeconds, true, 0);
	
	//GPTManager->RequestToGPT3(TEXT("Hello, GPT-3!")); 
}

void ACGameMode::SetTextSpeed(int32 InIndex, float InSpeed)
{
	TextsSpeed[InIndex] = InSpeed;
}

void ACGameMode::UpdateText()
{
	bStop = true;

	CheckTrue(bEnd);
	
	if (TurnIndex == DataLength - 1 && !bEnd)
	{
		bEnd = true;
		CreateInputMessage();
		UOpenAIUtils::setOpenAIApiKey("Chanwoong's Key");

		// OpenAI Chat
		FChatLog chatLog;
		chatLog.role = EOAChatRole::USER;
		chatLog.content = InputMessage;

		FChatSettings chatSettings;
		chatSettings.model = EOAChatEngineType::GPT_3_5_TURBO;
		chatSettings.messages.Add(chatLog);

		StartAI(chatSettings);
		//UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);

		return;
	}
	else
	{
		TurnIndex++;
		
		FTextData* data = TextDataTable->FindRow<FTextData>(*FString::FromInt(TurnIndex), TEXT(""));
		
		Texts.Add(data->Ans0);
		Texts.Add(data->Ans1);
		Texts.Add(data->Ans2);
		Texts.Add(data->Ans3);
		Texts.Add(data->Ans4);
		Texts.Add(data->Ans5);
		Texts.Add(data->Ans6);
		OnGameMode_Update_UI.Broadcast(Texts);
		
		Texts.Empty();
		OnGameMode_Update_Sprite.Broadcast(TurnIndex);
	}
}

void ACGameMode::CreateInputMessage()
{
	InputMessage = TEXT("I'm a woman in my 20s. The sentences below represent my current state , mood, and my needs. I have a date today, so please read all these sentences and predict in one short sentence how the date will go today\n");
	
	for (auto text : CollectedTexts)
	{
		const FString str = text.ToString();
		InputMessage += str;
		InputMessage += "\n";
	}
}

void ACGameMode::StartAI(const FChatSettings& InChatSettings)
{
	StartChatAI(InChatSettings);
}

void ACGameMode::StartChatAI(const FChatSettings& InChatSettings)
{
	UOpenAICallChat* openAICallChat = UOpenAICallChat::OpenAICallChat(InChatSettings);
	openAICallChat->Finished.AddDynamic(this, &ACGameMode::SetChatResult);
	openAICallChat->Activate();
}

void ACGameMode::SetChatResult(const FChatCompletion message, const FString& errorMessage, bool Success)
{
	OutputMessage = message.message.content;

	StartDALLEAI();
}

void ACGameMode::StartDALLEAI()
{
	// OpenAI DALLE
	InImageMessage += OutputMessage;
	InImageMessage = TEXT("\nMe and my boyfriend affected by my vibe");
	UOpenAICallDALLE* openAICallDalle = UOpenAICallDALLE::OpenAICallDALLE(EOAImageSize::SMALL, InImageMessage, 1);
	openAICallDalle->Finished.AddDynamic(this, &ACGameMode::SetDALLEResult);
	openAICallDalle->Activate();
}

void ACGameMode::SetDALLEResult(const TArray<FString>& generatedImageUrls, const FString& errorMessage, bool Success)
{
	OutputImageURL = generatedImageUrls[0];

	// Download Image
	UAsyncTaskDownloadImage* asyncTaskDownloadImage = UAsyncTaskDownloadImage::DownloadImage(OutputImageURL);
	asyncTaskDownloadImage->OnSuccess.AddDynamic(this, &ACGameMode::SetDownloadedTexture);
}

void ACGameMode::SetDownloadedTexture(UTexture2DDynamic* InTexture)
{
	OutputTexture = InTexture;

	ApplyToViewport(OutputTexture);
}

void ACGameMode::ApplyToViewport(UTexture2DDynamic* InTexture)
{
	MainInterface->UpdateCollectedText(FText::FromString(OutputMessage));
	
	MainInterface->StartResultAnim();
	MainInterface->ResultImage->SetBrushFromTextureDynamic(OutputTexture);

	SaveResultData();
}

void ACGameMode::SaveResultData()
{
	FResultData result;
	result.InMessage = InputMessage;
	result.OutMessage = OutputMessage;
	result.InImageMessage = InImageMessage;
	result.ImageURL = OutputImageURL;

	FString fullFilePath = FPaths::ProjectContentDir() + TEXT("/Json/ResultData.json");

	FString fileContent;
	// 파일이 이미 존재하는 경우 Append
	if (FFileHelper::LoadFileToString(fileContent, *fullFilePath))
	{
		// Json 파싱
		// TJsonReaderFactory를 사용하여 JSON 파일의 내용을 파싱하기 위한 reader 객체 생성
		// fileContent는 JSON 파일의 내용을 담고 있는 문자열
		TSharedPtr<FJsonObject> jsonObject;
		TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(fileContent);

		// JSON 역직렬화
		// Deserialize() 제공된 reader를 사용하여 JSON 문자열을 JSON object로 변환
		if (FJsonSerializer::Deserialize(reader, jsonObject) && jsonObject.IsValid())
		{
			// Result 배열 가져오기 jsonDataArray에 저장
			TArray<TSharedPtr<FJsonValue>> jsonDataArray = jsonObject->GetArrayField(TEXT("Result"));

			// 새로운 데이터 추가
			TSharedRef<FJsonObject> newResultObject(new FJsonObject());
			// result 구조체를 새로운 JSON 오브젝트로 변환
			if (FJsonObjectConverter::UStructToJsonObject(FResultData::StaticStruct(), &result, newResultObject, 0, 0))
			{
				// 변환된 오브젝트를 jsonDataArray에 추가
				jsonDataArray.Add(MakeShareable(new FJsonValueObject(newResultObject)));
			}

			//수정된 jsonDataArray를 다시 jsonObject의 Result 필드에 설정
			jsonObject->SetArrayField(TEXT("Result"), jsonDataArray);

			// Json 파일에 저장
			FString outputString;
			// jsonObject를 다시 JSON 문자열로 변환하고, 이 문자열을 파일에 저장
			TSharedRef<TJsonWriter<>> writer = TJsonWriterFactory<>::Create(&outputString);
			// jsonObject를 JSON 문자열로 직렬화 
			FJsonSerializer::Serialize(jsonObject.ToSharedRef(), writer);
			// 파일에 저장
			FFileHelper::SaveStringToFile(outputString, *fullFilePath);
		}
	}
	// 파일이 없는 경우
	else 
	{
		Result.Result.Add(result);

		FString jsonString;
		FJsonObjectConverter::UStructToJsonObjectString(Result, jsonString);
		FFileHelper::SaveStringToFile(jsonString, *fullFilePath);
	}
	
}






