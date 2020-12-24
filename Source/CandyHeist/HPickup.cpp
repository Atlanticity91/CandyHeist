/**
 *
 * MIT License
 *
 * Copyright (c) 2020 Alves Quentin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **/

#include "HPickup.h"
#include "HBonus.h"
#include "HCharacter.h"
#include "HFramework.h"
#include "HInventory.h"
#include "HGameMode.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Net/UnrealNetwork.h"

//////////////////////////////////////////////////////////////////////////////////////
//		PUBLIC 
//////////////////////////////////////////////////////////////////////////////////////
AHPickup::AHPickup( ) {
	this->MeshComp = CreateDefaultSubobject<UStaticMeshComponent>( FName( "Mesh Comp" ) );
	this->MeshComp->SetCollisionEnabled( ECollisionEnabled::NoCollision );

	this->DecalComp = CreateDefaultSubobject<UStaticMeshComponent>( FName( "Decal Comp" ) );
	this->DecalComp->SetupAttachment( this->MeshComp );
	this->DecalComp->SetCollisionEnabled( ECollisionEnabled::NoCollision );

	this->TriggerComp = CreateDefaultSubobject<UBoxComponent>( FName( "Trigger Comp" ) );
	this->TriggerComp->SetupAttachment( this->MeshComp );
	this->TriggerComp->SetCollisionEnabled( ECollisionEnabled::QueryOnly );
	this->TriggerComp->SetCollisionResponseToAllChannels( ECR_Ignore );
	this->TriggerComp->SetCollisionResponseToChannel( ECC_Pawn, ECR_Overlap );
	this->TriggerComp->OnComponentBeginOverlap.AddDynamic( this, &AHPickup::HandleOverlap );

	this->RootComponent = this->MeshComp;
	this->IsActive = true;
	this->SetReplicates( true );
}

void AHPickup::SetItem( AHObject* item ) {
	if ( item )
		this->Item = item;
}

void AHPickup::Enable( ) {
	if ( !this->IsActive ) {
		this->IsActive = true;

		this->OnEnable( );
	}
}

void AHPickup::Disable( ) {
	if ( this->IsActive ) {
		this->IsActive = false;

		this->OnDisable( );
	}
}

void AHPickup::Toggle( ) {
	this->IsActive = !this->IsActive;

	if ( this->IsActive )
		this->OnEnable( );
	else
		this->OnDisable( );
}

//////////////////////////////////////////////////////////////////////////////////////
//		PROTECTED 
//////////////////////////////////////////////////////////////////////////////////////
void AHPickup::BeginPlay( ) {
	Super::BeginPlay( );

	this->MeshComp->SetStaticMesh( this->Mesh );

	this->DecalComp->SetStaticMesh( this->DecalMesh );
	this->DecalComp->SetMaterial( 0, this->Decal );
	this->DecalComp->CreateDynamicMaterialInstance( 0 )->SetVectorParameterValue( "Color", this->DecalColor );
}

void AHPickup::HandleOverlap( UPrimitiveComponent* comp, AActor* other, UPrimitiveComponent* other_comp, int32 other_id, bool from_sweep, const FHitResult& sweep_result ) {
	AHCharacter* player = Cast<AHCharacter>( other );

	if ( player && this->IsActive ) {
		if ( !this->Item )
			this->Item = GetWorld( )->SpawnActor<AHObject>( this->Object );

		if ( !this->Item->IsA( AHBonus::StaticClass( ) ) )
			player->GetInventory( )->Append( this->Item );
		else {
			AHGameMode* game_mode = Cast<AHGameMode>( GetWorld( )->GetAuthGameMode( ) );

			if ( game_mode )
				game_mode->ApplyObject( this->Item );
		}
	}

	this->Destroy( );
}

void AHPickup::OnEnable_Implementation( ) {
	this->DecalComp->SetHiddenInGame( false );
	this->TriggerComp->SetCollisionEnabled( ECollisionEnabled::QueryOnly );
}

void AHPickup::OnDisable_Implementation( ) {
	this->DecalComp->SetHiddenInGame( true );
	this->TriggerComp->SetCollisionEnabled( ECollisionEnabled::NoCollision );
}

//////////////////////////////////////////////////////////////////////////////////////
//		PUBLIC GET
//////////////////////////////////////////////////////////////////////////////////////
bool AHPickup::GetIsActive( ) const { return this->IsActive; }

AHObject* AHPickup::GetItem( ) const { return this->Item; }

TSubclassOf<AHObject> AHPickup::GetType( ) const { return this->Object; }
