# Unreal5
My Unreal5 Programming Portfolio

![Front-ezgif com-video-to-gif-converter](https://github.com/showhohxc/Unreal5/assets/98040028/d5fe249a-34b8-493e-8584-cbf2cdf987a3)
![Left-ezgif com-video-to-gif-converter](https://github.com/showhohxc/Unreal5/assets/98040028/73c7bdcb-ba3a-4b47-9b0d-ea62db7449f4)
![Right-ezgif com-video-to-gif-converter](https://github.com/showhohxc/Unreal5/assets/98040028/f4311718-4e1a-4a32-8586-c8a830e23614)
![Behind-ezgif com-video-to-gif-converter](https://github.com/showhohxc/Unreal5/assets/98040028/530c97e2-367f-48ce-b9ae-33227a4b2c32)


![image](https://github.com/showhohxc/Unreal5/assets/98040028/a0a9db82-8aa7-4b3c-b347-da5ef0592d46)

Enemy의 전방벡터

![image](https://github.com/showhohxc/Unreal5/assets/98040028/9202e8ab-1379-4c63-a9f2-d6a79a6eb5a1)

Collision Box(Red Point)의 HitPoint 의 Location을 토대로 Enemy가 어느 방향으로 가야 하는지 알수있다. 
HitPoint Location을 가르키는 벡터(Green Line)를 만들 수 있다. 여기서 Hit 벡터는 Hit = HitPoint - EnemyLocation 으로 벡터값을 구할수 있다.

![image](https://github.com/showhohxc/Unreal5/assets/98040028/80f43967-66ec-4a57-9a32-30e0b4d80f33)

두 벡터( Enemy Forward Vector / Hit Vector ) 사이의 각도를 알면 Enemy가 어느 방향으로 동작하는지 알 수 있다.
> ● Front : -45 < 0 < 45 </br>
> ● Left : -135 < 0 < -45 </br>
> ● Right : 45 < 0 < 135 </br>
> ● Back : -135 < 0 < 135 </br>


## 두 Vector 사이의 계산


