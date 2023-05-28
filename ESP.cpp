#include "INCLUDE/struct.h"
#include "Offsets.h"
 
void p_write(uintptr_t address, void* buffer, int size) {
    struct iovec local[1];
    struct iovec remote[1];
    local[0].iov_base = (void*)buffer;
    local[0].iov_len = size;
    remote[0].iov_base = (void*)address;
    remote[0].iov_len = size;
    process_vm_writev(pid, local, 1, remote, 1, 0);
}
 
Vec2 getPointingAngle(Vec3 camera, Vec3 xyz, float distance)
{
    Vec2 PointingAngle;
    float ytime = distance / 88000;
    xyz.Z = xyz.Z + 360 * ytime * ytime;
    float zbcx = xyz.X - camera.X;
    float zbcy = xyz.Y - camera.Y;
    float zbcz = xyz.Z - camera.Z;
    PointingAngle.Y = atan2(zbcy, zbcx) * 180 / PI;
    PointingAngle.X = atan2(zbcz, sqrt(zbcx * zbcx + zbcy * zbcy)) * 180 / PI;
    return PointingAngle;
}
 
uintptr_t GWorld = 0, GNames = 0, ViewMatrix = 0;
uintptr_t getMatrix(uintptr_t base)
{
    return getA(getA(base + ViewMatrix) + Offsets64::ViewMatrixLevel);
}
 
uintptr_t getEntityAddr(uintptr_t base) {
	return getA(getA(getA(base + GWorld) + 0x10) + 0x30) + 0xA0;
}
 
char* getNameByte(uintptr_t address)
{
    char static lj[64];
    memset(lj, 0, 64);
    unsigned short int nameI[32];
    pvm(address, nameI, sizeof(nameI));
    char s[10];
    int i;
    for (i = 0; i < 32; i++)
    {
        if (nameI[i] == 0)
            break;
        sprintf(s, "%d:", nameI[i]);
        strcat(lj, s);
    }
    lj[63] = '\0';
 
    return lj;
}
 
PlayerBone getPlayerBone(uintptr_t pBase, struct D3DMatrix viewMatrix)
{
    PlayerBone b;
    b.isBone = true;
    struct D3DMatrix oMatrix;
    uintptr_t boneAddr = getA(pBase + Offsets64::SkeletalMeshComponent); / @gggg || @gggg
    struct D3DMatrix baseMatrix = getOMatrix(boneAddr + Offsets64::FixAttachInfoList);
    int bones[] = {6, 5, 2, 12, 33, 13, 34, 64, 63, 53, 57, 54, 58, 55, 59};
    boneAddr = getA(boneAddr + Offsets64::MinLOD);
    // neck 0
    oMatrix = getOMatrix(boneAddr + (bones[0]) * 48);
    b.neck = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));
    // cheast 1
    oMatrix = getOMatrix(boneAddr + (bones[1]) * 48);
    b.cheast = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));
    // pelvis 2
    oMatrix = getOMatrix(boneAddr + (bones[2]) * 48);
    b.pelvis = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));
    // lSh 3
    oMatrix = getOMatrix(boneAddr + (bones[3]) * 48);
    b.lSh = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));
    // rSh 4
    oMatrix = getOMatrix(boneAddr + (bones[4]) * 48);
    b.rSh = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));
    // lElb 5
    oMatrix = getOMatrix(boneAddr + (bones[5]) * 48);
    b.lElb = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));
    // rElb 6
    oMatrix = getOMatrix(boneAddr + (bones[6]) * 48);
    b.rElb = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));
    // lWr 7
    oMatrix = getOMatrix(boneAddr + (bones[7]) * 48);
    b.lWr = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));
    // rWr 8
    oMatrix = getOMatrix(boneAddr + (bones[8]) * 48);
    b.rWr = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));
    // lTh 9
    oMatrix = getOMatrix(boneAddr + (bones[9]) * 48);
    b.lTh = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));
    // rTh 10
    oMatrix = getOMatrix(boneAddr + (bones[10]) * 48);
    b.rTh = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));
    // lKn 11
    oMatrix = getOMatrix(boneAddr + (bones[11]) * 48);
    b.lKn = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));
    // rKn 12
    oMatrix = getOMatrix(boneAddr + (bones[12]) * 48);
    b.rKn = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));
    // lAn 13
    oMatrix = getOMatrix(boneAddr + (bones[13]) * 48);
    b.lAn = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));
    // rAn 14
    oMatrix = getOMatrix(boneAddr + (bones[14]) * 48);
    b.rAn = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));
    //root
    oMatrix = getOMatrix(boneAddr + (0) * 48);
    b.root = World2ScreenMain(viewMatrix, mat2Cord(oMatrix, baseMatrix));
 
    return b;
}
 
