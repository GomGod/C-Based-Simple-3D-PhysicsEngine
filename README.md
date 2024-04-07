# Cpp-Based-Simple-3D-Physics-Engine

## 1. 현황  
22.02~ : 중단 (개인 사정)
재개 시 처음부터 진행 예정

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
Rotate Matrix in GJK   
GJK를 수행할 때 오브젝트의 rotation 정보를 전달하여 변환행렬 생성 후 변환된 Collider에 대해 충돌 검사를 수행하도록 수정   

**2022-01-27**   
AABB 회전 대응   
단순히 확대된 AABB는 회전에 대해 충분히 처리할 수 없고 Bounding Sphere보다 나은 점이 없기 대문에 적절한 비용으로 AABB를 재계산하는 방법이 필요했음.   
   
AABB정보를 통해 얻을 수 있는 팔면체 vertex는 항상 오브젝트를 포함한다는 점에 주목함.   
AABB를 회전변환한 BOX의 AABB는 변환된 오브젝트의 AABB를 항상 포함함.   
물론 그렇게 구해진 AABB는 회전한 오브젝트에 대해 비교적 부정확한 AABB지만,   
많은 vertex를 순회하면서 정확한 AABB를 구하는 비용에 비하면 훨씬 싸게 먹힌다는 장점이 있음.   
![collisiontest](https://user-images.githubusercontent.com/46223506/151356605-649cefc9-6fa2-4873-913c-8b5c5e1489c7.gif)   

C++ Based 3D Physics Engine
