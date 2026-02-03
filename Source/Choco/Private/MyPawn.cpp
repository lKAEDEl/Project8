#include "MyPawn.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"

//생성자
AMyPawn::AMyPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	//캡슐컴포넌트
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = Capsule;

	Capsule->InitCapsuleSize(42.f, 96.f);
	Capsule->SetSimulatePhysics(false);

	//메쉬
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
	Mesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	Mesh->SetSimulatePhysics(false);

	//스프링암
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = false;

	//카메라
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;
}

//시작됬을떄
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LP = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LP))
			{
				if (DefaultMappingContext)
				{
					Subsystem->AddMappingContext(DefaultMappingContext, 0);
				}
			}
		}
	}
}

//틱
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsJumping)
	{
		VerticalVelocity += Gravity * DeltaTime;

		FVector NewLocation = GetActorLocation();
		NewLocation.Z += VerticalVelocity * DeltaTime;

		float GroundLevel = 96.0f;

		if (NewLocation.Z <= GroundLevel)
		{
			NewLocation.Z = GroundLevel;
			bIsJumping = false;
			VerticalVelocity = 0.0f;
		}

		SetActorLocation(NewLocation);
	}
}


//입력바인딩
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput =
		Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EnhancedInput->BindAction(
				MoveAction,
				ETriggerEvent::Triggered,
				this,
				&AMyPawn::Move
			);
		}

		if (LookAction)
		{
			EnhancedInput->BindAction(
				LookAction,
				ETriggerEvent::Triggered,
				this,
				&AMyPawn::Look
			);
		}
		if (JumpAction)
		{
			EnhancedInput->BindAction(
				JumpAction,
				ETriggerEvent::Triggered,
				this,
				&AMyPawn::Jump
			);
		}
	}
}

//이동
void AMyPawn::Move(const FInputActionValue& Value)
{
	const FVector2D Input = Value.Get<FVector2D>();

	FVector MoveDir =
		GetActorForwardVector() * Input.Y +
		GetActorRightVector() * Input.X;

	AddActorWorldOffset(
		MoveDir * MoveSpeed * GetWorld()->GetDeltaSeconds(),
		true
	);
}

//시야회전
void AMyPawn::Look(const FInputActionValue& Value)
{
	const FVector2D Input = Value.Get<FVector2D>();

	//몸 회전
	FRotator ActorRot = GetActorRotation();
	ActorRot.Yaw += Input.X;
	SetActorRotation(ActorRot);

	//카메라(SpringArm) 회전
	FRotator ArmRot = SpringArm->GetRelativeRotation();
	ArmRot.Pitch = FMath::Clamp(
		ArmRot.Pitch - Input.Y,
		-80.f,
		80.f
	);
	SpringArm->SetRelativeRotation(ArmRot);
}

//점프
void AMyPawn::Jump()
{
	if (!bIsJumping)
	{
		bIsJumping = true;
		VerticalVelocity = JumpPower;
	}
}
