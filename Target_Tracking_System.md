## Target_Tracking_System<br>

### Skill System을 활성한 이후 스킬들이 논타겟팅 / 타켓팅 시스템을 구분하고 타겟팅 시스템을 구현해야겠다는 생각하에 개발 후 구현

#### before
![Before-ezgif com-video-to-gif-converter](https://github.com/showhohxc/Unreal5/assets/98040028/1229af75-3bb6-472e-b25b-2f176366988d)<br>
**기본 Skill 사용시의 캐릭터**<br>

**타켓팅 시스템에 필요한 로직**<br>
**1. "나" 와 "Target" 간의 Rotation**<br>
**2. "나" 와 "Target" 간의 Location**<br>

#### 1. Target Rotation<br>
![After_1-ezgif com-video-to-gif-converter](https://github.com/showhohxc/Unreal5/assets/98040028/2594dd33-eeb0-4066-9c07-3d331616deeb)<br>

![image](https://github.com/showhohxc/Unreal5/assets/98040028/a4c2d7ee-d009-4280-92b6-2080cf0295f1)<br>
**FindLookAtRotation (타겟 위치를 가르키는 플레이어 시작위치의 개체에 대한 Z축 회전값을 찾는다)를 통하여 플레이어가 타겟에 대한 회전값을 찾는다.**

#### 2. Target Location<br>
![After_2-ezgif com-video-to-gif-converter](https://github.com/showhohxc/Unreal5/assets/98040028/62b53020-f625-44c1-9c79-edd5e82c266e)<br>

![image](https://github.com/showhohxc/Unreal5/assets/98040028/49dfb13c-49be-48ab-ad09-c855f24984bc)<br>
**Normalize 함수를 이용하여 플레이어 Location과 타겟 Location의 해당 벡터의 방향을 유지한채 크기가 1인 단위 벡터로 만든다.** <br>
**(플레이어의 Location + Normalize Vector) 결합시 Player에서 Target까지의 Location Vector를 구할수 있다.**
