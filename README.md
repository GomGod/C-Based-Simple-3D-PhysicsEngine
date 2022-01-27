# Cpp-Based-Simple-3D-Physics-Engine

## 1. 현황
**2021-12-12** 
1) 프로젝트 시작
2) 물리엔진 기초 루틴 설계 (force 적용 -> 위치&가속도 재계산 -> 충돌감지 -> 처리(충돌 오브젝트, 겹치는 오브젝트 등) -> 결과 렌더링)
3) 3D 벡터 객체 연산 정의 및 구현

2021.12.12~ 12.19 : 기말고사 기간으로 프로젝트 일시정지

**2021-12-20**
1) Physics World 구현 (중력 적용)
2) IDE 변경 (CLion -> VS2022)

![image](https://user-images.githubusercontent.com/46223506/146803365-e6c72aa2-a3ef-41e3-b035-a20a40540614.png)

중력 시뮬레이션 결과

**2021-12-21**
1) 충돌 처리 설계   
폐기
   
**2021-12-22 ~ 2022-01-10**
물리 시뮬레이션 렌더링을 위한 OpenGL 학습

**2022-01-10**   
![image](https://user-images.githubusercontent.com/46223506/148786982-a306a8da-907e-45b1-8579-57095c0ed812.png)   
Object class의 데이터를 기반으로 구 렌더링

**2022-01-12**   
![image](https://user-images.githubusercontent.com/46223506/149158898-c2fc7524-8633-44b7-9c66-761d5a346f59.png)   
엔진 기초 설계   
1. 메인 스레드와 물리 스레드를 분리하여 물리 연산을 별도 스레드에서 수행   
2. 렌더링 시 Latest Object Buffer에서 데이터를 가져와 렌더링
3. 만약 Latest Object Buffer가 업데이트 중이라면 대기

**2022-01-14**   
![zx](https://user-images.githubusercontent.com/46223506/149777949-838147f0-79d1-4ea5-b2aa-3aea06e384be.gif)   
구 오브젝트, 평면 오브젝트 렌더링 및 중력 시뮬레이션   

**2022-01-15**  
충돌 처리 기초 설계   
1. AABB로 충돌 검사 대상 추리기
2. GJK 알고리즘을 활용하여 정밀 검사
3. EPA 알고리즘을 활용하여 충돌 정보 획득 후 오브젝트의 정보 수정   

**2022-01-19**   
충돌 감지 구현(충돌처리 기초 설계의 1-2 구현)   
![zx](https://user-images.githubusercontent.com/46223506/150134210-315397b0-b4d0-40ff-b7b5-f164e03eae64.gif)   

**2022-01-20**   
충돌 정보 획득 및 충돌 반응 테스트
![asdf](https://user-images.githubusercontent.com/46223506/150374570-2db3113c-70b0-41c0-8f9b-a68a55a10b27.gif)   
Position Solver, Impulse Solver 적용   

**~2022-01-26**   
**자체 Vector class 삭제, glm 제공 클래스로 수정**   
각 벡터, 행렬 연산의 완성도 개선 및 성능 개선   
**Euler -> Quaternion 회전으로 수정**   
성능 향상 및 짐벌락 문제 방지 목적   
**Object Rotation 대응**   
1. Rotate Matrix in GJK   
GJK를 수행할 때 오브젝트의 rotation 정보를 전달하여 변환행렬 생성 후 변환된 Collider에 대해 충돌 검사를 수행하도록 수정   

![asdf](https://user-images.githubusercontent.com/46223506/151125084-a2da8714-0c9a-47a9-bb15-8257100688c3.gif)   

### !Issue
**1. Tunneling - 속도가 빠른 물체가 재배치 되거나 delta time이 커지면 물체를 뚫고 지나가는 문제가 있음.**   
AABB를 활용한 CCD 구현을 통해 어느정도 해결 가능 할 것으로 보임   
**2. Object Rotation 대응 - 확대된 AABB 만으로는 모든 회전에 대한 대응이 어려움.(현재 해결중인 문제)**   
-대안 1. 회전 시 새로운 AABB 계산
새로운 AABB를 구하는 것은 비용이 너무 큼. 특히 돌림힘으로 인해 지속적으로 회전이 변하므로 매 프레임 AABB가 다시 계산된다고 볼 수 있음.   
-대안 2. 사전검사 제거   
성능저하가 매우 심함.   
-대안 3. Bounding Sphere   
사전검사의 의미가 사실상 없는 수준인 경우가 많아짐.   
-대안 4. 저비용 AABB 재계산   
AABB를 재계산하는 비용을 줄이는 방안을 구상 중.
(기록용)
![image](https://user-images.githubusercontent.com/46223506/151185863-98e0aff2-3068-463b-b444-482b9ec281ed.png)   
![image](https://user-images.githubusercontent.com/46223506/151313379-2c20bceb-a50a-4af9-baa7-21e6c6a101e0.png)   



C++ Based 3D Physics Engine
