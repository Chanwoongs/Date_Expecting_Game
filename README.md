# 데이트 결과 예측 게임
### Unreal Engine 5 & OpenAI API를 이용하여 C++로 제작한 게임입니다.
### 학부 연구생 과제로 1인 제작한 게임이며, 코드 공개는 허가 받았습니다.

<div>
  <img src="https://github.com/Chanwoongs/Date_Expecting_Game/assets/26241243/41fa0fd7-34bd-41ae-8bcc-3a330489643d" width="854" height="480"/>    
</div>

###  [YouTube 영상](https://www.youtube.com/watch?v=PemvBxw_p0o)

## 개요
> ### 날아다니는 문장(나의 상태, 기분, 행동)을 선택하여 오늘있는 데이트의 결과를 예측해보자!

## 개발 기간 (1인 개발)
> ### 2023.07 ~ 2023.08

## 기술
> ### [OpenAI API Chat(GPT 3.5) & DALLE 활용 (StartAI 함수)](https://github.com/Chanwoongs/Date_Expecting_Game/blob/main/Source/CGameMode.cpp)
> OpenAI API Library를 import하여 C++ 코드로 적용
> ### C++ Widget 제작
> BindWidget, BindWidgetAnim 키워드를 사용하여 클래스의 프로퍼티를 UMG와 연결하고 모든 기능을 코드로 구현
> ### 데이터 가공 및 지속적인 테스트
> 보다 정확한 결과가 나오기 위해 Parameter에 세부 내용을 추가 가공하고 테스트하여 Develop
> ### 결과 데이터 저장
> JSON 파일 형식으로 Input과 Output를 기존의 JSON 파일을 파싱하고 새로운 데이터를 추가하여 저장
<div>
  <img src="https://github.com/Chanwoongs/Date_Expecting_Game/assets/26241243/41cf7288-48cf-4c54-8660-4764f6fc8633" width="854" height="480"/>    
</div>

## 프로젝트를 통해 얻은 점
> ### 이전 UE4 프로젝트에선 Widget에 대해 많이 다루지 못하였는데, Widget이 게임 화면 전부를 이루는 이 게임을 통해 더 많이 다뤄볼 수 있어 좋았다.
> ### C++로 OpenAI API를 다루는 정보가 없어 라이브러리를 찾아 반복적인 시도 끝에 성공했다.
> ### CSV 형식의 데이터를 자주 다뤘는데 JSON을 다루며 이에 대해서 공부할 수 있었다.

## 아쉬운 점
> ### 부족한 데이터셋
> 혼자 만든 데이터셋이라 양이 적을 뿐더러, 퀄리티가 그다지 좋지 못해 결과가 아쉬웠다.
> ### OpenAI
> GPT 3.5 버전과 DALLE가 예상되는 결과를 많이 보여주지 못했다. 다른 AI 모델 적용하여 비교해보고싶다.
> ### 비동기 처리
> Chat에서 나온 Output을 DALLE의 Input으로 사용하여 순차적으로 작업이 진행되는데 굉장히 오래걸린다.
> 지금 같은 경우는 어쩔 수 없지만 입 출력값에 서로 연관성이 없다면 비동기 처리를 하여 빠른 결과를 얻으면 더 좋을 것 같다.
