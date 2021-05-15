// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "GameFramework\SpringArmComponent.h"
#include "Camera\CameraComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	this->JumpMaxHoldTime = 0.5;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(this->RootComponent);
	CameraBoom->TargetArmLength = 300.0f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	CameraBoom->bUsePawnControlRotation = true;
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent); //prüfen, ob Speicher belegt ist
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("LayDown", IE_Pressed, this, &AMyCharacter::setLaying);
	PlayerInputComponent->BindAction("StandUp", IE_Pressed, this, &AMyCharacter::setStanding);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

}

void AMyCharacter::MoveForward(float value)
{
	if(this->Controller != nullptr && value != 0.0f && !bLaying)
	{
		const FRotator Rotation = this->Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		this->AddMovementInput(Direction, value);
	}
}

void AMyCharacter::MoveRight(float value)
{
		if(this->Controller != nullptr && value != 0.0f && !bLaying)
	{
		const FRotator Rotation = this->Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		this->AddMovementInput(Direction, value);
	}
}

void AMyCharacter::setLaying()
{
	bLaying = true;
}

void AMyCharacter::setStanding()
{
	bLaying = false;
}

void AMyCharacter::Jump()
{
	if(bLaying != true) 
	{
	bPressedJump = true;
	JumpKeyHoldTime = 0.0f;
	}
}
