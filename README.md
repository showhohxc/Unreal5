# Unreal5
My Unreal5 Programming Portfolio


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


