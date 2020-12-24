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

#include "HFramework.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////////////////
//		PUBLIC STATIC
//////////////////////////////////////////////////////////////////////////////////////
bool UHFramework::Is( UObject* query, TSubclassOf<UObject> type ) {
	if ( query )
		return query->IsA( type ) || query->GetClass( )->IsChildOf( type->GetClass( ) );

	return false;
}

bool UHFramework::IsServer( AActor* actor ) {
	return ( actor ) ? actor->GetLocalRole( ) == ROLE_Authority : false;
}

void UHFramework::LaunchPawn( ACharacter* pawn, const FVector& direction, const float power ) {
	if ( pawn ) {
		FVector launch_power = FVector{ 0.f };

		if ( direction.X != 0.f ) {
			if ( direction.X > 0.f )
				launch_power.X = power;
			else
				launch_power.X = -( power );
		}
			

		if ( direction.Y != 0.f ) {
			if ( direction.Y > 0.f )
				launch_power.Y = power;
			else
				launch_power.Y = -( power );
		}

		pawn->LaunchCharacter( launch_power, true, false );
	}
}

void UHFramework::ClearTimers( UObject* context ) {
	if ( context )
		context->GetWorld( )->GetTimerManager( ).ClearAllTimersForObject( context );
}
