// Copyright 2022 Danyang Chen https://github.com/DAN-AND-DNA

#include "MMoInputDraw.h"

#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"

UMMoInputDraw::UMMoInputDraw()
	: AttachPointComp(nullptr)
	, ArrowComponent(nullptr)
	, ArrowRangeOutComponent(nullptr)
	, RangeInComponent(nullptr)
	, RangeOutComponent(nullptr) {
	
	AttachPointComp = CreateDefaultSubobject<USceneComponent>("AttachComp");
	AttachPointComp->PrimaryComponentTick.bCanEverTick = false;
}

bool UMMoInputDraw::DrawRange(AActor* InActor,  const FVector2D& LocalOffset, float WidgetRadius, float RangeOutRadius, float RangeInRadius, UMaterial* InRangeIn, UMaterial* InRangeOut, FVector& OutResultWorld) {
	if (!IsValid(InActor) || WidgetRadius <= 0) {
		OutResultWorld = FVector::ZeroVector;
		return false;
	}

	this->AttachPointComp->AttachToComponent(InActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	this->AttachPointComp->SetAbsolute(false, true, false);
	
	// The maximum range of skill
	if (this->RangeOutComponent == nullptr && InRangeOut != nullptr) {
		this->RangeOutComponent = NewObject<UDecalComponent>(InActor);
		this->RangeOutComponent->RegisterComponent();
		this->RangeOutComponent->AttachToComponent(this->AttachPointComp, FAttachmentTransformRules::KeepRelativeTransform);
		this->RangeOutComponent->SetWorldRotation(FRotator(-90.f, 0.f, 0.f));
		this->RangeOutComponent->SetDecalMaterial(InRangeOut);
		this->RangeOutComponent->DecalSize = FVector(RangeOutRadius + 100.f, RangeOutRadius, RangeOutRadius);
		
	}

	// The effective range of skill
	if (this->RangeInComponent == nullptr && InRangeIn != nullptr) {
		this->RangeInComponent = NewObject<UDecalComponent>(InActor);
		this->RangeInComponent->RegisterComponent();
		this->RangeInComponent->AttachToComponent(this->AttachPointComp, FAttachmentTransformRules::KeepRelativeTransform);
		this->RangeInComponent->SetWorldRotation(FRotator(-90.f, 0.f, 0.f));
		this->RangeInComponent->SetDecalMaterial(InRangeIn);
		this->RangeInComponent->DecalSize = FVector(RangeInRadius + 100.f, RangeInRadius, RangeInRadius);
	}


	FVector2D Direction2D = LocalOffset;
	Direction2D.Normalize();
	FVector Direction3D = FVector(Direction2D.X, Direction2D.Y, 0.f);

	FRotator LocalToWorld = FRotator(0.f, 90.f, 0.f);
	FVector WorldDirection3D = LocalToWorld.RotateVector(Direction3D);

	float LocalOffsetDistance = LocalOffset.Size();

	
	// Convert screen coordinates to world coordinates
	FVector RangeInLoc =  InActor->GetActorLocation() + WorldDirection3D * (RangeOutRadius / WidgetRadius * LocalOffsetDistance);
	RangeInLoc.Z = InActor->GetActorLocation().Z;
	OutResultWorld = RangeInLoc;

	if (this->RangeInComponent != nullptr) {
		this->RangeInComponent->SetWorldLocation(RangeInLoc);
	}

	return true;

}


void UMMoInputDraw::CleanRange() {

	if (this->RangeOutComponent != nullptr) {
		this->RangeOutComponent->DestroyComponent();
		this->RangeOutComponent = nullptr;
	}

	if (this->RangeInComponent != nullptr) {
		this->RangeInComponent->DestroyComponent();
		this->RangeInComponent = nullptr;
	}

}


bool UMMoInputDraw::DrawArrow(AActor* InActor, const FVector2D& LocalOffset, float Radius, UMaterial* InArrow, UMaterial* InRangeOut, float FixedArrowSize, FVector& OutResultWorld) {
	if (!IsValid(InActor) || Radius <= 0) {
		OutResultWorld = FVector::ZeroVector;
		return false;
	}
	
	this->AttachPointComp->AttachToComponent(InActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	this->AttachPointComp->SetAbsolute(false, true, false);
	
	if (this->ArrowComponent == nullptr && InArrow != nullptr) {
		this->ArrowComponent = NewObject<UDecalComponent>(InActor);
		this->ArrowComponent->RegisterComponent();
		this->ArrowComponent->AttachToComponent(this->AttachPointComp, FAttachmentTransformRules::KeepRelativeTransform);	
		this->ArrowComponent->SetDecalMaterial(InArrow);
		this->ArrowComponent->DecalSize = FVector(Radius/2.f + 100.f, Radius/2.f, Radius/2.f);
		if (FixedArrowSize > 0.f) {
			this->ArrowComponent->DecalSize = FVector(Radius / 2.f + 100.f, FixedArrowSize/2.f, Radius / 2.f);
		}
		
	}

	if (this->ArrowRangeOutComponent == nullptr && InRangeOut != nullptr) {
		this->ArrowRangeOutComponent = NewObject<UDecalComponent>(InActor);
		this->ArrowRangeOutComponent->RegisterComponent();
		this->ArrowRangeOutComponent->AttachToComponent(this->AttachPointComp, FAttachmentTransformRules::KeepRelativeTransform);
		this->ArrowRangeOutComponent->SetWorldRotation(FRotator(-90.f, 0.f, 0.f));
		this->ArrowRangeOutComponent->SetDecalMaterial(InRangeOut);
		this->ArrowRangeOutComponent->DecalSize = FVector(Radius + 100.f, Radius, Radius);
		
	}

	FVector2D Direction2D = LocalOffset;
	FRotator Offset(0.f, -90.0f, 0.f);

	if (Direction2D == FVector2D::ZeroVector) {
		Direction2D = FVector2D(InActor->GetActorRotation().Vector());
		Offset = FRotator::ZeroRotator;
	}

	// Convert screen coordinates to world coordinates
	Direction2D.Normalize();
	const FVector Direction3D(Direction2D.X, Direction2D.Y, 0.f);
	const FVector WorldDirection3D = Offset.RotateVector(Direction3D);

	OutResultWorld = InActor->GetActorLocation() + WorldDirection3D * Radius;
	
	if (this->ArrowComponent != nullptr) {

		// 箭头外圈的位置
		FVector WorldPositionArrow = InActor->GetActorLocation() + WorldDirection3D * Radius / 2.f;
		WorldPositionArrow.Z = 0.f;
		
		const float CurrentYaw = (WorldDirection3D * Radius / 2.f).Rotation().Yaw;
		this->ArrowComponent->SetWorldLocation(WorldPositionArrow);
		this->ArrowComponent->SetWorldRotation(FRotator(-90.f, CurrentYaw, 0.f));
		//this->ArrowComponent->SetWorldRotation(FRotator(-90.f, (WorldPositionArrow - InActor->GetActorLocation()).Rotation().Yaw, 0.f));
	}

	return true;
}

void UMMoInputDraw::CleanArrow() {
	if (this->ArrowComponent != nullptr) {
		this->ArrowComponent->DestroyComponent();
		this->ArrowComponent = nullptr;
	}

	if (this->ArrowRangeOutComponent != nullptr) {
		this->ArrowRangeOutComponent->DestroyComponent();
		this->ArrowRangeOutComponent = nullptr;
	}
}