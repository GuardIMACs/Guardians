#pragma once
#include "Animation/AnimInstance.h"
#include "AliensAnimation.generated.h"

UCLASS(transient, Blueprintable, hideCategories = AnimInstance, BlueprintType)
class UAliensAnimation : public UAnimInstance
{
	GENERATED_UCLASS_BODY()

	/** Is Moving */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	bool IsMoving;


	APawn * OwningPawn;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

};

