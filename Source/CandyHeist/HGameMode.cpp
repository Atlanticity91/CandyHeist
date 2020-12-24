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

#include "HGameMode.h"
#include "HBonus.h"
#include "HCandy.h"
#include "HCharacter.h"
#include "HFramework.h"
#include "HPickup.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

//////////////////////////////////////////////////////////////////////////////////////
//		PUBLIC 
//////////////////////////////////////////////////////////////////////////////////////
AHGameMode::AHGameMode( ) {
	this->Timer.Handle = FTimerHandle( );
	this->Timer.Delegate = FTimerDelegate( );
}

void AHGameMode::ApplyMultiplicator( float value ) {
	value = abs( value );

	this->Multiplicator = FMath::Clamp( this->Multiplicator + value, 0.f, this->MaxMultiplicator );
}

void AHGameMode::RemoveMultiplicator( float value ) {
	value = abs( value );

	this->Multiplicator = FMath::Clamp( this->Multiplicator - value, 0.f, this->MaxMultiplicator );
}

void AHGameMode::ApplyObject( AHObject* object ) {
	if ( object ) {
		if ( object->IsA( AHBonus::StaticClass( ) ) )
			this->ApplyBonus( Cast<AHBonus>( object ) );
		else if ( object->IsA( AHCandy::StaticClass( ) ) )
			this->ApplyCandy( Cast<AHCandy>( object ) );
	}
}

void AHGameMode::EnablePickups( TSubclassOf<AHObject> type ) {
	UHFramework::ForeachActors<AHPickup>(
		this,
		[ & ]( AHPickup*& pickup ) {
			if ( pickup->GetType( )->IsChildOf( type ) && !pickup->GetIsActive( ) )
				pickup->Enable( );
		}
	);
}

void AHGameMode::DisablePickups( TSubclassOf<AHObject> type ) {
	UHFramework::ForeachActors<AHPickup>(
		this,
		[ & ]( AHPickup*& pickup ) {
			if ( pickup->GetType( )->IsChildOf( type ) && pickup->GetIsActive( ) )
				pickup->Disable( );
		}
	);
}

void AHGameMode::TogglePickups( TSubclassOf<AHObject> type ) {
	UHFramework::ForeachActors<AHPickup>(
		this,
		[ & ]( AHPickup*& pickup ) {
			if ( pickup->GetType( )->IsChildOf( type ) )
				pickup->Toggle( );
		}
	);
}

//////////////////////////////////////////////////////////////////////////////////////
//		PROTECTED  
//////////////////////////////////////////////////////////////////////////////////////
void AHGameMode::OnLevelStart_Implementation( ) { 
	int color_idx = 0;

	UHFramework::ForeachActors<AHCharacter>(
		this,
		[ ]( AHCharacter*& player, FColor color, int& color_idx ) {
			player->SetColor( color );

			if ( color_idx < HPLAYER_COUNT )
				color_idx += 1;
		}, this->PlayersColors[ color_idx ], color_idx
	);
}

void AHGameMode::OnLevelStop_Implementation( ) { }

//////////////////////////////////////////////////////////////////////////////////////
//		PRIVATE
//////////////////////////////////////////////////////////////////////////////////////
void AHGameMode::ApplyBonus( AHBonus* bonus ) {
	this->DisablePickups<AHBonus>( );

	bonus->OnBonusStart( );

	UHFramework::ForeachActors<AHCharacter>(
		this,
		[ ]( AHCharacter*& player, AHBonus* bonus ) {
			bonus->OnBonusIsApplyTo( player );
		}, bonus
	);

	UHFramework::Delay(
		this, this->Timer, bonus->GetDuration( ),
		[ ]( AHGameMode* game_mode, AHBonus* bonus ) {
			UHFramework::ForeachActors<AHCharacter>(
				bonus,
				[ ]( AHCharacter*& player, AHBonus* bonus ) {
					bonus->OnBonusIsRemoveOf( player );
				}, bonus
			);

			bonus->OnBonusStop( );
			bonus->Destroy( );

			game_mode->EnablePickups<AHBonus>( );
		}, this, bonus 
	);
}

void AHGameMode::ApplyCandy( AHCandy* candy ) {
	this->Score += (int)( candy->GetScore( ) * this->Multiplicator );

	if ( GEngine )
		GEngine->AddOnScreenDebugMessage( -1, 15.0f, FColor::Yellow, FString::Printf( TEXT( "Score : %d" ), this->Score ) );

	candy->Destroy( );
}

//////////////////////////////////////////////////////////////////////////////////////
//		PUBLIC GET
//////////////////////////////////////////////////////////////////////////////////////
FColor AHGameMode::GetPlayerColor( int player_id ) const { 
	return this->PlayersColors[ player_id % HPLAYER_COUNT ];
}

int AHGameMode::GetTargetScore( int score_id ) const { 
	return this->TargetsScore[ score_id % HSCORE_COUNT ];
}

int AHGameMode::GetScore( ) const { return this->Score; }
