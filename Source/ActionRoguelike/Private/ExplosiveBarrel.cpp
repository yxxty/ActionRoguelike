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
	// ����ģ��������Զ���Profile����Ϊ��ͼ�е�PhysicsActor����c++��������Ҫ�ֶ����ġ�
	StaticMeshComp->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	RootComponent = StaticMeshComp;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(StaticMeshComp);

	// ����ѡ���Ϊ��״̬ʱ����ͨ�������Tick��ʩ�ӽ�С�ĺ㶨��(��ѡ)
	ForceComp->SetAutoActivate(true);

	ForceComp->Radius = 750.0f;
	ForceComp->ImpulseStrength = 2500.0f;// ��ѡ���������bImpulseVelChange=false����Ϊ200000.0

	// ��ѡ��������������ġ������������Ϊfalse��������������ʹ�������������ǿ�Ȼ�ߵöࣩ
	ForceComp->bImpulseVelChange = true;

	// ��ѡ�ģ������Ĭ�Ϲ��캯���Ѿ������4���������ͣ�������WorldDynamic
	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

void AExplosiveBarrel::PostInitializeComponents()
{
	// �����˵�������
	Super::PostInitializeComponents();

	StaticMeshComp->OnComponentHit.AddDynamic(this, &AExplosiveBarrel::OnActorHit);
}

void AExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ForceComp->FireImpulse();

	//FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());
	//DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true, 1.0f);
}

