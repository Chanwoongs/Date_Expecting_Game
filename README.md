# 데이트 결과 예측 게임
### Unreal Engine 5 & OpenAI API를 이용하여 C++로 제작한 게임입니다.

<div>
  <img src="https://github.com/Chanwoongs/Date_Expecting_Game/assets/26241243/41fa0fd7-34bd-41ae-8bcc-3a330489643d" width="854" height="480"/>    
</div>

###  [YouTube 영상](https://www.youtube.com/watch?v=PemvBxw_p0o)

## 개요
> ### 날아다니는 문장(나의 상태, 기분, 행동)을 선택하여 오늘있는 데이트의 결과를 예측해보자!

## 기술
> ### [OpenAI API Chat(GPT 3.5) & DALLE 활용 (StartAI 함수)](https://github.com/Chanwoongs/Date_Expecting_Game/blob/main/Source/CGameMode.cpp)
> OpenAI Library를 C++ 코드로 적용
> ### C++ Widget 제작
> BindWidget, BindWidgetAnim 키워드를 사용하여 클래스의 프로퍼티를 UMG와 연결하고 모든 기능을 코드로 구현
> ### 데이터 가공 및 지속적인 테스트
> 보다 정확한 결과가 나오기 위해 Parameter에 세부 내용을 추가 가공하고 테스트하여 Develope
> ### 결과 데이터 저장
> JSON 파일 형식으로 Input과 Output를 기존의 JSON 파일을 파싱하고 새로운 데이터를 추가하여 저장
<div>
  <img src="https://github.com/Chanwoongs/Date_Expecting_Game/assets/26241243/41cf7288-48cf-4c54-8660-4764f6fc8633" width="854" height="480"/>    
</div>

## 아쉬운 점
> ### 부족한 데이터셋
> 혼자 만든 데이터셋이라 양이 적을 뿐더러, 퀄리티가 그다지 좋지 못해 결과가 아쉬웠다.
> ### OpenAI
> GPT 3.5 버전과 DALLE가 예상되는 결과를 많이 보여주지 못했다. 다른 AI 모델 적용하여 비교해보고싶다.
