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
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HFramework.generated.h"

class AActor;
struct HTimer;

UCLASS( )
class CANDYHEIST_API UHFramework : public UBlueprintFunctionLibrary {

	GENERATED_BODY( )

public:
	static bool Is( UObject* query, TSubclassOf<UObject> type );

	UFUNCTION( BlueprintCallable, Category="HFramework" )
	static bool IsServer( AActor* actor );

	UFUNCTION( BlueprintCallable, Category="HFramework" )
	static void LaunchPawn( ACharacter* pawn, const FVector& direction, const float power );

	static void ClearTimers( UObject* context );

public:
	template< typename Type = UObject >
	static bool Is( UObject* query ) { 
		return UHFramework::Is( query, Type::StaticClass( ) );
	};

	template< typename Type, typename Callback, typename... Args >
	static Type* SpawnActor( UObject* context, TSubclassOf<Type> sub_class, Callback&& callback, Args... args ) {
		if ( context ) {
			Type* actor = context->GetWorld( )->SpawnActor<Type>( sub_class );

			if ( actor )
				callback( actor, args... );

			return actor;
		}

		return nullptr;
	};

	template< typename Type, typename Callback, typename... Args >
	static Type* SpawnActor( UObject* context, TSubclassOf<Type> sub_class, FTransform& transform, Callback&& callback, Args... args ) {
		if ( context ) {
			Type* actor = context->GetWorld( )->SpawnActor<Type>( sub_class, transform );

			if ( actor )
				callback( actor, args... );

			return actor;
		}

		return nullptr;
	};

	template<typename Type>
	static TArray<Type*> GetAllActors( UObject* context ) {
		TArray<AActor*> loader;

		UGameplayStatics::GetAllActorsOfClass( context->GetWorld( ), Type::StaticClass( ), loader );

		TArray<Type*> elements;
		for ( auto& element : loader )
			elements.Add( Cast<Type>( element ) );

		return elements;
	};

	template<typename Type, typename Callback, typename... Args >
	static void ForeachActors( UObject* context, Callback&& callback, Args... args ) {
		TArray<Type*> actors = UHFramework::GetAllActors<Type>( context );

		for ( auto& actor : actors ) 
			callback( actor, args... );
	};

	template< typename Callback, typename... Args >
	static void Delay( UObject* context, HTimer& timer, float duration, Callback&& callback, Args... args ) {
		if ( context ) {
			FTimerManager& timer_manager = context->GetWorld( )->GetTimerManager( );

			timer.Delegate.BindLambda( callback, args... );
			timer_manager.ClearTimer( timer.Handle );
			timer_manager.SetTimer( timer.Handle, timer.Delegate, duration, false );
		}
	};

};
