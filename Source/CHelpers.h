#pragma once

#include "CoreMinimal.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"


class MY_VN5_API CHelpers
{
public:
	template<typename T>
	static void GetAsset(T** OutObject, FString InPath)
	{
		ConstructorHelpers::FObjectFinder<T> asset(*InPath); //문자열을 Text로 바꾸기 위해 *를 붙혀줌
		verifyf(asset.Succeeded(), L"asset.Succeeded()");

		*OutObject = asset.Object;
	}

	template<typename T>
	static void GetAssetDynamic(T** OutObject, FString InPath)
	{
		T* obj = Cast<T>(StaticLoadObject(T::StaticClass(), NULL, *InPath));
		verifyf(!!obj, L"!!asset");
		//!! NULL 아니면 True, NULL 이면 False

		*OutObject = obj;
	}

	template<typename T>
	static void GetClass(TSubclassOf<T>* OutClass, FString InPath)
	{
		ConstructorHelpers::FClassFinder<T> asset(*InPath); //문자열을 Text로 바꾸기 위해 *를 붙혀줌
		verifyf(asset.Succeeded(), L"asset.Succeeded()");

		*OutClass = asset.Class;
	}

	template<typename T>
	static void CreateComponent(AActor* InActor, T** InComponent, const FName InName, USceneComponent* InParent = NULL)
	{
		*InComponent = InActor->CreateDefaultSubobject<T>(InName);

		if (!!InParent)
		{
			(*InComponent)->SetupAttachment(InParent);

			return;
		}

		InActor->SetRootComponent(*InComponent);
	}

	template<typename T>
	static void CreateActorComponent(AActor* InActor, T** InComponent, FName InName)
	{
		*InComponent = InActor->CreateDefaultSubobject<T>(InName);
	}

	template<typename T>
	static T* GetComponent(AActor* InActor)
	{
		return Cast<T>(InActor->GetComponentByClass(T::StaticClass()));
	}

	template<typename T>
	static void FindActors(class UWorld* InWorld, TArray<T*>& OutActors)
	{
		OutActors.Empty();

		// UGameplayStatics -> Gameplay 하는데에 도움되는 것의 모음
		TArray<AActor*> actors; // TArray : 가변형 배열, vector container와 유사한 기능을 가진다.
		UGameplayStatics::GetAllActorsOfClass(InWorld, T::StaticClass(), actors); // StaticClass -> 자기 자신의 대한 UClass 포인터 리턴

		for (AActor* actor : actors)
		{
			OutActors.Add(Cast<T>(actor));
		}
	}

	template<typename T>
	static T* GetCustomGameMode(UWorld* worldContext)
	{
		return Cast<T>(worldContext->GetAuthGameMode());
	}
};
