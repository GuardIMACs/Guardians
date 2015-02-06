// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefense.h"
#include "BaseUnit.h"
#include "Widgets/BaseLifeBar.h"

ABaseUnit::ABaseUnit(const class FObjectInitializer& PCIP)
	: Super(PCIP), Type(EUnitType::Neutral), Locks(0), HealthBar(nullptr)
{
	uint8 max = static_cast<uint8>(EElement::Max);
	Defense.Reserve(max);
	for (int i = 0; i < max; ++i)
		Defense.Add(0.5f);

	static ConstructorHelpers::FObjectFinder<UBlueprint> blueprint(TEXT("WidgetBlueprint'/Game/Widgets/HealthBar.HealthBar'"));
	UE_LOG(LogTemp, Warning, TEXT("object: %p"), blueprint.Object);
	if (blueprint.Object)
	{
		UClass* uclass = blueprint.Object->GeneratedClass;
		UWorld* const world = GetWorld();
		if (world)
		{
			//HealthBar = CreateWidget<UBaseLifeBar>(world, uclass);
			UE_LOG(LogTemp, Warning, TEXT("lifebar: %p"), HealthBar);
			SetMaxLife(100);
			SetCurrentLife(100);

		}
	}

	
	Widget = PCIP.CreateDefaultSubobject<UWidgetComponent>(this, "Widget");
	/*auto widget = PCIP.defaul;
	widget->AttachTo(GetRootComponent());
	widget->Space = EWidgetSpace::Screen;
	widget->GetUserWidgetObject();
	widget->RegisterComponent();*/
}

void ABaseUnit::Tick(float DeltaSeconds)
{
	if (CurrentLife <= 0)
	{
		// Some units still references this unit
		if (IsLocked())
			return;

		OnDestroy();
		Destroy();
		return;
	}
	if (Behavior.IsValid())
		Behavior->Tick(DeltaSeconds);
	for (auto& atk : Attack)
		atk->Tick(DeltaSeconds);

	if (HealthBar)
	{
		//HealthBar->Update();
	}
}



bool ABaseUnit::IsAlive()
{
	return CurrentLife > 0;
}

void ABaseUnit::SetMaxLife(int32 life)
{
	MaxLife = life;
	//if (HealthBar)
		//HealthBar->SetMaxLife(MaxLife);
}

void ABaseUnit::SetCurrentLife(int32 life)
{
	CurrentLife = life;
	//if (HealthBar)
		//HealthBar->SetCurrentLife(CurrentLife);
}