// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemKey.h"
#include "../MyTestCharacter.h"

AItemKey::AItemKey()
{
    Item = EItemType::Item_Key;
}

void AItemKey::OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent,
                                AActor* OtherActor,
                                UPrimitiveComponent* OtherComp,
                                int32 OtherBodyIndex,
                                bool bFromSweep,
                                const FHitResult& SweepResult)
{
    AMyTestCharacter* player = Cast<AMyTestCharacter>(OtherActor);
    if (player != nullptr)
    {
        player->Inventory.Add(Item);
        player->keySet.Add(keyNum);
        UE_LOG(LogTemp, Log, TEXT("Get %d Key"), keyNum);
        Destroy();
    }
}