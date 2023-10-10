// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"

#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	StaticMeshComp->SetSimulatePhysics(true);
	// 启用模拟物理会自动将Profile更改为蓝图中的PhysicsActor，在c++中我们需要手动更改。
	StaticMeshComp->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	RootComponent = StaticMeshComp;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(StaticMeshComp);

	// 将此选项保留为打开状态时，会通过组件‘Tick’施加较小的恒定力(可选)
	ForceComp->SetAutoActivate(true);

	ForceComp->Radius = 750.0f;
	ForceComp->ImpulseStrength = 2500.0f;// 备选方案：如果bImpulseVelChange=false，则为200000.0

	// 可选，忽略其他对象的“质量”（如果为false，则根据质量推送大多数对象的脉冲强度会高得多）
	ForceComp->bImpulseVelChange = true;

	// 可选的，组件的默认构造函数已经添加了4个对象类型，不包括WorldDynamic
	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

void AExplosiveBarrel::PostInitializeComponents()
{
	// 别忘了调父函数
	Super::PostInitializeComponents();

	StaticMeshComp->OnComponentHit.AddDynamic(this, &AExplosiveBarrel::OnActorHit);
}

void AExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ForceComp->FireImpulse();

	//FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());
	//DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true, 1.0f);
}

