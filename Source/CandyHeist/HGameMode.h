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

#pragma once

#include "CoreMinimal.h"
#include "HTimer.h"
#include "GameFramework/GameModeBase.h"
#include "HGameMode.generated.h"

#define HPLAYER_COUNT 4
#define HSCORE_COUNT 3 

class AHBonus;
class AHCandy;
class AHObject;
class AHPlayer;
struct HTimer;

UCLASS( )
class CANDYHEIST_API AHGameMode : public AGameModeBase {

	GENERATED_BODY( )
	
protected:
	UPROPERTY( EditDefaultsOnly, Category = "HFramework|Gameplay" )
	FColor PlayersColors[ HPLAYER_COUNT ];
	
	UPROPERTY( EditDefaultsOnly, Category = "HFramework|Gameplay" )
	int TargetsScore[ HSCORE_COUNT ];

	UPROPERTY( EditDefaultsOnly, Category = "HFramework|Gameplay" )
	float MaxMultiplicator = 4.f;

private:
	HTimer Timer;
	int Score;
	float Multiplicator = 1.f;
	
public:
	AHGameMode( );

	UFUNCTION( BlueprintNativeEvent, Category = "HFramework|Object" )
	void OnLevelStart( );

	UFUNCTION( BlueprintNativeEvent, Category = "HFramework|Object" )
	void OnLevelStop( );

	UFUNCTION( BlueprintCallable, Category = "HFramework|Object" )
	void ApplyMultiplicator( float value );

	UFUNCTION( BlueprintCallable, Category = "HFramework|Object" )
	void RemoveMultiplicator( float value );

	UFUNCTION( BlueprintCallable, Category = "HFramework|GameMode" )
	void ApplyObject( AHObject* object );

	UFUNCTION( BlueprintCallable, Category = "HFramework|GameMode" )
	void EnablePickups( TSubclassOf<AHObject> type );

	UFUNCTION( BlueprintCallable, Category = "HFramework|GameMode" )
	void DisablePickups( TSubclassOf<AHObject> type );

	UFUNCTION( BlueprintCallable, Category = "HFramework|GameMode" )
	void TogglePickups( TSubclassOf<AHObject> type );

protected:
	virtual void OnLevelStart_Implementation( );

	virtual void OnLevelStop_Implementation( );

private:
	void ApplyBonus( AHBonus* bonus );

	void ApplyCandy( AHCandy* candy );

public:
	template< typename Type = AHObject >
	void EnablePickups( ) {
		this->EnablePickups( Type::StaticClass( ) );
	}

	template< typename Type = AHObject >
	void DisablePickups( ) {
		this->DisablePickups( Type::StaticClass( ) );
	}

	template< typename Type = AHObject >
	void TogglePickups( ) {
		this->TogglePickups( Type::StaticClass( ) );
	}

public:
	UFUNCTION( BlueprintCallable, Category = "HFramework|GameMode" )
	FColor GetPlayerColor( int player_id ) const;

	UFUNCTION( BlueprintCallable, Category = "HFramework|GameMode" )
	int GetTargetScore( int score_id ) const;

	UFUNCTION( BlueprintCallable, Category = "HFramework|GameMode" )
	int GetScore( ) const;

};