PlayerWeapon getPlayerWeapon(uintptr_t base) {
    PlayerWeapon p;
    uintptr_t addr[3];
    pvm(getA(base + Offsets64::ActorChildren), addr, sizeof(addr));
 
    if (isValid64(addr[0]) && getI(addr[0] + Offsets64::DrawShootLineTime) == 2) {
        p.isWeapon = true;
        p.id = getI(getA(addr[0] + Offsets64::WeaponEntityComp) + Offsets64::UploadInterval);
        p.ammo = getI(addr[0] + Offsets64::CurBulletNumInClip);
    } else if (isValid64(addr[1]) && getI(addr[1] + Offsets64::DrawShootLineTime) == 2) {
        p.isWeapon = true;
        p.id = getI(getA(addr[1] + Offsets64::WeaponEntityComp) + Offsets64::UploadInterval);
        p.ammo = getI(addr[1] + Offsets64::CurBulletNumInClip);
    } else if (isValid64(addr[2]) && getI(addr[2] + Offsets64::DrawShootLineTime) == 2) {
        p.isWeapon = true;
        p.id = getI(getA(addr[2] + Offsets64::WeaponEntityComp) + Offsets64::UploadInterval);
        p.ammo = getI(addr[2] + Offsets64::CurBulletNumInClip);
    }
    return p;
}
 
struct ShootWeaponBase {
    uintptr_t FromBase;
    uintptr_t Base;
    uintptr_t ShootWeaponEntity;
    int bIsWeaponFiring;
    ShootWeaponBase (uintptr_t pBase) {
        FromBase = getA(pBase + Offsets64::WeaponManagerComponent);
        Base = getA(FromBase + Offsets64::CurrentWeaponReplicated);
        ShootWeaponEntity = getA(Base + Offsets64::ShootWeaponEntityComp);
        bIsWeaponFiring = getI(pBase + Offsets64::bIsWeaponFiring);
    }
 
    void setInstantHit() {
        Write(ShootWeaponEntity + Offsets64::BulletFireSpeed, "900000", TYPE_FLOAT);
    }
 
    void setLessRecoil() {
        Write(ShootWeaponEntity + Offsets64::AccessoriesVRecoilFactor,"0", TYPE_FLOAT);
        Write(ShootWeaponEntity + Offsets64::AccessoriesHRecoilFactor, "0", TYPE_FLOAT);
        Write(ShootWeaponEntity + Offsets64::AccessoriesRecoveryFactor, "0", TYPE_FLOAT);
    }
    
    void setZeroRecoil(){
        Write(ShootWeaponEntity + Offsets64::RecoilKickADS, "0", TYPE_FLOAT);
    }
 
    void setFastShootInterval() {
        Write(ShootWeaponEntity + Offsets64::ShootInterval, "0.048000", TYPE_FLOAT);
    }
    
    void setSmallCrosshair() {
        Write(ShootWeaponEntity + Offsets64::GameDeviationFactor,"0", TYPE_FLOAT);
    }
    
    void setHitX() {
        Write(ShootWeaponEntity + Offsets64::ExtraHitPerformScale, "50", TYPE_FLOAT);
    }
 
    void setNoShake() {
        Write(ShootWeaponEntity + Offsets64::AnimationKick, "0", TYPE_FLOAT);
    }
    
    bool isFiring() {
        return (bIsWeaponFiring != 0);
    }
    bool isValid() {
        return (Base != 0);
    }
};
 / @gggg || @gggg
