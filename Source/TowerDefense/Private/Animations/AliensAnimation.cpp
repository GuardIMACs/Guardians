#include "TowerDefense.h"
#include "Animations/AliensAnimation.h"


UAliensAnimation::UAliensAnimation(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	IsMoving = false;
}

void UAliensAnimation::NativeInitializeAnimation()
{
	//Very Important Line
	Super::NativeInitializeAnimation();

	//Has to be set in KismetInit because it depends on the Owning Actor 
	//to have already been constructed

	//So this function is like PostInitAnimtree in UE3

	OwningPawn = TryGetPawnOwner();
}

void UAliensAnimation::NativeUpdateAnimation(float DeltaTimeX)
{
	//Very Important Line
	Super::NativeUpdateAnimation(DeltaTimeX);

	//Always Check Pointers
	if (!OwningPawn) return;
	//~~

	if (OwningPawn->GetVelocity().SizeSquared() > 25) IsMoving = true;
	else IsMoving = false;
}
