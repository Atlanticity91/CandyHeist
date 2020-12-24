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

#include "HExtraction.h"
#include "HCandy.h"
#include "HGameMode.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "Materials/MaterialInterface.h"

//////////////////////////////////////////////////////////////////////////////////////
//		PUBLIC 
//////////////////////////////////////////////////////////////////////////////////////
AHExtraction::AHExtraction( ) {
	this->DecalComp = CreateDefaultSubobject<UDecalComponent>( FName( "Decal Comp" ) );
	this->DecalComp->DecalSize = FVector( 200.0f, 200.0f, 200.0f );

	this->TriggerComp = CreateDefaultSubobject<USphereComponent>( FName( "Trigger Comp" ) );
	this->TriggerComp->SetupAttachment( this->DecalComp );
	this->TriggerComp->SetCollisionEnabled( ECollisionEnabled::QueryOnly );
	this->TriggerComp->SetCollisionResponseToAllChannels( ECR_Overlap );
	this->TriggerComp->OnComponentBeginOverlap.AddDynamic( this, &AHExtraction::HandleOverlap );

	this->RootComponent = this->DecalComp;
}

//////////////////////////////////////////////////////////////////////////////////////
//		PROTECTED 
//////////////////////////////////////////////////////////////////////////////////////
void AHExtraction::BeginPlay( ) {
	Super::BeginPlay( );
}

void AHExtraction::HandleOverlap( UPrimitiveComponent* comp, AActor* other, UPrimitiveComponent* other_comp, int32 other_id, bool from_sweep, const FHitResult& sweep_result ) {
	AHCandy* candy = Cast<AHCandy>( other );

	if ( candy ) { 
		AHGameMode* game_mode = Cast<AHGameMode>( GetWorld( )->GetAuthGameMode( ) );

		if ( game_mode )
			game_mode->ApplyObject( candy );
	}
}