int main()
{
    SetValue sv{};
    if (!isBeta)
    {
        if (!Create())
        {
            perror("Creation Failed");
            return 0;
        }
        if (!Connect())
        {
            perror("Connection Failed");
            return 0;
        }
        int no;
 
        receive((void*)&sv);
        no = sv.mode;
 
        if (no == 1) {
            GNames = Offsets64::GNames;
            GWorld = Offsets64::GWorld;
            ViewMatrix = Offsets64::ViewMatrix;
            strcpy(version, "com.tencent.ig");
        } else if (no == 2) {
            GNames = Offsets64::GNames;
            GWorld = Offsets64::GWorld;
            ViewMatrix = Offsets64::ViewMatrix;
            strcpy(version, "com.pubg.krmobile");
        } else if (no == 3) {
            GNames = Offsets64::GNamesVN;
            GWorld = Offsets64::GWorldVN;
            ViewMatrix = Offsets64::ViewMatrixVN;
            strcpy(version, "com.vng.pubgmobile");
        } else if (no == 4) {
            GNames = Offsets64::GNames;
            GWorld = Offsets64::GWorld;
            ViewMatrix = Offsets64::ViewMatrix;
            strcpy(version, "com.rekoo.pubgm");
        } else if (no == 5) {
            GNames = Offsets64::GNames;
            GWorld = Offsets64::GWorld;
            ViewMatrix = Offsets64::ViewMatrix;
            strcpy(version, "com.pubg.imobile");
        }
    }
 
    pid = getPid(version);
    if (pid < 10)
    {
        printf("\nGame is not running");
        Close();
        return 0;
    } / @gggg || @gggg
    if (isBeta == 1)
        printf("\n Game Pid: %d", pid);
    isPremium = true;
    uintptr_t base = getBase();
    if (isBeta == 1)
        printf("\n Base: %lX\n", base);
    uintptr_t vMatrix = getMatrix(base);
    if (!vMatrix)
    {
        if (isBeta == 1)
            puts("\nMatrix Not Found");
        return 0;
    }
    if (isBeta == 1)
        printf("\nvMatrix: %lX", vMatrix);
 
    uintptr_t enAddrPntr;
    float xy0, xy1;
    struct Vec3 xyz;
    struct Vec3 screen;
    struct Vec3 exyz;
    int isBack = 0, type = 69;
    int changed = 1;
    int myteamID = 101;
    uintptr_t cLoc = vMatrix + Offsets64::cLoc;
    uintptr_t fovPntr = vMatrix + Offsets64::fovPntr;
    vMatrix = vMatrix + Offsets64::vMatrix;
    char loaded[0x4000], loadedpn[20];
    char name[100];
    uintptr_t gname_buff[30];
    uintptr_t gname = getA(getA(base + GNames) + 0x110);
    pvm(gname, &gname_buff, sizeof(gname_buff));
    char cont[0x500];
    char boneData[1024];
    struct D3DMatrix vMat;
    char weaponData[100];
    int firing = 0, ads = 0, adsfiring = 0, adsfiring2;
    float aimRadius = 200;
    Vec2 pointingAngle;
    uintptr_t yawPitch = 0;
    bool aimbot = false;
    int aimFor = 1;
    bool aimKnoced = false;
    int aimBy = 1;
    int aimWhen = 1;
    Request request{};
    Response response{};
 
	bool isLessRecoil = false;
    bool isZeroRecoil = false;
    bool isInstantHit = false;
    bool isFastShootInterval = false;
    bool isSmallCrosshair = false;
    bool isFastSwitchWeapon = false;
    bool isHitX = false;
    bool isNoShake = false;
 
    while (isBeta || (receive((void*)&request) > 0)) {
        if (!isBeta) {
            height = request.ScreenHeight;
            width = request.ScreenWidth;
        }
        if (request.Mode == InitMode) {
            aimRadius = (float)request.options.aimingRange;
            aimFor = request.options.aimbotmode;
            aimbot = request.options.openState == 0;
            aimWhen = request.options.aimingState;
            aimBy = request.options.priority;
            aimKnoced = request.options.pour;
			isLessRecoil = request.memory.LessRecoil;
            isZeroRecoil = request.memory.ZeroRecoil;
            isInstantHit = request.memory.InstantHit;
            isFastShootInterval = request.memory.FastShootInterval;
            isHitX = request.memory.HitX;
            isNoShake = request.memory.NoShake;
            isSmallCrosshair = request.memory.SmallCrosshair;
            isFastSwitchWeapon = request.memory.FastSwitchWeapon;
        }
        response.Success = false;
        response.PlayerCount = 0;
        response.VehicleCount = 0;
        response.ItemsCount = 0;
        response.GrenadeCount = 0;
        pvm(cLoc, &xyz, sizeof(xyz));
        if ((xyz.Z == 88.441124f || xyz.X == 0 || xyz.Z == 5278.43f || xyz.Z == 88.440918f) && !isBeta)
        {
            changed = 1;
            send((void*)&response, sizeof(response));
            continue;
        }
        pvm(fovPntr, &response.fov, 4);
        pvm(vMatrix, &vMat, sizeof(vMat));
        if (isBeta)
            printf("\nvMatChk: %0.1f | FOV: %0.2f | XYZ: %f %f %f", vMat._43, response.fov, xyz.X, xyz.Y, xyz.Z);
        if (changed == 1) {
            enAddrPntr = getEntityAddr(base);
            changed = 0;
        }
        Ulevel ulevel;
        pvm(enAddrPntr, &ulevel, sizeof(ulevel));
        if (ulevel.size < 1 || ulevel.size > 0x1000 || !isValid64(ulevel.addr)) {
            if (isBeta)
                puts("\nWrong Entity Address");
            changed = 1;
            if (!isBeta) {
                send((void*)&response, sizeof(response));
                continue;
            }
        }
        if (isBeta)
            printf("\nEntity Address: %lX | Size: %d", enAddrPntr, ulevel.size);
        memset(loaded, 0, 1000);
        float nearest = -1.0f;
        firing = 0;
        ads = 0;
        adsfiring = 0;
        adsfiring2 = 0;
        for (int i = 0; i < ulevel.size; i++) {
            uintptr_t pBase = getA(ulevel.addr + i * SIZE);
            if (!isValid64(pBase))
                continue;
            if (getI(pBase + SIZE) != 8)
                continue;
            int ids = getI(pBase + 8 + 2 * SIZE);
            int page = ids / 0x4000;
            int index = ids % 0x4000;
            if (page < 1 || page>30)
                continue;
            if (gname_buff[page] == 0)
                gname_buff[page] = getA(gname + page * SIZE);
            strcpy(name, getText(getA(gname_buff[page] + index * SIZE)));
            if (strlen(name) < 5)
                continue;
            if (strstr(name, "BP_PlayerPawn")||strstr(name, "BP_PlayerCharacter")||strstr(name, "PlanET_FakePlayer")||strstr(name, "BP_PlayerCharacter_PlanX_C")||strstr(name, "PlanX_PlayerCharacter_AI_C")) {
                sprintf(loadedpn, "%lx,", pBase);
                if (strstr(loaded, loadedpn))
                    continue;
                strcat(loaded, loadedpn);
				
                if (getI(pBase + Offsets64::bDead))
                    continue;
                pvm(pBase + Offsets64::Health, healthbuff, sizeof(healthbuff));
                if (healthbuff[1] > 200.0f || healthbuff[1] < 50.0f || healthbuff[0]>healthbuff[1] || healthbuff[0] < 0.0f)
                    continue;
				if (pBase != 0) {
                    ShootWeaponBase shootWeaponBase(pBase);
                    if (shootWeaponBase.isValid()) {
                        if (shootWeaponBase.isFiring()) {
                            if (isLessRecoil || isZeroRecoil) {
                                shootWeaponBase.setLessRecoil();
                                if (isZeroRecoil) {
                                    shootWeaponBase.setZeroRecoil();
                                }
                            }
                            if (isInstantHit) {
                                shootWeaponBase.setInstantHit();
                            }
                            if (isFastShootInterval) {
                                shootWeaponBase.setFastShootInterval();
                            }
                            if (isHitX) {
                                shootWeaponBase.setHitX();
                            }
                            if (isSmallCrosshair) {
                                shootWeaponBase.setSmallCrosshair();
                            }
                            if (isNoShake) {
                                shootWeaponBase.setNoShake();
                            }
                        }
                        if (isFastSwitchWeapon) {
                            Write(pBase + Offsets64::SwitchWeaponSpeedScale, "100", TYPE_FLOAT);
                        }
                    }
                }
                PlayerData* data = &response.Players[response.PlayerCount];
                data->Health = healthbuff[0] / healthbuff[1] * 100;
                data->TeamID = getI(pBase + Offsets64::TeamID);
                uintptr_t me = getI(pBase + Offsets64::Role);
                if (me == 258) {
                    if (isBeta)
                        printf("\nMe(%d): %lX ", data->TeamID, pBase);
                    if (aimbot) {
                        yawPitch = getA(pBase + Offsets64::Controller) + Offsets64::ControlRotation;
                        if (aimWhen == 3) {
                            pvm(pBase + Offsets64::bIsWeaponFiring, &adsfiring, 1);
                            pvm(pBase + Offsets64::bIsGunADS, &adsfiring2, 1);
                        }
                        if (aimWhen == 2) {
                            pvm(pBase + Offsets64::bIsWeaponFiring, &firing, 1);
                        }
                        if (aimWhen == 1) {
                            pvm(pBase + Offsets64::bIsGunADS, &ads, 1);
                        }
                    }
                    myteamID = data->TeamID;
                    continue;
                } / @gggg || @gggg
                else if (me != 257)
                    continue;
                if (data->TeamID == myteamID && myteamID <= 100)
                    continue;
                pvm(getA(pBase + Offsets64::ObserverCameraFPPMode) + Offsets64::ObserverLevel, &exyz, sizeof(exyz));
                data->HeadLocation = World2Screen(vMat, exyz);
                data->Distance = getDistance(xyz, exyz);
                if (data->Distance > 320.0f)
                    continue;
                pvm(pBase + Offsets64::bIsAI, &data->isBot, sizeof(data->isBot));
                strcpy(data->PlayerNameByte, "66:111:116:");
                if (data->HeadLocation.Z != 1.0f && data->HeadLocation.X < width + 100 && data->HeadLocation.X > -50) {
                    data->Bone = getPlayerBone(pBase, vMat);
                    data->Weapon = getPlayerWeapon(pBase);
                    if (!data->isBot) {
                        strcpy(data->PlayerNameByte, getNameByte(getA(pBase + Offsets64::PlayerName)));
                        if (strlen(data->PlayerNameByte) < 4)
                            continue;
                    }
                }
                if (response.PlayerCount >= maxplayerCount) {
                    continue;
                }
                if (data->HeadLocation.Z != 1.0f && (aimKnoced || data->Health > 0) && aimbot) {
                    float centerDist = sqrt((data->HeadLocation.X - width / 2) * (data->HeadLocation.X - width / 2) + (data->HeadLocation.Y - height / 2) * (data->HeadLocation.Y - height / 2));
                    if (centerDist < aimRadius) {
                        if (aimBy != 1)
                            centerDist = data->Distance;
                        if (nearest > centerDist || nearest < 0) {
                            nearest = centerDist;
                            if (aimFor == 1)
                                pointingAngle = getPointingAngle(xyz, exyz, data->Distance);
                            else if (aimFor == 2) {
                                uintptr_t boneAddr = getA(pBase + Offsets64::SkeletalMeshComponent);
                                struct D3DMatrix baseMatrix = getOMatrix(boneAddr + Offsets64::FixAttachInfoList);
                                boneAddr = getA(boneAddr + Offsets64::MinLOD);
                                struct D3DMatrix oMatrix = getOMatrix(boneAddr + 4 * 48);
                                pointingAngle = getPointingAngle(xyz, mat2Cord(oMatrix, baseMatrix), data->Distance);
                            }
                            else {
                                uintptr_t boneAddr = getA(pBase + Offsets64::SkeletalMeshComponent);
                                struct D3DMatrix baseMatrix = getOMatrix(boneAddr + Offsets64::FixAttachInfoList);
                                boneAddr = getA(boneAddr + Offsets64::MinLOD);
                                struct D3DMatrix oMatrix = getOMatrix(boneAddr + 2 * 48);
                                pointingAngle = getPointingAngle(xyz, mat2Cord(oMatrix, baseMatrix), data->Distance);
                            }
                        }
                    }
                }
                response.PlayerCount++;
                if (isBeta)
                    printf("\nE | %lX > TI:%d | H:%0.1f | XY: %0.1f %0.1f | %d", pBase, data->TeamID, data->Health, data->HeadLocation.X, data->HeadLocation.Y, data->isBot);
            }
            else if (strstr(name, "VH") || (strstr(name, "PickUp_") && !strstr(name, "BP")) || strstr(name, "Rony") || strstr(name, "Mirado") || strstr(name, "LadaNiva") || strstr(name, "AquaRail")) {
                if (!isPremium)
                    continue;
                VehicleData* data = &response.Vehicles[response.VehicleCount];
                pvm(getA(pBase + Offsets64::RootComponent) + Offsets64::Position, &exyz, sizeof(exyz));
                data->Location = World2Screen(vMat, exyz);
                if (data->Location.Z == 1.0f || data->Location.X > width + 200 || data->Location.X < -200)
                    continue;
                data->Distance = getDistance(xyz, exyz);
                strcpy(data->VehicleName, name);
                if (response.VehicleCount >= maxvehicleCount) {
                    continue;
                }
                response.VehicleCount++;
                if (isBeta)
                    printf("\nV | %lX > XY: %0.1f %0.1f | N: %s", pBase, data->Location.X, data->Location.Y, name);
            }
            else if (strstr(name, "Pickup_C") || strstr(name, "PickUp") || strstr(name, "BP_Ammo") || strstr(name, "BP_QK") || strstr(name, "Wrapper")) {
                if (!isPremium)
                    continue;
                ItemData* data = &response.Items[response.ItemsCount];
                pvm(getA(pBase + Offsets64::RootComponent) + Offsets64::Position, &exyz, sizeof(exyz));
                data->Location = World2Screen(vMat, exyz);
                if (data->Location.Z == 1.0f || data->Location.X > width + 100 || data->Location.X < -50)
                    continue;
                data->Distance = getDistance(xyz, exyz);
                if (data->Distance > 200.0f)
                    continue;
                strcpy(data->ItemName, name);
                if (response.ItemsCount >= maxitemsCount) {
                    continue;
                }
                response.ItemsCount++;
                if (isBeta)
                    printf("\nI | %lX > XY: %0.1f %0.1f | D:%0.1fm %s", pBase, data->Location.X, data->Location.Y, data->Distance, name);
            }
            else if (strstr(name, "BP_AirDropPlane_C") || strstr(name, "PlayerDeadInventoryBox_C") || strstr(name, "BP_AirDropBox_C")) {
                if (!isPremium)
                    continue;
                ItemData* data = &response.Items[response.ItemsCount];
                pvm(getA(pBase + Offsets64::RootComponent) + Offsets64::Position, &exyz, sizeof(exyz));
                data->Location = World2Screen(vMat, exyz);
                if (data->Location.Z == 1.0f || data->Location.X > width + 100 || data->Location.X < -50)
                    continue;
                data->Distance = getDistance(xyz, exyz);
                strcpy(data->ItemName, name);
                if (response.ItemsCount >= maxitemsCount) {
                    continue;
                }
                response.ItemsCount++;
                if (isBeta)
                    printf("\nSp | %lX > XY: %0.1f %0.1f | D:%0.1fm %s", pBase, data->Location.X, data->Location.Y, data->Distance, name);
            } / @gggg || @gggg
            else if (strstr(name, "BP_Grenade_Shoulei_C") || strstr(name, "BP_Grenade_Burn_C") || strstr(name, "BP_Grenade_Stun_C") || strstr(name, "BP_Grenade_Smoke_C")) {
                if (!isPremium)
                    continue;
                GrenadeData* data = &response.Grenade[response.GrenadeCount];
                pvm(getA(pBase + Offsets64::RootComponent) + Offsets64::Position, &exyz, sizeof(exyz));
                data->Location = World2Screen(vMat, exyz);
                data->Distance = getDistance(xyz, exyz);
                if (data->Distance > 150.0f)
                    continue;
                if (strstr(name, "Shoulei"))
                    data->type = 1;
                else if (strstr(name, "Burn"))
                    data->type = 2;
                else if (strstr(name, "Stun"))
                    data->type = 3;
                else if (strstr(name, "Smoke"))
                    data->type = 4;
                if (response.GrenadeCount >= maxgrenadeCount) {
                    continue;
                }
                response.GrenadeCount++;
                if (isBeta)
                    printf("\nGW | %lX > XY: %0.1f %0.1f | D:%0.1fm %d", pBase, data->Location.X, data->Location.Y, data->Distance, name);
            }
        }
        if (response.PlayerCount + response.ItemsCount + response.VehicleCount + response.GrenadeCount + response.GrenadeCount > 0)
            response.Success = true;
        if (isBeta) {
            printf("\nPlayers: %d | Vehicle: %d | Items: %d ", response.PlayerCount, response.VehicleCount, response.ItemsCount);
            break;
        }
        send((void*)&response, sizeof(response));
        if ((firing || ads || adsfiring || adsfiring2) && nearest > 0) {
            p_write(yawPitch, &pointingAngle, 8);
        }
    } / @gggg || @gggg
    if (isBeta)
        puts("\n\nScript Completed ");
