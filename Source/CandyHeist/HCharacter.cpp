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

#include "HCharacter.h"
#include "HFramework.h"
#include "HInventory.h"
#include "HObject.h"
#include "HPickup.h"
#include "HWeapon.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

//////////////////////////////////////////////////////////////////////////////////////
//		PUBLIC 
//////////////////////////////////////////////////////////////////////////////////////
AHCharacter::AHCharacter( ) {
	PrimaryActorTick.bCanEverTick = true;

	this->SpringArm = CreateDefaultSubobject<USpringArmComponent>( TEXT( "Spring Arm Comp" ) );
	this->SpringArm->SetupAttachment( this->RootComponent );
	this->SpringArm->bDoCollisionTest = false;
	this->SpringArm->TargetArmLength = 600.f;
	this->SpringArm->SetWorldLocation( FVector{ -70.f,   0.f, 122.f } );
	this->SpringArm->SetWorldRotation( FVector{   0.f, -60.f,   0.f }.Rotation( ) );

	this->Camera = CreateDefaultSubobject<UCameraComponent>( TEXT( "Camera Comp" ) );
	this->Camera->SetupAttachment( this->SpringArm );

	this->Decal = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Decal Comp" ) );
	this->Decal->SetupAttachment( this->GetMesh( ) );

	FAttachmentTransformRules TransformRules = FAttachmentTransformRules{ EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, false };
	this->ItemHolder = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Item Holder" ) );
	this->ItemHolder->AttachToComponent( this->GetMesh( ), TransformRules, this->ItemSocket );

	this->Inventory = CreateDefaultSubobject<UHInventory>( TEXT( "Inventory Comp" ) );
	this->Inventory->SetupAttachment( this->RootComponent );

	this->IsInventoryVisible = false;
	this->IsDashActivate = false;
}

void AHCharacter::SetColor( FColor value ) {
	this->Decal->CreateDynamicMaterialInstance( 0 )->SetVectorParameterValue( this->PlayerColorLabel, value );
}

void AHCharacter::SetItem( int slot_id ) {
	AHObject* item = this->Inventory->Retrive( slot_id );

	if ( item ) {
		this->DropItem( );
		this->OnRep_Item( item );
	}
}

void AHCharacter::EnableDash( ) { this->OnRep_Dash( true ); }

void AHCharacter::DisableDash( ) { this->OnRep_Dash( false ); }

void AHCharacter::ToggleDash( ) { this->OnRep_Dash( !this->IsDashActivate ); }

void AHCharacter::Tick( float delta_time ) {
	Super::Tick( delta_time );
	
	FVector query_dir = this->GetVelocity( );

	if ( query_dir.X != 0.f || query_dir.Y != 0.f ) {
		FRotator source = this->GetMesh( )->GetComponentRotation( );
		FRotator target = query_dir.Rotation( );
		FRotator rotation = FMath::Lerp( source, target, this->RotationSpeed * delta_time );
		FQuat rot = rotation.Quaternion( );

		this->SetActorRotation( rot );
	}

	this->OnRep_Direction( query_dir );
}

void AHCharacter::SetupPlayerInputComponent( UInputComponent* input_comp ) {
	Super::SetupPlayerInputComponent( input_comp );

	input_comp->BindAxis( "MoveForward", this, &AHCharacter::MoveForward );
	input_comp->BindAxis( "MoveRight", this, &AHCharacter::MoveRight );

	input_comp->BindAction( "Use", IE_Pressed, this, &AHCharacter::UseItem );
	input_comp->BindAction( "Inventory", IE_Pressed, this, &AHCharacter::UseInventory );
	input_comp->BindAction( "Dash", IE_Pressed, this, &AHCharacter::UseDash );
}

//////////////////////////////////////////////////////////////////////////////////////
//		PROTECTED 
//////////////////////////////////////////////////////////////////////////////////////
void AHCharacter::BeginPlay( ) {
	Super::BeginPlay( );

	if ( this->Inventory )
		this->Inventory->Prepare( );
}

void AHCharacter::MoveForward( float value ) {
	if ( !this->IsInventoryVisible && value != 0.f )
		this->AddMovementInput( this->GetActorForwardVector( ), value );
}

