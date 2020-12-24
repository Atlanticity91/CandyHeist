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

#include "HInventory.h"
#include "HCharacter.h"
#include "HObject.h"
#include "Components/StaticMeshComponent.h"

//////////////////////////////////////////////////////////////////////////////////////
//		PUBLIC
//////////////////////////////////////////////////////////////////////////////////////
UHInventory::UHInventory( ) {
	for ( int idx = 0; idx < HINV_SIZE; idx++ ) {
		auto name = FName( FString( "Mesh Slot " ) + FString::FromInt( idx ) );

		this->Slots[ idx ] = CreateDefaultSubobject<UStaticMeshComponent>( name );
		this->Slots[ idx ]->SetupAttachment( this );
		this->Slots[ idx ]->SetVisibility( false );
		this->Slots[ idx ]->SetCollisionEnabled( ECollisionEnabled::NoCollision );

		this->Content[ idx ] = nullptr;
	}
}

void UHInventory::Prepare( ) {
	for ( int idx = 0; idx < HINV_SIZE; idx++ ) {
		if ( idx < this->SlotsPositions.Num( ) )
			this->Slots[ idx ]->AddWorldOffset( this->SlotsPositions[ idx ] );
	}
}

int UHInventory::Append( AHObject* object ) {
	if ( object && !this->GetIsFull( ) ) {
		for ( int idx = 0; idx < HINV_SIZE; idx++ ) {
			if ( !this->Content[ idx ] ) {
				AHCharacter* player = Cast<AHCharacter>( GetOwner( ) );

				this->SetSlot( idx, object );
				this->Content[ idx ]->OnPickObject( player );

				return idx;
			}
		}
	}

	return HINV_SIZE;
}

AHObject* UHInventory::Swap( AHObject* object, int slot_id ) {
	if ( slot_id > -1 && slot_id < HINV_SIZE ) {
		AHObject* _object = this->Content[ slot_id ];

		this->SetSlot( slot_id, object );

		return _object;
	}

	return nullptr;
}

AHObject* UHInventory::RetriveFirst( ) {
	for ( int idx = 0; idx < HINV_SIZE; idx++ ) {
		if ( !this->Content[ idx ] )
			continue;
		else {
			AHObject* object = this->Content[ idx ];

			this->SetSlot( idx, nullptr );

			return object;
		}
	}

	return nullptr;
}

AHObject* UHInventory::Retrive( int slot_id ) {
	if ( slot_id > -1 && slot_id < HINV_SIZE ) {
		AHObject* object = this->Content[ slot_id ];

		this->SetSlot( slot_id, nullptr );

		return object;
	}

	return nullptr;
}

void UHInventory::Show( ) {
	for ( int idx = 0; idx < HINV_SIZE; idx++ ) {
		if ( this->Content[ idx ] )
			this->Slots[ idx ]->SetVisibility( true );
	}
}

void UHInventory::Hide( ) {
	for ( int idx = 0; idx < HINV_SIZE; idx++ )
		this->Slots[ idx ]->SetVisibility( false );
}

//////////////////////////////////////////////////////////////////////////////////////
//		PRIVATE
//////////////////////////////////////////////////////////////////////////////////////
void UHInventory::SetSlot( int slot_id, AHObject* object ) {
	if ( object ) 
		this->Slots[ slot_id ]->SetStaticMesh( object->GetMesh( ) );
	
	this->Content[ slot_id ] = object;
}

//////////////////////////////////////////////////////////////////////////////////////
//		PUBLIC GET
//////////////////////////////////////////////////////////////////////////////////////
int UHInventory::GetLength( ) const { return HINV_SIZE; }

int UHInventory::GetSize( ) const {
	int count = 0;

	for ( int idx = 0; idx < HINV_SIZE; idx++ ) {
		if ( this->Content[ idx ] )
			count += 1;
	}

	return count;
}

bool UHInventory::GetIsFull( ) const { return this->GetSize( ) == HINV_SIZE; }

bool UHInventory::Contain( TSubclassOf<AHObject> type, int& count ) {
	count = 0;

	for ( int idx = 0; idx < HINV_SIZE; idx++ ) {
		if ( this->Content[ idx ] && this->Content[ idx ]->IsA( type ) )
			count += 1;
	}

	return count > 0;
}

bool UHInventory::Next( TSubclassOf<AHObject> type, int previous, int& slot_id ) {
	if ( previous < 0 || previous >= HINV_SIZE )
		previous = 0;

	while ( previous < HINV_SIZE ) {
		if ( !this->Content[ previous ] || !this->Content[ previous ]->IsA( type ) )
			previous += 1;
		else {
			slot_id = previous;

			return true;
		}
	}

	return false;
}
