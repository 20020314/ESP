#ifndef SanjaySrc_h
#define SanjaySrc_h
 
namespace Offsets64 {
	//Vietnam
	uintptr_t GWorldVN = 0xAE87740;
	uintptr_t GNamesVN = 0xAA272D0;
    uintptr_t ViewMatrixVN = 0xAE65000;
	//Global and others
    uintptr_t GWorld = 0xAE74AC0;
	uintptr_t GNames = 0xAA14650;
    uintptr_t ViewMatrix = 0xAE52380;
	
    uintptr_t ViewMatrixLevel = 0xc0;
    uintptr_t Position = 0x1b0; // int[]  
    uintptr_t RootComponent = 0x1c0; //  -- updated
    uintptr_t SkeletalMeshComponent = 0x438; // * Mesh
    uintptr_t FixAttachInfoList = 0x1a0; // 
    uintptr_t MinLOD = 0x748; // int ?
    uintptr_t Health = 0xce0; // float  -- updated
    uintptr_t bDead = 0xcfc; // bool 
    //Class: 
    uintptr_t PlayerName = 0x890; // FString 
    uintptr_t TeamID = 0x8d8; // int 
    uintptr_t Role = 0x148;//byte 
    uintptr_t bIsAI = 0x968; // bool 
    uintptr_t ObserverCameraFPPMode = 0x3f78; //* 
    uintptr_t ObserverLevel = 0x1b0;
    // Class: 
    uintptr_t ActorChildren = 0x1b0; // Actor*[] 
    uintptr_t WeaponEntityComp = 0xf48; //*  
    ///Class:  
    uintptr_t DrawShootLineTime = 0x134; // 
    uintptr_t UploadInterval = 0x170; // 
    uintptr_t CurBulletNumInClip = 0xe00; // int 
    uintptr_t bIsGunADS = 0xf69;// bool 
    uintptr_t bIsWeaponFiring = 0x1500; //bool 
    uintptr_t Controller = 0x410; //Class:  to * 
    uintptr_t ControlRotation = 0x408;//Class:  to Rotator 
	
	uintptr_t cLoc = 0x750;
	uintptr_t fovPntr = 0x660;
	uintptr_t vMatrix = 0x590;
	//uintptr_t oType = 0x60;
	
	uintptr_t WeaponManagerComponent = 0x1ec8; //* 
	uintptr_t CurrentWeaponReplicated = 0x580; //* 
	uintptr_t ShootWeaponEntityComp = 0xf48;  //* 
	
	//Class: 
	uintptr_t BulletFireSpeed = 0x4d0;
	
	uintptr_t AccessoriesVRecoilFactor = 0xa20;
	uintptr_t AccessoriesHRecoilFactor = 0xa24;
	uintptr_t AccessoriesRecoveryFactor = 0xa28;
	uintptr_t RecoilKickADS = 0xb30 ;
	uintptr_t ExtraHitPerformScale = 0xb2c;
    
	uintptr_t ShootInterval = 0x4f8;
	
	uintptr_t GameDeviationFactor = 0xa7c;
	
    uintptr_t AnimationKick = 0xb4c;
    
	uintptr_t SwitchWeaponSpeedScale = 0x2418; //float  
}
 
#endif