void AHCharacter::MoveRight( float value ) {
	if ( !this->IsInventoryVisible && value != 0.f )
		this->AddMovementInput( this->GetActorRightVector( ), value );
}

void AHCharacter::DropItem_Implementation( ) {
	if ( this->Item ) {
		this->Item->OnDropObject( this );

		FTransform transform = this->GetTransform( );
		transform.AddToTranslation( this->Direction * 0.5f );

		UHFramework::SpawnActor<AHPickup>(
			this,
			this->Item->GetPickup( ),
			transform,
			[ & ]( AHPickup*& pickup ) { 
				if ( pickup )
					pickup->SetItem( this->Item );
			} 
		);

		this->OnRep_Item( nullptr );
	}
}

void AHCharacter::UseItem_Implementation( ) {
	if ( !this->Item && this->Inventory->GetSize( ) > 0 )
		this->OnRep_Item( this->Inventory->RetriveFirst( ) );

	if ( this->Item ) {
		if ( UHFramework::Is<AHWeapon>( this->Item ) ) {
			AHWeapon* weapon = Cast<AHWeapon>( this->Item );

			if ( weapon && weapon->UseWeapon( this ) ) {
				weapon->Destroy( );

				this->OnRep_Item( nullptr );
			}
		} else
			this->DropItem( );
	}
}

void AHCharacter::UseInventory_Implementation( ) {
	if ( !this->IsInventoryVisible )
		this->OnOpenInventory( );
	else
		this->OnCloseInventory( );

	this->OnRep_Inventory( !this->IsInventoryVisible );
}

void AHCharacter::UseDash_Implementation( ) {
	if ( !this->IsInventoryVisible && this->IsDashActivate ) {
		this->OnCharacterDash( );

		if ( UHFramework::IsServer( this ) ) 
			UHFramework::LaunchPawn( this, this->Direction, 6000.f );
	}
}

void AHCharacter::OnOpenInventory_Implementation( ) {
	if ( this->Inventory )
		this->Inventory->Show( );
}

void AHCharacter::OnCloseInventory_Implementation( ) {
	if ( this->Inventory )
		this->Inventory->Hide( );
}

void AHCharacter::OnCharacterDash_Implementation( ) {
}

//////////////////////////////////////////////////////////////////////////////////////
//		PUBLIC GET
//////////////////////////////////////////////////////////////////////////////////////
FVector AHCharacter::GetDirection( ) const { return this->Direction; }

UHInventory* AHCharacter::GetInventory( ) const { return this->Inventory; }

bool AHCharacter::GetIsHoldingItem( ) const { return this->Item != nullptr; }

AHObject* AHCharacter::GetItem( ) const { return this->Item; }

bool AHCharacter::GetIsInventoryVisible( ) const { return this->IsInventoryVisible; }

bool AHCharacter::GetIsDashActivate( ) const { return this->IsDashActivate; }

//////////////////////////////////////////////////////////////////////////////////////
//		INTERNAL
//////////////////////////////////////////////////////////////////////////////////////
void AHCharacter::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const {
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( AHCharacter, Item );
	DOREPLIFETIME( AHCharacter, Direction );
	DOREPLIFETIME( AHCharacter, IsInventoryVisible );
	DOREPLIFETIME( AHCharacter, IsDashActivate );
}

void AHCharacter::OnRep_Item( AHObject* object ) {
	if ( object ) {
		this->ItemHolder->SetStaticMesh( object->GetMesh( ) );
		this->ItemHolder->SetVisibility( true );
		this->Item = object;
	} else {
		this->ItemHolder->SetStaticMesh( nullptr );
		this->ItemHolder->SetVisibility( false );
		this->Item = nullptr;
	}
}

void AHCharacter::OnRep_Direction( FVector target ) { this->Direction = target;  }

void AHCharacter::OnRep_Inventory( bool state ) { this->IsInventoryVisible = state; }

void AHCharacter::OnRep_Dash( bool state ) {
	if ( this->IsDashActivate != state )
		this->IsDashActivate = state;
}
